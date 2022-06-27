#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define EPS 0.00001
enum type{OP, NUM, MAT, VEC, SCA};typedef struct Variable {int feature;int dim1,dim2;double val[1000][1000];}Variable;double mut(double x){return (x < 0)?-x:x;}int round_double(double x){return (x < 0)?(int)(x-0.5):(int)(x+0.5);}bool isInteger(double x){return (mut(x - round_double(x)) < EPS);}struct Variable * createScalar(){Variable *newNode = (Variable*)malloc(sizeof(Variable));newNode->feature = SCA;newNode->dim1 = 1;newNode->dim2 = 1;newNode->val[0][0] = 0;return newNode;}struct Variable * createVector(int a){Variable *newNode = (Variable*)malloc(sizeof(Variable));newNode->feature = VEC;newNode->dim1 = a;newNode->dim2 = 1;for(int i = 0; i < a; i++){newNode->val[i][0] = 0;}return newNode;}struct Variable * createMatrix(int a , int b){Variable *newNode = (Variable*)malloc(sizeof(Variable));newNode->feature = MAT;newNode->dim1 = a;newNode->dim2 = b;for(int i = 0; i < a; i++){for(int j = 0; j < b; j++){newNode->val[i][j] = 0;}}return newNode;}struct Variable * multiplication(struct Variable * a, struct Variable * b){struct Variable* newNode = (struct Variable*)malloc(sizeof(struct Variable));if( (a->dim1 == 1 && a->dim2 == 1 ) && (b->dim1 == 1 && b->dim2 == 1)){newNode->feature = SCA;newNode->dim1=1;newNode->dim2=1;newNode->val[0][0] = a->val[0][0] * b->val[0][0];}else if( (a->dim1 == 1 && a->dim2 == 1 ) && !(b->dim1 == 1 && b->dim2 == 1) ){newNode->feature = MAT;newNode->dim1=b->dim1;newNode->dim2=b->dim2;for( int i = 0 ; i < b->dim1 ; i++ ){for( int j = 0 ; j < b->dim2 ; j++ ){newNode->val[i][j] = a->val[0][0] * b->val[i][j];}}}else if( !(a->dim1 == 1 && a->dim2 == 1 ) && (b->dim1 == 1 && b->dim2 == 1) ){newNode->feature = MAT;newNode->dim1=a->dim1;newNode->dim2=a->dim2;for( int i = 0 ; i < a->dim1 ; i++ ){for( int j = 0 ; j < a->dim2 ; j++ ){newNode->val[i][j] = b->val[0][0] * a->val[i][j];}}}else if( !(a->dim1 == 1 && a->dim2 == 1 ) && !(b->dim1 == 1 && b->dim2 == 1) ){newNode->feature = MAT;newNode->dim1=a->dim1;newNode->dim2=b->dim2;for( int i = 0 ;  i < a->dim1 ; i++ ){for( int j = 0 ; j < b->dim2 ; j++ ){newNode->val[i][j]=0;for( int k = 0 ; k < a->dim2 ; k++ ){newNode->val[i][j] += (a->val[i][k] * b->val[k][j]);}}}}return newNode;}struct Variable * generateScalarFromNumber(double var){Variable *newNode = (Variable*)malloc(sizeof(Variable));newNode->feature = SCA;newNode->dim1 = 1;newNode->dim2 = 1;newNode->val[0][0]=var;return newNode;}struct Variable * addition( struct Variable *a , struct Variable *b ){struct Variable *newNode = (struct Variable*)malloc(sizeof(struct Variable));newNode->feature = a->feature;newNode->dim1 = a->dim1;newNode->dim2 = a->dim2;for( int i = 0 ;  i < a->dim1 ; i++ ){for(int j = 0 ; j < a->dim2 ; j++ ){newNode->val[i][j] = a->val[i][j] + b->val[i][j];}}return newNode;}struct Variable * substraction(struct Variable *a, struct Variable *b){struct Variable* newNode = (struct Variable*)malloc(sizeof(struct Variable));newNode->feature = a->feature;newNode->dim1 = a->dim1;newNode->dim2 = a->dim2;for( int i = 0 ;  i < a->dim1 ; i++ ){for(int j = 0 ; j < a->dim2 ; j++ ){newNode->val[i][j] = a->val[i][j] - b->val[i][j];}}return newNode;}double custom_sqrt(double x){if(x <= 0){return 0;}double x0 = x;double x1 = (x0 + x/x0)/2;for(int i = 0; i < 100; i++){x0 = x1;x1 = (x0 + x/x0)/2;}return x1;}struct Variable * squareroot( struct Variable *a){struct Variable* newNode = (struct Variable*)malloc(sizeof(struct Variable));newNode->feature = a->feature;newNode->dim1 = 1;newNode->dim2 = 1;newNode->val[0][0]=custom_sqrt(a->val[0][0]);return newNode;}struct Variable * transpose( struct Variable * a){struct Variable* newNode = (struct Variable*)malloc(sizeof(struct Variable));newNode->feature = a->feature;newNode->dim1 = a->dim2;newNode->dim2 = a->dim1;for( int i = 0 ; i < a->dim1 ; i++ ){for( int j = 0 ; j < a->dim2 ; j++ ){newNode->val[j][i] = a->val[i][j];}}return newNode;}struct Variable * choose( struct Variable * expr1, struct Variable * expr2, struct Variable * expr3, struct Variable * expr4 ){if( mut(expr1->val[0][0] - 0) < EPS ){return expr2;}else if( expr1->val[0][0] > 0){return expr3;}else if( expr1->val[0][0] < 0 ){return expr4;}return expr1;}void print( struct Variable * a){for( int i = 0 ; i < a->dim1 ; i++ ){for( int  j = 0 ; j < a->dim2 ; j++ ){if(isInteger(a->val[i][j])){printf("%d ",(int)a->val[i][j]);}else{printf("%lf ",a->val[i][j]);}}printf("\n");}}void printsep(){printf("------------\n");}bool isLower(struct Variable * a, struct Variable * b){return (a->val[0][0] <= b->val[0][0]);}void increase( struct Variable * a, struct Variable * b){a->val[0][0] += b->val[0][0];return;}struct Variable * getSingleIndexInt(struct Variable * a , int index ){index--;struct Variable* newNode = (struct Variable*)malloc(sizeof(struct Variable));int row = index / a->dim2;int column = index % a->dim2;newNode->dim1=1;newNode->dim2=1;newNode->feature=SCA;newNode->val[0][0]=a->val[row][column];return newNode;}struct Variable * getSingleIndex(struct Variable * a , struct Variable * index ){int temp = index->val[0][0] - 1;struct Variable* newNode = (struct Variable*)malloc(sizeof(struct Variable));int row = temp / a->dim2;int column = temp % a->dim2;newNode->dim1=1;newNode->dim2=1;newNode->feature=SCA;newNode->val[0][0]=a->val[row][column];return newNode;}struct Variable * getDoubleIndexInt(struct Variable * a , int x, int y ){struct Variable* newNode = (struct Variable*)malloc(sizeof(struct Variable));newNode->dim1=1;newNode->dim2=1;newNode->feature=SCA;newNode->val[0][0]=a->val[x - 1][y - 1];return newNode;}struct Variable * getDoubleIndex(struct Variable * a , struct Variable * x, struct Variable * y ){struct Variable* newNode = (struct Variable*)malloc(sizeof(struct Variable));newNode->dim1=1;newNode->dim2=1;newNode->feature=SCA;newNode->val[0][0]=a->val[(int)x->val[0][0] - 1][(int)y->val[0][0] - 1];return newNode;}void assign(struct Variable * a, struct Variable * b ){a->dim1 = b->dim1;a->dim2 = b->dim2;a->feature = b->feature;for( int i = 0 ; i < 999 ; i++){for( int j  = 0 ;  j  < 999 ; j++){a->val[i][j] = b->val[i][j];}}return;}void assignToFlatten(struct Variable * a , double var , int index ){int row = index / a->dim2;int column = index % a->dim2;a->val[row][column] = var ;return;}void assignToIndex(struct Variable * a, struct Variable * index, struct Variable * var  ){a->val[(int)index->val[0][0] - 1][0] = var->val[0][0];return;}void assignToDoubleIndex(struct Variable * a, struct Variable * index1, struct Variable* index2, struct Variable * var ){a->val[(int)index1->val[0][0] - 1][(int)index2->val[0][0] - 1] = var->val[0][0];return;}int main(){
Variable *_a = createScalar();
Variable *_b = createScalar();
Variable *_A = createMatrix(3, 3);
Variable *_B = createMatrix(3, 5);
Variable *_C = createMatrix(5, 7);
Variable *_D = createMatrix(7, 9);
Variable *_E = createMatrix(3, 9);
Variable *_u = createVector(3);
Variable *_v = createVector(3);
Variable *_t = createVector(3);
Variable *_s = createVector(7);
Variable *_s2 = createVector(7);
Variable *_s3 = createVector(7);
assignToFlatten(_s, 1, 0);assignToFlatten(_s, 1, 1);assignToFlatten(_s, 1, 2);assignToFlatten(_s, 1, 3);assignToFlatten(_s, 1, 4);assignToFlatten(_s, 1, 5);assignToFlatten(_s, 1, 6);
assignToFlatten(_s2, 2, 0);assignToFlatten(_s2, 1, 1);assignToFlatten(_s2, 0, 2);assignToFlatten(_s2, 0, 3);assignToFlatten(_s2, 1, 4);assignToFlatten(_s2, 0, 5);assignToFlatten(_s2, 2, 6);
Variable *_fzxc0WG0lVAj6AT = substraction(_s, _s2);
assign(_s3, _fzxc0WG0lVAj6AT);
assignToFlatten(_u, 1, 0);assignToFlatten(_u, 3, 1);assignToFlatten(_u, 1, 2);
assignToFlatten(_t, 0, 0);assignToFlatten(_t, 0, 1);assignToFlatten(_t, 3, 2);
Variable *_jjX2A9VNFsfJ4qH = substraction(_v, _t);
assign(_v, _jjX2A9VNFsfJ4qH);
printsep();
print(_v);
printsep();
Variable *_LoGBufBRBHNmNqf = generateScalarFromNumber(1);
Variable *_XqfK9Nxwuggw4mF = generateScalarFromNumber(3);
Variable *_gqHSf71iezcBdjb = generateScalarFromNumber(1);
Variable *_EHKvrLK5t5pJqPy = generateScalarFromNumber(1);
Variable *_ehHGCc0Fy6fV74l = generateScalarFromNumber(3);
Variable *_olx4OhLjzJCaRYs = generateScalarFromNumber(1);
for(assign(_a,_LoGBufBRBHNmNqf); isLower(_a,_XqfK9Nxwuggw4mF); increase(_a,_gqHSf71iezcBdjb)){
for(assign(_b,_EHKvrLK5t5pJqPy); isLower(_b,_ehHGCc0Fy6fV74l); increase(_b,_olx4OhLjzJCaRYs)){
Variable *_VRab5y965CSUrp2 = generateScalarFromNumber(1);
Variable *_gZQKLSjEy4CZwcD = getSingleIndex( _t, _VRab5y965CSUrp2 );
Variable *_AQWygqUNk06ONHx = generateScalarFromNumber(1);
Variable *_649VOR3g7qLY34A = getSingleIndex( _u, _AQWygqUNk06ONHx );
Variable *_BBKNLvwJByKkPJn = addition(_649VOR3g7qLY34A, _gZQKLSjEy4CZwcD);
Variable *_kGR7Q0B0MwbPNRB = multiplication(_b, _BBKNLvwJByKkPJn);
Variable *_7BJI6dqpRz1glsX = addition(_kGR7Q0B0MwbPNRB, _a);
assignToDoubleIndex(_A, _a, _b, _7BJI6dqpRz1glsX);
}}
printsep();
print(_A);
printsep();
Variable *_JsDClAyQ67Rr71R = generateScalarFromNumber(1);
Variable *_83pEVYSS5RyWOLZ = generateScalarFromNumber(3);
Variable *_LVXDSZ7h6HJk0tJ = generateScalarFromNumber(1);
Variable *_BfIewjUiks8qojm = generateScalarFromNumber(1);
Variable *_Z4HUXbH5Xp338Xs = generateScalarFromNumber(5);
Variable *_bwQ4YUInOGVfXYO = generateScalarFromNumber(1);
for(assign(_a,_JsDClAyQ67Rr71R); isLower(_a,_83pEVYSS5RyWOLZ); increase(_a,_LVXDSZ7h6HJk0tJ)){
for(assign(_b,_BfIewjUiks8qojm); isLower(_b,_Z4HUXbH5Xp338Xs); increase(_b,_bwQ4YUInOGVfXYO)){
Variable *_xRhcckQQlPEutoN = generateScalarFromNumber(1);
Variable *_2XleWRbjzwgnZRc = getSingleIndex( _t, _xRhcckQQlPEutoN );
Variable *_NwTtplfjV5Z9yBm = generateScalarFromNumber(2);
Variable *_WWajJb4pssczxPD = getSingleIndex( _u, _NwTtplfjV5Z9yBm );
Variable *_zye88fNJe8BcXsq = addition(_WWajJb4pssczxPD, _2XleWRbjzwgnZRc);
Variable *_B6reMIm1YyBMjfi = squareroot( _zye88fNJe8BcXsq );
Variable *_NlSUTIn5yMpo9Eo = multiplication(_b, _B6reMIm1YyBMjfi);
Variable *_Mwqx4EQ3CR1Nxs6 = substraction(_a, _NlSUTIn5yMpo9Eo);
assignToDoubleIndex(_B, _a, _b, _Mwqx4EQ3CR1Nxs6);
}}
printsep();
print(_B);
printsep();
Variable *_OX1urOd6HsY0vPf = generateScalarFromNumber(1);
Variable *_68SR1HEuz0lFgmX = generateScalarFromNumber(5);
Variable *_LFd0Llfiatep167 = generateScalarFromNumber(1);
Variable *_f4PrLbGyikugyYq = generateScalarFromNumber(1);
Variable *_j21mDBcDzQx3TuS = generateScalarFromNumber(7);
Variable *_gN1cZv0t1PVfQi6 = generateScalarFromNumber(1);
for(assign(_a,_OX1urOd6HsY0vPf); isLower(_a,_68SR1HEuz0lFgmX); increase(_a,_LFd0Llfiatep167)){
for(assign(_b,_f4PrLbGyikugyYq); isLower(_b,_j21mDBcDzQx3TuS); increase(_b,_gN1cZv0t1PVfQi6)){
Variable *_zQ3tNpGkDaDJimC = generateScalarFromNumber(1);
Variable *_PAtvZNE82NL2s3V = getSingleIndex( _u, _zQ3tNpGkDaDJimC );
Variable *_gvC4N8uvOQ6cVZZ = multiplication(_a, _PAtvZNE82NL2s3V);
Variable *_xH6oUB3p6HY2B3b = generateScalarFromNumber(2);
Variable *_a6XEOrULyU0ejDS = getSingleIndex( _u, _xH6oUB3p6HY2B3b );
Variable *_ZZ33FWhRRb8ly1u = multiplication(_a6XEOrULyU0ejDS, _b);
Variable *_me3SklXz1YZRhR1 = generateScalarFromNumber(3);
Variable *_Te6N6APzGtIlTrt = getSingleIndex( _u, _me3SklXz1YZRhR1 );
Variable *_ci1a1dL1jKfpK0a = addition(_Te6N6APzGtIlTrt, _ZZ33FWhRRb8ly1u);
Variable *_TGU3gtFMtJRWX5U = addition(_ci1a1dL1jKfpK0a, _gvC4N8uvOQ6cVZZ);
assignToDoubleIndex(_C, _a, _b, _TGU3gtFMtJRWX5U);
}}
printsep();
print(_C);
printsep();
Variable *_wO3XEDDDu6HvGFs = generateScalarFromNumber(1);
Variable *_Qv7khFC4ReKwSvV = generateScalarFromNumber(7);
Variable *_kl4xLzcOtcSFR3J = generateScalarFromNumber(1);
Variable *_lxKYhvVuOltTOfE = generateScalarFromNumber(1);
Variable *_VaPFAjE8h826U2n = generateScalarFromNumber(9);
Variable *_B3TanCfbV4gkMxX = generateScalarFromNumber(1);
for(assign(_a,_wO3XEDDDu6HvGFs); isLower(_a,_Qv7khFC4ReKwSvV); increase(_a,_kl4xLzcOtcSFR3J)){
for(assign(_b,_lxKYhvVuOltTOfE); isLower(_b,_VaPFAjE8h826U2n); increase(_b,_B3TanCfbV4gkMxX)){
Variable *_vs6tWn9ImfyZUyw = getSingleIndex( _s3, _a );
Variable *_JH5twrNLA8jOUNP = generateScalarFromNumber(1);
Variable *_HY1nsYP2EgUo0WG = generateScalarFromNumber(2);
Variable *_6RvYHruejSBt3h1 = generateScalarFromNumber(3);
Variable *_K6ACYjJrzrZHxgk = choose( _vs6tWn9ImfyZUyw, _JH5twrNLA8jOUNP, _HY1nsYP2EgUo0WG, _6RvYHruejSBt3h1 );
assignToDoubleIndex(_D, _a, _b, _K6ACYjJrzrZHxgk);
}}
printsep();
print(_D);
printsep();
Variable *_3Bw6n8uORcGqeAX = multiplication(_C, _D);
Variable *_RIKUHovZVk9kmua = multiplication(_B, _3Bw6n8uORcGqeAX);
Variable *_5PM8NofeJEMBFff = multiplication(_A, _RIKUHovZVk9kmua);
assign(_E, _5PM8NofeJEMBFff);
Variable *_U3n3o91ZGHB62VF = generateScalarFromNumber(1);
Variable *_Ikm7EzqRkUCVlwV = generateScalarFromNumber(5);
Variable *_AFNkgCekPeBOhgB = generateScalarFromNumber(2);
for(assign(_a,_U3n3o91ZGHB62VF); isLower(_a,_Ikm7EzqRkUCVlwV); increase(_a,_AFNkgCekPeBOhgB)){
printsep();
}
print(_E);
Variable *_6cqYgoruzCpvoTg = generateScalarFromNumber(1);
Variable *_4fsIH3zFuL9pNMo = generateScalarFromNumber(5);
Variable *_2016Wtb2pAvxeVQ = generateScalarFromNumber(2);
for(assign(_a,_6cqYgoruzCpvoTg); isLower(_a,_4fsIH3zFuL9pNMo); increase(_a,_2016Wtb2pAvxeVQ)){
printsep();
}
return 0;
}
