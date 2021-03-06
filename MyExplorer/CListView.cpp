#include "Resource.h"
#include "CListView.h"
#include <tchar.h>
#include "CDrive.h"

#include <shlobj.h> //Shell object

//Dùng để sử dụng hàm StrCpy, StrNCat
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")

//usb27,  flopy23, cd82, folder3  , desktop105, computer104, unknoewFile2
//Vị trí icon trong imageres.dll
#define ID_IMAGE_HDD 30
#define ID_IMAGE_CD 82
#define ID_IMAGE_USB 27
#define ID_IMAGE_FLOPPY 23
#define ID_IMAGE_FOLDER 3
#define ID_IMAGE_DESKTOP 105
#define ID_IMAGE_COMPUTER 104
#define ID_IMAGE_UNKFILE 2	//Chú ý khác với Tree-View là FOLDER_SELECTED

//Vị trí trong image list sau khi add
#define ID_PFOLDER 0
#define ID_PUNKFILE 1
#define ID_PDESKTOP 2
#define ID_PCOMPUTER 3

#define ID_PFLOPPY 4
#define ID_PUSB 5
#define ID_PHDD 6
#define ID_PCD  7 

#define INITIAL_ICON_IN_TREE 8 //Tổng số icon lúc đầu trong tree, mặc định chỉ nạp những gì cần thiết
#define NUMBER_OF_ICON_TO_GROW 65535 //Số icon có thể mở rộng

//Độ dài tối đa đường dẫn
#define MAX_PATH_LEN 10240
/*****************************************************************************************/

CListView::CListView()
{
	m_hInst = NULL;
	m_hParent = NULL;
	m_hListView = NULL;
	m_nID = 0;
}

CListView::~CListView()
{
	DestroyWindow(m_hListView);
}

void CListView::Create(HWND parentWnd, long ID, HINSTANCE hParentInst, 
					   int nWidth, int nHeight, 
					   int x, int y, long lExtStyle, long lStyle)
{
	InitCommonControls();
	CListView::m_hInst = hParentInst;
	CListView::m_hParent = parentWnd;
	CListView::m_hListView = CreateWindowEx(lExtStyle, WC_LISTVIEW, _T("List View"), 
		WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_TABSTOP | lStyle,
		x, y, nWidth, nHeight, m_hParent, (HMENU) ID, m_hInst, NULL);
	CListView::m_nID = ID;

	InitImageList();
	Init4Cols();
}

void CListView::Init4Cols()
{
	LVCOLUMN lvCol;

	lvCol.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;
	lvCol.fmt=LVCFMT_LEFT;

	lvCol.cx = 130;
	lvCol.pszText = _T("Name");
	ListView_InsertColumn(m_hListView, 0, &lvCol);

	lvCol.fmt=LVCFMT_LEFT | LVCF_WIDTH;
	lvCol.cx = 100;
	lvCol.pszText = _T("Type");
	ListView_InsertColumn(m_hListView, 1, &lvCol);

	lvCol.fmt = LVCFMT_RIGHT;
	lvCol.pszText = _T("Size");
	ListView_InsertColumn(m_hListView, 2, &lvCol);

	lvCol.pszText = _T("Detail");
	ListView_InsertColumn(m_hListView, 3, &lvCol);
}

void CListView::InitDriveColumn()
{
	LVCOLUMN lvCol;

	lvCol.mask = LVCF_TEXT| LVCF_FMT;

	lvCol.fmt=LVCFMT_LEFT | LVCF_WIDTH;
	lvCol.cx = 100;
	lvCol.pszText = _T("Type");
	ListView_SetColumn(m_hListView, 1, &lvCol);

	lvCol.fmt = LVCFMT_RIGHT | LVCF_WIDTH;
	lvCol.cx = 80;
	lvCol.pszText = _T("Total Size");
	ListView_SetColumn(m_hListView, 2, &lvCol);
	
	lvCol.cx = 80;
	lvCol.pszText = _T("Free Space");
	ListView_SetColumn(m_hListView, 3, &lvCol);
}

