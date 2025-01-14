#include "game_window.h"
int MAIN = 1;
int GAME_START = 0;
int ROW = 8;
int COLUMN = 10;
Log logger;
int main()
{
    while(true)
    {
        if(MAIN)
        {
            main_window();
        }
        else if(GAME_START)
        {
            game_start_window();
        }
        if (_kbhit() && _getch() == 27)
        {
            break;
        }
    }
return 0;
}
