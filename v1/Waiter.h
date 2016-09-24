#ifndef __WAITER_H__
#define __WAITER_H__

#include "Person.h"
#include "Boss.h"

class CWaiter: public CPerson{	
private:
	unsigned short m_unNumber;		//编号
	double m_dAchievement;			//业绩
	double m_dSalary;				//工资
	static double m_dAllSalary;		//所有服务员的工资
	static unsigned short m_unWaiterCount;	//服务员人数
public:
	CWaiter * pNextWaiter;
public:
	CWaiter(const unsigned short &unNumber,
		const double &dSalary,
		const string & strName="", 
		const string & strSex="",
		const double & dAchievement=0);
	~CWaiter(){
		m_unWaiterCount--;
	}
	void StartUsingBoard()const;
	//取得自己的编号
	unsigned short  GetNumber()const;
	//取得自己的业绩
	double GetAchievement()const;
	//取得自己的工资
	double GetSalary()const;
	//给餐桌点餐
	bool AddBoardMenuItem(CBoard *pBoard,
		const unsigned &uFoodType,
		const string &strFoodName,
		const double &dPrice,
		const unsigned &uShare);
	//删除已点菜肴
	bool DelBoardMenuItem(CBoard * pBoard,
		const string &strFoodName,
		const unsigned &uShare);
	
	static unsigned short GetWaiterCount(){
		return m_unWaiterCount;
	}
	friend bool CBoss::SetWaiterSalary(CWaiter **ppWaiter, 
		const unsigned short &unNumber, 
		const double & dSalary)const;

	friend bool CBoss::SetWaiterSalary(CWaiter * pWaiter, 
		const double & dSalary)const;
};

#endif /*__CWAITER_H_*/;
	