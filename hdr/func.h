#ifndef FUNC_H
#define FUNC_H

#include "errors_enum.h"

const size_t SIZE_OF_BUFFER = 3000;
const int DONE = 1;
const int ERROR = 0;
const char UNKNOWN_OBJECT[] = "неизвестно кто"; 

struct Line {
    size_t len;

    char* start_line;
};

//база
void         TreeCtor   (BinaryTree_t* myTree);
EnumOfErrors TreeDtor   (BinaryTree_t* myTree);
void         InitNode   (Node_t* NewNode);
void         RecFree    (Node_t* CurrentNode);
//size_t       FileSize   (FILE *file_text);
//---------------------------------------------

//Интерфейс
const char* SelectOption (void);
EnumOfErrors AkinatorWork(BinaryTree_t* myTree, Stack_t* StackObject1, Stack_t* StackObject2, const char* file_database);
//--------------------------------------------------------------------------------------------

//угадайка
EnumOfErrors Guess (BinaryTree_t* myTree);
EnumOfErrors RecGuess (Node_t* CurrentNode, BinaryTree_t* myTree);
int UserChoice (void);
void clean_buffer (void);
void CleanCharBuffer(char* buffer, const size_t buffer_size);
//------------------------------------------------------------

// Дать определение и сравнить объекты
EnumOfErrors Definition (BinaryTree_t* myTree, Stack_t* StackObject);
Node_t* RecSearch (const Elem_t* Value, Node_t* CurrentNode);
EnumOfErrors RecReturn(Node_t* PreviousNode, Node_t* CurrentNode, Stack_t* StackObject);
void PrintDefinition (Stack_t* StackObject, BinaryTree_t* myTree);
void RecPrintDef (Stack_t* StackObject, Node_t* CurrentNode);
EnumOfErrors CompareObjects (BinaryTree_t* myTree, Stack_t* StackObject1, Stack_t* StackObject2);
void PrintComparing (Stack_t* StackObject1, Stack_t* StackObject2, BinaryTree_t* myTree);
void RecPrintComparing (Stack_t* StackObject1, Stack_t* StackObject2, Node_t* CurrentNode);
//-----------------------------------------------------------------------------------------------

//Сохранение базы данных
EnumOfErrors TreePreOrder  (BinaryTree_t* myTree, FILE* filestream);
EnumOfErrors TreeInOrder   (BinaryTree_t* myTree, FILE* filestream);
EnumOfErrors TreePostOrder (BinaryTree_t* myTree, FILE* filestream);
void PrintPreNode  (Node_t* CurrentNode, FILE* filestream);
void PrintInNode   (Node_t* CurrentNode, FILE* filestream);
void PrintPostNode (Node_t* CurrentNode, FILE* filestream);
//-------------------------------------------------------------------

//Подгрузка базы данных
//EnumOfErrors UploadDataBase (BinaryTree_t* myTree);

//------------------------------------------------------------

#endif