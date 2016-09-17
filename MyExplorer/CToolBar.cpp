#include "CToolBar.h"
#include "resource.h"
#include <tchar.h>

CToolBar::CToolBar()
{
	m_hInst = NULL;
	m_hParent = NULL;
	m_hToolBar = NULL;
	m_nID = 0;
}

void CToolBar::Create(HWND parentWnd, long ID, HINSTANCE hParentInst,
	int nWidth, int nHeight, int x, int y, long lStyle)
{
	InitCommonControls();
	m_hInst = hParentInst;
	m_hParent = parentWnd;
	m_hToolBar = CreateWindow(TOOLBARCLASSNAME, NULL, lStyle, x, y, nWidth, nHeight,
		m_hParent, (HMENU)ID, m_hInst, NULL);
	m_nID = ID;
	SendMessage(m_hToolBar, TB_BUTTONSTRUCTSIZE, sizeof(TBBUTTON), 0);

	//----------------
	SendMessage(m_hToolBar, TB_SETBITMAPSIZE, 0, MAKELONG(49, 32));
	HIMAGELIST hIml = ImageList_Create(24, 24, ILC_MASK | ILC_COLOR32, 47, 0);

	SetButtonImages(IDB_LEFT, hIml);
	SetButtonImages(IDB_NEXT, hIml);
	SetButtonImages(IDB_UP, hIml);
	SetButtonImages(IDB_SEARCH0, hIml);
	SetButtonImages(IDB_VIEW, hIml);
	SetButtonImages(IDB_CUT, hIml);
	SetButtonImages(IDB_COPY, hIml);
	SetButtonImages(IDB_PASTE, hIml);
	SetButtonImages(IDB_RENAME, hIml);
	SetButtonImages(IDB_NEW0, hIml);
	SetButtonImages(IDB_DEL, hIml);
	SetButtonImages(IDB_REFRESH, hIml);
	SetButtonImages(IDB_FAR, hIml);

	SendMessage(m_hToolBar, TB_SETIMAGELIST, 0, (LPARAM)hIml);

	//-------------
	AddButton(TBSTATE_ENABLED, BTNS_BUTTON, 0, IDC_TOOLBAR_BACK, _T("Back"), 0);
	AddButton(TBSTATE_ENABLED, BTNS_BUTTON, 1, IDC_TOOLBAR_FORWARD, _T("Forward"), 1);
	AddButton(TBSTATE_ENABLED, BTNS_BUTTON, 2, IDC_TOOLBAR_UP, _T("Up"), 2);
	AddButton(TBSTATE_ENABLED, BTNS_SEP, 0, NULL, NULL, 4);

	AddButton(TBSTATE_ENABLED, BTNS_BUTTON, 3, IDC_TOOLBAR_SEARCH, _T("Search"), 3);
	AddButton(TBSTATE_ENABLED, BTNS_WHOLEDROPDOWN, 4, IDC_TOOLBAR_VIEW, _T("View"), 4);
	AddButton(TBSTATE_ENABLED, BTNS_SEP, 0, NULL, NULL, 0);

	AddButton(TBSTATE_ENABLED, BTNS_BUTTON, 5, IDC_TOOLBAR_CUT, _T("Cut"), 5);
	AddButton(TBSTATE_ENABLED, BTNS_BUTTON, 6, IDC_TOOLBAR_COPY, _T("Copy"), 6);
	AddButton(TBSTATE_ENABLED, BTNS_BUTTON, 7, IDC_TOOLBAR_PASTE, _T("Paste"), 7);
	AddButton(TBSTATE_ENABLED, BTNS_SEP, 0, NULL, NULL, 0);

	AddButton(TBSTATE_ENABLED, BTNS_BUTTON, 8, IDC_TOOLBAR_RENAME, _T("Rename"), 8);
	AddButton(TBSTATE_ENABLED, BTNS_BUTTON, 9, IDC_TOOLBAR_NEW, _T("New Folder"), 9);
	AddButton(TBSTATE_ENABLED, BTNS_BUTTON, 10, IDC_TOOLBAR_DELETE, _T("Delete"), 10);
	AddButton(TBSTATE_ENABLED, BTNS_SEP, 0, NULL, NULL, 0);

	AddButton(TBSTATE_ENABLED, BTNS_BUTTON, 11, IDC_TOOLBAR_REFRESH, _T("Refresh"), 8);
	AddButton(TBSTATE_ENABLED, BTNS_BUTTON, 12, IDC_TOOLBAR_FAR, _T("Favorite"), 10);
}

