#include "Dnode.h"
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")

DNode::DNode()
{
	m_path = NULL;
	m_pNext = NULL;
	m_pPrev = NULL;
}

DNode::~DNode()
{
	delete[] m_path;
	delete m_pNext;
	delete m_pPrev;
}
//---------------------------------

LPCTSTR DNode::GetPath()
{
	return m_path;
}

DNode* DNode::GetNext()
{
	return m_pNext;
}

DNode* DNode::GetPrev()
{
	return m_pPrev;
}
//-----------------------------------

void DNode::SetPath(LPCWSTR path)
{
	int len = wcslen(path);
	m_path = new TCHAR[len + 1];
	StrCpy(m_path, path);
}

void DNode::SetNext(DNode *next)
{
	m_pNext = next;
}

void DNode::SetPrev(DNode *prev)
{
	m_pPrev = prev;
}