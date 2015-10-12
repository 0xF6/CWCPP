#pragma once
#include <Windows.h>
#include <mmsystem.h>
#include <Windowsx.h>
#include <Shlobj.h>
#include "CArray.h"
#include "CMap.h"
struct AMem {
private:
	void *pointer;
	size_t size;
public:
	AMem(void *pointer, size_t size);
	//Высвобождает память
	void Release(); 
	//Возвращает размер памяти
	__forceinline size_t get_size()
	{
		return this->size;
	}
	//Возвращает указатель на выделенную память
	__forceinline void *get_memory()
	{
		return this->pointer;
	}
};


struct MenuItem {
	CWString name;
	CWString text;
	HMENU menu;
	UINT code;
	~MenuItem();
};

struct WindItem {
	CWString name;
	CWString text;
	HWND item;
	UINT code;
	~WindItem();
};


struct FileFilter {
	CMap<CWString, CArray<CWString>> filters; //фильтры
	void AddFilter(CWString name, CArray<CWString> types); //добавляет фильтр
	void AddFilter(CWString name, CWString type); //добавляет фильтр
	CWString MakeFilter(); //создаёт строку-фильтр под WinAPI
};

namespace ComboBox {

	enum ComboBoxType {
		CBT_Simple, CBT_DropDown, CBT_DropDownList
	};

	struct ComboBoxItem {
		int index;
		CWString text;
		CWString name;
	};

	struct ComboBoxWND {
		HWND hWnd;
		ComboBoxType type;
		UINT code;
		RECT rect;
		CArray<ComboBoxItem> items;
		int Add(CWString text, CWString name);
		int GetSelId();
		CWString GetSelString();
		CWString GetSelName();
		void SetSel(int index);
		void SetSel(CWString name);
	};
}


namespace TreeView {

	struct TreeViewWND;

	struct TreeViewNode {
		TreeViewWND *wnd;
		CWString name;
		CWString text;
		HTREEITEM item;
		LPARAM level;
		CMap<CWString, TreeViewNode> nodes;
		TreeViewNode *Find(HTREEITEM item);
		TreeViewNode *Find(CWString name);
		TreeViewNode *FindLocal(CWString name);
		TreeViewNode *GetItemByPoint(Vec2 Point);
		TreeViewNode *parent;
		void Open();
		void Clear();
		void UpdateText();
		~TreeViewNode();
	};

	struct TreeViewWND {
		HWND hWnd;
		UINT code;
		CMap<CWString, TreeViewNode> nodes;
		TreeViewNode *Find(HTREEITEM item);
		TreeViewNode *Find(CWString name);
		TreeViewNode *FindLocal(CWString name);
		TreeViewNode *GetItemByPoint(Vec2 Point);
		~TreeViewWND();
		BOOL DestroyItem(TreeView::TreeViewNode &node);
		void ClearInItem(TreeView::TreeViewNode &node);
		void Clear();
	};
}



namespace ListView {
	struct ListViewItem {
		int image_index;
		int item_index;
		bool image;
		CWString name;
		CWString text;
		void Update();
	};

