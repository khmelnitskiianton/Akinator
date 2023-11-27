#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "colors.h"
#include "tree.h"
#include "stack_main.h"
#include "log.h"
#include "myassert.h"
#include "verificator.h"
#include "func.h"
#include "stack_base.h"
#include "stack_support.h"
#include "festival.h"

static FILE* ShowGraphFile = NULL;
static size_t position = 0;

void TreeCtor (BinaryTree_t* myTree)
{
    MYASSERT(myTree, ERR_BAD_POINTER_TREE, return)

    myTree->Root = NULL;
    myTree->Size = 0;
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
    if (!CurrentNode) {fprintf(filestream, " nil "); return;}
    fprintf(filestream, "(");

    fprintf(filestream, " [" SPECIFIER "] ", CurrentNode->Value);
    PrintPreNode(CurrentNode->Left, filestream);
    PrintPreNode(CurrentNode->Right, filestream);

    fprintf(filestream, ")");
}

void PrintInNode (Node_t* CurrentNode, FILE* filestream)
{
    if (!CurrentNode) {fprintf(filestream, " nil "); return;}
    fprintf(filestream, "(");

    PrintInNode(CurrentNode->Left, filestream);
    fprintf(filestream, " [" SPECIFIER "] ", CurrentNode->Value);
    PrintInNode(CurrentNode->Right, filestream);

    fprintf(filestream, ")");
}

void PrintPostNode (Node_t* CurrentNode, FILE* filestream)
{
    if (!CurrentNode) {fprintf(filestream, " nil "); return;}
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
ON_FESTIVAL( 
    system(ECHO "\"Давай я попробую отгадать\"" FESTIVAL);
)
    EnumOfErrors result = RecGuess(myTree->Root, myTree);
    PrintLogTree (myTree);
    return result;
}


