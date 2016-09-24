#include "Board.h"
#include <ctime>
double CBoard::m_dDailySales = 0;
unsigned  short CBoard::m_unBoardCount =0;
double CBoard::m_dDrinkSales = 0;
double CBoard::m_dEntreeSales = 0;
double CBoard::m_dSnackSales = 0;

CBoard::CBoard(const unsigned short & unNumber/*=0*/):m_unNumber(unNumber){
	this->m_bFlags = 0;
	this->SetConsume(0);
	this->m_uManCount =0;
	this->pNextBoard = NULL;
	this->m_pBoardMenu = NULL;
	this->m_lStartTime =0;	//time(&m_lStartTime);
	this->m_unBoardCount++;
}
bool CBoard::SetManCount(const unsigned & uManCount){
	if (uManCount>20){
		return false;
	}
	else{
		this->m_uManCount = uManCount;
		return true;
	}
}
void CBoard::SetStartTime(){
	time_t rtime;
	time(&rtime);
	this->m_lStartTime = rtime;
}

char * CBoard::GetStartTime()const{
	struct tm * timeTmp = localtime(&m_lStartTime);
	static char buffer[255]={0};
	if(true == m_bFlags)
		sprintf(buffer,"%d:%d:%d",timeTmp->tm_hour,timeTmp->tm_min,timeTmp->tm_sec);
	else
		sprintf(buffer,"%d:%d:%d",0,0,0);
	return buffer;
}

void CBoard::SetConsume(const double & dConsume){
	this->m_dConsume = dConsume;
}

void CBoard::AddConsume(const double & dMoney){
	this->m_dConsume += dMoney;
}

double CBoard::GetConsume()const{
	return this->m_dConsume;
}

void CBoard::Clean(){
	this->m_bFlags = 0;
	this->m_dDailySales += GetConsume();
	this->SetConsume(0);
	this->m_uManCount =0;
	DelBoardMenuAll(this->m_pBoardMenu);
	this->m_pBoardMenu = NULL;
	this->m_lStartTime =0;	//time(&m_lStartTime);
}

void CBoard::DelBoardMenuAll(CBoardMenu * current){
	CBoardMenu * pNext;
	pNext = current->m_pNextBoardFood;
	if(NULL != pNext)
		DelBoardMenuAll(pNext);
	//添加到各类型菜肴消费记录中
	if(SNACK == current->m_Food.m_uFoodType){
		this->m_dSnackSales += current->m_dMoney;
	}
	else if(DRINK == current->m_Food.m_uFoodType){
		this->m_dDrinkSales += current->m_dMoney;
	}
	else{
		this->m_dEntreeSales += current->m_dMoney;
	}
	delete current;
}