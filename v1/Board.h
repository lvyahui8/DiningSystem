#ifndef __BOARD_H__
#define __BOARD_H__

#include "FoodMenu.h"


class CBoard{	
private:
	const unsigned short m_unNumber;
	unsigned m_uManCount;
	time_t m_lStartTime;
	double m_dConsume;
	static double m_dDailySales;
	static unsigned  short m_unBoardCount;
	static double m_dSnackSales;	//小吃消费
	static double m_dDrinkSales;	//饮料消费
	static double m_dEntreeSales;	//主菜消费
public:
	bool m_bFlags;
	
	CBoardMenu * m_pBoardMenu;
	CBoard *pNextBoard;
public:
	CBoard(const unsigned short & unNumber=0);
	~CBoard(){
		this->m_unBoardCount--;
	}
	unsigned short GetNumber()const{
		return m_unNumber;
	}
	bool SetManCount(const unsigned & uManCount);
	unsigned GetManCount()const{
		return this->m_uManCount;
	}
	void Clean();									//清理餐桌痕迹
	void DelBoardMenuAll(CBoardMenu * current);		//删除当前餐桌上的所有菜单
	void SetStartTime();							//设置餐桌被使用的起始时间
	char * GetStartTime()const;						//获得起始时间
	void SetConsume(const double & dConsume);		//设置消费额
	void AddConsume(const double & dConsume);		//增加消费额
	double GetConsume()const;						//获得消费额
	static double GetSnackSales(){					//获得小吃营业额
		return m_dSnackSales;
	}
	static double GetDrinkSales(){					//获得饮料营业额
		return m_dDrinkSales;
	}
	static double GetEntreeSales(){					//获得主菜营业额
		return m_dEntreeSales;
	}
	static double GetDailySales(){					//获得日营业额
		return m_dDailySales;
	}
	static unsigned short GetBoardCount(){			//获得餐桌数目
		return m_unBoardCount;
	}
};

#endif /*__BOARD_H_*/