EnumOfErrors RecGuess (Node_t* CurrentNode, BinaryTree_t* myTree)
{
    //Определяем перед нами объект или определение? У определения два узла: нн или объект или определение
    if (CurrentNode->Left && CurrentNode->Right)    //если определение
    {
        fprintf(stdout, CYAN "Это %s?\n" RESET, CurrentNode->Value);
ON_FESTIVAL(        
        char festival1_buffer[SIZE_OF_FESTIVAL] = {};
        snprintf(festival1_buffer, SIZE_OF_FESTIVAL, ECHO "\"Это %s?\"" FESTIVAL, CurrentNode->Value);
        system(festival1_buffer);
)
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
ON_FESTIVAL( 
        char festival2_buffer[SIZE_OF_FESTIVAL] = {};
        snprintf(festival2_buffer, SIZE_OF_FESTIVAL, ECHO "\"Это %s?\"" FESTIVAL, CurrentNode->Value);
        system(festival2_buffer);
)
        int choice = UserChoice();
        if (choice == 'Y')
        {
            fprintf(stdout, GREEN "Я же говорил!\n" RESET); //отгадали
    ON_FESTIVAL( 
            system(ECHO "\"Я же говорил!\"" FESTIVAL);
    )        
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
    ON_FESTIVAL( 
            system(ECHO "\"А кто же это был?\"" FESTIVAL);
    )
            int buff_ch = 0; 
            size_t counter_symbols = 0;
            
            while ((buff_ch = getchar()) != '\n')
            {
                MYASSERT(counter_symbols < SIZE_OF_BUFFER, ERR_OVERFLOW_VALUE, return ERR_OVERFLOW_VALUE)
                buffer[counter_symbols++] = buff_ch;
            }
            buff_ch = 0;
            counter_symbols = 0;

            MYASSERT((strlen(buffer) + 1) < SIZE_OF_VALUE, ERR_OVERFLOW_VALUE, return ERR_OVERFLOW_VALUE)
            strcpy(NewObjectNode->Value, buffer);     
            CleanCharBuffer(buffer, SIZE_OF_BUFFER);
            //отличие
            fprintf(stdout, CYAN "А чем %s отличается от %s ?\nЭто..." RESET, NewObjectNode->Value, CurrentNode->Value); //Создаем новое определение
    ON_FESTIVAL(         
            char festival3_buffer[2*SIZE_OF_FESTIVAL] = {};
            snprintf(festival3_buffer, 2*SIZE_OF_FESTIVAL, ECHO "\"А чем %s отличается от %s Это\"" FESTIVAL, NewObjectNode->Value, CurrentNode->Value);
            system(festival3_buffer);
    )        
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
    ON_FESTIVAL( 
            system(ECHO "\"Я запомнил!\"" FESTIVAL);
    )
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
    ON_FESTIVAL( 
        system(ECHO "\"Ты шо крейзи Вводи Да или Нет\"" FESTIVAL);
    )
        CleanCharBuffer(buff, SIZE_OF_BUFFER);
        fgets(buff, SIZE_OF_BUFFER, stdin);
    }
    if (!strncmp(buff, "Н", 2) || !strncmp(buff, "н", 2)) return 'N';
    else return 'Y';
}

const char* SelectOption (void)
{
    fprintf(stdout, CYAN "Что ты хочешь?: [У]гадать, [Д]ать определение, [С]равнить объекты,\n\t\t[П]оказать дерево, [В]ыйти с сохранением или [Б]ез сохранения.\n" RESET);
ON_FESTIVAL(     
    system(ECHO "\"Что ты хочешь?\"" FESTIVAL);
)
    char buff[SIZE_OF_BUFFER] = {};
    fgets(buff, SIZE_OF_BUFFER, stdin);
    buff[strlen(buff)-1] = '\0';
    while (true)
    {
        if (RUS_TO_UPPER(buff, "У", "у")) return "У";
        if (RUS_TO_UPPER(buff, "Д", "д")) return "Д";
        if (RUS_TO_UPPER(buff, "С", "с")) return "С";
        if (RUS_TO_UPPER(buff, "П", "п")) return "П";
        if (RUS_TO_UPPER(buff, "В", "в")) return "В";
        if (RUS_TO_UPPER(buff, "Б", "б")) return "Б";
        fprintf (stdout,  RED "Я не понял команду :(\n" RESET);
    ON_FESTIVAL(
        system(ECHO "Я не понял команду" FESTIVAL);
    )
        CleanCharBuffer(buff, SIZE_OF_BUFFER);
        fgets(buff, SIZE_OF_BUFFER, stdin);
    }
    MYASSERT(0, ERR_BAD_ANSWER, return 0)
    return NULL;
}

EnumOfErrors AkinatorWork(BinaryTree_t* myTree, Stack_t* StackObject1, Stack_t* StackObject2, const char* file_database)
{
    MYASSERT(myTree, ERR_BAD_POINTER_TREE, return ERR_BAD_POINTER_TREE)
    MYASSERT(StackObject1, ERR_BAD_POINTER_STACK, return ERR_BAD_POINTER_STACK)
    MYASSERT(StackObject2, ERR_BAD_POINTER_STACK, return ERR_BAD_POINTER_STACK)

    if (!myTree->Root)  //если бд пустая то добавляем неизвестно кто
    {
        Node_t* FirstNode = (Node_t*) calloc (1,sizeof (Node_t));
        InitNode (FirstNode);
        MYASSERT(FirstNode, ERR_BAD_CALLOC, return ERR_BAD_CALLOC)
        strcpy(FirstNode->Value, UNKNOWN_OBJECT);
        myTree->Root = FirstNode;
        myTree->Size++;
    }

    fprintf(stdout, CYAN "Привет я Акинатор!\nТы можешь загадать объект а я его отгадаю!\n" RESET);
ON_FESTIVAL( 
    system(ECHO "\"Привет я Акинатор Ты можешь загадать объект а я его отгадаю!\"" FESTIVAL);
)    
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
        if (!strncmp (result, "П", 2))
        {
            ShowTree(myTree);
        }
        result = SelectOption();
    }

    if (!strncmp(result, "В", 2))
    {
        FILE* FileWrite = OpenFile(file_database, "w");
        fprintf(stdout, GREEN "Сохраняю базу данных...\n" RESET);
    ON_FESTIVAL(     
        system(ECHO "\"Сохраняю базу данных\"" FESTIVAL);    
    )    
        TreePreOrder(myTree, FileWrite);
        fprintf(stdout, GREEN "Сохранение завершено!\n\n" RESET);
    ON_FESTIVAL( 
        system(ECHO "\"Сохранение завершено!\"" FESTIVAL);
    )    
        CloseFile (FileWrite);
    }
    fprintf (stdout, GREEN "Всего хорошего!\n" RESET);
ON_FESTIVAL( 
    system(ECHO "\"Всего хорошего\"" FESTIVAL); 
)    
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
ON_FESTIVAL( 
    system(ECHO "\"Введите объект и я выведу вам его определение\"" FESTIVAL); 
)
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
    ON_FESTIVAL(     
        system(ECHO "\"Ваш объект не найден убедитесь что вы правильно его ввели\"" FESTIVAL);
    )
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
ON_FESTIVAL(
    system(ECHO "\"Это\"" FESTIVAL);
)
    RecPrintDef(StackObject, myTree->Root);
    fprintf(stdout, "всё!\n" RESET);
ON_FESTIVAL(  
    system(ECHO "\"всё\"" FESTIVAL);   
)
}

