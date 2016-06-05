#include "../head/head.h"
#include "../head/stack.h"
#include "../head/queue.h"

int main(int argc, char*argv[]){
    double begin, end;
    FILE* matrix;
    pthread_t* thread_arr;
    long i;
    
    thread_num = strtol(argv[1], NULL, 10);;
    
    // Read the total number of cities and matrix from disk
    matrix = fopen(argv[2], "r");
    Read_City_Graph(matrix);
    fclose(matrix);
    Print_City_Graph();
    
    // Allocate and initialize threads
    thread_arr = malloc(thread_num * sizeof(pthread_t));
    barrier = Init_Barrier(thread_num);
    pthread_mutex_init(&best_tour_mutex, NULL);

    // Allocate and initialize stacks and tours
    best_tour = Alloc_Tour(NULL);
    Init_Tour(best_tour, DIS_INFINITY);
    // Create threads and count the finished timeval
    GET_TIME(begin);
    
    for (i = 0; i < thread_num; i++)
      pthread_create(&thread_arr[i], NULL, Par_Tree_DFS, (void*) i);

    for (i = 0; i < thread_num; i++)
      pthread_join(thread_arr[i], NULL);    
    
    GET_TIME(end);
    
    // Display Results
    Print_Tour(RESULTS, best_tour, "Best Tour");
    printf("Cost: %d\n", best_tour->cost);
    printf("Finished time: %e seconds\n", end - begin);

    // Release Resources
    free(best_tour->cities);
    free(best_tour);
    free(thread_arr);
    free(city_graph);
    Destory_Barrier(barrier);
    pthread_mutex_destroy(&best_tour_mutex);
    return 0;
}

void Read_City_Graph(FILE* matrix){
   int i, j, n;

   fscanf(matrix, "%d", &city_num);
   n = city_num;
   city_graph = malloc(n * n * sizeof(int));

   for (i = 0; i < n; i++)
      for (j = 0; j < n; j++)
         fscanf(matrix, "%d", &city_graph[i * n + j]);
}


void Print_City_Graph() {
   int i, j;

   printf("Dimension: %d\n", city_num);
   printf("Matrix:\n");
   for (i = 0; i < city_num; i++) {
      for (j = 0; j < city_num; j++)
         printf("%2d ", city_graph[i * city_num + j]);
      printf("\n");
   }
   printf("\n");
} 

void Init_Tour(tour_p tour, int cost){

   tour->cities[SOURCE] = 0;
   for (int i = 1; i <= city_num; i++) {
      tour->cities[i] = NO_CITY;
   }
   
   tour->cost = cost;
   tour->count = 1;
}

tour_p Alloc_Tour(stack_p avail){
   tour_p tmp;

   if (avail == NULL || Empty_Stack(avail)) {
       
      tmp = malloc(sizeof(tour));
      tmp->cities = malloc((city_num + 1) * sizeof(int));
      return tmp;
   } else {
      return Pop_Stack(avail);
   }
}

void Free_Tour(tour_p tour, stack_p avail){
    if(avail != NULL){
       Push_Stack(avail, tour);
    }else{
       free(tour->cities);
       free(tour);
    }
}


void Print_Tour(long rank, tour_p tour, char* str){
    
   char string[MAX_STR];

   if (rank >= 0)
      sprintf(string, "Thread %ld; %s: ", rank, str);
   else
      sprintf(string, "%s: ", str);
      
   for (int i = 0; i < tour->count; i++)
      sprintf(string + strlen(string), "%d ", tour->cities[i]);
      
   printf("%s\n\n", string);
 
}

void* Par_Tree_DFS(void* thread){
   long rank = (long) thread;
   int nbr;
   stack_p stack;
   stack_p avail_stack;
   tour_p curr_tour;

   stack = Init_Stack();
   avail_stack = Init_Stack();
   Divide_Tree(rank, stack);

   while (!Empty_Stack(stack)) {
      curr_tour = Pop_Stack(stack);
      Print_Tour(rank, curr_tour, "Popped the top tour in the stack");
      printf("\n");
      if (curr_tour->count == city_num) {
         if (Best_Tour(curr_tour))
            Update_Best_Tour(curr_tour);
      } else {
         for (nbr = city_num - 1; nbr >= 1; nbr--) 
            if (Work(curr_tour, nbr)) {
               Add_City(curr_tour, nbr);
               Push_Copy(stack, curr_tour, avail_stack);
               Del_Last_City(curr_tour);
            }
      }
      Free_Tour(curr_tour, avail_stack);
   }
   
   Free_Stack(stack);    
   Free_Stack(avail_stack);
   Barrier(barrier);
   
   if(rank == 0) Free_Queue(queue);
   
   return NULL;
}

