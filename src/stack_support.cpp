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

StackElem_t* expansion (Stack_t *stk)
{

ON_CANARY_IF( 
    (stk -> capacity) = (stk -> capacity) * MULTIPLIER;                                                                                                                                     
    if ((stk -> l_canary_data = (Canary_t*) realloc ((stk -> l_canary_data), size_data(stk))) == nullptr)
    {   
        fprintf(stdout,            "ERROR IN INITIALIZATION OF CALLOC");
        fprintf(stk -> file_write, "ERROR IN INITIALIZATION OF CALLOC");  
        abort();
    }     
    void* new_place = nullptr;                                                                                                                                                              
    new_place = (void*)((stk -> l_canary_data) + 1);                                                                                                                                        
    mem_poison ((char*) new_place + sizeof(StackElem_t)*(stk -> size), (stk -> capacity)-(stk -> size));                                                                        
    ptr_r_canary_data (stk);                                                                                                                                                                
    *(stk -> r_canary_data) = 0xDEADBEEF; 
ON_HASH(
    rewrite_hash(stk, (const char*)(stk -> l_canary_data));
)
)
ON_CANARY_ELSE( 
    void* new_place = nullptr;
    (stk -> capacity) = (stk -> capacity) * MULTIPLIER;                                                                                      
    if ((new_place = realloc ((stk -> data), sizeof(StackElem_t)*((size_t) stk -> capacity))) == nullptr)
    {   
        fprintf(stdout,            "ERROR IN RECALLOC IN %s", __PRETTY_FUNCTION__);
        fprintf(stk -> file_write, "ERROR IN RECALLOC IN %s", __PRETTY_FUNCTION__);  
        abort();
    }
                                                 
    mem_poison ((void*) ((char*) new_place + sizeof(StackElem_t)*((size_t) stk -> size)), (stk -> capacity)-(stk -> size));                        
ON_HASH(
    rewrite_hash(stk, (const char*)(stk -> data));
)
)
    return (StackElem_t*) new_place;
}

StackElem_t* comprassion (Stack_t *stk)
{
ON_CANARY_IF( 
    (stk -> capacity) = (stk -> capacity) / MULTIPLIER;
    if ((stk -> l_canary_data = (Canary_t*) realloc ((stk -> l_canary_data), size_data(stk))) == nullptr)
    {   
        fprintf(stdout,            "ERROR IN RECALLOC IN %s", __PRETTY_FUNCTION__);
        fprintf(stk -> file_write, "ERROR IN RECALLOC IN %s", __PRETTY_FUNCTION__);  
        abort();
    }
    void* new_place = nullptr;
    new_place = (void*)((stk -> l_canary_data) + 1);
    ptr_r_canary_data (stk);
    *(stk -> r_canary_data) = 0xDEADBEEF;
ON_HASH(
    rewrite_hash(stk, (const char*)(stk -> l_canary_data));
)
)
ON_CANARY_ELSE(
    void* new_place = nullptr; 
    (stk -> capacity) = (stk -> capacity) / MULTIPLIER;
    if ((new_place = realloc ((stk -> data), sizeof(StackElem_t)*((size_t) stk -> capacity))) == nullptr)
    {
        fprintf(stdout,            "ERROR IN RECALLOC IN %s", __PRETTY_FUNCTION__);
        fprintf(stk -> file_write, "ERROR IN RECALLOC IN %s", __PRETTY_FUNCTION__);  
        abort();
    }
ON_HASH(
    rewrite_hash(stk, (const char*)(stk -> data));
)                             
)
    return (StackElem_t*) new_place;
}

int mem_poison (void* memptr, int num)
{
    for (int i = 0; i < num; i++)
    {
        *(((StackElem_t*) memptr) + i) = STACK_POISON_ELEMENT;
    }
    return 1;
}

int StackDtor (Stack_t *stk)
{
    ASSERT_STACK(stk, __PRETTY_FUNCTION__)
ON_CANARY_IF( 
    free(stk -> l_canary_data);        
    stk -> l_canary_data = NULL;
    stk -> r_canary_data = NULL;
)
ON_CANARY_ELSE( 
    free(stk -> data);
)
    file_close(stk -> file_write);
    stk -> data = NULL;
    stk = NULL;
    return 1;
}

