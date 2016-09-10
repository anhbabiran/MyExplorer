#ifndef CADDRESS_H
#define CADDRESS_H

#include <windows.h>
#include <commctrl.h>
#pragma comment(lib, "comctl32.lib")

#include "resource.h"

class CAddress
{
private:
	HINSTANCE	m_hInst;
	HWND		m_hParent;
	HWND		m_hAddress;
	int			m_nID;

public:
	CAddress();
	~CAddress();

	void Create(HWND hParent, long ID, HINSTANCE hInst);
	void SetButtonImages();
	BOOL AddButton(BYTE fsState, BYTE fsStyle, int iBitmap, int idCommand, 
			LPTSTR iString, DWORD_PTR dwData);
	HWND AddNonButtonControl(LPTSTR className,LPTSTR caption, int styles,
			int ID,int width,int height,int btnIndex,int iCommand, int lechTop);
	HWND GetHandle();
	void Size();
};

#endif