	struct ListViewWND {
		HWND wnd;
		RECT rect;
		CArray<ListViewItem> items;
		void Update();
	};
}
/*
namespace ListView {
	struct ListViewItem {
		int image_index;
		int item_index;
		bool image;
		CWString name;
		CWString text;
		void Update();
	};

	struct ListViewWND {
		HWND wnd;
		RECT rect;
		CArray<ListViewItem> items;
		void Update();
	};
}
*/
class WindAPI
{
	Vec2 ScreenSize;
	HDC GlobalDC;
	HGDIOBJ hfDefault;
	CWString name;
	void UpdateKeys();
	void TryKey(int key_num);
public:
	WindAPI();
	~WindAPI();
	//
	/*
	 * Windows and WinFuctions
	 */
	//
	void GetScreenSize(Vec2 &ScreenSize);
	static void GetWindowSize(HWND hWnd, Vec2 &WindowSize);
	static WNDCLASSEX CreateWindowClass(CString type,
		HINSTANCE hInstance = GetModuleHandle(0), WNDPROC proc = NULL);
	HWND CreateNewWindow(CString type,
		HINSTANCE hInstance = GetModuleHandle(0), CWString CustomName = L"", HWND Parent = NULL);
	HWND CreateNewWindowSimple(CString type, CWString CustomName = L"", HWND Parent = NULL);
	void SendName(CWString name);
	static HICON SimpleLoadIcon(CWString icon);
	/*
	 * Files and Folders
	 */
	//Загружает файл. @filename - путь, @file - файл, @direct - загружать ли локально.
	void LoadFile(CWString filename, File &file, bool direct = true);
	//Сохраняет файл. @filename - путь, @Data - данные, @size - размер, @direct - сохранять ли локально.
	void SaveFile(CWString filename, LPVOID Data, DWORD size, bool direct = true);
	void SaveFile(CWString filename, CString &data, bool direct = true);
	void SaveFile(CWString filename, File &file, bool direct = true);
	void OpenStream(FileStream &stream, CWString filename,
		StreamMode mode = StreamMode::FSM_READ, bool direct = true, bool create = false);
	size_t FileSize(CWString filename, bool direct = true);
	bool ExistsFile(CWString filename, bool direct = true);
	bool CreateFolder(CWString name, CWString path = L"");
	bool ExistsFolder(CWString name, CWString path = L"");
	void DeleteFolder(CWString name, CWString path = L"");
	//Перечисляет файлы в текущей или заданной папке
	void ListFiles(CWString path, CArray<CWString> &files, bool direct = true);
	//Получает папку, где лежит запущеный файл
	static CWString GetNativeFolder();
	//
	static CWString GetWorkingFolder();

	static CWString GetStartFolder();

	static CWString GetFullFileName();

	static CWString GetFileName(CWString path);

