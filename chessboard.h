#include<random>
#include<ctime>
#include<graphics.h>
#include<iostream>
#include<list>

#define ROW 8
#define COLUMN 10
#define BOOM 6
#define GRID_SIZE 40
#define GAP 120
#define LINE_WIDTH 1
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
    const std::list<int> dxdy = {-1, 0, 1};
    int find = 0; //已探索数目
    Cell board[ROW][COLUMN] = {0};  //is_boom site offset
    int end = 0; //-1->输 0->进行中 1->赢
    static int x_tmp;
    static int y_tmp;
public:
    ChessBoard();
    void show_line();
    void show_state();
    int sweep(int x, int y);
    void mouse_over(int x, int y);
    void reset_mouse_over();
    void run();
};
