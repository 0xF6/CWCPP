#pragma once
#include "CString.h"
#include "CWString.h"


bool IsSign(wchar_t symbol);

bool IsSign(char symbol);

bool IsXmlSign(wchar_t symbol);

bool IsXmlSign(char symbol);

CString ConvertText(CWString &text);

CWString ConvertText(CString &text);

//Конвертирует текст wstring/string | CString/CWString
#define CV(s) ConvertText(s)


//Нормализация текста
CWString NormaliseText(CWString text);

//Нормализация текста
CWString NormaliseXMLText(CWString text);

size_t STS(CString str);

CString STS(size_t size);

bool Boolean(CString str);

bool Boolean(CWString wstr);


CString DWTS(DWORD dword);

DWORD STDW(CString str);

#include <sstream>

//Конвертирует какой-либо стандартный тип в строку
template<typename type1> CString DefTypeToString(type1 arg)
{
	CString str;
	std::stringstream ss;
	ss << arg;
	ss >> str;
	return str;
}


//Конвертирует строку в какой-либо стандартный тип
template<typename type1> type1 StringToDefType(CString arg)
{
	type1 ret;
	std::stringstream ss;
	ss << arg;
	ss >> ret;
	return ret;
}

CString DefTypeToString(HWND arg);

HWND StringToDefType(CString arg);

//Converting default type to string
#define DTTS(t) DefTypeToString(t)

//Converting string to default type
#define STDT(s) StringToDefType(s)




bool IsMathSymbol(char Char);

char ToChar(unsigned short int num);

unsigned short int ToNum(char Char);

#ifndef STDExlude

CString ToText(long int Number);

long int ToNumber(CString Text);


bool Exists(CString str, CString what);

bool Exists(CWString str, CWString what);

#endif