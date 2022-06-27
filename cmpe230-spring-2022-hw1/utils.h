#ifndef _utils_h
#define _utils_h

#include "enums.h"
#include "globals.h"

void raiseError(){
	printf("Error (Line %d)\n", lineCount);
    fclose(out);
    remove(output_file_name);
	exit(0);
}

bool isVariableChar(char c) {
    if(c >= 'a' && c <= 'z')
        return true;
    if(c >= 'A' && c <= 'Z')
        return true;
    if(c >= '0' && c <= '9')
        return true;
    if(c == '_')
        return true;
    return false;
}

bool isAllowedCharacter(char c) {
    if(c >= 'a' && c <= 'z')
        return true;
    if(c >= 'A' && c <= 'Z')
        return true;
    if(c >= '0' && c <= '9')
        return true;
    switch(c) {
        case '_':
        case '[':
        case ']':
        case ',':
        case '{':
        case '}':
        case '(':
        case ')':
        case ':':
        case '*':
        case '+':
        case '-':
        case '.':
        case '#':
        case '=':
            return true;
    }
    return false;
}

bool isWhiteSpace(char c) {
    return (c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\v' || c == '\f');
}

bool isArraysEqual(int *arr1, int *arr2){
	int i = 0;
	while(arr1[i] != -1 && arr2[i] != -1){
		if(arr1[i] != arr2[i]){
			return false;
		}
		i++;
	}
	return (arr1[i] == -1 && arr2[i] == -1);
}



void removeComments(char *str) {
	int len = strlen(str);
	for (int i = 0; i < len; i++) {
		if (str[i] == '#') {
			str[i] = '\0';
			break;
		}
	}
}

bool isValidLine(char *str) {
	int len = strlen(str);
	for (int i = 0; i < len; i++) {
		if (!isAllowedCharacter(str[i]) && !isWhiteSpace(str[i])) {
			return false;
		}
	}
	return true;
}


int checkConstantAssignment(){
	if(!(tokens[0] == IDENTIFIER && tokens[1] == ASSIGNMENT && tokens[2] == LEFT_BRACE))
		return 0;
	int i = 3;
	for(; tokens[i] != -1; i++){
		if(tokens[i] == RIGHT_BRACE){
			if(tokens[i + 1] == -1)
				return i - 3;
			else
				return 0;
		} else if(tokens[i] != NUMBER){
			return 0;
		}
	}
	return 0;
}


#endif