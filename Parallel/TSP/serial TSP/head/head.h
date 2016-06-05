#ifndef _HEAD_H
#define _HEAD_H

#include <sys/time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define GET_TIME(now) { \
   struct timeval t; \
   gettimeofday(&t, NULL); \
   now = t.tv_sec + t.tv_usec/1000000.0; \
}

#define FALSE 0
#define TRUE 1
#define DIS_INFINITY 999999
#define DIS_SELF 0
#define NO_CITY -1
#define SOURCE 0

// patrial tour
typedef struct tour{
   int* cities; 
   int count;     
   int cost;   
} tour, *tour_p; 

typedef struct stack{
   tour_p* list;
   int list_sz;
}  stack, *stack_p;

// Global Variables
int city_num;
int* city_graph;
//int source;
tour_p best_tour;
stack_p avail_stack;

// Function Declaration
void Read_City_Graph(FILE* matrix);
void Print_City_Graph();
void Iterative_DFS(tour_p tour);
void Print_Tour(tour_p tour, char* str);
int Best_Tour(tour_p tour);
void Update_Best_Tour(tour_p tour);
void Clone_Tour(tour_p tour1, tour_p tour2);
void Add_City(tour_p tour, int city);
void Del_Last_City(tour_p tour);
int Work(tour_p tour, int city);
int Visited(tour_p tour, int city);
void Init_Tour(tour_p tour, int cost);
tour_p Alloc_Tour(void);
void Free_Tour(tour_p tour);

#endif