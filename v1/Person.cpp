#include "Person.h"
#include "FoodMenu.h"
#include <string>
using std::string;

CPerson::CPerson(const string &strName/* ="" */, const string &strSex/* ="" */){
	// TODO put your implementation here.
	this->m_strName=strName;
	this->m_strSex = strSex;
}

void CPerson::SetNameAndSex (const string & strName,const string &strSex/* =' ' */){
	this->m_strName = strName;
	this->m_strSex = strSex;
}
