#pragma once

#define DECLARE_SINGLETON(classname)\
private:\
classname();\
~classname();\
public:\
static classname* GetInstance();\
void DestroyInstance();\
private:\
static classname * m_pInstance; 



#define IMPLEMENT_SINGLETON(classname)\
classname* classname::m_pInstance =nullptr;\
classname* classname::GetInstance()\
{\
	if(m_pInstance==nullptr)\
		m_pInstance = new classname;\
	return m_pInstance;\
}\
void classname::DestroyInstance()\
{\
	if(m_pInstance!=nullptr)\
	{\
		delete m_pInstance;\
		m_pInstance = nullptr;\
	}\
}


