#pragma once
#include <Windows.h>
#include <iostream>


class CWString {
	wchar_t *str;
	size_t _size;
public:
	explicit CWString();
	explicit CWString(LPVOID data, size_t size);
	CWString(const wchar_t *);
	__forceinline CWString(const CWString &old) : str(0), _size(0)
	{
		if (old._size)
			this->operator=(old);
	}
	~CWString();
	/**/
	CWString operator+(const wchar_t *);
	CWString operator+(const CWString &);
	/**/
	CWString *operator=(wchar_t *);
	CWString *operator=(const wchar_t *);
	CWString *operator+=(wchar_t *);
	CWString *operator+=(const wchar_t *);
	CWString *operator+=(wchar_t);
	/**/
	CWString *operator+=(const CWString &obj);
	CWString *operator=(const CWString &obj);
	/**/
	bool operator==(const CWString &obj) const;
	bool operator==(const wchar_t *ch) const;
	__forceinline bool operator!=(const wchar_t *ch)
	{
		return !(this->operator==(ch));
	}
	__forceinline bool operator!=(const CWString &obj) const
	{
		return !(this->operator==(obj));
	}
	__forceinline bool operator!() const
	{
		return this->empty();
	}
	/**/
	wchar_t &operator[](size_t);
	wchar_t operator[](size_t) const;
	/**/
	wchar_t *data() const ;
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
	void add(wchar_t *str);
	void add(wchar_t *str, size_t len);
	void insert(wchar_t *str, size_t from);
	void insert(wchar_t *str, size_t from, size_t size);
	void insert(CWString &str, size_t from);
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
	friend std::wostream &operator<<(std::wostream &stream, CWString &obj);
	friend std::wistream &operator>>(std::wistream &stream, CWString &obj);
	bool find(const CWString what, size_t *pos = 0) const;
	CWString extr(size_t from = 0, size_t to = 0) const;
};


CWString operator+(const wchar_t *, const CWString);

__forceinline CWString operator+(const wchar_t wch, const CWString obj)
{
	wchar_t str[2];
	str[0] = wch;
	str[1] = L'\0';
	return CWString(str) + obj;
}