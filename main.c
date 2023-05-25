#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include "include/list.h"
#include "include/utils.h"

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

    return 0;
}