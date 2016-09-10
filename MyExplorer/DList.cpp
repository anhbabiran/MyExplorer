#include "DList.h"
DList::DList()
{
	m_pHead = NULL;
	m_pTail = NULL;
	m_pCur = NULL;
}

DList::~DList()
{
	delete m_pHead;
	delete m_pTail;
	delete m_pCur;
}

DNode* DList::GetCur()
{
	return m_pCur;
}

bool DList::IsEmpty()
{
	return (m_pHead == NULL);
}

DNode* DList::GetNode(LPCTSTR path)
{
	DNode *new_ele = new DNode;
	if (new_ele == NULL)
		return NULL;

	new_ele->SetPath(path);
	new_ele->SetNext(NULL);
	new_ele->SetPrev(NULL);

	return new_ele;
}

void DList::InsertAfterCur(LPCWSTR path)
{
	DNode* new_ele = GetNode(path);
	if (new_ele != NULL)
	{
		if (IsEmpty())
		{
			m_pHead = m_pTail = m_pCur = new_ele;
		}
		else 
		{
			if (m_pHead == m_pTail)
			{
				new_ele->SetPrev(m_pTail);
				m_pTail->SetNext(new_ele);
				m_pTail = new_ele;
			}
			else
			{
				DNode* pCurNext = m_pCur->GetNext();
				new_ele->SetPrev(m_pCur);
				new_ele->SetNext(pCurNext);
				m_pCur->SetNext(new_ele);
				
				if (pCurNext == NULL)
					m_pTail = new_ele;
				else
					pCurNext->SetPrev(new_ele);
			}//else

			m_pCur = new_ele;
		}//else
	}//if
}

void DList::GoBack()
{
	m_pCur = m_pCur->GetPrev();
}

void DList::GoForward()
{
	m_pCur = m_pCur->GetNext();
}