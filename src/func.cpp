#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "colors.h"
#include "tree.h"
#include "stack_main.h"
#include "log.h"
#include "myassert.h"
#include "verificator.h"
#include "func.h"
#include "stack_base.h"
#include "stack_support.h"

void TreeCtor (BinaryTree_t* myTree)
{
    MYASSERT(myTree, ERR_BAD_POINTER_TREE, return)

    Node_t* FirstNode = (Node_t*) calloc (1,sizeof (Node_t));
    InitNode (FirstNode);
    MYASSERT(FirstNode, ERR_BAD_CALLOC, return)

    strcpy(FirstNode->Value, UNKNOWN_OBJECT);

    myTree->Root = FirstNode;
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
    if (!CurrentNode) {fprintf(filestream, " [nil] "); return;}
    fprintf(filestream, "(");

    fprintf(filestream, " [" SPECIFIER "] ", CurrentNode->Value);
    PrintPreNode(CurrentNode->Left, filestream);
    PrintPreNode(CurrentNode->Right, filestream);

    fprintf(filestream, ")");
}

void PrintInNode (Node_t* CurrentNode, FILE* filestream)
{
    if (!CurrentNode) {fprintf(filestream, " [nil] "); return;}
    fprintf(filestream, "(");

    PrintInNode(CurrentNode->Left, filestream);
    fprintf(filestream, " [" SPECIFIER "] ", CurrentNode->Value);
    PrintInNode(CurrentNode->Right, filestream);

    fprintf(filestream, ")");
}

void PrintPostNode (Node_t* CurrentNode, FILE* filestream)
{
    if (!CurrentNode) {fprintf(filestream, " [nil] "); return;}
    fprintf(filestream, "(");

    PrintPostNode(CurrentNode->Left, filestream);
    PrintPostNode(CurrentNode->Right, filestream);
    fprintf(filestream, " [" SPECIFIER "] ", CurrentNode->Value);

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
            fprintf(stdout, GREEN "Я же говорила!\n" RESET); //отгадали
            return ERR_OK;
        }
        else if (choice == 'N')          //нет, значит новый объект + новое отличие
        {
            //Создание всего
            char buffer[SIZE_OF_BUFFER] = {};                                    //создали буфер для ввода пользователя

            Node_t* NewObjectNode = (Node_t*) calloc (1, sizeof (Node_t));        //создали новый объект
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
            int buff_ch = 0; 
            size_t counter_symbols = 0;
            
            while ((buff_ch = getchar()) != '\n') 
            {
                buffer[counter_symbols] = buff_ch;
                counter_symbols++;
            }
            buff_ch = 0;
            counter_symbols = 0;

            MYASSERT((strlen(buffer) + 1) < SIZE_OF_VALUE, ERR_OVERFLOW_VALUE, return ERR_OVERFLOW_VALUE)
            strcpy(NewObjectNode->Value, buffer);     
            CleanCharBuffer(buffer, SIZE_OF_BUFFER);
            //отличие
            fprintf(stdout, CYAN "А чем %s отличается от %s ?\nЭто..." RESET, NewObjectNode->Value, CurrentNode->Value); //Создаем новое определение
            
            while ((buff_ch = getchar()) != '\n') 
            {
                buffer[counter_symbols] = buff_ch;
                counter_symbols++;
            }
            
            MYASSERT((strlen(buffer) + 1) < SIZE_OF_VALUE, ERR_OVERFLOW_VALUE, return ERR_OVERFLOW_VALUE)
            strcpy(NewFeatureNode->Value, buffer);             
            CleanCharBuffer(buffer, SIZE_OF_BUFFER);       
            PrintLogTree(myTree);
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
    while ((strncmp(buff, "Н", 2) != 0) && (strncmp(buff, "н", 2) != 0))
    {
        if ((strncmp(buff, "Д", 2) == 0) || (strncmp(buff, "д", 2) == 0)) break;
        fprintf (stdout, RED "Ты шо крейзи? Вводи Да или Нет !!!\n" RESET);
        CleanCharBuffer(buff, SIZE_OF_BUFFER);
        fgets(buff, SIZE_OF_BUFFER, stdin);
    }
    if (!strncmp(buff, "Н", 2) || !strncmp(buff, "н", 2)) return 'N';
    else return 'Y';
}

