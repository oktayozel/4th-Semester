#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "../stack.h"

int main(){
    struct Stack* temp = createStack();

    // assert(temp != NULL);
    assert(isEmpty(temp) == 1);
    assert(sizeOf(temp) == 0);

    push(temp, 5);

    assert(isEmpty(temp) == 0);
    assert(sizeOf(temp) == 1);
    assert(top(temp) == 5);

    push(temp, 6);

    assert(isEmpty(temp) == 0);
    assert(sizeOf(temp) == 2);
    assert(top(temp) == 6);

    push(temp, 7);

    assert(isEmpty(temp) == 0);
    assert(sizeOf(temp) == 3);
    assert(top(temp) == 7);

    pop(temp);

    assert(isEmpty(temp) == 0);
    assert(sizeOf(temp) == 2);
    assert(top(temp) == 6);

    pop(temp);

    assert(isEmpty(temp) == 0);
    assert(sizeOf(temp) == 1);
    assert(top(temp) == 5);

    pop(temp);

    assert(isEmpty(temp) == 1);
    assert(sizeOf(temp) == 0);

    return 0;
}