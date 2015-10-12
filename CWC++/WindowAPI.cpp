#include <windows.h>
#include "Types.h"
#include "Keys.h"
#include "File.h"
#include "WindowAPI.h"
#include "MathLib.h"
#include "CArray.h"
#include "CMap.h"
#include "CQueue.h"
#include "CStack.h"
#include "CString.h"
#include "CWString.h"
#include <mmsystem.h>
#include <Windowsx.h>
#include <Shlobj.h>

#pragma region Constants & Defines & Prototypes

#define IDStart 1100
#pragma endregion

#pragma region Helpers

#pragma region Allocated Memory

AMem::AMem(void *pointer, size_t size)
{
	this->pointer = pointer;
	this->size = size;
}

void AMem::Release()
{
	if (this->pointer && this->size)
	{
		free(this->pointer);
	}
	this->pointer = NULL;
	this->size = 0;
}

#pragma endregion

#pragma endregion

#pragma region FileFilter

CWString FileFilter::MakeFilter()
{
	CWString filter;
	for (register size_t _pos = 0; _pos < this->filters.size(); _pos++)
	{
		filter += filters.get(_pos)->key;
		filter += L'\0';
		size_t size = filters.get(_pos)->object.size();
		for (size_t i = 0; i < size; i++)
		{
			if (i > 0) filter += L';';
			filter += *filters.get(_pos)->object[i];
		}
	}
	filter += L"\0\0";
	return filter;
}

void FileFilter::AddFilter(CWString name, CArray<CWString> types)
{
	Pair<CWString, CArray<CWString>> *p = this->filters.find(name);;
	if (p == NULL)
	{
		this->filters.add(name, types); 
	}
	else
	{
		p->object += types;
	}
}

void FileFilter::AddFilter(CWString name, CWString type)
{
	CArray<CWString> types;
	types.add(type);
	this->AddFilter(name, types);
}

#pragma endregion

#pragma region WinStructures

#pragma region MenuItem

MenuItem::~MenuItem()
{
	this->name.clear();
	this->text.clear();
}

#pragma endregion

#pragma region WindItem

WindItem::~WindItem()
{
	this->name.clear();
	this->text.clear();
}

#pragma endregion

#pragma region TreeView

TreeView::TreeViewNode *TreeView::TreeViewNode::Find(HTREEITEM item)
{
	if (item == this->item) return this;
	else
	{
		for (register size_t _pos = 0; _pos < nodes.size(); _pos++)
		{
			TreeView::TreeViewNode *node = nodes.get(_pos)->object.Find(item);
			if (node)
				return node;
		}
	}
	return NULL;
}

TreeView::TreeViewNode *TreeView::TreeViewNode::Find(CWString name)
{
	if (name == this->name);
	else
	{
		for (register size_t _pos = 0; _pos < nodes.size(); _pos++)
		{
			return nodes.get(_pos)->object.Find(name);
		}
	}
	return NULL;
}

TreeView::TreeViewNode *TreeView::TreeViewNode::FindLocal(CWString name)
{
	for (register size_t _pos = 0; _pos < nodes.size(); _pos++)
	{
		if (nodes.get(_pos)->object.name == name)
			return &nodes.get(_pos)->object;
	}
	return NULL;
}


TreeView::TreeViewNode *TreeView::TreeViewNode::GetItemByPoint(Vec2 Point)
{
	if (IsInRect(WindAPI::GetItemRect(*this->wnd, *this), Point)) return this;
	else
	{
		for (register size_t _pos = 0; _pos < nodes.size(); _pos++)
		{
			TreeViewNode *node = nodes.get(_pos)->object.GetItemByPoint(Point);
			if (node)
				if (!node->name.empty()) return node;
		}
	}
	return NULL;
}

TreeView::TreeViewNode::~TreeViewNode()
{
	this->nodes.clear();
}

void TreeView::TreeViewNode::UpdateText()
{
	TV_ITEM item;
	ZeroMemory(&item, sizeof(TV_ITEM));
	item.hItem = this->item;
	item.mask= TVIF_TEXT | TVIF_HANDLE;
	if (TreeView_GetItem(this->wnd->hWnd, &item) == TRUE)
	{
		item.cchTextMax = this->text.size();
		item.pszText = (wchar_t *) this->text.data();
		TreeView_SetItem(this->wnd->hWnd, &item);
	}
}

void TreeView::TreeViewNode::Open()
{
	TreeView_Expand(this->wnd->hWnd, this->item, TVE_EXPAND);
}

void TreeView::TreeViewNode::Clear()
{
	TreeView_Expand(this->wnd->hWnd, this->item, TVE_COLLAPSE | TVE_COLLAPSERESET);
	nodes.clear();
}


TreeView::TreeViewNode *TreeView::TreeViewWND::Find(HTREEITEM item)
{
	TreeView::TreeViewNode *node = NULL;
	for (register size_t _pos = 0; _pos < nodes.size(); _pos++)
	{
		node = nodes.get(_pos)->object.Find(item);
		if (node) break;
	}
	return node;
}

TreeView::TreeViewNode *TreeView::TreeViewWND::Find(CWString name)
{
	TreeView::TreeViewNode *node = NULL;
	for (register size_t _pos = 0; _pos < nodes.size(); _pos++)
	{
		node = nodes.get(_pos)->object.Find(name);
		if (node) break;
	}
	return node;
}

TreeView::TreeViewNode *TreeView::TreeViewWND::FindLocal(CWString name)
{
	TreeView::TreeViewNode *node = NULL;
	for (register size_t _pos = 0; _pos < nodes.size(); _pos++)
	{
		if (name == nodes.get(_pos)->object.name)
		{
			node = &nodes.get(_pos)->object;
			break;
		}
	}
	return node;
}

TreeView::TreeViewNode *TreeView::TreeViewWND::GetItemByPoint(Vec2 Point)
{
	for (register size_t _pos = 0; _pos < nodes.size(); _pos++)
	{
		TreeViewNode *node = nodes.get(_pos)->object.GetItemByPoint(Point);
		if (node)
			if (!node->name.empty()) return node;
	}
	return NULL;
}

BOOL TreeView::TreeViewWND::DestroyItem(TreeView::TreeViewNode &node)
{
	if (node.parent)
	{
		for (register size_t _pos = 0; _pos < nodes.size(); _pos++)
		{
			if (nodes.get(_pos)->object.item == node.item)
			{
				return TreeView_DeleteItem(this->hWnd, node.item);
				node.nodes.remove(_pos);
			}
		}
	}
	else
	{
		for (register size_t _pos = 0; _pos < nodes.size(); _pos++)
		{
			if (nodes.get(_pos)->object.item == node.item)
			{
				return TreeView_DeleteItem(this->hWnd, node.item);
				nodes.remove(_pos);
			}
		}
	}
	return FALSE;
}

void TreeView::TreeViewWND::ClearInItem(TreeView::TreeViewNode &node)
{
	node.Clear();
}

void TreeView::TreeViewWND::Clear()
{
	for (register size_t _pos = 0; _pos < nodes.size(); _pos++)
		this->DestroyItem(nodes.get(_pos)->object);
	this->nodes.clear();
}

