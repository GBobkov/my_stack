#ifndef MY_STACK_H
#define MY_STACK_H

#include "constants.h"


long long int Do_Stack_Push(STACK* stack, ELEMENT_TYPE new_element);
ELEMENT_TYPE Do_Stack_Pop(STACK* stack);
void Do_Stack_Init(STACK *stack, long int capacity, const char* name, const char* file, const char* func, const int line);
void Do_Stack_Destroy(STACK* stack, const char* name, const char* file, const char* func, const int line);


#endif