void CListView::InitFolderColumn()
{
	LVCOLUMN lvCol;

	lvCol.mask = LVCF_WIDTH;
	lvCol.cx = 180;
	ListView_SetColumn(m_hListView, 0, &lvCol);

	lvCol.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_FMT;
	lvCol.fmt = LVCFMT_LEFT;
	lvCol.cx = 130;
	lvCol.pszText = _T("Date Modified");
	ListView_SetColumn(m_hListView, 1, &lvCol);
	lvCol.pszText = _T("Type");
	ListView_SetColumn(m_hListView, 2, &lvCol);
	lvCol.pszText = _T("Size");
	ListView_SetColumn(m_hListView, 3, &lvCol);
}

//Add danh sách hình
void CListView::InitImageList()
{
	/// Resource win8 C:\\Windows\\System32\\imageres.dll
	HIMAGELIST *hLarge = new HIMAGELIST;
	HIMAGELIST *hSmall = new HIMAGELIST;

	*hSmall = ImageList_Create(16, 16, ILC_COLOR32 | ILC_MASK, 
		INITIAL_ICON_IN_TREE, NUMBER_OF_ICON_TO_GROW);
	*hLarge = ImageList_Create(32, 32, ILC_COLOR32 | ILC_MASK, 
		INITIAL_ICON_IN_TREE, NUMBER_OF_ICON_TO_GROW);

	HICON hIcon;

	//Các icon cho image list (2 loại lớn và nhỏ)
	hIcon = ExtractIcon(m_hInst, _T("C:\\Windows\\System32\\imageres.dll"), ID_IMAGE_FOLDER);
	ImageList_AddIcon(*hLarge, hIcon);
	ImageList_AddIcon(*hSmall, hIcon);

	hIcon = ExtractIcon(m_hInst, _T("C:\\Windows\\System32\\imageres.dll"), ID_IMAGE_UNKFILE);
	ImageList_AddIcon(*hLarge, hIcon);
	ImageList_AddIcon(*hSmall, hIcon);
	
	hIcon = ExtractIcon(m_hInst, _T("C:\\Windows\\System32\\imageres.dll"), ID_IMAGE_DESKTOP);
	ImageList_AddIcon(*hLarge, hIcon);
	ImageList_AddIcon(*hSmall, hIcon);

	hIcon = ExtractIcon(m_hInst, _T("C:\\Windows\\System32\\imageres.dll"), ID_IMAGE_COMPUTER);
	ImageList_AddIcon(*hLarge, hIcon);
	ImageList_AddIcon(*hSmall, hIcon);

	hIcon = ExtractIcon(m_hInst, _T("C:\\Windows\\System32\\imageres.dll"), ID_IMAGE_FLOPPY);
	ImageList_AddIcon(*hLarge, hIcon);
	ImageList_AddIcon(*hSmall, hIcon);

	hIcon = ExtractIcon(m_hInst, _T("C:\\Windows\\System32\\imageres.dll"), ID_IMAGE_USB);
	ImageList_AddIcon(*hLarge, hIcon);
	ImageList_AddIcon(*hSmall, hIcon);

	hIcon = ExtractIcon(m_hInst, _T("C:\\Windows\\System32\\imageres.dll"), ID_IMAGE_HDD);
	ImageList_AddIcon(*hLarge, hIcon);
	ImageList_AddIcon(*hSmall, hIcon);

	hIcon = ExtractIcon(m_hInst, _T("C:\\Windows\\System32\\imageres.dll"), ID_IMAGE_CD);
	ImageList_AddIcon(*hLarge, hIcon);
	ImageList_AddIcon(*hSmall, hIcon);
	
	ListView_SetImageList(m_hListView, *hSmall, LVSIL_SMALL);
	ListView_SetImageList(m_hListView, *hLarge, LVSIL_NORMAL);
}
/*****************************************************************************************/

