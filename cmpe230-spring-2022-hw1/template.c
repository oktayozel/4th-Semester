#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#define N 1000000
#define M 1000
#define EPS 0.00001
enum type{OP, NUM, MAT, VEC, SCA};

typedef struct Variable {
    int feature;
    int dim1,dim2;
    double val[1000][1000];
}Variable;

double mut(double x){
    return (x < 0)?-x:x;
}

int round_double(double x){
    return (x < 0)?(int)(x-0.5):(int)(x+0.5);
}

bool isInteger(double x){
    return (mut(x - round_double(x)) < EPS);
}

struct Variable * createScalar(){
    Variable *newNode = (Variable*)malloc(sizeof(Variable));
    newNode->feature = SCA;
    newNode->dim1 = 1;
    newNode->dim2 = 1;
    newNode->val[0][0] = 0;
    return newNode;
}
struct Variable * createVector(int a){
    Variable *newNode = (Variable*)malloc(sizeof(Variable));
    newNode->feature = VEC;
    newNode->dim1 = a;
    newNode->dim2 = 1;
    for(int i = 0; i < a; i++){
        newNode->val[i][0] = 0;
    }
    return newNode;
}
struct Variable * createMatrix(int a , int b){
    Variable *newNode = (Variable*)malloc(sizeof(Variable));
    newNode->feature = MAT;
    newNode->dim1 = a;
    newNode->dim2 = b;
    for(int i = 0; i < a; i++){
        for(int j = 0; j < b; j++){
            newNode->val[i][j] = 0;
        }
    }
    return newNode;
}




struct Variable * multiplication(struct Variable * a, struct Variable * b){
    struct Variable* newNode = (struct Variable*)malloc(sizeof(struct Variable));
    
    if( (a->dim1 == 1 && a->dim2 == 1 ) && (b->dim1 == 1 && b->dim2 == 1)){
        newNode->feature = SCA;
        newNode->dim1=1;
        newNode->dim2=1;
        newNode->val[0][0] = a->val[0][0] * b->val[0][0];
    }
    
    else if( (a->dim1 == 1 && a->dim2 == 1 ) && !(b->dim1 == 1 && b->dim2 == 1) ){
        newNode->feature = MAT;
        newNode->dim1=b->dim1;
        newNode->dim2=b->dim2;
        for( int i = 0 ; i < b->dim1 ; i++ ){
            for( int j = 0 ; j < b->dim2 ; j++ ){
                newNode->val[i][j] = a->val[0][0] * b->val[i][j];
            }
        }
    }

    else if( !(a->dim1 == 1 && a->dim2 == 1 ) && (b->dim1 == 1 && b->dim2 == 1) ){
        newNode->feature = MAT;
        newNode->dim1=a->dim1;
        newNode->dim2=a->dim2;
        for( int i = 0 ; i < a->dim1 ; i++ ){
            for( int j = 0 ; j < a->dim2 ; j++ ){
                newNode->val[i][j] = b->val[0][0] * a->val[i][j];
            }
        }
    }
    else if( !(a->dim1 == 1 && a->dim2 == 1 ) && !(b->dim1 == 1 && b->dim2 == 1) ){
        newNode->feature = MAT;
        newNode->dim1=a->dim1;
        newNode->dim2=b->dim2;
        for( int i = 0 ;  i < a->dim1 ; i++ ){
            for( int j = 0 ; j < b->dim2 ; j++ ){
                newNode->val[i][j]=0;
                for( int k = 0 ; k < a->dim2 ; k++ ){

                    newNode->val[i][j] += (a->val[i][k] * b->val[k][j]);
                }
            }
        }
    }
    return newNode;
    
}

struct Variable * generateScalarFromNumber(double var){
    Variable *newNode = (Variable*)malloc(sizeof(Variable));
    newNode->feature = SCA;
    newNode->dim1 = 1;
    newNode->dim2 = 1;
    newNode->val[0][0]=var;
    return newNode;
}

struct Variable * addition( struct Variable *a , struct Variable *b ){
    
    struct Variable *newNode = (struct Variable*)malloc(sizeof(struct Variable));
    
