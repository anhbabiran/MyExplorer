// MyExplorer.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "MyExplorer.h"

#include <string.h>
#include <shlobj.h>
//Dùng để sử dụng hàm StrCpy, StrNCat
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")

////////////////////////////////////TESTTTTTTTTTTTTT////////////////////
void DoViewChange(LPNMTOOLBAR lpnmToolBar);
void DoSizeTreeView();

void CreateNewFolder();

void DoCopy();
void DoCut();
void DoPaste();
void DoRefresh();
void DoDelete();

void DoGo();
void DoGoUp();
void DoGoBack();
void DoGoForward();

//include
#include "CListView.h"
//
#include "CAddress.h"
#include "CStatus.h"
#include "CToolBar.h"
#include "CTreeView.h"
#include "DList.h"
//globla variable
CListView Clv;
CDrive *Cdr = new CDrive;

///////////////
HWND hWnd;
vector<LPCWSTR> listFile; // Để chứa các file để copy hoặc paste
TCHAR sourceEvent[10240]; // Chứa đường dẫn event
int checkEvent = 0;  // 1 là copy, 2 là cut
////////////////////////////////
RECT g_TreeViewRect;
CAddress *g_Address = new CAddress;
CStatus *g_Status = new CStatus;
CToolBar *g_ToolBar = new CToolBar;
CTreeView *g_TreeView = new CTreeView;
DList *g_History = new DList;

NMHDR *pnm;
//////////////////////////////////////////////////////////////////////


