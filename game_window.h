#include<conio.h>
#include <graphics.h>
#include "log.h"
extern int ROW;
extern int COLUMN;
#define BOOM 6
#define GRID_SIZE 40
#define GAP 120
#define LINE_WIDTH 1

extern int MAIN;
extern int GAME_START;

void show_init();
void show_destroy();
void exit_program();
void main_window();
void game_start_window();
void main_window_button();
void game_start_window_button();

