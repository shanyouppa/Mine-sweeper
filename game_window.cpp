#include "game_window.h"
#include "chessboard.h"
#include "component.h"
#include"myJson.h"


void show_init()
{
    initgraph(COLUMN*GRID_SIZE+ (COLUMN+1)*LINE_WIDTH+ 2*GAP, ROW*GRID_SIZE+ (ROW+1)*LINE_WIDTH+ 2*GAP);
    setbkcolor(WHITE);
    cleardevice();
}
void show_clear()
{
    logger.out("clear window");
//    _getch();
    setbkcolor(WHITE);
    cleardevice();
}
void exit_program()
{
    MAIN = 0;
    GAME_START = 0;
    closegraph();
    logger.out("closegraph, termination of program");
    exit(0);
}
void main_window()
{
    logger.out("(enter window) -> in main_window");
    if(MAIN == 1)
    {
        Buttons butts;
        butts.setButton(GAP, GAP+(ROW-1)*(LINE_WIDTH+GRID_SIZE), 80, 40, RED, _T("start"));
        butts.setButton(GAP+4*(LINE_WIDTH*GRID_SIZE), GAP+(ROW-1)*(LINE_WIDTH+GRID_SIZE), 80, 40, BLUE, _T("setting"));
        butts.setButton(GAP+(COLUMN-1)*(LINE_WIDTH*GRID_SIZE), GAP+(ROW-1)*(LINE_WIDTH+GRID_SIZE), 80, 40, RGB(70, 120, 120), _T("exit"));
        butts.buttons[0].setFun_back(game_start_window_button, "click button: [start] -> game_start_window");
        butts.buttons[1].setFun_back(setting_window_button, "click button: [start] -> setting_window");
        butts.buttons[2].setFun_back(exit_program, "click button: [exit] -> exit_program");
        butts.draw({0, false, false, false}); //绘制初始界面，传入无效m
        while(MAIN == 1)
        {
            MOUSEMSG m = GetMouseMsg();
            BeginBatchDraw();
            cleardevice();
            butts.draw(m);
            EndBatchDraw();
            butts.trigger(m);
        }
    }
    show_clear();
}
void game_start_window()
{
    logger.out("(enter window) -> in game_start_window");
    while(GAME_START == 1)
    {
        ChessBoard game;
        logger.out(game.Boom_message());
        game.run();
        show_clear();
    }
}
void main_window_button()
{
    MAIN = 1;
    GAME_START = 0;
    SET = 0;
}
void game_start_window_button()
{
    MAIN = 0;
    GAME_START = 1;
    SET = 0;
}
void setting_window_button()
{
    MAIN = 0;
    GAME_START = 0;
    SET = 1;
}
void setting_window()
{
    if(SET)
    {
        TextBoxs ts;
        setbkmode(OPAQUE);
        settextcolor(BLACK);
        ts.set_TextBox(GAP+(COLUMN-5)*(LINE_WIDTH+GRID_SIZE), GAP+LINE_WIDTH+GRID_SIZE, GAP+(COLUMN-1)*(LINE_WIDTH+GRID_SIZE), GAP+2*(LINE_WIDTH+GRID_SIZE), 140, _T("ROW"));
        ts.set_TextBox(GAP+(COLUMN-5)*(LINE_WIDTH+GRID_SIZE), GAP+3*(LINE_WIDTH+GRID_SIZE), GAP+(COLUMN-1)*(LINE_WIDTH+GRID_SIZE), GAP+4*(LINE_WIDTH+GRID_SIZE), 140, _T("COLUMN"));

        Buttons bs;
        bs.setButton(GAP, GAP+(ROW-1)*(LINE_WIDTH+GRID_SIZE), 80, 40, GREEN, _T("save"));
        bs.setButton(GAP+(COLUMN-1)*(LINE_WIDTH*GRID_SIZE), GAP+(ROW-1)*(LINE_WIDTH+GRID_SIZE), 80, 40, RED, _T("exit"));
        bs.setButton(GAP, GAP+(ROW-1)*(LINE_WIDTH+GRID_SIZE), 80, 40, RGB(128, 128, 128), _T("save")); //buttons[0]的复制，表示无法保存
        auto saveFunction_button = [ObjectPtr = &ts]
        {
            ObjectPtr->save_date();
            closegraph();
            main_window_button();
            readMyJson();
            show_init();
        };
        bs.buttons[0].setFun_back(saveFunction_button);
        bs.buttons[1].setFun_back(main_window_button);

        ts.draw();
        bs.buttons[2].draw({0, false, false, false});
        bs.buttons[1].draw({0, false, false, false});  //绘制初始界面

        while (SET)
        {
            if(MouseHit())
            {
                MOUSEMSG m = GetMouseMsg();
                ts.trigger(m);
                if(!ts.textboxs[0].text_is_empty() && !ts.textboxs[1].text_is_empty())
                {
                    bs.buttons[0].LMouse(m);
                }
                bs.buttons[1].LMouse(m);


                BeginBatchDraw();
                cleardevice();
                ts.draw();
                if(!ts.textboxs[0].text_is_empty() && !ts.textboxs[1].text_is_empty())
                {
                    bs.buttons[0].draw(m);
                    bs.buttons[1].draw(m);
                }
                else
                {
                    bs.buttons[2].draw({0, false, false, false});
                    bs.buttons[1].draw(m);
                }
                EndBatchDraw();
            }
            if(kbhit())
            {
                ts.inputs();
                BeginBatchDraw();
                cleardevice();
                ts.draw();
                if(!ts.textboxs[0].text_is_empty() && !ts.textboxs[1].text_is_empty())
                {
                    bs.buttons[0].draw({0, false, false, false});
                    bs.buttons[1].draw({0, false, false, false});
                }
                else
                {
                    bs.buttons[2].draw({0, false, false, false});
                    bs.buttons[1].draw({0, false, false, false});
                }
                EndBatchDraw();
            }
        }
        show_clear();

    }

}