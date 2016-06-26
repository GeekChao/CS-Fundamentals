#include <stdio.h>
#include "csapp.h"
#include "cache.h"

/* You won't lose style points for including these long lines in your code */
static const char *user_agent_hdr = "User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:10.0.3) Gecko/20120305 Firefox/10.0.3\r\n";
static const char *connection_hdr = "Connection: close\r\n";
static const char *proxy_connection_hdr = "Proxy-Connection: close\r\n";

/* user defined functions */
void doit(int fd);
void clienterror(int fd, char *cause, char *errnum, 
                 char *shortmsg, char *longmsg);
int is_valid(char *buf);
int read_requesthdrs(rio_t *rp, char *reqs);
void parse_uri(char *url, char *hostname, char *port, char *uri);
void forward_response(rio_t *rp, int client_fd, char *url);
void *thread(void *vargp);

int main(int argc, char **argv)
{
    int listenfd;
    int *connfd;
    char hostname[MAXLINE], port[MAXLINE];
    struct sockaddr_in clientaddr;
    socklen_t clientlen = sizeof(clientaddr);
    pthread_t tid;

    /* check command line args */
    if (argc != 2) {
        fprintf(stderr, "usage: %s <port>\n", argv[0]);
        exit(1);
    }

    /* ignore SIGPIPE signal */
    signal(SIGPIPE, SIG_IGN);

    /* initialize the cache */
    init_cache();

    listenfd = Open_listenfd(argv[1]);
    while(1) {
        /* listen for incoming connections */
        connfd = malloc(sizeof(int));
        *connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);
        Getnameinfo((SA *)&clientaddr, clientlen, hostname, MAXLINE, 
                     port, MAXLINE, 0);
        printf("Accepted connection from (%s, %s)\n", hostname, port);
        /* create a seperate thread to handle this request */
        Pthread_create(&tid, NULL, thread, connfd);
    }
    return 0;
}

/*
 * thread to hanlde one HTTP transaction
 * all inner functions used shoul be thread-safe
 */
void *thread(void *vargp) {
    int client_fd;

    /* automatic reap and avoid memory leak */
    Pthread_detach(Pthread_self());
    client_fd = *((int *)vargp);
    free(vargp);
    /* service the client */
    doit(client_fd);
    /* close connection */
    Close(client_fd);
    return NULL;
}

/*
 * handles one HTTP request/response transaction
 * 1. read and parese client request
 * 2. if valid http request, establish connection to requested server, 
 * request the object on behalf of the client, and forward it to the client
 * 3. if invalid request, send error messgages to the connected client 
 */
void doit(int fd) {
    char buf[MAXLINE], method[MAXLINE], url[MAXLINE], uri[MAXLINE], 
         version[MAXLINE], hostname[MAXLINE], server_port[MAXLINE];
    char reqs[MAXBUF];
    rio_t rio_client, rio_proxy;
    int proxy_fd; /* descriptor to communicate with server */

    /* read first line of HTTP request */
    Rio_readinitb(&rio_client, fd);
    if (rio_readlineb(&rio_client, buf, MAXLINE) < 0) {
        printf("read request line failed\n");
        return;
    }

    /* see if the request is valid */
    printf("read request line: %s", buf);
    if (is_valid(buf)) {
        sscanf(buf, "%s %s %s", method, url, version);
        if (strcasecmp(method, "GET")) {
            clienterror(fd, method, "501", "Not Implemented", 
                        "Proxy does not implement this method");
            return;
        }
    }
    else {
        clienterror(fd, "", "400", "Bad Requset", 
                    "Proxy does not understand this request");
        return;
    }

    /* check cache to see if request cached */
    if (cache_out(url, fd)) {
        printf("Serve with cached content\n");
        return;
    }

    /* parse the url, and retrieve hostname, port number, and uri for server */
    parse_uri(url, hostname, server_port, uri);
    // printf("uri: %s\n", uri);
    // printf("hostname: %s\n", hostname);
    // printf("server port: %s\n", server_port);

    /* build the request line */
    sprintf(reqs, "GET %s HTTP/1.0\r\n", uri);

    /* read subsequent request headers and build the requst headers */
    if (!read_requesthdrs(&rio_client, reqs))
        sprintf(reqs, "%sHost: %s\r\n", reqs, hostname);
    sprintf(reqs, "%s%s", reqs, user_agent_hdr);
    sprintf(reqs, "%s%s", reqs, connection_hdr);
    sprintf(reqs, "%s%s", reqs, proxy_connection_hdr);
    sprintf(reqs, "%s\r\n", reqs); /* empty line to end headers */

    /* open a client-end socket and send request to server */
    // printf("hostname: %s\n", hostname);
    // printf("port number: %s\n", server_port);
    proxy_fd = open_clientfd(hostname, server_port);
    if (proxy_fd < 0) {
        printf("host access not allowed\n");
        return;
    }
    Rio_readinitb(&rio_proxy, proxy_fd);
    if (rio_writen(proxy_fd, reqs, strlen(reqs)) < 0) {
        printf("write request failed\n");
        return;
    }
    printf("writing the request to server:\n");
    printf("%s", reqs);
    
    /* forward server response to the client and close when finish */
    forward_response(&rio_proxy, fd, url);
    Close(proxy_fd);

    return;
}

