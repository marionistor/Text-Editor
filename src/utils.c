#include "../include/utils.h"

#define MIN_NUM_OF_LINES 500

// height = 20, width = 45 
#define HEIGHT 20
#define WIDTH 45

#define ctrl(x) ((x) & 0x1f)

void init_color_pairs(void) 
{
    start_color();
    init_pair(1, COLOR_BLACK, COLOR_CYAN);
    init_pair(2, COLOR_BLACK, COLOR_YELLOW);
    init_pair(3, COLOR_BLACK, COLOR_GREEN);
    init_pair(4, COLOR_WHITE, COLOR_BLACK);
    init_pair(5, COLOR_BLACK, COLOR_BLUE);
    init_pair(6, COLOR_BLACK, COLOR_WHITE);
    init_pair(7, COLOR_WHITE, COLOR_BLUE);
}

void save_file(FileData *myFileData)
{
    FILE *outputFile = fopen("untitled.txt", "w");
    int i;
    for (i = 0; i < myFileData->numOfLines; i++) {
        ListNode *tempNode = myFileData->fileLines[i].head;

        while (tempNode) {
            putc(tempNode->Chr, outputFile);
            tempNode = tempNode->next;
        }
        putc('\n', outputFile);
    }

    fclose(outputFile);
    free_FileData(myFileData);
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

void goToLine(FileData *myFileData)
{
    int yMax, xMax;
    getmaxyx(stdscr, xMax, yMax);

    wattron(stdscr, COLOR_PAIR(3));
    mvwprintw(stdscr, xMax - 2, 1, "Go to line: ");
    wattroff(stdscr, COLOR_PAIR(3));
    refresh();

    char number[300];

    char newChar = wgetch(stdscr);
    
    int y = 28;
    int index = 0;
    while (newChar != '\n') {
        if (strchr("1234567890", newChar) == NULL) {
            wattron(stdscr, COLOR_PAIR(3));
            mvwprintw(stdscr, xMax - 1, 1, "Enter a number!!!");
            wattroff(stdscr, COLOR_PAIR(3));
            refresh();
        } else {
            mvwprintw(stdscr, xMax - 1, 1, "                         ");
            refresh();
            wmove(stdscr, xMax - 2, 28);
            wattron(stdscr, COLOR_PAIR(3));
            mvwaddch(stdscr, xMax - 2, y, newChar);
            wattroff(stdscr, COLOR_PAIR(3));
            refresh();
            y++;
            number[index++] = newChar;
        }
        newChar = wgetch(stdscr);
    }

    number[index] = '\0';
    int num = atoi(number);

    if (num >= myFileData->numOfLines) {
        wattron(stdscr, COLOR_PAIR(3));
        mvwprintw(stdscr, xMax - 1, 1, "The line doesn't exist!!! Press any key to exit!");
        wattroff(stdscr, COLOR_PAIR(3));
        wgetch(stdscr);
        printFileDataOnStdScr(myFileData);
        wmove(stdscr, myFileData->xCursor, myFileData->yCursor);
        refresh();
        return;
    }

    myFileData->yCursor = 0;
    myFileData->xCursor = num;
    printFileDataOnStdScr(myFileData);
    wmove(stdscr, num, 0);
    refresh();
}

int isValidChar(char c)
{
    char specialChars[] = "!@#$%%^&*()-_+=`~{}[]\\|:;'\",./<>? ";
    if ((c < 'a' || c > 'z') && (c < 'A' || c > 'Z') && (c < '0' || c > '9') && (strchr(specialChars, c) == NULL)) {
        return 0;
    }
    return 1;    
}

void highlightApparitions(FileData *myFileData)
{

    int yMax, xMax;
    getmaxyx(stdscr, xMax, yMax);
    
    char word[100];
    int index = 0;

    wattron(stdscr, COLOR_PAIR(3));
    mvwprintw(stdscr, xMax - 2, 1, "Enter a word: ");
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
        } else if (isValidChar(newChar)) {
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
    mvwprintw(stdscr, xMax - 1, 1, "Press any key to exit!");
    wattroff(stdscr, COLOR_PAIR(3));
    refresh();
    wgetch(stdscr);
    printFileDataOnStdScr(myFileData);
    wmove(stdscr, myFileData->xCursor, myFileData->yCursor);
    refresh();
}

void menu(FileData *myFileData)
{
    clear();
    refresh();
    int xStart, yStart, xMax, yMax;
    getmaxyx(stdscr, xMax, yMax);
    xStart = (xMax - HEIGHT)/2;
    yStart = (yMax - WIDTH)/2;
    WINDOW *win = newwin(HEIGHT, WIDTH, xStart, yStart);
    box(win, 0, 0);
    getmaxyx(win, xMax, yMax);
    wbkgd(win, COLOR_PAIR(2));
    wrefresh(win);
    curs_set(0);
    mvwprintw(win, 4, (yMax - 30) / 2, "Use arrows to select an option");
    mvwprintw(win, 5, (yMax - 16) / 2, "Then press enter");
    mvwprintw(win, 8, (yMax - 4) / 2, "EXIT");
    mvwprintw(win, 10, (yMax - 6) / 2, "CANCEL");
    wrefresh(win);

    keypad(win, true);
    int ok = -1;
    int newChar;
    while ((newChar = wgetch(win))) {
        if (newChar == KEY_UP) {
            wattron(win, A_STANDOUT);
            mvwprintw(win, 8, (yMax - 4) / 2, "EXIT");
            wattroff(win, A_STANDOUT);
            mvwprintw(win, 10, (yMax - 6) / 2, "CANCEL");
            wrefresh(win);
            ok = 1;
        } else if (newChar == KEY_DOWN) {
            mvwprintw(win, 8, (yMax - 4) / 2, "EXIT");
            wattron(win, A_STANDOUT);
            mvwprintw(win, 10, (yMax - 6) / 2, "CANCEL");
            wattroff(win, A_STANDOUT);
            wrefresh(win);
            ok = 0;
        } else if (newChar == '\n' && ok == 1) {
            int i, j;
            for (i = 3; i <= 10; i++) {
                for (j = (yMax - 30) / 2; j < (yMax - 30) / 2 + 30; j++) {
                    mvwprintw(win, i, j, " ");
                }
            }
            mvwprintw(win, 6, (yMax - 30) / 2, "Do you want to save this file?");
            mvwprintw(win, 8, (yMax - 3) / 2, "YES");
            mvwprintw(win, 10, (yMax - 2) / 2, "NO");
            wrefresh(win);

            int ok1 = -1;
            int newChar1;
            while ((newChar1 = wgetch(win))) {
                if (newChar1 == KEY_UP) {
                    wattron(win, A_STANDOUT);
                    mvwprintw(win, 8, (yMax - 3) / 2, "YES");
                    wattroff(win, A_STANDOUT);
                    mvwprintw(win, 10, (yMax - 2) / 2, "NO");
                    wrefresh(win);
                    ok1 = 1;
              } else if ((newChar1 == KEY_DOWN)) {
                    mvwprintw(win, 8, (yMax - 3) / 2, "YES");
                    wattron(win, A_STANDOUT);
                    mvwprintw(win, 10, (yMax - 2) / 2, "NO");
                    wattroff(win, A_STANDOUT);
                    wrefresh(win);
                    ok1 = 0;
              } else if (newChar1 == '\n' && ok1 == 1) {
                    save_file(myFileData);
                    endwin();
                    exit(0);
              } else if (newChar1 == '\n' && ok1 == 0) {
                    free_FileData(myFileData);
		    endwin();
                    exit(0);
              }  
            }       
        } else if (newChar == '\n' && ok == 0) {
            erase();
            curs_set(1);
            refresh();
            printFileDataOnStdScr(myFileData);
            wmove(stdscr, myFileData->xCursor, myFileData->yCursor);
            refresh();
            return;       
        }
    }
}

void getCharsFromKeyboard(FileData *myFileData)
{
    wmove(stdscr, myFileData->xCursor, myFileData->yCursor);
    refresh();

    int xMax, yMax;
    getmaxyx(stdscr, xMax, yMax);

    while (true) {
        int newChar = wgetch(stdscr);
        switch (newChar)
        {
        case ctrl('i'):
            copyText(myFileData);
            wmove(stdscr, myFileData->xCursor, myFileData->yCursor);
            break;
        case ctrl('l'):
            goToLine(myFileData);
            break;
        case ctrl('v'): // paste buffer la pozita cursorului
            pasteBuffer(myFileData);
            printFileDataOnStdScr(myFileData);
            wmove(stdscr, myFileData->xCursor, myFileData->yCursor);
            break;  

        case ctrl('f'):
            highlightApparitions(myFileData);
            break;

        case KEY_F(12):
            menu(myFileData);
            break;

        case ctrl('w'):
            removeWord(myFileData);
            break;

        case ctrl('k'):
            if (myFileData->numOfLines == myFileData->xCursor) {
                break;
            }

            removeLine(myFileData);
            printFileDataOnStdScr(myFileData);
            wmove(stdscr, myFileData->xCursor, myFileData->yCursor);
            refresh();
            break;

        case KEY_DOWN:
            if (myFileData->xCursor == xMax - 3) {
                break;
            }

            if (myFileData->numOfLines == myFileData->xCursor + 1) {
                wmove(stdscr, ++myFileData->xCursor, 0);
                myFileData->yCursor = 0;
                myFileData->numOfLines++;
                refresh();
            } else {
                int tempNum = myFileData->fileLines[myFileData->xCursor + 1].numOfNodes;
                if (tempNum > myFileData->yCursor) {
                    wmove(stdscr, ++myFileData->xCursor, myFileData->yCursor);
                } else {
                    wmove(stdscr, ++myFileData->xCursor, tempNum);
                    myFileData->yCursor = tempNum;
                }
                refresh();
            }
            break;

        case KEY_RIGHT:
            int tempNum = myFileData->fileLines[myFileData->xCursor].numOfNodes;
            if (tempNum > myFileData->yCursor) {
                wmove(stdscr, myFileData->xCursor, ++myFileData->yCursor);
                refresh();
            } else {
                myFileData->xCursor++;
                myFileData->numOfLines++;
                myFileData->yCursor = 0;
                wmove(stdscr, myFileData->xCursor, myFileData->yCursor);
                refresh();
            }
            break;
        
        case KEY_LEFT:
            if (myFileData->yCursor) {
                wmove(stdscr, myFileData->xCursor, --myFileData->yCursor);
                refresh();
            } else if (myFileData->xCursor) {
                int tempNum = myFileData->fileLines[myFileData->xCursor - 1].numOfNodes;
                myFileData->xCursor--;
                myFileData->yCursor = tempNum;
                wmove(stdscr, myFileData->xCursor, myFileData->yCursor);
                refresh();
            }
            break;

        case KEY_UP:
            if (myFileData->xCursor) {
                int tempNum = myFileData->fileLines[myFileData->xCursor - 1].numOfNodes;
                if (tempNum > myFileData->yCursor) {
                    wmove(stdscr, --myFileData->xCursor, myFileData->yCursor);
                } else {
                    wmove(stdscr, --myFileData->xCursor, tempNum);
                    myFileData->yCursor = tempNum;
                }
                refresh();
            }
            break;
        
        case '\n':
            if (myFileData->xCursor == xMax - 3) {
                break;
            }
            keyEnter(myFileData);
            printFileDataOnStdScr(myFileData);
            wmove(stdscr, myFileData->xCursor, myFileData->yCursor);
            refresh();
            break;

        case KEY_BACKSPACE:
            if (myFileData->yCursor) {
                remove_char(myFileData);
                printFileDataOnStdScr(myFileData);
                wmove(stdscr, myFileData->xCursor, myFileData->yCursor);
                refresh();
            } else if (myFileData->xCursor) {
                keyBackspace(myFileData);
                printFileDataOnStdScr(myFileData);
                wmove(stdscr, myFileData->xCursor, myFileData->yCursor);
                refresh();
            }
            break;

        default:
            wmove(stdscr, myFileData->xCursor, myFileData->yCursor + 1);
            insert_char(myFileData, newChar);
            printFileDataOnStdScr(myFileData);
            wmove(stdscr, myFileData->xCursor, myFileData->yCursor);
            refresh();
            break;
        }
    }
}

