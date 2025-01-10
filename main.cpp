#include "game_window.h"
int MAIN = 1;
int GAME_START = 0;
Log logger;
int main()
{
    while(true)
    {
        if(MAIN == 1)
        {
            main_window();
        }
        else if(GAME_START == 1)
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
