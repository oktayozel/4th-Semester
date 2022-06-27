#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define EPS 0.00001
enum type{OP, NUM, MAT, VEC, SCA};typedef struct Variable {int feature;int dim1,dim2;double val[1000][1000];}Variable;double mut(double x){return (x < 0)?-x:x;}int round_double(double x){return (x < 0)?(int)(x-0.5):(int)(x+0.5);}bool isInteger(double x){return (mut(x - round_double(x)) < EPS);}struct Variable * createScalar(){Variable *newNode = (Variable*)malloc(sizeof(Variable));newNode->feature = SCA;newNode->dim1 = 1;newNode->dim2 = 1;newNode->val[0][0] = 0;return newNode;}struct Variable * createVector(int a){Variable *newNode = (Variable*)malloc(sizeof(Variable));newNode->feature = VEC;newNode->dim1 = a;newNode->dim2 = 1;for(int i = 0; i < a; i++){newNode->val[i][0] = 0;}return newNode;}struct Variable * createMatrix(int a , int b){Variable *newNode = (Variable*)malloc(sizeof(Variable));newNode->feature = MAT;newNode->dim1 = a;newNode->dim2 = b;for(int i = 0; i < a; i++){for(int j = 0; j < b; j++){newNode->val[i][j] = 0;}}return newNode;}struct Variable * multiplication(struct Variable * a, struct Variable * b){struct Variable* newNode = (struct Variable*)malloc(sizeof(struct Variable));if( (a->dim1 == 1 && a->dim2 == 1 ) && (b->dim1 == 1 && b->dim2 == 1)){newNode->feature = SCA;newNode->dim1=1;newNode->dim2=1;newNode->val[0][0] = a->val[0][0] * b->val[0][0];}else if( (a->dim1 == 1 && a->dim2 == 1 ) && !(b->dim1 == 1 && b->dim2 == 1) ){newNode->feature = MAT;newNode->dim1=b->dim1;newNode->dim2=b->dim2;for( int i = 0 ; i < b->dim1 ; i++ ){for( int j = 0 ; j < b->dim2 ; j++ ){newNode->val[i][j] = a->val[0][0] * b->val[i][j];}}}else if( !(a->dim1 == 1 && a->dim2 == 1 ) && (b->dim1 == 1 && b->dim2 == 1) ){newNode->feature = MAT;newNode->dim1=a->dim1;newNode->dim2=a->dim2;for( int i = 0 ; i < a->dim1 ; i++ ){for( int j = 0 ; j < a->dim2 ; j++ ){newNode->val[i][j] = b->val[0][0] * a->val[i][j];}}}else if( !(a->dim1 == 1 && a->dim2 == 1 ) && !(b->dim1 == 1 && b->dim2 == 1) ){newNode->feature = MAT;newNode->dim1=a->dim1;newNode->dim2=b->dim2;for( int i = 0 ;  i < a->dim1 ; i++ ){for( int j = 0 ; j < b->dim2 ; j++ ){newNode->val[i][j]=0;for( int k = 0 ; k < a->dim2 ; k++ ){newNode->val[i][j] += (a->val[i][k] * b->val[k][j]);}}}}return newNode;}struct Variable * generateScalarFromNumber(double var){Variable *newNode = (Variable*)malloc(sizeof(Variable));newNode->feature = SCA;newNode->dim1 = 1;newNode->dim2 = 1;newNode->val[0][0]=var;return newNode;}struct Variable * addition( struct Variable *a , struct Variable *b ){struct Variable *newNode = (struct Variable*)malloc(sizeof(struct Variable));newNode->feature = a->feature;newNode->dim1 = a->dim1;newNode->dim2 = a->dim2;for( int i = 0 ;  i < a->dim1 ; i++ ){for(int j = 0 ; j < a->dim2 ; j++ ){newNode->val[i][j] = a->val[i][j] + b->val[i][j];}}return newNode;}struct Variable * substraction(struct Variable *a, struct Variable *b){struct Variable* newNode = (struct Variable*)malloc(sizeof(struct Variable));newNode->feature = a->feature;newNode->dim1 = a->dim1;newNode->dim2 = a->dim2;for( int i = 0 ;  i < a->dim1 ; i++ ){for(int j = 0 ; j < a->dim2 ; j++ ){newNode->val[i][j] = a->val[i][j] - b->val[i][j];}}return newNode;}double custom_sqrt(double x){if(x <= 0){return 0;}double x0 = x;double x1 = (x0 + x/x0)/2;for(int i = 0; i < 100; i++){x0 = x1;x1 = (x0 + x/x0)/2;}return x1;}struct Variable * squareroot( struct Variable *a){struct Variable* newNode = (struct Variable*)malloc(sizeof(struct Variable));newNode->feature = a->feature;newNode->dim1 = 1;newNode->dim2 = 1;newNode->val[0][0]=custom_sqrt(a->val[0][0]);return newNode;}struct Variable * transpose( struct Variable * a){struct Variable* newNode = (struct Variable*)malloc(sizeof(struct Variable));newNode->feature = a->feature;newNode->dim1 = a->dim2;newNode->dim2 = a->dim1;for( int i = 0 ; i < a->dim1 ; i++ ){for( int j = 0 ; j < a->dim2 ; j++ ){newNode->val[j][i] = a->val[i][j];}}return newNode;}struct Variable * choose( struct Variable * expr1, struct Variable * expr2, struct Variable * expr3, struct Variable * expr4 ){if( mut(expr1->val[0][0] - 0) < EPS ){return expr2;}else if( expr1->val[0][0] > 0){return expr3;}else if( expr1->val[0][0] < 0 ){return expr4;}return expr1;}void print( struct Variable * a){for( int i = 0 ; i < a->dim1 ; i++ ){for( int  j = 0 ; j < a->dim2 ; j++ ){if(isInteger(a->val[i][j])){printf("%d ",(int)a->val[i][j]);}else{printf("%lf ",a->val[i][j]);}}printf("\n");}}void printsep(){printf("------------\n");}bool isLower(struct Variable * a, struct Variable * b){return (a->val[0][0] <= b->val[0][0]);}void increase( struct Variable * a, struct Variable * b){a->val[0][0] += b->val[0][0];return;}struct Variable * getSingleIndexInt(struct Variable * a , int index ){index--;struct Variable* newNode = (struct Variable*)malloc(sizeof(struct Variable));int row = index / a->dim2;int column = index % a->dim2;newNode->dim1=1;newNode->dim2=1;newNode->feature=SCA;newNode->val[0][0]=a->val[row][column];return newNode;}struct Variable * getSingleIndex(struct Variable * a , struct Variable * index ){int temp = index->val[0][0] - 1;struct Variable* newNode = (struct Variable*)malloc(sizeof(struct Variable));int row = temp / a->dim2;int column = temp % a->dim2;newNode->dim1=1;newNode->dim2=1;newNode->feature=SCA;newNode->val[0][0]=a->val[row][column];return newNode;}struct Variable * getDoubleIndexInt(struct Variable * a , int x, int y ){struct Variable* newNode = (struct Variable*)malloc(sizeof(struct Variable));newNode->dim1=1;newNode->dim2=1;newNode->feature=SCA;newNode->val[0][0]=a->val[x - 1][y - 1];return newNode;}struct Variable * getDoubleIndex(struct Variable * a , struct Variable * x, struct Variable * y ){struct Variable* newNode = (struct Variable*)malloc(sizeof(struct Variable));newNode->dim1=1;newNode->dim2=1;newNode->feature=SCA;newNode->val[0][0]=a->val[(int)x->val[0][0] - 1][(int)y->val[0][0] - 1];return newNode;}void assign(struct Variable * a, struct Variable * b ){a->dim1 = b->dim1;a->dim2 = b->dim2;a->feature = b->feature;for( int i = 0 ; i < 999 ; i++){for( int j  = 0 ;  j  < 999 ; j++){a->val[i][j] = b->val[i][j];}}return;}void assignToFlatten(struct Variable * a , double var , int index ){int row = index / a->dim2;int column = index % a->dim2;a->val[row][column] = var ;return;}void assignToIndex(struct Variable * a, struct Variable * index, struct Variable * var  ){a->val[(int)index->val[0][0] - 1][0] = var->val[0][0];return;}void assignToDoubleIndex(struct Variable * a, struct Variable * index1, struct Variable* index2, struct Variable * var ){a->val[(int)index1->val[0][0] - 1][(int)index2->val[0][0] - 1] = var->val[0][0];return;}int main(){
Variable *_image = createMatrix(5, 5);
Variable *_filter = createMatrix(3, 3);
Variable *_temp = createMatrix(3, 3);
Variable *_res = createMatrix(3, 3);
Variable *_i = createScalar();
Variable *_j = createScalar();
Variable *_n = createScalar();
Variable *_m = createScalar();
Variable *_sum = createScalar();
Variable *_filterSize = createScalar();
Variable *_fzxc0WG0lVAj6AT = generateScalarFromNumber(5);
assign(_n, _fzxc0WG0lVAj6AT);
Variable *_jjX2A9VNFsfJ4qH = generateScalarFromNumber(5);
assign(_m, _jjX2A9VNFsfJ4qH);
Variable *_LoGBufBRBHNmNqf = generateScalarFromNumber(3);
assign(_filterSize, _LoGBufBRBHNmNqf);
assignToFlatten(_image, 0, 0);assignToFlatten(_image, 1, 1);assignToFlatten(_image, 0, 2);assignToFlatten(_image, 0, 3);assignToFlatten(_image, 0, 4);assignToFlatten(_image, 1, 5);assignToFlatten(_image, 1, 6);assignToFlatten(_image, 1, 7);assignToFlatten(_image, 0, 8);assignToFlatten(_image, 0, 9);assignToFlatten(_image, 0, 10);assignToFlatten(_image, 1, 11);assignToFlatten(_image, 0, 12);assignToFlatten(_image, 1, 13);assignToFlatten(_image, 0, 14);assignToFlatten(_image, 0, 15);assignToFlatten(_image, 0, 16);assignToFlatten(_image, 1, 17);assignToFlatten(_image, 1, 18);assignToFlatten(_image, 1, 19);assignToFlatten(_image, 0, 20);assignToFlatten(_image, 1, 21);assignToFlatten(_image, 0, 22);assignToFlatten(_image, 1, 23);assignToFlatten(_image, 0, 24);
assignToFlatten(_filter, 0, 0);assignToFlatten(_filter, 1, 1);assignToFlatten(_filter, 0, 2);assignToFlatten(_filter, 1, 3);assignToFlatten(_filter, 1, 4);assignToFlatten(_filter, 1, 5);assignToFlatten(_filter, 0, 6);assignToFlatten(_filter, 1, 7);assignToFlatten(_filter, 0, 8);
Variable *_XqfK9Nxwuggw4mF = generateScalarFromNumber(1);
Variable *_gqHSf71iezcBdjb = substraction(_n, _filterSize);
Variable *_ehHGCc0Fy6fV74l = generateScalarFromNumber(1);
Variable *_EHKvrLK5t5pJqPy = addition(_ehHGCc0Fy6fV74l, _gqHSf71iezcBdjb);
Variable *_olx4OhLjzJCaRYs = generateScalarFromNumber(1);
Variable *_gZQKLSjEy4CZwcD = generateScalarFromNumber(1);
Variable *_VRab5y965CSUrp2 = substraction(_n, _filterSize);
Variable *_AQWygqUNk06ONHx = generateScalarFromNumber(1);
Variable *_649VOR3g7qLY34A = addition(_AQWygqUNk06ONHx, _VRab5y965CSUrp2);
Variable *_BBKNLvwJByKkPJn = generateScalarFromNumber(1);
for(assign(_i,_XqfK9Nxwuggw4mF); isLower(_i,_EHKvrLK5t5pJqPy); increase(_i,_olx4OhLjzJCaRYs)){
for(assign(_j,_gZQKLSjEy4CZwcD); isLower(_j,_649VOR3g7qLY34A); increase(_j,_BBKNLvwJByKkPJn)){
Variable *_kGR7Q0B0MwbPNRB = generateScalarFromNumber(1);
Variable *_7BJI6dqpRz1glsX = generateScalarFromNumber(1);
Variable *_JsDClAyQ67Rr71R = getDoubleIndex( _image, _i, _j );
assignToDoubleIndex(_temp, _kGR7Q0B0MwbPNRB, _7BJI6dqpRz1glsX, _JsDClAyQ67Rr71R);
Variable *_83pEVYSS5RyWOLZ = generateScalarFromNumber(1);
Variable *_LVXDSZ7h6HJk0tJ = generateScalarFromNumber(2);
Variable *_Z4HUXbH5Xp338Xs = generateScalarFromNumber(1);
Variable *_BfIewjUiks8qojm = addition(_Z4HUXbH5Xp338Xs, _j);
Variable *_bwQ4YUInOGVfXYO = getDoubleIndex( _image, _i, _BfIewjUiks8qojm );
assignToDoubleIndex(_temp, _83pEVYSS5RyWOLZ, _LVXDSZ7h6HJk0tJ, _bwQ4YUInOGVfXYO);
Variable *_2XleWRbjzwgnZRc = generateScalarFromNumber(1);
Variable *_xRhcckQQlPEutoN = generateScalarFromNumber(3);
Variable *_NwTtplfjV5Z9yBm = generateScalarFromNumber(2);
Variable *_WWajJb4pssczxPD = addition(_NwTtplfjV5Z9yBm, _j);
Variable *_zye88fNJe8BcXsq = getDoubleIndex( _image, _i, _WWajJb4pssczxPD );
assignToDoubleIndex(_temp, _2XleWRbjzwgnZRc, _xRhcckQQlPEutoN, _zye88fNJe8BcXsq);
Variable *_B6reMIm1YyBMjfi = generateScalarFromNumber(2);
Variable *_NlSUTIn5yMpo9Eo = generateScalarFromNumber(1);
Variable *_OX1urOd6HsY0vPf = generateScalarFromNumber(1);
Variable *_Mwqx4EQ3CR1Nxs6 = addition(_OX1urOd6HsY0vPf, _i);
Variable *_68SR1HEuz0lFgmX = getDoubleIndex( _image, _Mwqx4EQ3CR1Nxs6, _j );
assignToDoubleIndex(_temp, _B6reMIm1YyBMjfi, _NlSUTIn5yMpo9Eo, _68SR1HEuz0lFgmX);
Variable *_LFd0Llfiatep167 = generateScalarFromNumber(2);
Variable *_f4PrLbGyikugyYq = generateScalarFromNumber(2);
Variable *_gN1cZv0t1PVfQi6 = generateScalarFromNumber(1);
Variable *_j21mDBcDzQx3TuS = addition(_gN1cZv0t1PVfQi6, _i);
Variable *_zQ3tNpGkDaDJimC = generateScalarFromNumber(1);
Variable *_PAtvZNE82NL2s3V = addition(_zQ3tNpGkDaDJimC, _j);
Variable *_gvC4N8uvOQ6cVZZ = getDoubleIndex( _image, _j21mDBcDzQx3TuS, _PAtvZNE82NL2s3V );
assignToDoubleIndex(_temp, _LFd0Llfiatep167, _f4PrLbGyikugyYq, _gvC4N8uvOQ6cVZZ);
Variable *_a6XEOrULyU0ejDS = generateScalarFromNumber(2);
Variable *_xH6oUB3p6HY2B3b = generateScalarFromNumber(3);
Variable *_Te6N6APzGtIlTrt = generateScalarFromNumber(1);
Variable *_ZZ33FWhRRb8ly1u = addition(_Te6N6APzGtIlTrt, _i);
Variable *_ci1a1dL1jKfpK0a = generateScalarFromNumber(2);
Variable *_me3SklXz1YZRhR1 = addition(_ci1a1dL1jKfpK0a, _j);
Variable *_TGU3gtFMtJRWX5U = getDoubleIndex( _image, _ZZ33FWhRRb8ly1u, _me3SklXz1YZRhR1 );
assignToDoubleIndex(_temp, _a6XEOrULyU0ejDS, _xH6oUB3p6HY2B3b, _TGU3gtFMtJRWX5U);
Variable *_wO3XEDDDu6HvGFs = generateScalarFromNumber(3);
Variable *_Qv7khFC4ReKwSvV = generateScalarFromNumber(1);
Variable *_lxKYhvVuOltTOfE = generateScalarFromNumber(2);
Variable *_kl4xLzcOtcSFR3J = addition(_lxKYhvVuOltTOfE, _i);
Variable *_VaPFAjE8h826U2n = getDoubleIndex( _image, _kl4xLzcOtcSFR3J, _j );
assignToDoubleIndex(_temp, _wO3XEDDDu6HvGFs, _Qv7khFC4ReKwSvV, _VaPFAjE8h826U2n);
Variable *_B3TanCfbV4gkMxX = generateScalarFromNumber(3);
Variable *_vs6tWn9ImfyZUyw = generateScalarFromNumber(2);
Variable *_JH5twrNLA8jOUNP = generateScalarFromNumber(2);
Variable *_K6ACYjJrzrZHxgk = addition(_JH5twrNLA8jOUNP, _i);
Variable *_6RvYHruejSBt3h1 = generateScalarFromNumber(1);
Variable *_HY1nsYP2EgUo0WG = addition(_6RvYHruejSBt3h1, _j);
Variable *_3Bw6n8uORcGqeAX = getDoubleIndex( _image, _K6ACYjJrzrZHxgk, _HY1nsYP2EgUo0WG );
assignToDoubleIndex(_temp, _B3TanCfbV4gkMxX, _vs6tWn9ImfyZUyw, _3Bw6n8uORcGqeAX);
Variable *_RIKUHovZVk9kmua = generateScalarFromNumber(3);
Variable *_5PM8NofeJEMBFff = generateScalarFromNumber(3);
Variable *_Ikm7EzqRkUCVlwV = generateScalarFromNumber(2);
Variable *_U3n3o91ZGHB62VF = addition(_Ikm7EzqRkUCVlwV, _i);
Variable *_6cqYgoruzCpvoTg = generateScalarFromNumber(2);
Variable *_AFNkgCekPeBOhgB = addition(_6cqYgoruzCpvoTg, _j);
Variable *_4fsIH3zFuL9pNMo = getDoubleIndex( _image, _U3n3o91ZGHB62VF, _AFNkgCekPeBOhgB );
assignToDoubleIndex(_temp, _RIKUHovZVk9kmua, _5PM8NofeJEMBFff, _4fsIH3zFuL9pNMo);
Variable *_vXRL9MuNh44MojT = generateScalarFromNumber(1);
Variable *_1G2B6d8fvafnssG = generateScalarFromNumber(1);
Variable *_2016Wtb2pAvxeVQ = getDoubleIndex( _temp, _vXRL9MuNh44MojT, _1G2B6d8fvafnssG );
Variable *_H8gpAKMYXj81CyT = generateScalarFromNumber(1);
Variable *_MaecgkaoDrXhZ4h = generateScalarFromNumber(1);
Variable *_n2mwEEfvJOurxLI = getDoubleIndex( _filter, _H8gpAKMYXj81CyT, _MaecgkaoDrXhZ4h );
Variable *_r6e84NcGv0VNlwU = multiplication(_2016Wtb2pAvxeVQ, _n2mwEEfvJOurxLI);
Variable *_JS6TXMh6mWrmibg = generateScalarFromNumber(1);
Variable *_WJWnHb6a0RTJmja = generateScalarFromNumber(2);
Variable *_5RVSVDgPmQGioCT = getDoubleIndex( _temp, _JS6TXMh6mWrmibg, _WJWnHb6a0RTJmja );
Variable *_TC1KXZbFzx6o2Av = generateScalarFromNumber(1);
Variable *_PSgT8I62FKqARGC = generateScalarFromNumber(2);
Variable *_Og2idwjW7b41xdq = getDoubleIndex( _filter, _TC1KXZbFzx6o2Av, _PSgT8I62FKqARGC );
Variable *_lGzKdoQIcM0lysj = multiplication(_5RVSVDgPmQGioCT, _Og2idwjW7b41xdq);
Variable *_E8MQrVq3zC47BoX = generateScalarFromNumber(1);
Variable *_No2lo5lrZUGDSro = generateScalarFromNumber(3);
Variable *_KldIrfF05PlL1ss = getDoubleIndex( _temp, _E8MQrVq3zC47BoX, _No2lo5lrZUGDSro );
Variable *_LXUWTyqdnEhCxQQ = generateScalarFromNumber(1);
Variable *_O4TIP8TvxVOUsBT = generateScalarFromNumber(3);
Variable *_fFud4P9lD7RIPhG = getDoubleIndex( _filter, _LXUWTyqdnEhCxQQ, _O4TIP8TvxVOUsBT );
Variable *_wibcemdV7EWsjap = multiplication(_KldIrfF05PlL1ss, _fFud4P9lD7RIPhG);
Variable *_8SrJmcKBXUr2Xmk = addition(_wibcemdV7EWsjap, _lGzKdoQIcM0lysj);
Variable *_bPUoejZnNKJq3pv = addition(_8SrJmcKBXUr2Xmk, _r6e84NcGv0VNlwU);
assign(_sum, _bPUoejZnNKJq3pv);
Variable *_ZfjdB5kAvREPwjU = generateScalarFromNumber(2);
Variable *_5ORC1KEpNK8rQ8v = generateScalarFromNumber(1);
Variable *_paksljPEyiSMo9R = getDoubleIndex( _temp, _ZfjdB5kAvREPwjU, _5ORC1KEpNK8rQ8v );
Variable *_CgV5qiDbosFIvns = generateScalarFromNumber(2);
Variable *_xnn53iUcUjOv3yU = generateScalarFromNumber(1);
Variable *_7FyyE2DqtkRD2sg = getDoubleIndex( _filter, _CgV5qiDbosFIvns, _xnn53iUcUjOv3yU );
Variable *_9i4BkW8E5gT1EKM = multiplication(_paksljPEyiSMo9R, _7FyyE2DqtkRD2sg);
Variable *_Rmt7XK7gBB0b5uB = generateScalarFromNumber(2);
Variable *_ZrYiIDXEsp7nPHO = generateScalarFromNumber(2);
Variable *_rP7kJS36cogRln4 = getDoubleIndex( _temp, _Rmt7XK7gBB0b5uB, _ZrYiIDXEsp7nPHO );
Variable *_Kpw0MAdoD2IuLnL = generateScalarFromNumber(2);
Variable *_1lOsTZjvkXahho8 = generateScalarFromNumber(2);
Variable *_DCnqA3wchCWTvaX = getDoubleIndex( _filter, _Kpw0MAdoD2IuLnL, _1lOsTZjvkXahho8 );
Variable *_SHpAdgk5UB8aYJR = multiplication(_rP7kJS36cogRln4, _DCnqA3wchCWTvaX);
Variable *_rFVsCPBKcaj9WCV = generateScalarFromNumber(2);
Variable *_DzMCBGz6ZyV1I8I = generateScalarFromNumber(3);
Variable *_BQtaUrRiwVhUJq7 = getDoubleIndex( _temp, _rFVsCPBKcaj9WCV, _DzMCBGz6ZyV1I8I );
Variable *_RSRhMqrAicjRzFg = generateScalarFromNumber(2);
Variable *_1hglbbiEbePRJnY = generateScalarFromNumber(3);
Variable *_ShnMjmJwLiq5lfO = getDoubleIndex( _filter, _RSRhMqrAicjRzFg, _1hglbbiEbePRJnY );
Variable *_ysG6NZBqQAJeZrE = multiplication(_BQtaUrRiwVhUJq7, _ShnMjmJwLiq5lfO);
Variable *_wFOxJXsLTBYflIZ = addition(_ysG6NZBqQAJeZrE, _SHpAdgk5UB8aYJR);
Variable *_gWrRHeDpOgkb7Kh = addition(_wFOxJXsLTBYflIZ, _9i4BkW8E5gT1EKM);
Variable *_Wi1Aly8eWWygDRD = addition(_gWrRHeDpOgkb7Kh, _sum);
assign(_sum, _Wi1Aly8eWWygDRD);
Variable *_63TNH1JBv0kGvd6 = generateScalarFromNumber(3);
Variable *_eFmENV73bHaxQ2m = generateScalarFromNumber(1);
Variable *_lIEMtTL2uP8jj9H = getDoubleIndex( _temp, _63TNH1JBv0kGvd6, _eFmENV73bHaxQ2m );
Variable *_ud7loxaU67ek8P5 = generateScalarFromNumber(3);
Variable *_OiPflFl64shpWVr = generateScalarFromNumber(1);
Variable *_Gnfz9dJ7yoyA3vJ = getDoubleIndex( _filter, _ud7loxaU67ek8P5, _OiPflFl64shpWVr );
Variable *_zRHe7YN9TvG0SfS = multiplication(_lIEMtTL2uP8jj9H, _Gnfz9dJ7yoyA3vJ);
Variable *_Kvgiml3dwwSK3KS = generateScalarFromNumber(3);
Variable *_UGlwUEpzqM8O8lQ = generateScalarFromNumber(2);
Variable *_G7Rp9NNFO6lOQdv = getDoubleIndex( _temp, _Kvgiml3dwwSK3KS, _UGlwUEpzqM8O8lQ );
Variable *_z4N2kuipfI9AmCU = generateScalarFromNumber(3);
Variable *_0qhJgxnNQj0IDxy = generateScalarFromNumber(2);
Variable *_OsFhQ5bZVBtVZYW = getDoubleIndex( _filter, _z4N2kuipfI9AmCU, _0qhJgxnNQj0IDxy );
Variable *_Au0WtVDTD9E4xLS = multiplication(_G7Rp9NNFO6lOQdv, _OsFhQ5bZVBtVZYW);
Variable *_7nW8gEoVqifMVEC = generateScalarFromNumber(3);
Variable *_YbvhMuQPyHilO86 = generateScalarFromNumber(3);
Variable *_N4We0nDZZLueyrb = getDoubleIndex( _temp, _7nW8gEoVqifMVEC, _YbvhMuQPyHilO86 );
Variable *_BC05Uf9H4Bjp51B = generateScalarFromNumber(3);
Variable *_UCVXtjXd5tM6f5Y = generateScalarFromNumber(3);
Variable *_teUcv5G451x8iem = getDoubleIndex( _filter, _BC05Uf9H4Bjp51B, _UCVXtjXd5tM6f5Y );
Variable *_105RWMWzuAZZqJK = multiplication(_N4We0nDZZLueyrb, _teUcv5G451x8iem);
Variable *_XrMdCq4DHPb1EEy = addition(_105RWMWzuAZZqJK, _Au0WtVDTD9E4xLS);
Variable *_BZBqwtvbCxAa6EU = addition(_XrMdCq4DHPb1EEy, _zRHe7YN9TvG0SfS);
Variable *_IXgMXliSlv0upWH = addition(_BZBqwtvbCxAa6EU, _sum);
assign(_sum, _IXgMXliSlv0upWH);
Variable *_VCvtlmeVUWENLcC = generateScalarFromNumber(5);
Variable *_T3TLoPlAMxib0Fe = substraction(_sum, _VCvtlmeVUWENLcC);
Variable *_ILFvkkw34gZX7t6 = generateScalarFromNumber(1);
Variable *_26n2Xw7XD4v3laa = generateScalarFromNumber(0);
Variable *_AkxFDbnSQH4Sby3 = generateScalarFromNumber(0);
Variable *_UzVNC4F1uMnzLx1 = choose( _T3TLoPlAMxib0Fe, _ILFvkkw34gZX7t6, _26n2Xw7XD4v3laa, _AkxFDbnSQH4Sby3 );
assignToDoubleIndex(_res, _i, _j, _UzVNC4F1uMnzLx1);
}}
print(_res);
return 0;
}
