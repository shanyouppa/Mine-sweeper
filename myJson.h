
#ifndef C_PLUS_MYJSON_H
#define C_PLUS_MYJSON_H

#include <nlohmann/json.hpp>
#include <fstream>
#include<filesystem>

extern int ROW;
extern int COLUMN;
extern int BOOM;
using json = nlohmann::json;
extern std::string jsonfilename;

void createMyJson(const std::string& filename);
void readMyJson();


#endif