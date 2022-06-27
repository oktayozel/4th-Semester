#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "../stack_operations.h"
#include "../expr.h"


// i 6 x y * 5 - 4 1 i tr 0 Choose Choose 4 1 x 9 * tr 0 Choose +
int main(){

    ParserGraph *graph = createParserGraph();
    // tokenizeLine(graph, "choose(i,6,sqrt(x*y-5),choose(4,1, tr ( i ) , 0 )) + choose (4, 1, tr ( x*9 ) , 0 )");


    tokenizeLine(graph, "A[1, choose(i[2],(7)*((3)+5*2),4,5)]");
    // tokenizeLine(graph, "(2+3)*4");

  
    // tokenizeLine(graph, "choose(1, 2, 3, 4)");
    // tokenizeLine(graph, "7 + 8*9");
    int len = tokenLen(tokens);
    for(int i = 0; i < len; i++){
        printf("%s %s\n",tokenChars[i], enumToString(tokens[i]));
    }
    int last = parseExpression(0, len - 1);
    printf("PRINTING LIST\n");
    printList(&globalList);
    printf("PRINTING LIST\n");

    // for(int i = 0; i < len; i++){
    //     printf("%s %s\n",tokenChars[i], enumToString(tokens[i]));
    // }
    evaluateList(&globalList);
    return 0;
    // printf("BASLADI\n");
    // struct List* temp =    createList();

    // // assert(temp != NULL);
    // assert(sizeOfList(temp) == 0);

    // struct Variable var;
    // var.name = "5";
    // var.feature = NUM;
    // // var.variable_type = -1;
    // var.dim1=-1;
    // var.dim1=-1;
    // // var.next = NULL;


    // // assert(sizeOfList(temp) == 1);


    // struct Variable var1;
    // var1.name = "+";
    // var1.feature = OP;
    // // var1.variable_type = 0;
    // var1.dim1=-1;
    // var1. dim1=-1;
    // // var1.next = NULL;


    // // assert(sizeOfList(temp) == 2);

    // struct Variable var2;
    // var2.name = "6";
    // var2.feature = NUM;
    // // var2.variable_type = -1;
    // var2.dim1=-1;
    // var2.dim1=-1;
    // // var2.next = NULL;

    // addToList(temp, &var);
    // addToList(temp, &var2);
    // addToList(temp, &var1);


    // assert(sizeOfList(temp) == 3);
    // printf("sizeOfList: %d\n", sizeOfList(temp));

    // evaluateList(temp);
    // printf("BITTI\n");
    



    return 0;
}