void CListView::Size()
{
	RECT tree;
	GetWindowRect(GetDlgItem(m_hParent, IDC_TREEVIEW), &tree);

	RECT main;
	GetWindowRect(m_hParent, &main);

	MoveWindow(m_hListView, tree.right - tree.left, 75, main.right - tree.right, tree.bottom - tree.top, TRUE);
	ListView_Arrange(m_hListView, LVA_ALIGNTOP);
}

// Lấy cửa sổ
HWND CListView::GetHandle()
{
	return m_hListView;
}

//Lấy số tượng item dc chọn
int CListView::GetCountItem()
{
	return ListView_GetSelectedCount(m_hListView);
}
vector<int> CListView::GetItemSelected()
{
	vector<int> tmp;
	int iSelected = 0;
	iSelected = ListView_GetNextItem(m_hListView, -1, LVNI_SELECTED);
	while (iSelected != -1)
	{
		tmp.push_back(iSelected);
		iSelected = ListView_GetNextItem(m_hListView, iSelected, LVNI_SELECTED);
	}
	return tmp; // lấy tất cả file dc chọn
}
vector<LPCWSTR> CListView::GetListPath()
{
	vector<int> pos;
	pos = this->GetItemSelected();
	vector<LPCWSTR> tmp;
	for (int i = 0; i < pos.size(); i++)
	{
		LVITEM lv;
		lv.mask = LVIF_PARAM;
		lv.iItem = pos[i];
		lv.iSubItem = 0;
		ListView_GetItem(m_hListView, &lv);
		tmp.push_back((LPCWSTR)lv.lParam); /// =>>> Trả về đường dẫn 
	}
	return tmp;
}
/// Lấy thông tin đường dẫn 1 ITEM 
//     với iItem là vị trí của ITEM trong listView
LPCWSTR CListView::GetPath(int iItem)
{
	LVITEM lv;
	lv.mask = LVIF_PARAM;
	lv.iItem = iItem;
	lv.iSubItem = 0;
	ListView_GetItem(m_hListView, &lv);
	return (LPCWSTR)lv.lParam; /// =>>> Trả về đường dẫn 
}

LPCWSTR CListView::GetCurSelPath()
{
	return GetPath(ListView_GetSelectionMark(m_hListView));  /// -> Dùng để get cái vị trí của item đã dc click
}


//////////////////////////////////////////////////////////////////////////
LPCWSTR CListView::GetCurParentPath()
{
	LPCWSTR path = GetPath(0);
	if (path == NULL)
		return NULL;

	if (!StrCmp(path, L"MyComputer")) //Cha đang là Desktop
		return _T("Desktop");
	else
		if (wcslen(path) == 3) //Cha đang là My Computer
			return _T("MyComputer");
		else
		{	
			TCHAR *parent;
			int nBackSlachPos = (StrRStrI(path, NULL, _T("\\")) - path);
			parent = new TCHAR[nBackSlachPos+2];
			StrNCpy(parent, path, nBackSlachPos+1);
			return parent;
		}
}
/////////////////////////
void CListView::GetFocus()
{
	SetFocus(m_hListView);
}
/*****************************************************************************************/

