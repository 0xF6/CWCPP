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
	if (symbol == L'а') return 'а';
	else if (symbol == L'б') return 'б';
	else if (symbol == L'в') return 'в';
	else if (symbol == L'г') return 'г';
	else if (symbol == L'д') return 'д';
	else if (symbol == L'е') return 'е';
	else if (symbol == L'ё') return 'ё';
	else if (symbol == L'ж') return 'ж';
	else if (symbol == L'з') return 'з';
	else if (symbol == L'и') return 'и';
	else if (symbol == L'й') return 'й';
	else if (symbol == L'к') return 'к';
	else if (symbol == L'л') return 'л';
	else if (symbol == L'м') return 'м';
	else if (symbol == L'н') return 'н';
	else if (symbol == L'о') return 'о';
	else if (symbol == L'п') return 'п';
	else if (symbol == L'р') return 'р';
	else if (symbol == L'с') return 'с';
	else if (symbol == L'т') return 'т';
	else if (symbol == L'у') return 'у';
	else if (symbol == L'ф') return 'ф';
	else if (symbol == L'х') return 'х';
	else if (symbol == L'ц') return 'ц';
	else if (symbol == L'ч') return 'ч';
	else if (symbol == L'ш') return 'ш';
	else if (symbol == L'щ') return 'щ';
	else if (symbol == L'ъ') return 'ъ';
	else if (symbol == L'ы') return 'ы';
	else if (symbol == L'ь') return 'ь';
	else if (symbol == L'э') return 'э';
	else if (symbol == L'ю') return 'ю';
	else if (symbol == L'я') return 'я';
	else if (symbol == L'А') return 'А';
	else if (symbol == L'Б') return 'Б';
	else if (symbol == L'В') return 'В';
	else if (symbol == L'Г') return 'Г';
	else if (symbol == L'Д') return 'Д';
	else if (symbol == L'Е') return 'Е';
	else if (symbol == L'Ё') return 'Ё';
	else if (symbol == L'Ж') return 'Ж';
	else if (symbol == L'З') return 'З';
	else if (symbol == L'И') return 'И';
	else if (symbol == L'Й') return 'Й';
	else if (symbol == L'К') return 'К';
	else if (symbol == L'Л') return 'Л';
	else if (symbol == L'М') return 'М';
	else if (symbol == L'Н') return 'Н';
	else if (symbol == L'О') return 'О';
	else if (symbol == L'П') return 'П';
	else if (symbol == L'Р') return 'Р';
	else if (symbol == L'С') return 'С';
	else if (symbol == L'Т') return 'Т';
	else if (symbol == L'У') return 'У';
	else if (symbol == L'Ф') return 'Ф';
	else if (symbol == L'Х') return 'Х';
	else if (symbol == L'Ц') return 'Ц';
	else if (symbol == L'Ч') return 'Ч';
	else if (symbol == L'Ш') return 'Ш';
	else if (symbol == L'Щ') return 'Щ';
	else if (symbol == L'Ъ') return 'Ъ';
	else if (symbol == L'Ы') return 'Ы';
	else if (symbol == L'Ь') return 'Ь';
	else if (symbol == L'Э') return 'Э';
	else if (symbol == L'Ю') return 'Ю';
	else if (symbol == L'Я') return 'Я';
	else
	{
		return (char) symbol;
	}
}

inline wchar_t ConvertSymbol(char symbol)
{
	if (symbol == 'а') return L'а';
	else if (symbol == 'б') return L'б';
	else if (symbol == 'в') return L'в';
	else if (symbol == 'г') return L'г';
	else if (symbol == 'д') return L'д';
	else if (symbol == 'е') return L'е';
	else if (symbol == 'ё') return L'ё';
	else if (symbol == 'ж') return L'ж';
	else if (symbol == 'з') return L'з';
	else if (symbol == 'и') return L'и';
	else if (symbol == 'й') return L'й';
	else if (symbol == 'к') return L'к';
	else if (symbol == 'л') return L'л';
	else if (symbol == 'м') return L'м';
	else if (symbol == 'н') return L'н';
	else if (symbol == 'о') return L'о';
	else if (symbol == 'п') return L'п';
	else if (symbol == 'р') return L'р';
	else if (symbol == 'с') return L'с';
	else if (symbol == 'т') return L'т';
	else if (symbol == 'у') return L'у';
	else if (symbol == 'ф') return L'ф';
	else if (symbol == 'х') return L'х';
	else if (symbol == 'ц') return L'ц';
	else if (symbol == 'ч') return L'ч';
	else if (symbol == 'ш') return L'ш';
	else if (symbol == 'щ') return L'щ';
	else if (symbol == 'ъ') return L'ъ';
	else if (symbol == 'ы') return L'ы';
	else if (symbol == 'ь') return L'ь';
	else if (symbol == 'э') return L'э';
	else if (symbol == 'ю') return L'ю';
	else if (symbol == 'я') return L'я';
	else if (symbol == 'А') return L'А';
	else if (symbol == 'Б') return L'Б';
	else if (symbol == 'В') return L'В';
	else if (symbol == 'Г') return L'Г';
	else if (symbol == 'Д') return L'Д';
	else if (symbol == 'Е') return L'Е';
	else if (symbol == 'Ё') return L'Ё';
	else if (symbol == 'Ж') return L'Ж';
	else if (symbol == 'З') return L'З';
	else if (symbol == 'И') return L'И';
	else if (symbol == 'Й') return L'Й';
	else if (symbol == 'К') return L'К';
	else if (symbol == 'Л') return L'Л';
	else if (symbol == 'М') return L'М';
	else if (symbol == 'Н') return L'Н';
	else if (symbol == 'О') return L'О';
	else if (symbol == 'П') return L'П';
	else if (symbol == 'Р') return L'Р';
	else if (symbol == 'С') return L'С';
	else if (symbol == 'Т') return L'Т';
	else if (symbol == 'У') return L'У';
	else if (symbol == 'Ф') return L'Ф';
	else if (symbol == 'Х') return L'Х';
	else if (symbol == 'Ц') return L'Ц';
	else if (symbol == 'Ч') return L'Ч';
	else if (symbol == 'Ш') return L'Ш';
	else if (symbol == 'Щ') return L'Щ';
	else if (symbol == 'Ъ') return L'Ъ';
	else if (symbol == 'Ы') return L'Ы';
	else if (symbol == 'Ь') return L'Ь';
	else if (symbol == 'Э') return L'Э';
	else if (symbol == 'Ю') return L'Ю';
	else if (symbol == 'Я') return L'Я';
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

//Переводит текст в значение bool ("true"/"false")
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


//Конвертирует DWORD в String
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

//Конвертирует String в DWORD
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


//Конвертирует HWND в строку
CString DefTypeToString(HWND arg)
{
	CString str;
	DWORD temp = 0;
	temp = reinterpret_cast<DWORD> (arg);
	str = DWTS(temp);
	return str;
}


//Конвертирует строку в HWND
HWND StringToDefType(CString arg)
{
	HWND ret;
	DWORD temp = STDW(arg);
	ret = reinterpret_cast<HWND> (temp);
	return ret;
}

#include <sstream>

//Конвертирует String в size_t
size_t STS(CString str)
{
	std::stringstream ss;
	size_t size;
	ss << str;
	ss >> size;
	return size;
}

//Конвертирует size_t в String
CString STS(size_t size)
{
	CString data;
	std::stringstream ss;
	ss << size;
	ss >> data;
	return data;
}


