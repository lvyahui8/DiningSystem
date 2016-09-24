#ifndef __PERSON_H__
#define __PERSON_H__
#include <string>
#include "FoodMenu.h"
using std::string;
//////////////////////////////////////////////////////////////////////////
//		人类
//		拥有老板和服务员的公共属性和行为
//////////////////////////////////////////////////////////////////////////
class CPerson{	
private:
	string m_strName;		//姓名
	string m_strSex;		//性别
public:
	CPerson(const string &strName="", const string &strSex="");
	//设置名字，性别
	void SetNameAndSex(const string & strName,const string &strSex="");
	//取得姓名
	string GetName()const{return this->m_strName;}
	//取得性别
	string GetSex()const{return this->m_strSex;}
};

#endif /*__PERSON_H_*/