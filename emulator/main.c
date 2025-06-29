#include <curses.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "emulator.h"

struct {
    uint8_t fooo;
} d;


struct Memory Memory_instance;
struct Context Context_instance;
void test();

int main(int argc, char *argv[]) {
    test_instruction();

    enum { N = 12};                                     // Hello world! 文字数.
    int  x, y, w, h, count = 0;
    initscr();                                          // curses:(スクリーン)初期化.
    noecho();                                           // curses:キー入力で表示を行わない.
    cbreak();                                           // curses:入力バッファリングしない.
    curs_set(0);                                        // curses:カーソルを表示しない.
    getmaxyx(stdscr, h, w);                             // curses:画面サイズ取得.
    x = (w - N) / 2;                                    // x初期位置:画面中央.
    y = (h - 1) / 2;                                    // y初期位置:画面中央.
    start_color();			/* Start color 			*/
    init_pair(1, COLOR_WHITE, COLOR_RED );
    init_pair(2, COLOR_WHITE, 8 );
    init_emulator();

    for (int i=0; i < 8; i++) UpdateDisplay(i);
    int pos = 0;
    /*
    for (int i=0; i< 1000; i++) {
        usleep(15000);
        if ((i & 0xff) == 0xff) {
            pos++;
            if (pos==8) pos = 0;
        }
        MemoryManager_ByteWrite(0x83f8 + pos, i & 0xff);
    }*/
    getch();
    endwin();                                           // curses 終了.
    return 0;
    printf("Hello from emulator!\n");
    return 0;
}




