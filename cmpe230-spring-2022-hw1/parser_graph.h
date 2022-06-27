#ifndef _char_graph_h
#define _char_graph_h

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "enums.h"



int tokenLen(int *tokens){
	for(int i = 0; i < 1024; i++){
		if(tokens[i] == -1){
			return i;
		}
	}
	return 1024;
}



char *enumToString(int type){
    switch(type){
        case START:
            return "START";
        case IDENTIFIER:
            return "IDENTIFIER";
        case NUMBER:
            return "NUMBER";
        case ASSIGNMENT:
            return "ASSIGNMENT";
        case ADDITION:
            return "ADDITION";
        case SUBTRACTION:
            return "SUBTRACTION";
        case MULTIPLICATION:
            return "MULTIPLICATION";
        case LEFT_PARENTHESIS:
            return "LEFT_PARENTHESIS";
        case RIGHT_PARENTHESIS:
            return "RIGHT_PARENTHESIS";
        case LEFT_BRACKET:
            return "LEFT_BRACKET";
        case RIGHT_BRACKET:
            return "RIGHT_BRACKET";
        case LEFT_BRACE:
            return "LEFT_BRACE";
        case RIGHT_BRACE:
            return "RIGHT_BRACE";
        case COMMA:
            return "COMMA";
        case FOR:
            return "FOR";
        case PRINT:
            return "PRINT";
        case PRINTSEP:
            return "PRINTSEP";
        case SCALAR:
            return "SCALAR";
        case VECTOR:
            return "VECTOR";
        case MATRIX:
            return "MATRIX";
        case UNKNOWN:
            return "UNKNOWN";
        case IN:
            return "IN";
        case TWO_DOTS:
            return "TWO_DOTS";
        case END_OF_FILE:
            return "END_OF_FILE";
        case SQRT:
            return "SQRT";
        case CHOOSE:
            return "CHOOSE";
        case TR:
            return "TR";
        default:
            return "UNKNOWN";
    }
}



/**
 * @brief Depth-first search of the graph
 * @param graph The graph to search
 */
void dfs(ParserNode *node){
    ParserNode *next;

    for(int i = 0; i < node->edgeCount; i++){
        next = node->edges[i];
        printf("%d %d %c\n", node->nodeId, next->nodeId, node->edgeChars[i]);
        if(next->nodeId != node->nodeId){
            dfs(next);
        }
    }
}

/**
 * @brief Adds a new edge to the graph.
 * @param node The node to add the edge to.
 * @param child The child node to add.
 * @param edgeChar The character that represents the edge.
 */
void addEdge(ParserNode *node, ParserNode *child, char edgeChar){
    node->edges[node->edgeCount] = child;
    node->edgeChars[node->edgeCount] = edgeChar;
    node->edgeCount++;
}

/**
 * @brief Adds multiple edges to the graph.
 * @param node The node to add the edges to.
 * @param children The children nodes to add.
 * @param edgeCharsStart The start of the edge characters.
 * @param edgeCharsEnd The end of the edge characters.
 * 
 */
void addEdges(ParserNode *node, ParserNode *children, char edgeCharsStart, char edgeCharsEnd){
    for(int i = edgeCharsStart; i <= edgeCharsEnd; i++){
        addEdge(node, children, i);
    }
}

/**
 * @brief creates a new node for the graph
 * @param tokenType the token type of the node
 */
ParserNode *createNode(int tokenType) {
    ParserNode *node = (ParserNode *)malloc(sizeof(ParserNode));
    node->tokenType = tokenType;
    node->edgeCount = 0;
    node->nodeId = lastNodeId;
    lastNodeId++;
    return node;
}

/**
 * @brief adds a word to the graph
 */
