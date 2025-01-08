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

using namespace std;
class Button {
public:
    int x, y, width, height, color;
    const TCHAR *text;
    int gap;
    function<void()> Fun_back;
    std::string log_message;
    Button(int x, int y, int width, int height, int color, const TCHAR* text);
    void setFun_back(function<void()> fun, std::string log_message = "");
    void LMouse(MOUSEMSG m) const;
    void draw(MOUSEMSG m);
};
class Buttons {
public:
    vector<Button> buttons;
    void setButton(int x, int y, int width, int height, int color, const TCHAR* text);
    void draw(MOUSEMSG m);
};