/*
 * determines whether if is a valid http request
 * valid form: <method> http://<hostname>/ <version
 * 1 for valid, 0 for not valid
 */
int is_valid(char *buf) {
    char *cp = buf;

    if (*cp != ' ') {
        if ((cp = strchr(buf, ' '))) {
            if (cp == strstr(cp, " http://")) {
                cp += 8;
                if (*cp != ' ') {
                    if ((*cp != '/') && (strchr(cp, '/')))
                        return 1;
                }
            }
        }
    }
    return 0;
}

/*
 * parses the uri from client, retrieve hostname, port number (if provideded),
 * and update the uri to be sent to the server as request
 */
void parse_uri(char *url, char *hostname, char *port, char *uri) {
    char *cp_hd, *cp_ft;
    char temp[MAXLINE];

    cp_hd = strstr(url, "http://");
    cp_hd = cp_hd + strlen("http://");
    cp_ft = strchr(cp_hd, '/');
    *cp_ft = '\0';
    memcpy(temp, cp_hd, strlen(cp_hd)+1);
    /* port number provided */
    if (strchr(temp, ':')) {
        cp_ft = strchr(temp, ':');
        *cp_ft = '\0';
        memcpy(hostname, temp, strlen(temp)+1);
        memcpy(port, cp_ft+1, strlen(cp_ft+1)+1);
    }
    /* port numbe not provided */
    else {
        memcpy(hostname, temp, strlen(temp)+1);
        memcpy(port, "80", strlen("80")+1);
    }
    cp_hd = strchr(cp_hd, '\0');
    *cp_hd = '/';
    memcpy(uri, cp_hd, strlen(cp_hd)+1);   
}

/*
 * reads request headers from client request
 * add the Host header if client provides and return 1
 * otherwise return 0
 * add other adders client provides except those specified
 * in the document
 */
int read_requesthdrs(rio_t *rp, char *reqs) {
    char buf[MAXBUF], hdr_name[MAXLINE], hdr_data[MAXLINE];
    int ret = 0;

    while (1) {
        Rio_readlineb(rp, buf, MAXLINE);
        if (!strcmp(buf, "\r\n"))
            break;
        // printf("read hdr: %s\n", buf);
        sscanf(buf, "%s %s", hdr_name, hdr_data);
        if (!strcmp(hdr_name, "Host:")) {   
            sprintf(reqs, "%s%s", reqs, buf);
            ret = 1;
        }
        else if (strcmp(hdr_name, "User-Agent:") && 
                 strcmp(hdr_name, "Connection:") &&
                 strcmp(hdr_name, "Proxy-Connection:")) {
            sprintf(reqs, "%s%s", reqs, buf);
        }
    }
    // printf("leave read_requesthdrs\n");
    return ret;

}