void addKeyword(ParserNode *root, char *word, int tokenType, ParserNode *elseNode) {
    ParserNode *currentNode = root;
    int u = strlen(word);
    for(int i = 0; i < u; i++) {
        bool found = false;
        ParserNode *nextNode = NULL;
        // if(i < u - 1){
            nextNode = createNode(IDENTIFIER);
        // } else{
        //     // printf("Adding %c %s\n", word[i], enumToString(tokenType));
        //     nextNode = createNode(tokenType);
        // }
        for(int j = 0; j < currentNode->edgeCount; j++) {
            if(currentNode->edgeChars[j] == word[i]) {
                // currentNode->edges[j] = nextNode;
                currentNode = currentNode->edges[j];
                found = true;
                break;
            }
        }
        if(found) {
            continue;
        }
        
        addEdge(currentNode, nextNode, word[i]);
        if(i != 0){
            addEdges(currentNode, elseNode,'a', word[i] - 1);
            addEdges(currentNode, elseNode, word[i] + 1, 'z');
            addEdges(currentNode, elseNode, 'A', 'Z');
            addEdges(currentNode, elseNode, '0', '9');
            addEdge(currentNode, elseNode, '_');
        }
        currentNode = nextNode;
    }
    currentNode->tokenType = tokenType;
    addEdges(currentNode, elseNode,'a', 'z');
    addEdges(currentNode, elseNode, 'A', 'Z');
    addEdges(currentNode, elseNode, '0', '9');
    addEdge(currentNode, elseNode, '_');

}
/**
 * @brief Generates a new ParserGraph
 * 
 */
ParserGraph *createParserGraph() {
    ParserGraph *graph = (ParserGraph*)malloc(sizeof(ParserGraph));
    graph->root = createNode(START);

    ParserNode *numberNode = createNode(NUMBER);
    addEdges(graph->root, numberNode, '0', '9');
    addEdges(numberNode, numberNode, '0', '9');
    ParserNode *dotNode = createNode(NUMBER);
    addEdge(numberNode, dotNode, '.');
    ParserNode *decimalNumberNode = createNode(NUMBER);
    addEdges(dotNode, decimalNumberNode, '0', '9');
    addEdges(decimalNumberNode, decimalNumberNode, '0', '9');

    // graph->root->edgeChars[graph->root->edgeCount] = '\0';
    // printf("DEBUGGG #%s# %d\n", graph->root->edgeChars, graph->root->edgeCount);

    ParserNode *identifierNode = createNode(IDENTIFIER);


    addKeyword(graph->root, "sqrt", SQRT, identifierNode);
    addKeyword(graph->root, "scalar", SCALAR, identifierNode);
    addKeyword(graph->root, "for", FOR, identifierNode);
    addKeyword(graph->root, "vector", VECTOR, identifierNode);
    addKeyword(graph->root, "matrix", MATRIX, identifierNode);
    addKeyword(graph->root, "in", IN, identifierNode);
    addKeyword(graph->root, "choose", CHOOSE, identifierNode);
    addKeyword(graph->root, "tr", TR, identifierNode);
    addKeyword(graph->root, "printsep", PRINTSEP, identifierNode);
    addKeyword(graph->root, "print", PRINT, identifierNode);
  // addKeyword(graph->root, "", SQRT, identifierNode);

    // graph->root->edgeChars[graph->root->edgeCount] = '\0';
    // printf("DEBUGGG #%s# %d\n", graph->root->edgeChars, graph->root->edgeCount);
    // return graph;


    addEdges(graph->root, identifierNode, 'a', 'z');
    addEdges(graph->root, identifierNode, 'A', 'Z');
    addEdge(graph->root, identifierNode, '_');
    addEdges(identifierNode, identifierNode, 'a', 'z');
    addEdges(identifierNode, identifierNode, 'A', 'Z');
    addEdges(identifierNode, identifierNode, '0', '9');
    addEdge(identifierNode, identifierNode, '_');

    ParserNode *assignmentNode = createNode(ASSIGNMENT);
    addEdge(graph->root, assignmentNode, '=');

    ParserNode *additionNode = createNode(ADDITION);
    addEdge(graph->root, additionNode, '+');

    ParserNode *subtractionNode = createNode(SUBTRACTION);
    addEdge(graph->root, subtractionNode, '-');

    ParserNode *multiplicationNode = createNode(MULTIPLICATION);
    addEdge(graph->root, multiplicationNode, '*');

    ParserNode *leftParenthesisNode = createNode(LEFT_PARENTHESIS);
    addEdge(graph->root, leftParenthesisNode, '(');

    ParserNode *rightParenthesisNode = createNode(RIGHT_PARENTHESIS);
    addEdge(graph->root, rightParenthesisNode, ')');

    ParserNode *leftBracketNode = createNode(LEFT_BRACKET);
    addEdge(graph->root, leftBracketNode, '[');

    ParserNode *rightBracketNode = createNode(RIGHT_BRACKET);
    addEdge(graph->root, rightBracketNode, ']');

    ParserNode *leftBraceNode = createNode(LEFT_BRACE);
    addEdge(graph->root, leftBraceNode, '{');

    ParserNode *rightBraceNode = createNode(RIGHT_BRACE);
    addEdge(graph->root, rightBraceNode, '}');

    ParserNode *commaNode = createNode(COMMA);
    addEdge(graph->root, commaNode, ',');

    ParserNode *twoDotsNode = createNode(TWO_DOTS);
    addEdge(graph->root, twoDotsNode, ':');


    return graph;
}