#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_MYEXPLORER, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MYEXPLORER));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MYEXPLORER));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_MYEXPLORER);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, SW_MAXIMIZE); // bật max cỡ CT
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_CREATE:
	{
		// Đăng kí hotKey chả cần HOOK
		RegisterHotKey(hWnd, CTRL_C, MOD_CONTROL, 0x43);
		RegisterHotKey(hWnd, CTRL_V, MOD_CONTROL, 0x56);
		RegisterHotKey(hWnd, CTRL_X, MOD_CONTROL, 0x58);
		RegisterHotKey(hWnd, CTRL_R, MOD_CONTROL, 0x52);
		RegisterHotKey(hWnd, CTRL_N, MOD_CONTROL, 0x4E);

		RECT main;
		GetWindowRect(hWnd, &main);

		////
		g_Address = new CAddress;
		g_Address->Create(hWnd, IDC_ADDRESS, hInst);

		//Khởi tạo các control của mình

		g_ToolBar = new CToolBar;
		g_ToolBar->Create(hWnd, IDC_TOOLBAR, hInst, 0, 0, 0, 0);
		g_ToolBar->EnableBack(FALSE);
		g_ToolBar->EnableForward(FALSE);

		g_Status = new CStatus;
		g_Status->Create(hWnd, IDC_STATUSBAR, hInst);

		///

		Cdr->GetSystemDrives();
		///
		
		g_TreeView = new CTreeView;
		g_TreeView->Create(hWnd, IDC_TREEVIEW, hInst, main.right / 3, main.bottom);
		g_TreeView->LoadMyComputer(Cdr);
		g_TreeView->GetFocus();
		
		//
		Clv.Create(hWnd, IDC_LISTVIEW, hInst, (main.right - main.left) * 2 / 3 + 1, main.bottom, main.right / 3);
		Clv.LoadMyComputer(Cdr); // Lỗi ch load hết dc listview
		break;
	}
	////////////////
	case WM_HOTKEY:
	{
		if ((int)wParam == CTRL_C)
			DoCopy();
		if ((int)wParam == CTRL_V)
			DoPaste();
		if ((int)wParam == CTRL_X)
			DoCut();
		if ((int)wParam == CTRL_N)
			CreateNewFolder();
		break;
	}

	case WM_NOTIFY:
	{
		pnm = (LPNMHDR)lParam;
		int nCurSelIndex;
		LPNMTOOLBAR lpnmToolBar = (LPNMTOOLBAR)pnm;
		LPNMTREEVIEW lpnmTree = (LPNMTREEVIEW)pnm;

		switch (pnm->code)
		{
		case TVN_ITEMEXPANDING:
			g_TreeView->PreloadExpanding(lpnmTree->itemOld.hItem, lpnmTree->itemNew.hItem);
			break;
			//------------------------------------------------------------------------------
		case TVN_SELCHANGED:
			g_TreeView->Expand(g_TreeView->GetCurSel());
			Clv.DeleteAll(); //Xóa sạch List View để nạp cái mới
			Clv.LoadChild(g_TreeView->GetCurPath(), Cdr);
			
			g_History->InsertAfterCur(g_TreeView->GetCurPath());
			if (g_History->GetCur()->GetPrev() != NULL)
			{
			g_ToolBar->EnableBack(TRUE);
			}
			break;
			//------------------------------------------------------------------------------
		case NM_CLICK:
			if (pnm->hwndFrom == Clv.GetHandle())
			{
				nCurSelIndex = ListView_GetNextItem(Clv.GetHandle(), -1, LVNI_FOCUSED);
				if (nCurSelIndex != -1)
					Clv.DisplayInfoCurSel();
			}
			else
				switch (pnm->idFrom)
			{
				
				case IDC_TOOLBAR:
				switch (lpnmToolBar->iItem)
				{
					case IDC_TOOLBAR_COPY:
						DoCopy();
						break;
					case IDC_TOOLBAR_CUT:
						DoCut();
						break;
					case IDC_TOOLBAR_PASTE:
						DoPaste();
						break;
					case IDC_TOOLBAR_DELETE:
						DoDelete();
						break;
						
					
					case IDC_TOOLBAR_UP:
					DoGoUp();
					break;
					case IDC_TOOLBAR_BACK:
					DoGoBack();
					break;
					case IDC_TOOLBAR_FORWARD:
					DoGoForward();
					break;
					
				}
				break; //Case IDC_TOOLBAR
				//-----------------------------------------------------------------------
				case IDC_ADDRESS:
				if (lpnmToolBar->iItem == IDC_ADDRESS_GO)
				DoGo();
				break;
			}//switch (pnm->idFrom)
			break; //case NM_CLICK:

			//------------------------------------------------------------------------------
		case NM_DBLCLK:
		{
			int iSelected = 0;
			if (pnm->hwndFrom == Clv.GetHandle())
			{
				iSelected = ListView_GetNextItem(Clv.GetHandle(), -1, LVNI_SELECTED); // ĐIều kiện click
				if (iSelected != -1)
					Clv.LoadCurSel();
				break;
			}
		}
			//------------------------------------------------------------------------------
			case NM_CUSTOMDRAW: //Ve lai cua so con
			if (pnm->hwndFrom == g_TreeView->GetHandle())
			DoSizeTreeView();
			break;
			//------------------------------------------------------------------------------
			case TBN_DROPDOWN:
			if (lpnmToolBar->iItem == IDC_TOOLBAR_VIEW)
			DoViewChange(lpnmToolBar);
			break;
			
		}break;

	}
	case WM_SIZE:
		g_TreeView->Size(lParam);
		GetWindowRect(g_TreeView->GetHandle(), &g_TreeViewRect); //Cap nhat lai cho TreeView

		Clv.Size();
		g_Status->Size();

		SendMessage(g_ToolBar->GetHandle(), TB_AUTOSIZE, 0, (LPARAM)0);

		SendMessage(g_Address->GetHandle(), TB_AUTOSIZE, 0, (LPARAM)0);
		g_Address->Size();
		break;
///////////////////////////////
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		///////////////////////////////
			// lỗi k load hết dc program file
		case ID_VIEW_DETAILS:
			Clv.ChangeViewOption(1);
			break;
		case ID_VIEW_ICONS:
			Clv.ChangeViewOption(0);
			break;
		//////////////////////////////
		case IDM_ABOUT:
		{
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		}
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here...
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}


void DoViewChange(LPNMTOOLBAR lpnmToolBar)
{
	HMENU     hPopupMenu = NULL;
	HMENU     hMenuLoaded;
	RECT rc;
	TPMPARAMS tpm;

	SendMessage(lpnmToolBar->hdr.hwndFrom, TB_GETRECT,
		(WPARAM)lpnmToolBar->iItem, (LPARAM)&rc);
	MapWindowPoints(lpnmToolBar->hdr.hwndFrom,
		HWND_DESKTOP, (LPPOINT)&rc, 2);
	tpm.cbSize = sizeof(TPMPARAMS);
	tpm.rcExclude = rc;

	hMenuLoaded = LoadMenu(hInst, MAKEINTRESOURCE(IDR_MENU3));

	hPopupMenu = GetSubMenu(hMenuLoaded, 0);

	TrackPopupMenuEx(hPopupMenu,
		TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_VERTICAL,
		rc.left, rc.bottom, g_ToolBar->GetHandle(), &tpm);

	DestroyMenu(hMenuLoaded);
}

