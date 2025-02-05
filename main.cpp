#include "game_window.h"
#include <iostream>


#include "myJson.h"

using json = nlohmann::json;
std::string jsonfilename = "./game_date/config.json";
std::string logfilename = "./game_date/log.txt";

int MAIN = 1;
int GAME_START = 0;
int SET = 0;
int GRID_SIZE = 30;
int GAP = 120;
int LINE_WIDTH = 1;
int ROW;
int COLUMN;
int BOOM;
Log logger;
// 创建默认 JSON 文件
int main() {
    readMyJson();
    logger.out("read jsonfile");
    show_init();
    while (true) {
        if (MAIN) {
            main_window();
        } else if (GAME_START) {
            game_start_window();
        } else if (SET) {
            setting_window();
        }
        if (_kbhit() && _getch() == 27) {
            break;
        }
    }
    closegraph();
    logger.out("closegraph, termination of program");
    return 0;
}