TreeView::TreeViewWND::~TreeViewWND()
{
	this->nodes.clear();
}

#pragma endregion

#pragma region ListView

#pragma endregion

#pragma region ComboBox

int ComboBox::ComboBoxWND::Add(CWString text, CWString name)
{
	ComboBox::ComboBoxItem item;
	item.text = text;
	item.name = name;
	item.index = ComboBox_AddString(this->hWnd, text.data());
	if (item.index != CB_ERR && item.index != CB_ERRSPACE)
		this->items.push_back(item);
	return item.index;
}

int ComboBox::ComboBoxWND::GetSelId()
{
	return ComboBox_GetCurSel(this->hWnd);
}

CWString ComboBox::ComboBoxWND::GetSelString()
{
	int index = ComboBox_GetCurSel(this->hWnd);
	if (index == CB_ERR)
		return CWString();
	for (register size_t i = 0; i < this->items.size(); i++)
	{
		if (this->items[i]->index == index)
		{
			return this->items[i]->text;
		}
	}
	return CWString();
}

CWString ComboBox::ComboBoxWND::GetSelName()
{
	int index = ComboBox_GetCurSel(this->hWnd);
	if (index == CB_ERR)
		return CWString();
	for (register size_t i = 0; i < this->items.size(); i++)
	{
		if (this->items[i]->index == index)
		{
			return this->items[i]->name;
		}
	}
	return CWString();
}

void ComboBox::ComboBoxWND::SetSel(int index)
{
	ComboBox_SetCurSel(this->hWnd, index);
}


void ComboBox::ComboBoxWND::SetSel(CWString name)
{
	for (register size_t i = 0; i < this->items.size(); i++)
	{
		if (this->items[i]->name == name)
		{
			ComboBox_SetCurSel(this->hWnd, this->items[i]->index);
			break;
		}
	}
}

#pragma endregion

#pragma endregion

#pragma region Registers Array & Simple Functions

namespace {
	UINT Last = IDStart;
	CMap<UINT, HWND> Windows;
	CMap<UINT, HWND> elements;
	CMap<UINT, HMENU> menus;
	CMap<UINT, CString> codes;
	CArray<UINT> deleted_codes; //разрегестрированные коды
	
	__forceinline bool BaseRegisterCode(UINT code)
	{	
		if (deleted_codes.empty())
		{
			if (code > Last - 1)
			{
				Last++;
				return true;
			}
		}
		else
		{
			for (register size_t i = 0; i < deleted_codes.size(); i++)
			{
				if (*deleted_codes[i] == code)
				{
					deleted_codes.remove(i);
					return true;
				}
			}
		}
		return false;
	}

	__forceinline bool BaseUnregisterCode(UINT code)
	{
		bool deleted = false;
		if (Windows.exists(code))
		{
			Windows.removekey(code);
			deleted = true;
		}
		if (elements.exists(code))
		{
			elements.removekey(code);
			deleted = true;
		}
		if (menus.exists(code))
		{
			menus.removekey(code);
			deleted = true;
		}
		if (codes.exists(code))
		{
			codes.removekey(code);
			deleted = true;
		}
		if (deleted)
			deleted_codes.push_back(code);
		return deleted;
	}

	__forceinline void RegisterWindowS(UINT code, HWND hWnd)
	{
		if (BaseRegisterCode(code))
		{
			Windows.insert(code, hWnd);
		}
	}

	__forceinline void RegisterHWND(UINT code, HWND hWnd)
	{
		if (BaseRegisterCode(code))
		{
			elements.insert(code, hWnd);
		}
		else
		{
			if (codes.find(code) != NULL)
			{
				elements.insert(code, hWnd);
			}
		}
	}

	__forceinline void RegisterHMENU(UINT code, HMENU hMenu)
	{
		if (BaseRegisterCode(code))
		{
			menus.insert(code, hMenu);
		}
		else
		{
			if (codes.find(code) != NULL)
			{
				menus.insert(code, hMenu);
			}
		}
	}

	__forceinline void RegisterCodeS(UINT code, CString data)
	{
		if (BaseRegisterCode(code))
		{
			codes.insert(code, data);
		}
	}

	__forceinline UINT GetLast()
	{
		if (deleted_codes.empty())
			return Last;
		else
			return deleted_codes.back();
	}

	__forceinline HWND GetHWND(UINT code)
	{
		if (elements.exists(code))
			return elements[code]->object;
		return NULL;
	}
	__forceinline HMENU GetHMENU(UINT code)
	{
		if (menus.exists(code))
			return menus[code]->object;
		return NULL;
	}

	__forceinline CString GetCode(UINT code)
	{
		if (codes.exists(code))
			return codes[code]->object;
		return NULL;
	}

	__forceinline UINT _GetCodeByElement(HWND hWnd)
	{
		for (register size_t _pos = 0; _pos < elements.size(); _pos++)
		{
			if (elements.get(_pos)->object == hWnd)
			{
				return elements.get(_pos)->key;
			}
		}
		for (register size_t _pos = 0; _pos < Windows.size(); _pos++)
		{
			if (Windows.get(_pos)->object == hWnd)
			{
				return Windows.get(_pos)->key;
			}
		}
		return 0;
	}
}

#pragma endregion

///////////////////////////////////////////////////////////////////////////////////////////////////

#pragma region Constants

namespace {

	const CWString StartFolder = WindAPI::GetWorkingFolder();

}

#pragma endregion

///////////////////////////////////////////////////////////////////////////////////////////////////

#pragma region WindAPI Common


WindAPI::WindAPI()
{
	GlobalDC = GetDC(0);
	ScreenSize.X = GetSystemMetrics(0);
	ScreenSize.Y = GetSystemMetrics(1);
	Keys::InitKeys();
	hfDefault = GetStockObject(DEFAULT_GUI_FONT); //загрузка стандартного шрифта
	InitCommonControls(); //Убеждаемся в хорошей загрузку DLL
}

WindAPI::~WindAPI()
{
	if (!elements.empty()) elements.clear();
	if (!menus.empty()) menus.clear();
}

void WindAPI::SendName(CWString name)
{
	this->name = name;
}

void WindAPI::Update()
{
	UpdateKeys();
}

#pragma endregion

///////////////////////////////////////////////////////////////////////////////////////////////////

#pragma region Windows API Common


HWND WindAPI::CreateNewWindowSimple(CString type, CWString CustomName, HWND Parent)
{
	return this->CreateNewWindow(type, GetModuleHandle(0), CustomName, Parent);
}