	static CWString GetFileType(CWString name);
	/*
	 * Files and Folders Dialogs 
	 */
	//Открывает диалоговое окно открытия файла
	CWString OpenFileOpenDialog(CWString Directory = GetWorkingFolder(),
		wchar_t *FileFilter = NULL);
	//Открывает диалоговое окно сохранения файла
	CWString OpenFileSaveDialog(CWString Directory = GetWorkingFolder(),
		wchar_t *FileFilter = NULL);
	//Открывает диалоговое окно открытия файла
	CWString OpenFileOpenDialogC(CWString Directory = GetWorkingFolder(),
		FileFilter filter = FileFilter());
	//Открывает диалоговое окно сохранения файла
	CWString OpenFileSaveDialogC(CWString Directory = GetWorkingFolder(),
		FileFilter filter = FileFilter());
	//Открывает диалоговое окно для выбора папки
	CWString OpenFolderDialog(CWString Directory = GetWorkingFolder(),
		CWString DialogName = L"Выбирете папку");
	/*
	 * Common WindAPI functions
	 */
	//
	void Update();
	void HideAllWindows();
	void SetDefaultFont(HWND hWnd);
	void SetWindowTop(HWND hWnd);
	/*
	 * Audio API's
	 */
	BOOL PlayAudio(CWString file, bool async = true);
	BOOL PlayAudio(File &file, bool async = true);
	BOOL PlayAudio(FileStream &stream, bool async = true);
	BOOL StopAudio();
	/*
	 * Registers
	 */
	void RegisterWindow(UINT code, HWND hWnd);
	void RegisterElement(UINT code, HWND hWnd);
	void RegisterMenu(UINT code, HMENU hMenu);
	void RegisterCode(UINT code, CString name);
	void Unregister(UINT code);
	UINT GetLastRegCode();
	HWND GetElement(UINT code);
	UINT GetCodeByElement(HWND hWnd);
	HMENU GetMenu(UINT code);
	CString GetRegisterByCode(UINT code);
	/*
	 * Common Controls API's
	 */
	/*Static/Text API*/
	//Создаёт статический текст
	HWND CreateText(CWString text, UINT code, Vec2 Pos, Vec2 Size, HWND Parent = NULL);
	//Создаёт статический текст
	HWND CreateText(CWString text, UINT code, RECT rect, HWND Parent = NULL);
	/*Buttons API*/
	//Создаёт кнопку
	HWND CreateButton(CWString text, UINT code, RECT rect, HWND Parent = NULL);
	/*Menus API*/
	//
	static BOOL CreateMenuItem(HMENU hMenu,
		CWString str, UINT uIns, UINT uCom, HMENU hSubMenu, BOOL flag, UINT fType);
	void CreateMenu(HMENU hMenu, HMENU child, int id, UINT code, CWString text);
	BOOL GetMenuRect(HWND hwnd, HMENU hmenu, RECT *pmenurect);
	static BOOL CreateContextMenuItem(HMENU hMenu, UINT uFlags, UINT code, CWString text);
	/*Toolbars API*/
	//
	HWND CreateHToolbar(HWND hWndParent, UINT code);
	/*ProgressBar API*/
	HWND CreateProgressBar(HWND hWndParent, UINT code, RECT rect, CString type = "Progress",
		size_t range_max = 100, size_t range_min = 0, size_t step = 1, size_t time = 30);
	void ProgressBarSetRange(HWND ProgressBar, size_t range_min, size_t range_max);
	void ProgressBarSetStep(HWND ProgressBar, size_t step);
	void ProgressBarSetValue(HWND ProgressBar, size_t value);
	void ProgressBarSetType(HWND ProgressBar, CString type);
	/*Panels API*/
	//
	HWND CreatePanel(HWND Parent, UINT code, RECT rect);
	/*Group Box API*/
	//
	HWND CreateGroupBox(HWND Parent, CWString text, UINT code, RECT rect);
	/*Tabs API*/
	//
	HWND CreateTab(HWND Parent);
	//
	BOOL InsertTab(HWND Tab, int pos, CWString text);
	//
	int GetCurTabPos(HWND tab);
	//
	CWString GetCurTabName(HWND tab);
	/*ComboBox API*/
	ComboBox::ComboBoxWND CreateComboBox(ComboBox::ComboBoxType type,
		HWND Parent, RECT rect, UINT code);
	
	/*Tree View API*/
	//
	TreeView::TreeViewWND CreateTreeView(HWND Parent, RECT rect, UINT code);
	//
	TreeView::TreeViewNode *InsertItemInTreeView(TreeView::TreeViewWND &TreeView, 
		CWString name, CWString text, UINT level,
		TreeView::TreeViewNode *Parent = NULL, bool image_select = false, bool image = false);
	//
	void SetSelectedItem(TreeView::TreeViewWND &TreeView, TreeView::TreeViewNode *item);
	//
	TreeView::TreeViewNode *GetSelectedItem(TreeView::TreeViewWND &TreeView);
	//Getting TreeView item
	CWString GetItemName(TreeView::TreeViewWND &TreeView, HTREEITEM item);
	//Getting TreeView item rectangle
	static RECT GetItemRect(TreeView::TreeViewWND &TreeView, TreeView::TreeViewNode &item);
	//Getting TreeView item by position (Vec2)
	TreeView::TreeViewNode *GetItemByPoint(TreeView::TreeViewWND &TreeView, Vec2 Point);
	//Getting TreeView item by position (POINT)
	TreeView::TreeViewNode *GetItemByPoint(TreeView::TreeViewWND &TreeView, POINT Point);
	//Destroy TreeView item in TreeView node
	BOOL DestroyItem(TreeView::TreeViewNode &node);
	/*List View API*/
	//Creating List View
	ListView::ListViewWND CreateListView(HWND Parent, RECT rect, UINT code);
	//Insert Item in LIstView
	ListView::ListViewItem *InsertItemInListView(ListView::ListViewWND &wnd,
		CWString name,
		CWString text, int item_id, int image_id = -1);
};