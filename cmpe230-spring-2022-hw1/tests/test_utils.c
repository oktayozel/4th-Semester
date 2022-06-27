#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "../utils.h"


int main(){
    char *str = " \thela";
    str = stripLeft(str);
    assert(strcmp(str, "hela") == 0);

    str = "a = b";
    stringPair x =  split(str, '=');

    assert(strcmp(x.first, "a ") == 0);
    assert(strcmp(x.second, " b") == 0);

    char *str2 = "ekrem test";
    stringPair x2 = split(str2, '=');
    // printf("#%s#\n", x2.first);
    // printf("#%s#\n", x2.second);

    assert(x2.first == NULL);
    assert(strlen(x2.second) == 0);

    char *str3 = "a =  b";
    char *result = parseAssignment(str3);
    assert(strcmp(result, "assign(a ,  b);") == 0);

    char *str4 = "b+d*c";
    int result2 = findExpressionLength(str4);
    assert(result2 == (int)strlen("add(b,multiply(d,c))"));

    char *str5 = "b +d * c+ a";
    char *result3 = parseExpression(str5);
    printf("%s\n", result3);

    char *str6 = "F = b +d[1, 2] * c+ a";
    char *result4 = parseAssignment(str6);
    printf("%s\n", result4);

    str = "asfdjnkgdlmh of[] dfsgdh";
    int foundIndex = findIndex(str, '[');
    printf("%d\n", foundIndex);

    char *str7 = "choose(A[i,j]-4,1,1,tr(A))";

    // char *str7 = "a = matrix[1,2]";
    char *result5 = parseGetIndex(str7);
    printf("%s\n", result5);

    char *str8 = "a = matrix[1,2]";
    char *result6 = getStringBetween(str8, '[', ']');
    printf("getStringBetween:%s#\n", result6);


    char *str9 = "a     aa  btb sdf  dverasv      sjfhlmva 1";
    char *result7 = removeMultipleSpaces(str9);
    printf("removeMultipleSpaces:%s#\n", result7);



    return 0;
}