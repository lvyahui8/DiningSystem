// RunApp.cpp: implementation of the CRunApp class.
//
//////////////////////////////////////////////////////////////////////

#include "RunApp.h"
#include <graphics.h>
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#include <iostream>
#include <iomanip>
#include <math.h>
#include <fstream>
#include <ctime>
#include <conio.h>
using namespace std;

CRunApp::CRunApp()
{
	m_pFoodMenu = NULL;
	m_pBoard =NULL;
	m_pWaiter =NULL;
	int i=0;

	ifstream ifTemp("major.dat");
	if(!ifTemp){
		exit(0);
		cout<<"丢失文件major.dat"<<endl;
	}
	typedef struct{
			char bossName[255];
			char bossSex[25];
			char bossPassword[255];
			unsigned boardCount;
	}Major;
	Major theMajor;
	ifTemp.read((char*)&theMajor,sizeof(theMajor));
	m_pBoss = CBoss::GetBoss(theMajor.bossPassword,theMajor.bossName,theMajor.bossSex);
	//加载服务员和餐桌对象
	for(i=0;i<theMajor.boardCount && i < INIT_BOARD_COUNT+5;i++){//
		m_pBoss->AddBoard(&m_pBoard);
	}
	ifTemp.close();
	//加载菜单
	ifTemp.clear(ios::goodbit);//必须在读另一个文件之前必须调用 clear 清除该流的状态。
	ifTemp.open("Food.dat",ios_base::in);
	if(!ifTemp){
		exit(0);
		cout<<"丢失文件Food.dat"<<endl;
	}
	typedef struct{ 
		char buffer[255];
		unsigned uType;
		double dPrice;
	}theFood;
	for(i=0;!ifTemp.eof();i++){
		theFood tempFood;
		if(!ifTemp.read((char*)&tempFood,sizeof(tempFood))){
			break;
		}
		m_pBoss->AddFoodMenuItem(&m_pFoodMenu,tempFood.uType,tempFood.buffer,tempFood.dPrice);
	}
	ifTemp.close();
	ifTemp.clear(ios::goodbit);//必须在读另一个文件之前必须调用 clear 清除该流的状态。
	ifTemp.open("waiter.dat",ios_base::in);
	if(!ifTemp){
		exit(0);
		cout<<"丢失文件waiter.dat"<<endl;
	}
	typedef struct{
		unsigned uNumber;
		char sName[255];
		char sSex[25];
		double dAchieve;
		double dSalarys;
	}theWaiter;
	theWaiter tempWaiter;
	while(!ifTemp.eof()){
		if(!ifTemp.read((char*)&tempWaiter,sizeof(tempWaiter))){
			break;
		}
		m_pBoss->AddWaiter(&m_pWaiter,tempWaiter.uNumber,
			tempWaiter.dSalarys,tempWaiter.sName,tempWaiter.sSex,tempWaiter.dAchieve);
	}
	ifTemp.close();

}

