// Draw.h: interface for the CDraw class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRAW_H__8DD2D970_EE43_4C9C_867A_6E2DBEE1D254__INCLUDED_)
#define AFX_DRAW_H__8DD2D970_EE43_4C9C_867A_6E2DBEE1D254__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include<graphics.h>
class CDraw  
{
private:
	color_t		foreground_color;		//	前景色
	color_t		background_color;		//	背景色
	LOGFONT		font;					//	字体
	char * picture;						//	图片位置
public:
	void DrawRect(int left, int top, int right, int bottom,
		color_t frameColor=BLACK,color_t fillColor=-1,
		int frameWidth=0,
		const char * caption=NULL,
		bool isSolid=true);
	color_t SetForeColor(const color_t & foreColor);
	color_t SetBackColor(const color_t & backColor);
	void PrintText(int x,int y,const char *str,int nHeight=0,const char *fontName=NULL);
	CDraw(char * bkPic=NULL,color_t foreColor = BLACK,color_t backColor = WHITE);
	void VirtualKeyboard(const char *hint,int * pDeposit);
	void SetBackground(char * newPicture = NULL)const;
	virtual ~CDraw();
};

#endif // !defined(AFX_DRAW_H__8DD2D970_EE43_4C9C_867A_6E2DBEE1D254__INCLUDED_)
