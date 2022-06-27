#ifndef _stack_operations_h
#define _stack_operations_h

#include <stdio.h>
#include <string.h>
#include "enums.h"
#include "globals.h"
#include "utils.h"


#include "dictionary.h"




void push(struct Stack* stack, struct Variable* data){
    struct StackNode* newNode = (struct StackNode*)malloc(sizeof(struct StackNode));
    newNode->data = data;
    newNode->prev = stack->top;
    stack->top = newNode;
    stack->size++;

    return;
}
struct Variable* top(struct Stack* stack){
    return stack->top->data;
}
void pop(struct Stack* stack){
    struct StackNode* temp = stack->top;
    stack->top = stack->top->prev;
    // stack->top = stack->top->next;
    free(temp);
    stack->size--;
}
int isEmpty(struct Stack* stack){
    return stack->size == 0;
}
struct Stack* createStack(){
    struct Stack* stack = (struct Stack*)malloc(sizeof(struct Stack));
    stack->top = NULL;
    stack->size = 0;
    return stack;
}
int sizeOfStack(struct Stack* stack){
    return stack->size;
}
int sizeOfList(struct List* list){
    return list->size;
}

struct List* createList(){
    struct List* list = (struct List*)malloc(sizeof(struct List));
    list->start = NULL;
    list->end = NULL;
    list->size = 0;
    return list;
}

void printList(struct List* list){
    ListNode* temp = list->start;
    while(temp != NULL){
        printf("%s\n", temp->data->name);
        temp = temp->next;
    }
}

void addToList( struct List* list , struct Variable* var ){
    // printf("Adding this var: %s %d\n",var->name, var->feature);
    struct ListNode* newNode = (struct ListNode*)malloc(sizeof(struct ListNode));
    newNode->data = var;
    newNode->next = NULL;
    if(list->size == 0){
        list->start = newNode;
        list->end = newNode;
        list->size = 1;
    }
    else{
        list->end->next = newNode;
        list->end = newNode;
        list->size++;
    }
}   


void addOperationToList(const char *name){
    Variable* newVar = (Variable*)malloc(sizeof(Variable));
    newVar->name = (char*)malloc(sizeof(char)*(strlen(name)+1));
    strcpy(newVar->name,name);
    newVar->feature = OP;
    newVar->dim1 = -1;
    newVar->dim2 = -1;
    addToList(&globalList, newVar);
}


void addNumberToList(char *name){
    Variable* newVar = (Variable*)malloc(sizeof(Variable));
    newVar->name = (char*)malloc(sizeof(char)*(strlen(name)+1));
    strcpy(newVar->name,name);
    newVar->feature = NUM;
    newVar->dim1 = 1;
    newVar->dim2 = 1;
    addToList(&globalList, newVar);
}
void addIdentifierToList(char *name){
    Variable* newVar = (Variable*)malloc(sizeof(Variable));
    newVar->name = (char*)malloc(sizeof(char)*(strlen(name)+1));
    Variable* temp = getFromDict(name);
    if(temp == NULL){
        if(DEBUG){printf("Variable %s not found\n", name);}
        raiseError();
    }

    strcpy(newVar->name,temp->name);
    newVar->feature = temp->feature;
    newVar->dim1 = temp->dim1;
    newVar->dim2 = temp->dim2;
    addToList(&globalList, newVar);
}

char *randString() {
    int length = 15;
    static char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";        
    char *randomString = NULL;

    if (length) {
        randomString = malloc(sizeof(char) * (length +1));

        if (randomString) {            
            for (int n = 0;n < length;n++) {            
                int key = rand() % (int)(sizeof(charset) -1);
                randomString[n] = charset[key];
            }

            randomString[length] = '\0';
        }
    }
    return randomString;
}

// void raiseError(){
//     printf("Error Occured\n");
//     exit(0);
// }

Variable* generateScalarFromNumber(Variable *var){
    if(var->feature != NUM){
        raiseError();
    }
    Variable *newVar = (Variable*)malloc(sizeof(Variable));
    newVar->name = randString();
    newVar->feature = SCA;
    newVar->dim1 = 1;
    newVar->dim2 = 1;
    fprintf(out, "Variable *_%s = generateScalarFromNumber(%s);\n", newVar->name, var->name);
    addNewScalar(newVar->name);
    return newVar;
}

