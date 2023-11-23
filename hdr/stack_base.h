#ifndef STACK_BASE_H
#define STACK_BASE_H

int StackCtor       (Stack_t *stk);
int StackDtor       (Stack_t* stk);
int push            (Stack_t* stk, StackElem_t value);
int pop             (Stack_t* stk, StackElem_t* ret_value);

#endif