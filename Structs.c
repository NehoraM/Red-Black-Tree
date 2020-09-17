#include "Structs.h"
#include "stdlib.h"
#include "string.h"


double fineNorm(Vector *vec);

/**
 * CompFunc for strings (assumes strings end with "\0")
 * @param a - char* pointer
 * @param b - char* pointer
 * @return equal to 0 iff a == b. lower than 0 if a < b. Greater than 0 iff b < a. (lexicographic
 * order)
 */
int stringCompare(const void *a, const void *b){
    char *str1=(char *)a;
    char *str2=(char *)b;

    double diff = strcmp(str1, str2);
    if (diff < 0)
    {
        return -1;
    }
    else if (diff > 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }

}

/**
 * ForEach function that concatenates the given word and \n to pConcatenated. pConcatenated is
 * already allocated with enough space.
 * @param word - char* to add to pConcatenated
 * @param pConcatenated - char*
 * @return 0 on failure, other on success
 */
int concatenate(const void *word, void *pConcatenated){
    if(pConcatenated==NULL || word==NULL){
        return 0;
    }
    char *str1=(char*) word;
    char *str2=(char*) pConcatenated;
    char *temp="\n";

    strcat(str2,temp);
    strcat(str2,str1);

    word=(void*) str2;
    return 1;
}

/**
 * FreeFunc for strings
 */
void freeString(void *s){
    free(s);
}

/**
 * CompFunc for Vectors, compares element by element, the vector that has the first larger
 * element is considered larger. If vectors are of different lengths and identify for the length
 * of the shorter vector, the shorter vector is considered smaller.
 * @param a - first vector
 * @param b - second vector
 * @return equal to 0 iff a == b. lower than 0 if a < b. Greater than 0 iff b < a.
 */
int vectorCompare1By1(const void *a, const void *b){
    Vector *vec1=(Vector *)a;
    Vector *vec2=(Vector *)b;
    if(vec1->len < vec2->len){
        return -1;
    }
    else if(vec1->len > vec2->len){
        return 1;
    }
    else{
        for(int i=0 ; i<vec1->len ; i++){
            if(vec1->vector[i] < vec2->vector[i]){
                return 1;
            }
            if(vec1->vector[i] > vec2->vector[i]){
                return -1;
            }
        }
    }
    return 0;
}

/**
 * FreeFunc for vectors
 */
void freeVector(void *pVector){
    free(pVector);
}

/**
 * copy pVector to pMaxVector if : 1. The norm of pVector is greater then the norm of pMaxVector.
 * 								   2. pMaxVector->vector == NULL.
 * @param pVector pointer to Vector
 * @param pMaxVector pointer to Vector that will hold a copy of the data of pVector.
 * @return 1 on success, 0 on failure (if pVector == NULL || pMaxVector==NULL: failure).
 */
int copyIfNormIsLarger(const void *pVector, void *pMaxVector){
    if(pMaxVector==NULL || pVector==NULL){
        return 0;
    }
    Vector *vec=(Vector *)pVector;
    Vector *maxVec=(Vector *)pMaxVector;
    double vecNorm= fineNorm(vec);
    double maxVecNorm= fineNorm(maxVec);
    if(vecNorm > maxVecNorm ){
        vec->len=maxVec->len;
        vec->vector=maxVec->vector;
        for(int i=0 ; i<vec->len ; i++){
            vec->vector[i]=maxVec->vector[i];
        }
    }
    return 1;
}


double fineNorm(Vector *vec){
    if(vec==NULL){
        return -1;
    }
    double norm=0;
    for(int i=0 ; i<vec->len ; i++){
        norm=norm+ (vec->vector[i]*vec->vector[i]);
    }
    return norm;
}
/**
 * This function allocates memory it does not free.
 * @param tree a pointer to a tree of Vectors
 * @return pointer to a *copy* of the vector that has the largest norm (L2 Norm), NULL on failure.
 */
Vector *findMaxNormVectorInTree(RBTree *tree){
    Node *z=tree->root;
    for (; z != NULL; z = z->right);
    Vector *new=tree->root->data;
    copyIfNormIsLarger(new,z);
    return new;
}// You must use copyIfNormIsLarger in the implementation!


