#ifndef LOG_H
#define LOG_H

#define DEBUG_LOG 1

#ifdef DEBUG_LOG
    #define PrintLogStart() \
            _PrintLogStart()
    #define PrintLogFinish() \
            _PrintLogFinish()
    #define PrintLogTree(myTree) \
            _PrintLogTree(myTree, __FILE__, __PRETTY_FUNCTION__, __LINE__)
#else
    #define PrintLogTree(myTree)
    #define PrintLogStart()
    #define PrintLogFinish()
#endif

void _PrintLogTree (BinaryTree_t* myTree, 
                    const char* file,  const char* function, const size_t line);

void  _PrintLogStart  (void);
void  _PrintLogFinish (void);
FILE* GenerateImage (void);
void  GenerateGraph  (BinaryTree_t* myTree);
void  WriteNode      (Node_t* CurrentNode);
void  WriteNullNode  (const char* Place, Node_t* CurrentNode);
void  WriteHead      (BinaryTree_t* myTree);
void  WriteTree      (BinaryTree_t* myTree);
FILE* OpenFile       (const char* file_text);
void  CloseFile      (FILE* file_text);

#define FOLDER_LOG "log"
#define FILE_LOG "log/log.html"
#define FILE_GRAPH "log/graph.dot"
#define TYPE_OF_IMAGE "svg"
const size_t SIZE_OF_COMMAND = 500;
const size_t SIZE_OF_POINTER = 13;

#define FILL_BACK_COLOR "#ff7f50"
#define FILL_BACK_GRAPH "#ffe4c4"
#define FILL_BACK_GRAPH_NULL "#BC8F8F"
#define COLOR_FRAME "#800000"
#define COLOR_STIPPLE "#800080"
#define COLOR_EDGE_GRAPH "#000000"
#define COLOR_EDGE_HEAD "#FFFFFF"

#endif