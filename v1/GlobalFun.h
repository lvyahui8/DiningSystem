#ifndef __GLOBALFUN_H__
#define __GLOBALFUN_H__

#include <iostream>
#include <windows.h>
#include <typeinfo.h>
using std::cout;
//设定光标位置
bool GotoPos(const int &x,const int &y){
	COORD pos={x,y};
	HANDLE hOut=::GetStdHandle(STD_OUTPUT_HANDLE);
	if(INVALID_HANDLE_VALUE == hOut)
		return false;
    ::SetConsoleCursorPosition(hOut, pos);
	return true;
}
//设定窗口大小
void SetWindowSize(const int &cx,const int &cy){
	char buffer[40]={0};
	sprintf(buffer,"mode con cols=%d lines=%d",cx,cy);
	system(buffer);
}
//获取当前的文本和背景颜色
bool GetTextColor(WORD * lpwAttributes){
	CONSOLE_SCREEN_BUFFER_INFO info;
	HANDLE hOut=::GetStdHandle(STD_OUTPUT_HANDLE);
	if(INVALID_HANDLE_VALUE == hOut)
		return false;
	::GetConsoleScreenBufferInfo(hOut,&info);
	*lpwAttributes=info.wAttributes;
	return true;
}
//设定文本和背景颜色
WORD SetTextColor(const WORD wAttributes = 0){
	HANDLE hOut=::GetStdHandle(STD_OUTPUT_HANDLE);
	WORD wOldAttributes;
	GetTextColor(&wOldAttributes);
	::SetConsoleTextAttribute(hOut, wAttributes);
	return wOldAttributes;
}

//清屏
void Cls(){
	system("cls");
	system("color 07");
}

//清除指定缓冲区
void CleanBuffer(int left, int top, int right, int bottom){
	short i,j;
	for(i=0;i<bottom-top;i++){
		GotoPos(left,top+i);
		for(j=0;j<right-left;j+=2){
			cout<<"  ";
		}
	}
}

template<class vType>
void Input(vType & value,const COORD & currentPos,string *pStr=NULL){
	string strTemp;
	GotoPos(currentPos.X,currentPos.Y);
	cout<<"    ";	
	do{	//保证在没有输入的前提下不换行
		char ch;
		GotoPos(currentPos.X,currentPos.Y);
		while((ch=getchar()) != 10){
			strTemp.append(1,ch);
		}
	}while(""==strTemp);

	if(typeid(value)==typeid(double)){
		value = atof(strTemp.c_str());
	}
	else if(typeid(value)==typeid(string)){
		*pStr = strTemp;
	}
	else{
		value = atoi(strTemp.c_str());
	}
}

template<class vType>
void TextOut(const int &x,const int& y,vType value){
	GotoPos(x,y);
	cout<<value;
}
#endif//__GLOBALFUN_H__