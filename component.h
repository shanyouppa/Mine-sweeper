#include<iostream>
#include<random>
#include<ctime>
#include<list>
#include<string>
#include<graphics.h>
#include<conio.h>
#include <functional>
#include"log.h"

extern Log logger;
extern int MAIN;
extern int GAME_START;

using namespace std;
class Button {
public:
    int x, y, width, height, color;
    LPCTSTR text;
    int gap;
    function<void()> Fun_back;
    std::string log_message;
    Button(int x, int y, int width, int height, int color, LPCTSTR text);
    void setFun_back(function<void()> fun, std::string message = "");
    void LMouse(MOUSEMSG m) const;
    void draw(MOUSEMSG m);
};
class Buttons {
public:
    vector<Button> buttons;
    void setButton(int x, int y, int width, int height, int color, LPCTSTR text);
    void draw(MOUSEMSG m);
};