/*
 * forwards server's response to client
 */
void forward_response(rio_t *rp, int client_fd, char *url) {
    char buf[MAXBUF], hdr_name[MAXLINE], hdr_data[MAXLINE];
    int bytes;
    // int content_length = 0;
    // char content_type[MAXLINE];
    web_buf w_buf;
    init_web_buf(&w_buf);

    /* read response line */
    bytes = rio_readlineb(rp, buf, MAXLINE);
    if (bytes <= 0) {
        printf("read response line failed or EOF encountered\n");
        return;
    }
    bytes = rio_writen(client_fd, buf, bytes);
    if (bytes <= 0) {
        printf("forward response line failed\n");
        return;
    }
    // printf("read response line: %s", buf);
    /* buffer response line */
    buffer(&w_buf, buf, bytes);

    /* forward response headers */
    while (1) {
        bytes = rio_readlineb(rp, buf, MAXLINE);
        if (bytes <= 0) {
            printf("read response header failed or EOF encountered\n");
            return;
        }
        // printf("read response header: %s", buf);
        bytes = rio_writen(client_fd, buf, strlen(buf));
        if (bytes <= 0) {
            printf("forward response header failed\n");
            return;
        }
        /* buffer response headers */
        buffer(&w_buf, buf, bytes);

        /* empty line, response hdrs finish */
        if (!strcmp(buf, "\r\n")) 
            break;
        /* not empty line, extract content type and content length */
        sscanf(buf, "%s %s", hdr_name, hdr_data);
        // printf("hdr_name: %s\n", hdr_name);
        // printf("hdr_data: %s\n", hdr_data);
        // if (!strcmp(hdr_name, "Content-Type:")
        //     || !strcmp(hdr_name, "Content-type:"))
        //     strcpy(content_type, hdr_data);
        // if (!strcmp(hdr_name, "Content-Length:")
        //     || !strcmp(hdr_name, "Content-length:"))
        //     content_length = atoi(hdr_data);
    }
    // printf("content type: %s\n", content_type);
    // printf("content length: %d\n", content_length);

    /* forward response body */
    // printf("forward response body begin...\n");
    while ((bytes = rio_readnb(rp, buf, MAXBUF))) {
        if (bytes < 0) {
            printf("read response body failed\n");
            return;
        } 
        bytes = rio_writen(client_fd, buf, bytes);
        if (bytes < 0) {
            printf("write response body failed\n");
            return;
        }
        printf("write %d bytes to client\n", bytes);
        /* buffer response body */
        buffer(&w_buf, buf, bytes);
    }  

    // printf("buffer over size? %d\n", w_buf.over_cacheable);
    // printf("buffered content:\n%s", w_buf.buf);

    if (!w_buf.over_cacheable)
        cache_in(url, w_buf.buf, w_buf.buffered_bytes);
}

/*
 * returns an error message to the client when request is invalid
 */
void clienterror(int fd, char *cause, char *errnum, 
                 char *shortmsg, char *longmsg) {
    char buf[MAXLINE], body[MAXBUF];

    /* build HTTP response body */
    sprintf(body, "<html><title>Proxy Error</title>");
    sprintf(body, "%s<body bgcolor=""ffffff"">\r\n", body);
    sprintf(body, "%s%s: %s\r\n", body, errnum, shortmsg);
    sprintf(body, "%s<p>%s: %s\r\n", body, longmsg, cause);
    sprintf(body, "%s<hr><em>The Proxy</em>\r\n", body);

    /* print the HTTP response */
    sprintf(buf,  "HTTP/1.0 %s %s\r\n", errnum, shortmsg);
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "Content-type: text/html\r\n");
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "Content-length: %d\r\n\r\n", (int)strlen(body));
    Rio_writen(fd, buf, strlen(buf));
    Rio_writen(fd, body, strlen(body));  
} 