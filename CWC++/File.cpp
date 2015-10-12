#include <Windows.h>
#include "CString.h"
#include "File.h"

#pragma region File

File::File(CString &str) 
	: Data(NULL), large(false), null(false), string(false), size_D(0), size_L(LARGE_INTEGER())
{
	register size_t size = str.len();
	this->size_D  = (DWORD) size;
	this->size_L.LowPart = (DWORD) size;
	char *temp = new char[size];
	for (register size_t i = 0; i < size; i++)
	{
		temp[i] = str[i];
	}
	this->Data = (void *) temp;
	this->string = true;
	this->large = false;
	this->null = false;
}

File::File(unsigned int null) //если функция вернёт NULL, значит файл равен нулю
	: Data(NULL), large(false), null(false), string(false), size_D(0), size_L(LARGE_INTEGER())
{
	if (!null)
	{
		this->null = true;
		this->size_D = NULL;
		this->size_L.QuadPart = NULL;
		this->size_L.LowPart = NULL;
		this->size_L.HighPart = NULL;
		this->large = false;
		this->string = false;
		this->Data = NULL;
	}
}

File::File(LPVOID Data, LARGE_INTEGER size)
	: Data(NULL), large(false), null(false), string(false), size_D(0), size_L(LARGE_INTEGER())
{
	this->Load(Data, size);
}

File::File(LPVOID Data, DWORD size) 
	: Data(NULL), large(false), null(false), string(false), size_D(0), size_L(LARGE_INTEGER())
{
	this->Load(Data, size);
}

File::~File()
{
	this->Release();
}

File::File(File &f)
	: Data(NULL), large(false), null(false), string(false), size_D(0), size_L(LARGE_INTEGER())
{
	this->Data = (void *) new char[f.large ? f.size_L.LowPart : f.size_D];
	this->large = f.large;
	this->string = f.string;
	this->size_D = f.size_D;
	this->size_L = f.size_L;
	memcpy(this->Data, f.Data, f.large ? f.size_L.LowPart : f.size_D);
}

File::File(const FileStream &fs)
{
	ZeroMemory(this, sizeof(File));
	*this = fs.operator=(fs);
}

LPVOID File::GetData()
{
	return this->Data;
}

bool File::IsLarge()
{
	return this->large;
}

DWORD File::GetSize_D()
{
	return this->size_D;
}

LARGE_INTEGER File::GetSize_L()
{
	return this->size_L;
}

void File::Release()
{
	if (Data)
	{
		delete [this->size_D] this->Data;
		Data = NULL;
	}
	this->null = true;
}

bool File::operator!()
{
	if (this->null) return true;
	return false;
}

File *File::operator=(File &obj)
{
	this->Release();
	if (obj.null)
	{
		return this;
	}
	register size_t size = (size_t) obj.large ? obj.size_L.LowPart : obj.size_D;
	char *data = (char *) obj.Data;
	char *temp = new char[size];
	for (register size_t i = 0; i <size; i++)
	{
		temp[i] = data[i];
	}
	this->string = obj.string;
	this->size_D = obj.size_D;
	this->size_L = obj.size_L;
	this->large = obj.large;
	this->Data = (void *) temp;
	this->null = false;
	return this;
}

void File::GetStringData(CString *str)
{
	if (!str) return;
	str->clear();
	str->add((char *) this->Data, 
		(size_t) (this->large ? this->size_L.LowPart : this->size_D) );
}


void File::Load(LPVOID Data, LARGE_INTEGER size)
{
	this->Release();
	this->large = true;
	this->Data = Data;
	this->size_L = size;
	this->size_D = size.LowPart;
	this->string = false;
	if (this->Data == NULL) this->null = true;
	else this->null = false;
}

void File::Load(LPVOID Data, DWORD size)
{
	this->Release();
	this->large = false;
	this->Data = Data;
	this->size_D = size;
	this->string = false;
	if (this->Data == NULL) this->null = true;
	else this->null = false;
}


#pragma endregion

#pragma region FileStream

#pragma region Common
FileStream::FileStream()
{
	ZeroMemory(this, sizeof(FileStream));
}

FileStream::FileStream(const FileStream &fs)
{
	ZeroMemory(this, sizeof(FileStream));
	if (fs.do_not_close)
	{
		FileStream *copy_fs = fs.copy();
		if (copy_fs)
			*this = *copy_fs;
		return;
	}
	FileStream *old = &const_cast<FileStream &> (fs);
	old->do_not_close = true;
	old->new_stream = this;
	this->file = fs.file;
	this->_size = fs._size;
	this->_pos = fs._pos;
	this->mode = fs.mode;
	const_cast<const FileStream &> (fs);
}