void CListView::LoadDesktop()
{
	SetDlgItemText(GetDlgItem(m_hParent, IDC_ADDRESS), IDC_ADDRESS_EDIT, _T("Desktop"));
	DeleteAll();
	InitFolderColumn();

	LV_ITEM lv;
	lv.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM;
	lv.iItem = 0;
	lv.iSubItem = 0;
	lv.pszText = _T("My Computer");
	lv.iImage = ID_PCOMPUTER;
	lv.lParam = (LPARAM)_T("MyComputer");
	ListView_InsertItem(m_hListView, &lv);

	SendMessage(GetDlgItem(m_hParent, IDC_STATUSBAR),SB_SETTEXT, 2, (LPARAM)_T("Desktop"));
}
void CListView::LoadMyComputer(CDrive *drive)
{
	SetDlgItemText(GetDlgItem(m_hParent, IDC_ADDRESS), IDC_ADDRESS_EDIT, _T("My Computer"));
	InitDriveColumn();
	DeleteAll();
	LV_ITEM lv;
	
	for (int i = 0; i < drive->GetCount(); ++i)
	{
		//Nạp cột đầu tiên cũng là thông tin chính
		lv.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM;
		lv.iItem = i;	                                     /// --->> ITEM ở vị trí thứ I của listView
		lv.iImage = drive->GetIconIndex(i);

		lv.iSubItem = 0;
		lv.pszText = drive->GetDisplayName(i);
		lv.lParam = (LPARAM) drive->GetDriveName(i);
		ListView_InsertItem(m_hListView, &lv);

		//Nạp các cột còn lại (Type, Total Size, Free Space)
		lv.mask = LVIF_TEXT;

		//Type
		lv.iSubItem = 1;
		lv.pszText = drive->GetType(i);
		ListView_SetItem(m_hListView, &lv);
		
		//Total Size
		lv.iSubItem = 2;

		if (drive->GetIconIndex(i) != ID_PFLOPPY && drive->GetIconIndex(i) != ID_PCD)
			lv.pszText = drive->GetSizeStr(i);
		else
			lv.pszText = NULL;

		ListView_SetItem(m_hListView, &lv);

		//Free Space
		lv.iSubItem = 3;

		if (drive->GetIconIndex(i) != ID_PFLOPPY && drive->GetIconIndex(i) != ID_PCD)
			lv.pszText = drive->GetFreeSizeStr(i);
		else
			lv.pszText = NULL;

		ListView_SetItem(m_hListView, &lv);
	}

	
	TCHAR *buffer = new TCHAR[34];
	wsprintf(buffer, _T("My Computer có tổng cộng %d ổ đĩa"), drive->GetCount());
	SendMessage(GetDlgItem(m_hParent, IDC_STATUSBAR),SB_SETTEXT, 0, (LPARAM) buffer);
	SendMessage(GetDlgItem(m_hParent, IDC_STATUSBAR),SB_SETTEXT, 2, (LPARAM)_T("My Computer"));
}

void CListView::LoadChild(LPCWSTR path, CDrive *drive)
{
	if (path == NULL)
		return;

	if (!StrCmp(path, _T("Desktop")))
		LoadDesktop();
	else
		if (!StrCmp(path, _T("MyComputer")))
			LoadMyComputer(drive);
		else
			LoadFileAndFolder(path);
}

