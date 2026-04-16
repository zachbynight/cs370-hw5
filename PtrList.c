#include <stdlib.h>
#include "PtrList.h"
#include <stdio.h>


PtrList* newPtrList(int size) {
    PtrList* newPtrListPtr = malloc(sizeof(newPtrList));
    (*newPtrListPtr).size = size;
    // These are all null pointers by default
    // Null pointers cause a seg fault if you try to follow them
    (*newPtrListPtr).list = malloc(size * sizeof(void*));
    for (int i = 0; i < size; i++) {
        newPtrListPtr->list[i] = NULL;
    }
    return newPtrListPtr;
}



char** getString(PtrList list, int i) {
    if (i >= list.size) {
        printf("Error: index %d exceeds bounds %d\n", i, list.size);
        return NULL;
    }
    return (char**) (list.list[i]);
}

int* getInt(PtrList list, int i) {
    if (i >= list.size) {
        printf("Error: index %d exceeds bounds %d\n", i, list.size);
        return NULL;
    }
    return (int*) (list.list[i]);
}

void freeList(PtrList list) {
    for (int i = 0; i < list.size; i++) {
        free(list.list[i]);
    }
}

int compareInts(const void* p, const void* q) {
    if  (*((void**) p) == NULL) {
        return 1;
    }
    if  (*((void**) q) == NULL) {
        return -1;
    }
    int a = *((int*) *((void**) p));
    int b = *((int*) *((void**) q));
    return a - b;
}

void sortIntList(PtrList* listPtr) {
    qsort((*listPtr).list, (*listPtr).size, sizeof(void*), compareInts);
}

// assumes list is already sorted, with nulls at the back
int append(PtrList* listPtr, void* p) {
    for (int i = 0; i < (*listPtr).size; i++) {
        if ((*listPtr).list[i] != NULL) {
            continue;
        }
        (*listPtr).list[i] = p;
        return i;
    }
}

/*int main(int argCount, char** args) {
    PtrList* strListPtr = newPtrList(5);
    PtrList strList = *strListPtr;
    int* raw = malloc((argCount - 1) * sizeof(char*));
    for (int i = 1; i < argCount; i++) {
        raw[i - 1] = args[i];
    }
    for (int i = 0; i < 5; i++) {
        strList.list[i] = &(raw[i]);
    }
    qsort(strList.list, 5, sizeof(void*), strcmp);
    for (int i = 0; i < 5; i++) {
        printf("%s\n", *(getInt(strList, i)));
    }
}*/

/*int main(int argCount, char** args) {
    PtrList list = *newPtrList(5);
    for (int i = 0; i < 5; i++) {
        printf("%d\n", NULL == getInt(list, i));
    }
}*/