CRunApp::~CRunApp()
{
	//保存所有菜单信息
	ofstream ofTemp("Food.dat");
	if(ofTemp){
		typedef struct{ 
			char buffer[255];
			unsigned uType;
			double dPrice;
		}theFood;
		theFood tempFood;
		for(int i=1;i<=CFoodMenu::m_unFoodCount;i++){
			CFoodMenu * pFood=GetPFood(i);
			strcpy(tempFood.buffer,pFood->m_Food.m_strFoodName.c_str());
			tempFood.uType=pFood->m_Food.m_uFoodType;
			tempFood.dPrice=pFood->m_Food.m_dPrice;
			ofTemp.write((char*)&tempFood,sizeof(tempFood));
			//m_pBoss->DelFoodMenuItem(&m_pFoodMenu,pFood->m_Food.m_strFoodName);
		}
	}
	ofTemp.close();
	//保存所有服务员对象
	ofTemp.open("waiter.dat");
	if(ofTemp){
		typedef struct{
			unsigned uNumber;
			char sName[255];
			char sSex[25];
			double dAchieve;
			double dSalarys;
		}theWaiter;
		theWaiter tempWaiter;
		CWaiter *current;
		CWaiter **ppWaiter=&m_pWaiter;
		while((current = *ppWaiter)!=NULL){
			tempWaiter.uNumber = current->GetNumber();
			strcpy(tempWaiter.sName,current->GetName().c_str());
			strcpy(tempWaiter.sSex,current->GetSex().c_str());
			tempWaiter.dAchieve = current->GetAchievement();
			tempWaiter.dSalarys = current->GetSalary();
			ofTemp.write((char*)&tempWaiter,sizeof(tempWaiter));
			ppWaiter = &current->pNextWaiter;
		}
	}
	ofTemp.close();
	ofTemp.open("major.dat");
	if(ofTemp){
		typedef struct{
			char bossName[255];
			char bossSex[25];
			char bossPassword[255];
			unsigned boardCount;
		}Major;
		Major theMajor;
		strcpy(theMajor.bossName ,m_pBoss->GetName().c_str());
		strcpy(theMajor.bossSex,m_pBoss->GetSex().c_str());
		strcpy(theMajor.bossPassword,m_pBoss->GetPassword().c_str());
		theMajor.boardCount=CBoard::GetBoardCount();
		ofTemp.write((char*)&theMajor,sizeof(theMajor));
	}
	ofTemp.close();
// 释放内存空间
	unsigned unFoodCount	=	CFoodMenu::m_unFoodCount;
	unsigned unWaiterCount	=	CWaiter::GetWaiterCount();
	unsigned unBoardCount	=	CBoard::GetBoardCount();
	for(int i=1;i<=unFoodCount || i<= unWaiterCount || i<= unBoardCount;i++){
		if(i<=unFoodCount){
			m_pBoss->DelFoodMenuItem(&m_pFoodMenu,GetPFood(1)->m_Food.m_strFoodName);
		
		}
		if(i<=unWaiterCount){
			if( GetPWaiter(i)!= NULL)
				m_pBoss->DelWaiter(&m_pWaiter,GetPWaiter(i)->GetNumber());
		}
		if(i<=unBoardCount){
			m_pBoss->DelBoard(&m_pBoard);
		}
	}
	delete m_pBoss;
}