HWND WindAPI::CreateNewWindow(CString type, HINSTANCE hInstance, CWString CustomName, HWND Parent)
{
	CWString name = CustomName.empty() ? this->name : CustomName;
	if (type == "Std")
	{
		int x, y, sx, sy;
		sx = 1024;
		sy = 512;
		x = ( (int) ScreenSize.X / 2) - (sx / 2);
		y = ( (int) ScreenSize.Y / 2) - (sy / 2);
		return CreateWindow( L"MainClass", 
							 name.data(),
							 /*WS_POPUP |*/ WS_SYSMENU | WS_VISIBLE | WS_OVERLAPPEDWINDOW,
							 x, y,
							 sx, sy, Parent, NULL, hInstance, NULL );
	}
	else if (type == "Editor")
	{
		int x, y, sx, sy;
		x = 0;
		y = 0;
		sx = (int) ScreenSize.X;
		sy = (int) ScreenSize.Y;
		return CreateWindow( L"MainClassEditor", 
                             name.data(),
							 WS_OVERLAPPEDWINDOW | WS_MAXIMIZE | WS_CAPTION,
							 x, y,
							 sx, sy, Parent, NULL, hInstance, NULL );
	}
	else if (type == "EditorContentWindow")
	{
		int x, y, sx, sy;
		x = 0;
		y = 0;
		sx = (int) ScreenSize.X;
		sy = (int) ScreenSize.Y;
		sx = (sx / 2) + (sx / 4);
		sy = sy - (sy / 6);
		return CreateWindow( L"MainClassEditor", 
                             name.data(),
							 WS_OVERLAPPEDWINDOW | WS_MAXIMIZE | WS_CAPTION,
							 x, y,
							 sx, sy, Parent, NULL, hInstance, NULL );
	}
	else if (type == "PB")
	{

		int x, y, sx, sy;
		sx = 400;
		sy = 200;
		x = ( (int) ScreenSize.X / 2) - (sx / 2);
		y = ( (int) ScreenSize.Y / 2) - (sy / 2);
		return CreateWindow( L"MainClassEditor", 
                             L"Введите пароль",
							 WS_CAPTION,
							 x, y,
							 sx, sy, Parent, NULL, hInstance, NULL );
	}
	else if (type == "Updater")
	{

		int x, y, sx, sy;
		sx = 400;
		sy = 100;
		x = ( (int) ScreenSize.X / 2) - (sx / 2);
		y = ( (int) ScreenSize.Y / 2) - (sy / 2);
		return CreateWindow( L"MainClassUpdater", 
							 L"Идёт обновление",
							 WS_CAPTION,
							 x, y,
							 sx, sy, Parent, NULL, hInstance, NULL );
	}
	else if (type == "EditorImportWindow")
	{
		int x, y, sx, sy;
		sx = 600;
		sy = 380;
		x = ( (int) ScreenSize.X / 2) - (sx / 2);
		y = ( (int) ScreenSize.Y / 2) - (sy / 2);
		return CreateWindow( L"EditorImportWindowClass", 
							 name.data(),
							 WS_CAPTION,
							 x, y,
							 sx, sy, Parent, NULL, hInstance, NULL );
	}
	return NULL;
}


WNDCLASSEX WindAPI::CreateWindowClass(CString type, HINSTANCE hInstance, WNDPROC proc)
{
	WNDCLASSEX StdClass;
	ZeroMemory(&StdClass, sizeof(WNDCLASSEX));
	StdClass.hInstance = hInstance;
	StdClass.lpfnWndProc = proc;
	StdClass.cbSize = sizeof(WNDCLASSEX);
	StdClass.lpszMenuName  = NULL;
	StdClass.cbClsExtra    = 0;
	StdClass.cbWndExtra    = 0;
	if (type == "Std")
	{
		StdClass.lpszClassName = L"MainClass";
		StdClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
		StdClass.hCursor       = LoadCursor(NULL, IDC_ARROW);
		StdClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	}
	else if (type == "Launcher")
	{
		StdClass.lpszClassName = L"MainClass";
		StdClass.style         = CS_HREDRAW | CS_VREDRAW;
		StdClass.hIcon		   = SimpleLoadIcon(L"Aincrad.ico");
		StdClass.hIconSm	   = SimpleLoadIcon(L"Aincrad.ico");
		StdClass.hCursor       = LoadCursor(NULL, IDC_ARROW);
		StdClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	}
	else if (type == "Editor")
	{
		StdClass.lpszClassName = L"MainClassEditor";
		StdClass.style         = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
		StdClass.hIcon		   = SimpleLoadIcon(L"Aincrad.ico");
		StdClass.hIconSm	   = SimpleLoadIcon(L"Aincrad.ico");
		StdClass.hCursor       = LoadCursor(NULL, IDC_ARROW);
		StdClass.hbrBackground = (HBRUSH)(5);
	}
	else if (type == "Updater")
	{
		StdClass.lpszClassName = L"MainClassUpdater";
		StdClass.style         = CS_HREDRAW | CS_VREDRAW | CS_OWNDC | CS_DROPSHADOW;
		StdClass.hIcon		   = SimpleLoadIcon(L"Aincrad.ico");
		StdClass.hIconSm	   = SimpleLoadIcon(L"Aincrad.ico");
		StdClass.hCursor       = LoadCursor(NULL, IDC_ARROW);
		StdClass.hbrBackground = (HBRUSH)(5);
	}
	else if (type == "EditorImportWindow")
	{
		StdClass.lpszClassName = L"EditorImportWindowClass";
		StdClass.style         = CS_HREDRAW | CS_VREDRAW | CS_OWNDC | CS_DROPSHADOW;
		StdClass.hIcon		   = SimpleLoadIcon(L"Aincrad.ico");
		StdClass.hIconSm	   = SimpleLoadIcon(L"Aincrad.ico");
		StdClass.hCursor       = LoadCursor(NULL, IDC_ARROW);
		StdClass.hbrBackground = (HBRUSH)(5);
	}
	return StdClass;
}



HICON WindAPI::SimpleLoadIcon(CWString icon)
{
	return (HICON) LoadImage( // returns a HANDLE so we have to cast to HICON
		GetModuleHandle(0),             // hInstance must be NULL when loading from a file
		icon.data(),   // the icon file name
		IMAGE_ICON,       // specifies that the file is an icon
		0,                // width of the image (we'll specify default later on)
		0,                // height of the image
		LR_LOADFROMFILE|  // we want to load a file (as opposed to a resource)
		LR_DEFAULTSIZE|   // default metrics based on the type (IMAGE_ICON, 32x32)
		LR_SHARED         // let the system release the handle when it's no longer used
		);
}

#pragma endregion

///////////////////////////////////////////////////////////////////////////////////////////////////

#pragma region FilesAndFolders

CWString WindAPI::GetNativeFolder()
{
	CWString Folder = GetFullFileName();
	for (unsigned long max = (unsigned long) Folder.size(); max > 0; max--)
	{
		if (Folder[max] == '\\')
		{
			break;
		}
		else
		{
			Folder.erase(max);
		}
	}
	return Folder;
}

CWString WindAPI::GetWorkingFolder()
{
	wchar_t *folder = new wchar_t[1024];
	DWORD result = GetCurrentDirectory(1024, folder);
	CWString str_folder = folder;
	delete [] folder;
	for (size_t i = 0; i < str_folder.size(); i++)
	{
		if (i > result)
			str_folder.erase(i);
	}
	return str_folder + L"\\";
}

