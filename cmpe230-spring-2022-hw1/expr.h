#ifndef _expr_h
#define _expr_h

#include "globals.h"
int factor(int, int, bool);
int term(int, int, bool);
int expr(int, int, bool);
int moreterms(int, int, bool);
int morefactors(int, int, bool);



int factor(int left, int right, bool flag){
    if(DEBUG){ printf("factor(%d, %d)->", left, right); for(int i = left; i <= right; i++) printf("%s ", tokenChars[i]); printf("\n"); }
    if(left > right)return -1;
    int tempLeft = 0;
	if(tokens[left] == LEFT_PARENTHESIS && (tempLeft = expr(left + 1, right, false)) != -1 && tokens[tempLeft] == RIGHT_PARENTHESIS ){
        if(flag){
            tempLeft = 0;
            tempLeft = expr(left + 1, right, true);
        }
        return tempLeft+1;
	}
    tempLeft = 0;
    if(
        tokens[left] == IDENTIFIER &&
        tokens[left + 1] == LEFT_BRACKET &&
        (tempLeft = expr(left + 2, right, false)) != -1 &&
        tokens[tempLeft] == COMMA &&
        (tempLeft = expr(tempLeft + 1, right, false)) != -1 &&
        tokens[tempLeft] == RIGHT_BRACKET
        )
        {
            if(flag){
                tempLeft = 0;
                tempLeft = expr(left + 2, right, true);
                tempLeft = expr(tempLeft + 1, right, true);
                addIdentifierToList(tokenChars[left]);
                addOperationToList("getDoubleIndex");
            }
            return tempLeft + 1;
        }
    tempLeft = 0;
    if(
        tokens[left] == IDENTIFIER &&
        tokens[left + 1] == LEFT_BRACKET &&
        (tempLeft = expr(left + 2, right, false)) != -1 &&
        tokens[tempLeft] == RIGHT_BRACKET
        )
        {
            if(flag){
                tempLeft = 0;
                tempLeft = expr(left + 2, right, true);
                addIdentifierToList(tokenChars[left]);
                addOperationToList("getSingleIndex");
            }
            return tempLeft + 1;
        }
    tempLeft = 0;
    if(
        tokens[left] == SQRT &&
        tokens[left + 1] == LEFT_PARENTHESIS &&
        (tempLeft = expr(left + 2, right, false)) != -1 &&
        tokens[tempLeft] == RIGHT_PARENTHESIS
        )
        {
            if(flag){
                tempLeft = 0;
                tempLeft = expr(left + 2, right, true);
                addOperationToList("sqrt");
            }
            return tempLeft + 1;
        }
    tempLeft = 0;
    if(
        tokens[left] == CHOOSE &&
        tokens[left + 1] == LEFT_PARENTHESIS &&
        (tempLeft = expr(left + 2, right, false)) != -1 &&
        tokens[tempLeft] == COMMA &&
        (tempLeft = expr(tempLeft + 1, right, false)) != -1 &&
        tokens[tempLeft] == COMMA &&
        (tempLeft = expr(tempLeft + 1, right, false)) != -1 &&
        tokens[tempLeft] == COMMA &&
        (tempLeft = expr(tempLeft + 1, right, false)) != -1 &&
        tokens[tempLeft] == RIGHT_PARENTHESIS
        )
        {
            if(flag){
                tempLeft = 0;
                tempLeft = expr(left + 2, right, true);
                tempLeft = expr(tempLeft + 1, right, true);
                tempLeft = expr(tempLeft + 1, right, true);
                tempLeft = expr(tempLeft + 1, right, true);
                addOperationToList("choose");
            }
            return tempLeft + 1;
        }
    tempLeft = 0;
    if(tokens[left] == IDENTIFIER){
        if(flag){
            addIdentifierToList(tokenChars[left]);
            // printf("%s ", tokenChars[left]);
        }
        // addIdentifierToList(tokenChars[left]);
        // printf("#%s# ",tokenChars[left]);
        return left + 1;
    }
    tempLeft = 0;
    if(tokens[left] == NUMBER){
        if(flag){
            addNumberToList(tokenChars[left]);
        }
        return left + 1;
    }
    tempLeft = 0;
    if(
        tokens[left] == TR &&
        tokens[left + 1] == LEFT_PARENTHESIS &&
        (tempLeft = expr(left + 2, right, false)) != -1 &&
        tokens[tempLeft] == RIGHT_PARENTHESIS
        )
        {
            if(flag){
                tempLeft = 0;
                tempLeft = expr(left + 2, right, true);
                addOperationToList("tr");
            }
            return tempLeft + 1;
        }
    return -1;
}