    newNode->feature = a->feature;
    newNode->dim1 = a->dim1;
    newNode->dim2 = a->dim2;
    for( int i = 0 ;  i < a->dim1 ; i++ ){
        for(int j = 0 ; j < a->dim2 ; j++ ){
            newNode->val[i][j] = a->val[i][j] + b->val[i][j];
        }
    }
    return newNode;
    
}

struct Variable * substraction(struct Variable *a, struct Variable *b){
    struct Variable* newNode = (struct Variable*)malloc(sizeof(struct Variable));
    newNode->feature = a->feature;
    newNode->dim1 = a->dim1;
    newNode->dim2 = a->dim2;
    for( int i = 0 ;  i < a->dim1 ; i++ ){
        for(int j = 0 ; j < a->dim2 ; j++ ){
            newNode->val[i][j] = a->val[i][j] - b->val[i][j];
        }
    }
    return newNode;
}


double custom_sqrt(double x){
    if(x <= 0){
        return 0;
    }
    double x0 = x;
    double x1 = (x0 + x/x0)/2;
    for(int i = 0; i < 100; i++){
        x0 = x1;
        x1 = (x0 + x/x0)/2;
    }
    return x1;
}

struct Variable * squareroot( struct Variable *a){
    struct Variable* newNode = (struct Variable*)malloc(sizeof(struct Variable));
    newNode->feature = a->feature;
    newNode->dim1 = 1;
    newNode->dim2 = 1;
    newNode->val[0][0]=custom_sqrt(a->val[0][0]);
    return newNode;
}
struct Variable * transpose( struct Variable * a){
    struct Variable* newNode = (struct Variable*)malloc(sizeof(struct Variable));
    newNode->feature = a->feature;
    newNode->dim1 = a->dim2;
    newNode->dim2 = a->dim1;
    for( int i = 0 ; i < a->dim1 ; i++ ){
        for( int j = 0 ; j < a->dim2 ; j++ ){
            newNode->val[j][i] = a->val[i][j];
        }
    }
    return newNode;
}

struct Variable * choose( struct Variable * expr1, struct Variable * expr2, struct Variable * expr3, struct Variable * expr4 ){
    if( mut(expr1->val[0][0] - 0) < EPS ){
        return expr2;
    }
    else if( expr1->val[0][0] > 0){
        return expr3;
    }
    else if( expr1->val[0][0] < 0 ){
        return expr4;
    }
    return expr1;
}
void print( struct Variable * a){
    for( int i = 0 ; i < a->dim1 ; i++ ){
        for( int  j = 0 ; j < a->dim2 ; j++ ){
            if(isInteger(a->val[i][j])){
                printf("%d ",(int)a->val[i][j]);
            }
            else{
                printf("%lf ",a->val[i][j]);
            }
        }
        printf("\n");
    }
}
void printsep(){
    printf("------------\n");
}
bool isLower(struct Variable * a, struct Variable * b){
    return (a->val[0][0] <= b->val[0][0]);
}
void increase( struct Variable * a, struct Variable * b){
    a->val[0][0] += b->val[0][0];
    return;
}

struct Variable * getSingleIndexInt(struct Variable * a , int index ){
        index--;
        struct Variable* newNode = (struct Variable*)malloc(sizeof(struct Variable));
        int row = index / a->dim2;
        int column = index % a->dim2;
        newNode->dim1=1;
        newNode->dim2=1;
        newNode->feature=SCA;
        newNode->val[0][0]=a->val[row][column];
        return newNode;
}
struct Variable * getSingleIndex(struct Variable * a , struct Variable * index ){
        int temp = index->val[0][0] - 1;
        struct Variable* newNode = (struct Variable*)malloc(sizeof(struct Variable));
        int row = temp / a->dim2;
        int column = temp % a->dim2;
        newNode->dim1=1;
        newNode->dim2=1;
        newNode->feature=SCA;
        newNode->val[0][0]=a->val[row][column];
        return newNode;
}

