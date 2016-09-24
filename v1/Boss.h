#ifndef __SHOPKEEPER_H__
#define __SHOPKEEPER_H__
#include "FoodMenu.h"
#include "Person.h"
#include "Board.h"

#include "Waiter.h"
class CWaiter;
//////////////////////////////////////////////////////////////////////////
//	店长类，继承于人类
//
//////////////////////////////////////////////////////////////////////////
class CBoss:public CPerson{
private:
	string m_strPassword;
public:
	CBoss(const string & strPassword,
		const string & strName="", 
		const string & strSex="");
	void ChangePassword(const string & strPassword);				//更改密码
	string GetPassword()const;
	CFoodMenu * AddFoodMenuItem(CFoodMenu ** ppFoodMenu,			//增加餐厅菜肴
		const unsigned &uFoodType,
		const string &strFoodName,
		const double & dPrice)const;
	bool DelFoodMenuItem(CFoodMenu **ppFoodMenu,					//删除餐厅菜肴
		const string &strFoodName);
	bool AddBoard(CBoard ** ppBoard)const;							//增加餐桌
	bool DelBoard(CBoard ** ppBoard);								//减少餐桌 
	bool AddWaiter(CWaiter **ppWaiter,
		const unsigned short &unNumber,
		const double &dSalary,
		const string & strName="", 
		const string & strSex="",
		const double & dAchievement=0)const;								//增加服务员
	bool DelWaiter(CWaiter **ppWaiter,								//删除服务员
		const unsigned short &unNumber)const;
	//设置服务员工资
	bool SetWaiterSalary(CWaiter **ppWaiter,
		const unsigned short &unNumber,
		const double & dSalary)const;
	bool SetWaiterSalary(CWaiter * pWaiter, 
		const double & dSalary)const;
};
#endif//__SHOPKEEPER_H__