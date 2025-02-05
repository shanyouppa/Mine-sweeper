#include "component.h"

#include <utility>

Button::Button(int x, int y, int width, int height, int color, LPCTSTR text)
        : x(x), y(y), width(width), height(height), color(color), text(text), gap(0) {};
void Button::setFun_back(function<void()> fun, std::string message)
{
    this->log_message = std::move(message);
    Fun_back = std::move(fun);
}
void Button::LMouse(MOUSEMSG m) const
{
    if(m.x>x && m.x<x+width && m.y>y && m.y<y+height && m.uMsg == WM_LBUTTONDOWN)
    {
        logger.out(log_message);
        Fun_back();
    }
}
void Button::draw(MOUSEMSG m) {
    gap = (m.x >= x && m.x <= x + width && m.y >= y && m.y <= y + height) ? 5 : 0;
    setfillcolor(color);
    solidrectangle(x+ gap, y+ gap, x+ width- gap, y+ height- gap);
    setbkmode(TRANSPARENT);
    settextcolor(BLACK);
    settextstyle(16, 0, _T(""), 0, 0, 1600, false, false, false);
    outtextxy(x+ width/2- textwidth(text)/2, y+ height/2- textheight(text)/2, text); //居中显示文本
}
void Buttons::setButton(int x, int y, int width, int height, int color, LPCTSTR text) {
    buttons.emplace_back(x, y, width, height, color, text);
}
void Buttons::draw(MOUSEMSG m)
{
    for(Button& b : buttons)
    {
        b.draw(m);
    }
}
void Buttons::trigger(MOUSEMSG m)
{
    for(Button& b : buttons)
    {
        b.LMouse(m);
    }
}




int TextBoxs::now_index = -1;
TextBox::TextBox(int left, int top, int right, int bottom, int gap, LPCTSTR title) : left(left), top(top), right(right), bottom(bottom), gap(gap), title(title) {}
int TextBox::text_is_empty()
{
    return text.empty();
}
void TextBox::draw()
{
    setcolor(BLACK);
    rectangle(left, top, right, bottom);
    settextcolor(BLACK);
    settextstyle(bottom-top-10, 14, _T("宋体"));
    outtextxy(left + 20, (2*top+10)/2, text.c_str());  //输入框数据,间隔20       //(2*top+10)/2 = (bottom+top)/2-(bottom-top-10)/2 = 输入框中心-半个字体高度
    outtextxy(left -10* textwidth('A'), (2*top+10)/2, title);     //输入框左标题
    if(sw)
    {
        setcolor(BLACK);
        int cursor_x = left+20+cursor_pos* textwidth('A');
        line(cursor_x, (2*top+10)/2, cursor_x, (2*bottom-10)/2);   //画输入位置黑线
    }
}
int TextBox::select(MOUSEMSG m)
{

    if(m.x>=left && m.x<=right && m.y>=top && m.y<=bottom && m.uMsg == WM_LBUTTONDOWN)
    {
        cursor_pos = (m.x-left-20+7)/14<=text.size()? (m.x-left-20+7)/14: static_cast<int>(text.size()); //具体光标位置
        return 1;    //黑线位置会对不上
    }
    else
    {
        return 0;
    }
}
void TextBox::init_text(int num)
{
    text = std::to_string(num);
}
void TextBoxs::set_TextBox(int left, int top, int right, int bottom, int gap, LPCTSTR title)
{
    textboxs.emplace_back(left, top, right, bottom, gap, title);
}
void TextBoxs::draw()
{
    for(TextBox& t: textboxs)
    {
        t.draw();
    }
}
void TextBoxs::trigger(MOUSEMSG m)
{
    for(int i=0;i<textboxs.size();i++) //确定输入框窗口
    {
        if(textboxs[i].select(m) && !textboxs[i].sw)
        {
            if(now_index != -1)
            {
                textboxs[now_index].sw = false;
            }
            now_index = i;
            textboxs[i].sw = true;
            return;
        }
    }
}
void TextBoxs::input(TextBox& t)
{
    if(!t.sw)
    {
        FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE)); //清除缓存区数据
        return;
    }
    unsigned char ch = _getch();
    if (ch == 8)  // 退格键
    {
        if (!t.text.empty() && t.cursor_pos > 0)
        {
            t.text.erase(t.cursor_pos-1, 1);
            t.cursor_pos--;
        }
    }
    else if (ch == 13)  // 回车键
    {
        t.sw = false;
    }
    else if (ch == 224)  // 特殊键
    {
        ch = _getch();
        if (ch == 75)  // 左箭头
        {
            if (t.cursor_pos > 0)
            {
                t.cursor_pos--;
            }
        }
        else if (ch == 77)  // 右箭头
        {
            if (t.cursor_pos < t.text.size())
            {
                t.cursor_pos++;
            }
        }
        else if (ch == 72)  //上箭头
        {
            t.sw = false;
            if(now_index == 0)
            {
                now_index = static_cast<int>(textboxs.size())-1;
                textboxs[now_index].sw = true;
            }
            else
            {
                now_index--;
                textboxs[now_index].sw = true;
            }
        }
        else if(ch == 80) //下箭头
        {
            t.sw = false;
            if(now_index == static_cast<int>(textboxs.size())-1)
            {
                now_index = 0;
                textboxs[now_index].sw = true;
            }
            else
            {
                now_index++;
                textboxs[now_index].sw = true;
            }
        }
    }
    else if (ch >= 32 && ch <= 126)  // 可打印字符
    {
        if (t.text.size() < 9)
        {
            t.text.insert(t.cursor_pos, 1, static_cast<char>(ch));
            t.cursor_pos++;
        }
    }
}
void TextBoxs::inputs()
{
    if(now_index != -1)
    {
        input(textboxs[now_index]);
    }
    else
    {
        FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
    }
}
void TextBoxs::save_date()
{
    std::ifstream file(jsonfilename);
    json date;
    file>>date;
    for(TextBox& tx: textboxs)
    {
        if(date.contains(tx.title))
        {
            date[tx.title] = std::stoi(tx.text);
        }
    }
    file.close();
    std::ofstream outFile(jsonfilename);
    if (outFile.is_open())
    {
        outFile << date.dump(4);
        outFile.close();
    }
}
TextBoxs::~TextBoxs()
{
    now_index = -1;//重置静态成员数据
}