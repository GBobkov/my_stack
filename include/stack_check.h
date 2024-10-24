#include "constants.h"

#ifdef DEBUG

#ifndef STACK_ERRORS_H
#define STACK_ERRORS_H

#include <stdio.h>


void Update_Hashsums(STACK* stack);
long long int Check_Damage(STACK* stack);
void Do_Stack_Dump(STACK* stack, const char* file, const char* func, const int line);
long long Do_Stack_Assert(STACK *stack, const char* file, const char* func, const int line);
void Printf_Errors(long long int problem);
void Fill_Poizon(STACK *stack);
uint64_t Calculate_Correct_Data_Size(size_t elements_num, size_t element_size);
long long Stack_Error(STACK* stack);

#endif

#endif