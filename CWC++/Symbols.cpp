#include <windows.h>
#include "CString.h"
#include "CWString.h"



#include "Symbols.h"
#include "MathLib.h"


bool IsSign(wchar_t symbol)
{
	return (symbol == ' ' || symbol == '\n' || symbol == '\r' || symbol == '\0' 
		|| symbol == '=' || symbol == '+' || symbol == '\'' || symbol == '\"'
		|| symbol == '<' || symbol == '>' || symbol == '\t' || symbol == '/');
}

bool IsSign(char symbol)
{
	return (symbol == ' ' || symbol == '\n' || symbol == '\r' || symbol == '\0' 
		|| symbol == '=' || symbol == '+' || symbol == '\'' || symbol == '\"'
		|| symbol == '<' || symbol == '>' || symbol == '\t' || symbol == '/');
}

bool IsXmlSign(wchar_t symbol)
{
	return (symbol == '<' || symbol == '>' || symbol == '\t' || symbol == '/');
}

bool IsXmlSign(char symbol)
{
	return (symbol == '<' || symbol == '>' || symbol == '\t' || symbol == '/');
}

inline char ConvertSymbol(wchar_t symbol)
{
	if (symbol == L'�') return '�';
	else if (symbol == L'�') return '�';
	else if (symbol == L'�') return '�';
	else if (symbol == L'�') return '�';
	else if (symbol == L'�') return '�';
	else if (symbol == L'�') return '�';
	else if (symbol == L'�') return '�';
	else if (symbol == L'�') return '�';
	else if (symbol == L'�') return '�';
	else if (symbol == L'�') return '�';
	else if (symbol == L'�') return '�';
	else if (symbol == L'�') return '�';
	else if (symbol == L'�') return '�';
	else if (symbol == L'�') return '�';
	else if (symbol == L'�') return '�';
	else if (symbol == L'�') return '�';
	else if (symbol == L'�') return '�';
	else if (symbol == L'�') return '�';
	else if (symbol == L'�') return '�';
	else if (symbol == L'�') return '�';
	else if (symbol == L'�') return '�';
	else if (symbol == L'�') return '�';
	else if (symbol == L'�') return '�';
	else if (symbol == L'�') return '�';
	else if (symbol == L'�') return '�';
	else if (symbol == L'�') return '�';
	else if (symbol == L'�') return '�';
	else if (symbol == L'�') return '�';
	else if (symbol == L'�') return '�';
	else if (symbol == L'�') return '�';
	else if (symbol == L'�') return '�';
	else if (symbol == L'�') return '�';
	else if (symbol == L'�') return '�';
	else if (symbol == L'�') return '�';
	else if (symbol == L'�') return '�';
	else if (symbol == L'�') return '�';
	else if (symbol == L'�') return '�';
	else if (symbol == L'�') return '�';
	else if (symbol == L'�') return '�';
	else if (symbol == L'�') return '�';
	else if (symbol == L'�') return '�';
	else if (symbol == L'�') return '�';
	else if (symbol == L'�') return '�';
	else if (symbol == L'�') return '�';
	else if (symbol == L'�') return '�';
	else if (symbol == L'�') return '�';
	else if (symbol == L'�') return '�';
	else if (symbol == L'�') return '�';
	else if (symbol == L'�') return '�';
	else if (symbol == L'�') return '�';
	else if (symbol == L'�') return '�';
	else if (symbol == L'�') return '�';
	else if (symbol == L'�') return '�';
	else if (symbol == L'�') return '�';
	else if (symbol == L'�') return '�';
	else if (symbol == L'�') return '�';
	else if (symbol == L'�') return '�';
	else if (symbol == L'�') return '�';
	else if (symbol == L'�') return '�';
	else if (symbol == L'�') return '�';
	else if (symbol == L'�') return '�';
	else if (symbol == L'�') return '�';
	else if (symbol == L'�') return '�';
	else if (symbol == L'�') return '�';
	else if (symbol == L'�') return '�';
	else if (symbol == L'�') return '�';
	else
	{
		return (char) symbol;
	}
}