// 获得指定编号餐桌的指针
CBoard * CRunApp::GetPBoard(const unsigned short & unNumber){
	CBoard **ppBoard = &m_pBoard;
	register CBoard * current;
	while((current = *ppBoard) != NULL && current->GetNumber() != unNumber){
		ppBoard = &current->pNextBoard;
	}
	return current;
}
// 获得指定编号服务员的指针
CWaiter * CRunApp::GetPWaiter(const unsigned short & unNumber){
	CWaiter **ppWaiter = &m_pWaiter;
	register CWaiter * current;
	while((current = *ppWaiter) != NULL && current->GetNumber() != unNumber){
		ppWaiter = &current->pNextWaiter;
	}
	return current;	
}
// 获得指定编号菜肴的指针
CFoodMenu * CRunApp::GetPFood(const unsigned short & unNumber){
	CFoodMenu **ppFood = &m_pFoodMenu;
	register CFoodMenu * current;
	unsigned short i=0;
	while(i<unNumber && (current = *ppFood) != NULL){
		ppFood = &current->m_NextFood;
		i++;
	}
	return current;	
}
int Count(int temp,int selectNumber){
	int count = 0;
	while(temp){
		int m=temp;
		while(m){
			if(selectNumber==m%10) break;
			m/=10;
		}
		if(m!=0)
			count++;
		temp--;
	}
	return count;
}
//////////////////////////////////////////////////////////////////////////
//	运行
//////////////////////////////////////////////////////////////////////////
void CRunApp::Run(){
	const int BOARD_WIDTH		=	100;
	const int BOARD_HEIGHT		=	50;
	const RECT	BOARD_AREA ={16,16,616,576};		//	餐桌区域
	const RECT	INFO_AREA = {636,16,996,576};		//	信息区域
	const RECT	ADMIBT_AREA = {636,600,800,635};	//	管理按钮
	const RECT	QIUTBT_AREA = {832,600,996,635};	//	退出按钮
	const COORD cirPoint = {1010,570};				//	收缩按钮的圆心位置
	const int INTERVAL_WIDTH	=	9;
	mouse_msg	msg={0};
	theDraw.SetBackground();
	for ( ; 1 ; delay_fps(60))//is_run()
	{
		static bool isIn = false;
		static bool isVsible = false;
		if(isVsible){
			theDraw.DrawRect(ADMIBT_AREA.left+4,ADMIBT_AREA.top+4,ADMIBT_AREA.right+4,ADMIBT_AREA.bottom+4,
						EGERGB(0,128,0),GREEN,4,NULL,true);
			theDraw.PrintText(ADMIBT_AREA.left+22,ADMIBT_AREA.top,"后台管理",26,"宋体");
			theDraw.DrawRect(QIUTBT_AREA.left+4,QIUTBT_AREA.top+4,QIUTBT_AREA.right+4,QIUTBT_AREA.bottom+4,
						EGERGB(0,128,0),GREEN,4,NULL,true);
			theDraw.PrintText(QIUTBT_AREA.left+52,QIUTBT_AREA.top,"退出",26,"宋体");
		}
		theDraw.DrawRect(BOARD_AREA.left+4,BOARD_AREA.top+4,BOARD_AREA.right+4,BOARD_AREA.bottom+4,
			EGERGB(0,128,0),EGERGB(255,230,242),4,"餐桌",true);//BLACK  EGERGB(0,128,0) YELLOW  BROWN
		theDraw.DrawRect(INFO_AREA.left+4,INFO_AREA.top+4,INFO_AREA.right+4,INFO_AREA.bottom+4,
			EGERGB(0,128,0),EGERGB(255,230,242),4,"信息",true);
		circle(cirPoint.X,cirPoint.Y,7);
		//画出所有餐桌{
		CBoard **ppBoard = &m_pBoard;
		register CBoard * current;
		short i=0;
		COORD rPos={BOARD_AREA.left,BOARD_AREA.top};
		char buffer[255];
		while((current = *ppBoard) != NULL){
			if(true == current->m_bFlags){
				theDraw.DrawRect(rPos.X+INTERVAL_WIDTH,rPos.Y+INTERVAL_WIDTH,
					rPos.X+INTERVAL_WIDTH+BOARD_WIDTH,rPos.Y+INTERVAL_WIDTH+BOARD_HEIGHT,
					YELLOW,RED,3,NULL,false);
			}
			else{
				theDraw.DrawRect(rPos.X+INTERVAL_WIDTH,rPos.Y+INTERVAL_WIDTH,
					rPos.X+INTERVAL_WIDTH+BOARD_WIDTH,rPos.Y+INTERVAL_WIDTH+BOARD_HEIGHT,
					YELLOW,GREEN,3,NULL,false);
			}
			sprintf(buffer,"%02d",current->GetNumber());
			theDraw.SetForeColor(WHITE);
			theDraw.PrintText(rPos.X+INTERVAL_WIDTH+10,rPos.Y+INTERVAL_WIDTH+10,buffer,30);
			sprintf(buffer,"人数:%02d",current->GetManCount());
			theDraw.PrintText(rPos.X+INTERVAL_WIDTH+40,rPos.Y+INTERVAL_WIDTH+5,buffer,16);
			theDraw.PrintText(rPos.X+INTERVAL_WIDTH+42,rPos.Y+INTERVAL_WIDTH+28,current->GetStartTime(),13);
			rPos.X+=(INTERVAL_WIDTH*2+BOARD_WIDTH);
			if(rPos.X>=BOARD_AREA.bottom){
				rPos.X=BOARD_AREA.left;
				rPos.Y+=(INTERVAL_WIDTH*2+BOARD_HEIGHT);
			}
			ppBoard = &current->pNextBoard;
		}
		//}//结束画餐桌。进入消息循环
		msg.msg = mouse_msg_move;
		while (mousemsg())
		{
			msg = getmouse();
		}
		
		if(msg.is_down() &&	msg.is_left()){
			if(msg.x>BOARD_AREA.left && msg.x<BOARD_AREA.right && msg.y>BOARD_AREA.top && msg.y<BOARD_AREA.bottom){
				int line	=	(msg.y-BOARD_AREA.top)/(INTERVAL_WIDTH*2+BOARD_HEIGHT);
				int column	=	(msg.x-BOARD_AREA.left)/(INTERVAL_WIDTH*2+BOARD_WIDTH);
				//xyprintf(700,300,"line=%3d,column=%3d",line,column);
				unsigned number = (line*5+column)+1;
				int  temp;
				int count=Count(number,4);
				number += count;
				temp = Count(number,4);
				if(temp!=count){
					number++;
				}
				theDraw.SetForeColor(RED);
				//xyprintf(650,30,"number=%3d",number);
				CBoard *pBoard = GetPBoard(number);
				if(NULL != pBoard)
					Order(pBoard);
			}
			else if(msg.x>QIUTBT_AREA.left && msg.x<QIUTBT_AREA.right && msg.y>QIUTBT_AREA.top && msg.y<QIUTBT_AREA.bottom){
				break;	//退出程序
			}
			else if(sqrt(pow(abs(msg.x-cirPoint.X),2)+pow(abs(msg.y-cirPoint.Y),2)) <= 7){
				//xyprintf(650,60,"在园内");
				closegraph();
				if(true == isVsible){
					//window_setviewport(0,0,1020,600);
					initgraph(1020,600);
					isVsible = false;
				}
				else{
					//window_setviewport(0,0,1020,630);
					initgraph(1020,640);
					isVsible = true;
				}
			}
			isIn = true;
		}

		if (true == isIn) {
			cleardevice();
			theDraw.SetBackground();
			isIn = false;
		}
	}
}

