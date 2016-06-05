#include "head.h"

// Queue
extern queue_p Init_Queue(int size);
extern tour_p Dequeue(queue_p queue);
extern void Enqueue(queue_p queue, tour_p tour);
extern int Empty_Queue(queue_p queue);
extern void Free_Queue(queue_p queue);