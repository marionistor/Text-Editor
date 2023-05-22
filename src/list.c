#include "list.h"

#define MIN_NUM_OF_LINES 500

FileData* initFile(void)
{
    FileData *myFileData = malloc(sizeof(FileData));
    myFileData->xCursor = 0;
    myFileData->yCursor = 0;
    myFileData->fileLines = malloc(MIN_NUM_OF_LINES * sizeof(LinkedList));
    myFileData->numOfLines = MIN_NUM_OF_LINES;

    int i;
    for (i = 0; i < MIN_NUM_OF_LINES; i++) {
        myFileData->fileLines[i].head = NULL;
        myFileData->fileLines[i].tail = NULL;
        myFileData->fileLines[i].numOfNodes = 0;
    }

    return myFileData;
}

// eliberarea memoriei
void free_FileData(FileData *myFileData)
{
    int i;
    for (i = 0; i < MIN_NUM_OF_LINES; i++) {
        ListNode *aux = myFileData->fileLines[i].head;
        while (aux) {
            ListNode *temp = aux;
            aux = aux->next;
            free(temp);
        }
    }
    free(myFileData->fileLines);
    free(myFileData);
}

// insereaza un caracter in functie de pozitia cursorului
void insert_char(FileData *myFileData, char newChar)
{
    int x = myFileData->xCursor;
    int y = myFileData->yCursor;
    LinkedList currentLine = myFileData->fileLines[x];
    int nodesCount = currentLine.numOfNodes;
    ListNode *newNode = malloc(sizeof(newNode));
    newNode->Chr = newChar; 
    if (nodesCount == 0) {
        currentLine.head = newNode;
        currentLine.head->next = currentLine.head->prev = NULL;
        currentLine.tail = currentLine.head;
    } else if (y == 0) {
        newNode->prev = NULL;
        newNode->next = currentLine.head;
        currentLine.head->prev = newNode;
        currentLine.head = newNode;
    } else if (y == nodesCount) {
        newNode->prev = currentLine.tail;
        newNode->next = NULL;
        currentLine.tail->next = newNode;
        currentLine.tail = newNode;
    } else {
        ListNode *aux;
        int i;
        if (y <= nodesCount / 2) {
            aux = currentLine.head;
            for (i = 0; i < y - 1; i++) {
                aux = aux->next;
            }
            newNode->prev = aux;
            newNode->next = aux->next;
            aux->next->prev = newNode;
            aux->next = newNode;
        } else {
            aux = currentLine.tail;
            for (i = nodesCount - 1; i > y; i--) {
                aux = aux->prev;               
            }
            newNode->prev = aux->prev;
            newNode->next = aux;
            aux->prev->next = newNode;
            aux->prev = newNode;
        }
    }
    myFileData->yCursor++;
    currentLine.numOfNodes++;
}

// sterge un caracter in functie de pozitia cursorului
void remove_char(FileData *myFileData)
{
    int y = myFileData->yCursor;
    int x = myFileData->xCursor;
    LinkedList currentLine = myFileData->fileLines[x];
    int nodesCount = currentLine.numOfNodes;
    ListNode *aux;
    if (y == 0) {
        aux = currentLine.head;
        currentLine.head = currentLine.head->next;
        currentLine.head->prev = NULL;
        free(aux);
    } else if (y == nodesCount - 1) {
        aux = currentLine.tail;
        currentLine.tail = currentLine.tail->prev;
        currentLine.tail->next = NULL;
        free(aux);
    } else {
        ListNode *temp;
        int i;
        if (y <= nodesCount / 2) {
            aux = currentLine.head;
            for (i = 0; i < y - 1; i++) {
                aux = aux->next;
            }
            temp = aux->next;
            temp->next->prev = aux;
            temp->prev->next = temp->next;
            free(temp);
        } else {
            aux = currentLine.tail;
            for (i = nodesCount - 1; i > y + 1; i--) {
                aux = aux->prev;
            }
            temp = aux->prev;
            temp->prev->next = aux;
            temp->next->prev = temp->prev;
            free(temp); 
        }
    }  
    myFileData->yCursor--;
    currentLine.numOfNodes--;
}