void RecPrintDef (Stack_t* StackObject, Node_t* CurrentNode)
{
    if (StackObject->size == 0) return;

    int selection = -1;
    pop(StackObject, &selection);

    if (selection == 0) 
    {
        fprintf(stdout, "не %s, ", CurrentNode->Value);
    ON_FESTIVAL(
        char festival1_buffer[SIZE_OF_FESTIVAL] = {};
        snprintf(festival1_buffer, SIZE_OF_FESTIVAL, ECHO "\"не %s\"" FESTIVAL, CurrentNode->Value);
        system(festival1_buffer);
    )
        RecPrintDef (StackObject, CurrentNode->Left);
    }
    if (selection == 1)
    {
        fprintf(stdout, "%s, ", CurrentNode->Value);
    ON_FESTIVAL(
        char festival2_buffer[SIZE_OF_FESTIVAL] = {};
        snprintf(festival2_buffer, SIZE_OF_FESTIVAL, ECHO "\"%s\"" FESTIVAL, CurrentNode->Value);
        system(festival2_buffer);
    )    
        RecPrintDef (StackObject, CurrentNode->Right);
    }
}

EnumOfErrors CompareObjects (BinaryTree_t* myTree, Stack_t* StackObject1, Stack_t* StackObject2)
{
    fprintf(stdout, CYAN "Введите два объекта и я выведу их общие и различные черты!\n");
ON_FESTIVAL(
    system(ECHO "\"Введите два объекта и я выведу их общие и различные черты!\"" FESTIVAL);
)
    char object1_buffer[SIZE_OF_BUFFER] = {}; 
    int object1_ch = 0;
    size_t counter1_symbols = 0;
    fprintf(stdout, CYAN "Введите первый объект: " RESET);
ON_FESTIVAL(
    system(ECHO "\"Введите первый объект\"" FESTIVAL);
)
    while ((object1_ch = getchar()) != '\n') 
    {
        object1_buffer[counter1_symbols] = object1_ch;
        counter1_symbols++;
    }

    char object2_buffer[SIZE_OF_BUFFER] = {}; 
    int object2_ch = 0;
    size_t counter2_symbols = 0;
    fprintf(stdout, CYAN "Введите второй объект: " RESET);
ON_FESTIVAL(
    system(ECHO "\"Введите второй объект\"" FESTIVAL);
)
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
    ON_FESTIVAL(
        system(ECHO "\"Ваш первый объект не найден убедитесь что вы правильно его ввели!\"" FESTIVAL);
    )
        return ERR_OK;
    }
    if (!find_node2)
    {
        fprintf(stdout, YELLOW "Ваш второй объект не найден, убедитесь что вы правильно его ввели!\n");
    ON_FESTIVAL(
        system(ECHO "\"Ваш второй объект не найден убедитесь что вы правильно его ввели!\"" FESTIVAL);
    )
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
ON_FESTIVAL(
    system(ECHO "\"Они оба\"" FESTIVAL);
)
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
        ON_FESTIVAL(
            char festival_buffer[SIZE_OF_FESTIVAL] = {};
            snprintf(festival_buffer, SIZE_OF_FESTIVAL, ECHO "\"не %s\"" FESTIVAL, CurrentNode->Value);
            system(festival_buffer);
        )
            RecPrintComparing (StackObject1, StackObject2,CurrentNode->Left);
        }
        if (selection1 == 1) 
        {
            fprintf(stdout, "%s, ", CurrentNode->Value);
        ON_FESTIVAL(
            char festival_buffer[SIZE_OF_FESTIVAL] = {};
            snprintf(festival_buffer, SIZE_OF_FESTIVAL, ECHO "\"%s\"" FESTIVAL, CurrentNode->Value);
            system(festival_buffer);
        )
            RecPrintComparing (StackObject1, StackObject2, CurrentNode->Right);
        }
    }

    if (selection1 != selection2)
    {
        fprintf(stdout, "\nНо они отличаются.\n");
    ON_FESTIVAL(
        system(ECHO "\"Но они отличаются\"" FESTIVAL);
    )
        fprintf(stdout, "Первый объект отличается от второго тем, что он: ");
    ON_FESTIVAL(
        system(ECHO "\"Первый объект отличается от второго тем что он\"" FESTIVAL);
    )
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
    ON_FESTIVAL(
        system(ECHO "\"Второй объект отличается от первого тем что он\"" FESTIVAL);
    )
        if (selection2 == 0) 
        {
            fprintf(stdout, "не %s, ", CurrentNode->Value);
        ON_FESTIVAL(
            char festival_buffer[SIZE_OF_FESTIVAL] = {};
            snprintf(festival_buffer, SIZE_OF_FESTIVAL, ECHO "\"не %s\"" FESTIVAL, CurrentNode->Value);
            system(festival_buffer);
        )
            RecPrintDef (StackObject2, CurrentNode->Left);
        }
        else
        {
            fprintf(stdout, "%s, ", CurrentNode->Value);
        ON_FESTIVAL(
            char festival_buffer[SIZE_OF_FESTIVAL] = {};
            snprintf(festival_buffer, SIZE_OF_FESTIVAL, ECHO "\"%s\"" FESTIVAL, CurrentNode->Value);
            system(festival_buffer);
        )
            RecPrintDef (StackObject2, CurrentNode->Right);
        }

    }
}

