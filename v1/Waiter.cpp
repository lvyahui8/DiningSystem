#include "Waiter.h"
#include "FoodMenu.h"
#include "Person.h"
#include <string>

using std::string;
double CWaiter::m_dAllSalary = 0;
unsigned short CWaiter::m_unWaiterCount=0;


CWaiter::CWaiter(const unsigned short &unNumber,
				 const double &dSalary, 
				 const string & strName/* ="" */, 
				 const string & strSex/* ="" */,
				 const double & dAchievement/* =0 */)
				 :m_unNumber(unNumber),CPerson(strName,strSex){
	this->m_dSalary = dSalary;
	this->m_dAchievement = 0;
	this->pNextWaiter = NULL;
	CWaiter::m_dAllSalary += dSalary;
	this->m_unWaiterCount++;
}

unsigned short  CWaiter::GetNumber()const{
	return this->m_unNumber;
}
double CWaiter::GetAchievement()const{
	return this->m_dAchievement;
}
double CWaiter::GetSalary()const{
	return this->m_dSalary;
}
bool CWaiter::AddBoardMenuItem(CBoard *pBoard,
							   const unsigned &uFoodType,
							   const string &strFoodName,
							   const double &dPrice,		
							   const unsigned &uShare){
	register CBoardMenu * current,**ppBoardMenu=&pBoard->m_pBoardMenu;
	while((current = *ppBoardMenu)!=NULL && current->m_Food.m_uFoodType != uFoodType){
		ppBoardMenu = &current->m_pNextBoardFood;
	}
	CBoardMenu * pNewFood = new CBoardMenu;

	if(NULL == pNewFood)
		return false;

	pNewFood->m_Food.m_uFoodType = uFoodType;
	pNewFood->m_Food.m_strFoodName = strFoodName;
	pNewFood->m_Food.m_dPrice = dPrice;
	pNewFood->m_uShare = uShare;
	pNewFood->m_dMoney = pNewFood->m_Food.m_dPrice * pNewFood->m_uShare;
	pNewFood->m_NextFood=NULL;
	pNewFood->m_unWaiterNumber = this->GetNumber();
	
	pNewFood->m_pNextBoardFood = current;

	*ppBoardMenu = pNewFood;
	//增加服务员业绩
	this->m_dAchievement  += pNewFood->m_dMoney ;
	pBoard->AddConsume(pNewFood->m_dMoney);
	return true;
}

bool CWaiter::DelBoardMenuItem(CBoard * pBoard,
							   const string &strFoodName,
							   const unsigned &uShare){
	register CBoardMenu * current,**ppBoardMenu=&pBoard->m_pBoardMenu;
	while((current = *ppBoardMenu)!=NULL && 
		!(current->m_Food.m_strFoodName==strFoodName && current->m_uShare == uShare)){
		ppBoardMenu = &current->m_pNextBoardFood;
	}
	if(NULL == current){
		return false;
	}
	*ppBoardMenu = current->m_pNextBoardFood;
	delete current;
	return true;
}