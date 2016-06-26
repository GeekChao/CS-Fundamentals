/*
 * cache.c a web cache used by web proxy to cache web objects as needed
 *
 *
 * maximum cache size: 100 MiB
 * maximum object size: 100 KiB
 *
 * cache is maintained in a double-linked list of cache nodes
 * a bag of cache node pointers is maintained for queuing up
 * any cache hits happened in reader threads.
 *
 * cache node:
 * url, id
 * buf, holds the actual web object
 * content_length, web object size
 * pre/succ, pointers to previous/next cache node
 *
 * cache list:
 * free_bytes, remaining available bytes
 * head, list front
 * tail, list rear
 *
 * cache bag:
 * bag, array of cache node pointers
 * bag_index, array index to the bag
 * bag_size, array size
 *
 * cache out operation:
 * multiple threads can access reader section to determine whether
 * a cache hit. If so, cache node according to hit is collected in
 * cache bag. Only one thread can access writer section to modify
 * the cache list. All threads in reader section finish before
 * any writer section begin.
 *
 */

#include "csapp.h"
#include "cache.h"

#define CACHE_HITS 32

/* global variables */
cache_list cache;
cache_bag cache_hits;
int read_cnt;

/* mutexes */
sem_t w; /* modify list lock */
sem_t u; /* access to hits bag */
sem_t read_mutex; /* reader counter lock */


/*
 * initialize cache hits
 */
void init_cache_hits() {
	cache_hits.bag = malloc(CACHE_HITS * sizeof(cache_node *));
	cache_hits.bag_index = 0;
	cache_hits.bag_size = CACHE_HITS;
}

/*
 * free cache hits
 */
void free_cache_hits() {
	free(cache_hits.bag);
}

/*
 * check if a cache_hits is empty
 */
int is_cache_hits_empty() {
	return !cache_hits.bag_index;
}

/*
 * put a cache_node pointer in cache_hits
 */
void cache_hits_put(cache_node *n) {
	cache_hits.bag[cache_hits.bag_index] = n;
	cache_hits.bag_index++;
	if (cache_hits.bag_index >= cache_hits.bag_size)
		resize_cache_hits();
}

/*
 * pop a cache_node pointer in cache_hits
 */
cache_node *cache_hits_get() {
	if (!is_cache_hits_empty()) {
		int i = --cache_hits.bag_index;
		return cache_hits.bag[i];
	}
	return NULL;
}

/*
 * double the size of cache_hits
 */
void resize_cache_hits() {
	cache_hits.bag = realloc(cache_hits.bag,
		                     2*cache_hits.bag_size*sizeof(cache_node *));
	cache_hits.bag_size = 2*cache_hits.bag_size;
}

/*
 * initialize cache list
 */
void init_cache_list() {
	cache.free_bytes = MAX_CACHE_SIZE;
	cache.head = NULL;
	cache.tail = NULL;
}

/*
 * deinit cache list
 */
void free_cache_list() {
	cache_node *p = cache.head;
	cache_node *p_succ;
	while (p) {
		p_succ = p->succ;
		free(p->buf);
		free(p);
		p = p_succ;
	}
}

/*
 * initialize a working cache and its parallel
 */
void init_cache() {
	/* init global variables */
	init_cache_list();
	init_cache_hits();
	read_cnt = 0;
	/* init semaphores */
	Sem_init(&w, 0, 1);
	Sem_init(&u, 0, 1);
	Sem_init(&read_mutex, 0, 1);
}

/*
 * clear all cache related data
 */
void deinit_cache() {
	free_cache_list();
	free_cache_hits();
}

/*
 * delete a node from the cache list
 */
void delete_node(cache_node *node) {
	if (node->pre)
		node->pre->succ = node->succ;
	if (node->succ)
		node->succ->pre = node->pre;
	else
		cache.tail = node->pre;
}

/*
 * insert a node in front of the cache list
 */
void insert_node(cache_node *node) {
	if (cache.head) {
		node->pre = NULL;
		node->succ = cache.head;
		cache.head->pre = node;
		cache.head = node;
	}
	else {
		node->pre = NULL;
		node->succ = NULL;
		cache.head = node;
		cache.tail = node;
	}
}

/*
 * LRU policy: least used node is put in the tail of
 * cache list
 */
void evict_cache(int bytes) {
	cache_node *lru_node;
	lru_node = cache.tail;
	while (cache.free_bytes < bytes) {
		delete_node(lru_node);
		cache.free_bytes += lru_node->content_length;
		cache.tail = lru_node->pre;
		free(lru_node->buf);
		free(lru_node);
		lru_node = cache.tail;
	}
}

/*
 * cache in the web object passed by proxy buffer
 * 1. update list ordering for queued cache hits in readers
 * 2. if not enough space, evict least recently used nodes
 * 3. create new node to hold the buffer data
 * 4. insert the new node to cache list
 * 5. update available bytes in the cache
 */
void cache_in(char *url, char *buf, int bytes) {
	cache_node *curr_n;

	P(&w);

	/* update the list ordering for any queued cache hits */
	while((curr_n = cache_hits_get())) {
		if (curr_n != cache.head) {
			delete_node(curr_n);
			insert_node(curr_n);
		}
	}

	/* evict nodes till enough available space */
	evict_cache(bytes);

	/* create a new node to hold the cached in data */
	cache_node *new_node = malloc(sizeof(cache_node));
	new_node->buf = malloc(bytes);
	strcpy(new_node->url, url);
	memcpy(new_node->buf, buf, bytes);
	new_node->content_length = bytes;

	/* insert the new node to front of working list */
	insert_node(new_node);
	cache.free_bytes -= bytes;

	V(&w);}

/*
 * traverse the list to find if a client request has been cached
 * return pointer to the node if cached, other wise return NULL
 */
cache_node *find_cached(char *url) {
	cache_node *curr_node = cache.head;
	for (; curr_node; curr_node = curr_node->succ) {
		if (!strcmp(url, curr_node->url))
			return curr_node;
	}
	return NULL;
}

/*
 * cache out the web object to client request
 * if url matched in cache, write cached content to client,
 * and update hit bag.
 * return 1, if request cached
 * return 0, if not cached
 */
int cache_out(char *url, int client_fd) {
	cache_node *cached_node;
	int cached = 0;

	P(&read_mutex);
	read_cnt++;
	if (read_cnt == 1) /* first in */
		P(&w);
	V(&read_mutex);

	/* read the cache list */
	cached_node = find_cached(url);
	if (cached_node) {
		P(&u);
		cached = 1;
		rio_writen(client_fd, cached_node->buf, cached_node->content_length);
		cache_hits_put(cached_node);
		V(&u);
	}

	P(&read_mutex);
	read_cnt--;
	if (read_cnt == 0) /* last out */
		V(&w);
	V(&read_mutex);

	return cached;
}

/*
 * helper buffer in proxy
 */
void buffer(web_buf *w_buf, char *buf, int bytes) {
	if (!w_buf->over_cacheable) {
		if ((w_buf->buffered_bytes+bytes) < MAX_OBJECT_SIZE) {
			memcpy(w_buf->buf_index, buf, bytes);
			w_buf->buf_index += bytes;
			w_buf->buffered_bytes += bytes;
		}
		else
			w_buf->over_cacheable = 1;
	}
}

/*
 * helper buffer init
 */
void init_web_buf(web_buf *w_buf) {
	w_buf->over_cacheable = 0;
	w_buf->buf_index = w_buf->buf;
	w_buf->buffered_bytes = 0;
}
