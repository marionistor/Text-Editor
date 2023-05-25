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
    for (i = 0; i < myFileData->numOfLines; i++) {
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

void highlightApparitions(FileData *myFileData)
{

    int yMax, xMax;
    getmaxyx(stdscr, xMax, yMax);
    
    char word[100];
    int index = 0;

    wattron(stdscr, COLOR_PAIR(3));
    mvwprintw(stdscr, xMax - 2, 1, "Introduceti cuvantul pentru cautare: ");
    wattroff(stdscr, COLOR_PAIR(3));
    wmove(stdscr, xMax - 2, 38);
    refresh();

    int newChar = wgetch(stdscr);
    int y = 38;
    while (newChar != '\n') {
        if (newChar == KEY_BACKSPACE) {
            if (index != 0) {
                y--;
                mvwprintw(stdscr, xMax - 2, y, " ");
                wmove(stdscr, xMax - 2, y);
                index--;
            }
        } else {
            wattron(stdscr, COLOR_PAIR(3));
            mvwaddch(stdscr, xMax - 2, y, newChar);
            wattroff(stdscr, COLOR_PAIR(3));
            refresh();
            y++;
            word[index++] = newChar;
        }
        newChar = wgetch(stdscr);
    }
    word[index] = '\0';
    int i;

    for (i = 0; i < xMax; i++) {
        int positions[100];
        ListNode *currentLine = myFileData->fileLines[i].head;
        char line[500];
        int index = 0;
        while (currentLine) {
            line[index++] = currentLine->Chr;
            currentLine = currentLine->next;
        }
        line[index] = '\0';
        index = 0;
        int y = 0, j = 0;
        while (j < strlen(line)) {
            if (word[index] == line[j]) {
                if (index == strlen(word) - 1) {
                    positions[index] = j;
                    index = 0;
                    int k;
                    for (k = 0; k < strlen(word); k++) {
                        wattron(stdscr, COLOR_PAIR(3));
                        mvwaddch(stdscr, i, positions[k], word[k]);
                        wattroff(stdscr, COLOR_PAIR(3));
                    }
                    y++;
                    j = y;    
                } else {
                    positions[index++] = j;
                    j++;
                } 
            } else {
                index = 0;
                y++;
                j = y;
            }
        }
    }
    refresh();
    wattron(stdscr, COLOR_PAIR(3));
    mvwprintw(stdscr, xMax - 1, 1, "Apasa orice tasta pentru a iesi!");
    wattroff(stdscr, COLOR_PAIR(3));
    refresh();
    wgetch(stdscr);
    printFileDataOnStdScr(myFileData);
    wmove(stdscr, myFileData->xCursor, myFileData->yCursor);
    refresh();
}

void removeLine(FileData *myFileData)
{
    int i;

    int index = 0;
    ListNode *tempNode = myFileData->fileLines[myFileData->xCursor].head;
    while (tempNode) {
        myFileData->buffer[index++] = tempNode->Chr;
        tempNode = tempNode->next;
    }
    myFileData->buffer[index] = '\0';

    for (i = myFileData->xCursor; i < myFileData->numOfLines - 1; i++) {
        myFileData->fileLines[i] = myFileData->fileLines[i + 1];
    }

    myFileData->fileLines[myFileData->numOfLines - 1].head = NULL;
    myFileData->fileLines[myFileData->numOfLines - 1].tail = NULL;
    myFileData->fileLines[myFileData->numOfLines - 1].numOfNodes = 0;
    myFileData->yCursor = 0;

    if (myFileData->numOfLines > 1 && myFileData->xCursor != myFileData->numOfLines - 1) {
        myFileData->numOfLines--;
    }
}

void removeWord(FileData *myFileData)
{
    int x = myFileData->xCursor;
    int y = myFileData->yCursor;

    int i;

    if (y == myFileData->fileLines[x].numOfNodes) {
        return;
    }

    ListNode *tempNode = myFileData->fileLines[x].head;

    for (i = 0; i < y; i++) {
        tempNode = tempNode->next;
    }

    if (tempNode->Chr == ' ') {
        return;
    }

    ListNode *start = tempNode, *end = tempNode;
    int lastWord = 0;
    int firstWord = 0;

    int leftChars = 0;
    int rightChars = 0;

    while (start->Chr != ' ') {
        start = start->prev;
        leftChars++;
        if (start == NULL) {
            firstWord = 1;
            break;
        }
    }

    while (end->Chr != ' ') {
        end = end->next;
        rightChars++;
        if (end == NULL) {
            lastWord = 1;
            break;
        }
    }

    if (start == NULL) {
        tempNode = myFileData->fileLines[x].head;
    } else {
        tempNode = start->next;
    }
    int index = 0;
    while (tempNode != end) {
        myFileData->buffer[index++] = tempNode->Chr;
        tempNode = tempNode->next;
    }
    myFileData->buffer[index] = '\0';

    if (!firstWord && !lastWord) {
        tempNode = start->next;
        while (tempNode != end) {
            ListNode *aux = tempNode;
            tempNode = tempNode->next;
            free(aux);
        }
        start->next = end;
        end->prev = start;

        int y = 0;
        tempNode = myFileData->fileLines[x].head;
        while (tempNode != start) {
            y++;
            tempNode = tempNode->next;
        } 

        printFileDataOnStdScr(myFileData);
        myFileData->yCursor = y;
        myFileData->fileLines[x].numOfNodes = myFileData->fileLines[x].numOfNodes - rightChars - leftChars + 1;
        wmove(stdscr, myFileData->xCursor, y);
        refresh();
        return;
    }

    if (firstWord && lastWord) {
        removeLine(myFileData);
        printFileDataOnStdScr(myFileData);
        wmove(stdscr, myFileData->xCursor, myFileData->yCursor);
        refresh();
        return;
    }

    if (firstWord) {
        ListNode *aux = myFileData->fileLines[x].head;

        while (aux != end) {
            ListNode *temp = aux;
            aux = aux->next;
            free(temp);
        }

        myFileData->fileLines[x].head = end;
        end->prev = NULL;

        myFileData->fileLines[x].numOfNodes = myFileData->fileLines[x].numOfNodes - leftChars - rightChars + 1;
        myFileData->yCursor = 0;

        printFileDataOnStdScr(myFileData);
        wmove(stdscr, myFileData->xCursor, 0);
        refresh();
        return;
    }

    if (lastWord) {
        ListNode *aux = start->next;

        while (aux) {
            ListNode *temp = aux;
            aux = aux->next;
            free(temp);
        }

        start->next = NULL;
        myFileData->fileLines[x].tail = start;
        myFileData->fileLines[x].numOfNodes = myFileData->fileLines[x].numOfNodes - leftChars - rightChars + 1;
        
        int y = 0;
        aux = myFileData->fileLines[x].head;

        while (aux != start) {
            y++;
            aux = aux->next;
        }

        myFileData->yCursor = y;
        printFileDataOnStdScr(myFileData);
        wmove(stdscr, myFileData->xCursor, myFileData->yCursor);
        refresh();
        return;
    }
}

void pasteBuffer(FileData *myFileData)
{
    int i;
    for (i = 0; i < strlen(myFileData->buffer); i++) {
        insert_char(myFileData, myFileData->buffer[i]);
    }
    return;
}    

void printFileDataOnStdScr(FileData *myFileData)
{
    clear();
    refresh();   
    int i;
    for (i = 0; i < myFileData->numOfLines; i++) {
        ListNode *tempNode = myFileData->fileLines[i].head;

        int y = 0;
        while (tempNode) {
            mvprintw(i, y, "%c", tempNode->Chr);
            y++;
            tempNode = tempNode->next;
        }
    }
}

void printHighlghtedFileDataOnStdScr(FileData *myFileData, int firstX, int firstY, int secondX, int secondY)
{
    clear();
    refresh();   
    int i;

    int highlighted = 0;
    for (i = 0; i < myFileData->numOfLines; i++) {
        ListNode *tempNode = myFileData->fileLines[i].head;

        int y = 0;
        while (tempNode) {

            if (i < firstX || i > secondX) {
                highlighted = 0;
            } else if (i == firstX && y < firstY) {
                highlighted = 0;
            } else if (i == secondX && y > secondY) {
                highlighted = 0;
            } else {
                highlighted = 1;
            }

            if (highlighted) {
                wattron(stdscr, COLOR_PAIR(3));
                mvprintw(i, y, "%c", tempNode->Chr);
                wattroff(stdscr, COLOR_PAIR(3));
            }

            if (!highlighted) {
                mvprintw(i, y, "%c", tempNode->Chr);
            }

            y++;
            tempNode = tempNode->next;
        }
    }
}

void getBuffer(FileData *myFileData, int firstX, int firstY, int secondX, int secondY)
{
    int highlighted = 0;
    int i;
    int index = 0;

    for (i = 0; i < myFileData->numOfLines; i++) {
        ListNode *tempNode = myFileData->fileLines[i].head;

        int y = 0;
        while (tempNode) {

            if (i < firstX || i > secondX) {
                highlighted = 0;
            } else if (i == firstX && y < firstY) {
                highlighted = 0;
            } else if (i == secondX && y > secondY) {
                highlighted = 0;
            } else {
                highlighted = 1;
            }

            if (highlighted) {
                myFileData->buffer[index++] = tempNode->Chr;
            }

            y++;
            tempNode = tempNode->next;
        }

    }
    myFileData->buffer[index] = '\0';
}

void copyText(FileData *myFileData)
{
    curs_set(0);
    int firstX = myFileData->xCursor;
    int firstY = myFileData->yCursor;

    ListNode *tempNode = myFileData->fileLines[firstX].head;

    if (myFileData->fileLines[myFileData->xCursor].numOfNodes == myFileData->yCursor) {
        curs_set(1);
        return;
    }

    int i;
    for (i = 0; i < firstY; i++) {
        tempNode = tempNode->next;
    }

    wattron(stdscr, COLOR_PAIR(3));
    mvprintw(firstX, firstY, "%c", tempNode->Chr);
    wattroff(stdscr, COLOR_PAIR(3));
    refresh();


    int newChar;
    while (newChar = wgetch(stdscr)) {
        switch(newChar) 
        {
            case KEY_RIGHT:

                int tempNum = myFileData->fileLines[myFileData->xCursor].numOfNodes;
                if (tempNum > myFileData->yCursor) {
                    wmove(stdscr, myFileData->xCursor, ++myFileData->yCursor);
                } else {
                    break;
                }

                if (firstX < myFileData->xCursor) {
                    printHighlghtedFileDataOnStdScr(myFileData, firstX, firstY, myFileData->xCursor, myFileData->yCursor);
                } else if (firstX > myFileData->xCursor) {
                    printHighlghtedFileDataOnStdScr(myFileData, myFileData->xCursor, myFileData->yCursor, firstX, firstY);
                } else if (firstY < myFileData->yCursor) {
                    printHighlghtedFileDataOnStdScr(myFileData, firstX, firstY, myFileData->xCursor, myFileData->yCursor);
                } else {
                    printHighlghtedFileDataOnStdScr(myFileData, myFileData->xCursor, myFileData->yCursor, firstX, firstY);
                }
                wmove(stdscr, myFileData->xCursor, myFileData->yCursor);
                refresh();

                break;
            
            case KEY_LEFT:

                if (myFileData->yCursor) {
                    wmove(stdscr, myFileData->xCursor, --myFileData->yCursor);
                } else {
                    break;
                }

                if (firstX < myFileData->xCursor) {
                    printHighlghtedFileDataOnStdScr(myFileData, firstX, firstY, myFileData->xCursor, myFileData->yCursor);
                } else if (firstX > myFileData->xCursor) {
                    printHighlghtedFileDataOnStdScr(myFileData, myFileData->xCursor, myFileData->yCursor, firstX, firstY);
                } else if (firstY < myFileData->yCursor) {
                    printHighlghtedFileDataOnStdScr(myFileData, firstX, firstY, myFileData->xCursor, myFileData->yCursor);
                } else {
                    printHighlghtedFileDataOnStdScr(myFileData, myFileData->xCursor, myFileData->yCursor, firstX, firstY);
                }
                wmove(stdscr, myFileData->xCursor, myFileData->yCursor);
                refresh();

                break;

            case '\n':
                
                if (firstX < myFileData->xCursor) {
                    getBuffer(myFileData, firstX, firstY, myFileData->xCursor, myFileData->yCursor);
                } else if (firstX > myFileData->xCursor) {
                    getBuffer(myFileData, myFileData->xCursor, myFileData->yCursor, firstX, firstY);
                } else if (firstY < myFileData->yCursor) {
                    getBuffer(myFileData, firstX, firstY, myFileData->xCursor, myFileData->yCursor);
                } else {
                    getBuffer(myFileData, myFileData->xCursor, myFileData->yCursor, firstX, firstY);
                }

                
                printFileDataOnStdScr(myFileData);
                curs_set(1);
                refresh();
                return;
        }
    }
}