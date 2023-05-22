#include "list.h"

#define MIN_NUM_OF_LINES 100

FileData* initFile(void)
{
    FileData *myFileData = malloc(sizeof(FileData));
    myFileData->xCursor = 0;
    myFileData->yCursor = 0;
    myFileData->fileLines = malloc(MIN_NUM_OF_LINES * sizeof(LinkedList));
    myFileData->numOfLines = MIN_NUM_OF_LINES;

    int i;
    for (i = 0; i < MIN_NUM_OF_LINES; i++) {
        myFileData->fileLines[i].firstNode = NULL;
        myFileData->fileLines[i].lastNode = NULL;
        myFileData->fileLines[i].numOfNodes = 0;
    }

    return myFileData;
}

void insert_char(FileData *myFileData, char newChar)
{
    // insereaza un caracter in functie de pozitia cursorului
}

void remove_char(FileData *myFileData)
{
    // sterge un caracter in functie de pozitia cursorului
}