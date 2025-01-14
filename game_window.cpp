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
    logger.out("show_destroy");
//    _getch();
    closegraph();
}
void exit_program()
{
    MAIN = 0;
    GAME_START = 0;
    exit(0);
}
void main_window()
{
    logger.out("(enter window) -> in main_window");
    if(MAIN == 1)
    {
        show_init();
        Buttons butts;
        butts.setButton(GAP, GAP+(ROW-1)*(LINE_WIDTH+GRID_SIZE), 80, 40, RED, _T("start"));
        butts.setButton(GAP+(COLUMN-1)*(LINE_WIDTH*GRID_SIZE), GAP+(ROW-1)*(LINE_WIDTH+GRID_SIZE), 80, 40, RGB(70, 120, 120), _T("exit"));
        butts.buttons[0].setFun_back(game_start_window_button, "click button: [start] -> game_start_window");
        butts.buttons[1].setFun_back(exit_program, "click button: [exit] -> exit_program");
        butts.draw({0, false, false, false}); //绘制初始按钮，传入无效m
        while(MAIN == 1)
        {
            MOUSEMSG m = GetMouseMsg();
            BeginBatchDraw();
            cleardevice();
            butts.draw(m);
            butts.trigger(m);
            EndBatchDraw();
        }
    }
    show_destroy();
}
void game_start_window()
{
    logger.out("(enter window) -> in game_start_window");
    while(GAME_START == 1)
    {
        show_init();
        ChessBoard game;
        logger.out(game.Boom_message());
        game.run();
        show_destroy();
    }
}
void main_window_button()
{
    MAIN = 1;
    GAME_START = 0;
}
void game_start_window_button()
{
    MAIN = 0;
    GAME_START = 1;
}