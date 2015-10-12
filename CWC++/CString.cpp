#include <Windows.h>
#include "Symbols.h"
#include "CString.h"

CString::CString() : str(NULL), _size(0)
{

}

CString::CString(LPVOID data, size_t _size)
	: str(0), _size(0)
{
	this->str = new char[_size + 1];
	char *temp = (char *) data;
	for (register size_t i = 0; i < _size; i++)
	{
		this->str[i] = temp[i];
	}
	this->str[_size] = '\0';
	this->_size = _size + 1;
}

CString::~CString()
{
	clear();
}

CString::CString(const char *str)
	: str(0), _size(0)
{
	*this = str;
}


CString CString::operator+(const char *str)
{
	CString temp = *this;
	temp += str;
	return temp;
}


CString CString::operator+(const CString &obj)
{
	CString temp = *this;
	temp += obj;
	return temp;
}

CString *CString::operator=(char *str)
{
	if (!str) return this;
	register size_t len = strlen(str) + 1;
	if (this->str)
	{
		delete [this->_size] this->str;
		this->str = new char[len];
	}
	else this->str = new char[len];
	for (register size_t i = 0; i < len - 1; i++)
		this->str[i] = str[i];
	this->str[len - 1] = '\0';
	this->_size = len;
	return this;
}

CString *CString::operator=(const char *str)
{
	if (!str) return this;
	register size_t len = strlen(str) + 1;
	if (this->str)
	{
		delete [this->_size] this->str;
		this->str = new char[len];
	}
	else this->str = new char[len];
	for (register size_t i = 0; i < len - 1; i++)
		this->str[i] = str[i];
	this->str[len - 1] = '\0';
	this->_size = len;
	return this;
}

CString *CString::operator+=(char *str)
{
	if (!str) return this;
	register size_t last_size = this->_size - 1;
	if (!this->str || !this->_size)
	{
		last_size = 0;
	}
	register size_t len = strlen(str);
	register size_t total_size = len + last_size + 1;
	if (total_size < len || total_size < this->_size) return this;
	char *new_str = new char[total_size];
	for (register size_t i = 0; i < last_size; i++)
	{
		new_str[i] = this->str[i];
	}
	if (this->str) delete [this->_size] this->str;
	for (register size_t i = 0; i < len; i++)
	{
		new_str[i+last_size] = str[i];
	}
	new_str[total_size-1] = '\0';
	this->str = new_str;
	this->_size = total_size;
	return this;
}

void CString::add(char *str, size_t len)
{
	if (!str) return;
	register size_t last_size = this->_size - 1;
	if (!this->str || !this->_size)
	{
		last_size = 0;
	}
	register size_t total_size = len + last_size + 1;
	if (total_size < len || total_size < this->_size) return;
	char *new_str = new char[total_size];
	for (register size_t i = 0; i < last_size; i++)
	{
		new_str[i] = this->str[i];
	}
	if (this->str) delete [this->_size] this->str;
	for (register size_t i = 0; i < len; i++)
	{
		new_str[i+last_size] = str[i];
	}
	new_str[total_size-1] = '\0';
	this->str = new_str;
	this->_size = total_size;
}

void CString::add(char *str)
{
	if (!str) return;
	register size_t last_size = this->_size - 1;
	if (!this->str || !this->_size)
	{
		last_size = 0;
	}
	register size_t len = strlen(str);
	register size_t total_size = len + last_size + 1;
	if (total_size < len || total_size < this->_size) return;
	char *new_str = new char[total_size];
	for (register size_t i = 0; i < last_size; i++)
	{
		new_str[i] = this->str[i];
	}
	if (this->str) delete [this->_size] this->str;
	for (register size_t i = 0; i < len; i++)
	{
		new_str[i+last_size] = str[i];
	}
	new_str[total_size-1] = '\0';
	this->str = new_str;
	this->_size = total_size;
}

CString *CString::operator+=(const char *str)
{
	if (!str) return this;
	register size_t last_size = this->_size - 1;
	if (!this->str || !this->_size)
	{
		last_size = 0;
	}
	register size_t len = strlen(str);
	register size_t total_size = len + last_size + 1;
	if (total_size < len || total_size < this->_size) return this;
	char *new_str = new char[total_size];
	for (register size_t i = 0; i < last_size; i++)
	{
		new_str[i] = this->str[i];
	}
	if (this->str) delete [this->_size] this->str;
	for (register size_t i = 0; i < len; i++)
	{
		new_str[i+last_size] = str[i];
	}
	new_str[total_size-1] = '\0';
	this->str = new_str;
	this->_size = total_size;
	return this;
}

