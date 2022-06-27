#ifndef _dictionary_h
#define _dictionary_h
#include <string.h>
#include "stack_operations.h"
#include "globals.h"
#include "utils.h"

void addToDict(Variable *v){
    dictVars[dictSize++] = v;
}

void addNewScalar(char *name){
    // Malloc new Variable
    Variable *v = (Variable *)malloc(sizeof(Variable));
    // Copy name to new Variable
    v->name = (char *)malloc(sizeof(char) * (strlen(name) + 1));
    strcpy(v->name, name);
    // Set feature to SCALAR
    v->feature = SCA;
    // Set dim1 and dim2 to 1
    v->dim1 = 1;
    v->dim2 = 1;
    addToDict(v);
}

void addNewVector(char *name, int dim){
    // Malloc new Variable
    Variable *v = (Variable *)malloc(sizeof(Variable));
    // Copy name to new Variable
    v->name = (char *)malloc(sizeof(char) * (strlen(name) + 1));
    strcpy(v->name, name);
    // Set feature to VECTOR
    v->feature = VEC;
    // Set dim1 and dim2 to dim
    v->dim1 = dim;
    v->dim2 = 1;
    addToDict(v);
}

void addNewMatrix(char *name, int dim1, int dim2){
    // Malloc new Variable
    Variable *v = (Variable *)malloc(sizeof(Variable));
    // Copy name to new Variable
    v->name = (char *)malloc(sizeof(char) * (strlen(name) + 1));
    strcpy(v->name, name);
    // Set feature to MATRIX
    v->feature = MAT;
    // Set dim1 and dim2 to dim1 and dim2
    v->dim1 = dim1;
    v->dim2 = dim2;
    addToDict(v);
}

Variable *getFromDict(char *name){
    for(int i = 0; i < dictSize; i++){
        if(strcmp(dictVars[i]->name, name) == 0){
            return dictVars[i];
        }
    }
    return NULL;
}

bool isExist(char *name){
    for(int i = 0; i < dictSize; i++){
        if(strcmp(dictVars[i]->name, name) == 0){
            return true;
        }
    }
    return false;
}

void checkExistence(Variable *a){
    if(!isExist(a->name)){
        if(DEBUG){printf("Variable %s does not exist.\n", a->name);}
        raiseError();
    }
}


#endif