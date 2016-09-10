#include "CStatus.h"

CStatus::CStatus()
{
	m_hInst = NULL;
	m_hParent = NULL;
	m_hStatus = NULL;
	m_nID = 0;
}

void CStatus::Create(HWND parentWnd, long ID, HINSTANCE hParentInst, long lStyle)
{
	InitCommonControls();
	m_hInst = hParentInst;
	m_hParent = parentWnd;
	m_hStatus = CreateWindow(STATUSCLASSNAME, NULL, lStyle, 0, 0, CW_USEDEFAULT, 100,
		m_hParent, (HMENU) ID, m_hInst, NULL);
	m_nID = ID;
}

HWND CStatus::GetHandle()
{
	return m_hStatus;
}

void CStatus::Size()
{
	RECT main;
	GetWindowRect(m_hParent, &main);

	int nStatusSize[] = {main.right*6/9, main.right*6/9 + main.right/12 , -1};
	SendMessage(m_hStatus, SB_SETPARTS, 3, (LPARAM) &nStatusSize);
	MoveWindow(m_hStatus, 0, 0, main.right, main.bottom, TRUE);
}