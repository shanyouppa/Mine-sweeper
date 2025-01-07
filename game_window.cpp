
#include "game_window.h"
#include "chessboard.h"
#include "component.h"
void show_init()
{
    initgraph(COLUMN*GRID_SIZE+ (COLUMN+1)*LINE_WIDTH+ 2*GAP, ROW*GRID_SIZE+ (ROW+1)*LINE_WIDTH+ 2*GAP);
    setbkcolor(WHITE);
    cleardevice();
}
void show_destroy()
{
    _getch();
    closegraph();
}
void exit_program()
{
    exit(0);
}
void window_main()
{
    show_init();
    Buttons butts;
    butts.setButton(GAP, GAP+(ROW-1)*(LINE_WIDTH+GRID_SIZE), 80, 40, _T("start"));
    butts.setButton(GAP+(COLUMN-1)*(LINE_WIDTH*GRID_SIZE), GAP+(ROW-1)*(LINE_WIDTH+GRID_SIZE), 80, 40, _T("exit"));
    butts.buttons[0].setfunback(game_start_window);
    butts.buttons[1].setfunback(exit_program);
    while(1)
    {
        MOUSEMSG m = GetMouseMsg();
        BeginBatchDraw();
        cleardevice();
        butts.draw(m);
        EndBatchDraw();
    }
    show_destroy();
}
void game_start_window()
{
    ChessBoard game;
    show_init();
    game.run();
    show_destroy();
}