void Divide_Tree(long rank, stack_p stack){
   int begin, end, i;

   if (rank == 0) {
       queue_sz = city_num;
       Set_Init_Queue();//BFS
   }
   
   Barrier(barrier); // synchronization
   
   Set_Thread_Tours(rank, &begin, &end);//Divide the elements of the queue

   // Push patrial tour in its private stack
   for (i = end; i >= begin; i--){
      int index = (queue->head + i) % queue->alloc_sz;
      Push_Stack(stack, queue->list[index]);
   }
}

void Set_Thread_Tours(long rank, int* begin, int* end){
    int stride, left, count;
    
    stride = init_tour_num / thread_num;
    left = init_tour_num % thread_num;
    
    if(rank < left){
        count = stride + 1;
        *begin = rank * count;
    }else{
        count = stride;
        *begin = rank * count + left;
    }
    
    *end = *begin + count - 1;
}

void Set_Init_Queue(){
    int nbr;
    int count = 0;
    tour_p tour = Alloc_Tour(NULL);
    
    Init_Tour(tour, SOURCE); 
    queue = Init_Queue(2 * queue_sz);
    
    // Breadth fist Search
    Enqueue(queue, tour);
    Free_Tour(tour, NULL);
    count++;
   //until reach a level of the tree that has at least the number of threads. 
    while(count < thread_num){
        tour = Dequeue(queue);//start from the hometown
        count--;
        for (nbr = 1; nbr < city_num; nbr++)// a complete tour
            if (!Visited(tour, nbr)) { // not visited and less than the cost of best tour
                Add_City(tour, nbr);
                Enqueue(queue, tour);
                count++;
                Del_Last_City(tour);
            }
      Free_Tour(tour, NULL);
   }  
   
   init_tour_num = count;// get the number of nodes at this level
}

void Add_City(tour_p tour, int city){
   int end = tour->cities[(tour->count)-1];
   tour->cities[tour->count] = city;
   (tour->count)++;
   tour->cost += city_graph[end * city_num + city];
}

void Del_Last_City(tour_p tour){
   int old_end = tour->cities[tour->count - 1];
   int new_end;
   
   tour->cities[tour->count - 1] = NO_CITY;
   (tour->count)--;
   new_end = tour->cities[tour->count - 1];
   tour->cost -= city_graph[new_end * city_num + old_end];
}

int Best_Tour(tour_p tour){
    int cost_curr_tour = tour->cost;
    int end = tour->cities[(tour->count)-1];
    int distance = cost_curr_tour + city_graph[end * city_num + SOURCE];
    
    if (distance < best_tour->cost)
      return TRUE;
    else
      return FALSE;
}

void Update_Best_Tour(tour_p tour){
    pthread_mutex_lock(&best_tour_mutex);
    if(Best_Tour(tour)){
        Clone_Tour(tour, best_tour);
        Add_City(best_tour, SOURCE);
    }
    pthread_mutex_unlock(&best_tour_mutex);
}

void Clone_Tour(tour_p tour1, tour_p tour2){
   for (int i = 0; i <= city_num; i++)
    tour2->cities[i] =  tour1->cities[i];
    
   tour2->count = tour1->count;
   tour2->cost = tour1->cost;
}

int Work(tour_p tour, int city){
    int end = tour->cities[tour->count - 1];
    int distance = tour->cost + city_graph[end * city_num + city];
    
    if( !Visited(tour, city) && (distance < best_tour->cost) )
      return TRUE;
   else
      return FALSE;
}

int Visited(tour_p tour, int city){

   for (int i = 0; i < tour->count; i++){
      if ( tour->cities[i] == city ) 
        return TRUE;
   }
   
   return FALSE;
}

// Barrier
barrier_p Init_Barrier(int count){
    
    barrier_p bar = malloc(sizeof(barrier_struct));
    bar->count = 0;
    bar->max = count;
    pthread_mutex_init(&bar->mutex, NULL);
    pthread_cond_init(&bar->cond, NULL);

    return bar;    
}

void Destory_Barrier(barrier_p bar){
   pthread_mutex_destroy(&bar->mutex);
   pthread_cond_destroy(&bar->cond);
   free(bar);    
}

void Barrier(barrier_p bar){
   
   pthread_mutex_lock(&bar->mutex);
   bar->count++;
   
   if (bar->count == bar->max) {
      bar->count = 0;
      pthread_cond_broadcast(&bar->cond); //send a singal to wake up all threads
   } else {
//Only a call to pthread_cond_broadcast or pthread_cond_signal will return zero values.
      while (pthread_cond_wait(&bar->cond, &bar->mutex) != 0);
   }
   pthread_mutex_unlock(&bar->mutex);

}