/// Dùng để load tập tin và folder lên LISTVIEW
void CListView::LoadFileAndFolder(LPCWSTR path)
{
	SetDlgItemText(GetDlgItem(m_hParent, IDC_ADDRESS), IDC_ADDRESS_EDIT, path);
	InitFolderColumn();
	DeleteAll();

	TCHAR buffer[10240];
	StrCpy(buffer, path);

	if ( wcslen(path) == 3) //Nếu quét các ổ đĩa
		StrCat(buffer, _T("*"));
	else
		StrCat(buffer, _T("\\*"));

	///////////////////////////////////////////////////////////////
	WIN32_FIND_DATA fd;
	HANDLE hFile;
	LV_ITEM lv;

	TCHAR *folderPath;
	DWORD folderCount = 0;
	TCHAR *filePath;
	DWORD fileCount = 0;

	DWORD fileSizeCount = 0;
	int nItemCount = 0;

	////////////////////////// LOAD FILE VÀ FOLDER ////////////////////////
	hFile = FindFirstFileW(buffer, &fd);

	if (hFile == INVALID_HANDLE_VALUE)
		return;
	
	do
	{
		if ((fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) &&
			((fd.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) != FILE_ATTRIBUTE_HIDDEN) &&
			(StrCmp(fd.cFileName, _T(".")) != 0) && (StrCmp(fd.cFileName, _T("..")) != 0)) ///// Lưu ý: _T("..") chính là BACK
		{
			folderPath = new TCHAR[wcslen(path) + wcslen(fd.cFileName) + 2];
			StrCpy(folderPath, path);
			
			if (wcslen(path) != 3)
				StrCat(folderPath, _T("\\"));
			StrCat(folderPath, fd.cFileName);

			lv.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM;
			lv.iItem = nItemCount;
			lv.iSubItem = 0;
			lv.pszText = fd.cFileName;
			lv.lParam = (LPARAM)folderPath;
			lv.iImage = ID_PFOLDER;
			ListView_InsertItem(m_hListView, &lv);

			//Date modified
			ListView_SetItemText(m_hListView, nItemCount, 1, _GetDateModified(fd.ftLastWriteTime));

			//Type
			ListView_SetItemText(m_hListView, nItemCount, 2, _T("Folder"));

			++nItemCount;
			++folderCount;
		}
		else if ((fd.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) != FILE_ATTRIBUTE_HIDDEN &&
			(StrCmp(fd.cFileName, _T(".")) != 0) && (StrCmp(fd.cFileName, _T("..")) != 0)) // _T(..) là NÚT BACK
		{
			filePath = new TCHAR[wcslen(path) + wcslen(fd.cFileName) + 2];
			StrCpy(filePath, path);

			StrCat(filePath, _T("\\"));
			StrCat(filePath, fd.cFileName);

			//Name column
			lv.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM;
			lv.iItem = nItemCount;
			lv.iSubItem = 0;
			lv.pszText = fd.cFileName;
			lv.lParam = (LPARAM)filePath;
			lv.iImage = _GetImageListIconIndex(filePath, fd);
			ListView_InsertItem(m_hListView, &lv);

			//Date Modified	
			ListView_SetItemText(m_hListView, nItemCount, 1, _GetDateModified(fd.ftLastWriteTime));

			//Type
			ListView_SetItemText(m_hListView, nItemCount, 2, _GetType(fd));

			//Size
			ListView_SetItemText(m_hListView, nItemCount, 3, FormatSize(fd.nFileSizeLow));
			fileSizeCount += fd.nFileSizeLow;

			++nItemCount;
			++fileCount;
		}

	} while (FindNextFile(hFile, &fd) != 0);

	TVITEMEX tv;
	TCHAR *folder = new TCHAR[512];
	TCHAR *info = new TCHAR[256];

	tv.mask = TVIF_TEXT;
	tv.hItem = TreeView_GetNextItem(GetDlgItem(m_hParent, IDC_TREEVIEW), NULL, TVGN_CARET);
	tv.pszText = folder;
	tv.cchTextMax = 256;
	TreeView_GetItem(GetDlgItem(m_hParent, IDC_TREEVIEW), &tv);

	wsprintf(info, _T(" %d folders and %d files"), folderCount, fileCount);
	StrCat(folder, info);
	
	SendMessage(GetDlgItem(m_hParent, IDC_STATUSBAR),SB_SETTEXT, 0, (LPARAM)tv.pszText);
	SendMessage(GetDlgItem(m_hParent, IDC_STATUSBAR), SB_SETTEXT, 1, (LPARAM)FormatSize(fileSizeCount));
}

///////////////// Để OPEN folder hoặc file
void CListView::LoadCurSel()
{
	LPCWSTR path = GetCurSelPath();

	WIN32_FIND_DATA fd;
	GetFileAttributesEx(path, GetFileExInfoStandard, &fd);

	//Nếu là thư mục
	if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
	{
		ListView_DeleteAllItems(m_hListView);
		LoadFileAndFolder(path);
	}
	else //Nếu là tập tin thì chạy nó
		ShellExecute(NULL, _T("open"), path, NULL, NULL, SW_SHOWNORMAL);
}

void CListView::DisplayInfoCurSel()
{
	int nCurSelIndex = ListView_GetNextItem(GetDlgItem(m_hParent, IDC_LISTVIEW), -1, LVNI_FOCUSED);
	TCHAR *text = new TCHAR[256];
	LVITEM lv;
	lv.mask = LVIF_TEXT;
	lv.iItem = nCurSelIndex;
	lv.iSubItem = 0;
	lv.pszText = text;
	lv.cchTextMax = 256;
	
	ListView_GetItem(m_hListView, &lv);
	SendMessage(GetDlgItem(m_hParent, IDC_STATUSBAR), SB_SETTEXT, 2,(LPARAM) text);

	lv.iSubItem = 2;
	ListView_GetItem(m_hListView, &lv);

	if (!StrCmpI(lv.pszText, _T("Thư mục")))
		SendMessage(GetDlgItem(m_hParent, IDC_STATUSBAR), SB_SETTEXT, 1, NULL);
	else
	{
		lv.iSubItem = 1;
		ListView_GetItem(m_hListView, &lv);
		SendMessage(GetDlgItem(m_hParent, IDC_STATUSBAR), SB_SETTEXT, 1,(LPARAM) text);
	}
}
/*****************************************************************************************/


