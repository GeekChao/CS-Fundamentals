#ifndef _HEAD_H
#define _HEAD_H

#include <sys/time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

#define GET_TIME(now) { \
   struct timeval t; \
   gettimeofday(&t, NULL); \
   now = t.tv_sec + t.tv_usec/1000000.0; \
}

#define FALSE 0
#define TRUE 1
#define DIS_INFINITY 99999
#define DIS_SELF 0
#define NO_CITY -1
#define MAX_STR 1024
#define RESULTS -1
#define SOURCE 0

// patrial tour
typedef struct tour{
   int* cities; 
   int count;     
   int cost;   
} tour, *tour_p; 

// stack
typedef struct stack{
   tour_p* list;
   int list_sz;
   int alloc_sz;
}  stack, *stack_p;

// queue
typedef struct queue{
   tour_p* list;
   int head;
   int tail;
   int alloc_sz;
   int full;
}  queue_struct, *queue_p;

// barrier
typedef struct barrier{ 
   pthread_mutex_t mutex;
   pthread_cond_t cond;
   int count;  // current number of threads
   int max;    // maximal number of threads needed
}  barrier_struct, *barrier_p;

// Global Variables
int city_num;
int* city_graph;
int thread_num;
tour_p best_tour;
pthread_mutex_t best_tour_mutex;
queue_p queue;
int queue_sz;
int init_tour_num;
barrier_p barrier;

// Function Declaration
void Read_City_Graph(FILE* matrix);
void Print_City_Graph();

void* Par_Tree_DFS(void* thread);
void Divide_Tree(long rank, stack_p stack);
void Set_Thread_Tours(long rank, int* begin, int* end);

void Set_Init_Queue(void);

void Print_Tour(long rank, tour_p tour, char* str);
int Best_Tour(tour_p tour);
void Update_Best_Tour(tour_p tour);
void Clone_Tour(tour_p tour1, tour_p tour2);
void Add_City(tour_p tour, int city);
void Del_Last_City(tour_p tour);
int Work(tour_p tour, int city);
int Visited(tour_p tour, int city);
void Init_Tour(tour_p tour, int cost);
tour_p Alloc_Tour(stack_p avail);
void Free_Tour(tour_p tour, stack_p avail);

// Barrier
barrier_p Init_Barrier(int count);
void Destory_Barrier(barrier_p bar);
void Barrier(barrier_p bar);

#endif