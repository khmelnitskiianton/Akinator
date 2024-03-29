#ifndef MYASSERT_H
#define MYASSERT_H

#include <assert.h>
#include "colors.h"
#include "errors_enum.h"

#define DEBUG_MYASSERT 1

#ifdef DEBUG_MYASSERT
#define MYASSERT(check, error_code, ending)                                                                             \
        do                                                                                                              \
        {                                                                                                               \
            if (!(check))                                                                                               \
            {                                                                                                           \
                fprintf(stderr, MAGENTA ">>>>> Error in code! Error code: %s <<<<<\n", #error_code);                    \
                fprintf(stderr, "In File: %s, In Line: %d, In Function: %s\n",__FILE__,__LINE__,__PRETTY_FUNCTION__);   \
                fprintf(stderr, "In this condition:\t\"%s\"\n\n" RESET, #check);                                        \
                ending;                                                                                                 \
            }                                                                                                           \
        }                                                                                                               \
        while(0);
#else   
#define MYASSERT(check, error_code, ending)
#endif

#endif