void parseLine(ParserGraph *graph, char *line){
    // ParserGraph *graph = createParserGraph();
    int len = strlen(line);
    ParserNode *currentNode = graph->root;
    for(int i = 0; i <= len; i++){
        // currentNode->edgeChars[currentNode->edgeCount] = '\0';
        // printf("%s\n", currentNode->edgeChars);
        bool found = false;
        for(int j = 0; j < currentNode->edgeCount; j++){
            if(currentNode->edgeChars[j] == line[i]){
                currentNode = currentNode->edges[j];
                found = true;
                break;
            }
        }
        if(!found){
            if(currentNode == graph->root){
                continue;
            }
            printf(" %s\n", enumToString(currentNode->tokenType));
            currentNode = graph->root;
            i--;
        } else{
            printf("%c", line[i]);
        }
        // printf("%c %d %d\n", line[i] , currentNode->tokenType, found);
    }
}
#define MAX_TOKENS 1024

// int tokens[MAX_TOKENS];
// char tokenChars[MAX_TOKENS][MAX_TOKENS];


// void tokenizeLine(ParserGraph *graph, char *line, int *tokens, char **tokenChars){
void tokenizeLine(ParserGraph *graph, char *line){
    int lastToken = 0;
    int lastChar = 0;
    // ParserGraph *graph = createParserGraph();
    int len = strlen(line);
    ParserNode *currentNode = graph->root;
    for(int i = 0; i <= len; i++){
        for(int j = 0; j < currentNode->edgeCount; j++){
            // printf("%s to %c %s\n", enumToString(currentNode->tokenType), currentNode->edgeChars[j] , enumToString(currentNode->edges[j]->tokenType));
        }
        // currentNode->edgeChars[currentNode->edgeCount] = '\0';
        // printf("%s\n", currentNode->edgeChars);
        bool found = false;
        for(int j = 0; j < currentNode->edgeCount; j++){
            if(currentNode->edgeChars[j] == line[i]){
                currentNode = currentNode->edges[j];
                found = true;
                break;
            }
        }
        if(!found){
            if(currentNode == graph->root){
                continue;
            }
            tokenChars[lastToken][lastChar] = '\0';
            tokens[lastToken++] = currentNode->tokenType;
            lastChar = 0;
            // printf(" %s\n", enumToString(currentNode->tokenType));
            currentNode = graph->root;
            i--;
        } else{
            // add line[i] to tokenChars
            tokenChars[lastToken][lastChar++] = line[i];
            // printf("%c", line[i]);
        }
        // printf("%c %s %d\n", line[i] , enumToString(currentNode->tokenType), found);
    }
    tokens[lastToken] = -1;
}

#endif
