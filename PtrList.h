#ifndef PtrListDefinition
#define PtrListDefinition
typedef struct {
    int size;
    void** list;
} PtrList;
#endif

extern int get();

extern PtrList* newPtrList();

extern char* getAsString();

extern int getAsInt();

extern void freeList();

extern int append();