#include "../head/stack.h"
// Stack
stack_p Init_Stack(void){
   int n = city_num;
   
   stack_p stack = malloc(sizeof(stack));
   stack->list = malloc(n * n * sizeof(tour_p));
   
   for (int i = 0; i < n * n; i++)
      stack->list[i] = NULL;
      
   stack->list_sz = 0;
   stack->alloc_sz = n * n;
   
   return stack;
}

int  Empty_Stack(stack_p stack){
   if (stack->list_sz == 0)
      return TRUE;
   else
      return FALSE;
}

void Push_Copy(stack_p stack, tour_p tour, stack_p avail){
    tour_p tmp;
    
    tmp = Alloc_Tour(avail);
    Clone_Tour(tour, tmp);
    stack->list[stack->list_sz] = tmp;
    (stack->list_sz)++;
}

void Push_Stack(stack_p stack, tour_p tour){

   stack->list[stack->list_sz] = tour;
   (stack->list_sz)++;
}

tour_p Pop_Stack(stack_p stack){
   tour_p tmp;

   tmp = stack->list[stack->list_sz-1];
   stack->list[stack->list_sz-1] = NULL;
   (stack->list_sz)--;
   return tmp;
}

void Free_Stack(stack_p stack){
    
   for (int i = 0; i < stack->list_sz; i++) {
      free(stack->list[i]->cities);
      free(stack->list[i]);
   }
   
   free(stack->list);
   free(stack);
}