const char* SelectOption (void)
{
    fprintf(stdout, CYAN "Что ты хочешь?: [У]гадать, [Д]ать определение, [С]равнить объекты,\n\t\t[П]оказать дерево, [В]ыйти с сохранением или [Б]ез сохранения.\n" RESET);
    char buff[SIZE_OF_BUFFER] = {};
    fgets(buff, SIZE_OF_BUFFER, stdin);
    buff[strlen(buff)-1] = '\0';
    while (true)
    {
        if (!strncmp(buff, "У", 2) || !strncmp(buff, "у", 2)) break;        //TODO: убрать копипаст
        if (!strncmp(buff, "Д", 2) || !strncmp(buff, "д", 2)) break;
        if (!strncmp(buff, "С", 2) || !strncmp(buff, "с", 2)) break;
        if (!strncmp(buff, "П", 2) || !strncmp(buff, "п", 2)) break;
        if (!strncmp(buff, "В", 2) || !strncmp(buff, "в", 2)) break;
        if (!strncmp(buff, "Б", 2) || !strncmp(buff, "б", 2)) break;
        fprintf (stdout,  RED "Я не понял команду :(\n" RESET);
        clean_buffer();
        CleanCharBuffer(buff, SIZE_OF_BUFFER);
        fgets(buff, SIZE_OF_BUFFER, stdin);
    }
    if (!strncmp(buff, "У", 2) || !strncmp(buff, "у", 2)) return "У";
    if (!strncmp(buff, "Д", 2) || !strncmp(buff, "д", 2)) return "Д";
    if (!strncmp(buff, "С", 2) || !strncmp(buff, "с", 2)) return "С";
    if (!strncmp(buff, "П", 2) || !strncmp(buff, "п", 2)) return "П";
    if (!strncmp(buff, "В", 2) || !strncmp(buff, "в", 2)) return "В";
    if (!strncmp(buff, "Б", 2) || !strncmp(buff, "б", 2)) return "Б";
    clean_buffer();
    MYASSERT(0, ERR_BAD_ANSWER, return 0)
}

EnumOfErrors AkinatorWork(BinaryTree_t* myTree, Stack_t* StackObject1, Stack_t* StackObject2, const char* file_database)
{
    MYASSERT(myTree, ERR_BAD_POINTER_TREE, return ERR_BAD_POINTER_TREE)
    MYASSERT(StackObject1, ERR_BAD_POINTER_STACK, return ERR_BAD_POINTER_STACK)
    MYASSERT(StackObject2, ERR_BAD_POINTER_STACK, return ERR_BAD_POINTER_STACK)

    FILE* FileWrite = OpenFile(file_database, "w");

    fprintf(stdout, CYAN "Привет я Акинатор!\nТы можешь загадать объект а я его отгадаю!\n" RESET);
    const char* result = SelectOption ();
    while (strncmp (result, "В", 2) && strncmp (result, "Б", 2))
    {
        if (!strncmp (result, "У", 2))
        {
            Guess(myTree);
        }
        if (!strncmp (result, "Д", 2))
        {
            Definition(myTree, StackObject1);
        }
        if (!strncmp (result, "С", 2))
        {
            CompareObjects(myTree, StackObject1, StackObject2);
        }
        result = SelectOption();
    }

    if (!strncmp(result, "В", 2))
    {
        fprintf(stdout, GREEN "Сохраняю базу данных...\n" RESET);
        TreePreOrder(myTree, FileWrite);
        fprintf(stdout, GREEN "Сохранение завершено!\n\n" RESET);
    }
    CloseFile (FileWrite);
    fprintf (stdout, GREEN "Всего хорошего!\n" RESET);
    return ERR_OK;
}

void clean_buffer (void)
{
    char buff[SIZE_OF_BUFFER] = {};
    fgets(buff, SIZE_OF_BUFFER, stdin);
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
    MYASSERT(NewNode, ERR_BAD_POINTER_NODE, return)

    CleanCharBuffer(NewNode->Value, SIZE_OF_VALUE);
    NewNode->Right  = NULL;
    NewNode->Left   = NULL;
    NewNode->Parent = NULL;
}