CWString WindAPI::GetStartFolder()
{
	return StartFolder;
}

CWString WindAPI::GetFullFileName()
{
	wchar_t buffer[5120];
	GetModuleFileName(NULL, buffer, 5120);
	return CWString(buffer);
}

/*******************************/
//Функция загрузки файла в память
//@filename - имя файла
//@direct - загружать ли файл, используя локальное местоположение
void WindAPI::LoadFile(CWString filename, File &file, bool direct)
{
	HANDLE hFile = NULL; 
	DWORD wmWritten = NULL;
	DWORD filesize = NULL;
	LPVOID out = NULL;
	LARGE_INTEGER largefilesize;
	bool large = false;
	ZeroMemory(&largefilesize, sizeof(LARGE_INTEGER));
	CWString path;
	if (direct)
	{
		path = GetStartFolder();
		path += filename;
	}
	else
	{
		path = filename;
	}
	hFile = CreateFile(path.data(),GENERIC_READ, //создаём файл
		FILE_SHARE_READ,NULL,OPEN_EXISTING,0,NULL);
	if (hFile == INVALID_HANDLE_VALUE) return;
	filesize = GetFileSize(hFile, NULL); //берём размер файла
	if (filesize == INVALID_FILE_SIZE) //если успешно, вернёт ноль
	{
		if (!GetFileSizeEx(hFile, &largefilesize)) 
		{
			CloseHandle(hFile);
			file = File(0);
			return;
		}
		large = true;
	}
	if (large)
	{
		out = malloc((size_t) largefilesize.LowPart);
		if (!ReadFileEx(hFile, out, largefilesize.LowPart, NULL, NULL)) //если успешно, вернёт ноль
		{
			free(out);
			CloseHandle(hFile);
			file = File(0);
			return;
		}
	}
	else
	{
		out = malloc((size_t) filesize); //выделяет память
		ZeroMemory(out, filesize); //очищает память от рандомных значений
		if (!ReadFile(hFile, out, filesize, &wmWritten, NULL)) //если успешно, вернёт ноль
		{
			free(out);
			CloseHandle(hFile);
			file = File(0);
			return; 
		}
	}
	CloseHandle(hFile);
	if (large)
	{
		file.Load(out, largefilesize);
	}
	else
	{
		file.Load(out, filesize);
	}
}

size_t WindAPI::FileSize(CWString filename, bool direct)
{
	HANDLE hFile = NULL; 
	DWORD filesize = NULL;
	LARGE_INTEGER largefilesize;
	bool large = false;
	ZeroMemory(&largefilesize, sizeof(LARGE_INTEGER));
	CWString path;
	if (direct)
	{
		path = GetStartFolder();
		path += filename;
	}
	else
	{
		path = filename;
	}
	hFile = CreateFile(path.data(),GENERIC_READ, //создаём файл
		FILE_SHARE_READ,NULL,OPEN_EXISTING,0,NULL);
	if (hFile == INVALID_HANDLE_VALUE) return 0;
	filesize = GetFileSize(hFile, NULL); //берём размер файла
	if (filesize == INVALID_FILE_SIZE) //если успешно, вернёт ноль
	{
		if (!GetFileSizeEx(hFile, &largefilesize)) 
		{
			CloseHandle(hFile);
			return 0;
		}
		large = true;
	}
	CloseHandle(hFile);
	if (large)
	{
		return (size_t) largefilesize.LowPart;
	}
	else
	{
		return (size_t) filesize;
	}
}


void WindAPI::SaveFile(CWString filename, LPVOID Data, DWORD size, bool direct)
{
	HANDLE hFile;
	DWORD wmWritten;
	CWString path;
	if (direct)
	{
		path = GetStartFolder();
		path += filename;
	}
	else
	{
		path = filename;
	}
	hFile = CreateFile(path.data(),GENERIC_WRITE, //создаём файл
		FILE_SHARE_READ,NULL,CREATE_ALWAYS,0,NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		CloseHandle(hFile);
		return;
	}
	WriteFile(hFile, Data, size, &wmWritten, NULL);
	CloseHandle(hFile);
}


void WindAPI::SaveFile(CWString filename, CString &data, bool direct)
{
	HANDLE hFile;
	DWORD wmWritten;
	CWString path;
	if (direct)
	{
		path = GetStartFolder();
		path += filename;
	}
	else
	{
		path = filename;
	}
	hFile = CreateFile(path.data(),GENERIC_WRITE, //создаём файл
		FILE_SHARE_READ,NULL,CREATE_ALWAYS,0,NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		CloseHandle(hFile);
		return;
	}
	WriteFile(hFile, data.data(), data.size(), &wmWritten, NULL);
	CloseHandle(hFile);
}


void WindAPI::SaveFile(CWString filename, File &file, bool direct)
{
	HANDLE hFile;
	DWORD wmWritten;
	CWString path;
	if (direct)
	{
		path = GetStartFolder();
		path += filename;
	}
	else
	{
		path = filename;
	}
	hFile = CreateFile(path.data(),GENERIC_WRITE, //создаём файл
		FILE_SHARE_READ,NULL,CREATE_ALWAYS,0,NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		CloseHandle(hFile);
		return;
	}
	WriteFile(hFile, file.GetData(), file.IsLarge() ? file.GetSize_L().LowPart :file.GetSize_D(), &wmWritten, NULL);
	CloseHandle(hFile);
}

void WindAPI::OpenStream(FileStream &stream, CWString filename, StreamMode mode, bool direct, bool create)
{
	if (stream.mode)
		return;
	stream.mode = mode;
	CWString path;
	if (direct)
	{
		path = GetStartFolder();
		path += filename;
	}
	else
	{
		path = filename;
	}
	HANDLE hFile;
	DWORD dwDesiredAccess = 0;
	DWORD dwShareMode = 0;
	switch (stream.mode)
	{
	case StreamMode::FSM_READ:
		dwDesiredAccess = GENERIC_READ;
		dwShareMode = FILE_SHARE_READ;
		break;
	case StreamMode::FSM_WRITE:
		dwDesiredAccess = GENERIC_WRITE;
		dwShareMode = FILE_SHARE_WRITE;
		break;
	case StreamMode::FSM_ALL:
		dwDesiredAccess = GENERIC_WRITE | GENERIC_READ;
		dwShareMode = FILE_SHARE_READ | FILE_SHARE_WRITE;
		break;
	default:
		break;
	}
	DWORD dwCreationDisposition = create ? 
		(this->ExistsFile(filename, direct) ?  CREATE_ALWAYS : CREATE_NEW)
		: OPEN_EXISTING;
	hFile = CreateFile(path.data(),dwDesiredAccess, //создаём файл
		dwShareMode,NULL,dwCreationDisposition,FILE_FLAG_OVERLAPPED,NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		CloseHandle(hFile);
		return;
	}
	stream._size = GetFileSize(hFile, NULL); //берём размер файла
	stream._pos = 0;
	stream.file = hFile;
}



