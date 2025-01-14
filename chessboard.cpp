#include "chessboard.h"
#include "component.h"
#include "game_window.h"
int ChessBoard::x_tmp = 0;
int ChessBoard::y_tmp = 0;
ChessBoard::ChessBoard()
{
    find = 0;
    start_time = 0;
    end_time = 0;
    std::mt19937 gen(std::random_device{}());
    std::uniform_int_distribution<> dis_x(0, ROW-1);
    std::uniform_int_distribution<> dis_y(0, COLUMN-1);
    board.resize(ROW, std::vector<Cell>(COLUMN));  //初始化大小
    for(int i=0;i<BOOM; )
    {
        int x = dis_x(gen);
        int y = dis_y(gen);
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
    logger.out("chessboard construct success......");
}
ChessBoard::~ChessBoard()
{
    find = 0;   //游戏结束清除所有数据
    end = 0;
    flag_count = 0;
    CountTime = false;
    start_time = 0;
    end_time = 0;
    logger.out("chessboard distruct success......");
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
                        char text = static_cast<char>('0' + board[x][y].other_boom);
                        settextcolor(BLUE);
                        settextstyle(16, 0, "", 0, 0, 1600, false, false, false);
                        outtextxy(im_x+GRID_SIZE/2-textwidth(text)/2, im_y+GRID_SIZE/2- textheight(text)/2, text);
                    }
                }
                else if(board[x][y].is_boom == 1)
                {
                    setfillcolor(BLACK);
                    solidcircle(im_x+GRID_SIZE/2, im_y+GRID_SIZE/2, 20);
                }
            }
        }
    }
    if(end == -1)
    {
        LPCTSTR over = _T("GAME OVER");
        settextcolor(RED);
        settextstyle(30, 0, _T(""), 0, 0, 1600, false, false, false);
        outtextxy(GAP+ COLUMN*(LINE_WIDTH+ GRID_SIZE)/2- textwidth(over)/2, GAP/4, over);
    }
    else if(end == 1)
    {
        LPCTSTR win = _T("YOU WIN");
        settextcolor(GREEN);
        settextstyle(30, 0, _T(""), 0, 0, 1600, false, false, false);
        outtextxy(GAP+ COLUMN*(LINE_WIDTH+ GRID_SIZE)/2- textwidth(win)/2, GAP/4, win);
    }
    int int_time = static_cast<int>(end_time - start_time)/CLOCKS_PER_SEC;   //绘制游戏时间, 插旗数量, 炸弹数量
    std::string str_time = "time: " + std::to_string(int_time) + " s";
    std::string str_flag_count = "flag: " + std::to_string(flag_count);
    std::string str_boom = "boom: " + std::to_string(BOOM);
    settextcolor(BLACK);
    settextstyle(30, 0, _T(""), 0, 0, 1600, false, false, false);
    outtextxy(GAP/4+GRID_SIZE+LINE_WIDTH, GAP/2, str_time.c_str());
    outtextxy(GAP/4+(COLUMN-2)*(GRID_SIZE+LINE_WIDTH), GAP/2, str_flag_count.c_str());
    outtextxy(GAP/4+(COLUMN+2)*(GRID_SIZE+LINE_WIDTH), GAP/2, str_boom.c_str());
}
void ChessBoard::sweep(int x, int y)
{
    if(board[x][y].state == 1) //点击空地无效
    {
        return;
    }
    if(board[x][y].flag)   //清除错误flag
    {
        board[x][y].flag = false;
        flag_count--;
    }
    board[x][y].state = 1;
    if(board[x][y].is_boom == 1)   //输了
    {
        end = -1;
        CountTime = false;
        logger.out("SWEEP_END: (" + std::to_string(x) + ", " + std::to_string(y) + "), date: " + EndFind_message());
    }
    else if(board[x][y].is_boom == 0)
    {
        find++;
        logger.out("SWEEP: (" + std::to_string(x) + ", " + std::to_string(y) + "), date: " + EndFind_message());
    }
    if(find == ROW*COLUMN-BOOM)  //赢了
    {
        end = 1;
        CountTime = false;
        logger.out("SWEEP_END: (" + std::to_string(x) + ", " + std::to_string(y) + "), date: " + EndFind_message());
        return;
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
    while(end == 0 && GAME_START == 1)
    {
        if(MouseHit())  //防止GetMouseMsg阻塞循环
        {
            MOUSEMSG m = GetMouseMsg();
            if(m.x>=GAP+LINE_WIDTH && m.x<GAP+COLUMN*(LINE_WIDTH+GRID_SIZE) && m.y>=GAP+LINE_WIDTH && m.y<GAP+ROW*(LINE_WIDTH+GRID_SIZE))
            {
                int x = (m.y-GAP)/(LINE_WIDTH+GRID_SIZE);
                int y = (m.x-GAP)/(LINE_WIDTH+GRID_SIZE);
                if(m.uMsg == WM_LBUTTONDOWN)
                {
                    logger.out("SWEEP: ("+std::to_string(x)+ ", "+std::to_string(y)+ ")");
                    if(CountTime == false)  //第一次sweep， 计时器开启时间
                    {
                        start_time = clock();
                        end_time = start_time;
                        CountTime = true;
                    }
                    sweep(x, y);
                }
                else if(m.uMsg == WM_RBUTTONDOWN && !board[x][y].state)  //已探索区域插旗无效
                {
                    if(board[x][y].flag)
                    {
                        flag_count--;
                    }
                    else
                    {
                        flag_count++;
                    }
                    logger.out("flag(" + std::to_string(x) + ", " + std::to_string(y) + ") :" + std::to_string(board[x][y].flag) + "->" + std::to_string(!board[x][y].flag) + ", date: "+ EndFind_message());
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
        if(CountTime == TRUE)   //更新结束时间
        {
            end_time = clock();
        }
        BeginBatchDraw();
        cleardevice();
        show_state();
        EndBatchDraw();

    }
    if((end == -1 || end == 1) && GAME_START == 1)
    {
        Buttons butt;
        butt.setButton(GAP, GAP+ROW*(LINE_WIDTH+GRID_SIZE)+GRID_SIZE/2, 80, 40, RED, _T("restart")); //设置GRID_SIZE为按钮与网格线间隙
        butt.setButton(GAP+(COLUMN-1)*(LINE_WIDTH*GRID_SIZE), GAP+ROW*(LINE_WIDTH+GRID_SIZE)+GRID_SIZE/2, 80, 40, RGB(70, 120, 120), _T("exit"));
        butt.buttons[0].setFun_back([this](){this->reset_end();}, "click button: [restart] -> game_start_window");
        butt.buttons[1].setFun_back(main_window_button, "click button: [exit] -> main_window");
        while((end == -1 || end == 1) && GAME_START == 1)
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
void ChessBoard::reset_end()
{
    end = 0;
}
std::string ChessBoard::EndFind_message() const
{
    return "{end: "+std::to_string(end)+", find: "+ std::to_string(find)+ ", flag_count: "+ std::to_string(flag_count)+ "}";
}
std::string ChessBoard::Boom_message() const
{
    std::string text = "create chessboard: (" + std::to_string(ROW) + ", " + std::to_string(COLUMN) + "), BOOM:" + std::to_string(BOOM);
    text += "\nstate: Boom:\n";
    for(const auto & i : board)
    {
        for(const auto & j : i)
        {
            text += (std::to_string(j.is_boom) + "  ");
        }
        text += "\n";
    }
    text += "state: other_boom:\n";
    for(const auto & i : board)
    {
        for(const auto & j : i)
        {
            text += (std::to_string(j.other_boom) + "  ");
        }
        text += "\n";
    }
    return text;
}