EnumOfErrors UploadDataBase (BinaryTree_t* myTree, const char* file_database)
{
    MYASSERT(myTree, ERR_BAD_POINTER_TREE, return ERR_BAD_POINTER_TREE)
    MYASSERT(file_database, ERR_WHAT_FILE_OF_DATA, return ERR_WHAT_FILE_OF_DATA)

    FILE* FileRead = OpenFile (file_database, "r");

    size_t size_text = FileSize (FileRead);

    char* text_buffer = NULL;
    text_buffer = (char*) calloc (size_text, sizeof (char));
    MYASSERT(text_buffer, ERR_BAD_CALLOC, return ERR_BAD_CALLOC)
    size_t result_size = fread (text_buffer, 1, size_text, FileRead);

    MYASSERT(result_size == size_text, ERR_BAD_FREAD, return ERR_BAD_FREAD);

    if (*text_buffer == '\0') 
    {
        fprintf(stdout, YELLOW "База данных пуста!\n" RESET);
    ON_FESTIVAL(
        system(ECHO "\"База данных пуста\"" FESTIVAL);
    )
        return ERR_OK;
    }

	*(text_buffer + size_text - 1) = '\0';
    CloseFile (FileRead);

    RecScanData(text_buffer, NULL, myTree->Root, myTree);

    free(text_buffer);

    return ERR_OK;   
}

size_t FileSize (FILE *file_text)
{
    MYASSERT(file_text, ERR_BAD_POINTER_FILE, return 0)
	struct stat st;
    int fd = fileno (file_text); 
    fstat (fd, &st);
    size_t size_text = (size_t) st.st_size;
	return size_text;
}

