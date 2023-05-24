#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include "list.h"

#define ctrl(x) ((x) & 0x1f)

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

void goToLine(FileData *myFileData)
{
    int yMax, xMax;
    getmaxyx(stdscr, xMax, yMax);

    wattron(stdscr, COLOR_PAIR(3));
    mvwprintw(stdscr, xMax - 2, 1, "Introduceti linia cautata: ");
    wattroff(stdscr, COLOR_PAIR(3));
    refresh();

    char number[300];

    char newChar = wgetch(stdscr);
    
    int y = 28;
    int index = 0;
    while (newChar != '\n') {
        if (strchr("1234567890", newChar) == NULL) {
            wattron(stdscr, COLOR_PAIR(3));
            mvwprintw(stdscr, xMax - 1, 1, "Introduceti doar cifre!!!");
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
        mvwprintw(stdscr, xMax - 1, 1, "Linia introdusa nu exista!!! Apasa orice tasta pentru a iesi!");
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

void init_color_pairs (void) 
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
            //wgetch(stdscr);
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

        case KEY_F(5):
            save_file(myFileData);
            exit(0);

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

int main(void)
{
    initscr();
    cbreak();
    noecho();
    init_color_pairs();

    refresh();
    keypad(stdscr, true);

    FileData *myFileData = initFile();

    getCharsFromKeyboard(myFileData);

    //while (1);

    return 0;
}