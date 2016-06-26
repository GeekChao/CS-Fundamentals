/*
 * cache.h - prototypes and definitions for cache.c
 */

#include "csapp.h"

/* Recommended max cache and object sizes */
#define MAX_CACHE_SIZE 1049000
#define MAX_OBJECT_SIZE 102400

/* cache block unit */
typedef struct cache_node {
	char url[MAXLINE];
	char *buf; 
	int content_length;
	struct cache_node *pre;
	struct cache_node *succ; 
} cache_node;
 
/* list of nodes */
typedef struct cache_list {
	int free_bytes;
	cache_node *head;
	cache_node *tail;
} cache_list;

/* bag of nodes */
typedef struct cache_bag {
	cache_node **bag;
	int bag_index;
	int bag_size; 
} cache_bag;

/* temperary buffer for web objects */
typedef struct web_buf {
	int over_cacheable;
	char buf[MAX_OBJECT_SIZE];
	char *buf_index;
	int buffered_bytes;
} web_buf;

/* function prototypes */
/* cache node bag prototypes  */
void init_cache_hits();
void free_cache_hits();
int is_cache_hits_empty();
void cache_hits_put(cache_node *n);
cache_node *cache_hits_get();
void resize_cache_hits();
/* cache protoptypes */
void init_cache_list();
void init_cache();
void deinit_cache();
void delete_node(cache_node *node);
void insert_node(cache_node *node);
void evict_cache(int bytes);
void cache_in(char *url, char *buf, int bytes);
cache_node *find_cached(char *url);
int cache_out(char *url, int client_fd);
/* web_buf prototypes */
void init_web_buf(web_buf *w_buf);
void buffer(web_buf *w_buf, char *buf, int bytes);