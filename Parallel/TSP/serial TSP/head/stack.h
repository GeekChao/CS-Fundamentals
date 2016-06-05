#include "head.h"
// Stack
extern stack_p Init_Stack(void);
extern void Push_Avail_Stack(tour_p tour);
extern void Push_Stack(stack_p stack, tour_p tour);
extern tour_p Pop_Stack(stack_p stack);
extern int  Empty(stack_p stack);
extern void Free_Stack(stack_p stack);
extern void Free_Avail_Stack(void);