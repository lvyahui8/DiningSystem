//#include <iostream>
#define SHOW_CONSOLE
#include <windows.h>
#include <graphics.h>
#include <stdio.h>
#include "FoodMenu.h"
#include "Board.h"
#include "Waiter.h"
#include "Boss.h"
#include "RunApp.h"
unsigned short CFoodMenu::m_unFoodCount =0 ;
//using namespace std;
#include "Draw.h"
int main(){
	setinitmode(0x0);
	initgraph(1020,600);
	setbkcolor(EGERGB(255,255,255));
	CRunApp theApp;
	theApp.Run();
	closegraph();
//*/
	return 0;	
}