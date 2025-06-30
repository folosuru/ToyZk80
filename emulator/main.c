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
    Byte program[] = {
        0x3e,
        0x80,
        0x06,
        0x70,
        0x0e,
        0x7f,
        0x21,
        0xff,
        0x83,
        0x90,
        0xc2,
        0x0e,
        0x80,
        0x0c,
        0x71,
        0x76
    };
    memcpy(Memory_instance.program, &program, sizeof(program));
    for (int i=0; i < 8; i++) UpdateDisplay(i);
    int pos = 0;
    mainloop();
    getch();
    endwin();                                           // curses 終了.
    return 0;
    printf("Hello from emulator!\n");
    return 0;
}




