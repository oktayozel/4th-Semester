#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "../dictionary.h"

int main(){

    printf("DICTIONARY TEST\n");

    struct Dictionary* temp = createDictionary();

    printf("DICTIONARY HAS BEEN CREATED SUCCESFULLY\n");

    
    assert(temp != NULL);
    assert(isEmpty(temp) == 1);
    
    add(temp, "AAAA1111",1);
    printf("START NODE HAS BEEN ADDED SUCCESFULLY\n");

    
    assert(isEmpty(temp) == 0);
    assert(query(temp,  "AAAA1111") == 1);
    
    printf("FIRST QUERY HAS BEEN PASSED SUCCESFULLY\n");

    //printf("%d\n",isEmpty(temp));
    
    add(temp, "BBBBB22222", 2);
    //printf("%d\n",query(temp,"BBBBB22222"));

    
    assert(isEmpty(temp) == 0);
    assert(query(temp, "BBBBB22222" ) == 2);
    printf("SECOND QUERY HAS BEEN PASSED SUCCESFULLY\n");

    add(temp, "CCCCCC33333" , 3 );

    assert(isEmpty(temp) == 0);
    assert(query(temp, "CCCCCC33333") == 3 );
    
    printf("THIRD QUERY HAS BEEN PASSED SUCCESFULLY\n");


    //printf("%d\n", query(temp, "CCCCCC33333"));
    

    return 0;
}