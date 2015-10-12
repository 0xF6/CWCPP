#pragma once

#include "CString.h"
#include "CWString.h"

#pragma region File

class FileStream;

class File {
	friend FileStream;
	LPVOID Data;
	LARGE_INTEGER size_L;
	DWORD size_D;
	bool large;
	bool null;
	bool string;
public:
	File(CString &str);
	File(LPVOID Data, LARGE_INTEGER size);
	File(LPVOID Data, DWORD size);
	File(unsigned int null);
	File(File &f);
	File(const FileStream &fs);
	~File();
	LPVOID GetData();
	DWORD GetSize_D();
	LARGE_INTEGER GetSize_L();
	void GetStringData(CString *str);
	bool IsLarge();
	void Release();
	bool operator!();
	File *operator=(File &obj);
	void Load(LPVOID Data, LARGE_INTEGER size);
	void Load(LPVOID Data, DWORD size);
};

#pragma endregion

#pragma region FileStream

enum StreamMode {
	FSM_NULL, FSM_READ, FSM_WRITE, FSM_ALL
};


class FileStream {
	friend class WindAPI;
	HANDLE file;
	size_t _pos;
	size_t _size;
	StreamMode mode;
	bool do_not_close;
	FileStream *new_stream;
	FileStream *copy() const; 
public:
	FileStream();
	FileStream(const FileStream &stream);
	__forceinline ~FileStream()
	{
		Close();
	}
	__forceinline void SetPos(size_t pos)
	{
		this->_pos = pos;
	}
	__forceinline size_t GetPos()
	{
		return _pos;
	}
	__forceinline size_t GetSize()
	{
		return _size;
	}
	void WriteData(const CString &data, bool wait = false);
	void WriteData(const char *data, size_t data_size, bool wait = false);
	CString ReadData(size_t size = 0);
	CString ReadLine();
	void put(char sym);
	char get();
	__forceinline void Close()
	{
		if (!do_not_close)
		{
			CloseHandle(file);
			file = NULL;
		}
	}
	__forceinline operator File() const
	{
		return this->operator=(*this);
	}
	File operator=(const FileStream &s) const;
	__forceinline bool operator!()
	{
		return (!file);
	}
	__forceinline bool end()
	{
		return (this->_size - 1 <= this->_pos);
	}
};



#pragma endregion