#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "colors.h"
#include "tree.h"
#include "log.h"
#include "myassert.h"
#include "verificator.h"
#include "func.h"

void TreeCtor (BinaryTree_t* myTree)
{
    MYASSERT(myTree, ERR_BAD_POINTER_TREE, return)

    Node_t* NewNode = (Node_t*) calloc (1,sizeof (Node_t));
    MYASSERT(NewNode, ERR_BAD_CALLOC, return)

    NewNode->Value = (char*) calloc(2, strlen("неизвестно кто") + 1);    //выделили память для нового слова и заполнили его
    strcpy(NewNode->Value, "неизвестно кто");  

    NewNode->Right  = NULL;
    NewNode->Left   = NULL;
    NewNode->Parent = NULL;

    myTree->Root = NewNode;
    myTree->Size = 1;
}

EnumOfErrors TreeDtor (BinaryTree_t* myTree)
{
    MYASSERT(myTree, ERR_BAD_POINTER_TREE, return ERR_BAD_POINTER_TREE)

    if (myTree->Root == NULL)
    {
        return ERR_OK;
    }

    Verify(myTree);

    RecFree (myTree->Root); 

    return ERR_OK;
}

EnumOfErrors TreeInsert (const char* LeftRight, Elem_t Value, Node_t* CurrentNode, BinaryTree_t* myTree)
{
    MYASSERT(myTree, ERR_BAD_POINTER_TREE, return ERR_BAD_POINTER_TREE)
    Node_t* NewNode = (Node_t*) calloc (1,sizeof (Node_t));
    MYASSERT(NewNode, ERR_BAD_CALLOC, return ERR_BAD_CALLOC)
    InitNode(NewNode);

    NewNode->Value  = Value;
    NewNode->Parent = CurrentNode;

    Verify(myTree);

    if (!strcmp(LeftRight, "ДА"))
    {
        CurrentNode->Left = NewNode;
    } 
    else if (!strcmp(LeftRight,"НЕТ"))
    {
        CurrentNode->Right = NewNode;
    }
    else 
    {
        printf(YELLOW "Непонятный ответ" RESET);
        return ERR_BAD_ANSWER;
    }

    myTree->Size++;

    PrintLogTree (myTree);
    return ERR_OK;
}


void RecFree (Node_t* CurrentNode)
{
    MYASSERT(CurrentNode, ERR_BAD_POINTER_NODE, return)

    if (CurrentNode->Left)
    {
        RecFree (CurrentNode->Left);
    }
    if (CurrentNode->Right)
    {
        RecFree (CurrentNode->Right);
    }

    free(CurrentNode->Value);
    free(CurrentNode);
}

EnumOfErrors TreePreOrder (BinaryTree_t* myTree, FILE* filestream)
{
    if (!filestream)
    {
        fprintf(stderr, MAGENTA "Forgot to write file where to write tree!\nTry to start with [./tree.exe <name_of_file>]\n\n" RESET);
        return ERR_BAD_FILESTREAM;
    }
    MYASSERT(myTree, ERR_BAD_POINTER_TREE, return ERR_BAD_POINTER_TREE)
    Verify(myTree);
    PrintPreNode(myTree->Root, filestream);
    fprintf(filestream, "\n");
    return ERR_OK;
}

EnumOfErrors TreeInOrder (BinaryTree_t* myTree, FILE* filestream)
{
    if (!filestream)
    {
        fprintf(stderr, MAGENTA "Forgot to write file where to write tree!\nTry to start with [./tree.exe <name_of_file>]\n\n" RESET);
        return ERR_BAD_FILESTREAM;
    }
    MYASSERT(myTree, ERR_BAD_POINTER_TREE, return ERR_BAD_POINTER_TREE)
    Verify(myTree);
    PrintInNode(myTree->Root, filestream);
    fprintf(filestream, "\n");
    return ERR_OK;
}

EnumOfErrors TreePostOrder (BinaryTree_t* myTree, FILE* filestream)
{
    if (!filestream)
    {
        fprintf(stderr, MAGENTA "Forgot to write file where to write tree!\nTry to start with [./tree.exe <name_of_file>]\n\n" RESET);
        return ERR_BAD_FILESTREAM;
    }
    MYASSERT(myTree, ERR_BAD_POINTER_TREE, return ERR_BAD_POINTER_TREE)
    Verify(myTree);
    PrintPostNode(myTree->Root, filestream);
    fprintf(filestream, "\n");
    return ERR_OK;
}

void PrintPreNode (Node_t* CurrentNode, FILE* filestream)
{
    if (!CurrentNode) {fprintf(filestream, " nil "); return;}
    fprintf(filestream, "(");

    fprintf(filestream, " " SPECIFIER " ", CurrentNode->Value);
    PrintPreNode(CurrentNode->Left, filestream);
    PrintPreNode(CurrentNode->Right, filestream);

    fprintf(filestream, ")");
}

void PrintInNode (Node_t* CurrentNode, FILE* filestream)
{
    if (!CurrentNode) {fprintf(filestream, " nil "); return;}
    fprintf(filestream, "(");

    PrintInNode(CurrentNode->Left, filestream);
    fprintf(filestream, " " SPECIFIER " ", CurrentNode->Value);
    PrintInNode(CurrentNode->Right, filestream);

    fprintf(filestream, ")");
}

void PrintPostNode (Node_t* CurrentNode, FILE* filestream)
{
    if (!CurrentNode) {fprintf(filestream, " nil "); return;}
    fprintf(filestream, "(");

    PrintPostNode(CurrentNode->Left, filestream);
    PrintPostNode(CurrentNode->Right, filestream);
    fprintf(filestream, " " SPECIFIER " ", CurrentNode->Value);

    fprintf(filestream, ")");
}