struct Variable * getDoubleIndexInt(struct Variable * a , int x, int y ){
        struct Variable* newNode = (struct Variable*)malloc(sizeof(struct Variable));
        newNode->dim1=1;
        newNode->dim2=1;
        newNode->feature=SCA;
        newNode->val[0][0]=a->val[x - 1][y - 1];
        return newNode;
}
struct Variable * getDoubleIndex(struct Variable * a , struct Variable * x, struct Variable * y ){
        struct Variable* newNode = (struct Variable*)malloc(sizeof(struct Variable));
        newNode->dim1=1;
        newNode->dim2=1;
        newNode->feature=SCA;
        newNode->val[0][0]=a->val[(int)x->val[0][0] - 1][(int)y->val[0][0] - 1];
        return newNode;
}
void assign(struct Variable * a, struct Variable * b ){
    a->dim1 = b->dim1;
    a->dim2 = b->dim2;
    a->feature = b->feature;
    for( int i = 0 ; i < 999 ; i++){
        for( int j  = 0 ;  j  < 999 ; j++){
            a->val[i][j] = b->val[i][j];
        }
    }
    return;
}
void assignToFlatten(struct Variable * a , double var , int index ){
    int row = index / a->dim2;
    int column = index % a->dim2;
        
    a->val[row][column] = var ;
    return;
}
void assignToIndex(struct Variable * a, struct Variable * index, struct Variable * var  ){
    a->val[(int)index->val[0][0] - 1][0] = var->val[0][0];
    return;
}
void assignToDoubleIndex(struct Variable * a, struct Variable * index1, struct Variable* index2, struct Variable * var ){
    a->val[(int)index1->val[0][0] - 1][(int)index2->val[0][0] - 1] = var->val[0][0];
    return;
}