inline wchar_t ConvertSymbol(char symbol)
{
	if (symbol == '�') return L'�';
	else if (symbol == '�') return L'�';
	else if (symbol == '�') return L'�';
	else if (symbol == '�') return L'�';
	else if (symbol == '�') return L'�';
	else if (symbol == '�') return L'�';
	else if (symbol == '�') return L'�';
	else if (symbol == '�') return L'�';
	else if (symbol == '�') return L'�';
	else if (symbol == '�') return L'�';
	else if (symbol == '�') return L'�';
	else if (symbol == '�') return L'�';
	else if (symbol == '�') return L'�';
	else if (symbol == '�') return L'�';
	else if (symbol == '�') return L'�';
	else if (symbol == '�') return L'�';
	else if (symbol == '�') return L'�';
	else if (symbol == '�') return L'�';
	else if (symbol == '�') return L'�';
	else if (symbol == '�') return L'�';
	else if (symbol == '�') return L'�';
	else if (symbol == '�') return L'�';
	else if (symbol == '�') return L'�';
	else if (symbol == '�') return L'�';
	else if (symbol == '�') return L'�';
	else if (symbol == '�') return L'�';
	else if (symbol == '�') return L'�';
	else if (symbol == '�') return L'�';
	else if (symbol == '�') return L'�';
	else if (symbol == '�') return L'�';
	else if (symbol == '�') return L'�';
	else if (symbol == '�') return L'�';
	else if (symbol == '�') return L'�';
	else if (symbol == '�') return L'�';
	else if (symbol == '�') return L'�';
	else if (symbol == '�') return L'�';
	else if (symbol == '�') return L'�';
	else if (symbol == '�') return L'�';
	else if (symbol == '�') return L'�';
	else if (symbol == '�') return L'�';
	else if (symbol == '�') return L'�';
	else if (symbol == '�') return L'�';
	else if (symbol == '�') return L'�';
	else if (symbol == '�') return L'�';
	else if (symbol == '�') return L'�';
	else if (symbol == '�') return L'�';
	else if (symbol == '�') return L'�';
	else if (symbol == '�') return L'�';
	else if (symbol == '�') return L'�';
	else if (symbol == '�') return L'�';
	else if (symbol == '�') return L'�';
	else if (symbol == '�') return L'�';
	else if (symbol == '�') return L'�';
	else if (symbol == '�') return L'�';
	else if (symbol == '�') return L'�';
	else if (symbol == '�') return L'�';
	else if (symbol == '�') return L'�';
	else if (symbol == '�') return L'�';
	else if (symbol == '�') return L'�';
	else if (symbol == '�') return L'�';
	else if (symbol == '�') return L'�';
	else if (symbol == '�') return L'�';
	else if (symbol == '�') return L'�';
	else if (symbol == '�') return L'�';
	else if (symbol == '�') return L'�';
	else if (symbol == '�') return L'�';
	else
	{
		return (wchar_t) symbol;
	}
}

CString ConvertText(CWString &text)
{
	CString str;
	register size_t len = text.len();
	char *temp = new char[len];
	for (register size_t pos = 0; pos < len; pos++)
	{
		temp[pos] = ConvertSymbol(text[pos]);
	}
	str.add(temp, len);
	delete [len] temp;
	return str;
}

CWString ConvertText(CString &text)
{
	CWString str;
	register size_t len = text.len();
	wchar_t *temp = new wchar_t[len];
	for (register size_t pos = 0; pos < len; pos++)
	{
		temp[pos] = ConvertSymbol(text[pos]);
	}
	str.add(temp, len);
	delete [len] temp;
	return str;
}

