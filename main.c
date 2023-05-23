#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include "list.h"

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

    while (true) {
        int newChar = wgetch(stdscr);
        switch (newChar)
        {
        case KEY_DOWN:
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