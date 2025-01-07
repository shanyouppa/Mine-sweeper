#include "chessboard.h"
using namespace std;
int ChessBoard::x_tmp = 0;
int ChessBoard::y_tmp = 0;
ChessBoard::ChessBoard()
{
    find = 0;
    int site;
    srand(time(NULL));
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
    cout<<"chessboard construct success!"<<endl;
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
    BeginBatchDraw();  //双缓冲
    cleardevice();
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
                TCHAR* over = _T("GAME OVER");
                settextcolor(RED);
                outtextxy(GAP+ COLUMN*(LINE_WIDTH+ GRID_SIZE)/2- textwidth(over)/2, GRID_SIZE/2, over);
            }
            else if(end == 1)
            {
                TCHAR* win = _T("YOU WIN");
                settextcolor(YELLOW);
                outtextxy(GAP+ COLUMN*(LINE_WIDTH+ GRID_SIZE)/2- textwidth(win)/2, GRID_SIZE/2, win);
            }
        }
    }
    Sleep(10);
    EndBatchDraw();
}
int ChessBoard::sweep(int x, int y)
{
    int im_x = GAP+LINE_WIDTH+y*(GRID_SIZE+LINE_WIDTH);
    int im_y = GAP+LINE_WIDTH+x*(GRID_SIZE+LINE_WIDTH);
    board[x][y].state = 1;
    if(board[x][y].is_boom == 1)   //判断输赢
    {
        end = -1;
    }
    else if(board[x][y].is_boom == 0)
    {
        find++;
        if(find == ROW*COLUMN-BOOM)
        {
            end = 1;
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
    return 0;
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
        show_state();

    }
}
