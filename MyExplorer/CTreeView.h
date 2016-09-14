#ifndef CTREEVIEW_H
#define CTREEVIEW_H

#include <windows.h>
#include <tchar.h>

#include <commctrl.h>
#pragma comment(lib, "comctl32.lib")

#include "CDrive.h"

class CTreeView
{
private:
	HINSTANCE	m_hInst;
	HWND		m_hParent;
	HWND		m_hTreeView;
	int			m_nID;
	
	void GetImageList();

public:
	CTreeView();
	~CTreeView();

	void Create(HWND parentWnd, long ID, HINSTANCE hParentInst, int nWidth, int nHeight, 
		long lExtStyle = 0, long lStyle = TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | TVS_SHOWSELALWAYS, 
		int x = CW_USEDEFAULT, int y = 0);

	HWND		GetHandle();
	int			GetID();
	LPCWSTR		GetPath(HTREEITEM hItem);
	LPCWSTR		GetCurPath();
	HTREEITEM	GetCurSel();
	void		GetFocus();
	HTREEITEM	GetDesktop();
	HTREEITEM	GetMyComputer();

	void LoadMyComputer(CDrive *);
	void LoadChild(HTREEITEM &hParent, LPCWSTR path, BOOL bShowHiddenSystem = FALSE);
	void PreloadExpanding(HTREEITEM hPrev, HTREEITEM hCurSel);
	void Expand(HTREEITEM hItem);
	void Size(int cy);
	void PreLoad(HTREEITEM hItem);
};

#endif