bool WindAPI::ExistsFile(CWString filename, bool direct)
{
	CWString path;
	if (direct)
		path = GetStartFolder() + filename;
	else
		path = filename;
	HANDLE hFile;
	hFile = CreateFile(path.data(),GENERIC_READ, //создаём файл
		FILE_SHARE_READ,NULL,OPEN_EXISTING,0,NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		CloseHandle(hFile);
		return false;
	}
	CloseHandle(hFile);
	return true;
}

bool WindAPI::CreateFolder(CWString name, CWString path)
{
	if (path.empty())
	{
		path = GetStartFolder() + name;
	}
	else
	{
		path += name;
	}
#pragma warning(disable:4800)
	return CreateDirectory(path.data(), NULL);
}

bool WindAPI::ExistsFolder(CWString name, CWString path)
{
	if (path.empty())
	{
		path = GetStartFolder() + name;
	}
	else
	{
		path += L"\\" + name;
	}
	return !(FindFirstChangeNotification(path.data(), FALSE, FILE_NOTIFY_CHANGE_FILE_NAME)
		== INVALID_HANDLE_VALUE);
}

void WindAPI::DeleteFolder(CWString name, CWString path)
{
	if (path.empty())
	{
		path = GetStartFolder() + name;
	}
	else
	{
		path += L"\\" + name;
	}
	RemoveDirectory(path.data());
}


void WindAPI::ListFiles(CWString path, CArray<CWString> &files, bool direct)
{
	if (direct)
	{
		CWString temp = path;
		path = GetStartFolder();
		path += temp;
	}
	if (path[path.size() - 1] != L'\\')
	{
		path += L"\\";
	}
	path += L"*.*";
	WIN32_FIND_DATA data;
	HANDLE hFind;
	hFind = FindFirstFile(path.data(), &data);
	if (hFind != INVALID_HANDLE_VALUE)
	{
		do {
			if (CWString(data.cFileName) != L"." && CWString(data.cFileName) != L"..")
				files.push_back(data.cFileName);
		} while (FindNextFile(hFind, &data));
	}
	FindClose(hFind);
}

CWString WindAPI::GetFileName(CWString path)
{
	CWString name;
	CStack<wchar_t> temp;
	for (size_t i = path.size() - 1; i + 1 > 0; i--)
	{
		if (path[i] == L'/' || path[i] == L'\\' || path[i] == L'/') break;
		temp.push(path[i]);
	}
	while (!temp.empty())
	{
		name += *temp.top();
		temp.pop();
	}
	return name;
}

CWString WindAPI::GetFileType(CWString name)
{
	CWString type;
	CStack<wchar_t> temp;
	for (size_t i = name.size() - 1; i + 1> 0; i--)
	{
		if (name[i] == L'.')
		{
			break;
		}
		else
		{
			temp.push(name[i]);
		}
	}
	while (!temp.empty())
	{
		type += *temp.top();
		temp.pop();
	}
	return type;
}

#pragma endregion

///////////////////////////////////////////////////////////////////////////////////////////////////

#pragma region Files And Folders Dialogs

CWString WindAPI::OpenFileOpenDialog(CWString Directory, wchar_t *FileFilter)
{
	CWString FileName;
	wchar_t *file_temp = new wchar_t[1024];
	OPENFILENAME file;
	ZeroMemory(&file, sizeof(OPENFILENAME));
	ZeroMemory(file_temp, sizeof(wchar_t) * 1024);
	file.lStructSize = sizeof(OPENFILENAME);
	file.hInstance = GetModuleHandle(0);
	file.lpstrFilter = FileFilter;
	file.nMaxFile = 1024;
	file.lpstrFile = file_temp;
	file.lpstrTitle = L"Открыть файл";
	file.Flags = OFN_EXPLORER|OFN_FILEMUSTEXIST|OFN_PATHMUSTEXIST|OFN_NODEREFERENCELINKS;
	file.lpstrInitialDir = Directory.data();
	if (GetOpenFileName(&file)==TRUE)
	{
		FileName = file_temp;
	}
	else
	{
		DWORD er = CommDlgExtendedError();
		if (er)
		{
			printf_s("Something happens when I tryed to create OpenFileDialog: %u", er);
		}
	}
	delete [] file_temp;
	return FileName;
}

CWString WindAPI::OpenFileSaveDialog(CWString Directory, wchar_t *FileFilter)
{
	static CWString LastDirectory = Directory;
	CWString FileName;
	wchar_t *file_temp = new wchar_t[1024];
	OPENFILENAME file;
	ZeroMemory(&file, sizeof(OPENFILENAME));
	ZeroMemory(file_temp, sizeof(wchar_t) * 1024);
	file.lStructSize = sizeof(OPENFILENAME);
	file.hInstance = GetModuleHandle(0);
	file.lpstrFilter = FileFilter;
	file.nMaxFile = 1024;
	file.lpstrFile = file_temp;
	file.lpstrTitle = L"Сохранить файл";
	file.Flags = OFN_EXPLORER|OFN_PATHMUSTEXIST;
	file.lpstrInitialDir = Directory.data();
	if (GetSaveFileName(&file)==TRUE)
	{
		FileName = file_temp;
	}
	else
	{
		DWORD er = CommDlgExtendedError();
		if (er)
		{
			printf_s("Something happens when I tryed to create OpenFileDialog: %u", er);
		}
	}
	delete [] file_temp;
	return FileName;
}


CWString WindAPI::OpenFileOpenDialogC(CWString Directory, FileFilter filter)
{
	return OpenFileOpenDialog(Directory, (wchar_t *) filter.MakeFilter().data());
}


CWString WindAPI::OpenFileSaveDialogC(CWString Directory, FileFilter filter)
{
	return OpenFileSaveDialog(Directory, (wchar_t *) filter.MakeFilter().data());
}

CWString WindAPI::OpenFolderDialog(CWString Directory, CWString DialogName)
{
	CWString FolderName;
	wchar_t buff[1024];
	BROWSEINFO bInfo;
	bInfo.hwndOwner = NULL;
	bInfo.pidlRoot = NULL; 
	bInfo.pszDisplayName = buff;
	bInfo.lpszTitle = DialogName.data();
	bInfo.ulFlags = 0;
	bInfo.lpfn = NULL;
	bInfo.lParam = 0;
	bInfo.iImage = -1;

	LPITEMIDLIST lpItem = SHBrowseForFolder( &bInfo);
	if( lpItem != NULL )
	{
		SHGetPathFromIDList(lpItem, buff);
		FolderName = buff;
	}
	return FolderName;
}

#pragma endregion

///////////////////////////////////////////////////////////////////////////////////////////////////

#pragma region Keys

void WindAPI::UpdateKeys()
{
	for (int i = 0; i < (int) Keys::GetSize(); i++)
	{
		TryKey(i);
	}
}

#pragma warning(disable:4800)

void WindAPI::TryKey(int key_num)
{
	Key key = Keys::GetKey(key_num);
	key.active = ((unsigned short) GetAsyncKeyState(key.code)) >> 15;
	Keys::UpdateKey(key_num, key);
}

#pragma endregion

///////////////////////////////////////////////////////////////////////////////////////////////////

