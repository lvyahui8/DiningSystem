#include <iostream>
#include "FoodMenu.h"
#include "Board.h"
#include "Waiter.h"
#include "Boss.h"
#include "RunApp.h"
unsigned short CFoodMenu::m_unFoodCount =0 ;
using namespace std;

int main(){
	CRunApp theApp;
	theApp.Run();
	system("pause");
	return 0;	
}