int checkStack (Stack_t *stk, const char* func)
{
    int error_code = 0;
    
    if (!stk)                    
    {
        error_code |= 1<<0;
        return error_code;
    }
    if (!(stk -> file_write))                    
    {
        error_code |= 1<<1;
        return error_code;
    }

ON_CANARY(
    if (!(stk -> l_canary_data)) 
    {
        error_code |= 1<<2;  
        return error_code;
    }
    if (!(stk -> r_canary_data))
    { 
        error_code |= 1<<3;
        return error_code;
    }
)   

    if (!(stk -> data))
    {
        error_code |= 1<<4;
        return error_code;
    }

    if ((strcmp(func, "int pop(Stack_t*, StackElem_t*)") == 0)&&(!(stk -> ret_value)))
    {
        error_code |= 1<<5;
        return error_code;
    }
    if ((stk -> capacity) <= 0)             
    {
        error_code |= 1<<6;
        return error_code;
    }
        if ((stk -> size) < 0)                  
    {
        error_code |= 1<<7;
        return error_code;
    }
    if ((stk -> size) > (stk -> capacity))  
    {
        error_code |= 1<<8;
        return error_code;
    }
    if ((strcmp(func, "int pop(Stack_t*, StackElem_t*)") == 0)&&((stk -> size) == 0)) 
    {
        error_code |= 1<<9;
        return error_code;
    }
ON_CANARY(
    if ((stk -> l_canary)       != 0xDEADBABE) error_code |= 1<<10; 
    if ((stk -> r_canary)       != 0xDEADBABE) error_code |= 1<<11;
 
    if (*(stk -> l_canary_data) != 0xDEADBEEF) error_code |= 1<<12;  
    if (*(stk -> r_canary_data) != 0xDEADBEEF) error_code |= 1<<13;
)
ON_CANARY_IF(
ON_HASH(
    Hash_t save_hash = (stk -> hash_struct);
    stk -> hash_struct = 0;
    if ((save_hash) != hash_func((const char*)(stk), sizeof(Stack_t))) 
    {
        error_code |= 1<<14;
        return error_code;
    }
    else stk -> hash_struct = hash_func((const char*)(stk), sizeof(Stack_t));
    if ((stk -> hash_data) != hash_func((const char*)(stk -> l_canary_data), size_data(stk))) error_code |= 1<<15;
) 
)
ON_CANARY_ELSE(
ON_HASH(
    Hash_t save_hash = (stk -> hash_struct);
    stk -> hash_struct = 0;
    if ((save_hash) != hash_func((const char*)(stk), sizeof(Stack_t))) 
    {
        error_code |= 1<<14;
        return error_code;
    }
    else stk -> hash_struct = hash_func((const char*)(stk), sizeof(Stack_t));
    if ((stk -> hash_data) != hash_func((const char*)(stk -> data), size_data(stk))) error_code |= 1<<15;
)
)
    return error_code;
}

int output_error (Stack_t *stk, const char* file, const size_t line, const char* pretty_function, int error_code)
{   
    const char* mass_of_errors[N_ERRORS] = {
        "ADDRESS OF STRUCTURE == NULL. OUTPUT IN TERMINAL",
        "ADDRESS OF FILE_WRITE == NULL",
        "ADDRESS OF LEFT CANARY IN DATA == NULL",
        "ADDRESS OF RIGHT CANARY IN DATA == NULL",
        "ADDRESS OF ARRAY IN STRUCTURE == NULL",
        "ADDRESS OF RETURNING ARGUMENT IN POP == NULL",
        "CAPACITY <= 0",
        "SIZE < 0",
        "SIZE > CAPACITY",
        "POP() BUT DATA EMPTY",
        "LEFT CANARY IN STRUCT IS DEAD. ANAL PENETRATION IN STRUCT",
        "RIGHT CANARY IN STRUCT IS DEAD. ANAL PENETRATION IN STRUCT",
        "LEFT CANARY IN DATA IS DEAD. ANAL PENETRATION IN DATA",
        "RIGHT CANARY IN DATA IS DEAD. ANAL PENETRATION IN DATA",
        "HASH BROKE. ANAL PENETRATION IN STRUCT",
        "HASH BROKE. ANAL PENETRATION IN DATA"
    };
    int z = error_code;
    unsigned long bin_error = 0;
    int element = 0;
    int fatal_error = 0;
    printf("<<<<<!!!!YOU HAVE ERROR.CHECK ./log/log_stack.txt OR TERMINAL!!!!>>>>>>>\n");
    if (z == 1)
    {
        printf("\n<<<<<<<<<<<<<<<YOU HAVE ERROR>>>>>>>>>>>>>>>>>\n"
            "Stack_t[%p] called from %s (string: %lu) in function %s\n"
            "1: [%s]\n", stk, file, line, pretty_function,mass_of_errors[0]);
        abort();
    }
    if (z == 2)
    {
        printf("\n<<<<<<<<<<<<<<<YOU HAVE ERROR>>>>>>>>>>>>>>>>>\n"
            "Stack_t[%p] called from %s (string: %lu) in function %s\n"
            "0: [OK]\n"
            "1: [%s]\n", stk, file, line, pretty_function,mass_of_errors[1]);
        abort();
    }

    fprintf(stk -> file_write, "\n<<<<<<<<<<<<<<<YOU HAVE ERROR>>>>>>>>>>>>>>>>>\n");
    fprintf(stk -> file_write, "\nERROR:\n");

    while (z > 0)
    {
        if (z % 2)
        {
            bin_error += (size_t) 1<<(element);
            fprintf(stk -> file_write, "1: [%s]\n", mass_of_errors[element]);
            if(element != 15) fatal_error = 1;
        }
        else
        {
            fprintf(stk -> file_write, "0: [OK]\n");
        }
        z = z / 2;
        element++;
    }
    fprintf(stk -> file_write,"ERROR CODE: [%lu]\n\n", bin_error);
    
    if (!fatal_error)
    {
    fprintf (stk -> file_write,"Stack_t[%p] called from %s (string: %lu) in function %s\n"
    //      "\t\t%s             \n"
            "{                  \n"
            "\tsize        = %d \n"
            "\tcapacity    = %d \n"
        ON_HASH(
            "\thash_data   = %d \n"
            "\thash_struct = %d \n"
        )
            "\tdata[%p]         \n"
            "\t{                \n", stk, file, line, pretty_function, stk -> size, stk -> capacity, ON_HASH(stk -> hash_data, stk -> hash_struct,) stk -> data);

    for (ssize_t i = 0; (i < (stk -> capacity)); i++)
    {
        if (i == stk -> size)                               fprintf(stk -> file_write,"\t >[%ld] = %d<\n", i, *((stk -> data) + i));
        else if (*((stk -> data) + i) != STACK_POISON_ELEMENT)    fprintf(stk -> file_write,"\t #[%ld] = %d\n", i, *((stk -> data) + i));
        else                                                fprintf(stk -> file_write,"\t @[%ld] = %d(POISON)\n", i, *((stk -> data) + i));
    }
    
    fprintf (stk -> file_write,"\t} \n"
            "}   \n");
    }
    file_close(stk -> file_write);
    abort();
    return 1;
}