#pragma region Menus

BOOL WindAPI::CreateMenuItem(HMENU hMenu, CWString str, UINT uIns, UINT uCom, HMENU hSubMenu, BOOL flag, UINT fType)
{
	CWString temp;
	if (str[0] != L'&')
	{
		temp = L'&' + str;
	}
	else
	{
		temp = str;
	}
    MENUITEMINFO mii; 
    mii.cbSize = sizeof(MENUITEMINFO);
    mii.fMask = MIIM_STATE | MIIM_TYPE | MIIM_SUBMENU | MIIM_ID; 
    mii.fType = fType;
    mii.fState = MFS_ENABLED; 
	mii.dwTypeData = (wchar_t *) temp.data();
	mii.cch = temp.size();
    mii.wID = uCom;
    mii.hSubMenu = hSubMenu; 
    return InsertMenuItem(hMenu, uIns, flag, &mii);
}

void WindAPI::CreateMenu(HMENU hMenu, HMENU child, int id, UINT code, CWString text)
{
	text = L"&" + text;
	this->CreateMenuItem( hMenu, (wchar_t *) text.data(), id, code, child, FALSE, MFT_STRING);
}

BOOL WindAPI::GetMenuRect(HWND hwnd, HMENU hmenu, RECT *pmenurect)
{
    RECT rect;
    int  i;
    SetRect(pmenurect, 0, 0, 0, 0);
    for(i = 0; i < GetMenuItemCount(hmenu); i++)
    {
        GetMenuItemRect(hwnd, hmenu, i, &rect);
        UnionRect(pmenurect, pmenurect, &rect);
    }
    return TRUE;
}

BOOL WindAPI::CreateContextMenuItem(HMENU hMenu, UINT uFlags, UINT code, CWString text)
{
	return AppendMenu( hMenu, uFlags, code, 
		( text.empty() ? NULL :(L'&' + text).data() )
		);
}

#pragma endregion

///////////////////////////////////////////////////////////////////////////////////////////////////

#pragma region Simple WindAPI

void WindAPI::GetScreenSize(Vec2 &ScreenSize)
{
	ScreenSize.X = this->ScreenSize.X;
	ScreenSize.Y = this->ScreenSize.Y;
}

void WindAPI::GetWindowSize(HWND hWnd, Vec2 &WindowSize)
{
	RECT rect;
	GetWindowRect(hWnd, &rect);
	WindowSize.X = rect.right - rect.left;
	WindowSize.Y = rect.bottom - rect.top;
}

void WindAPI::SetDefaultFont(HWND hWnd)
{
	SendMessage(hWnd, WM_SETFONT, (WPARAM)hfDefault, MAKELPARAM(FALSE,0));
}

void WindAPI::SetWindowTop(HWND hWnd)
{
	if (!hWnd)
		return;
	ShowWindow(hWnd, SW_SHOW);
	SetForegroundWindow(hWnd);
}

void WindAPI::HideAllWindows()
{
	for (register size_t _pos = 0; _pos < Windows.size(); _pos++)
	{
		ShowWindow(Windows.get(_pos)->object, SW_HIDE);
	}
}

#pragma endregion

///////////////////////////////////////////////////////////////////////////////////////////////////

#pragma region Audio

BOOL WindAPI::PlayAudio(CWString file, bool async)
{
	return PlaySound(file.data(), NULL, SND_FILENAME | SND_NOWAIT | 
		SND_NOSTOP | (async ? SND_ASYNC : 0x0)
		| SND_NOSTOP);
}


BOOL WindAPI::PlayAudio(File &file, bool async)
{
	return PlaySound((LPCWSTR) file.GetData(),
		GetModuleHandle(0), SND_MEMORY | 
		SND_NOSTOP | SND_NOWAIT | (async ? SND_ASYNC : 0x0)
		| SND_NOSTOP);
}


BOOL WindAPI::PlayAudio(FileStream &stream, bool async)
{
	return this->PlayAudio((File) stream, async);
}


BOOL WindAPI::StopAudio()
{
	return PlaySound(NULL, 0, 0);
}

#pragma endregion

///////////////////////////////////////////////////////////////////////////////////////////////////

#pragma region WindAPI Registers

void WindAPI::RegisterWindow(UINT code, HWND hWnd)
{
	RegisterWindowS(code, hWnd);
}

void WindAPI::RegisterElement(UINT code, HWND hWnd)
{
	RegisterHWND(code, hWnd);
}

void WindAPI::RegisterMenu(UINT code, HMENU hMenu)
{
	RegisterHMENU(code, hMenu);
}

void WindAPI::RegisterCode(UINT code, CString name)
{
	RegisterCodeS(code, name);
}


void WindAPI::Unregister(UINT code)
{
	BaseUnregisterCode(code);
}

UINT WindAPI::GetCodeByElement(HWND hWnd)
{
	return _GetCodeByElement(hWnd);
}

UINT WindAPI::GetLastRegCode()
{
	return GetLast();
}

HWND WindAPI::GetElement(UINT code)
{
	return GetHWND(code);
}

HMENU WindAPI::GetMenu(UINT code)
{
	return GetHMENU(code);
}

CString WindAPI::GetRegisterByCode(UINT code)
{
	return GetCode(code);
}

#pragma endregion

///////////////////////////////////////////////////////////////////////////////////////////////////

#pragma region Common Controls

#pragma region Text

HWND WindAPI::CreateText(CWString text, UINT code, Vec2 Pos, Vec2 Size, HWND Parent)
{
	HWND hWnd;
	hWnd = CreateWindow(L"STATIC", text.data(),  WS_CHILD | WS_OVERLAPPED | WS_VISIBLE,
		(int) Pos.X, (int) Pos.Y, (int) Size.X, (int) Size.Y, Parent, (HMENU) code, GetModuleHandle(0), NULL);
	SendMessage(hWnd, WM_SETFONT, (WPARAM)hfDefault, MAKELPARAM(FALSE,0));
	return hWnd;
}


HWND WindAPI::CreateText(CWString text, UINT code, RECT rect, HWND Parent)
{
	Vec2 Pos, Size;
	MakeVectors(rect, Pos, Size);
	return CreateText(text, code, Pos, Size, Parent);
}

#pragma endregion

#pragma region Button

HWND WindAPI::CreateButton(CWString text, UINT code, RECT rect, HWND Parent)
{
	HWND button;
	button = 
		CreateWindow(L"BUTTON", text.data(), WS_CHILD | WS_VISIBLE, 
		rect.left, rect.top,
		rect.right - rect.left, rect.bottom - rect.top,
		Parent, (HMENU) code, GetModuleHandle(0), NULL);
	SendMessage(button, WM_SETFONT, (WPARAM)hfDefault, MAKELPARAM(FALSE,0));
	return button;
}

#pragma endregion

#pragma region Toolbars

