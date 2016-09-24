// RunApp.cpp: implementation of the CRunApp class.
//
//////////////////////////////////////////////////////////////////////

#include "RunApp.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#include "GlobalFun.h"
#include "resource.h"
#include <iostream>
#include <iomanip>
#include <math.h>
#include <fstream>
#include <ctime>
#include <conio.h>
using namespace std;

CRunApp::CRunApp()
{
 	system("title #点餐系统#");
	SetWindowSize(WINDOW_MAX_WIDTH,WINDOW_MAX_HIGHT);
	//因为暂时没有找到直接获得控制台窗口句柄的函数。所以使用间接方法获得句柄
	char  buffer[40];
	::GetConsoleTitle(buffer,40);			//获的窗口标题
	HWND hWnd;
	hWnd=::FindWindow(NULL,buffer);	//到当前所有窗口中找到标题为buffer的窗口，返回其句柄
	HINSTANCE hInst=::GetModuleHandle("餐厅点餐系统.exe");		//获得应用程序实例句柄
	::SendMessage(hWnd,WM_SETICON,ICON_SMALL,					//发送消息，更换图标
		(LPARAM)::LoadIcon(hInst,MAKEINTRESOURCE(IDI_ICON1))); 
/*
//删除标题栏

	long lStyle = ::GetWindowLong(hWnd, GWL_STYLE);
	::SetWindowLong(hWnd, GWL_STYLE, lStyle & ~WS_CAPTION);	//删除菜单
	::SetWindowPos(hWnd, NULL, 0, 0, 0, 0,SWP_NOSIZE			//重画窗口
		| SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED);
*/	
//删除标题栏的最大化和关闭按钮
	::DeleteMenu(GetSystemMenu(hWnd, FALSE), SC_CLOSE, MF_BYCOMMAND);
	::DeleteMenu(GetSystemMenu(hWnd, FALSE), SC_MAXIMIZE, MF_BYCOMMAND);
	::DrawMenuBar(hWnd);		//重画菜单

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
	m_pBoss = new CBoss(theMajor.bossPassword,theMajor.bossName,theMajor.bossSex);
	//加载服务员和餐桌对象
	for(i=0;i<theMajor.boardCount && i < INIT_BOARD_COUNT;i++){
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

//画主要界面：所有餐桌
COORD CRunApp::DrawAllBoard(){
	CBoard **ppBoard = &m_pBoard;
	register CBoard * current;
	short i=0;
	SetTextColor(BACKGROUND_WHITE);
	//画分隔栏
	for(i=0;i<WINDOW_MAX_HIGHT;i++){
		GotoPos(WINDOW_MAX_WIDTH-29,i);
		cout<<" ";
	}
	//画餐桌
	COORD rPos={0,0};
	while((current = *ppBoard) != NULL){
		GotoPos(rPos.X+3,rPos.Y+2);
		if(true == current->m_bFlags){
			SetTextColor(FOREGROUND_BLUE | BACKGROUND_RED | BACKGROUND_INTENSITY);
			cout<<"   正使用中  ";
		}
		else{
			SetTextColor(FOREGROUND_PURPLE| BACKGROUND_GREEN | BACKGROUND_INTENSITY );
			cout<<"   正空闲中  ";
		}
		
		GotoPos(rPos.X+3,rPos.Y+3);
		cout<<setfill('0');
		cout<<"   桌号:"<<setw(2)<<current->GetNumber()<<"   ";
		GotoPos(rPos.X+3,rPos.Y+4);
		cout<<setfill('\0');
		cout<<"   人数:"<<setw(2)<<current->GetManCount()<<"   ";
		GotoPos(rPos.X+3,rPos.Y+5);
		cout<<"             ";
		GotoPos(rPos.X+3,rPos.Y+5);
		cout<<"起始:"<<current->GetStartTime();
		rPos.X+=28;
		if (rPos.X>WINDOW_MAX_WIDTH-30) {		//换行
			rPos.X=0;
			rPos.Y+=WINDOW_MAX_HIGHT/6;
		}
		ppBoard = &current->pNextBoard;
	}
	SetTextColor(BACKGROUND_WHITE );
	TextOut(WINDOW_MAX_WIDTH-28,WINDOW_MAX_HIGHT/2-6," 1 点餐                     ");
	TextOut(WINDOW_MAX_WIDTH-28,WINDOW_MAX_HIGHT/2-4," 2 后台管理                 ");
	TextOut(WINDOW_MAX_WIDTH-28,WINDOW_MAX_HIGHT/2-2," 0 退出                     ");
	TextOut(WINDOW_MAX_WIDTH-28,WINDOW_MAX_HIGHT/2,  "   你的选择:                ");
	
	SetTextColor(BACKGROUND_RED | BACKGROUND_INTENSITY);
	TextOut(WINDOW_MAX_WIDTH-20,4,"    ");
	SetTextColor(FOREGROUND_WHITE);
	cout<<" 使用中";
	
	SetTextColor(BACKGROUND_GREEN | BACKGROUND_INTENSITY);
	TextOut(WINDOW_MAX_WIDTH-20,6,"    ");
	SetTextColor(FOREGROUND_WHITE);
	cout<<" 空闲中";

	SetTextColor(FOREGROUND_GREEN |BACKGROUND_WHITE );
	COORD tmpPos={WINDOW_MAX_WIDTH-15,WINDOW_MAX_HIGHT/2};
	return tmpPos;	//返回可供用户输入的点的坐标
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
// 画初始化餐桌界面
COORD CRunApp::DrawInitBoard(){
	CleanBuffer(WINDOW_MAX_WIDTH/3+2,WINDOW_MAX_HIGHT/4,
		2*WINDOW_MAX_WIDTH/3-1,3*WINDOW_MAX_HIGHT/4);
	WORD wOldAttributes=SetTextColor(FOREGROUND_WHITE);
	GotoPos(WINDOW_MAX_WIDTH/3+10,WINDOW_MAX_HIGHT/3);
	cout<<"此餐桌还没启用，请问几位:     (最多20人)";
	SetTextColor(wOldAttributes);
	COORD tmpPos={WINDOW_MAX_WIDTH/3+35,WINDOW_MAX_HIGHT/3};
	return tmpPos;
}
// 画指定餐桌信息
void CRunApp::DrawBoard(CBoard * pBoard){
	CleanBuffer(0,0,WINDOW_MAX_WIDTH/3-1,WINDOW_MAX_HIGHT-2);
	WORD wOldAttributes=SetTextColor(FOREGROUND_WHITE);
	GotoPos(0,0);
	cout<<setfill('0');
	cout<<"桌号: "<<setw(2)<<pBoard->GetNumber()<<setfill('\0')<<endl;
	if(false==pBoard->m_bFlags){
		cout<<"此餐桌处于空闲中"<<endl;	
	}
	else{
		cout<<"从"<<pBoard->GetStartTime()<<"开始"<<"餐桌被使用"<<endl;
		cout<<"就餐人数: "<<setw(2)<<pBoard->GetManCount()<<" 人"<<endl;
		cout<<"餐桌菜单:"<<endl;
		COORD pos={0,5};
		PrintBoardMenu(&pBoard->m_pBoardMenu,pos);
		GotoPos(pos.X,pos.Y);
		cout<<"总消费金额:  "<<pBoard->GetConsume()<<"元";
	}
	SetTextColor(wOldAttributes);
}
//画指定餐桌菜单
void CRunApp::PrintBoardMenu(CBoardMenu** ppBoardMenu,COORD & pos){
	register CBoardMenu * current;
	WORD wOldAttributes=SetTextColor(FOREGROUND_WHITE | COMMON_LVB_UNDERSCORE);
	CleanBuffer(WINDOW_MAX_WIDTH,0,2*WINDOW_MAX_WIDTH/3+1,38);
	GotoPos(pos.X,pos.Y);
	cout<<"菜名        单价      份数     金额    点单服务员";
	while((current = *ppBoardMenu)!=NULL){
		pos.Y+=1;
		TextOut(pos.X,pos.Y,current->m_Food.m_strFoodName);
		TextOut(pos.X+13,pos.Y,current->m_Food.m_dPrice);
		TextOut(pos.X+23,pos.Y,current->m_uShare);
		TextOut(pos.X+33,pos.Y,current->m_dMoney);
		TextOut(pos.X+43,pos.Y,current->m_unWaiterNumber);
		ppBoardMenu = &current->m_pNextBoardFood;
	}
	pos.Y+=2;
	SetTextColor(wOldAttributes);
}
//打印餐厅主菜单
void CRunApp::PrintFoodMenu(){
	CleanBuffer(2*WINDOW_MAX_WIDTH/3+1,0,WINDOW_MAX_WIDTH-1,WINDOW_MAX_HIGHT-2);
	WORD wOldAttributes=SetTextColor(FOREGROUND_WHITE | COMMON_LVB_UNDERSCORE);
	register CFoodMenu * current;
	CFoodMenu ** ppBoardMenu= &m_pFoodMenu;
	GotoPos(FOODMENU_OUPUT_POSITION_X,
		FOODMENU_OUPUT_POSITION_Y-2);
	cout<<"餐厅现有菜肴";
	GotoPos(FOODMENU_OUPUT_POSITION_X,
		FOODMENU_OUPUT_POSITION_Y);
	short i=1;
	cout<<"编号    菜名        类型      单价    ";
	while((current = *ppBoardMenu)!=NULL){
		
		GotoPos(FOODMENU_OUPUT_POSITION_X,FOODMENU_OUPUT_POSITION_Y+i);
		cout<<setfill('0')<<setw(2)<<i<<setfill('\0');
		GotoPos(FOODMENU_OUPUT_POSITION_X+6,FOODMENU_OUPUT_POSITION_Y+i);
		cout<<current->m_Food.m_strFoodName;
		GotoPos(FOODMENU_OUPUT_POSITION_X+20,FOODMENU_OUPUT_POSITION_Y+i);
		switch(current->m_Food.m_uFoodType){
		case SNACK:cout<<"小吃";
			break;
		case DRINK:cout<<"饮料";
			break;
		case ENTREE:cout<<"主菜";
			break;
		}
		GotoPos(FOODMENU_OUPUT_POSITION_X+30,FOODMENU_OUPUT_POSITION_Y+i);
		cout<<setw(2)<<current->m_Food.m_dPrice<<" 元/份";
		i++;
		ppBoardMenu = &current->m_NextFood;
	}
	SetTextColor(wOldAttributes);
}
// 画第一个服务员输入界面（选择桌号，选择服务员编号）
COORD CRunApp::DrawWaiterInputOne(){
	WORD wOldAttributes;
	GetTextColor(&wOldAttributes);
	short i=4;
	CBoard ** ppBoard=&m_pBoard;
	CBoard * current;
	while((current=*ppBoard) != NULL){
		if(current->m_bFlags==true){
			SetTextColor(FOREGROUND_BLUE | BACKGROUND_RED | BACKGROUND_INTENSITY);
		}
		else{	
			SetTextColor(BACKGROUND_GREEN | BACKGROUND_INTENSITY );
		}
		GotoPos(12,i);
		cout<<"桌号"<<setfill('0')<<setw(2)<<current->GetNumber()<<setfill('\0');
		ppBoard = &current->pNextBoard;
		i++;
	}
	SetTextColor(BACKGROUND_WHITE |BACKGROUND_INTENSITY);
	for(i=0;i<WINDOW_MAX_HIGHT;i++){
		GotoPos(WINDOW_MAX_WIDTH/3,i);
		cout<<" ";
		GotoPos(2*WINDOW_MAX_WIDTH/3,i);
		cout<<" ";
	}
	SetTextColor(FOREGROUND_WHITE);
	GotoPos(WINDOW_MAX_WIDTH/3+10,WINDOW_MAX_HIGHT/3);
	cout<<"输入餐桌号    :     请输入正确值";
	GotoPos(WINDOW_MAX_WIDTH/3+10,WINDOW_MAX_HIGHT/3+1);
	cout<<"输入服务员编号:     请输入正确值";
	GotoPos(WINDOW_MAX_WIDTH/3+10,WINDOW_MAX_HIGHT/3+3);
	cout<<"0 退出";
	GotoPos(WINDOW_MAX_WIDTH/3+10,WINDOW_MAX_HIGHT/3+4);
	cout<<"服务员编号栏输入  -1  为结账付款";
	COORD tmpPos={WINDOW_MAX_WIDTH/3+25,WINDOW_MAX_HIGHT/3};
	SetTextColor(wOldAttributes);
	return tmpPos;
}
//画第二个服务员输入界面（bFlags == true 点餐  bFlags ==false  删除菜肴）
COORD CRunApp::DrawWaiterInputTwo(const bool &bFlags){
	CleanBuffer(WINDOW_MAX_WIDTH/3+2,WINDOW_MAX_HIGHT/4+2,
		2*WINDOW_MAX_WIDTH/3-1,3*WINDOW_MAX_HIGHT/4);
	WORD wOldAttributes=SetTextColor(FOREGROUND_WHITE);
	GotoPos(WINDOW_MAX_WIDTH/3+10,WINDOW_MAX_HIGHT/3-4);
	if(true == bFlags){
		cout<<"正在点餐        ";
	}
	else{
		cout<<"正在删除已点菜肴";
	}
	GotoPos(WINDOW_MAX_WIDTH/3+10,WINDOW_MAX_HIGHT/3-2);
	cout<<"0 : 退出  -1 : 删除菜单";
	GotoPos(WINDOW_MAX_WIDTH/3+10,WINDOW_MAX_HIGHT/3);
	cout<<"菜肴编号        :     请输入正确值";
	GotoPos(WINDOW_MAX_WIDTH/3+10,WINDOW_MAX_HIGHT/3+1);
	cout<<"份数            :     请输入正确值";
	COORD tmpPos={WINDOW_MAX_WIDTH/3+27,WINDOW_MAX_HIGHT/3};
	SetTextColor(wOldAttributes);
	return tmpPos;
}
//////////////////////////////////////////////////////////////////////////
//	运行
//////////////////////////////////////////////////////////////////////////
void CRunApp::Run(){
	string strInput="";
	COORD currentPos;
	while(strInput!="0"){
		Cls();
		currentPos=this->DrawAllBoard();
		do{
			Input(strInput,currentPos,&strInput);
		}while(!("1"==strInput || "2" == strInput || "0"==strInput));

		
		if("1"==strInput){
			Order();
		}
		else if("2" == strInput){
			Administrate();
		}
		else{
			Cls();
			TextOut(WINDOW_MAX_WIDTH/2-9,WINDOW_MAX_HIGHT/2,"你确定要退出吗? 1 不是 任意键 是 :");
			currentPos.X=WINDOW_MAX_WIDTH/2+27;
			currentPos.Y=WINDOW_MAX_HIGHT/2;
			Input(strInput,currentPos,&strInput);
			if("1" != strInput)
				strInput = "0";
		}
	}
}
//////////////////////////////////////////////////////////////////////////
//	进入点餐
//////////////////////////////////////////////////////////////////////////

bool CRunApp::Order(){
	Cls();
	COORD currentPos;
	currentPos =DrawWaiterInputOne();
	PrintFoodMenu();
	int iInput;   //用来存放临时输入的数值
	unsigned short unBoardNumber;
	unsigned short unWaiterNumber;
	//选择餐桌
	do{
		Input(unBoardNumber,currentPos);
	}while(unBoardNumber<0 || unBoardNumber> CBoard::GetBoardCount());
	if(0 == unBoardNumber) return false;
	CBoard * pBoard = GetPBoard(unBoardNumber);
	//输入服务员编号
	currentPos.Y+=1;
	do{
		Input(iInput,currentPos);
	}while(!(iInput==-1 || iInput ==0 || (NULL != GetPWaiter(iInput))));

	if(-1 == iInput){
		Cls();
		GotoPos(WINDOW_MAX_WIDTH/2-12,WINDOW_MAX_HIGHT/2);
		if(false == pBoard->m_bFlags){
			cout<<"此餐桌没有使用  ";
			system("echo 按任意键返回 & pause > nul");
			return false;
		}
		DrawBoard(pBoard);
		GotoPos(WINDOW_MAX_WIDTH/2-12,WINDOW_MAX_HIGHT/2);
		currentPos.X=WINDOW_MAX_WIDTH/2+14;
		currentPos.Y=WINDOW_MAX_HIGHT/2;
		cout<<"你确定要结账(1:是 0:不是):";
		do {
			Input(iInput,currentPos);
		} while(!(1==iInput || 0==iInput));
		if(0 == iInput){
			return false;
		}
		pBoard->Clean();
		return true;
	}
	if(0 == iInput) return false;
	unWaiterNumber =iInput;
	CWaiter * pWaiter = GetPWaiter(unWaiterNumber);
	//如果餐桌未被使用。初始化餐桌
	if(false == pBoard->m_bFlags){
		currentPos=DrawInitBoard();
		unsigned uManCount;
		do{
			Input(uManCount,currentPos);
		}while(uManCount<0  || uManCount >20);
		if(0 == uManCount) return false;
		pBoard->m_bFlags = true;
		pBoard->SetManCount(uManCount);
		pBoard->SetStartTime();
	}
	//点餐
	bool bFlags=true;
	
	do
	{
		DrawBoard(pBoard);
		currentPos=this->DrawWaiterInputTwo(bFlags);
		CFoodMenu * pFood;
		unsigned short unFoodNumber;
		unsigned short unShare;
		//选择菜肴
		do{
			Input(iInput,currentPos);
		}while(iInput<-1 || iInput>CFoodMenu::m_unFoodCount);
		if(-1 == iInput){
			bFlags=false;
			continue;
		}
		if(0 == iInput) break;
		unFoodNumber = iInput;
		currentPos.Y+=1;
		//选择份数
		do{
			Input(iInput,currentPos);
		}while(iInput<-1);
		if(-1 == iInput){
			bFlags=false;
			continue;
		}
		if(0 == iInput) break;
		unShare = iInput;
		pFood = GetPFood(unFoodNumber);
		char strTime[25];
		char buffer[80];
		time_t t;
		time(&t);
		sprintf(strTime,"%s",ctime(&t));
		if(true == bFlags){
			pWaiter->AddBoardMenuItem(pBoard,pFood->m_Food.m_uFoodType,
				pFood->m_Food.m_strFoodName,pFood->m_Food.m_dPrice,unShare);
			
			sprintf(buffer,"新增订单桌号: %d 人数: %d 菜名: %s 份数: %d 时间: %s",
				pBoard->GetNumber(),pBoard->GetManCount(),pFood->m_Food.m_strFoodName.c_str(),unShare,strTime);
		}
		else{
			pWaiter->DelBoardMenuItem(pBoard,pFood->m_Food.m_strFoodName,unShare);
			sprintf(buffer,"取消订单桌号: %d 人数: %d 菜名: %s 份数: %d 时间: %s",
				pBoard->GetNumber(),pBoard->GetManCount(),pFood->m_Food.m_strFoodName.c_str(),unShare,strTime);
		}
		ofstream ofTemp;
		if(SNACK == pFood->m_Food.m_uFoodType){
			ofTemp.open("Snack.txt",ios::app);
		}
		else if(DRINK ==pFood->m_Food.m_uFoodType){
			ofTemp.open("Drink.txt",ios::app);
		}
		else
			ofTemp.open("Entree.txt",ios::app);
		//system(buffer);
		if(ofTemp){
			ofTemp.write(buffer,sizeof(buffer));
		}
		ofTemp.close();
		if(false == bFlags) bFlags = true;
	}while(1);
	return true;
}
//打印所有服务员信息
void CRunApp::DrawAllWaiter(){
	CleanBuffer(2*WINDOW_MAX_WIDTH/3+1,0,
		WINDOW_MAX_WIDTH-1,WINDOW_MAX_HIGHT-4);
	WORD wOldAttributes=SetTextColor(FOREGROUND_WHITE | COMMON_LVB_UNDERSCORE);
	TextOut(FOODMENU_OUPUT_POSITION_X,
		FOODMENU_OUPUT_POSITION_Y-2,"餐厅现有服务员");
	TextOut(FOODMENU_OUPUT_POSITION_X,
		FOODMENU_OUPUT_POSITION_Y,"编号    姓名       性别        业绩     工资");
	CWaiter ** ppWaiter = &m_pWaiter;
	CWaiter * current;
	int iTemp=FOODMENU_OUPUT_POSITION_Y+1;
	while((current = * ppWaiter) != NULL){
		GotoPos(FOODMENU_OUPUT_POSITION_X,iTemp);
		cout<<setfill('0')<<setw(2)<<current->GetNumber()<<setfill('\0');
		TextOut(FOODMENU_OUPUT_POSITION_X+6,iTemp,current->GetName());
		TextOut(FOODMENU_OUPUT_POSITION_X+20,iTemp,current->GetSex());
		TextOut(FOODMENU_OUPUT_POSITION_X+33,iTemp,current->GetAchievement());
		TextOut(FOODMENU_OUPUT_POSITION_X+40,iTemp,current->GetSalary());
		ppWaiter = &current->pNextWaiter;
		iTemp+=1;
	}
	SetTextColor(wOldAttributes);
}
//画管理选项
COORD CRunApp::DrawAdminMenu(){
	CleanBuffer(0,0,
		WINDOW_MAX_WIDTH/3-1,WINDOW_MAX_HIGHT-4);
	WORD wOldAttributes=SetTextColor(BACKGROUND_WHITE);
	TextOut(5,5 ,"1 新增餐桌      ");
	TextOut(5,7 ,"2 减少餐桌      ");
	TextOut(5,9 ,"3 新增服务员    ");
	TextOut(5,11,"4 解雇服务员    ");
	TextOut(5,13,"5 设置工资      ");
	TextOut(5,15,"6 新增菜肴      ");
	TextOut(5,17,"7 删除菜肴      ");
	TextOut(5,19,"8 查询营业额    ");
	TextOut(5,21,"9 重置密码      ");
	TextOut(5,23,"0 退出          ");
	TextOut(5,25,"  您的选择:     ");
	SetTextColor(wOldAttributes);
	COORD tempPos={16,25};
	return tempPos;
}
//画处理服务员界面（iFlags == SET_WAITER 更改服务员信息  iFlag == ADD_WAITER  新增服务员  iFlag == DEL_WAITER 解雇服务员）
COORD CRunApp::DrawDisposeWaiter(const int &iFlags){
	CleanBuffer(WINDOW_MAX_WIDTH/5,WINDOW_MAX_HIGHT/5,
		3*WINDOW_MAX_WIDTH/5,3*WINDOW_MAX_HIGHT/5);
	WORD wOldAttributes=SetTextColor(FOREGROUND_WHITE);
	COORD tempPos={WINDOW_MAX_WIDTH/5+2,WINDOW_MAX_HIGHT/5-2};
	if(SET_WAITER == iFlags){
		TextOut(tempPos.X,tempPos.Y,      "更改服务员工资     输入 0 可退出");
		TextOut(tempPos.X,tempPos.Y+2,    "服务员编号:    请确定编号可以使用");
		TextOut(tempPos.X,tempPos.Y+3,    "工资      :");
		tempPos.X+=11;
		tempPos.Y+=2;
		return tempPos;
	}
	if(ADD_WAITER == iFlags){
		TextOut(tempPos.X,tempPos.Y,  "新增服务员     输入 0 可退出");
		TextOut(tempPos.X,tempPos.Y+3,"姓名      :");
		TextOut(tempPos.X,tempPos.Y+4,"性别      :");
		TextOut(tempPos.X,tempPos.Y+5,"工资      :");
	}
	else
		TextOut(tempPos.X,tempPos.Y,  "解雇服务员     输入 0 可退出");
	TextOut(tempPos.X,tempPos.Y+2,    "服务员编号:    请确定编号可以使用");
	tempPos.X+=11;
	tempPos.Y+=2;
	SetTextColor(wOldAttributes);
	return tempPos;
}
// 画处理餐厅主菜单界面 （bFlags == true 新增菜肴  bFlags ==false  删除菜肴）
COORD CRunApp::DrawDisposeFoodMenu(const bool &bFlags){
	CleanBuffer(WINDOW_MAX_WIDTH/5,WINDOW_MAX_HIGHT/5,
		3*WINDOW_MAX_WIDTH/5,3*WINDOW_MAX_HIGHT/5);
	WORD wOldAttributes=SetTextColor(FOREGROUND_WHITE);
	COORD tempPos={WINDOW_MAX_WIDTH/5+2,WINDOW_MAX_HIGHT/5-2};
	if(true == bFlags){
		TextOut(tempPos.X,tempPos.Y,  "新增菜肴     输入 0 可退出");
		TextOut(tempPos.X,tempPos.Y+3,"类型:");
		TextOut(tempPos.X,tempPos.Y+4,"价格:");
		TextOut(tempPos.X,tempPos.Y+6,"注: 1 小吃  2 饮料 3 主菜");
		TextOut(tempPos.X,tempPos.Y+2,    "菜名:");
	}
	else{
		TextOut(tempPos.X,tempPos.Y,  "删除菜肴     输入 0 可退出");
		TextOut(tempPos.X,tempPos.Y+2,    "编号:");
	}
	
	tempPos.X+=5;
	tempPos.Y+=2;
	SetTextColor(wOldAttributes);
	return tempPos;
}
//////////////////////////////////////////////////////////////////////////
//	Boss管理
//////////////////////////////////////////////////////////////////////////

bool CRunApp::Administrate(){
	Cls();
	int iInput;
	COORD  currentPos;
	string strTemp1;
	string strTemp2;
	//输入密码
	TextOut(WINDOW_MAX_WIDTH/2-8,WINDOW_MAX_HIGHT/2  ,"密码:");
	currentPos.X=WINDOW_MAX_WIDTH/2-3;
	currentPos.Y=WINDOW_MAX_HIGHT/2;
	do{
		TextOut(currentPos.X,currentPos.Y,"                 ");
		GotoPos(currentPos.X,currentPos.Y);
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
		Cls();
		currentPos=DrawAdminMenu();
		do{
			Input(iInput,currentPos);
		}while(iInput<0 || iInput>9);
		switch(iInput){
		case 1:		//新增餐桌
			m_pBoss->AddBoard(&m_pBoard);
			TextOut(WINDOW_MAX_WIDTH/2,WINDOW_MAX_HIGHT/2,"增加了1个餐桌");
			system("echo 按任意键确认  & pause > nul");
			break;
		case 2:		//减少餐桌
			m_pBoss->DelBoard(&m_pBoard);
			TextOut(WINDOW_MAX_WIDTH/2,WINDOW_MAX_HIGHT/2,"减少了1个餐桌");
			system("echo 按任意键确认  & pause > nul");
			break;
		case 3:		//新增服务员
			do{
				currentPos=DrawDisposeWaiter(ADD_WAITER);
				DrawAllWaiter();
				unsigned uNumber;
				double dSalarys;
				do{
					Input(uNumber,currentPos);
				}while(uNumber!=0 && GetPWaiter(uNumber));
				if(0==uNumber) break;
				currentPos.Y+=1;
				Input(strTemp1,currentPos,&strTemp1);
				if("0" == strTemp1) break;
				currentPos.Y+=1;
				Input(strTemp2,currentPos,&strTemp2);
				if("0" == strTemp2) break;
				currentPos.Y+=1;
				do{
					Input(dSalarys,currentPos);
				}while(dSalarys<0);
				m_pBoss->AddWaiter(&m_pWaiter,uNumber,dSalarys,strTemp1,strTemp2);
			}while(1);
			break;
		case 4:		//解雇服务员
			do{
				currentPos=DrawDisposeWaiter(DEL_WAITER);
				DrawAllWaiter();
				unsigned uNumber;
				do{
					Input(uNumber,currentPos);
				}while(uNumber!=0 && !GetPWaiter(uNumber));
				if(0==uNumber) break;
				m_pBoss->DelWaiter(&m_pWaiter,uNumber);
			}while(1);
			break;
		case 5:		//设置服务员工资
			do{
				currentPos=DrawDisposeWaiter(SET_WAITER);
				DrawAllWaiter();
				unsigned uNumber;
				do{
					Input(uNumber,currentPos);
				}while(uNumber!=0 && !GetPWaiter(uNumber));
				if(0==uNumber) break;
				currentPos.Y+=1;
				double dSalarys;
				do{
					Input(dSalarys,currentPos);
				}while(dSalarys<0);
				m_pBoss->SetWaiterSalary(&m_pWaiter,uNumber,dSalarys);
			}while(1);
			break;
		case 6:		//增加菜肴
			do{
				currentPos=DrawDisposeFoodMenu(true);
				PrintFoodMenu();
				unsigned uFType;
				double dPrice;
				Input(strTemp1,currentPos,&strTemp1);
				currentPos.Y+=1;
				if("0" == strTemp1) break;
				do{
					Input(uFType,currentPos);
				}while(!(1==uFType || 2==uFType || 3==uFType || 0==uFType));
				if(0==uFType) break;
				currentPos.Y++;
				do {
					Input(dPrice,currentPos);
				} while(dPrice<0);
				if(0==dPrice) break;
				m_pBoss->AddFoodMenuItem(&m_pFoodMenu,uFType,strTemp1,dPrice);
			} while(1);
			break;
		case 7:		//减少菜肴
			do{
				currentPos=DrawDisposeFoodMenu(false);
				PrintFoodMenu();
				do{
					Input(iInput,currentPos);
				}while(iInput<0 || GetPFood(iInput)==NULL);
				if(0 == iInput) break;
				m_pBoss->DelFoodMenuItem(&m_pFoodMenu,GetPFood(iInput)->m_Food.m_strFoodName);
			}while(1);
			break;
		case 8:		//查看营业额
			CleanBuffer(2*WINDOW_MAX_WIDTH/3+1,0,
				WINDOW_MAX_WIDTH-1,WINDOW_MAX_HIGHT-4);
			char buffer[255];
			sprintf(buffer,"日营业额 : %-.4lf",CBoard::GetDailySales());
			TextOut(2*WINDOW_MAX_WIDTH/3+3,4,buffer);
			sprintf(buffer,"小吃销售 : %-.4lf",CBoard::GetSnackSales());
			TextOut(2*WINDOW_MAX_WIDTH/3+3,5,buffer);
			sprintf(buffer,"饮料销售 : %-.4lf",CBoard::GetDrinkSales());
			TextOut(2*WINDOW_MAX_WIDTH/3+3,6,buffer);
			sprintf(buffer,"主菜销售 : %-.4lf",CBoard::GetEntreeSales());
			TextOut(2*WINDOW_MAX_WIDTH/3+3,7,buffer);
			GotoPos(2*WINDOW_MAX_WIDTH/3+3,9);
			system("echo 按任意键返回  & pause > nul");
			break;
		case 9:		//修改密码
			CleanBuffer(WINDOW_MAX_WIDTH/5,WINDOW_MAX_HIGHT/5,
				3*WINDOW_MAX_WIDTH/5,3*WINDOW_MAX_HIGHT/5);
			TextOut(WINDOW_MAX_WIDTH/5-2,WINDOW_MAX_HIGHT/5-2,"新的密码:               (任意字符串)");
			currentPos.X=WINDOW_MAX_WIDTH/5+9,currentPos.Y=WINDOW_MAX_HIGHT/5-2;
			Input(strTemp1,currentPos,&strTemp1);
			m_pBoss->ChangePassword(strTemp1);
			GotoPos(WINDOW_MAX_WIDTH/5-2,WINDOW_MAX_HIGHT/5+1);
			cout<<"更改成功! 密码:"<<m_pBoss->GetPassword()<<" 请牢记";
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