void CListView::ChangeViewOption(int nNewStyle)
{
	LONG dNotView = ~(LVS_ICON | LVS_SMALLICON | LVS_LIST | LVS_REPORT);

	SetWindowLong(m_hListView, GWL_STYLE, GetWindowLong(m_hListView, GWL_STYLE) & dNotView | nNewStyle);

}
void CListView::DeleteAll()
{
	ListView_DeleteAllItems(m_hListView);
}
void CListView::SelectAll()
{
	int nOld = ListView_GetSelectionMark(m_hListView);
	ListView_SetItemState(m_hListView, nOld, 0, LVIS_FOCUSED | LVIS_SELECTED);

	for (int i = ListView_GetItemCount(m_hListView) - 1; i >= 0; --i)
	{
		ListView_SetItemState(m_hListView, i, LVIS_FOCUSED | LVIS_SELECTED, LVIS_FOCUSED | LVIS_SELECTED);
		ListView_SetSelectionMark(m_hListView, i);
	}

	GetFocus();
}
void CListView::Up(CDrive *drive)
{
	LPCWSTR path = GetPath(0);
	if (!StrCmp(path, L"MyComputer"))
		return;
	
	if (wcslen(path) == 3) //Ổ đĩa
	{
		DeleteAll();
		LoadDesktop();
		return;
	}

	TCHAR *parent;
	TCHAR *grandPa;
	int nBackSlachPos = (StrRStrI(path, NULL, _T("\\")) - path);
	if (nBackSlachPos == 2)
	{
		DeleteAll();
		LoadMyComputer(drive);
		return;
	}

	parent = new TCHAR[nBackSlachPos+2];
	StrNCpy(parent, path, nBackSlachPos+1);
	nBackSlachPos = (StrRStrI(parent, NULL, _T("\\")) - parent);

	grandPa = new TCHAR[nBackSlachPos  + 2];
	StrNCpy(grandPa, parent, nBackSlachPos+1);
	DeleteAll();
	LoadFileAndFolder(grandPa);
}

/*****************************************************************************************/
#define KB 1
#define MB 2
#define GB 3
#define TB 4
#define RADIX 10
LPWSTR CListView::FormatSize(__int64 nSize)
{
	int nType = 0; //Bytes

	while (nSize >= 1048576) //
	{
		nSize /= 1024;
		++nType;
	}

	__int64 nRight;

	if (nSize >= 1024)
	{
		//Lấy một chữ số sau thập phân của nSize chứa trong nRight
		nRight = nSize % 1024;

		while (nRight > 99)
			nRight /= 10;

		nSize /= 1024;
		++nType;
	}
	else
		nRight = 0;

	TCHAR *buffer = new TCHAR[11];
	_itow_s(nSize, buffer, 11, RADIX);

	if (nRight != 0 && nType > KB)
	{
		StrCat(buffer, _T("."));
		TCHAR *right = new TCHAR[3];
		_itow_s(nRight, right, 3, RADIX);
		StrCat(buffer, right);
	}

	switch (nType)
	{
	case 0://Bytes
		StrCat(buffer, _T(" bytes"));
		break;
	case KB:
		StrCat(buffer, _T(" KB"));
		break;
	case MB:
		StrCat(buffer, _T(" MB"));
		break;
	case GB:
		StrCat(buffer, _T(" GB"));
		break;
	case TB:
		StrCat(buffer, _T(" TB"));
		break;
	}

	return buffer;
}

#define ENTIRE_STRING NULL

