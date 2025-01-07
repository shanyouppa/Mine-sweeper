#include "component.h"


Button::Button(int x, int y, int width, int height, TCHAR* text)
        : x(x), y(y), width(width), height(height), text(text), gap(0) {};
void Button::LMouse(MOUSEMSG m)
{
    if(m.x>x && m.x<x+width && m.y>y && m.y<y+height && m.uMsg == WM_LBUTTONDOWN)
    {
        funback();
    }
}
void Button::setfunback(function<void()> fun)
{
    funback = fun;
}

void Button::draw(MOUSEMSG m) {
    gap = (m.x >= x && m.x <= x + width && m.y >= y && m.y <= y + height) ? 5 : 0;
    setfillcolor(RGB(128, 128, 128));
    solidrectangle(x + gap, y + gap, x + width - gap, y + height - gap);
    settextcolor(BLACK);
    outtextxy(x + gap, y + (height - 16) / 2, text); // 假设字体高度为16，居中显示文本
}
void Buttons::setButton(int x, int y, int width, int height, TCHAR* text) {
    buttons.push_back(Button(x, y, width, height, text));
}

void Buttons::draw(MOUSEMSG m) {
    for (Button& b : buttons) {
        b.draw(m);
        b.LMouse(m);
    }
}