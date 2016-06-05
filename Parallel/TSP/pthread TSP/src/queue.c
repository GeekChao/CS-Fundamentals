#include "../head/queue.h"

// Queue
queue_p Init_Queue(int size){
   queue_p q = malloc(sizeof(queue_struct));
   q->list = malloc(size * sizeof(tour_p));
   q->alloc_sz = size;
   q->head = q->tail = q->full = 0;

   return q;
}

tour_p Dequeue(queue_p queue){
   tour_p tmp;

   tmp = queue->list[queue->head];
   queue->head = (queue->head + 1) % queue->alloc_sz;
   
   return tmp;
}

void Enqueue(queue_p queue, tour_p tour){
   tour_p tmp;
   
   tmp = Alloc_Tour(NULL);
   Clone_Tour(tour, tmp);

   queue->list[queue->tail] = tmp;
   queue->tail = (queue->tail + 1) % queue->alloc_sz; 
   
   if (queue->tail == queue->head)
      queue->full = TRUE;
}

int Empty_Queue(queue_p queue){
   if (queue->full == TRUE)
      return FALSE;
   else if (queue->head != queue->tail)
      return FALSE;
   else
      return TRUE;
}

void Free_Queue(queue_p queue){
    
   free(queue->list);
   free(queue);
}