EnumOfErrors Definition (BinaryTree_t* myTree, Stack_t* StackObject)
{
    MYASSERT(myTree, ERR_BAD_POINTER_TREE, return ERR_BAD_POINTER_TREE)
    MYASSERT(StackObject, ERR_BAD_POINTER_STACK, return ERR_BAD_POINTER_STACK)

    char object_buffer[SIZE_OF_BUFFER] = {}; 
    int object_ch = 0;
    size_t counter_symbols = 0;
    fprintf(stdout, CYAN "Введите объект и я выведу вам его определение: " RESET);
    while ((object_ch = getchar()) != '\n') 
    {
        object_buffer[counter_symbols] = object_ch;
        counter_symbols++;
    }        
    //fprintf(stdout,"\n");

    Node_t* find_node = RecSearch (object_buffer, myTree->Root);    
    
    if (!find_node)
    {
        fprintf(stdout, YELLOW "Ваш объект не найден, убедитесь что вы правильно его ввели!\n");
        return ERR_OK;
    }
    //нашли узел теперь идем к корню и заполняем стек
    RecReturn (find_node, find_node->Parent, StackObject);
    //заполнили стек
    PrintDefinition (StackObject, myTree); 
    return ERR_OK;
}

Node_t* RecSearch (const Elem_t* Value, Node_t* CurrentNode)
{
    MYASSERT(CurrentNode, ERR_BAD_POINTER_NODE, return NULL)

    Node_t* find_node = NULL; 

    if ((!strcmp(Value, CurrentNode->Value))&&(CurrentNode->Left == NULL)&&(CurrentNode->Right == NULL))
    {
        find_node = CurrentNode;
    }
    if (find_node)
    {
        return find_node;
    }

    if (CurrentNode->Left)
    {
        find_node = RecSearch (Value, CurrentNode->Left);   
    }
    if (find_node)
    {
        return find_node;
    }

    if (CurrentNode->Right)
    {
        find_node = RecSearch (Value, CurrentNode->Right);   
    } 
    if (find_node)
    {
        return find_node;
    }
    return NULL;
}

EnumOfErrors RecReturn(Node_t* PreviousNode, Node_t* CurrentNode, Stack_t* StackObject)
{
    if (CurrentNode == NULL) return ERR_OK;

    if (CurrentNode->Left == PreviousNode) push (StackObject, 0);
    if (CurrentNode->Right == PreviousNode) push (StackObject, 1);

    return RecReturn(CurrentNode, CurrentNode->Parent, StackObject);
}

void PrintDefinition (Stack_t* StackObject, BinaryTree_t* myTree)
{
    fprintf(stdout, GREEN "Это: ");
    RecPrintDef(StackObject, myTree->Root);
    fprintf(stdout, "все!\n" RESET);   
}

void RecPrintDef (Stack_t* StackObject, Node_t* CurrentNode)
{
    if (StackObject->size == 0) return;

    int selection = -1;
    pop(StackObject, &selection);

    if (selection == 0) 
    {
        fprintf(stdout, "не %s, ", CurrentNode->Value);
        RecPrintDef (StackObject, CurrentNode->Left);
    }
    if (selection == 1)
    {
        fprintf(stdout, "%s, ", CurrentNode->Value);
        RecPrintDef (StackObject, CurrentNode->Right);
    }
}

EnumOfErrors CompareObjects (BinaryTree_t* myTree, Stack_t* StackObject1, Stack_t* StackObject2)
{
    fprintf(stdout, CYAN "Введите два объекта и я выведу их общие и различные черты!\n");

    char object1_buffer[SIZE_OF_BUFFER] = {}; 
    int object1_ch = 0;
    size_t counter1_symbols = 0;
    fprintf(stdout, CYAN "Введите первый объект: " RESET);
    while ((object1_ch = getchar()) != '\n') 
    {
        object1_buffer[counter1_symbols] = object1_ch;
        counter1_symbols++;
    }

    char object2_buffer[SIZE_OF_BUFFER] = {}; 
    int object2_ch = 0;
    size_t counter2_symbols = 0;
    fprintf(stdout, CYAN "Введите второй объект: " RESET);
    while ((object2_ch = getchar()) != '\n') 
    {
        object2_buffer[counter2_symbols] = object2_ch;
        counter2_symbols++;
    }

    Node_t* find_node1 = RecSearch (object1_buffer, myTree->Root);    
    Node_t* find_node2 = RecSearch (object2_buffer, myTree->Root);    
    if (!find_node1)
    {
        fprintf(stdout, YELLOW "Ваш первый объект не найден, убедитесь что вы правильно его ввели!\n");
        return ERR_OK;
    }
    if (!find_node2)
    {
        fprintf(stdout, YELLOW "Ваш второй объект не найден, убедитесь что вы правильно его ввели!\n");
        return ERR_OK;
    }
    //нашли узелы теперь идем к корню и заполняем стеки
    RecReturn (find_node1, find_node1->Parent, StackObject1);
    RecReturn (find_node2, find_node2->Parent, StackObject2);    
    //заполнили стеки
    PrintComparing (StackObject1, StackObject2, myTree); 
    return ERR_OK;
}

