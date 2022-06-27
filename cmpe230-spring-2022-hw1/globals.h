#ifndef _globals_h
#define _globals_h

#define MAX_VARS 100005
#define MAX_CHAR_GRAPH_SIZE 256

#include "enums.h"

char *output_file_name;
FILE *out;

const int DEBUG = 0;

int lastNodeId = 0;
typedef struct ParserNode {
    int nodeId;
    struct ParserNode *edges[MAX_CHAR_GRAPH_SIZE];
    char edgeChars[MAX_CHAR_GRAPH_SIZE];
    int edgeCount;
    int tokenType;
} ParserNode;

typedef struct ParserGraph {
    struct ParserNode *root;
} ParserGraph;

typedef struct Variable {
    char *name;
    int feature;
    // int variable_type;
    int dim1,dim2;
}Variable;
// #include "dictionary.h"

typedef struct StackNode{
    struct Variable* data;
    struct StackNode* prev;
}StackNode;

typedef struct ListNode{
    struct Variable* data;
    struct ListNode* next;
} ListNode;

typedef struct Stack {
    struct StackNode* top;
    int size;
} Stack;

typedef struct List {
    ListNode* start;
    ListNode* end;
    int size;
} List;
List globalList;



int lineCount = 0;
int nonDecleration = 0;

int CREATING_SCALAR_TOKENS[] = {SCALAR, IDENTIFIER, -1};
int CREATING_VECTOR_TOKENS[] = {VECTOR, IDENTIFIER, LEFT_BRACKET, NUMBER, RIGHT_BRACKET, -1};
int CREATING_MATRIX_TOKENS[] = {MATRIX, IDENTIFIER, LEFT_BRACKET, NUMBER, COMMA, NUMBER, RIGHT_BRACKET, -1};
int CONSTANT_ASSIGNMENT_TOKENS[] = {IDENTIFIER, ASSIGNMENT, LEFT_BRACE, LIST_OF_NUMBERS, RIGHT_BRACE, -1};
int EXPRESSION_ASSIGNMENT_TOKENS[] = {IDENTIFIER, ASSIGNMENT, EXPRESSION, -1};
int PRINT_SCALAR_TOKENS[] = {PRINT, LEFT_PARENTHESIS, EXPRESSION, RIGHT_PARENTHESIS, -1};
int PRINT_VECTOR_TOKENS[] = {PRINT, LEFT_PARENTHESIS, EXPRESSION, RIGHT_PARENTHESIS, -1};
// int PRINT_TOKENS[] = {PRINT, LEFT_PARENTHESIS, IDENTIFIER, RIGHT_PARENTHESIS, -1};
int PRINTSEP_TOKENS[] = {PRINTSEP, LEFT_PARENTHESIS, RIGHT_PARENTHESIS, -1};
int FOR_LOOP_END_TOKENS[] = {RIGHT_BRACE, -1};


int tokens[1024];
char tokenChars[1024][1024];


Variable *dictVars[MAX_VARS];
int dictSize = 0;

#endif