#include "list.h"

#define MIN_NUM_OF_LINES 500

FileData* initFile(void)
{
    FileData *myFileData = malloc(sizeof(FileData));
    myFileData->xCursor = 0;
    myFileData->yCursor = 0;
    myFileData->fileLines = malloc(MIN_NUM_OF_LINES * sizeof(LinkedList));
    myFileData->numOfLines = 1;

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

    LinkedList* currentLine = &myFileData->fileLines[x];
    int nodesCount = currentLine->numOfNodes;
    ListNode *newNode = malloc(sizeof(newNode));
    newNode->Chr = newChar; 

    if (nodesCount == 0) {
        currentLine->head = newNode;
        currentLine->head->next = currentLine->head->prev = NULL;
        currentLine->tail = currentLine->head;
    } else if (y == 0) {
        newNode->prev = NULL;
        newNode->next = currentLine->head;
        currentLine->head->prev = newNode;
        currentLine->head = newNode;
    } else if (y == nodesCount) {
        newNode->prev = currentLine->tail;
        newNode->next = NULL;
        currentLine->tail->next = newNode;
        currentLine->tail = newNode;
    } else {
        ListNode *aux;
        int i;
        if (y <= nodesCount / 2) {
            aux = currentLine->head;
            for (i = 0; i < y - 1; i++) {
                aux = aux->next;
            }
            newNode->prev = aux;
            newNode->next = aux->next;
            aux->next->prev = newNode;
            aux->next = newNode;
        } else {
            aux = currentLine->tail;
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
    currentLine->numOfNodes++;
}

// sterge un caracter in functie de pozitia cursorului
void remove_char(FileData *myFileData)
{
    int y = myFileData->yCursor;
    int x = myFileData->xCursor;
    LinkedList *currentLine = &myFileData->fileLines[x];
    int nodesCount = currentLine->numOfNodes;
    ListNode *aux;
    if (y == 0) {
        // TODO
    } else if (y == 1) {
        if (nodesCount == 1) {
            aux = currentLine->head;
            free(aux);
            currentLine->head = NULL;
            currentLine->tail = NULL;
        } else {
            aux = currentLine->head;
            currentLine->head = currentLine->head->next;
            currentLine->head->prev = NULL;
            free(aux);
        }
    } else if (y == nodesCount) {
        aux = currentLine->tail;
        currentLine->tail = currentLine->tail->prev;
        currentLine->tail->next = NULL;
        free(aux);
    } else {
        ListNode *temp;
        int i;
        aux = currentLine->head;
        for (i = 0; i < y - 2; i++) {
            aux = aux->next;
        }
        temp = aux->next;
        temp->next->prev = aux;
        temp->prev->next = temp->next;
        free(temp);
    }  
    myFileData->yCursor--;
    currentLine->numOfNodes--;
}

void keyEnter(FileData *myFileData)
{
    ListNode *tempNode = myFileData->fileLines[myFileData->xCursor].head;
    int i;

    int x = myFileData->xCursor;
    int y = myFileData->yCursor;

    if (y == 0) {
        for (i = myFileData->numOfLines; i > x; i--) {
            myFileData->fileLines[i] = myFileData->fileLines[i - 1];
        }
        myFileData->fileLines[x].head = NULL;
        myFileData->fileLines[x].tail = NULL;
        myFileData->fileLines[x].numOfNodes = 0;
        myFileData->numOfLines++;
        myFileData->xCursor++;
        return;
    }

    if (y == myFileData->fileLines[x].numOfNodes) {
        for (i = myFileData->numOfLines; i > x + 1; i--) {
            myFileData->fileLines[i] = myFileData->fileLines[i - 1];
        }
        myFileData->fileLines[x + 1].head = NULL;
        myFileData->fileLines[x + 1].tail = NULL;
        myFileData->fileLines[x + 1].numOfNodes = 0;
        myFileData->numOfLines++;
        myFileData->xCursor++;
        myFileData->yCursor = 0;
        return;
    }

    for (i = myFileData->numOfLines; i > x + 1; i--) {
        myFileData->fileLines[i] = myFileData->fileLines[i - 1];
    }

    for (i = 0; i < y - 1; i++) {
        tempNode = tempNode->next;
    }

    myFileData->fileLines[x + 1].head = tempNode->next;
    tempNode->next->prev = NULL;
    tempNode->next = NULL;

    myFileData->fileLines[x + 1].numOfNodes = myFileData->fileLines[x].numOfNodes - y;
    myFileData->fileLines[x + 1].tail = myFileData->fileLines[x].tail;
    myFileData->fileLines[x].numOfNodes = y;
    myFileData->fileLines[x].tail = tempNode;

    myFileData->xCursor++;
    myFileData->yCursor = 0;

    myFileData->numOfLines++;
}

void keyBackspace(FileData *myFileData)
{
    int x = myFileData->xCursor;
    int y = myFileData->yCursor;

    if (myFileData->fileLines[x - 1].numOfNodes == 0) {
        int i;
        for (i = x - 1; i < myFileData->numOfLines - 1; i++) {
            myFileData->fileLines[i] = myFileData->fileLines[i + 1];
        }
        myFileData->fileLines[myFileData->numOfLines - 1].head = NULL;
        myFileData->fileLines[myFileData->numOfLines - 1].tail = NULL;
        myFileData->fileLines[myFileData->numOfLines - 1].numOfNodes = 0;
        myFileData->xCursor--;
        myFileData->numOfLines--;
        return;
    }

    if (myFileData->fileLines[x].numOfNodes == 0) {
        int i;
        for (i = x; i < myFileData->numOfLines - 1; i++) {
            myFileData->fileLines[i] = myFileData->fileLines[i + 1];
        }

        myFileData->xCursor--;
        myFileData->yCursor = myFileData->fileLines[x - 1].numOfNodes;
        myFileData->numOfLines--;
        return;
    }

    myFileData->fileLines[x - 1].tail->next = myFileData->fileLines[x].head;
    myFileData->fileLines[x].head->prev = myFileData->fileLines[x - 1].tail;
    myFileData->fileLines[x - 1].tail = myFileData->fileLines[x].tail;

    myFileData->yCursor = myFileData->fileLines[x - 1].numOfNodes;
    myFileData->fileLines[x - 1].numOfNodes += myFileData->fileLines[x].numOfNodes;

    int i;
    for (i = x; i < myFileData->numOfLines - 1; i++) {
        myFileData->fileLines[i] = myFileData->fileLines[i + 1];
    }

    myFileData->fileLines[myFileData->numOfLines - 1].head = NULL;
    myFileData->fileLines[myFileData->numOfLines - 1].tail = NULL;
    myFileData->fileLines[myFileData->numOfLines - 1].numOfNodes = 0;

    myFileData->numOfLines--;
    myFileData->xCursor--;
}

void printFileDataOnStdScr(FileData *myFileDta)
{
    clear();
    refresh();   
    int i;
    for (i = 0; i < myFileDta->numOfLines; i++) {
        ListNode *tempNode = myFileDta->fileLines[i].head;

        int y = 0;
        while (tempNode) {
            mvprintw(i, y, "%c", tempNode->Chr);
            y++;
            tempNode = tempNode->next;
        }
    }
}