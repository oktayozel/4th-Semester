#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define EPS 0.00001
enum type{OP, NUM, MAT, VEC, SCA};typedef struct Variable {int feature;int dim1,dim2;double val[1000][1000];}Variable;double mut(double x){return (x < 0)?-x:x;}int round_double(double x){return (x < 0)?(int)(x-0.5):(int)(x+0.5);}bool isInteger(double x){return (mut(x - round_double(x)) < EPS);}struct Variable * createScalar(){Variable *newNode = (Variable*)malloc(sizeof(Variable));newNode->feature = SCA;newNode->dim1 = 1;newNode->dim2 = 1;newNode->val[0][0] = 0;return newNode;}struct Variable * createVector(int a){Variable *newNode = (Variable*)malloc(sizeof(Variable));newNode->feature = VEC;newNode->dim1 = a;newNode->dim2 = 1;for(int i = 0; i < a; i++){newNode->val[i][0] = 0;}return newNode;}struct Variable * createMatrix(int a , int b){Variable *newNode = (Variable*)malloc(sizeof(Variable));newNode->feature = MAT;newNode->dim1 = a;newNode->dim2 = b;for(int i = 0; i < a; i++){for(int j = 0; j < b; j++){newNode->val[i][j] = 0;}}return newNode;}struct Variable * multiplication(struct Variable * a, struct Variable * b){struct Variable* newNode = (struct Variable*)malloc(sizeof(struct Variable));if( (a->dim1 == 1 && a->dim2 == 1 ) && (b->dim1 == 1 && b->dim2 == 1)){newNode->feature = SCA;newNode->dim1=1;newNode->dim2=1;newNode->val[0][0] = a->val[0][0] * b->val[0][0];}else if( (a->dim1 == 1 && a->dim2 == 1 ) && !(b->dim1 == 1 && b->dim2 == 1) ){newNode->feature = MAT;newNode->dim1=b->dim1;newNode->dim2=b->dim2;for( int i = 0 ; i < b->dim1 ; i++ ){for( int j = 0 ; j < b->dim2 ; j++ ){newNode->val[i][j] = a->val[0][0] * b->val[i][j];}}}else if( !(a->dim1 == 1 && a->dim2 == 1 ) && (b->dim1 == 1 && b->dim2 == 1) ){newNode->feature = MAT;newNode->dim1=a->dim1;newNode->dim2=a->dim2;for( int i = 0 ; i < a->dim1 ; i++ ){for( int j = 0 ; j < a->dim2 ; j++ ){newNode->val[i][j] = b->val[0][0] * a->val[i][j];}}}else if( !(a->dim1 == 1 && a->dim2 == 1 ) && !(b->dim1 == 1 && b->dim2 == 1) ){newNode->feature = MAT;newNode->dim1=a->dim1;newNode->dim2=b->dim2;for( int i = 0 ;  i < a->dim1 ; i++ ){for( int j = 0 ; j < b->dim2 ; j++ ){newNode->val[i][j]=0;for( int k = 0 ; k < a->dim2 ; k++ ){newNode->val[i][j] += (a->val[i][k] * b->val[k][j]);}}}}return newNode;}struct Variable * generateScalarFromNumber(double var){Variable *newNode = (Variable*)malloc(sizeof(Variable));newNode->feature = SCA;newNode->dim1 = 1;newNode->dim2 = 1;newNode->val[0][0]=var;return newNode;}struct Variable * addition( struct Variable *a , struct Variable *b ){struct Variable *newNode = (struct Variable*)malloc(sizeof(struct Variable));newNode->feature = a->feature;newNode->dim1 = a->dim1;newNode->dim2 = a->dim2;for( int i = 0 ;  i < a->dim1 ; i++ ){for(int j = 0 ; j < a->dim2 ; j++ ){newNode->val[i][j] = a->val[i][j] + b->val[i][j];}}return newNode;}struct Variable * substraction(struct Variable *a, struct Variable *b){struct Variable* newNode = (struct Variable*)malloc(sizeof(struct Variable));newNode->feature = a->feature;newNode->dim1 = a->dim1;newNode->dim2 = a->dim2;for( int i = 0 ;  i < a->dim1 ; i++ ){for(int j = 0 ; j < a->dim2 ; j++ ){newNode->val[i][j] = a->val[i][j] - b->val[i][j];}}return newNode;}double custom_sqrt(double x){if(x <= 0){return 0;}double x0 = x;double x1 = (x0 + x/x0)/2;for(int i = 0; i < 100; i++){x0 = x1;x1 = (x0 + x/x0)/2;}return x1;}struct Variable * squareroot( struct Variable *a){struct Variable* newNode = (struct Variable*)malloc(sizeof(struct Variable));newNode->feature = a->feature;newNode->dim1 = 1;newNode->dim2 = 1;newNode->val[0][0]=custom_sqrt(a->val[0][0]);return newNode;}struct Variable * transpose( struct Variable * a){struct Variable* newNode = (struct Variable*)malloc(sizeof(struct Variable));newNode->feature = a->feature;newNode->dim1 = a->dim2;newNode->dim2 = a->dim1;for( int i = 0 ; i < a->dim1 ; i++ ){for( int j = 0 ; j < a->dim2 ; j++ ){newNode->val[j][i] = a->val[i][j];}}return newNode;}struct Variable * choose( struct Variable * expr1, struct Variable * expr2, struct Variable * expr3, struct Variable * expr4 ){if( mut(expr1->val[0][0] - 0) < EPS ){return expr2;}else if( expr1->val[0][0] > 0){return expr3;}else if( expr1->val[0][0] < 0 ){return expr4;}return expr1;}void print( struct Variable * a){for( int i = 0 ; i < a->dim1 ; i++ ){for( int  j = 0 ; j < a->dim2 ; j++ ){if(isInteger(a->val[i][j])){printf("%d ",(int)a->val[i][j]);}else{printf("%lf ",a->val[i][j]);}}printf("\n");}}void printsep(){printf("------------\n");}bool isLower(struct Variable * a, struct Variable * b){return (a->val[0][0] <= b->val[0][0]);}void increase( struct Variable * a, struct Variable * b){a->val[0][0] += b->val[0][0];return;}struct Variable * getSingleIndexInt(struct Variable * a , int index ){index--;struct Variable* newNode = (struct Variable*)malloc(sizeof(struct Variable));int row = index / a->dim2;int column = index % a->dim2;newNode->dim1=1;newNode->dim2=1;newNode->feature=SCA;newNode->val[0][0]=a->val[row][column];return newNode;}struct Variable * getSingleIndex(struct Variable * a , struct Variable * index ){int temp = index->val[0][0] - 1;struct Variable* newNode = (struct Variable*)malloc(sizeof(struct Variable));int row = temp / a->dim2;int column = temp % a->dim2;newNode->dim1=1;newNode->dim2=1;newNode->feature=SCA;newNode->val[0][0]=a->val[row][column];return newNode;}struct Variable * getDoubleIndexInt(struct Variable * a , int x, int y ){struct Variable* newNode = (struct Variable*)malloc(sizeof(struct Variable));newNode->dim1=1;newNode->dim2=1;newNode->feature=SCA;newNode->val[0][0]=a->val[x - 1][y - 1];return newNode;}struct Variable * getDoubleIndex(struct Variable * a , struct Variable * x, struct Variable * y ){struct Variable* newNode = (struct Variable*)malloc(sizeof(struct Variable));newNode->dim1=1;newNode->dim2=1;newNode->feature=SCA;newNode->val[0][0]=a->val[(int)x->val[0][0] - 1][(int)y->val[0][0] - 1];return newNode;}void assign(struct Variable * a, struct Variable * b ){a->dim1 = b->dim1;a->dim2 = b->dim2;a->feature = b->feature;for( int i = 0 ; i < 999 ; i++){for( int j  = 0 ;  j  < 999 ; j++){a->val[i][j] = b->val[i][j];}}return;}void assignToFlatten(struct Variable * a , double var , int index ){int row = index / a->dim2;int column = index % a->dim2;a->val[row][column] = var ;return;}void assignToIndex(struct Variable * a, struct Variable * index, struct Variable * var  ){a->val[(int)index->val[0][0] - 1][0] = var->val[0][0];return;}void assignToDoubleIndex(struct Variable * a, struct Variable * index1, struct Variable* index2, struct Variable * var ){a->val[(int)index1->val[0][0] - 1][(int)index2->val[0][0] - 1] = var->val[0][0];return;}int main(){
Variable *_A = createMatrix(2, 2);
Variable *_InvA = createMatrix(2, 2);
Variable *_detA = createScalar();
Variable *_minusOne = createScalar();
Variable *_a = createScalar();
Variable *_b = createScalar();
Variable *_c = createScalar();
Variable *_d = createScalar();
Variable *_jjX2A9VNFsfJ4qH = generateScalarFromNumber(2);
Variable *_LoGBufBRBHNmNqf = generateScalarFromNumber(1);
Variable *_fzxc0WG0lVAj6AT = substraction(_jjX2A9VNFsfJ4qH, _LoGBufBRBHNmNqf);
assign(_minusOne, _fzxc0WG0lVAj6AT);
assignToFlatten(_A, 1, 0);assignToFlatten(_A, 2, 1);assignToFlatten(_A, 3, 2);assignToFlatten(_A, 5, 3);
Variable *_gqHSf71iezcBdjb = generateScalarFromNumber(1);
Variable *_EHKvrLK5t5pJqPy = generateScalarFromNumber(1);
Variable *_XqfK9Nxwuggw4mF = getDoubleIndex( _A, _gqHSf71iezcBdjb, _EHKvrLK5t5pJqPy );
assign(_a, _XqfK9Nxwuggw4mF);
Variable *_olx4OhLjzJCaRYs = generateScalarFromNumber(1);
Variable *_gZQKLSjEy4CZwcD = generateScalarFromNumber(2);
Variable *_ehHGCc0Fy6fV74l = getDoubleIndex( _A, _olx4OhLjzJCaRYs, _gZQKLSjEy4CZwcD );
assign(_b, _ehHGCc0Fy6fV74l);
Variable *_649VOR3g7qLY34A = generateScalarFromNumber(2);
Variable *_AQWygqUNk06ONHx = generateScalarFromNumber(1);
Variable *_VRab5y965CSUrp2 = getDoubleIndex( _A, _649VOR3g7qLY34A, _AQWygqUNk06ONHx );
assign(_c, _VRab5y965CSUrp2);
Variable *_kGR7Q0B0MwbPNRB = generateScalarFromNumber(2);
Variable *_7BJI6dqpRz1glsX = generateScalarFromNumber(2);
Variable *_BBKNLvwJByKkPJn = getDoubleIndex( _A, _kGR7Q0B0MwbPNRB, _7BJI6dqpRz1glsX );
assign(_d, _BBKNLvwJByKkPJn);
Variable *_JsDClAyQ67Rr71R = multiplication(_a, _d);
Variable *_83pEVYSS5RyWOLZ = multiplication(_b, _c);
Variable *_LVXDSZ7h6HJk0tJ = substraction(_JsDClAyQ67Rr71R, _83pEVYSS5RyWOLZ);
assign(_detA, _LVXDSZ7h6HJk0tJ);
Variable *_BfIewjUiks8qojm = generateScalarFromNumber(1);
Variable *_Z4HUXbH5Xp338Xs = generateScalarFromNumber(1);
assignToDoubleIndex(_InvA, _BfIewjUiks8qojm, _Z4HUXbH5Xp338Xs, _d);
Variable *_bwQ4YUInOGVfXYO = generateScalarFromNumber(1);
Variable *_2XleWRbjzwgnZRc = generateScalarFromNumber(2);
Variable *_xRhcckQQlPEutoN = multiplication(_minusOne, _b);
assignToDoubleIndex(_InvA, _bwQ4YUInOGVfXYO, _2XleWRbjzwgnZRc, _xRhcckQQlPEutoN);
Variable *_WWajJb4pssczxPD = generateScalarFromNumber(2);
Variable *_NwTtplfjV5Z9yBm = generateScalarFromNumber(1);
Variable *_zye88fNJe8BcXsq = multiplication(_minusOne, _c);
assignToDoubleIndex(_InvA, _WWajJb4pssczxPD, _NwTtplfjV5Z9yBm, _zye88fNJe8BcXsq);
Variable *_B6reMIm1YyBMjfi = generateScalarFromNumber(2);
Variable *_NlSUTIn5yMpo9Eo = generateScalarFromNumber(2);
assignToDoubleIndex(_InvA, _B6reMIm1YyBMjfi, _NlSUTIn5yMpo9Eo, _a);
print(_InvA);
printsep();
print(_detA);
return 0;
}