HWND WindAPI::CreateHToolbar(HWND hWndParent, UINT code)
{
	HINSTANCE hInst = GetModuleHandle(0);
	HWND hWndToolbar = CreateWindowEx(WS_EX_TOOLWINDOW, TOOLBARCLASSNAME, NULL, 
                                      WS_CHILD | WS_VISIBLE | CCS_TOP | TBSTYLE_WRAPABLE, 0, 0, 0, 0, 
                                      hWndParent, (HMENU) code, hInst, NULL);
	if (hWndToolbar == NULL)
		return NULL;

	SendMessage(hWndToolbar, TB_AUTOSIZE, 0, 0);
	return hWndToolbar;
}

#pragma endregion

#pragma region ProgressBar


HWND WindAPI::CreateProgressBar(HWND hWndParent, UINT code, RECT rect, CString type,
		size_t range_max, size_t range_min, size_t step, size_t time)
{
	HWND hWnd;
	hWnd = CreateWindowEx(0, PROGRESS_CLASS, (LPTSTR) NULL, 
		WS_CHILD | WS_VISIBLE | (type == "Animation" ? PBS_MARQUEE : 0),
							rect.left, rect.top, 
							rect.right - rect.left, rect.bottom - rect.top, 
                            hWndParent, (HMENU) code, GetModuleHandle(0), NULL);
	if (type == "Progress")
	{
		SendMessage(hWnd, PBM_SETRANGE32, (WPARAM) range_min, (LPARAM) range_max);
		SendMessage(hWnd, PBM_SETSTEP, (WPARAM) step, 0);
		SendMessage(hWnd, PBM_SETMARQUEE, (WPARAM) false, 0);
	}
	else if (type == "Animation")
	{
		SendMessage(hWnd, PBM_SETMARQUEE, (WPARAM) true, (LPARAM) time);
	}
	return hWnd;
}


void WindAPI::ProgressBarSetRange(HWND ProgressBar, size_t range_min, size_t range_max)
{
	SendMessage(ProgressBar, PBM_SETRANGE32, (WPARAM) range_min, (LPARAM) range_max);
}


void WindAPI::ProgressBarSetStep(HWND ProgressBar, size_t step)
{

}


void WindAPI::ProgressBarSetValue(HWND ProgressBar, size_t value)
{
	SendMessage(ProgressBar, PBM_SETPOS, value, 0);
	UpdateWindow(ProgressBar);
}

void WindAPI::ProgressBarSetType(HWND ProgressBar, CString type)
{
	if (type == "Progress")
	{
		SendMessage(ProgressBar, PBM_SETMARQUEE, (WPARAM) false, 0);
		LONG l = GetWindowLong(ProgressBar, GWL_STYLE);
		SetWindowLong(ProgressBar, GWL_STYLE, (LONG) l & ~PBS_MARQUEE);
	}
	else if (type == "Animation")
	{
		LONG l = GetWindowLong(ProgressBar, GWL_STYLE);
		SetWindowLong(ProgressBar, GWL_STYLE, (LONG) l | PBS_MARQUEE);
		SendMessage(ProgressBar, PBM_SETMARQUEE, (WPARAM) true, (LPARAM) 0);
	}
}

#pragma endregion

#pragma region Panel

HWND WindAPI::CreatePanel(HWND Parent, UINT code, RECT rect)
{
	HWND Panel = NULL;
	HINSTANCE hInst = GetModuleHandle(0);
	DWORD dwStyle = WS_CHILD|WS_VISIBLE|SS_CENTER|SS_CENTERIMAGE|SS_SUNKEN|WS_CLIPCHILDREN|WS_BORDER;
	Panel = CreateWindowEx(NULL, L"STATIC", L"",
		dwStyle, rect.left, rect.top, (rect.right - rect.left),
		(rect.bottom - rect.top), Parent, (HMENU)code, hInst, (LPVOID)NULL);
	return Panel;
}

#pragma endregion

#pragma region GroupBox

HWND WindAPI::CreateGroupBox(HWND Parent, CWString text, UINT code, RECT rect)
{
	HWND GB;

	GB = CreateWindow(WC_BUTTON, text.data(), WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
		rect.left, rect.top, (rect.right - rect.left), (rect.bottom - rect.top),
		Parent, (HMENU) code, GetModuleHandle(0), NULL);
	this->SetDefaultFont(GB);
	return GB;
}

#pragma endregion

#pragma region Tabs

HWND WindAPI::CreateTab(HWND Parent)
{
	RECT rcClient; 
    INITCOMMONCONTROLSEX icex;
    HWND hwndTab; //tab control

	//Инициализация tab control
	icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
    icex.dwICC = ICC_TAB_CLASSES;
    InitCommonControlsEx(&icex);

	//Создание tab control
	GetClientRect(Parent, &rcClient); 
    hwndTab = CreateWindow(WC_TABCONTROL, L"", 
        WS_CHILD | WS_CLIPSIBLINGS | WS_VISIBLE, 
        0, 0, rcClient.right, rcClient.bottom, 
		Parent, NULL, GetModuleHandle(0), NULL);
	SendMessage(hwndTab, WM_SETFONT, (WPARAM)hfDefault, MAKELPARAM(FALSE,0)); //настройка шрифта
	return hwndTab;
}

BOOL WindAPI::InsertTab(HWND Tab, int pos, CWString text)
{
	TCITEM tie;
	tie.mask = TCIF_TEXT | TCIF_IMAGE; 
    tie.iImage = -1; 
	tie.pszText = (wchar_t *) text.data();
	if (TabCtrl_InsertItem(Tab, pos, &tie) == -1)
	{
		return FALSE;
	}
	return TRUE;
}


CWString WindAPI::GetCurTabName(HWND tab)
{
	CWString text;
	TCITEM item;
	ZeroMemory(&item, sizeof(item));
	item.dwStateMask = TCIF_TEXT;
	item.cchTextMax = 256;
	wchar_t *temp = new wchar_t[256];
	item.pszText = temp;
	TabCtrl_GetItem(tab, this->GetCurTabPos(tab), &item);
	text = temp;
	delete [256] temp;
	return text;
}

int WindAPI::GetCurTabPos(HWND tab)
{
	return TabCtrl_GetCurSel(tab);
}

#pragma endregion

#pragma region ComboBox

ComboBox::ComboBoxWND WindAPI::CreateComboBox(ComboBox::ComboBoxType type,
		HWND Parent, RECT rect, UINT code)
{
	ComboBox::ComboBoxWND cb;
	cb.type = type;
	cb.code = code;
	cb.rect = rect;
	int x = rect.left, y = rect.top,
		w = rect.right - rect.left, h = rect.bottom - rect.top;
	ZeroMemory(&cb, sizeof(ComboBox::ComboBoxWND));
	DWORD dwType = 0;
	switch (type)
	{
	case ComboBox::ComboBoxType::CBT_Simple:
		dwType = CBS_SIMPLE;
		break;
	case ComboBox::ComboBoxType::CBT_DropDown:
		dwType = CBS_DROPDOWN;
		break;
	case ComboBox::ComboBoxType::CBT_DropDownList:
		dwType = CBS_DROPDOWNLIST;
		break;
	default:
		break;
	}
	cb.hWnd = CreateWindow(WC_COMBOBOX, L"", 
			dwType | CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE,
			x, y, w, h, Parent, (HMENU) code, GetModuleHandle(0), NULL);
	this->SetDefaultFont(cb.hWnd);
	return cb;
}

