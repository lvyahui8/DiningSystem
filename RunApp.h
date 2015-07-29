// RunApp.h: interface for the CRunApp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RUNAPP_H__BE98F325_7CB1_4BFF_8EAE_2E07C9E0E242__INCLUDED_)
#define AFX_RUNAPP_H__BE98F325_7CB1_4BFF_8EAE_2E07C9E0E242__INCLUDED_


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "FoodMenu.h"
#include "Board.h"
#include "Waiter.h"
#include "Boss.h"
#include "Draw.h"	// Added by ClassView

class CRunApp  
{
public:
	CRunApp();
	virtual ~CRunApp();
	void Run();
	bool Order(CBoard * pBoard);
	bool Administrate();
	void PrintBoardInfo(CBoard *pBoard,const RECT & INFO_AREA);
	CBoard * GetPBoard(const unsigned short & unNumber);
	CWaiter * GetPWaiter(const unsigned short & unNumber);
	CFoodMenu * GetPFood(const unsigned short & unNumber);
private:
	CDraw theDraw;
	CWaiter * m_pWaiter;
	CBoard * m_pBoard;
	CFoodMenu * m_pFoodMenu;
	CBoss * m_pBoss;
};

#endif // !defined(AFX_RUNAPP_H__BE98F325_7CB1_4BFF_8EAE_2E07C9E0E242__INCLUDED_)