EnumOfErrors Guess (BinaryTree_t* myTree)
{
    MYASSERT(myTree, ERR_BAD_POINTER_TREE, return ERR_BAD_POINTER_TREE)
    MYASSERT(myTree->Root, ERR_ROOT_NULL, return ERR_ROOT_NULL)
    fprintf(stdout, CYAN "Давай я попробую отгадать:\n" RESET);
    EnumOfErrors result = RecGuess(myTree->Root, myTree);
    PrintLogTree (myTree);
    return result;
}

EnumOfErrors RecGuess (Node_t* CurrentNode, BinaryTree_t* myTree)
{
    //Определяем перед нами объект или определение? У определения два узла: нн или объект или определение
    if (CurrentNode->Left && CurrentNode->Right)    //если определение
    {
        fprintf(stdout, CYAN "Это %s ?\n" RESET, CurrentNode->Value);
        int choice = UserChoice();
        if (choice == 'Y')
        {
            RecGuess(CurrentNode->Right, myTree);
        }
        else if (choice == 'N')
        {
            RecGuess(CurrentNode->Left, myTree);
        }
        return ERR_OK;
    }
    if (!CurrentNode->Left && !CurrentNode->Right)                //Добрались до объекта типа
    {
        fprintf(stdout, CYAN "Это %s ?\n" RESET, CurrentNode->Value);
        int choice = UserChoice();
        if (choice == 'Y')
        {
            fprintf(stdout, GREEN "Я же говорила!" RESET); //отгадали
            return ERR_OK;
        }
        else if (choice == 'N')          //нет, значит новый объект + новое отличие
        {
            //Создание всего
            char buffer[SIZE_OF_BUFFER] = {};                                    //создали буфер для ввода пользователя

            Node_t* NewObjectNode = (Node_t*) calloc (1,sizeof (Node_t));        //создали новый объект
            MYASSERT(NewObjectNode, ERR_BAD_CALLOC, return ERR_BAD_CALLOC)
            InitNode(NewObjectNode);
            myTree->Size++;
            Node_t* NewFeatureNode = (Node_t*) calloc (1,sizeof (Node_t));        //создали новую особенность
            MYASSERT(NewFeatureNode, ERR_BAD_CALLOC, return ERR_BAD_CALLOC)
            InitNode(NewFeatureNode);
            myTree->Size++;

            //Пересвязка
            Node_t* MainNode = CurrentNode->Parent;
            NewFeatureNode->Parent = MainNode;
            if (MainNode)                       //Проверка что это не корень
            { 
                if (MainNode->Left == CurrentNode) MainNode->Left = NewFeatureNode;
                if (MainNode->Right == CurrentNode) MainNode->Right = NewFeatureNode;
            }
            else
            {
                myTree->Root = NewFeatureNode;              //если корень
            }
            CurrentNode->Parent = NewFeatureNode;
            NewObjectNode->Parent = NewFeatureNode;
            NewFeatureNode->Left = CurrentNode;
            NewFeatureNode->Right = NewObjectNode;

            //Заполняем новые данные по объекту и отличию
            //объект
            fprintf(stdout, CYAN "А кто же это был?\n" RESET);
            scanf("%s", buffer);
            clean_buffer();
            NewObjectNode->Value = (char*) calloc(2, strlen(buffer) + 1);    //выделили память для нового слова и заполнили его
            strcpy(NewObjectNode->Value, buffer);     
            CleanCharBuffer(buffer, SIZE_OF_BUFFER);
            //отличие
            fprintf(stdout, CYAN "А чем %s отличается от %s ?\nЭто..." RESET, NewObjectNode->Value, CurrentNode->Value); //Создаем новое определение
            scanf("%s", buffer);
            clean_buffer();
            fprintf(stdout, "\n");
            NewFeatureNode->Value = (char*) calloc(2, strlen(buffer) + 1);    //выделили память для нового слова и заполнили его
            strcpy(NewFeatureNode->Value, buffer);                     //заполнили объект кроме Parent
            CleanCharBuffer(buffer, SIZE_OF_BUFFER);       

            fprintf(stdout, CYAN "Я запомнил!!!\n" RESET);
            return ERR_OK;
        }
    }
    
    return ERR_NODE_HAVE_ONE_BRANCH;
}

int UserChoice (void)
{
    char buff[SIZE_OF_BUFFER] = {};
    fgets(buff, SIZE_OF_BUFFER, stdin);
    while (strcmp(buff,"Н\n"))
    {
        if (!strcmp(buff, "Д\n")) break;
        printf ( RED "Ты шо крейзи? Вводи Д или Н !!!" RESET "\n");
        CleanCharBuffer(buff, SIZE_OF_BUFFER);
        fgets(buff, SIZE_OF_BUFFER, stdin);
    }
    if (!strcmp(buff, "Н\n")) return 'N';
    else return 'Y';
}

void clean_buffer (void)
{
    int ch = 0;                     
    while ((ch = getchar ()) != '\n') {}   
}

void CleanCharBuffer(char* buffer, const size_t buffer_size)
{
    for (size_t i = 0; i < buffer_size; i++)
    {
        *(buffer + i) = 0;
    }
}

void InitNode(Node_t* NewNode)
{
    NewNode->Value  = NULL;
    NewNode->Right  = NULL;
    NewNode->Left   = NULL;
    NewNode->Parent = NULL;
}

// EnumOfErrors Definition (BinaryTree_t* myTree)
// {

// }

// EnumOfErrors RecSearch (Node_t* CurrentNode, BinaryTree_t* myTree)
// {

// }