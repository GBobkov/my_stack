#include "my_stack.h"

#include <stdio.h>
#include <stdlib.h>


int main()
{   
    STACK stk = {};
    
    Stack_Init(stk, 0);
    
    
    for (int i = 1; i <= 9; i++)
        Stack_Push(stk, i);
    

    Stack_Dump(stk);
    // for (int i = 1; i <= 20; i++)
    // {
    //     Stack_Dump(stk);
    //     Stack_Pop(stk); 
    // }
    // Stack_Dump(stk);
    
    return 0;
}