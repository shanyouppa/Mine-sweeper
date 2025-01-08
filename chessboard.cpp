#include "chessboard.h"
#include "component.h"
#include "game_window.h"
int ChessBoard::x_tmp = 0;
int ChessBoard::y_tmp = 0;
ChessBoard::ChessBoard()
{
    find = 0;
    int site;
    srand(time(nullptr));
    for(int i=0;i<BOOM; )
    {
        site = rand()%(ROW*COLUMN);
        int x = site/ROW;
        int y = site%COLUMN;
        if(board[x][y].is_boom == 0)
        {
            board[x][y].is_boom = 1;  //设置boom
            i++;
            for(int dx: dxdy)   //每个boom周围+1
            {
                if(x+dx>=0&&x+dx<ROW)
                {
                    for(int dy: dxdy)
                    {
                        if(y+dy>=0&&y+dy<COLUMN)
                        {
                            board[x+dx][y+dy].other_boom++;
                        }
                    }
                }
            }
        }

    }
    logger.out("chessboard construct success!");
}
void ChessBoard::show_line()
{
    setfillcolor(RGB(128, 128, 128));
//        solidrectangle(GAP, GAP, GAP+ COLUMN*(LINE_WIDTH+ GRID_SIZE), GAP+ ROW*(LINE_WIDTH+ GRID_SIZE));
    setlinecolor(BLACK); //横线竖线
    setlinestyle(PS_SOLID, LINE_WIDTH);
    for(int i = GAP;i<=GAP+ ROW*(LINE_WIDTH+ GRID_SIZE);i += LINE_WIDTH+ GRID_SIZE)
    {
        line(GAP, i, GAP+ COLUMN*(LINE_WIDTH+ GRID_SIZE), i);
    }
    for(int j = GAP;j<=GAP+ COLUMN*(LINE_WIDTH+ GRID_SIZE);j += LINE_WIDTH+ GRID_SIZE)
    {
        line(j, GAP, j, GAP+ ROW*(LINE_WIDTH+ GRID_SIZE));
    }
}
void ChessBoard::show_state() //show chessboard
{
    show_line();
    for(int x = 0;x<ROW;x++)
    {
        for(int y = 0;y<COLUMN;y++)
        {
            int im_x = GAP+LINE_WIDTH+y*(GRID_SIZE+LINE_WIDTH);
            int im_y = GAP+LINE_WIDTH+x*(GRID_SIZE+LINE_WIDTH);
            if(board[x][y].state == 0)
            {
                if(board[x][y].mouse_over == 0)
                {
                    setfillcolor(RGB(128, 128, 128));
                    solidrectangle(im_x, im_y, im_x+GRID_SIZE-1, im_y+GRID_SIZE-1);
                }
                else if(board[x][y].mouse_over == 1)
                {
                    setfillcolor(YELLOW);
                    solidrectangle(im_x, im_y, im_x+GRID_SIZE-1, im_y+GRID_SIZE-1);
                }
                if(board[x][y].flag == 1)
                {
                    setfillcolor(RED);
                    solidcircle(im_x+ GRID_SIZE/2, im_y+ GRID_SIZE/2, 10);
                }
            }
            else if(board[x][y].state == 1)
            {
                if(board[x][y].is_boom == 0)
                {
                    setfillcolor(WHITE);
                    solidrectangle(im_x, im_y, im_x+GRID_SIZE-1, im_y+GRID_SIZE-1);
                    if(board[x][y].other_boom != 0)
                    {
                        char text = '0' + board[x][y].other_boom;
                        if(board[x][y].other_boom != 0)
                        {
                            char text = '0' + board[x][y].other_boom;
                            settextcolor(BLUE);
                            outtextxy(im_x+GRID_SIZE/2, im_y+GRID_SIZE/2, text);
                        }   outtextxy(im_x+GRID_SIZE/2, im_y+GRID_SIZE/2, text);
                    }
                }
                else if(board[x][y].is_boom == 1)
                {
                    setfillcolor(BLACK);
                    solidcircle(im_x+GRID_SIZE/2, im_y+GRID_SIZE/2, 20);
                }

            }
            if(end == -1)
            {
                const TCHAR* over = _T("GAME OVER");
                settextcolor(RED);
                outtextxy(GAP+ COLUMN*(LINE_WIDTH+ GRID_SIZE)/2- textwidth(over)/2, GRID_SIZE/2, over);
            }
            else if(end == 1)
            {
                const TCHAR* win = _T("YOU WIN");
                settextcolor(YELLOW);
                outtextxy(GAP+ COLUMN*(LINE_WIDTH+ GRID_SIZE)/2- textwidth(win)/2, GRID_SIZE/2, win);
            }
        }
    }
}
void ChessBoard::sweep(int x, int y)
{
    if(board[x][y].state == 1) //重复点击无效
    {
        return;
    }
    board[x][y].state = 1;
    if(board[x][y].is_boom == 1)   //判断输赢
    {
        end = -1;
        logger.out("SWEEP_END: ("+std::to_string(x)+ ", "+std::to_string(y)+"), date: "+ log_message());
    }
    else if(board[x][y].is_boom == 0)
    {
        find++;
        logger.out("SWEEP: ("+std::to_string(x)+ ", "+std::to_string(y)+"), date: "+ log_message());
        if(find == ROW*COLUMN-BOOM)
        {
            end = 1;
            logger.out("SWEEP_END: ("+std::to_string(x)+ ", "+std::to_string(y)+"), date: "+ log_message());
        }

    }
    if(board[x][y].other_boom == 0)  //周围没有boom，遍历周围
    {
        for(int dx: dxdy)
        {
            if(x+dx>=0&&x+dx<ROW)
            {
                for(int dy: dxdy)
                {
                    if(y+dy>=0&y+dy<COLUMN)
                    {
                        if(board[x+dx][y+dy].state == 1)
                        {
                            continue;
                        }
                        else
                        {
                            sweep(x+dx, y+dy);
                        }
                    }
                }
            }
        }
    }
}
void ChessBoard::mouse_over(int x, int y)
{
    board[x][y].mouse_over = 1;
    if(x != x_tmp || y != y_tmp)
    {
        board[x_tmp][y_tmp].mouse_over = 0;
        x_tmp = x;
        y_tmp = y;
    }
}
void ChessBoard::reset_mouse_over()
{
    board[x_tmp][y_tmp].mouse_over = 0;
}
void ChessBoard::run()
{
    logger.out("game running......");
    while(end == 0)
    {
        if(MouseHit())
        {
            MOUSEMSG m = GetMouseMsg();
            if(m.x>=GAP+LINE_WIDTH && m.x<GAP+COLUMN*(LINE_WIDTH+GRID_SIZE) && m.y>=GAP+LINE_WIDTH && m.y<GAP+ROW*(LINE_WIDTH+GRID_SIZE))
            {
                int x = (m.y-GAP)/(LINE_WIDTH+GRID_SIZE);
                int y = (m.x-GAP)/(LINE_WIDTH+GRID_SIZE);
                if(m.uMsg == WM_LBUTTONDOWN)
                {
                    logger.out("SWEEP: ("+std::to_string(x)+ ", "+std::to_string(y)+ ")");
                    sweep(x, y);
                }
                else if(m.uMsg == WM_RBUTTONDOWN)
                {
                    board[x][y].flag = !board[x][y].flag;
                }
                else if(m.uMsg == WM_MOUSEMOVE)
                {
                    mouse_over(x, y);  //更新mouse_over以及tmp
                }

            }
            else
            {
                reset_mouse_over();  //mouse移出边界时初始化tmp mouse_over
            }
        }
        BeginBatchDraw();
        cleardevice();
        show_state();
        EndBatchDraw();

    }
    if(end == -1 || end == 1)
    {
        Buttons butt;
        butt.setButton(GAP, GAP+(ROW-1)*(LINE_WIDTH+GRID_SIZE), 80, 40, RED, _T("restart"));
        butt.setButton(GAP+(COLUMN-1)*(LINE_WIDTH*GRID_SIZE), GAP+(ROW-1)*(LINE_WIDTH+GRID_SIZE), 80, 40, RGB(128, 128, 128), _T("exit"));
        butt.buttons[0].setFun_back(game_start_window, "click button: [restart] -> game_start_window");
        butt.buttons[1].setFun_back(main_window, "click button: [exit] -> main_window");
        while(end == -1 || end == 1)
        {
            MOUSEMSG m = GetMouseMsg();
            BeginBatchDraw();
            cleardevice();
            show_state();
            butt.draw(m);
            EndBatchDraw();

        }
    }
}
std::string ChessBoard::log_message()
{
    return "{end: "+std::to_string(end)+", find: "+ std::to_string(find)+ "}";
}