int moreterms(int left, int right, bool flag){
    if(DEBUG){ printf("moreterms(%d, %d)->", left, right); for(int i = left; i <= right; i++) printf("%s ", tokenChars[i]); printf("\n"); }
    if(left > right)return left;
    // if(DEBUG) printf("moreterms(%d, %d)\n", left, right);
    int tempLeft = 0;
    if( ((tokens[left] == ADDITION)) && ((tempLeft = term(left + 1, right, false)) != -1) && ((tempLeft = moreterms(tempLeft, right, false)) != -1)){
        if(flag){
            tempLeft = 0;
            tempLeft = term(left + 1, right, true);
            tempLeft = moreterms(tempLeft, right, true);
            addOperationToList("+");
        }
		return tempLeft;
	}
    tempLeft = 0;
    if( ((tokens[left] == SUBTRACTION)) && ((tempLeft = term(left + 1, right, false)) != -1) && ((tempLeft = moreterms(tempLeft, right, false)) != -1)){
        if(flag){
            tempLeft = 0;
            tempLeft = term(left + 1, right, true);
            addOperationToList("-");
            tempLeft = moreterms(tempLeft, right, true);
            
        }
		return tempLeft;
	}
    
	return left;
}

int morefactors(int left, int right, bool flag){
    if(DEBUG){ printf("morefactors(%d, %d)->", left, right); for(int i = left; i <= right; i++) printf("%s ", tokenChars[i]); printf("\n"); }
    if(left > right)return left;
    int tempLeft;
    if(left >= right)
        return left;
	if(tokens[left] == MULTIPLICATION && ((tempLeft = factor(left + 1, right, false)) != -1) && ((tempLeft = morefactors(tempLeft, right, false)) != -1)){
        if(flag){
            tempLeft = 0;
            tempLeft = factor(left + 1, right, true);
            tempLeft = morefactors(tempLeft, right, true);
            addOperationToList("*");
        }
        return tempLeft;
    }
    return left;
}

int term(int left, int right, bool flag){
    if(DEBUG){ printf("term(%d, %d)->", left, right); for(int i = left; i <= right; i++) printf("%s ", tokenChars[i]); printf("\n"); }
    if(left > right)return -1;
    int tempLeft;
    if((tempLeft = factor(left, right, false)) != -1 && ((tempLeft = morefactors(tempLeft, right, false)) != -1)){
        if(flag){
            tempLeft = 0;
            tempLeft = factor(left, right, true);
            tempLeft = morefactors(tempLeft, right, true);
            // addOperationToList("term");
            // printf("term ");
        }
        return tempLeft;
    }
    return -1;
}

int expr(int left, int right, bool flag){
    if(DEBUG){ printf("expr(%d, %d)->", left, right); for(int i = left; i <= right; i++) printf("%s ", tokenChars[i]); printf("\n"); }
    if(left > right)return -1;
	int tempLeft;
    if((tempLeft = term(left, right, false)) != -1 && ((tempLeft = moreterms(tempLeft, right, false)) != -1)){
        if(flag){
            tempLeft = 0;
            tempLeft = term(left, right, true);
            tempLeft = moreterms(tempLeft, right, true);
            // addOperationToList("expr");
            // printf("expr ");
        }
        return tempLeft;
    }
    return -1;
}

int parseExpression(int left, int right){
    if(DEBUG){ printf("parseExpression(%d, %d)->", left, right); for(int i = left; i <= right; i++) printf("%s ", tokenChars[i]); printf("\n"); }
    globalList.size = 0;
    int tempLeft = expr(left, right, true);
    // printf("%d %d %d\n", left, right, tempLeft);
    if(tempLeft == -1){
        if(DEBUG){printf("Hata: Parse hatasi\n");}
        return -1;
    }
    return tempLeft;
}
#endif