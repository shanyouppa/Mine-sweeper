#include<random>
#include<ctime>
#include<graphics.h>
#include<iostream>
#include<list>
#include <functional>
#include<string>
#include<vector>

extern int ROW;
extern int COLUMN;
extern int BOOM;

extern int GRID_SIZE;
extern int GAP;
extern int LINE_WIDTH;

extern int MAIN;
extern int GAME_START;

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
    std::vector<std::vector<Cell>> board;  //is_boom site offset
    int end = 0; //-1->输 0->进行中 1->赢
    int flag_count = 0;
    static int x_tmp;
    static int y_tmp;
    bool CountTime = false; //F->计时器关闭 T->开启
    clock_t start_time = 0;
    clock_t end_time = 0;
public:
    ChessBoard();
    ~ChessBoard();
    static void show_line();
    void show_state();
    void sweep(int x, int y);
    void mouse_over(int x, int y);
    void reset_mouse_over();
    void run();
    void reset_end(); //reset end = 0 for reset game
    std::string EndFind_message() const;
    std::string Boom_message() const;

};
