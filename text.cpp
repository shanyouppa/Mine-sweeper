#include<iostream>
#include<random>
#include<ctime>
#include<list>
#include<graphics.h>
#include<conio.h>
#include <functional>

#define ROW 8
#define COLUMN 10
#define BOOM 6
#define GRID_SIZE 40
#define GAP 120
#define LINE_WIDTH 1

using namespace std;
void game_start_window();
void window_main();
void exit_program()
{
    exit(0);
}
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
class Cell
{
public:
    int is_boom = 0;  //1->有, 0->无
    int state = 0; //1->探索,0->未探索
    int other_boom = 0; //周围boom
    int mouse_over = 0; //鼠标悬停 0->未 1->有
    bool flag = false; //0->无 1->有
};

class ChessBoard
{
private:
    int find = 0; //已探索数目
    Cell board[ROW][COLUMN] = {0};
    const list<int> dxdy = {-1, 0, 1};  //is_boom site offset
    int end = 0; //-1->输 0->进行中 1->赢
    static int x_tmp;
    static int y_tmp;
public:
    ChessBoard()
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
    void show_line()
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
    void show_state() //show chessboard
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
    int sweep(int x, int y)
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
    void mouse_over(int x, int y)
    {
        board[x][y].mouse_over = 1;
        if(x != x_tmp || y != y_tmp)
        {
            board[x_tmp][y_tmp].mouse_over = 0;
            x_tmp = x;
            y_tmp = y;
        }
    }
    void reset_mouse_over()
    {
        board[x_tmp][y_tmp].mouse_over = 0;
    }
    void run()
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
};
int ChessBoard::x_tmp = 0;
int ChessBoard::y_tmp = 0;

class Button {
public:
    int x, y, width, height;
    TCHAR* text;
    int gap;
    function<void()> funback;

    Button(int x, int y, int width, int height, TCHAR* text)
            : x(x), y(y), width(width), height(height), text(text), gap(0) {};
    void LMouse(MOUSEMSG m)
    {
        if(m.x>x && m.x<x+width && m.y>y && m.y<y+height && m.uMsg == WM_LBUTTONDOWN)
        {
            funback();
        }
    }
    void setfunback(function<void()> fun)
    {
        funback = fun;
    }

    void draw(MOUSEMSG m) {
        gap = (m.x >= x && m.x <= x + width && m.y >= y && m.y <= y + height) ? 5 : 0;
        setfillcolor(RGB(128, 128, 128));
        solidrectangle(x + gap, y + gap, x + width - gap, y + height - gap);
        settextcolor(BLACK);
        outtextxy(x + gap, y + (height - 16) / 2, text); // 假设字体高度为16，居中显示文本
    }
};
class Buttons {
public:
    vector<Button> buttons;

    void setButton(int x, int y, int width, int height, TCHAR* text) {
        buttons.push_back(Button(x, y, width, height, text));
    }

    void draw(MOUSEMSG m) {
        for (Button& b : buttons) {
            b.draw(m);
            b.LMouse(m);
        }
    }
};

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

int main()
{
//    game_start_window();
    window_main();



return 0;
}
