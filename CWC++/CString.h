#pragma once
#include <iostream>
#include <Windows.h>

class CString {
	char *str;
	size_t _size;
public:
	explicit CString();
	explicit CString(LPVOID data, size_t size);
	CString(const char *);
	__forceinline CString(const CString &old) : str(0), _size(0)
	{
		if (old._size)
			this->operator=(old);
	}
	~CString();
	/**/
	CString operator+(const char *);
	CString operator+(const CString &);
	/**/
	CString *operator=(char *);
	CString *operator=(const char *);
	CString *operator+=(char *);
	CString *operator+=(const char *);
	CString *operator+=(char);
	/**/
	CString *operator+=(const CString &obj);
	CString *operator=(const CString &obj);
	/**/
	bool operator==(const CString &obj) const;
	bool operator==(const char *ch) const ;
	__forceinline bool operator!=(const char *ch) const
	{
		return !(this->operator==(ch));
	}
	__forceinline bool operator!=(const CString &obj) const
	{
		return !(this->operator==(obj));
	}
	__forceinline bool operator!() const
	{
		return this->empty();
	}
	/**/
	char &operator[](size_t);
	char operator[](size_t) const;
	/**/
	char *data() const;
	__forceinline size_t len() const
	{
		if (_size > 1)
			return this->_size - 1;
		return 0;
	}
	__forceinline size_t size() const
	{
		if (_size > 1)
			return this->_size - 1;
		return 0;
	}
	__forceinline bool empty() const
	{
		return (_size > 1) ? false : true;
	}
	void add(char *str);
	void add(char *str, size_t len);
	void insert(char *str, size_t from);
	void insert(char *str, size_t from, size_t size);
	void insert(CString &str, size_t from);
	void remove(size_t i);
	void remove(size_t from, size_t num, bool is_num_end = false);
	__forceinline void erase(size_t i)
	{
		remove(i);
	}
	__forceinline void erase(size_t from, size_t num, bool is_num_end = false)
	{
		remove(from, num, is_num_end);
	}
	void clear();
	friend std::ostream &operator<<(std::ostream &stream, CString &obj);
	friend std::istream &operator>>(std::istream &stream, CString &obj);
	bool find(const CString what, size_t *pos = 0) const;
	CString extr(size_t from = 0, size_t to = 0) const;
};


CString operator+(const char *, const CString);

__forceinline CString operator+(const char ch, const CString obj)
{
	char str[2];
	str[0] = ch;
	str[1] = '\0';
	return CString(str) + obj;
}