#include "myJson.h"

void createMyJson(const std::string& filename)
{
    std::ofstream file(filename);
    if(file)
    {
        json config;
        config["ROW"] = 8;
        config["COLUMN"] = 10;
        config["BOOM"] = 6;
        file<<config.dump(4);
    }
    file.close();
}
void readMyJson()
{
    if(!std::filesystem::exists("game_date"))
    {
        std::filesystem::create_directory("game_date");
    }
    std::ifstream file(jsonfilename);
    if (!file.is_open()) {
        createMyJson(jsonfilename);
        file.open(jsonfilename); // 重新打开创建好的文件
    }

    // 解析 JSON 数据
    json config;
    file >> config;
    // 从 JSON 中获取值并赋值给变量
    ROW = config["ROW"].get<int>();
    COLUMN = config["COLUMN"].get<int>();
    BOOM = config["BOOM"].get<int>();
    file.close();
}
