#include<iostream>
#include<random>
#include<ctime>
#include<list>
#include<string>
#include<graphics.h>
#include<conio.h>
#include <functional>
#include <fstream>
#include <nlohmann/json.hpp>
#include <filesystem>
#include"log.h"

using json = nlohmann::json;

extern Log logger;
extern int MAIN;
extern int GAME_START;
extern std::string jsonfilename;

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
    void trigger(MOUSEMSG m);
};



#include <graphics.h>
#include <conio.h>
#include <iostream>
#include<vector>
class TextBox
{
private:
    std::string text;
    int left;
    int top;
    int right;
    int bottom;
    int gap;
    LPCTSTR title;  //左标题
    int cursor_pos = 0;
    bool sw = false;
public:
    TextBox(int left, int top, int right, int bottom, int gap, LPCTSTR title);
    int text_is_empty();
    void draw();
    int select(MOUSEMSG m);
    friend class TextBoxs;
};
class TextBoxs
{
public:
    static int now_index; //-1->无输入框激活
    std::vector<TextBox> textboxs;
    ~TextBoxs();
    void set_TextBox(int left, int top, int right, int bottom, int gap, LPCTSTR title);
    void draw();
    void trigger(MOUSEMSG m);
    void input(TextBox& t);
    void inputs();
    void save_date();
    friend class TextBox;
};