#pragma endregion

#pragma region TreeView

TreeView::TreeViewWND WindAPI::CreateTreeView(HWND Parent, RECT rect, UINT code)
{
	TreeView::TreeViewWND TreeView;
	TreeView.hWnd = CreateWindowEx(0,
                            WC_TREEVIEW,
                            TEXT("Tree View"),
                            WS_VISIBLE | WS_CHILD | WS_BORDER | TVS_HASLINES, 
							rect.left,
							rect.top,
                            rect.right,
                            rect.bottom,
                            Parent, 
                            (HMENU)code, 
							GetModuleHandle(0), 
                            NULL);
	TreeView.code = code;
	return TreeView;
}

TreeView::TreeViewNode *WindAPI::InsertItemInTreeView(TreeView::TreeViewWND &TreeView,
													 CWString name, CWString text, UINT level,
													 TreeView::TreeViewNode *Parent, bool bimage_select, bool bimage)
{
	TreeView::TreeViewNode item;
	item.text = text;
	item.wnd = &TreeView;
	item.name = name;
	if (Parent)
	{
		TVITEM tvi;
		BOOL fl;
		tvi.hItem = Parent->item;
		tvi.cchTextMax = 256;
		tvi.mask = TVIF_HANDLE;
		fl = TreeView_GetItem(TreeView.hWnd, &tvi);
		if (fl)
		{
			tvi.cChildren = 1;
			fl = TreeView_SetItem(TreeView.hWnd, &tvi);
			if (!fl)
			{
				return &TreeView::TreeViewNode();
			}
		}
	}
	TVITEM tvi; 
    TVINSERTSTRUCT tvins;
	tvi.mask = TVIF_TEXT | TVIF_PARAM;
	tvi.mask = tvi.mask | (bimage ? TVIF_IMAGE : 0);
	tvi.mask = tvi.mask | (bimage_select ? TVIF_SELECTEDIMAGE : 0);
	tvi.mask = tvi.mask | (Parent ? TVIF_CHILDREN : NULL);
	tvi.pszText = (wchar_t *) text.data();
	tvi.cchTextMax = text.size();
	tvi.lParam = (LPARAM) level;
	tvins.item = tvi;
	tvins.hInsertAfter = (HTREEITEM) TVI_LAST; //всегда добавлять сверху
	tvins.hParent = Parent ? Parent->item : TVI_ROOT;
	item.item = TreeView_InsertItem(TreeView.hWnd, &tvins);
	if (level > 0)
    { 
		HTREEITEM hti;
		hti = TreeView_GetParent(TreeView.hWnd, item.item); 
        tvi.mask = TVIF_IMAGE | TVIF_SELECTEDIMAGE; 
        tvi.hItem = hti;
		TreeView_SetItem(TreeView.hWnd, &tvi); 
    }
	item.parent = Parent;
	item.level = tvi.lParam;
	if (Parent)
	{
		Parent->nodes.insert(name, item);
		return &Parent->nodes.find(name)->object;
	}
	else
	{
		TreeView.nodes.insert(name, item);
		return &TreeView.nodes.find(name)->object;
	}
}

TreeView::TreeViewNode *WindAPI::GetSelectedItem(TreeView::TreeViewWND &TreeView)
{
	HTREEITEM item = TreeView_GetSelection(TreeView.hWnd);
	return TreeView.Find(item);
}

void WindAPI::SetSelectedItem(TreeView::TreeViewWND &TreeView, TreeView::TreeViewNode *item)
{
	TreeView_SelectItem(TreeView.hWnd, item->item);
}

CWString WindAPI::GetItemName(TreeView::TreeViewWND &TreeView, HTREEITEM item)
{
	TreeView::TreeViewNode *Item;
	Item = TreeView.Find(item);
	return Item->name;
}

RECT WindAPI::GetItemRect(TreeView::TreeViewWND &TreeView, TreeView::TreeViewNode &item)
{
	RECT rect;
	ZeroMemory(&rect, sizeof(RECT));
	if (item.item == NULL || TreeView.hWnd == NULL) return rect;
	TreeView_GetItemRect(TreeView.hWnd, item.item, &rect, FALSE);
	POINT min, max;
	min.x = rect.left;
	min.y = rect.top;
	max.x = rect.right;
	max.y = rect.bottom;
	ClientToScreen(TreeView.hWnd, &min);
	ClientToScreen(TreeView.hWnd, &max);
	rect.left = min.x;
	rect.top = min.y;
	rect.right = max.x;
	rect.bottom = max.y;
	return rect;
}

//
TreeView::TreeViewNode *WindAPI::GetItemByPoint(TreeView::TreeViewWND &TreeView, Vec2 Point)
{
	return TreeView.GetItemByPoint(Point);
}

//
TreeView::TreeViewNode *WindAPI::GetItemByPoint(TreeView::TreeViewWND &TreeView, POINT Point)
{
	return GetItemByPoint(TreeView, Vec(Point.x, Point.y));
}

BOOL WindAPI::DestroyItem(TreeView::TreeViewNode &node)
{
	return node.wnd->DestroyItem(node);
}

#pragma endregion

#pragma region ListView

ListView::ListViewWND WindAPI::CreateListView(HWND Parent, RECT rect, UINT code)
{
	ListView::ListViewWND ListViewWND;
	RECT rcClient;
	GetClientRect (Parent, &rcClient);
	ListViewWND.wnd = CreateWindow(WC_LISTVIEW, 
                                     L"",
									 WS_CHILD | WS_BORDER | WS_VISIBLE,
                                     0, 0,
                                     rcClient.right - rcClient.left,
                                     rcClient.bottom - rcClient.top,
                                     Parent,
                                     (HMENU)code,
									 GetModuleHandle(0),
                                     NULL);
	ListViewWND.rect = rcClient;
	return ListViewWND;
}

ListView::ListViewItem *WindAPI::InsertItemInListView(ListView::ListViewWND &wnd, //окно
													  CWString name, //имя
													  CWString text, //текст
													  int item_id, //ИД итемки
													  int image_id //ИД картинки
													  )
{
	ListView::ListViewItem item;
	LVITEM lvI;
	lvI.pszText   = text.data(); // Sends an LVN_GETDISPINFO message.
    lvI.mask      = LVIF_TEXT | (image_id < 0 ? 0 : LVIF_IMAGE) | LVIF_STATE;
    lvI.stateMask = 0;
    lvI.iSubItem  = 0;
    lvI.state     = 0;
	lvI.iItem = item_id;
	lvI.iImage = image_id;
	item.text = text;
	item.name = name;
	item.image_index = image_id;
	item.item_index = item_id;
	if (ListView_InsertItem(wnd.wnd, &lvI) == -1)
            return NULL;
	wnd.items.push_back(item);
	return &wnd.items.back();
}

#pragma endregion


#pragma region ListView


#pragma endregion
#pragma endregion