#ifndef LIST_H_
#define LIST_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct FileData {
    int xCursor;
    int yCursor;
    int numOfLines;
    struct LinkedList *fileLines;
} FileData;

typedef struct LinkedList {
    int numOfNodes;
    ListNode *head;
    ListNode *tail;
} LinkedList;

typedef struct ListNode {
    char Chr;
    ListNode *next;
    ListNode *prev;
} ListNode;

FileData* initFile(void);
void free_FileData(FileData *myFileData);
void insert_char(FileData *myFileData, char newChar);
void remove_char(FileData *myFileData);

#endif