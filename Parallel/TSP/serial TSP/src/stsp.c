#include "../head/head.h"
#include "../head/stack.h"

int main(int argc, char*argv[]){
    double begin, end;
    FILE* matrix;
    tour_p tour;
    
    // Read the total number of cities and matrix from disk
    matrix = fopen(argv[1], "r");
    Read_City_Graph(matrix);
    fclose(matrix);
    
    Print_City_Graph();
    
    // Allocate and initialize stacks and tours
    avail_stack = Init_Stack();
    best_tour = Alloc_Tour();
    Init_Tour(best_tour, DIS_INFINITY);
    
    tour = Alloc_Tour();
    Init_Tour(tour, DIS_SELF);
    
    Print_Tour(tour, "Beginning from the source");
    printf("City: %d\n",  tour->count);
    printf("Cost: %d\n\n", tour->cost);
    
    // Run depth-first search and count the finished timeval
    GET_TIME(begin);
    Iterative_DFS(tour);
    GET_TIME(end);
    
    // Display Results
    Print_Tour(best_tour, "Best Tour");
    printf("Cost: %d\n", best_tour->cost);
    printf("Finished time: %e seconds\n", end - begin);

    // Release Resources
    Free_Tour(tour);
    free(best_tour->cities);
    free(best_tour);
    Free_Avail_Stack();
    free(city_graph);
    
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

tour_p Alloc_Tour(){
   tour_p tmp;

   if (!Empty(avail_stack))
      return Pop_Stack(avail_stack);
   else {
      tmp = malloc(sizeof(tour));
      tmp->cities = malloc((city_num + 1) * sizeof(int));
      return tmp;
   }
}

void Free_Tour(tour_p tour){
    Push_Avail_Stack(tour);
}


void Print_Tour(tour_p tour, char* str){
   printf("%s: ", str);
    
   for (int i = 0; i < tour->count; i++)
      printf("%d ", tour->cities[i]);
      
   printf("\n");
}

void Iterative_DFS(tour_p tour){
   int nbr;
   stack_p stack;
   tour_p curr_tour;

   stack = Init_Stack();
   Push_Stack(stack, tour);
   
   while (!Empty(stack)) {
      curr_tour = Pop_Stack(stack); // start from the hometown
      Print_Tour(curr_tour, "Popped the top tour in the stack");
      printf("\n");
      if (curr_tour->count == city_num) { // a complete tour
         if (Best_Tour(curr_tour))
            Update_Best_Tour(curr_tour);
      } else {
         for (nbr = city_num - 1; nbr >= 1; nbr--) // a exhaustive search
            if (Work(curr_tour, nbr)) { // not visited and less than the cost of best tour
               Add_City(curr_tour, nbr);
               Push_Stack(stack, curr_tour);
               Del_Last_City(curr_tour);
            }
      }
      Free_Tour(curr_tour);// push current tour into the avail_stack for reusing
   }
   Free_Stack(stack);    
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
    Clone_Tour(tour, best_tour);
    Add_City(best_tour, SOURCE);
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

