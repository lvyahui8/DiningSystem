#include "Waiter.h"
#include "Boss.h"
#include "FoodMenu.h"

#include <string>
using std::string;
CBoss::CBoss(const string & strPassword, 
			 const string & strName/* ="" */,
			 const string & strSex/* ="" */):CPerson(strName,strSex){
	this->m_strPassword = strPassword;
}


void CBoss::ChangePassword(const string & strPassword){
	this->m_strPassword = strPassword;
}
CFoodMenu * CBoss::AddFoodMenuItem(CFoodMenu ** ppFoodMenu,
								   const unsigned &uFoodType,
								   const string &strFoodName,
								   const double & dPrice)const{
	register CFoodMenu * current;

	while((current = *ppFoodMenu)!=NULL && current->m_Food.m_uFoodType != uFoodType){
		ppFoodMenu = &current->m_NextFood;
	}
	while((current = *ppFoodMenu)!=NULL && current->m_Food.m_uFoodType == uFoodType){
		ppFoodMenu = &current->m_NextFood;
	}
	CBoardMenu * pNewFood = new CBoardMenu;
	if(NULL == pNewFood)
		return NULL;
	pNewFood->m_Food.m_uFoodType = uFoodType;
	pNewFood->m_Food.m_strFoodName = strFoodName;
	pNewFood->m_Food.m_dPrice = dPrice;
	CFoodMenu::m_unFoodCount++;
	pNewFood->m_NextFood = current;
	*ppFoodMenu = pNewFood;
	return pNewFood;
}
bool CBoss::DelFoodMenuItem(CFoodMenu **ppFoodMenu, const string &strFoodName){
	register CFoodMenu * current;
	while((current = *ppFoodMenu)!=NULL && current->m_Food.m_strFoodName != strFoodName){
		ppFoodMenu = &current->m_NextFood;
	}

	if(NULL == current){
		return false;
	}
	CFoodMenu::m_unFoodCount--;
	*ppFoodMenu = current->m_NextFood;
	delete current;
	return true;
}
bool CBoss::AddBoard(CBoard ** ppBoard)const{
	register CBoard * current;
	short i=0;
	while((current = *ppBoard) != NULL){
		ppBoard = &current->pNextBoard;
		i++;
	}
	
	CBoard * pNewBoard;
	pNewBoard = new CBoard(i+1);
	if(NULL == pNewBoard)
		return false;

	*ppBoard = pNewBoard;
	return true;
}

bool CBoss::DelBoard(CBoard ** ppBoard){
	register CBoard * current;

	while((current = *ppBoard)->pNextBoard != NULL){
		ppBoard = &current->pNextBoard;
	}

	*ppBoard = NULL;
	delete current;		//当我将DelBoard函数设为常函数。这条语句执行不彻底.没有调用CBoard的析构函数
	return true;
}

bool CBoss::AddWaiter(CWaiter ** ppWaiter,
					  const unsigned short &unNumber,
					  const double &dSalary,
					  const string & strName/* ="" */, 
					  const string & strSex/* ="" */,
					  const double & dAchievement/* =0 */)const{
	register CWaiter * current;
	while((current = *ppWaiter) != NULL){
		ppWaiter = &current->pNextWaiter;
	}
	
	CWaiter * pNewWaiter;
	pNewWaiter = new CWaiter(unNumber,dSalary,strName,strSex);
	if(NULL == pNewWaiter)
		return false;
	*ppWaiter = pNewWaiter;
	return true;
}
bool CBoss::DelWaiter(CWaiter **ppWaiter,
					  const unsigned short &unNumber)const{
	register CWaiter * current;

	while((current = *ppWaiter) != NULL && current->GetNumber() != unNumber){
		ppWaiter = &current->pNextWaiter;
	}

	if (NULL == current) {
		return false;
	}
	*ppWaiter = current->pNextWaiter;
	delete current;
	return true;
}

bool CBoss::SetWaiterSalary(CWaiter **ppWaiter,
							const unsigned short &unNumber,
							const double & dSalary)const{
	register CWaiter * current;

	while((current = *ppWaiter) != NULL && current->GetNumber() != unNumber){
		ppWaiter = &current->pNextWaiter;
	}
	
	if(NULL == current){
		return false;
	}
	current->m_dSalary = dSalary;
	return true;
}

bool CBoss::SetWaiterSalary(CWaiter * pWaiter, const double & dSalary)const{
	if(NULL == pWaiter)
		return false;
	pWaiter->m_dSalary = dSalary;
	return true;
}


string CBoss::GetPassword()const{
	return this->m_strPassword;
}