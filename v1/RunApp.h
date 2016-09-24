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
#include <windows.h>

class CRunApp  
{
public:
	CRunApp();
	virtual ~CRunApp();
	void Run();
	bool Order();
	bool Administrate();
	COORD DrawAllBoard();
	COORD DrawInitBoard();
	COORD DrawWaiterInputOne();
	COORD DrawWaiterInputTwo(const bool &bFlags);
	COORD DrawAdminMenu();
	COORD DrawDisposeWaiter(const int &iFlags);
	COORD DrawDisposeFoodMenu(const bool &bFlags);
	void DrawAllWaiter();
	void DrawBoard(CBoard * pBoard);
	void PrintBoardMenu(CBoardMenu**ppBoardMenu,COORD & pos);
	void PrintFoodMenu();
	CBoard * GetPBoard(const unsigned short & unNumber);
	CWaiter * GetPWaiter(const unsigned short & unNumber);
	CFoodMenu * GetPFood(const unsigned short & unNumber);
private:
	CWaiter * m_pWaiter;
	CBoard * m_pBoard;
	CFoodMenu * m_pFoodMenu;
	CBoss * m_pBoss;
};

#endif // !defined(AFX_RUNAPP_H__BE98F325_7CB1_4BFF_8EAE_2E07C9E0E242__INCLUDED_)
