#include "CAddress.h"
#include "tchar.h"
#include <windowsx.h>

CAddress::CAddress()
{
	m_hInst = NULL;
	m_hParent = NULL;
	m_hAddress = NULL;
	m_nID = 0;
}

CAddress::~CAddress()
{

}

void CAddress::Create(HWND hParent, long ID, HINSTANCE hInst)
{
	InitCommonControls();
	m_hInst = hInst;
	m_hParent = hParent;
	m_hAddress = CreateWindow(TOOLBARCLASSNAME, NULL, WS_CHILD | WS_VISIBLE | TBSTYLE_LIST,
		0, 100, 100, 200,
		m_hParent, (HMENU) ID, m_hInst, NULL);
	m_nID = ID;
	SetButtonImages();
	AddNonButtonControl(_T("STATIC"), _T("Address"), WS_CHILD | WS_VISIBLE, 
			IDC_ADDRESS_STATIC, 50, 50, 0, IDC_ADDRESS_STATIC, 3);
	HFONT hFont = (HFONT) GetStockObject(DEFAULT_GUI_FONT);
	SendMessage(GetDlgItem(m_hAddress, IDC_ADDRESS_STATIC), WM_SETFONT, (WPARAM) hFont, TRUE);

	RECT main;
	GetWindowRect(m_hParent, &main);
	AddNonButtonControl(_T("EDIT"), NULL, WS_CHILD | WS_VISIBLE, 
		IDC_ADDRESS_EDIT, 900, 20, 1, IDC_ADDRESS_EDIT, 1);
	SendMessage(GetDlgItem(m_hAddress, IDC_ADDRESS_EDIT), WM_SETFONT, (WPARAM) hFont, TRUE);

	AddButton(TBSTATE_ENABLED, BTNS_BUTTON, 0 , IDC_ADDRESS_GO, _T("Go"), 0);
}

void CAddress::SetButtonImages()
{
	SendMessage(m_hAddress,TB_SETBITMAPSIZE,0,MAKELONG(16, 16));

	HIMAGELIST hIml = ImageList_Create(16, 16, ILC_MASK | ILC_COLOR32, 1, 0);
	HICON hIcon = LoadIcon(m_hInst, MAKEINTRESOURCE(IDI_GO));
	ImageList_AddIcon(hIml, hIcon);

	//ImageList_SetBkColor(hIml, CLR_NONE);

	SendMessage(m_hAddress,TB_SETIMAGELIST, 0,(LPARAM) hIml);
}

BOOL CAddress::AddButton(BYTE fsState, BYTE fsStyle, int iBitmap, int idCommand, LPTSTR iString, DWORD_PTR dwData)
{
	int iStr = SendMessage(m_hAddress, TB_ADDSTRING, 0, (LPARAM) iString); 	
	TBBUTTON button[1];	

	button[0].dwData = dwData;
	button[0].fsState = fsState;
	button[0].iBitmap = iBitmap;

	button[0].fsStyle = fsStyle;
	button[0].idCommand = idCommand;
	button[0].iString = iStr;

	return SendMessage(m_hAddress,TB_ADDBUTTONS ,1,(LPARAM)&button);		
}

HWND CAddress::AddNonButtonControl(LPTSTR className,LPTSTR caption, int styles,int ID,int width,int height,int btnIndex,int iCommand, int lechTop)
{
	TBBUTTON button;		
	button.fsState=TBSTATE_ENABLED;	
	button.fsStyle=BTNS_SEP;
	button.idCommand=iCommand;	
	BOOL kq=SendMessage(m_hAddress, TB_INSERTBUTTON ,btnIndex,(LPARAM)&button);

	TBBUTTONINFO info;
	info.cbSize=sizeof(TBBUTTONINFO);
	info.dwMask=TBIF_SIZE;
	info.cx=width;	
	SendMessage(m_hAddress, TB_SETBUTTONINFO,iCommand,(LPARAM)&info);

	RECT rect;
	SendMessage(m_hAddress, TB_GETITEMRECT,btnIndex,(LPARAM) &rect);	

	HWND hWnd=CreateWindow( className, caption, styles, rect.left,rect.top + lechTop,
		rect.right-rect.left, height==0?rect.bottom-rect.top:height, 
		m_hAddress, (HMENU) ID, m_hInst, 0 );

	return hWnd;
}

HWND CAddress::GetHandle()
{
	return m_hAddress;
}

void CAddress::Size()
{
	RECT main;
	GetWindowRect(m_hParent, &main);
	SetWindowPos(m_hAddress, GetDlgItem(m_hParent, IDC_TOOLBAR), 0, 50, 1000,25, TRUE);
}