LPWSTR CListView::_GetType(const WIN32_FIND_DATA &fd)
{
	int nDotPos = StrRStrI(fd.cFileName, ENTIRE_STRING, _T(".")) - fd.cFileName;
	int len = wcslen(fd.cFileName);
	
	if (nDotPos < 0 || nDotPos >= len) //Nếu không tìm thấy
		return _T("Không biết");

	TCHAR *szExtension = new TCHAR[len - nDotPos + 1];
	int i;

	for (i = nDotPos; i < len; ++i)
		szExtension[i - nDotPos] = fd.cFileName[i];
	szExtension[i - nDotPos] = NULL; //Kí tự kết thúc chuỗi
	
	if (!StrCmpI(szExtension, _T(".htm")) || !StrCmpI(szExtension, _T(".html")))
	{
		return _T("Web page");
	}
	TCHAR pszOut[256];
	HKEY hKey;
	DWORD dwType = REG_SZ;
	DWORD dwSize = 256;
	
	//Kiếm handle của extension tương ứng trong registry
	if (RegOpenKeyEx(HKEY_CLASSES_ROOT, szExtension, 0, KEY_READ, &hKey) != ERROR_SUCCESS)
	{
		RegCloseKey(hKey);
		return _T("Không biết");
	}

	if (RegQueryValueEx( hKey, NULL, NULL, &dwType, (PBYTE) pszOut, &dwSize) != ERROR_SUCCESS)
	{
		RegCloseKey(hKey);
		return _T("Không biết");
	}
	RegCloseKey(hKey);

	//Kiếm mô tả về thông tin của extension thông qua handle của key tương ứng trong registry
	TCHAR *pszPath = new TCHAR[1000];
	dwSize = 1000;
	if (RegOpenKeyEx(HKEY_CLASSES_ROOT, pszOut, 0, KEY_READ, &hKey) != ERROR_SUCCESS)
	{
		RegCloseKey(hKey);
		return _T("Không biết");
	}

	if (RegQueryValueEx( hKey, NULL, NULL, &dwType, (PBYTE) pszPath, &dwSize)!= ERROR_SUCCESS)
	{
		RegCloseKey(hKey);
		return _T("Không biết");
	}
	RegCloseKey(hKey);

	return pszPath;
}

LPWSTR CListView::_GetDateModified(const FILETIME &ftLastWrite)
{
	// Chuyển đổi thời gian 
	TCHAR		buf[20];
	LPTSTR		buffer = new TCHAR[40];
	SYSTEMTIME	stime;

	FileTimeToSystemTime(&ftLastWrite, &stime);

	if (GetDateFormat(LOCALE_SYSTEM_DEFAULT, NULL, &stime, _T("dd/MM/yyyy"), buffer, 20) == 0 ||
		GetTimeFormat(LOCALE_SYSTEM_DEFAULT, NULL, &stime, _T("hh:mm tt"), buf, 20) == 0) {
		StrCpy(buffer, _T(""));
	}
	else {
		StrCat(buffer, _T(" "));
		StrCat(buffer, buf);
	}

	return buffer;
}

