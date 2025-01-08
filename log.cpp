#include "log.h"


Log::Log()
{
    file.open("log.txt", std::ios::app);
    if(!file.is_open())
    {
        std::cerr<<"cannot open log.txt"<<std::endl;
    }
    else
    {
        file<<"[ ]";
        time_out();
        file<<"log open"<<std::endl;
    }
}

Log::~Log()
{
    if (file.is_open()) {
        file<<"[ ]";
        time_out();
        file<<"log close"<<std::endl<<std::endl;
        file.close();
    }
    else
    {
        std::cerr<<"log close failed because of log has been closed"<<std::endl;
    }
}

void Log::out(const std::string& text)
{
    if (file.is_open()) {
        time_out();
        file<<text<<std::endl;
    } else {
        std::cerr<<"failed write log because of file clsed"<<std::endl;
    }
}
void Log::time_out()
{
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);
    std::tm* tm_info = std::localtime(&in_time_t);
    // 输出当前时间
    file<<std::put_time(tm_info, "%Y-%m-%d %X")<<" -- ";

}