CString *CString::operator+=(char symb)
{
	register size_t total_size = this->_size + 1;
	if (total_size < 1 || total_size < this->_size) return this;
	char *buff = NULL;
	if (this->str)
	{
		buff = new char[this->_size - 1];
		for (register size_t i = 0; i < this->_size - 1; i++)
			buff[i] = this->str[i];
		delete [this->_size] this->str;
		this->str = new char[total_size];
	}
	else this->str = new char[++total_size];
	if (buff)
	{
		for (register size_t i = 0; i < this->_size - 1; i++)
			this->str[i] = buff[i];
		delete [this->_size] buff;
	}
	this->str[total_size - 2] = symb;
	this->str[total_size - 1] = '\0';
	this->_size = total_size;
	return this;
}

CString *CString::operator+=(const CString &obj)
{
	if (!obj.str) return this;
	if (!this->_size || !this->str)
	{
		register size_t _size = obj._size;
		this->str = new char[_size];
		for (register size_t i = 0; i < _size; i++)
			this->str[i] = obj.str[i];
		this->_size = obj._size;
		return this;
	}
	register size_t len = obj._size - 1;
	register size_t _size = this->_size;
	register size_t total_size = len + _size;
	if (total_size < len || total_size < _size) return this;
	char *new_str = new char[total_size];
	for (register size_t i = 0; i < _size - 1; i++)
	{
		new_str[i] = this->str[i];
	}
	if (this->str) delete [this->_size] this->str;
	for (register size_t i = 0; i < len; i++)
	{
		new_str[i+_size-1] = obj.str[i];
	}
	new_str[total_size-1] = '\0';
	this->str = new_str;
	this->_size = total_size;
	return this;
}

CString *CString::operator=(const CString &obj)
{
	if (!obj.str)
	{
		this->clear();
		return this;
	}
	register size_t new_size = obj._size;
	if (this->str)
	{
		delete [this->_size] this->str;
	}
	this->str = new char[new_size];
	for (register size_t i = 0; i < new_size; i++)
		this->str[i] = obj.str[i];
	this->_size = obj._size;
	return this;
}


bool CString::operator==(const CString &obj) const
{
	if (this->_size != obj._size) return false;
	else
	{
		register size_t wrong = 0;
		for (register size_t _pos = 0; _pos < this->_size; _pos++)
		{
			if (this->str[_pos] != obj.str[_pos])
				return false;
		}
	}
	return true;
}


bool CString::operator==(const char *ch) const
{
	if (!ch)
		return false;
	size_t len = strlen(ch);
	if (!len)
	{
		if (ch[0] == '\0' && !_size)
			return true;
	}
	if (this->_size - 1 != len) return false;
	for (register size_t i = 0; i < len; i++)
	{
		if (this->str[i] == ch[i])
			continue;
		else
			return false;
	}
	return true;
}

char CString::operator[](size_t i) const
{
	return this->str[i];
}


char &CString::operator[](size_t i)
{
	return this->str[i];
}


char *CString::data() const
{
	return this->str;
}

void CString::insert(char *str, size_t from)
{
	if (!_size || !this->str || !str) return;
	register size_t len = strlen(str);
	insert(str, from, len);
}

void CString::insert(char *str, size_t from, size_t _size)
{
	register size_t total_size = this->_size + _size + (this->empty() ? 1 : 0);
	if (from > this->_size - 1) return;
	char *new_str = new char[total_size];
	register size_t pos = 0;
	for (pos; pos < from; pos++)
	{
		new_str[pos] = this->str[pos];
	}
	register size_t min = pos;
	for (pos; pos < _size && pos < total_size - 1; pos++)
	{
		new_str[pos] = str[pos-min];
	}
	for (pos; pos < total_size - 1; pos++)
	{
		new_str[pos] = this->str[pos - _size];
	}
	delete [this->_size] this->str;
	this->str = new_str;
	if (this->empty())
		this->str[total_size - 1] = '\0';
	this->_size = total_size;
}

