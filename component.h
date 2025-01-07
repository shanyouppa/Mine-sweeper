#include<iostream>
#include<random>
#include<ctime>
#include<list>
#include<graphics.h>
#include<conio.h>
#include <functional>

using namespace std;
class Button {
public:
    int x, y, width, height;
    TCHAR* text;
    int gap;
    function<void()> funback;
    Button(int x, int y, int width, int height, TCHAR* text);
    void LMouse(MOUSEMSG m);
    void setfunback(function<void()> fun);
    void draw(MOUSEMSG m);
};
class Buttons {
public:
    vector<Button> buttons;
    void setButton(int x, int y, int width, int height, TCHAR* text);
    void draw(MOUSEMSG m);
};