EnumOfErrors RecScanData(const char* text_buffer, Node_t** ResNode, Node_t* CurrentNode, BinaryTree_t* myTree)
{
    position = SkipSpaces (position, text_buffer); //пропускаем пробелы
    if (!strncmp(text_buffer + position, "nil", 3))
    {
        //дальше поддерево - пустота
        position = position + 3;
        position = SkipSpaces (position, text_buffer);
        return ERR_OK;
    }

    //нашли открывающуюся скобочку
    if (*(text_buffer + position) == '(')
    {
        position++;
        position = SkipSpaces (position, text_buffer); //пропускаем пробелы
        //пытаемся считать значение
        MYASSERT(*(text_buffer + position) == '[', ERR_NO_OPEN_BRACKET_OBJ, return ERR_NO_OPEN_BRACKET_OBJ)
        position++; //было на [ станет на первой букве
        
        char object_buffer[SIZE_OF_BUFFER] = {};
        Node_t* NewNode = (Node_t*) calloc (1, sizeof (Node_t));        //создали новый объект
        MYASSERT(NewNode, ERR_BAD_CALLOC, return ERR_BAD_CALLOC)
        InitNode(NewNode);
        myTree->Size++;

        size_t shift = 0;
        while (*(text_buffer + position) != ']') 
        {
            *(object_buffer + shift) = *(text_buffer + position);
            shift++;
            position++;
        }
        MYASSERT((strlen(object_buffer) + 1) < SIZE_OF_VALUE, ERR_OVERFLOW_VALUE, return ERR_OVERFLOW_VALUE)
        strcpy(NewNode->Value, object_buffer);     
        CleanCharBuffer(object_buffer, SIZE_OF_BUFFER);
        position++;
        position = SkipSpaces (position, text_buffer);
        //считали значение
        NewNode->Parent = CurrentNode;//подвязка от предыдущего
        if (CurrentNode)//проверка на корень
        {
            *ResNode = NewNode;
        }
        else
        {
            myTree->Root = NewNode;
        }
        //Далее рекурсивно идем дальше
        EnumOfErrors result = ERR_OK;   
        result = RecScanData(text_buffer, &NewNode->Left, NewNode, myTree);
        MYASSERT(result == ERR_OK, ERR_BAD_REC_SCAN, return ERR_BAD_REC_SCAN);
        result = RecScanData(text_buffer, &NewNode->Right, NewNode, myTree);
        MYASSERT(result == ERR_OK, ERR_BAD_REC_SCAN, return ERR_BAD_REC_SCAN);
        position = SkipSpaces (position, text_buffer);
        if (*(text_buffer + position) == ')')
        {
            position++;
            return ERR_OK;
        }
        else
        {
            MYASSERT(0, ERR_NO_CLOSE_BRACKET_NODE, return ERR_NO_CLOSE_BRACKET_NODE)
        }
    }
    else
    {
        MYASSERT(0, ERR_NO_OPEN_BRACKET_NODE, return ERR_NO_OPEN_BRACKET_NODE)
    }
}

size_t SkipSpaces(size_t current_position, const char* text_buffer)
{
    size_t end_position = current_position;
    MYASSERT(text_buffer, ERR_BAD_POINTER_DATA, return 0)
    while (*(text_buffer + end_position) == ' ') { end_position++; }
    return end_position;
}

void ShowTree (BinaryTree_t* myTree)
{
    system("touch " SHOW_GRAPH_FILE);

    ShowGraphFile = OpenFile(SHOW_GRAPH_FILE, "w");

    fprintf (ShowGraphFile, "digraph G{\n"
                            "rankdir = TB;\n"
                            "bgcolor = \"" FILL_BACK_COLOR "\";\n"
                            "node[color = \"" COLOR_FRAME "\", fontsize = 10];\n"
                            "edge[color = \"" COLOR_FRAME "\", fontsize = 15];\n"

                            "subgraph cluster_list {\n"
                            "\tnode[style = filled, fontsize = 10];\n"
                            "\tedge[color = \"" COLOR_FRAME "\", fontsize = 15];\n"
                            "\tstyle = \"dashed\";\n"
                            "\tcolor = \"" COLOR_STIPPLE "\";\n");

    if (!(myTree->Root)) 
    {
        return;
    }
    else
    {
        ShowWriteNode (myTree->Root);
    }
    fprintf (ShowGraphFile,     "}\n");
    fprintf (ShowGraphFile,     "}\n");
    CloseFile(ShowGraphFile);

    system ("dot " SHOW_GRAPH_FILE " -T " SHOW_GRAPH_TYPE " -o " SHOW_GRAPH_IMAGE);
    system ("rm " SHOW_GRAPH_FILE);

    fprintf(stdout, CYAN "Сделано!\n" RESET);
ON_FESTIVAL(
    system(ECHO "\"Сделано!\"" FESTIVAL);
)
}

void ShowWriteNode (Node_t* CurrentNode)
{
    fprintf (ShowGraphFile, "\tnode%p [shape = Mrecord, style = filled, fillcolor = \"" FILL_BACK_GRAPH "\", color = \"" COLOR_FRAME "\", label = \"" SPECIFIER "\"];\n"
            ,CurrentNode, CurrentNode->Value);
    
    if (CurrentNode->Left)
    {
        ShowWriteNode (CurrentNode->Left);
        fprintf (ShowGraphFile, "\tnode%p -> node%p [color = \"" COLOR_EDGE_GRAPH "\", label = \"нет\"]\n"
                , CurrentNode, CurrentNode->Left);
    }
    if (CurrentNode->Right)
    {
        ShowWriteNode (CurrentNode->Right);
        fprintf (ShowGraphFile, "\tnode%p -> node%p [color = \"" COLOR_EDGE_GRAPH "\", label = \"да\"]\n"
                , CurrentNode, CurrentNode->Right);
    }
}