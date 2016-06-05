#include "../head/head.h"

// Stack
stack_p Init_Stack(void){
   int n = city_num;
   stack_p stack = malloc(sizeof(stack));
   stack->list = malloc(n * n * sizeof(tour_p));
   for (int i = 0; i < n * n; i++)
      stack->list[i] = NULL;
   stack->list_sz = 0;

   return stack;
}

int  Empty(stack_p stack){
   if (stack->list_sz == 0)
      return TRUE;
   else
      return FALSE;
}

void Push_Stack(stack_p stack, tour_p tour){
   tour_p tmp;

   tmp = Alloc_Tour();
   Clone_Tour(tour, tmp);
   stack->list[stack->list_sz] = tmp;
   (stack->list_sz)++;
}

void Push_Avail_Stack(tour_p tour){

    avail_stack->list[avail_stack->list_sz] = tour;
    (avail_stack->list_sz)++;
}

tour_p Pop_Stack(stack_p stack){
   tour_p tmp;

   tmp = stack->list[stack->list_sz-1];
   stack->list[stack->list_sz-1] = NULL;
   (stack->list_sz)--;
   return tmp;
}

void Free_Avail_Stack(){
   tour_p tmp;

   for (int i = 0; i < avail_stack->list_sz; i++) {
      tmp = avail_stack->list[i];
      if (tmp != NULL) {
         free(tmp->cities);
         free(tmp);
      }
   }
   free(avail_stack->list);
   free(avail_stack);
}

void Free_Stack(stack_p stack){
   free(stack->list);
   free(stack);
}