BOOL CToolBar::AddButton(BYTE fsState, BYTE fsStyle, int iBitmap, int idCommand, LPTSTR iString, DWORD_PTR dwData)
{
	int iStr = SendMessage(m_hToolBar, TB_ADDSTRING, 0, (LPARAM)iString);
	TBBUTTON button[1];

	button[0].dwData = dwData;
	button[0].fsState = fsState;
	button[0].iBitmap = iBitmap;

	button[0].fsStyle = fsStyle;
	button[0].idCommand = idCommand;
	button[0].iString = iStr;

	return SendMessage(m_hToolBar, TB_ADDBUTTONS, 1, (LPARAM)&button);
}

void CToolBar::SetButtonImages(int iBitmap, HIMAGELIST hIml)
{
	//SendMessage(m_hToolBar,TB_SETBITMAPSIZE,0,MAKELONG(49, 32));

	//	HIMAGELIST hIml = ImageList_Create(24, 24, ILC_MASK | ILC_COLOR32, 47, 0);
	HBITMAP hBmp = LoadBitmap(m_hInst, MAKEINTRESOURCE(iBitmap));

	ImageList_AddMasked(hIml, hBmp, RGB(0, 0, 0));
	ImageList_SetBkColor(hIml, CLR_NONE);

	//SendMessage(m_hToolBar,TB_SETIMAGELIST, 0,(LPARAM) hIml);
}

//Neu goi ham nay nhieu lan thi phai goi theo thu tu btnIndex tang dan
HWND CToolBar::AddNonButtonControl(LPTSTR className, LPTSTR caption, int styles, int ID, int width, int height, int btnIndex, int iCommand)
{
	TBBUTTON button;
	button.fsState = TBSTATE_ENABLED;
	button.fsStyle = BTNS_SEP;
	button.idCommand = iCommand;
	BOOL kq = SendMessage(m_hToolBar, TB_INSERTBUTTON, btnIndex, (LPARAM)&button);

	TBBUTTONINFO info;
	info.cbSize = sizeof(TBBUTTONINFO);
	info.dwMask = TBIF_SIZE;
	info.cx = width;
	SendMessage(m_hToolBar, TB_SETBUTTONINFO, iCommand, (LPARAM)&info);

	RECT rect;
	SendMessage(m_hToolBar, TB_GETITEMRECT, btnIndex, (LPARAM)&rect);

	//Neu height=0 thi chieu cao se duoc tinh bang bottom-top
	//Neu height<>0 thi chieu cao se la height 
	HWND hWnd = CreateWindow(className, caption, styles, rect.left, rect.top,
		rect.right - rect.left, height == 0 ? rect.bottom - rect.top : height,
		m_hToolBar, (HMENU)ID, m_hInst, 0);

	return hWnd;
}

HWND CToolBar::GetHandle()
{
	return m_hToolBar;
}

void CToolBar::EnableBack(BOOL bEnable)
{
	SendMessage(m_hToolBar, TB_ENABLEBUTTON, IDC_TOOLBAR_BACK, (LPARAM)MAKELONG(bEnable, 0));
}

void CToolBar::EnableForward(BOOL bEnable)
{
	SendMessage(m_hToolBar, TB_ENABLEBUTTON, IDC_TOOLBAR_FORWARD, (LPARAM)MAKELONG(bEnable, 0));
}