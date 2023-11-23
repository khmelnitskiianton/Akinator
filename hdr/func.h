#ifndef FUNC_H
#define FUNC_H

#include "errors_enum.h"

const size_t SIZE_OF_BUFFER = 3000;
const int DONE = 1;
const int ERROR = 0;

//база
void         TreeCtor   (BinaryTree_t* myTree);
EnumOfErrors TreeDtor   (BinaryTree_t* myTree);
void         InitNode   (Node_t* NewNode);
void         RecFree    (Node_t* CurrentNode);
//---------------------------------------------

EnumOfErrors TreeInsert (const char* LeftRight, Elem_t Value, Node_t* CurrentNode, BinaryTree_t* myTree);

//распечатка
EnumOfErrors TreePreOrder  (BinaryTree_t* myTree, FILE* filestream);
EnumOfErrors TreeInOrder   (BinaryTree_t* myTree, FILE* filestream);
EnumOfErrors TreePostOrder (BinaryTree_t* myTree, FILE* filestream);
void PrintPreNode  (Node_t* CurrentNode, FILE* filestream);
void PrintInNode   (Node_t* CurrentNode, FILE* filestream);
void PrintPostNode (Node_t* CurrentNode, FILE* filestream);
//------------------------------------------------------------------ 

//угадайка
EnumOfErrors Guess (BinaryTree_t* myTree);

EnumOfErrors RecGuess (Node_t* CurrentNode, BinaryTree_t* myTree);
int UserChoice (void);
void clean_buffer (void);
void CleanCharBuffer(char* buffer, const size_t buffer_size);
//------------------------------------------------------------

//Дать определение
// EnumOfErrors Definition (BinaryTree_t* myTree);

// EnumOfErrors RecSearch (Node_t* CurrentNode, BinaryTree_t* myTree);
//------------------------------------------------------------

//Сохранение базы данных

//------------------------------------------------------------

//Подгрузка базы данных

//------------------------------------------------------------

//Сравнить объекты

//------------------------------------------------------------

#endif