Variable *multiplication(Variable* a, Variable* b){
    char *rndstr = randString();
    struct Variable* newNode = (struct Variable*)malloc(sizeof(struct Variable));
    newNode->name=rndstr;
    // newNode->feature= VARIABLE;
    if(a->feature == NUM){
        a = generateScalarFromNumber(a);
    }
    if(b->feature == NUM){
        b = generateScalarFromNumber(b);
    }
    if(a->feature == SCA && b->feature == SCA){
        newNode->feature = SCA;
        newNode->dim1 = a->dim1;
        newNode->dim2 = b->dim2;
        fprintf(out, "Variable *_%s = multiplication(_%s, _%s);\n", newNode->name, a->name, b->name);
        addNewScalar(newNode->name);
    }
    else if(a->dim2 == b->dim1){
        newNode->feature = MAT;
        newNode->dim1 = a->dim1;
        newNode->dim2 = b->dim2;
        fprintf(out, "Variable *_%s = multiplication(_%s, _%s);\n", newNode->name, a->name, b->name);
        addNewMatrix(newNode->name, newNode->dim1, newNode->dim2);
    }
    else if(a->feature == SCA && b->feature == MAT){
        newNode->feature = MAT;
        newNode->dim1 = b->dim1;
        newNode->dim2 = b->dim2;
        fprintf(out, "Variable *_%s = multiplication(_%s, _%s);\n", newNode->name, a->name, b->name);
        addNewMatrix(newNode->name, newNode->dim1, newNode->dim2);
    }
    else if(a->feature == MAT && b->feature == SCA){
        newNode->feature = MAT;
        newNode->dim1 = a->dim1;
        newNode->dim2 = a->dim2;
        fprintf(out, "Variable *_%s = multiplication(_%s, _%s);\n", newNode->name, a->name, b->name);
        addNewMatrix(newNode->name, newNode->dim1, newNode->dim2);
    }
    else{
        if(DEBUG){printf("Error: Cannot multiply %s and %s\n", a->name, b->name);}
        raiseError();
    }
    return newNode;
}

Variable *substraction(Variable* a, Variable* b){
    char *rndstr = randString();
    struct Variable* newNode = (struct Variable*)malloc(sizeof(struct Variable));
    newNode->name=rndstr;
    // newNode->feature= VARIABLE;
    if(a->feature == NUM){
        a = generateScalarFromNumber(a);
    }
    if(b->feature == NUM){
        b = generateScalarFromNumber(b);
    }
    if(a->feature == SCA && b->feature == SCA){
        newNode->feature = SCA;
        newNode->dim1 = a->dim1;
        newNode->dim2 = b->dim2;
        fprintf(out, "Variable *_%s = substraction(_%s, _%s);\n", newNode->name, a->name, b->name);
        addNewScalar(newNode->name);
    }
    else if(a->dim1 == b->dim1 && a->dim2 == b->dim2){
        newNode->feature = MAT;
        newNode->dim1 = a->dim1;
        newNode->dim2 = b->dim2;
        fprintf(out, "Variable *_%s = substraction(_%s, _%s);\n", newNode->name, a->name, b->name);
        addNewMatrix(newNode->name, newNode->dim1, newNode->dim2);
    }
    else{
        if(DEBUG){printf("Error: Cannot substract %s and %s\n", a->name, b->name);}
        raiseError();
    }
    return newNode;
}


Variable *addition(Variable* a, Variable* b){
    char *rndstr = randString();
    struct Variable* newNode = (struct Variable*)malloc(sizeof(struct Variable));
    newNode->name=rndstr;
    // newNode->feature= VARIABLE;
    if(a->feature == NUM){
        a = generateScalarFromNumber(a);
    }
    if(b->feature == NUM){
        b = generateScalarFromNumber(b);
    }
    if(a->feature == SCA && b->feature == SCA){
        newNode->feature = SCA;
        newNode->dim1 = a->dim1;
        newNode->dim2 = b->dim2;
        fprintf(out, "Variable *_%s = addition(_%s, _%s);\n", newNode->name, a->name, b->name);
        addNewScalar(newNode->name);
    }
    else if(a->dim1 == b->dim1 && a->dim2 == b->dim2){
        newNode->feature = MAT;
        newNode->dim1 = a->dim1;
        newNode->dim2 = b->dim2;
        fprintf(out, "Variable *_%s = addition(_%s, _%s);\n", newNode->name, a->name, b->name);
        addNewMatrix(newNode->name, newNode->dim1, newNode->dim2);
    }
    else{
        if(DEBUG){printf("Error: Cannot substract %s and %s\n", a->name, b->name);}
        raiseError();
    }
    return newNode;
}