int main(){
    printf("%f\n",custom_sqrt(4.0));
    printf("%f\n",custom_sqrt(2.0));
    printf("%f\n",custom_sqrt(0.00001));
    printf("%f\n",custom_sqrt(-2.0));
    printf("%f\n", sqrt(2));
    printf("%f\n", sqrt(0));
    printf("%f\n", sqrt(-2));
    return 0;
    /*
    printf("AAAAAAAAAA");
    // VARIABLE DEFINITIONS
    printf("HERE1\n");
    
    
    struct Variable *sc1 = (struct Variable*)malloc(sizeof(struct Variable));
    sc1->dim1=1;
    sc1->dim2=1;
    sc1->feature=SCA;
    sc1->val[0][0]=5;
    printf("HERE2\n");
    
    struct Variable *sc2 = (struct Variable*)malloc(sizeof(struct Variable));
    sc2->dim1=1;
    sc2->dim2=1;
    sc2->feature=SCA;
    sc2->val[0][0]=3;
    printf("HERE3\n");
    
    
    struct Variable *v1 = (struct Variable*)malloc(sizeof(struct Variable));
    v1->dim1=5;
    v1->dim2=1;
    v1->feature=VEC;
    v1->val[0][0]=1; v1->val[1][0]=2; v1->val[2][0]=3; v1->val[3][0]=4; v1->val[4][0]=5; 

    printf("HERE4\n");
    
    
    struct Variable *v2 = (struct Variable*)malloc(sizeof(struct Variable));
    v2->dim1=4;
    v2->dim2=1;
    v2->feature=VEC;
    v2->val[0][0]= 2; v2->val[1][0]= 3; v2->val[2][0]= 4; v2->val[3][0]= 5; 
    
    printf("HERE5\n");

    
    struct Variable *m1 = (struct Variable*)malloc(sizeof(struct Variable));
    m1->dim1 = 3;
    m1->dim2 = 2;
    m1->feature = MAT;
    m1->val[0][0] = 1; m1->val[0][1] = 2;
    m1->val[1][0] = 3; m1->val[1][1] = 4;
    m1->val[2][0] = 5; m1->val[2][1] = 6;  
    
    printf("HERE6\n");
    
    struct Variable *m2 = (struct Variable*)malloc(sizeof(struct Variable));
    m2->dim1 = 2;
    m2->dim2 = 4;
    m2->feature = MAT;
    m2->val[0][0] = 1; m2->val[0][1] = 2; m2->val[0][2] = 3; m2->val[0][3] = 4;
    m2->val[1][0] = 1; m2->val[1][1] = 2; m2->val[1][2] = 3; m2->val[1][3] = 4;
    printf("HERE7\n");
    
    struct Variable *m3 = (struct Variable*)malloc(sizeof(struct Variable));
    m3->dim1 = 2;
    m3->dim2 = 4;
    m3->feature = MAT;
    m3->val[0][0] = 1; m3->val[0][1] = 2; m3->val[0][2] = 3; m3->val[0][3] = 4;
    m3->val[1][0] = 1; m3->val[1][1] = 2; m3->val[1][2] = 3; m3->val[1][3] = 4;
    printf("HERE7\n");

    
    struct Variable *ans ; // =(struct Variable*)malloc(sizeof(struct Variable));
    
    printf("\n CREATE SCALAR TEST\n");
    ans = createScalar();
    print(ans);

    printf("\n CREATE VECTOR TEST\n");
    ans = createVector(10);
    print(ans);

    printf("\n CREATE MATRIX TEST\n");
    ans = createMatrix(4,3);
    print(ans);    

    assignToFlatten(ans,5 , 8);

    print(ans);    



    printf("\nADDITION TEST\n");
    ans = addition(m2,m3);
    print(ans);

    printf("\nSUBTRACTION TEST\n");
    ans = substraction(m2,m3);
    print(ans);

    printf("\nCHOOSE TEST\n");
    ans = choose(sc1,v1,v2,m1);
    print(ans);

    printf("\nTRANSPOSE TEST VECTOR\n");
    ans = transpose(v1);
    print(ans);

    printf("\nTRANSPOSE TEST MATRIX\n");
    ans = transpose(m1);
    print(ans);

    printf("\nSQUAREROOT TEST\n");
    ans = squareroot(sc1);
    print(ans);

    printf("\nGENERATESCALARFROMNUMBER TEST\n");
    ans = generateScalarFromNumber(5   );
    print(ans);

    printf("\nSCALAR X SCALAR MULTIPLICATION TEST\n");
    ans = multiplication(sc1,sc2);
    print(ans);

    printf("\nMATRIX X SCALAR MULTIPLICATION TEST\n");
    ans = multiplication(m1,sc2);
    print(ans);

    printf("\nSCALAR X VECTOR MULTIPLICATION TEST\n");
    ans = multiplication(sc1,v1);
    print(ans);

    
    printf("\nMATRIX X MATRIX MULTIPLICATION TEST\n");
    ans = multiplication(m1,m2);
    print(ans);
    */



Variable *image = createMatrix(5, 5);
Variable *filter = createMatrix(3, 3);
Variable *temp = createMatrix(3, 3);
Variable *res = createMatrix(3, 3);
Variable *i = createScalar();
Variable *j = createScalar();
Variable *n = createScalar();
Variable *m = createScalar();
Variable *sum = createScalar();
Variable *lrfkQyuQFjKXyQV = generateScalarFromNumber(5);
assign(n, lrfkQyuQFjKXyQV);
Variable *NRTySFrzrmzlYGF = generateScalarFromNumber(5);
assign(m, NRTySFrzrmzlYGF);
Variable *filterSize = createScalar();
Variable *vEulQfpDBHlqDqr = generateScalarFromNumber(3);
assign(filterSize, vEulQfpDBHlqDqr);
assignToFlatten(image, 0, 0);assignToFlatten(image, 1, 1);assignToFlatten(image, 0, 2);assignToFlatten(image, 0, 3);assignToFlatten(image, 0, 4);assignToFlatten(image, 1, 5);assignToFlatten(image, 1, 6);assignToFlatten(image, 1, 7);assignToFlatten(image, 0, 8);assignToFlatten(image, 0, 9);assignToFlatten(image, 0, 10);assignToFlatten(image, 1, 11);assignToFlatten(image, 0, 12);assignToFlatten(image, 1, 13);assignToFlatten(image, 0, 14);assignToFlatten(image, 0, 15);assignToFlatten(image, 0, 16);assignToFlatten(image, 1, 17);assignToFlatten(image, 1, 18);assignToFlatten(image, 1, 19);assignToFlatten(image, 0, 20);assignToFlatten(image, 1, 21);assignToFlatten(image, 0, 22);assignToFlatten(image, 1, 23);assignToFlatten(image, 0, 24);
assignToFlatten(filter, 0, 0);assignToFlatten(filter, 1, 1);assignToFlatten(filter, 0, 2);assignToFlatten(filter, 1, 3);assignToFlatten(filter, 1, 4);assignToFlatten(filter, 1, 5);assignToFlatten(filter, 0, 6);assignToFlatten(filter, 1, 7);assignToFlatten(filter, 0, 8);
Variable *rCRwDnXeuOQqekl = generateScalarFromNumber(1);
Variable *sFyfvlADzPBfudk = generateScalarFromNumber(1);
Variable *AITGDPHCSPIjtHb = addition(sFyfvlADzPBfudk, filterSize);
Variable *KlrwqAOzMiXrpif = substraction(n, AITGDPHCSPIjtHb);
Variable *EffECLhbVFUkBye = generateScalarFromNumber(1);
Variable *qfQOJWTwosILEeZ = generateScalarFromNumber(1);
Variable *CQptkhHqrQdwfcA = generateScalarFromNumber(1);
Variable *TxWJLkNgbqQmBxQ = addition(CQptkhHqrQdwfcA, filterSize);
Variable *YSSYoQcJomwUFBd = substraction(m, TxWJLkNgbqQmBxQ);
Variable *fXudZHiftaKCZVH = generateScalarFromNumber(1);
for(assign(i,rCRwDnXeuOQqekl); isLower(i,KlrwqAOzMiXrpif); increase(i,EffECLhbVFUkBye)){
for(assign(j,qfQOJWTwosILEeZ); isLower(j,YSSYoQcJomwUFBd); increase(j,fXudZHiftaKCZVH)){
Variable *sYBlOetsWCRFHPX = generateScalarFromNumber(1);
Variable *PRbsshSjXDFileB = generateScalarFromNumber(1);
Variable *xwBCTOaYaxzFBjb = getDoubleIndex( image, i, j );
assignToDoubleIndex(temp, sYBlOetsWCRFHPX, PRbsshSjXDFileB, xwBCTOaYaxzFBjb);
Variable *KRxIRImqPZWMSHl = generateScalarFromNumber(1);
Variable *PJhtAZhbuXhwaDL = generateScalarFromNumber(2);
Variable *zGDIxrPDdZpLcrW = generateScalarFromNumber(1);
Variable *PTOyeziWkMGSovq = addition(zGDIxrPDdZpLcrW, j);
Variable *nqwQeCYjyIBfjyk = getDoubleIndex( image, i, PTOyeziWkMGSovq );
assignToDoubleIndex(temp, KRxIRImqPZWMSHl, PJhtAZhbuXhwaDL, nqwQeCYjyIBfjyk);
Variable *MjFQwlTvZKQTPVO = generateScalarFromNumber(1);
Variable *LPHCkCYuFdqMlgl = generateScalarFromNumber(3);
Variable *hcvpFdFbRpzlKvs = generateScalarFromNumber(2);
Variable *ImklfzkTGYgDTTN = addition(hcvpFdFbRpzlKvs, j);
Variable *hwYWSHtDGMBqBKK = getDoubleIndex( image, i, ImklfzkTGYgDTTN );
assignToDoubleIndex(temp, MjFQwlTvZKQTPVO, LPHCkCYuFdqMlgl, hwYWSHtDGMBqBKK);
Variable *XcVGUMONmwvYTBY = generateScalarFromNumber(2);
Variable *tNUqhMFjAQtgnGC = generateScalarFromNumber(1);
Variable *EvHWLeZOHyEehbR = generateScalarFromNumber(1);
Variable *WkuZyaMNeRpHfmw = addition(EvHWLeZOHyEehbR, i);
Variable *cEwjXvCEZiFTIqt = getDoubleIndex( image, WkuZyaMNeRpHfmw, j );
assignToDoubleIndex(temp, XcVGUMONmwvYTBY, tNUqhMFjAQtgnGC, cEwjXvCEZiFTIqt);
Variable *NtFsrPTugtIznOR = generateScalarFromNumber(2);
Variable *VonzjfEaCGAmaya = generateScalarFromNumber(2);
Variable *svNkNBErBltlkGg = generateScalarFromNumber(1);
Variable *pWlMbZitZSZHzKO = addition(svNkNBErBltlkGg, i);
Variable *HyBLjVKyPCFLSAq = generateScalarFromNumber(1);
Variable *dGPLjFiSyLtMmfv = addition(HyBLjVKyPCFLSAq, j);
Variable *EVCiJcYRGMQIrZn = getDoubleIndex( image, pWlMbZitZSZHzKO, dGPLjFiSyLtMmfv );
assignToDoubleIndex(temp, NtFsrPTugtIznOR, VonzjfEaCGAmaya, EVCiJcYRGMQIrZn);
Variable *IAXAKHolawOydVC = generateScalarFromNumber(2);
Variable *HVeiGTtXwpukZjF = generateScalarFromNumber(3);
Variable *ngQVoijxuVqBZtV = generateScalarFromNumber(1);
Variable *hxbRTSpfTToTafs = addition(ngQVoijxuVqBZtV, i);
Variable *YkJqlRzZFmLvYos = generateScalarFromNumber(2);
Variable *aaLSEhZxbSHnRVB = addition(YkJqlRzZFmLvYos, j);
Variable *hiKToDnSjjpQpLC = getDoubleIndex( image, hxbRTSpfTToTafs, aaLSEhZxbSHnRVB );
assignToDoubleIndex(temp, IAXAKHolawOydVC, HVeiGTtXwpukZjF, hiKToDnSjjpQpLC);
Variable *ikLZqRXLOQXrudY = generateScalarFromNumber(3);
Variable *gJTyzleiZMeaINx = generateScalarFromNumber(1);
Variable *xJyGhRVEuVPHKnQ = generateScalarFromNumber(2);
Variable *slwhHJWslQENDjV = addition(xJyGhRVEuVPHKnQ, i);
Variable *tSDtwxCktMwWwSD = getDoubleIndex( image, slwhHJWslQENDjV, j );
assignToDoubleIndex(temp, ikLZqRXLOQXrudY, gJTyzleiZMeaINx, tSDtwxCktMwWwSD);
Variable *ThZmLMbHjkmOUhp = generateScalarFromNumber(3);
Variable *bQURQfXGqlOJmwS = generateScalarFromNumber(2);
Variable *LHHDKBDxQGrgEdp = generateScalarFromNumber(2);
Variable *OMOwsjVPVHznbsi = addition(LHHDKBDxQGrgEdp, i);
Variable *ezNWhPIiduxDNnL = generateScalarFromNumber(1);
Variable *zchRGEFEuKmcOWo = addition(ezNWhPIiduxDNnL, j);
Variable *BNmyJYsSbsocOCD = getDoubleIndex( image, OMOwsjVPVHznbsi, zchRGEFEuKmcOWo );
assignToDoubleIndex(temp, ThZmLMbHjkmOUhp, bQURQfXGqlOJmwS, BNmyJYsSbsocOCD);
Variable *ZCuuNkRFDuvOuag = generateScalarFromNumber(3);
Variable *HhcYvMLkzAAJpFP = generateScalarFromNumber(3);
Variable *ISwJuteNUycpbcN = generateScalarFromNumber(2);
Variable *YljtYJJpYntsefX = addition(ISwJuteNUycpbcN, i);
Variable *myWegMtuNodVuZY = generateScalarFromNumber(2);
Variable *MHfUqMmIdmVkNyx = addition(myWegMtuNodVuZY, j);
Variable *GvGuxtRdSDFzFSS = getDoubleIndex( image, YljtYJJpYntsefX, MHfUqMmIdmVkNyx );
assignToDoubleIndex(temp, ZCuuNkRFDuvOuag, HhcYvMLkzAAJpFP, GvGuxtRdSDFzFSS);
Variable *zVnDtaURdKugsBD = generateScalarFromNumber(1);
Variable *PAwxitivdUbbqEO = generateScalarFromNumber(1);
Variable *mElUODJgdGZfMRA = getDoubleIndex( temp, zVnDtaURdKugsBD, PAwxitivdUbbqEO );
Variable *raoheuCSVPitEqR = generateScalarFromNumber(1);
Variable *SwGKaaAOHXxZhQJ = generateScalarFromNumber(1);
Variable *NYCAEGxfJKKlrfk = getDoubleIndex( filter, raoheuCSVPitEqR, SwGKaaAOHXxZhQJ );
Variable *TkqAqHkWBerBPMG = multiplication(mElUODJgdGZfMRA, NYCAEGxfJKKlrfk);
Variable *lKYrDEjAQufOwbI = generateScalarFromNumber(1);
Variable *GRsNjniEGVDVOTu = generateScalarFromNumber(2);
Variable *lbJIpNUjYwoGWCz = getDoubleIndex( temp, lKYrDEjAQufOwbI, GRsNjniEGVDVOTu );
Variable *riXibbDFRZUQSYr = generateScalarFromNumber(1);
Variable *FQGHoyqEvCuaNUu = generateScalarFromNumber(2);
Variable *GoCedktCBBufNXo = getDoubleIndex( filter, riXibbDFRZUQSYr, FQGHoyqEvCuaNUu );
Variable *JszitaOaowSXyGl = multiplication(lbJIpNUjYwoGWCz, GoCedktCBBufNXo);
Variable *yQIgNPITruiJVYl = generateScalarFromNumber(1);
Variable *LSiboBJLTuSRypA = generateScalarFromNumber(3);
Variable *aFBWzDDoznRvJQe = getDoubleIndex( temp, yQIgNPITruiJVYl, LSiboBJLTuSRypA );
Variable *dtyOZoOlzSlGDgo = generateScalarFromNumber(1);
Variable *wijATKlVJzSCIzR = generateScalarFromNumber(3);
Variable *NVyBsfRXTLfmPDI = getDoubleIndex( filter, dtyOZoOlzSlGDgo, wijATKlVJzSCIzR );
Variable *kuPMSoxFTumyxif = multiplication(aFBWzDDoznRvJQe, NVyBsfRXTLfmPDI);
Variable *yUNxUCubVkfcTkq = addition(kuPMSoxFTumyxif, JszitaOaowSXyGl);
Variable *lRoqGZJVjWzIzpp = addition(yUNxUCubVkfcTkq, TkqAqHkWBerBPMG);
assign(sum, lRoqGZJVjWzIzpp);
Variable *hFJtBnwEdtuByNS = generateScalarFromNumber(2);
Variable *bIrepgcxfgsfomH = generateScalarFromNumber(1);
Variable *VSomFlViOEMYcnP = getDoubleIndex( temp, hFJtBnwEdtuByNS, bIrepgcxfgsfomH );
Variable *siBBEaxniwjkfvA = generateScalarFromNumber(2);
Variable *BnRlLkMAglYtHkG = generateScalarFromNumber(1);
Variable *vpMymKdoHeTtyyC = getDoubleIndex( filter, siBBEaxniwjkfvA, BnRlLkMAglYtHkG );
Variable *lAUZGKeulyRPaEu = multiplication(VSomFlViOEMYcnP, vpMymKdoHeTtyyC);
Variable *hMfRjijaNxKHRqd = generateScalarFromNumber(2);
Variable *lLecyhBsUxnlftM = generateScalarFromNumber(2);
Variable *RDvExQLwGAkDTBi = getDoubleIndex( temp, hMfRjijaNxKHRqd, lLecyhBsUxnlftM );
Variable *UhKkiOVYtgaufPj = generateScalarFromNumber(2);
Variable *lXIWbNZhYBSxfMU = generateScalarFromNumber(2);
Variable *jCNYYBwSjmAJbWt = getDoubleIndex( filter, UhKkiOVYtgaufPj, lXIWbNZhYBSxfMU );
Variable *mbHkjqMDAbmyULb = multiplication(RDvExQLwGAkDTBi, jCNYYBwSjmAJbWt);
Variable *HvhsGzVwiiXxaOB = generateScalarFromNumber(2);
Variable *hfsOpqnebMflCoo = generateScalarFromNumber(3);
Variable *rgLWStjkOxBCZkj = getDoubleIndex( temp, HvhsGzVwiiXxaOB, hfsOpqnebMflCoo );
Variable *dgZdMQHogGVRvjQ = generateScalarFromNumber(2);
Variable *RPMXbhKKfgZZXjE = generateScalarFromNumber(3);
Variable *etJIZolqrmsXpHQ = getDoubleIndex( filter, dgZdMQHogGVRvjQ, RPMXbhKKfgZZXjE );
Variable *GSYoVdRmwcjAvpM = multiplication(rgLWStjkOxBCZkj, etJIZolqrmsXpHQ);
Variable *shoJZXAxnBiztkF = addition(GSYoVdRmwcjAvpM, mbHkjqMDAbmyULb);
Variable *OMZdhUjDMCyxdQT = addition(shoJZXAxnBiztkF, lAUZGKeulyRPaEu);
Variable *EQjalGQgsoMONvb = addition(OMZdhUjDMCyxdQT, sum);
assign(sum, EQjalGQgsoMONvb);
Variable *UdVLznfHbaOKxVC = generateScalarFromNumber(3);
Variable *elYkfhXbLdylqqE = generateScalarFromNumber(1);
Variable *rEQqzZpwqLiHDNV = getDoubleIndex( temp, UdVLznfHbaOKxVC, elYkfhXbLdylqqE );
Variable *QLFvObEyolyYoVV = generateScalarFromNumber(3);
Variable *iWHXfPBuiuJLoLn = generateScalarFromNumber(1);
Variable *wdNJzrlbSKqgFVn = getDoubleIndex( filter, QLFvObEyolyYoVV, iWHXfPBuiuJLoLn );
Variable *JLdgVWXLJboAYPA = multiplication(rEQqzZpwqLiHDNV, wdNJzrlbSKqgFVn);
Variable *ELIumZPNOIEIpog = generateScalarFromNumber(3);
Variable *wILASWNTYwueGDG = generateScalarFromNumber(2);
Variable *oTdZJxxrschMWRV = getDoubleIndex( temp, ELIumZPNOIEIpog, wILASWNTYwueGDG );
Variable *DGxAiGMXPyvyaqg = generateScalarFromNumber(3);
Variable *aUlToDtlgzcYvfI = generateScalarFromNumber(2);
Variable *yeTHsRznlZrfFmw = getDoubleIndex( filter, DGxAiGMXPyvyaqg, aUlToDtlgzcYvfI );
Variable *ykMklLfbZXqYhvI = multiplication(oTdZJxxrschMWRV, yeTHsRznlZrfFmw);
Variable *tzYwUeypMabiNMy = generateScalarFromNumber(3);
Variable *appzAEcVMxiRqsm = generateScalarFromNumber(3);
Variable *ZQMAMJZLvVgoiFl = getDoubleIndex( temp, tzYwUeypMabiNMy, appzAEcVMxiRqsm );
Variable *heKNevXTEHXtbth = generateScalarFromNumber(3);
Variable *QKjvtFzDDLgCHxC = generateScalarFromNumber(3);
Variable *zcUIDDymnLfusKi = getDoubleIndex( filter, heKNevXTEHXtbth, QKjvtFzDDLgCHxC );
Variable *ZOhPYeWfMUfzAZy = multiplication(ZQMAMJZLvVgoiFl, zcUIDDymnLfusKi);
Variable *xTQxCuWBRXpFymU = addition(ZOhPYeWfMUfzAZy, ykMklLfbZXqYhvI);
Variable *vFvsyxRrcfuUsIC = addition(xTQxCuWBRXpFymU, JLdgVWXLJboAYPA);
Variable *CzAQHlSwbZiEVIJ = addition(vFvsyxRrcfuUsIC, sum);
assign(sum, CzAQHlSwbZiEVIJ);
Variable *TYZfFEcdKSUXTee = generateScalarFromNumber(5);
Variable *hRVdUDVMaRaVupi = substraction(sum, TYZfFEcdKSUXTee);
Variable *YPBNKCyPqODXrRW = generateScalarFromNumber(1);
Variable *IkFUXwjHTUjRLIu = generateScalarFromNumber(2);
Variable *aifPpzVkmXyspWV = generateScalarFromNumber(3);
Variable *ITxtgWdFuydrTbF = choose( hRVdUDVMaRaVupi, YPBNKCyPqODXrRW, IkFUXwjHTUjRLIu, aifPpzVkmXyspWV );
assignToDoubleIndex(res, i, j, ITxtgWdFuydrTbF);
}}
print(res);


    

    
    

}