CWString NormaliseText(CWString text)
{
	size_t pos = 0, size = text.size();
	while (pos < size)
	{
		if (text[pos] > 8000)
		{
			if (text[pos] == 9834)
			{
				text[pos] = '\n';
			}
			else if (text[pos] == 9689)
			{
				text[pos] = '\r';
			}
			else if (text[pos] == 9675)
			{
				text[pos] = '\t';
			}
		}
		else if (text[pos] == 164)
		{
			text.remove(pos);
			pos--;
			size--;
		}
		pos++;
	}
	return text;
}

CWString NormaliseXMLText(CWString text)
{
	size_t pos = 0, size = text.size();
	while (pos < size)
	{
		if (text[pos] > 8000)
		{
			text.remove(pos);
			pos--;
			size--;
		}
		else if (text[pos] == 164)
		{
			text.remove(pos);
			pos--;
			size--;
		}
		pos++;
	}
	return text;
}

bool Exists(CString str, CString what)
{
	bool result = false;
	size_t equal;
	size_t pos_str = 0, pos_what = 0, size_str = str.size(), what_size = what.size();
	for (pos_str; pos_str < size_str; pos_str++)
	{
		if (str[pos_str] == what[pos_what])
		{
			equal++;
			pos_what++;
		}
		else
		{
			equal = 0;
			pos_what = 0;
		}
		if (equal == what_size)
		{
			result = true;
			break;
		}
	}
	return result;
}

bool Exists(CWString str, CWString what)
{
	bool result = false;
	size_t equal;
	size_t pos_str = 0, pos_what = 0, size_str = str.size(), what_size = what.size();
	for (pos_str; pos_str < size_str; pos_str++)
	{
		if (str[pos_str] == what[pos_what])
		{
			equal++;
			pos_what++;
		}
		else
		{
			equal = 0;
			pos_what = 0;
		}
		if (equal == what_size)
		{
			result = true;
			break;
		}
	}
	return result;
}

//��������� ����� � �������� bool ("true"/"false")
bool Boolean(CString str)
{
	for (register size_t i = 0; i < str.size(); i++)
	{
		if (str[i] == '\0')
		{
			str.remove(i);
		}
		str[i] = (char) tolower(str[i]);
	}
	if (str == "true" || str == "false")
		return (str == "true");
	else
		throw "ERROR_TEXT_BOOLEAN";
	return false;
}

//
bool Boolean(CWString wstr)
{
	return Boolean(ConvertText(wstr));
}


//������������ DWORD � String
CString DWTS(DWORD dword)
{
	register unsigned char txt[4];
	register DWORD temp[4];
	for (register unsigned int i = 0; i < 4; i++)
	{
		temp[3-i] = dword >> (8 * i);
		txt[3-i] = (unsigned char) temp[3-i];
	}
	CString str;
	str.insert((char *) txt, 0, 4);
	return str;
}

//������������ String � DWORD
DWORD STDW(CString str)
{
	DWORD dw = 0;
	register DWORD temp[4];
	for (register unsigned char i = 0; i < 4; i++)
	{
		temp[i] = 0x00000000 | str[i];
		temp[i] = temp[i] << (8 *3);
		temp[i] = temp[i] >> (8 *3);
		dw = dw | (temp[i] << (8 * (3 - i)));
	}
	return dw;
}


//������������ HWND � ������
CString DefTypeToString(HWND arg)
{
	CString str;
	DWORD temp = 0;
	temp = reinterpret_cast<DWORD> (arg);
	str = DWTS(temp);
	return str;
}


//������������ ������ � HWND
HWND StringToDefType(CString arg)
{
	HWND ret;
	DWORD temp = STDW(arg);
	ret = reinterpret_cast<HWND> (temp);
	return ret;
}

#include <sstream>

//������������ String � size_t
size_t STS(CString str)
{
	std::stringstream ss;
	size_t size;
	ss << str;
	ss >> size;
	return size;
}

//������������ size_t � String
CString STS(size_t size)
{
	CString data;
	std::stringstream ss;
	ss << size;
	ss >> data;
	return data;
}


