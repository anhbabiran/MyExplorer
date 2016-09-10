#ifndef DNODE_H
#define	DNODE_H
#include <windows.h>
#include <tchar.h>
#include <malloc.h>
#include <memory.h>

class DNode
{
private:
	TCHAR *m_path;	
	DNode *m_pNext; 
	DNode *m_pPrev;
public:
	DNode();
	~DNode();
	LPCTSTR GetPath();
	DNode* GetNext();
	DNode* GetPrev();
	
	void SetPath(LPCWSTR path);
	void SetNext(DNode *next);
	void SetPrev(DNode *prev);
};
#endif