#ifndef LIST_H_
#define LIST_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ncurses.h>

typedef struct FileData {
    int xCursor;
    int yCursor;
    int numOfLines;
    struct LinkedList *fileLines;
} FileData;

typedef struct LinkedList {
    int numOfNodes;
    struct ListNode *head;
    struct ListNode *tail;
} LinkedList;

typedef struct ListNode {
    char Chr;
    struct ListNode *next;
    struct ListNode *prev;
} ListNode;

FileData* initFile(void);
void free_FileData(FileData *myFileData);
void insert_char(FileData *myFileData, char newChar);
void remove_char(FileData *myFileData);
void printFileDataOnStdScr(FileData *myFileData);
void keyEnter(FileData *myFileData);
void keyBackspace(FileData *myFileData);

#endif