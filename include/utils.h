#ifndef UTILS_H_
#define UTILS_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ncurses.h>
#include "list.h"

void init_color_pairs (void);
void save_file(FileData *myFileData);
void printFileDataOnStdScr(FileData *myFileData);
void keyEnter(FileData *myFileData);
void keyBackspace(FileData *myFileData);
void removeLine(FileData *myFileData);
void removeWord(FileData *myFileData);
void pasteBuffer(FileData *myFileData);
void printHighlghtedFileDataOnStdScr(FileData *myFileData, int firstX, int firstY, int secondX, int secondY);
void getBuffer(FileData *myFileData, int firstX, int firstY, int secondX, int secondY);
void copyText(FileData *myFileData);
void goToLine(FileData *myFileData);
int isValidChar(char c);
void highlightApparitions(FileData *myFileData);
void menu(FileData *myFileData);
void getCharsFromKeyboard(FileData *myFileData);

#endif