#ifndef LOG_H
#define LOG_H
#include <fstream>
#include <iostream>
#include <string>
#include <chrono>
#include <iomanip>
#include <synchapi.h>

class Log
{
private:
    std::ofstream file;

public:
    Log();
    ~Log();
    void out(const std::string& text);
    void time_out();
};
#endif