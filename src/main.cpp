#include <stdio.h>
#include <stdlib.h>
#include <locale.h>

#include "colors.h"
#include "tree.h"
#include "stack_main.h"
#include "log.h"
#include "func.h"
#include "myassert.h"
#include "stack_base.h"
#include "stack_support.h"
#include "festival.h"

int main(int argc, char *argv[])
{
    setlocale(LC_ALL, "Russian");
    if (argc == 2)
    {
        printf(GREEN "\n<<<Файл для сохранения базы данных: %s>\n" RESET, argv[1]);
    }
    
ON_FESTIVAL(
    fprintf(stdout, GREEN "\nЗагрузка...\n" RESET);
    system(ECHO "Загрузка" FESTIVAL);
)
    BinaryTree_t myTree = {};
    Stack_t StackObject1 = {};    
    Stack_t StackObject2 = {};

    TreeCtor (&myTree);
    StackCtor (&StackObject1);
    StackCtor (&StackObject2);
    PrintLogStart();
    PrintLogTree (&myTree);

    fprintf(stdout, GREEN "Загрузка завершена!\n\n" RESET);
    
    fprintf(stdout, GREEN "Загрузить старую базу данных?\n" RESET);
ON_FESTIVAL(
    system(ECHO "Загрузить старую базу данных?" FESTIVAL);
)    
    int choice = UserChoice();
    if (choice == 'Y')
    {
        fprintf(stdout, GREEN "Загрузка базы данных...\n" RESET);
        UploadDataBase(&myTree, argv[1]);
        fprintf(stdout, GREEN "Загрузка базы данных завершена!\n\n" RESET);
ON_FESTIVAL(
        system(ECHO "Загрузка базы данных завершена!" FESTIVAL);
)    
    }   

//Подгрузка БД

    AkinatorWork(&myTree, &StackObject1, &StackObject2, argv[1]);

    PrintLogFinish();
    StackDtor (&StackObject1);
    StackDtor (&StackObject2);
    TreeDtor (&myTree);
    printf(GREEN "\n<<<GOOD ENDING>>>\n\n" RESET);
    return 0;   
}