FileStream *FileStream::copy() const
{
	if (this->do_not_close)
	{
		if (this->new_stream)
		{
			return this->new_stream->copy();
		}
		else
		{
			return NULL;
		}
	}
	else
	{
		return (FileStream *) this;
	}
}
#pragma endregion

#pragma region Operators

File FileStream::operator=(const FileStream &s) const
{
	if (s.mode != StreamMode::FSM_READ &&
		s.mode != StreamMode::FSM_ALL)
		return NULL;
	DWORD file_size = 0;
	file_size = GetFileSize(s.file, NULL);
	OVERLAPPED overlapped;
	ZeroMemory(&overlapped, sizeof(OVERLAPPED));
	overlapped.hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	LPVOID data = malloc(file_size);
	ReadFile(s.file, data, file_size, NULL, &overlapped);
	DWORD num_bytes_read;
	GetOverlappedResult(s.file, &overlapped, &num_bytes_read, true);
	return File(data, file_size);
}

#pragma endregion

#pragma region Stream

void FileStream::WriteData(const CString &data, bool wait)
{
	DWORD _size_to_write = data.len();
	OVERLAPPED overlapped;
	ZeroMemory(&overlapped, sizeof(OVERLAPPED));
	overlapped.Offset = this->_pos;
	overlapped.hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	WriteFile(this->file, data.data(), _size_to_write, NULL, &overlapped);
	DWORD num_bytes_write;
	GetOverlappedResult(this->file, &overlapped, &num_bytes_write, wait);
	this->_size += _size_to_write;
	this->_pos += _size_to_write;
}

void FileStream::WriteData(const char *data, size_t data_size, bool wait)
{
	DWORD _size_to_write = data_size;
	OVERLAPPED overlapped;
	ZeroMemory(&overlapped, sizeof(OVERLAPPED));
	overlapped.Offset = this->_pos;
	overlapped.hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	WriteFile(this->file, (void *) data, _size_to_write, NULL, &overlapped);
	DWORD num_bytes_write;
	GetOverlappedResult(this->file, &overlapped, &num_bytes_write, wait);
	this->_size += _size_to_write;
	this->_pos += _size_to_write;
}


CString FileStream::ReadData(size_t count)
{
	if (!count)
		count = this->_size;
	OVERLAPPED overlapped;
	ZeroMemory(&overlapped, sizeof(OVERLAPPED));
	overlapped.Offset = this->_pos;
	overlapped.hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	LPVOID data = malloc(count);
	ReadFile(this->file, data, count, NULL, &overlapped);
	DWORD num_bytes_read;
	GetOverlappedResult(this->file, &overlapped, &num_bytes_read, true);
	CString temp;
	temp.insert((char *) data, 0, num_bytes_read);
	free(data);
	this->_pos += num_bytes_read;
	return temp;
}

CString FileStream::ReadLine()
{
	if (this->end())
		return CString();
	const size_t count = 1;
	OVERLAPPED overlapped;
	ZeroMemory(&overlapped, sizeof(OVERLAPPED));
	overlapped.hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	char sym = '\0'; //символ
	CString temp; //выходная строка
	do {
		overlapped.Offset = this->_pos;
		this->_pos++;
		ReadFile(this->file, &sym, count, NULL, &overlapped);
		DWORD num_bytes_read;
		GetOverlappedResult(this->file, &overlapped, &num_bytes_read, true);
		if (sym != '\0' && sym != '\n' && sym != '\r' && sym != '\0')
				temp += sym;
		else
			break;
		if (this->_pos >= this->_size)
			break;
	} while (true);
	return temp;
}

void FileStream::put(char s)
{
	DWORD _size_to_write = 1;
	OVERLAPPED overlapped;
	ZeroMemory(&overlapped, sizeof(OVERLAPPED));
	overlapped.Offset = this->_pos;
	overlapped.hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	WriteFile(this->file, (void *) &s, _size_to_write, NULL, &overlapped);
	DWORD num_bytes_write;
	GetOverlappedResult(this->file, &overlapped, &num_bytes_write, true);
	this->_size += _size_to_write;
	this->_pos += _size_to_write;
}

char FileStream::get()
{
	size_t count = 1;
	OVERLAPPED overlapped;
	ZeroMemory(&overlapped, sizeof(OVERLAPPED));
	overlapped.Offset = this->_pos;
	overlapped.hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	LPVOID data = malloc(count);
	ReadFile(this->file, data, count, NULL, &overlapped);
	char s = ((char *) data)[0];
	free(data);
	return s;
}

#pragma endregion

#pragma region Others



#pragma endregion

#pragma endregion