#include <stdio.h>
#include <stdlib.h>
#include <locale.h>

#include "colors.h"
#include "tree.h"
#include "log.h"
#include "func.h"
#include "myassert.h"
#include "stack_main.h"
#include "stack_base.h"

int main(int argc, char *argv[])
{
    setlocale(LC_ALL, "Russian");
    BinaryTree_t myTree = {};
    
    FILE* filestream = NULL;

    if (argc == 2)
    {
        printf(GREEN "\n<<<FILE TO PRINT TREE: %s>\n\n" RESET, argv[1]);
        filestream = OpenFile(argv[1]);
    }

    Stack_t myStack = {};
    StackCtor (&myStack);

    TreeCtor (&myTree);
    PrintLogStart();
    PrintLogTree (&myTree);

//Подгрузка БД

    Guess(&myTree);
    Guess(&myTree);

    TreePreOrder(&myTree, filestream);
    TreeInOrder(&myTree, filestream);
    TreePostOrder(&myTree, filestream);

    PrintLogFinish();
    StackDtor (&myStack);
    TreeDtor (&myTree);
    CloseFile(filestream);
    printf(GREEN "\n<<<GOOD ENDING>>>\n\n" RESET);
    return 0;   
}