void PrintComparing (Stack_t* StackObject1, Stack_t* StackObject2, BinaryTree_t* myTree)
{
    fprintf(stdout, GREEN "Они оба: ");
    RecPrintComparing(StackObject1, StackObject2, myTree->Root);
    fprintf(stdout, "\n" RESET);  
}

void RecPrintComparing (Stack_t* StackObject1, Stack_t* StackObject2, Node_t* CurrentNode)
{
    int selection1 = -1;
    int selection2 = -1;
    pop(StackObject1, &selection1);
    pop(StackObject2, &selection2);

    if (selection1 == selection2)
    {
        if (selection1 == 0) 
        {
            fprintf(stdout, "не %s, ", CurrentNode->Value);
            RecPrintComparing (StackObject1, StackObject2,CurrentNode->Left);
        }
        if (selection1 == 1) 
        {
            fprintf(stdout, "%s, ", CurrentNode->Value);
            RecPrintComparing (StackObject1, StackObject2, CurrentNode->Right);
        }
    }

    if (selection1 != selection2)
    {
        fprintf(stdout, "\nНо они отличаются.\n");

        fprintf(stdout, "Первый объект отличается от второго тем, что он: ");
        if (selection1 == 0) 
        {
            fprintf(stdout, "не %s, ", CurrentNode->Value);
            RecPrintDef (StackObject1, CurrentNode->Left);
        }
        else
        {
            fprintf(stdout, "%s, ", CurrentNode->Value);
            RecPrintDef (StackObject1, CurrentNode->Right);
        }        
        fprintf(stdout, "\n");
        fprintf(stdout, "Второй объект отличается от первого тем, что он: ");
        if (selection2 == 0) 
        {
            fprintf(stdout, "не %s, ", CurrentNode->Value);
            RecPrintDef (StackObject2, CurrentNode->Left);
        }
        else
        {
            fprintf(stdout, "%s, ", CurrentNode->Value);
            RecPrintDef (StackObject2, CurrentNode->Right);
        }

    }
}

// EnumOfErrors UploadDataBase (BinaryTree_t* myTree, const char* file_database)
// {
//     FILE* FileRead = OpenFile (file_database);

//     size_t size_text = FileSize (FileRead);
//     size_t n_strings = 0;
//     size_t good_strings = 0;

//     char* text_buffer = NULL;
//     text_buffer = (char*) calloc (size_text + 1, sizeof (char));
//     MYASSERT(text_buffer, ERR_BAD_CALLOC, return ERR_BAD_CALLOC)
//     size_t result = fread (text_buffer, 1, size_text, FileRead);
// 	MYASSERT(result == (size_text - 1), ERR_BAD_IN_READ_FILE, return ERR_BAD_IN_READ_FILE)
// 	*(text_buffer + size_text - 1) = '\0';

//     n_strings = 0;
// 	for (size_t i = 0; i < (size_text); i++)
//     {
//         if ((*(text_buffer + i) == '[') || (*(text_buffer + i) == '\0'))
//         {
//             n_strings += 1;
//         }
//     }
// 	Line* string_buffer = NULL;
// 	string_buffer = (Line*) calloc (n_strings + 1, sizeof (Line));
//     MYASSERT(string_buffer, ERR_BAD_CALLOC, return ERR_BAD_CALLOC)

//     size_t shift_buffer = 0;
//     size_t counter_string = 0;

//     while (*(text_buffer + shift_buffer) == '\0')
//     {
//         if (*(text_buffer + shift_buffer) == '[')
//         {
               
//             counter_string++;
//         }
//     }


//     CloseFile (FileRead);
//     return ERR_OK;   
// }

// size_t FileSize (FILE *file_text)
// {
//     MYASSERT(file_text, ERR_BAD_POINTER_FILE, return 0)
// 	struct stat st;
//     int fd = fileno (file_text); 
//     fstat (fd, &st);
//     size_t size_text = st.st_size;
// 	return size_text;
// }