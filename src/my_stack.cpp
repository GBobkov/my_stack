#include "my_stack.h"
#include "stack_check.h"
#include "colors.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

// ON_DEBUG(extern ELEMENT_TYPE POIZON_VALUE;
//         extern const CANARY_t LST_CNR;
//         extern const CANARY_t RST_CNR;
//         extern const CANARY_t LDT_CNR;
//         extern const CANARY_t RDT_CNR;)



const size_t ELEMENTARY_CAPACITY = 4;
const int MEMORY_INCREASE_COEFFICIENT=2;
const int MEMORY_DECREASE_COEFFICIENT=2;
static const int PUSH_ELEMENT_MODE= 1;
static const int POP_ELEMENT_MODE =2;



static long long Stack_Realloc_Increase(STACK* stack) 
{
    ON_DEBUG(if (Stack_Assert(stack)) return stack->errors;)
    stack->size += 1;
    if (stack->size > stack->capacity)
        {
            if (stack->capacity)
                stack->capacity *= MEMORY_INCREASE_COEFFICIENT;
            else
                stack->capacity = ELEMENTARY_CAPACITY;
                
            ON_DEBUG
            (if (stack->data)
                stack->data = (ELEMENT_TYPE *)((char *)stack->data - sizeof(CANARY_t));)

            stack->data = (ELEMENT_TYPE *) realloc((void *) stack->data, stack->capacity * sizeof(ELEMENT_TYPE) ON_DEBUG(+ 2 * (sizeof(CANARY_t)) + 7));
            
            ON_DEBUG
            (
            
            ((CANARY_t *)stack->data)[0] = LDT_CNR;
            stack->data = (ELEMENT_TYPE *)((char *)stack->data + sizeof(CANARY_t));
            *(CANARY_t *)((char *)stack->data + Calculate_Correct_Data_Size(stack->capacity, sizeof(ELEMENT_TYPE))) = RDT_CNR;
            )
        }
    if (!stack->data) return NULL_DATA_PTR;
    ON_DEBUG(Fill_Poizon(stack);)
    return NO_ERROR;
}


static long long Stack_Realloc_Desrease(STACK* stack)
{
    ON_DEBUG(if (Stack_Assert(stack)) return stack->errors;)
    stack->size -= 1;
    if (stack->size * MEMORY_INCREASE_COEFFICIENT * MEMORY_INCREASE_COEFFICIENT <= stack->capacity || stack->size == 0)
    {
        stack->capacity /= MEMORY_DECREASE_COEFFICIENT;     
        ON_DEBUG(if (stack->data)
            stack->data = (ELEMENT_TYPE *)((char *)stack->data - sizeof(CANARY_t));)

        stack->data = (ELEMENT_TYPE *) realloc((void *) stack->data, stack->capacity * sizeof(ELEMENT_TYPE) ON_DEBUG(+ 2 * (sizeof(CANARY_t)) + 7));
        
        ON_DEBUG
        (
        ((CANARY_t *)stack->data)[0] = LDT_CNR;
        stack->data = (ELEMENT_TYPE *)((char *)stack->data + sizeof(CANARY_t));
        *(CANARY_t *)((char *)stack->data + Calculate_Correct_Data_Size(stack->capacity, sizeof(ELEMENT_TYPE))) = RDT_CNR;
        )
    }
    if (!stack->data) return NULL_DATA_PTR;
    ON_DEBUG(Fill_Poizon(stack);)
    return NO_ERROR;
}


void Do_Stack_Init(STACK *stack, long int capacity, const char* name, const char* file, const char* func, const int line) 
{  
    
    if (!stack)
    {
        printf(ANSI_RED "%s:%d(%s) Can't construct stack:%s=NULL" ANSI_RESET_COLOR, file, line, func, name);
        return;
    }


    stack->size = 0;
    stack->capacity = capacity;
    stack->data = (ELEMENT_TYPE *)calloc(capacity * sizeof(ELEMENT_TYPE) ON_DEBUG(+ 2 * (sizeof(CANARY_t)) + 7), 1);


    ON_DEBUG
    (

        stack->name=name;
        stack->file=file;
        stack->line=line;
        stack->func=func;
        stack->left_canary=LST_CNR;
        stack->right_canary=RST_CNR;

        ((CANARY_t *)stack->data)[0] = LDT_CNR;
        stack->data = (ELEMENT_TYPE *)((char *)stack->data + sizeof(CANARY_t));
        *(CANARY_t *)((char *)stack->data + Calculate_Correct_Data_Size(stack->capacity, sizeof(ELEMENT_TYPE))) = RDT_CNR;

        Fill_Poizon(stack);
        Update_Hashsums(stack);

        Stack_Error(stack);
        if (stack->errors != NULL_DATA_PTR && stack->errors != NO_ERROR)
            {
                printf(ANSI_RED "%s:%d(%s) problems with construct. Errors=%lld" ANSI_RESET_COLOR, file, line, func, stack->errors);
                Do_Stack_Dump(stack, file, func, line);
                Do_Stack_Destroy(stack, name, file, func, line);
                return;
            }
    )

}


void Do_Stack_Destroy(STACK* stack, const char* name, const char* file, const char* func, const int line)
{
    if(!stack)
    { 
        printf("Can't destroy \"%s\"(=NULL) at %s:%d(%s)", name, file, line, func);
        return;
    }

    stack->capacity = 0;
    stack->size = 0;
    if (!stack->data)
        free(stack->data ON_DEBUG(- 1));
    stack->data = NULL;
}


long long int Do_Stack_Push(STACK* stack, ELEMENT_TYPE new_element)
{
    ON_DEBUG(if(Stack_Assert(stack)) return stack->errors;)

    if (Stack_Realloc_Increase(stack)) return NULL_DATA_PTR;


    stack->data[stack->size - 1] = new_element;
    //Do_Stack_Dump(stack, __FILE__, __FUNCTION__, __LINE__);
    ON_DEBUG
    (
        Update_Hashsums(stack);
        Stack_Assert(stack);
        return stack->errors;
    )

    return 0;

}


ELEMENT_TYPE Do_Stack_Pop(STACK* stack)
{   

    ON_DEBUG(if (Stack_Assert(stack)) return (ELEMENT_TYPE) stack->errors;)
    if (stack->size <= 0)
    {
        ON_DEBUG(return POIZON_VALUE;)
        return 0;
    }

    ELEMENT_TYPE value_return = stack->data[stack->size - 1];
    
    if (Stack_Realloc_Desrease(stack))
    {
        ON_DEBUG(return POIZON_VALUE;)
        return 0;
    }
    ON_DEBUG
    (
        Update_Hashsums(stack);
        if (Stack_Assert(stack)) return POIZON_VALUE;
    )
    return value_return;
}