int CListView::_GetImageListIconIndex(LPCWSTR path, const WIN32_FIND_DATA &fd)
{
	unsigned int nDotPos = StrRStrI(fd.cFileName, ENTIRE_STRING, _T(".")) - fd.cFileName;
	unsigned int len = wcslen(fd.cFileName);

	if (nDotPos < 0 || nDotPos >= len) //Nếu không tìm thấy
		return ID_PUNKFILE;

	TCHAR *szExtension = new TCHAR[len - nDotPos + 1];
	unsigned int i;

	for (i = nDotPos; i < len; ++i)
		szExtension[i - nDotPos] = fd.cFileName[i];
	szExtension[i - nDotPos] = NULL; //Kí tự kết thúc chuỗi
	
	HICON hicon;

	// ok
	if (!StrCmpI(szExtension, _T(".exe")))
	{
		hicon = ExtractIcon(m_hInst, path, NULL);
		ImageList_AddIcon(ListView_GetImageList(m_hListView, LVSIL_SMALL), hicon);

		return ImageList_AddIcon(ListView_GetImageList(m_hListView, LVSIL_NORMAL), hicon);
	}
	
	if (!StrCmpI(szExtension, _T(".htm")) || !StrCmpI(szExtension, _T(".html")))
	{
		hicon = ExtractIcon(m_hInst, _T("C:\Program Files\Internet Explorer"), NULL);
		ImageList_AddIcon(ListView_GetImageList(m_hListView, LVSIL_SMALL), hicon);

		return ImageList_AddIcon(ListView_GetImageList(m_hListView, LVSIL_NORMAL), hicon);
	}
	///////////////////////////////////////////////////
	// Phần này set image cho tất cả các loại còn lại
	TCHAR pszOut[256];
	HKEY hKey;
	DWORD dwType = REG_SZ;
	DWORD dwSize = 256;

	//Kiếm handle của extension tương ứng trong registry
	if (RegOpenKeyEx(HKEY_CLASSES_ROOT, szExtension, 0, KEY_READ, &hKey) != ERROR_SUCCESS)
	{
		RegCloseKey(hKey);
		return ID_PUNKFILE;
	}

	if (RegQueryValueEx(hKey, NULL, NULL, &dwType, (PBYTE) pszOut, &dwSize) != ERROR_SUCCESS)
	{
		RegCloseKey(hKey);
		return ID_PUNKFILE;
	}
	RegCloseKey(hKey);

	//Kiếm mô tả về thông tin của extension thông qua handle của key tương ứng trong registry
	TCHAR pszOut1[256];
	StrCpy(pszOut1, pszOut);
	StrCat(pszOut1, _T("\\DefaultIcon"));

	if (RegOpenKeyEx(HKEY_CLASSES_ROOT, pszOut1, 0, KEY_READ, &hKey) != ERROR_SUCCESS)
	{
		RegCloseKey(hKey);
		return ID_PUNKFILE;
	}

	TCHAR *pszPath = new TCHAR[1000];
	dwSize = 1000;
	if (RegQueryValueEx( hKey, NULL, NULL, &dwType, (PBYTE) pszPath, &dwSize)!= ERROR_SUCCESS)
	{
		RegCloseKey(hKey);
		return ID_PUNKFILE;
	}

	RegCloseKey(hKey);

	//Có được đường dẫn đến file chứa icon và vị trí icon trong file nhưng phải xử lí
	//Tìm coi có dấu chấm không
	nDotPos = StrRStrI(pszPath, ENTIRE_STRING, _T(".")) - pszPath;
	len = wcslen(pszPath);

	if (nDotPos < 0 || nDotPos >= len) //Nếu không có dấu chấm
		return ID_PUNKFILE;

	//Tìm coi có dấu phẩy không
	int nIconIndex;
	unsigned int nCommaPos = StrRStrI(pszPath, ENTIRE_STRING, _T(",")) - pszPath;
	TCHAR *filePath;

	if (nCommaPos < 0 || nCommaPos >= len) //Nếu không tìm thấy dấu phẩy thì mặc định là 0
	{
		nIconIndex = 0;
		filePath = new TCHAR[len];
		StrCpy(filePath, pszPath);
	}
	else
	{
		TCHAR *szIndex = new TCHAR[len - nCommaPos];
		
		++nCommaPos;
		for (i = nCommaPos ; i < len; ++i)
			szIndex[i - nCommaPos] = pszPath[i];
		szIndex[i - nCommaPos] = NULL;

		nIconIndex = _wtoi(szIndex);//Chuyển sang số là xong
		filePath = new TCHAR[nCommaPos];
		StrNCpy(filePath, pszPath, nCommaPos);
	}

	hicon = ExtractIcon(m_hInst, filePath, nIconIndex);
	ImageList_AddIcon(ListView_GetImageList(m_hListView, LVSIL_SMALL), hicon);

	return ImageList_AddIcon(ListView_GetImageList(m_hListView, LVSIL_NORMAL), hicon);
}	
