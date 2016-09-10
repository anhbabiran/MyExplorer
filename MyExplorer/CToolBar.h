#ifndef CTOOLBAR_H
#define CTOOLBAR_H

#include <windows.h>
#include <commctrl.h>
#pragma comment(lib, "comctl32.lib")

class CToolBar
{
private:
	HINSTANCE	m_hInst;
	HWND		m_hParent;
	HWND		m_hToolBar;
	int			m_nID;

public:
	CToolBar();
	void Create(HWND parentWnd, long ID, HINSTANCE hParentInst,
		int nWidth, int nHeight, int x = CW_USEDEFAULT, int y = 0,
		long lStyle = WS_CHILD | WS_VISIBLE | WS_BORDER | TBSTYLE_FLAT);
	BOOL AddButton(BYTE fsState, BYTE fsStyle, int iBitmap, int idCommand, LPTSTR iString, DWORD_PTR dwData);
	void SetButtonImages();
	HWND AddNonButtonControl(LPTSTR className,LPTSTR caption, int styles,int ID,int width,int height,int btnIndex,int iCommand);
	HWND GetHandle();
	void EnableBack(BOOL bEnable);
	void EnableForward(BOOL bEnable);
};


#endif