int printing_stack (Stack_t* stk, const char* file, const size_t line, const char* pretty_function)
{
    fprintf (stk -> file_write,"\nStack[%p] called from %s (string: %lu) in function %s\n"
            "{                  \n"
            "\tsize        = %d \n"
            "\tcapacity    = %d \n"
        ON_HASH(
            "\thash_data   = %d \n"
            "\thash_struct = %d \n"
        )
            "\tdata[%p]         \n"
            "\t{                \n", stk, file, line, pretty_function, stk -> size, stk -> capacity, ON_HASH(stk -> hash_data, stk -> hash_struct,) stk -> data);

    for (ssize_t i = 0; (i < (stk -> capacity)); i++)
    {
        if (i == (stk -> size))                             fprintf(stk -> file_write,"\t >[%ld] = %d<\n", i, *((stk -> data) + i));
        else if (*((stk -> data) + i) != STACK_POISON_ELEMENT)    fprintf(stk -> file_write,"\t #[%ld] = %d\n", i, *((stk -> data) + i));
        else                                                fprintf(stk -> file_write,"\t @[%ld] = %d(POISON)\n", i, *((stk -> data) + i));
    }
    
    fprintf (stk -> file_write,"\t} \n"
            "}   \n");
    return 1;
}

int degree(int a, int b)
{
    int x = 1;
    for (int i = 0; i < b; i++)
    {
        x = x * a;
    }
    return x;
}

ON_CANARY(
Canary_t* ptr_r_canary_data (Stack_t* stk)                                                                                                    
{                                                                                                                                           
    Canary_t* ptr = (Canary_t*)((char*)((stk -> data) + (stk -> capacity))+((stk -> capacity)*sizeof(StackElem_t)) % sizeof(Canary_t));          
    stk -> r_canary_data = ptr;                                                                                                             
    return ptr;                                                                                                                             
}   
)

Hash_t hash_func(const char * str, size_t size)
{
    return MYHashFAQ6(str, size);
}

Hash_t MYHashFAQ6(const char * str, size_t size)
{
    unsigned int hash = 0;
    const char* z = str;

    for (; str < (z + size); str++)
    {
        hash += (unsigned char)(*str);
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }
    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);
    return hash;
}

size_t size_data (Stack_t* stk)
{
ON_CANARY_IF(
    return sizeof(Canary_t)*2 + sizeof(StackElem_t)*(stk -> capacity) + ((stk -> capacity)*sizeof(StackElem_t)) % sizeof(Canary_t);
)
ON_CANARY_ELSE(
    return sizeof(StackElem_t)*((size_t) (stk -> capacity));
)
}

FILE* file_write (void)
{
    FILE *file_write = fopen ("./log/log_stack.txt", "w");

    assert(file_write);

    return file_write;
}

int file_close (FILE *file_text)
{
	assert(file_text);
	assert(!fclose (file_text));
	
    return 1;
}
ON_HASH(
int rewrite_hash (Stack_t* stk, const char* ptr)
{
    stk -> hash_struct = 0;
    stk -> hash_data = hash_func(ptr, size_data(stk));
    stk -> hash_struct = hash_func((const char*)(stk), sizeof(Stack_t));
    return 1;
}
)