void CRunApp::PrintBoardInfo(CBoard *pBoard,const RECT & INFO_AREA){
	//	打印餐桌信息
	theDraw.DrawRect(INFO_AREA.left+4,INFO_AREA.top+4,INFO_AREA.right+4,INFO_AREA.bottom+4,
		EGERGB(0,128,0),EGERGB(255,230,242),4,"信息",true);
	register CBoardMenu * currentMenu;
	CBoardMenu ** ppBoardMenu = &pBoard->m_pBoardMenu;
	COORD currentPos = {INFO_AREA.left,INFO_AREA.top};
	char buffer[255];
	sprintf(buffer,"%02d",pBoard->GetNumber());
	theDraw.PrintText(currentPos.X+10,currentPos.Y,buffer,60);
	sprintf(buffer,"人数:%02d  光临时间:%s",pBoard->GetManCount(),pBoard->GetStartTime());
	theDraw.PrintText(currentPos.X+80,currentPos.Y,buffer,20);
	sprintf(buffer,"消费:%-5.1lf￥",pBoard->GetConsume());
		theDraw.PrintText(INFO_AREA.left+80,INFO_AREA.top+30,buffer,20);
	currentPos.X=INFO_AREA.left+10,currentPos.Y=INFO_AREA.top + 60;
	while((currentMenu = *ppBoardMenu)!=NULL){
		
		sprintf(buffer,"%-12s %2.0lf %2u  %3.0lf",
			currentMenu->m_Food.m_strFoodName.c_str(),currentMenu->m_Food.m_dPrice,currentMenu->m_uShare,currentMenu->m_dMoney);
		theDraw.PrintText(currentPos.X,currentPos.Y,buffer,14);
		currentPos.Y += 16;
		if(currentPos.Y>=INFO_AREA.bottom-20){
			currentPos.X += 170;
			currentPos.Y = INFO_AREA.top + 60;
		}
		ppBoardMenu = &currentMenu->m_pNextBoardFood;
	}
}
//////////////////////////////////////////////////////////////////////////
//	进入点餐
//////////////////////////////////////////////////////////////////////////
bool CRunApp::Order(CBoard *pBoard){
	cleardevice();
	int waiternumber=0;
	theDraw.SetBackground();
	theDraw.VirtualKeyboard("请输入服务员编号",(int *)&waiternumber);
	CWaiter *pWaiter;
	pWaiter = GetPWaiter(waiternumber);
	
	if (NULL==pWaiter) {
		return false;
	}
	cleardevice();
	if(false==pBoard->m_bFlags){
		int mancout;
		theDraw.SetBackground();
		theDraw.VirtualKeyboard("餐桌未使用,请问几位",(int*)&mancout);
		if(0 == mancout) return false;
		pBoard->m_bFlags = true;
		pBoard->SetManCount(mancout);
		pBoard->SetStartTime();
		cleardevice();
		
	}
	theDraw.SetBackground();
	const RECT OPTION_AREA = {16,496,616,576};
	COORD currentPos={OPTION_AREA.left,OPTION_AREA.top};
	theDraw.DrawRect(OPTION_AREA.left+4,OPTION_AREA.top+4,OPTION_AREA.right+4,OPTION_AREA.bottom+4,
		EGERGB(0,128,0),EGERGB(255,230,242),4,"选项",true);
	theDraw.SetForeColor(WHITE);
	for(int k=0;k<3;k++){
		const char * str[] ={"返回","撤销","结账"};
		theDraw.DrawRect(currentPos.X,currentPos.Y,currentPos.X+197,currentPos.Y+70,
			YELLOW,GREEN,3,NULL,false);
		theDraw.PrintText(currentPos.X+60,currentPos.Y+20,str[k],30,"楷体");
		currentPos.X+=197;
	}
	mouse_msg msg = {0};
	

	for ( ; is_run(); delay_fps(60)){
		register CFoodMenu * current;
		CFoodMenu ** ppBoardMenu= &m_pFoodMenu;
		const int FOOD_WIDTH	=	98;
		const int FOOD_HEIGHT	=	50;
		const RECT	FOOD_AREA ={16,16,616,366};
		
		char buffer[255];

		currentPos.X = FOOD_AREA.left,currentPos.Y = FOOD_AREA.top;
		theDraw.DrawRect(FOOD_AREA.left+4,FOOD_AREA.top+4,FOOD_AREA.right+4,FOOD_AREA.bottom+4,
			EGERGB(0,128,0),EGERGB(255,230,242),4,"菜肴",true);
		while((current = *ppBoardMenu)!=NULL){
			theDraw.DrawRect(currentPos.X,currentPos.Y,currentPos.X+FOOD_WIDTH,currentPos.Y+FOOD_HEIGHT,
				YELLOW,GREEN,3,NULL,false);
			theDraw.SetForeColor(WHITE);
			theDraw.PrintText(currentPos.X+4,currentPos.Y+16,current->m_Food.m_strFoodName.c_str(),18);
			//outtextrect(currentPos.X,currentPos.Y,currentPos.X+FOOD_WIDTH,currentPos.Y+FOOD_HEIGHT,current->m_Food.m_strFoodName.c_str());
			currentPos.X+=(FOOD_WIDTH);
			if(currentPos.X>=(FOOD_AREA.right-12)){
				currentPos.X=FOOD_AREA.left;
				currentPos.Y+=(FOOD_HEIGHT);
			}
			ppBoardMenu = &current->m_NextFood;
		}
		const RECT	INFO_AREA = {636,16,996,576};
		theDraw.SetForeColor(BLACK);
		PrintBoardInfo(pBoard,INFO_AREA);	
		//do{
		msg = getmouse();
		//}while(!(msg.is_down() && msg.is_left()));
		if(msg.is_down() &&	msg.is_left()){
			// 如果点中了菜肴
			if(msg.x>FOOD_AREA.left && msg.x<FOOD_AREA.right && msg.y>FOOD_AREA.top && msg.y<FOOD_AREA.bottom){
				int line	=	(msg.y-FOOD_AREA.top)/(FOOD_HEIGHT);
				int column	=	(msg.x-FOOD_AREA.left)/(FOOD_WIDTH);
				//theDraw.SetForeColor(RED);
				unsigned number = (line*6+column)+1;
				CFoodMenu *pFood = GetPFood(number);
				if(NULL != pFood)
				{
					// 选择数量
					const RECT COUNT_AREA =  {16,386,616,476};
					theDraw.DrawRect(COUNT_AREA.left+4,COUNT_AREA.top+4,COUNT_AREA.right+4,COUNT_AREA.bottom+4,
						EGERGB(0,128,0),EGERGB(255,230,242),4,"数量",true);
					currentPos.X = COUNT_AREA.left,currentPos.Y = COUNT_AREA.top;
					theDraw.SetForeColor(WHITE);
					for(int i=0;i<10;i++){
						theDraw.DrawRect(currentPos.X,currentPos.Y,currentPos.X+59,currentPos.Y+80,
							YELLOW,GREEN,3,NULL,false);
						sprintf(buffer,"%02d",i+1);

						theDraw.PrintText(currentPos.X+6,currentPos.Y+20,buffer,40);
						currentPos.X+=59;
					}
					do{
						msg = getmouse();
					}while(
						!(msg.x>COUNT_AREA.left && 
						msg.x<COUNT_AREA.right && 
						msg.y>COUNT_AREA.top && 
						msg.y<COUNT_AREA.bottom && 
						msg.is_down() && 
						msg.is_left())
						);
					unsigned share = (msg.x-COUNT_AREA.left)/60 + 1;
					//点餐
					pWaiter->AddBoardMenuItem(pBoard,pFood,share);
				}
			}
			else if(msg.x>OPTION_AREA.left && msg.x<OPTION_AREA.right && msg.y>OPTION_AREA.top && msg.y<OPTION_AREA.bottom){
				int index = (msg.x/((OPTION_AREA.right-OPTION_AREA.left)/3) + 1);
				if(1 == index){
					return true;
				}
				else if(2 == index){
					pWaiter->DelBoardMenuItem(pBoard);
				}
				else if(3 == index){
					if (true == pBoard->m_bFlags) {
						pBoard->Clean();
						return true;
					}
				}
			}
		}
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////
//	Boss管理
//////////////////////////////////////////////////////////////////////////

bool CRunApp::Administrate(){
	int iInput;
	COORD  currentPos;
	string strTemp1;
	string strTemp2;
	//输入密码
	currentPos.X=WINDOW_MAX_WIDTH/2-3;
	currentPos.Y=WINDOW_MAX_HIGHT/2;
	do{
		char str[100];
		char * pStr = str;
		while((*pStr=getch()) != 13){		//当没有按回车
			if(8 == *pStr){					//如果按了退格
				if(pStr>str){
					cout<<'\b'<<' '<<'\b';
					pStr--;
				}
				
			}
			else{
				cout<<"*";
				pStr++;
			}
		}
		*pStr='\0';						//给字符串加上结尾
		strTemp2.assign(str);			//将字符串加到string对象尾部
	}while(!(m_pBoss->GetPassword()==strTemp2 || "0"==strTemp2));	// ()里是条件满足 !()就是当条件不满足时,执行循环
	if("0"==strTemp2) return false;

	//进入了管理界面
	do{
		switch(iInput){
		case 1:		//新增餐桌
			break;
		case 2:		//减少餐桌
			break;
		case 3:		//新增服务员
			break;
		case 4:		//解雇服务员
			break;
		case 5:		//设置服务员工资
			break;
		case 6:		//增加菜肴
			break;
		case 7:		//减少菜肴
			break;
		case 8:		//查看营业额
			break;
		case 9:		//修改密码
			system("echo 按任意键返回  & pause > nul");
			break;
		case 0:
			return false;
		default:
			;
		}
	}while(1);
	return true;
}