// Variable* processVars(Variable* a, Variable* b, const char *op){
//     char *rndstr = randString();
//     struct Variable* newNode = (struct Variable*)malloc(sizeof(struct Variable));
//     newNode->name=rndstr;
//     // newNode->feature= VARIABLE;
//     if(a->feature == NUM){
//         a = generateScalarFromNumber(a);
//     }
//     if(b->feature == NUM){
//         b = generateScalarFromNumber(b);
//     }
//     fprintf(out, "Variable *%s = %s( %s , %s );\n",  rndstr,  op, a->name , b->name  );
//     return newNode;
// }

Variable* processTr(Variable *a){
    char *rndstr = randString();
    struct Variable* newNode = (struct Variable*)malloc(sizeof(struct Variable));
    newNode->name=rndstr;
    
    if(a->feature == NUM){
        a = generateScalarFromNumber(a);
    }
    if(a->feature == SCA){
        newNode->dim1 = a->dim1;
        newNode->dim2 = a->dim2;
        newNode->feature= SCA;
        // fprintf(out, "Variable *_%s = transpose(_%s);\n", newNode->name, a->name);
        addNewScalar(newNode->name);
    }
    else if(a->feature == MAT || a->feature==VEC){
        newNode->dim1 = a->dim2;
        newNode->dim2 = a->dim1;
        newNode->feature= MAT;
        addNewMatrix(newNode->name, newNode->dim1, newNode->dim2);
    }
    else {
        if(DEBUG){printf("Error: Cannot transpose %s\n", a->name);}
        raiseError();
    }
    fprintf(out, "Variable *_%s = transpose( _%s );\n",  rndstr,  a->name  );
    return newNode;
}

Variable* processChoose(Variable *a,Variable *b,Variable *c,Variable *d){
    char *rndstr = randString();
    struct Variable* newNode = (struct Variable*)malloc(sizeof(struct Variable));
    newNode->name=rndstr;
    newNode->feature= SCA;
    newNode->dim1 = 1;
    newNode->dim2 = 1;
    if(a->feature == NUM){
        a = generateScalarFromNumber(a);
    }
    if(b->feature == NUM){
        b = generateScalarFromNumber(b);
    }
    if(c->feature == NUM){
        c = generateScalarFromNumber(c);
    }
    if(d->feature == NUM){
        d = generateScalarFromNumber(d);
    }
    checkExistence(a);
    checkExistence(b);
    checkExistence(c);
    checkExistence(d);
    if(a->feature != SCA || b->feature != SCA || c->feature != SCA || d->feature != SCA){
        if(DEBUG){printf("Error: Cannot choose %s, %s, %s, %s\n", a->name, b->name, c->name, d->name);}
        raiseError();
    }
    fprintf(out, "Variable *_%s = choose( _%s, _%s, _%s, _%s );\n",  rndstr,  a->name, b->name, c->name, d->name  );
    addNewScalar(newNode->name);
    return newNode;
}

Variable* processSqrt(Variable *a){
    char *rndstr = randString();
    struct Variable* newNode = (struct Variable*)malloc(sizeof(struct Variable));
    newNode->name=rndstr;
    newNode->feature= SCA;
    if(a->feature == NUM){
        a = generateScalarFromNumber(a);
    }
    checkExistence(a);
    if(a->feature != SCA){
        if(DEBUG){printf("Error: Cannot sqrt %s\n", a->name);}
        raiseError();
    }
    newNode->dim1 = 1;
    newNode->dim2 = 1;

    fprintf(out, "Variable *_%s = squareroot( _%s );\n",  rndstr,  a->name  );
    addNewScalar(newNode->name);
    return newNode;
}

Variable* processGetDoubleIndex(Variable *a, Variable *b, Variable *c){
    char *rndstr = randString();
    struct Variable* newNode = (struct Variable*)malloc(sizeof(struct Variable));
    newNode->name=rndstr;
    newNode->feature= SCA;
    newNode->dim1 = 1;
    newNode->dim2 = 1;
    if(a->feature == NUM){
        a = generateScalarFromNumber(a);
    }
    if(b->feature == NUM){
        b = generateScalarFromNumber(b);
    }
    if(c->feature == NUM){
        c = generateScalarFromNumber(c);
    }
    checkExistence(a);
    checkExistence(b);
    checkExistence(c);
    if(a->feature != MAT || b->feature != SCA || c->feature != SCA){
        if(DEBUG){printf("Error: Cannot get double index %s, %s, %s\n", a->name, b->name, c->name);}
        raiseError();
    }
    fprintf(out, "Variable *_%s = getDoubleIndex( _%s, _%s, _%s );\n",  rndstr,  a->name, b->name, c->name  );
    addNewScalar(newNode->name);
    return newNode;
}

