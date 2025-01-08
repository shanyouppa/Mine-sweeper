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
//    logger.out("show_destroy");
//    _getch();
    closegraph();
}
void exit_program()
{
    exit(0);
}
void main_window()
{
    logger.out("(enter window) -> main_window");
    Buttons butts;
    show_init();
    butts.setButton(GAP, GAP+(ROW-1)*(LINE_WIDTH+GRID_SIZE), 80, 40, RED, _T("start"));
    butts.setButton(GAP+(COLUMN-1)*(LINE_WIDTH*GRID_SIZE), GAP+(ROW-1)*(LINE_WIDTH+GRID_SIZE), 80, 40, RGB(128, 128, 128), _T("exit"));
    butts.buttons[0].setFun_back(game_start_window, "click button: [start] -> game_start_window");
    butts.buttons[1].setFun_back(exit_program, "click button: [exit] -> exit_program");
    butts.draw({0, false, false, false}); //绘制初始按钮，传入无效m
    while(true)
    {
        MOUSEMSG m = GetMouseMsg();
        BeginBatchDraw();
        cleardevice();
        butts.draw(m);
        EndBatchDraw();
    }
}
void game_start_window()
{
    logger.out("(enter window) -> game_start_window");
    ChessBoard game;
    closegraph();
    show_init();
    game.run();
    show_destroy();
}