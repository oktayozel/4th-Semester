#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "enums.h"
#include "globals.h"
#include "utils.h"
#include "parser_graph.h"
#include "stack_operations.h"
#include "expr.h"
#include "dictionary.h"


int main(int argc, char *argv[]) {
	// Get the input file name
	char *input_file_name = argv[1];
    // Copy input file name to output file name and change the extension to .c
    output_file_name = (char *)malloc(strlen(input_file_name) + 5);
    strcpy(output_file_name, input_file_name);
    int output_file_len = strlen(output_file_name);

    output_file_name[output_file_len - 3] = 'c';
    output_file_name[output_file_len - 2] = '\0';

	// printf("input filename is %s\n", input_file_name);
	// printf("output filename is %s\n", output_file_name);
	// Open file
	FILE *input_file = fopen(input_file_name, "r");
	if (input_file == NULL) {
		if(DEBUG){printf("Error opening file\n");}
        raiseError();
	}
    // Open output file
    out = fopen(output_file_name, "w");
    if (out == NULL) {
        if(DEBUG){printf("Error opening file\n");}
        raiseError();
    }

    fprintf(out, "#include <stdio.h>\n");
    fprintf(out, "#include <stdlib.h>\n");
    fprintf(out, "#include <stdbool.h>\n");
    // fprintf(out, "#define N 1000000\n");
    // fprintf(out, "#define M 1000\n");
    fprintf(out, "#define EPS 0.00001\n");
    fprintf(out, "enum type{OP, NUM, MAT, VEC, SCA};typedef struct Variable {int feature;int dim1,dim2;double val[1000][1000];}Variable;double mut(double x){return (x < 0)?-x:x;}int round_double(double x){return (x < 0)?(int)(x-0.5):(int)(x+0.5);}bool isInteger(double x){return (mut(x - round_double(x)) < EPS);}struct Variable * createScalar(){Variable *newNode = (Variable*)malloc(sizeof(Variable));newNode->feature = SCA;newNode->dim1 = 1;newNode->dim2 = 1;newNode->val[0][0] = 0;return newNode;}struct Variable * createVector(int a){Variable *newNode = (Variable*)malloc(sizeof(Variable));newNode->feature = VEC;newNode->dim1 = a;newNode->dim2 = 1;for(int i = 0; i < a; i++){newNode->val[i][0] = 0;}return newNode;}struct Variable * createMatrix(int a , int b){Variable *newNode = (Variable*)malloc(sizeof(Variable));newNode->feature = MAT;newNode->dim1 = a;newNode->dim2 = b;for(int i = 0; i < a; i++){for(int j = 0; j < b; j++){newNode->val[i][j] = 0;}}return newNode;}struct Variable * multiplication(struct Variable * a, struct Variable * b){struct Variable* newNode = (struct Variable*)malloc(sizeof(struct Variable));if( (a->dim1 == 1 && a->dim2 == 1 ) && (b->dim1 == 1 && b->dim2 == 1)){newNode->feature = SCA;newNode->dim1=1;newNode->dim2=1;newNode->val[0][0] = a->val[0][0] * b->val[0][0];}else if( (a->dim1 == 1 && a->dim2 == 1 ) && !(b->dim1 == 1 && b->dim2 == 1) ){newNode->feature = MAT;newNode->dim1=b->dim1;newNode->dim2=b->dim2;for( int i = 0 ; i < b->dim1 ; i++ ){for( int j = 0 ; j < b->dim2 ; j++ ){newNode->val[i][j] = a->val[0][0] * b->val[i][j];}}}else if( !(a->dim1 == 1 && a->dim2 == 1 ) && (b->dim1 == 1 && b->dim2 == 1) ){newNode->feature = MAT;newNode->dim1=a->dim1;newNode->dim2=a->dim2;for( int i = 0 ; i < a->dim1 ; i++ ){for( int j = 0 ; j < a->dim2 ; j++ ){newNode->val[i][j] = b->val[0][0] * a->val[i][j];}}}else if( !(a->dim1 == 1 && a->dim2 == 1 ) && !(b->dim1 == 1 && b->dim2 == 1) ){newNode->feature = MAT;newNode->dim1=a->dim1;newNode->dim2=b->dim2;for( int i = 0 ;  i < a->dim1 ; i++ ){for( int j = 0 ; j < b->dim2 ; j++ ){newNode->val[i][j]=0;for( int k = 0 ; k < a->dim2 ; k++ ){newNode->val[i][j] += (a->val[i][k] * b->val[k][j]);}}}}return newNode;}struct Variable * generateScalarFromNumber(double var){Variable *newNode = (Variable*)malloc(sizeof(Variable));newNode->feature = SCA;newNode->dim1 = 1;newNode->dim2 = 1;newNode->val[0][0]=var;return newNode;}struct Variable * addition( struct Variable *a , struct Variable *b ){struct Variable *newNode = (struct Variable*)malloc(sizeof(struct Variable));newNode->feature = a->feature;newNode->dim1 = a->dim1;newNode->dim2 = a->dim2;for( int i = 0 ;  i < a->dim1 ; i++ ){for(int j = 0 ; j < a->dim2 ; j++ ){newNode->val[i][j] = a->val[i][j] + b->val[i][j];}}return newNode;}struct Variable * substraction(struct Variable *a, struct Variable *b){struct Variable* newNode = (struct Variable*)malloc(sizeof(struct Variable));newNode->feature = a->feature;newNode->dim1 = a->dim1;newNode->dim2 = a->dim2;for( int i = 0 ;  i < a->dim1 ; i++ ){for(int j = 0 ; j < a->dim2 ; j++ ){newNode->val[i][j] = a->val[i][j] - b->val[i][j];}}return newNode;}double custom_sqrt(double x){if(x <= 0){return 0;}double x0 = x;double x1 = (x0 + x/x0)/2;for(int i = 0; i < 100; i++){x0 = x1;x1 = (x0 + x/x0)/2;}return x1;}struct Variable * squareroot( struct Variable *a){struct Variable* newNode = (struct Variable*)malloc(sizeof(struct Variable));newNode->feature = a->feature;newNode->dim1 = 1;newNode->dim2 = 1;newNode->val[0][0]=custom_sqrt(a->val[0][0]);return newNode;}struct Variable * transpose( struct Variable * a){struct Variable* newNode = (struct Variable*)malloc(sizeof(struct Variable));newNode->feature = a->feature;newNode->dim1 = a->dim2;newNode->dim2 = a->dim1;for( int i = 0 ; i < a->dim1 ; i++ ){for( int j = 0 ; j < a->dim2 ; j++ ){newNode->val[j][i] = a->val[i][j];}}return newNode;}struct Variable * choose( struct Variable * expr1, struct Variable * expr2, struct Variable * expr3, struct Variable * expr4 ){if( mut(expr1->val[0][0] - 0) < EPS ){return expr2;}else if( expr1->val[0][0] > 0){return expr3;}else if( expr1->val[0][0] < 0 ){return expr4;}return expr1;}void print( struct Variable * a){for( int i = 0 ; i < a->dim1 ; i++ ){for( int  j = 0 ; j < a->dim2 ; j++ ){if(isInteger(a->val[i][j])){printf(\"%%d \",(int)a->val[i][j]);}else{printf(\"%%lf \",a->val[i][j]);}}printf(\"\\n\");}}void printsep(){printf(\"------------\\n\");}bool isLower(struct Variable * a, struct Variable * b){return (a->val[0][0] <= b->val[0][0]);}void increase( struct Variable * a, struct Variable * b){a->val[0][0] += b->val[0][0];return;}struct Variable * getSingleIndexInt(struct Variable * a , int index ){index--;struct Variable* newNode = (struct Variable*)malloc(sizeof(struct Variable));int row = index / a->dim2;int column = index %% a->dim2;newNode->dim1=1;newNode->dim2=1;newNode->feature=SCA;newNode->val[0][0]=a->val[row][column];return newNode;}struct Variable * getSingleIndex(struct Variable * a , struct Variable * index ){int temp = index->val[0][0] - 1;struct Variable* newNode = (struct Variable*)malloc(sizeof(struct Variable));int row = temp / a->dim2;int column = temp %% a->dim2;newNode->dim1=1;newNode->dim2=1;newNode->feature=SCA;newNode->val[0][0]=a->val[row][column];return newNode;}struct Variable * getDoubleIndexInt(struct Variable * a , int x, int y ){struct Variable* newNode = (struct Variable*)malloc(sizeof(struct Variable));newNode->dim1=1;newNode->dim2=1;newNode->feature=SCA;newNode->val[0][0]=a->val[x - 1][y - 1];return newNode;}struct Variable * getDoubleIndex(struct Variable * a , struct Variable * x, struct Variable * y ){struct Variable* newNode = (struct Variable*)malloc(sizeof(struct Variable));newNode->dim1=1;newNode->dim2=1;newNode->feature=SCA;newNode->val[0][0]=a->val[(int)x->val[0][0] - 1][(int)y->val[0][0] - 1];return newNode;}void assign(struct Variable * a, struct Variable * b ){a->dim1 = b->dim1;a->dim2 = b->dim2;a->feature = b->feature;for( int i = 0 ; i < 999 ; i++){for( int j  = 0 ;  j  < 999 ; j++){a->val[i][j] = b->val[i][j];}}return;}void assignToFlatten(struct Variable * a , double var , int index ){int row = index / a->dim2;int column = index %% a->dim2;a->val[row][column] = var ;return;}void assignToIndex(struct Variable * a, struct Variable * index, struct Variable * var  ){a->val[(int)index->val[0][0] - 1][0] = var->val[0][0];return;}void assignToDoubleIndex(struct Variable * a, struct Variable * index1, struct Variable* index2, struct Variable * var ){a->val[(int)index1->val[0][0] - 1][(int)index2->val[0][0] - 1] = var->val[0][0];return;}");
    fprintf(out, "int main(){\n");




	// Read file line by line
	char line[1024];
	ParserGraph *graph = createParserGraph();
    int forLoopType = 0;
	while (fgets(line, sizeof(line), input_file) != NULL) {
        
		lineCount++;
		// Remove comments
		removeComments(line);

		// Check if line is valid
		if (!isValidLine(line))
			raiseError();

		tokenizeLine(graph, line);
		
		int len = tokenLen(tokens);
		int constantAssignment = checkConstantAssignment();
		if(tokens[0] == -1){
			continue;
		} else if(isArraysEqual(tokens, CREATING_SCALAR_TOKENS)){
            if(nonDecleration){
                raiseError();
            }
			// printf("CREATING SCALAR\n");
            if(isExist(tokenChars[1])){
                if(DEBUG){printf("Error in line %d: Variable %s already exists\n", lineCount, tokenChars[1]);}
                raiseError();
            }
			fprintf(out, "Variable *_%s = createScalar();\n", tokenChars[1]);
            addNewScalar(tokenChars[1]);
		} else if(isArraysEqual(tokens, CREATING_VECTOR_TOKENS)){
            if(nonDecleration){
                raiseError();
            }
            if(isExist(tokenChars[1])){
                if(DEBUG){printf("Error in line %d: Variable %s already exists\n", lineCount, tokenChars[1]);}
                raiseError();
            }
			fprintf(out, "Variable *_%s = createVector(%d);\n", tokenChars[1], atoi(tokenChars[3]));
            addNewVector(tokenChars[1], atoi(tokenChars[3]));
		} else if(isArraysEqual(tokens, CREATING_MATRIX_TOKENS)){
            if(nonDecleration){
                raiseError();
            }
            if(isExist(tokenChars[1])){
                if(DEBUG){printf("Error in line %d: Variable %s already exists\n", lineCount, tokenChars[1]);}
                raiseError();
            }
			fprintf(out, "Variable *_%s = createMatrix(%d, %d);\n", tokenChars[1], atoi(tokenChars[3]), atoi(tokenChars[5]));
            addNewMatrix(tokenChars[1], atoi(tokenChars[3]), atoi(tokenChars[5]));
		} else if(constantAssignment){
            nonDecleration = 1;
			// printf("CONSTANT ASSIGNMENT to the variable %s with %d numbers\n", tokenChars[0], constantAssignment);
			// TODO: check if sizes match
			// printf("assignMultiple(&%s, ", tokenChars[0]);
            if(!isExist(tokenChars[0])){
                if(DEBUG){printf("Error in line %d: Variable %s does not exist\n", lineCount, tokenChars[0]);}
                raiseError();
            }

            Variable * var = getFromDict(tokenChars[0]);
            // printf("Got variable %s with sizes %d %d\n", tokenChars[0], var->dim1, var->dim2);
            if(var->feature == SCA){
                if(DEBUG){printf("Error in line %d: Variable %s is a scalar\n", lineCount, tokenChars[0]);}
                raiseError();
            }
            int size = var->dim1 * var->dim2;
            if(constantAssignment != size){
                if(DEBUG){printf("Error in line %d: Constant assignment size does not match the size of the variable %s\n", lineCount, tokenChars[0]);}
                raiseError();
            }

			for(int i = 0; i < constantAssignment; i++){
                fprintf(out, "assignToFlatten(_%s, %s, %d);", tokenChars[0], tokenChars[i + 3], i);
			}
			fprintf(out, "\n");
		} else if(isArraysEqual(tokens, PRINTSEP_TOKENS)){
            nonDecleration = 1;
			fprintf(out, "printsep();\n");
		} else if(tokens[0] == PRINT){
            nonDecleration = 1;
            // printf("PRINTEE geldiii\n");
            int tempLeft = 0;
            if(tokens[1] == LEFT_PARENTHESIS
            && (tempLeft = expr(2, len, false)) != -1
            && (tokens[tempLeft] == RIGHT_PARENTHESIS)
            && (tokens[tempLeft + 1] == -1)
            ){
                tempLeft = parseExpression(2, len);
                Variable* expr1 = evaluateList(&globalList);

                fprintf(out, "print(_%s);\n", expr1->name);
            // } else if(tokens[1] == LEFT_PARENTHESIS
            // && (tokens[2] == IDENTIFIER)
            // && (tokens[3] == LEFT_BRACKET)
            // && (tokens[4] == NUMBER)
            // && (tokens[5] == RIGHT_BRACKET)
            // && (tokens[6] == RIGHT_PARENTHESIS)
            // && (tokens[tempLeft + 1] == -1)
            // ){
            //     if(!isExist(tokenChars[2])){
            //         if(DEBUG){printf("Error in line %d: Variable %s does not exist\n", lineCount, tokenChars[2]);}
            //         raiseError();
            //     }
            //     Variable *var = getFromDict(tokenChars[2]);
            //     if(var->feature != VEC){
            //         if(DEBUG){printf("Error in line %d: Variable %s is not a vector\n", lineCount, tokenChars[2]);}
            //         raiseError();
            //     }
            //     fprintf(out, "print(getSingleIndexInt(_%s, %s));\n", tokenChars[2], tokenChars[4]);
            // } else if(tokens[1] == LEFT_PARENTHESIS
            // && (tokens[2] == IDENTIFIER)
            // && (tokens[3] == LEFT_BRACKET)
            // && (tokens[4] == NUMBER)
            // && (tokens[5] == COMMA)
            // && (tokens[6] == NUMBER)
            // && (tokens[7] == RIGHT_BRACKET)
            // && (tokens[8] == RIGHT_PARENTHESIS)
            // ){
            //     if(!isExist(tokenChars[2])){
            //         if(DEBUG){printf("Error in line %d: Variable %s does not exist\n", lineCount, tokenChars[2]);}
            //         raiseError();
            //     }
            //     Variable *var = getFromDict(tokenChars[2]);
            //     if(var->feature != MAT){
            //         if(DEBUG){printf("Error in line %d: Variable %s is not a matrix\n", lineCount, tokenChars[2]);}
            //         raiseError();
            //     }
            //     // TODO check if the indices are correct
            //     fprintf(out, "print(getDoubleIndexInt(_%s, _%s, _%s));\n", tokenChars[2], tokenChars[4], tokenChars[6]);
            } else {
                raiseError();
            }
        }

        else if(tokens[0] == FOR){
            nonDecleration = 1;
            int tempLeft;
            if(tokens[1] == LEFT_PARENTHESIS
            && (tokens[2] == IDENTIFIER)
            && (tokens[3] == IN)
            && (tempLeft = expr(4, len, false)) != -1
            && (tokens[tempLeft] == TWO_DOTS)
            && (tempLeft = expr(tempLeft + 1, len, false)) != -1
            && (tokens[tempLeft] == TWO_DOTS)
            && (tempLeft = expr(tempLeft + 1, len, false)) != -1
            && (tokens[tempLeft] == RIGHT_PARENTHESIS)
            && (tokens[tempLeft + 1] == LEFT_BRACE)
            && (tokens[tempLeft + 2] == -1)
            ){
                tempLeft = parseExpression(4, len);
                Variable* expr1 = evaluateList(&globalList);

                tempLeft = parseExpression(tempLeft + 1, len);
                Variable* expr2 = evaluateList(&globalList);

                tempLeft = parseExpression(tempLeft + 1, len);
                Variable* expr3 = evaluateList(&globalList);
                if(forLoopType != 0){
                    if(DEBUG){printf("Error in line %d: For loop already defined\n", lineCount);}
                    raiseError();
                }
                forLoopType=1;

                Variable* var = getFromDict(tokenChars[2]);
                if(var->feature != SCA){
                    if(DEBUG){printf("Error in line %d: Variable %s is not a scalar\n", lineCount, tokenChars[2]);}
                    raiseError();
                }
                if(expr1->feature != SCA || expr2->feature != SCA || expr3->feature != SCA){
                    if(DEBUG){printf("Error in line %d: Expression in for loop is not a scalar\n", lineCount);}
                    raiseError();
                }
                fprintf(out, "for(assign(_%s,_%s); isLower(_%s,_%s); increase(_%s,_%s)){\n", tokenChars[2],expr1->name, tokenChars[2],expr2->name, tokenChars[2],expr3->name);
            } else if(tokens[1] == LEFT_PARENTHESIS
            && (tokens[2] == IDENTIFIER)
            && (tokens[3] == COMMA)
            && (tokens[4] == IDENTIFIER)
            && (tokens[5] == IN)
            && (tempLeft = expr(6, len, false)) != -1
            && (tokens[tempLeft] == TWO_DOTS)
            && (tempLeft = expr(tempLeft + 1, len, false)) != -1
            && (tokens[tempLeft] == TWO_DOTS)
            && (tempLeft = expr(tempLeft + 1, len, false)) != -1
            && (tokens[tempLeft] == COMMA)
            && (tempLeft = expr(tempLeft + 1, len, false)) != -1
            && (tokens[tempLeft] == TWO_DOTS)
            && (tempLeft = expr(tempLeft + 1, len, false)) != -1
            && (tokens[tempLeft] == TWO_DOTS)
            && (tempLeft = expr(tempLeft + 1, len, false)) != -1
            && (tokens[tempLeft] == RIGHT_PARENTHESIS)
            && (tokens[tempLeft + 1] == LEFT_BRACE)
            && (tokens[tempLeft + 2] == -1)
            ){
                tempLeft = parseExpression(6, len);
                Variable* expr1 = evaluateList(&globalList);

                tempLeft = parseExpression(tempLeft + 1, len);
                Variable* expr2 = evaluateList(&globalList);

                tempLeft = parseExpression(tempLeft + 1, len);
                Variable* expr3 = evaluateList(&globalList);

                tempLeft = parseExpression(tempLeft + 1, len);
                Variable* expr4 = evaluateList(&globalList);

                tempLeft = parseExpression(tempLeft + 1, len);
                Variable* expr5 = evaluateList(&globalList);


                tempLeft = parseExpression(tempLeft + 1, len);
                Variable* expr6 = evaluateList(&globalList);
                if(forLoopType != 0){
                    if(DEBUG){printf("Error in line %d: For loop already defined\n", lineCount);}
                    raiseError();
                }
                forLoopType=2;

                if(!isExist(tokenChars[2])){
                    if(DEBUG){printf("Error in line %d: Variable %s does not exist\n", lineCount, tokenChars[2]);}
                    raiseError();
                }
                Variable* var = getFromDict(tokenChars[2]);
                if(var->feature != SCA){
                    if(DEBUG){printf("Error in line %d: Variable %s is not a scalar\n", lineCount, tokenChars[2]);}
                    raiseError();
                }
                if(expr1->feature != SCA || expr2->feature != SCA || expr3->feature != SCA || expr4->feature != SCA || expr5->feature != SCA || expr6->feature != SCA){
                    if(DEBUG){printf("Error in line %d: Expression in for loop is not a scalar\n", lineCount);}
                    raiseError();
                }
                fprintf(out, "for(assign(_%s,_%s); isLower(_%s,_%s); increase(_%s,_%s)){\n", tokenChars[2],expr1->name, tokenChars[2],expr2->name, tokenChars[2],expr3->name);
                fprintf(out, "for(assign(_%s,_%s); isLower(_%s,_%s); increase(_%s,_%s)){\n", tokenChars[4],expr4->name, tokenChars[4],expr5->name, tokenChars[4],expr6->name);
            } else {
                raiseError();
            }
        }
        else if(isArraysEqual(tokens, FOR_LOOP_END_TOKENS)){
            nonDecleration = 1;
            if(forLoopType==1){
                fprintf(out, "}\n");
            } else if(forLoopType==2){
                fprintf(out, "}}\n");
            } else{
                if(DEBUG){printf("Error in line %d: For loop not started\n", lineCount);}
                raiseError();
            }
            forLoopType=0;
        }
        
        else{
            if(len <= 0){
                continue;
            }
            nonDecleration = 1;
            int tempLeft = 0;
            if(tokens[0] == IDENTIFIER
            && (tokens[1] == ASSIGNMENT)
            && (tempLeft = expr(2, len - 1, false)) != -1
            && (tokens[tempLeft] == -1)
            ){
                tempLeft = parseExpression(2, len - 1);
                
                if(DEBUG){printList(&globalList);}
                Variable* expr1 = evaluateList(&globalList);
                if(!isExist(tokenChars[0])){
                    if(DEBUG){printf("Error in line %d: Variable %s does not exist\n", lineCount, tokenChars[0]);}
                    raiseError();
                }
                Variable* var = getFromDict(tokenChars[0]);
                if(var->dim1 != expr1->dim1 || var->dim2 != expr1->dim2){
                    if(DEBUG){printf("Error in line %d: Variable %s is not of the same size as the expression\n", lineCount, tokenChars[0]);}
                    if(DEBUG){printf("Variable size: %d, %d\n", var->dim1, var->dim2);}
                    if(DEBUG){printf("Expression size: %d, %d\n", expr1->dim1, expr1->dim2);}
                    raiseError();
                }
                fprintf(out, "assign(_%s, _%s);\n", tokenChars[0], expr1->name);
            } else if(
            tokens[0] == IDENTIFIER
            && (tokens[1] == LEFT_BRACKET)
            && (tempLeft = expr(2, len - 1, false)) != -1
            && (tokens[tempLeft] == RIGHT_BRACKET)
            && (tokens[tempLeft + 1] == ASSIGNMENT)
            && (tempLeft = expr(tempLeft + 2, len - 1, false)) != -1
            && (tokens[tempLeft] == -1)
            ){
                tempLeft = parseExpression(2, len - 1);
                Variable* loc1 = evaluateList(&globalList);

                tempLeft = parseExpression(tempLeft + 2, len - 1);
                Variable* expr1 = evaluateList(&globalList);
                if(!isExist(tokenChars[0])){
                    if(DEBUG){printf("Error in line %d: Variable %s does not exist\n", lineCount, tokenChars[0]);}
                    raiseError();
                }
                Variable* var = getFromDict(tokenChars[0]);
                if(var->feature != VEC){
                    if(DEBUG){printf("Error in line %d: Variable %s is not a vector\n", lineCount, tokenChars[0]);}
                    raiseError();
                }
                if(expr1->feature != SCA){
                    if(DEBUG){printf("Error in line %d: Expression in single assingment is not a scalar\n", lineCount);}
                    raiseError();
                }
                if(loc1->feature != SCA){
                    if(DEBUG){printf("Error in line %d: Expression in single assingment is not a scalar\n", lineCount);}
                    raiseError();
                }

                fprintf(out, "assignToIndex(_%s, _%s, _%s);\n", tokenChars[0], loc1->name, expr1->name);
            } else if(
            tokens[0] == IDENTIFIER
            && (tokens[1] == LEFT_BRACKET)
            && (tempLeft = expr(2, len - 1, false)) != -1
            && (tokens[tempLeft] == COMMA)
            && (tempLeft = expr(tempLeft + 1, len - 1, false)) != -1
            && (tokens[tempLeft] == RIGHT_BRACKET)
            && (tokens[tempLeft + 1] == ASSIGNMENT)
            && (tempLeft = expr(tempLeft + 2, len - 1, false)) != -1
            && (tokens[tempLeft] == -1)
            ){
                tempLeft = parseExpression(2, len - 1);
                Variable* loc1 = evaluateList(&globalList);
                tempLeft = parseExpression(tempLeft + 1, len - 1);
                Variable* loc2 = evaluateList(&globalList);
                tempLeft = parseExpression(tempLeft + 2, len - 1);
                Variable* expr1 = evaluateList(&globalList);

                if(!isExist(tokenChars[0])){
                    if(DEBUG){printf("Error in line %d: Variable %s does not exist\n", lineCount, tokenChars[0]);}
                    raiseError();
                }
                Variable* var = getFromDict(tokenChars[0]);
                if(var->feature != MAT){
                    if(DEBUG){printf("Error in line %d: Variable %s is not a vector\n", lineCount, tokenChars[0]);}
                    raiseError();
                }
                if(expr1->feature != SCA || loc1->feature != SCA || loc2->feature != SCA){
                    if(DEBUG){printf("Error in line %d: Expression in single assingment is not a scalar\n", lineCount);}
                    raiseError();
                }
                fprintf(out, "assignToDoubleIndex(_%s, _%s, _%s, _%s);\n", tokenChars[0], loc1->name, loc2->name, expr1->name);
            } else {
                if(DEBUG){printf("Buradayken error veri expr assignment bulunamadi\n");}
                if(DEBUG){printf("%s %d\n", line, len);}
                raiseError();
            }
		}
	}
    if(forLoopType!=0){
        if(DEBUG){printf("Error in line %d: For loop not ended\n", lineCount);}
        raiseError();
    }
	// Close file
	fclose(input_file);
    fprintf(out, "return 0;\n}\n");
    fclose(out);
	return 0;
}