void CString::insert(CString &str, size_t from)
{
	register size_t _size = this->_size;
	if (!_size)
	{
		*this = str;
		return;
	}
	register size_t size_str = str._size - 1;
	if (!size_str) return;
	register size_t total_size = _size + size_str;
	register size_t rfrom = from;
	char *new_data = new char[total_size];
	register size_t last = 0;
	register size_t pick = 0;
	register size_t now = 0;
	for (now; now < from && now < size_str && now < _size; now++)
	{
		new_data[now] = this->str[now];
		last++;
	}
	for (register size_t snow = 0; snow < size_str; snow++)
	{
		new_data[now] = str.str[snow];
		now++;
		pick++;
	}
	for (now; now < total_size - 1; now++)
	{
		new_data[now] = this->str[now - pick];
	}
	new_data[total_size-1] = '\0';
	delete [_size] this->str;
	this->str = new_data;
	this->_size = total_size;
}

void CString::remove(size_t i)
{
	if (!str || !_size) return;
	if (i > _size) return;
	register size_t total_size = this->_size - 1;
	char *buff = NULL;
	if (this->str)
	{
		buff = new char[this->_size - 1];
		for (register size_t i = 0; i < this->_size - 1; i++)
			buff[i] = this->str[i];
		delete [this->_size] this->str;
		this->str = new char[total_size];
	}
	if (buff)
	{
		for (size_t _i = 0; _i < total_size - 1; _i++)
			this->str[_i] = buff[_i < i ? _i : _i + 1];
		delete [this->_size - 1] buff;
		this->str[total_size - 1] = '\0';
	}
	this->_size = total_size;
}

void CString::remove(size_t from, size_t num, bool is_num_end)
{
	if (!str || !_size) return;
	size_t to;
	if (is_num_end)
		to = num;
	else
		to = from + num;
	if (to < from) return;
	register size_t len = to - from;
	register size_t total_size = this->_size - len;
	register size_t _size = this->_size;
	char *buff = NULL;
	if (this->str)
	{
		buff = new char[total_size];
		for (register size_t i = 0; i < from; i++)
			buff[i] = this->str[i];
		for (register size_t i = to; i < _size; i++)
			buff[i-len] = this->str[i];
		delete [this->_size] this->str;
		this->str = new char[total_size];
	}
	if (buff)
	{
		for (register size_t i = 0; i < total_size; i++)
			this->str[i] = buff[i];
		delete [total_size] buff;
	}
	this->_size = total_size;
}

CString CString::extr(size_t from, size_t to) const
{
	if (from == to && from == 0)
		return *this;
	register size_t _size = to - from;
	if (to == 0)
		to = this->_size;
	else
		if (_size > this->_size)
			return CString();
	register size_t _from = from;
	register size_t _to = to;
	char *temp = new char[_size];
	register size_t _temp_pos = 0;
	for (register size_t _pos = from; _pos < _to; _pos++)
	{
		temp[_temp_pos] = (*this)[_pos];
		_temp_pos++;
	}
	CString temp_str;
	temp_str.add(temp, _size);
	delete [_size] temp;
	return temp_str;
}

bool CString::find(const CString what, size_t *pos) const
{
	if (what.size() > this->_size)
		return false;
	else if (what.size() == this->_size)
		return what == *this;
	else
	{
		char *temp = new char[what.size()];
		register size_t _now_pos; //позиция, где остановились сравнивать
		for (_now_pos = 0; _now_pos < this->_size - what.size(); _now_pos++)
		{
			//Заполняем массив
			for (register size_t temp_pos = 0; temp_pos < what.size(); temp_pos++)
			{
				temp[temp_pos] = this->str[temp_pos + _now_pos];
			}
			//сравниваем
			if (what == CString(temp, what.size()))
			{
				if (pos)
					*pos = _now_pos;
				delete [what.size()] temp;
				return true;
			}
		}
		delete [what.size()] temp;
	}
	return false;
}

void CString::clear()
{
	if (str) delete [this->_size] this->str;
	str = NULL;
	_size = 0;
}


CString operator+(const char *str, const CString obj)
{
	CString temp;
	temp += str;
	temp += obj;
	return temp;
}

std::ostream &operator<<(std::ostream &stream, CString &obj)
{
	for (register size_t _pos = 0; _pos < obj._size; _pos++)
	{
		stream << obj.str[_pos];
	}
	return stream;
}

std::istream &operator>>(std::istream &stream, CString &obj)
{
	while (!stream.eof())
	{
		char c;
		stream.get(c);
		obj.add(&c, 1);
	}
	obj.remove(obj._size - 1);
	return stream;
}