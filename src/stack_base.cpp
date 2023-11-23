#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <limits.h>
#include <math.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "stack_main.h"
#include "stack_base.h"
#include "stack_support.h"

int StackCtor (Stack_t *stk)
{
    stk -> file_write = file_write();
    stk -> size = 0; 
    stk -> capacity = 1;
    stk -> ret_value = 1;

ON_CANARY_IF(                                                
    stk -> l_canary = 0xDEADBABE;                                                                                                                   
    if ((stk -> l_canary_data = (Canary_t*) calloc (1, size_data(stk))) == nullptr)
    {
        fprintf(stdout,            "ERROR IN INITIALIZATION OF CALLOC IN %s", __PRETTY_FUNCTION__);
        fprintf(stk -> file_write, "ERROR IN INITIALIZATION OF CALLOC IN %s", __PRETTY_FUNCTION__);
        abort();
    }     
    stk -> data = (StackElem_t*) ((stk -> l_canary_data) + 1);                                                                                           
    ptr_r_canary_data (stk);                                                                                                                        
    *(stk -> l_canary_data) = 0xDEADBEEF;                                                                                                           
    *(stk -> r_canary_data) = 0xDEADBEEF;                                                                                                           
    stk -> r_canary = 0xDEADBABE; 
ON_HASH(
    rewrite_hash(stk, (const char*)(stk -> l_canary_data));
)                                      
)
ON_CANARY_ELSE( 
    if ((stk -> data = (StackElem_t*) calloc (1, size_data(stk))) == nullptr)
    {   
        fprintf(stdout,            "ERROR IN INITIALIZATION OF CALLOC");
        fprintf(stk -> file_write, "ERROR IN INITIALIZATION OF CALLOC");  
        abort();
    }
ON_HASH(
    rewrite_hash(stk, (const char*)(stk -> data));
)
)
    ASSERT_STACK(stk, __PRETTY_FUNCTION__)
    return 1;
}

int push (Stack_t *stk, StackElem_t value)
{ 
    ASSERT_STACK(stk, __PRETTY_FUNCTION__)

    if ((stk -> size) == (stk -> capacity))
    {
        (stk -> data) = expansion (stk);
    }

    *((stk -> data) + (stk -> size)) = value;
    (stk -> size)++; 

ON_CANARY_IF(
ON_HASH(
    rewrite_hash(stk, (const char*)(stk -> l_canary_data));
)
)
ON_CANARY_ELSE(
ON_HASH(
    rewrite_hash(stk, (const char*)(stk -> data));
)
)
    ASSERT_STACK(stk, __PRETTY_FUNCTION__)
ON_PRINTING(
    printing_stack (stk, __FILE__, __LINE__, __PRETTY_FUNCTION__);
)
    return 1;
}

int pop (Stack_t *stk, StackElem_t* ret_value)
{
    if (!stk) 
    {
        ASSERT_STACK(stk, __PRETTY_FUNCTION__)
    }
    else if (ret_value == NULL) 
    {
        stk -> ret_value = 0;
        ASSERT_STACK(stk, __PRETTY_FUNCTION__)
    }
    else 
    {
        stk -> ret_value = 1;
        ASSERT_STACK(stk, __PRETTY_FUNCTION__)
    }

    StackElem_t save_value = STACK_POISON_ELEMENT;
    (stk -> size)--;
    save_value = *((stk -> data) + (stk -> size));  
    if (((stk -> size) > 0) && ((stk -> size)*MULTIPLIER < (stk -> capacity)) && ((stk -> capacity) > MIN_LEN))
    {
        (stk -> data) = comprassion (stk);
    }
    *((stk -> data) + (stk -> size)) = STACK_POISON_ELEMENT;
    *ret_value = save_value;

ON_CANARY_IF(
ON_HASH(
    rewrite_hash(stk, (const char*)(stk -> l_canary_data));
)
)
ON_CANARY_ELSE(
ON_HASH(
    rewrite_hash(stk, (const char*)(stk -> data));
)
)
    ASSERT_STACK(stk, "")

ON_PRINTING(
    printing_stack (stk, __FILE__, __LINE__, __PRETTY_FUNCTION__);
)
    return 1;
}