Variable* processGetSingleIndex(Variable *a, Variable *b){
    char *rndstr = randString();
    struct Variable* newNode = (struct Variable*)malloc(sizeof(struct Variable));
    newNode->name=rndstr;
    newNode->feature= SCA;
    newNode->dim1 = 1;
    newNode->dim2 = 1;

    if(a->feature == NUM){
        a = generateScalarFromNumber(a);
    }
    if(b->feature == NUM){
        b = generateScalarFromNumber(b);
    }
    checkExistence(a);
    checkExistence(b);
    if(a->feature != VEC || b->feature != SCA ){
        if(DEBUG){printf("Error: Cannot get double index %s, %s\n", a->name, b->name);}
        raiseError();
    }
    fprintf(out, "Variable *_%s = getSingleIndex( _%s, _%s );\n",  rndstr,  a->name, b->name  );
    addNewScalar(newNode->name);
    return newNode;
}

//TODO: NORMALLY RETURNS STRUCT VARIABLE BUT FOR NOW IT WILL BE VOID
Variable* evaluateList( struct List* list){
    // printf("EVALUATIONA GIRDI\n");
    struct Stack* stack = (struct Stack*)malloc(sizeof(struct Stack));
    // printf("A\n");
    stack->top = NULL;
    // printf("B\n");
    //stack->size = 0;
    // printf("STACK OLUSTURDU list size = %d\n",list->size);

    for(struct ListNode* ind = list->start; ind != NULL; ind=ind->next){
        Variable* var = ind->data;
    // for(int i = 0; i < 3; i++){

        
        // printf("%s %d\n",var->name, var->feature);
        

        if(var->feature == NUM){
            // printf("NUM in the list\n");
            push(stack, var);
        }
        if(var->feature == MAT || var->feature == VEC || var->feature == SCA){
            // printf("VARIABLE in the list\n");
            push(stack, var);
        }
        if(var->feature == OP){
            // printf("OP in the list\n");
            if(strcmp(var->name , "+")==0){
                Variable* a = top(stack);
                pop(stack);
                Variable* b = top(stack);
                pop(stack);
                Variable* c = addition(a,b);
                push(stack, c);

            }
            if(strcmp(var->name , "-")==0){
                Variable* a = top(stack);
                pop(stack);
                Variable* b = top(stack);
                pop(stack);
                Variable* c = substraction(b, a);
                push(stack, c);
            }   
            if(strcmp(var->name , "*")==0){
                Variable* a = top(stack);
                pop(stack);
                Variable* b = top(stack);
                pop(stack);
                Variable* c = multiplication(b, a);
                push(stack, c);
            }
            if(strcmp(var->name , "tr")==0){  
                Variable* a = top(stack);
                pop(stack);
                Variable* c = processTr(a);
                push(stack, c);
            }
            if(strcmp(var->name , "choose")==0){
                Variable* a = top(stack);
                pop(stack);
                Variable* b = top(stack);
                pop(stack);
                Variable* c = top(stack);
                pop(stack);
                Variable* d = top(stack);
                pop(stack);
                Variable* result = processChoose(d, c, b, a);
                push(stack, result);
            }
            if(strcmp(var->name , "sqrt")==0){
                Variable* a = top(stack);
                pop(stack);
                Variable* result = processSqrt(a);
                push(stack, result);
            }
            if(strcmp(var->name , "getSingleIndex")==0){
                Variable* a = top(stack);
                pop(stack);
                Variable* b = top(stack);
                pop(stack);
                Variable* c = processGetSingleIndex(a, b);
                push(stack, c);
            }
            if(strcmp(var->name , "getDoubleIndex")==0){
                Variable* a = top(stack);
                pop(stack);
                Variable* b = top(stack);
                pop(stack);
                Variable* c = top(stack);
                pop(stack);
                Variable* result = processGetDoubleIndex(a, c, b);
                push(stack, result);
            }

        }

    }
    // printf("stack size = %d\n", stack->size);
    // Print stack
    
    if(stack->size != 1){
        raiseError();
    }
    Variable* topp = top(stack);
    if(topp->feature == NUM){
        topp = generateScalarFromNumber(topp);
    }
    pop(stack);
    return topp;
    // printf("last stack size: %d\n", stack->size);

}

/*

choose(i,6,sqrt(x*y-5),choose(4,1, tr ( i ) , 0 )) + choose (4, 1, tr ( x*9 ) , 0 )
*/
#endif