void DoSizeTreeView()
{
	RECT newTreeRC;
	GetClientRect(g_TreeView->GetHandle(), &newTreeRC);

	if (newTreeRC.right != g_TreeViewRect.right)
	{
		Clv.Size();
		g_TreeViewRect = newTreeRC;
	}
}

void DoCopy()
{
	listFile = Clv.GetListPath(); // List các file dc chọn để copy
	checkEvent = 1;
}

void DoCut()
{
	listFile = Clv.GetListPath(); // List các file dc chọn để copy
	checkEvent = 2;
}


void DoPaste()
{
	//Lấy đường dẫn cần paste tới
	TCHAR dest[10240];
	StrCpy(dest, Clv.GetCurParentPath()); //folder trống là ăn gạch ->> ham Parent sida
	if (wcslen(dest) < 3)
		StrCat(dest, _T("\\"));
	//Xử lí đường dẫn file
	TCHAR destTmp[10240];
	for (int i = 0; i < listFile.size(); i++)
	{
		StrCpy(destTmp, dest);
		//Lấy tên file
		TCHAR *parent;
		int nBackSlachPos = (StrRStrI(listFile[i], NULL, _T("\\")) - listFile[i]);
		parent = new TCHAR[wcslen(listFile[i])];
		StrNCpy(parent, listFile[i] + nBackSlachPos + 1, wcslen(listFile[i]));
		
		StrCat(destTmp, _T("\\"));
		StrCat(destTmp, parent);
		/// Thêm Đk kiểm tra
		/// thêm hook
		if (GetLastError() == ERROR_ALREADY_EXISTS)
		{
			MessageBox(hWnd, _T("Thư mục đã tồn tại!"), _T("Lỗi"), NULL);
		}
		if (checkEvent == 1)
		{
			CopyFile(listFile[i], destTmp, NULL);
		}
		if (checkEvent == 2)
		{
			MoveFile(listFile[i], destTmp);
		}
		//MessageBox(hWnd, destTmp, NULL, NULL);
	}
	listFile.clear();
	DoRefresh();
	
}

void DoDelete()
{
	listFile = Clv.GetListPath();
	for (int i = 0; i < listFile.size(); i++)
	{
		DeleteFile(listFile[i]);
	}
	listFile.clear();
	DoRefresh();
}


void CreateNewFolder()
{
	TCHAR pathtmp[10240];
	StrCpy(pathtmp, Clv.GetCurParentPath());
	if (wcslen(pathtmp) != 3)
	{
		StrCat(pathtmp, _T("\\"));
	}
	StrCat(pathtmp, _T("New folder"));
	if (CreateDirectory(pathtmp, NULL) == 0 || GetLastError() == ERROR_ALREADY_EXISTS)
	{
		MessageBox(hWnd, _T("Không thể tạo thư mục!"), _T("Lỗi"), NULL);
	}
	else
	{
		MessageBox(hWnd, _T("Tạo thành công!"), _T("Thông báo"), NULL);
	}
	DoRefresh();	//Refresh. Đúng. Nhưng hàm parent sida

	// Cho nhập tên ...
}

void DoRefresh()
{
	Clv.LoadFileAndFolder(Clv.GetCurParentPath());
}


void DoGoForward()
{
	g_History->GoForward();
	g_ToolBar->EnableBack(TRUE);

	if (g_History->GetCur()->GetNext() == NULL) //Không thể forward nữa
		g_ToolBar->EnableForward(FALSE);

	Clv.LoadChild(g_History->GetCur()->GetPath(), Cdr);
}

void DoGoBack()
{
	g_History->GoBack();
	g_ToolBar->EnableForward(TRUE);

	if (g_History->GetCur()->GetPrev() == NULL) //Không thể Back nữa
		g_ToolBar->EnableBack(FALSE);

	Clv.LoadChild(g_History->GetCur()->GetPath(), Cdr);
}

void DoGoUp()
{
	Clv.Up(Cdr);
	g_History->InsertAfterCur(Clv.GetCurParentPath());
	g_ToolBar->EnableBack(TRUE);
}

void DoGo()
{
	TCHAR *buffer = new TCHAR[10240];
	GetDlgItemText(g_Address->GetHandle(), IDC_ADDRESS_EDIT, buffer, 10240);
	Clv.LoadChild(buffer, Cdr);
}