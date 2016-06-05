#include "head.h"

// Stack
extern stack_p Init_Stack(void);
extern void Push_Stack(stack_p stack, tour_p tour);
extern tour_p Pop_Stack(stack_p stack);
extern int  Empty_Stack(stack_p stack);
extern void Free_Stack(stack_p stack);
extern void Push_Copy(stack_p stack, tour_p tour, stack_p avail);