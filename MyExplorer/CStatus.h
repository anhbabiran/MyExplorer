#ifndef CSTATUS_H
#define CSTATUS_H

#include <windows.h>
#include <commctrl.h>
#pragma comment(lib, "comctl32.lib")

class CStatus
{
private:
	HINSTANCE	m_hInst;
	HWND		m_hParent;
	HWND		m_hStatus;
	int			m_nID;
public:
	CStatus();
	void Create(HWND parentWnd, long ID, HINSTANCE hParentInst,
		long lStyle = WS_CHILD | WS_VISIBLE | SBARS_SIZEGRIP |SBARS_TOOLTIPS);
	HWND GetHandle();
	void Size();
};
#endif