#ifndef FUNC_H
#define FUNC_H

#include "errors_enum.h"

const size_t SIZE_OF_BUFFER = 500;
const int DONE = 1;
const int ERROR = 0;
const char UNKNOWN_OBJECT[] = "неизвестно кто"; 

#define SHOW_GRAPH_FILE "show_graph.dot"
#define SHOW_GRAPH_IMAGE "show_graph.png"
#define SHOW_GRAPH_TYPE "png"

#define RUS_TO_UPPER(str_buff, up_str, low_str) (!strncmp(str_buff, up_str, 2) || !strncmp(str_buff, low_str, 2))

//база
void         TreeCtor   (BinaryTree_t* myTree);
EnumOfErrors TreeDtor   (BinaryTree_t* myTree);
void         InitNode   (Node_t* NewNode);
void         RecFree    (Node_t* CurrentNode);
size_t       FileSize   (FILE *file_text);
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
EnumOfErrors UploadDataBase (BinaryTree_t* myTree, const char* file_database);
EnumOfErrors RecScanData(const char* text_buffer, Node_t** ResNode, Node_t* CurrentNode, BinaryTree_t* myTree);
size_t SkipSpaces(size_t position, const char* text_buffer);
//--------------------------------------------------------------------------------------

//Показать дерево
void ShowTree (BinaryTree_t* myTree);
void ShowWriteNode (Node_t* CurrentNode);
//use openfile closefile 
//---------------------------------------------------------------------------------------

#endif