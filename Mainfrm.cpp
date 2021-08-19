// mainfrm.cpp : implementation of the CMainFrame class
//
// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.

//#include <windows.h>
#include "stdafx.h"
#include "cube.h"
#include "cubedoc.h"
#include "cubeview.h"
#include "mainfrm.h"
#include "extern.h"
#include <mmsystem.h>
#pragma comment(lib,"winmm")
#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif
char recentfilelist[14][10000];
bool kj768,ghkj6,j5j58,midiout = false, tfnm=1;
int h54ui=0,jhj56=2,j65j8=100;

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_PALETTECHANGED()
	ON_WM_QUERYNEWPALETTE()
	//}}AFX_MSG_MAP
	ON_WM_GETMINMAXINFO()
	ON_COMMAND(ID_HELP_GETCURRENTVERSIONOFFIRMWARE, &CMainFrame::OnHelpGetcurrentversionoffirmware)
	ON_COMMAND(ID_HELP_VIEWREADME, &CMainFrame::OnHelpViewreadme)
	ON_WM_MENUSELECT()
//	ON_COMMAND(ID_SETTINGS_VIEWSETTINGS, &CMainFrame::OnSettingsViewsettings)
ON_WM_MOVE()
ON_WM_MOVING()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// arrays of IDs used to initialize control bars

// toolbar buttons - IDs are command buttons

static unsigned int indicators[] = { ID_SEPARATOR };
/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here

}

CMainFrame::~CMainFrame()
{
}
bool sq1 = 0;

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	printf("B");

	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable
	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
			sizeof(indicators) / sizeof(UINT)))
	{
		TRACE0("���� ǥ������ ������ ���߽��ϴ�.\n");
		return -1;      // ������ ���߽��ϴ�.
	}
	printf("C");
	FILE *ini;
	char stf[1000];

	GetModuleFileNameA(NULL, stf, 1000);
	for (int i = strlen(stf) - 1; i >= 0; i--)
		if (stf[i] == '\\') { stf[i] = 0; break; }
	strcat(stf, "\\hmp6.ini");
	if ((ini = fopen(stf, "r")) != NULL) {
		printf("D");
		int cx, cy,windx,windy;
		char iafs[1000];
		fscanf(ini, " %[^\n]", iafs); fscanf(ini, " %[^\n]", iafs); fscanf(ini, " %[^\n]", iafs);
		fscanf(ini, " %[^\n]", iafs); fscanf(ini, " %[^\n]", iafs); fscanf(ini, " %[^\n]", iafs);
		fscanf(ini, " %[^\n]", iafs); fscanf(ini, " %[^\n]", iafs); fscanf(ini, " %[^\n]", iafs);
		fscanf(ini, " %[^\n]", iafs); fscanf(ini, " %[^\n]", iafs); fscanf(ini, " %[^\n]", iafs);
		printf("%x", ftell(ini)); fscanf(ini, "%d", &windx); fscanf(ini, "%d", &windy);
		fscanf(ini, "%d", &cx); fscanf(ini, "%d", &cy);
		fclose(ini);
		printf(" @%d %d!\n", cx, cy);
		printf("E");
		SetWindowPos(NULL, windx, windy, cx, cy,   SWP_NOACTIVATE | SWP_NOZORDER);
		printf("F");
	}sq1 = 1;
	printf("G");
	// TODO: Remove this if you don't want tool tips
	LANGID LangType = PRIMARYLANGID(GetSystemDefaultLangID());
	if (LangType == LANG_KOREAN) kj768 = true;
	CMenu* pMenu = GetMenu();
	CMenu* pMenu2=pMenu->GetSubMenu(0);
	HWAVEOUT device_;
	WAVEOUTCAPS pwoc;
	waveOutGetDevCaps((UINT)device_, &pwoc, sizeof(WAVEOUTCAPS));
	printf("H");
	if (pwoc.dwSupport & WAVECAPS_PITCH) {
		pMenu->ModifyMenuA(ID_PLAY_PITCHUP, MF_BYCOMMAND, ID_PLAY_PITCHUP, _T("Pitch Up(&-)"));
		pMenu->ModifyMenuA(ID_PLAY_PITCHDOWN, MF_BYCOMMAND, ID_PLAY_PITCHDOWN, _T("Pitch Down(&=)"));
		////printf("!!!!!!!!!!!!!!!!!!!!!!!!!");
	}

	if (kj768 == true) {
		pMenu->ModifyMenuA(ID_FILE_OPEN32773, MF_BYCOMMAND, ID_FILE_OPEN32773, _T("���� (&O)"));
		pMenu->ModifyMenuA(ID_FILE_RESET, MF_BYCOMMAND, ID_FILE_RESET, _T("�ݱ� (&C)"));
		pMenu->ModifyMenuA(ID_PLAY_PLAY, MF_BYCOMMAND, ID_PLAY_PLAY, _T("��� (&1)"));
		pMenu->ModifyMenuA(ID_PLAY_PAUSE, MF_BYCOMMAND, ID_PLAY_PAUSE, _T("�Ͻ����� (&2)"));
		pMenu->ModifyMenuA(ID_PLAY_STOP, MF_BYCOMMAND, ID_PLAY_STOP, _T("���� (&3)"));
		pMenu->ModifyMenuA(ID_PLAY_REPLAY, MF_BYCOMMAND, ID_PLAY_REPLAY, _T("�ٽ� ��� (&4)"));
		pMenu->ModifyMenuA(ID_PLAY_PITCHSPEED, MF_BYCOMMAND, ID_PLAY_PITCHSPEED, _T("����(Tab)"));
		
		pMenu->ModifyMenuA(ID_PLAY_VOLUMEUP, MF_BYCOMMAND, ID_PLAY_VOLUMEUP, _T("�Ҹ� Ű���(&5)"));
		pMenu->ModifyMenuA(ID_PLAY_VOLUMEDOWN, MF_BYCOMMAND, ID_PLAY_VOLUMEDOWN, _T("�Ҹ� ���̱�(&6)"));

		pMenu->ModifyMenuA(ID_PLAY_SKIPFORWARD, MF_BYCOMMAND, ID_PLAY_SKIPFORWARD, _T("������(&7)"));
		pMenu->ModifyMenuA(ID_PLAY_SKIPBACK, MF_BYCOMMAND, ID_PLAY_SKIPBACK, _T("�ڷ�(&8)"));
		pMenu->ModifyMenuA(ID_PLAY_PITCHUP, MF_BYCOMMAND, ID_PLAY_PITCHUP, _T("������ �ø���(&9)"));
		pMenu->ModifyMenuA(ID_PLAY_PITCHDOWN, MF_BYCOMMAND, ID_PLAY_PITCHDOWN, _T("������ ������(&0)"));

		pMenu->ModifyMenuA(ID_PLAY_SPEEDUP, MF_BYCOMMAND, ID_PLAY_SPEEDUP, _T("������(&-)"));
		pMenu->ModifyMenuA(ID_PLAY_SPEEDDOWN, MF_BYCOMMAND, ID_PLAY_SPEEDDOWN, _T("������(&=)"));
		pMenu->ModifyMenuA(ID_PLAY_RESET, MF_BYCOMMAND, ID_PLAY_RESET, _T("�ʱ�ȭ"));
		pMenu2->ModifyMenuA(2, MF_BYPOSITION, 2, "�ֱٿ� �� ���� (&R)");
		pMenu->ModifyMenuA(ID_APP_EXIT, MF_BYCOMMAND, ID_APP_EXIT, _T("���� (&E)"));
		pMenu->ModifyMenuA(ID_VIEW_SETBACKGROUNDIMAGE, MF_BYCOMMAND, ID_VIEW_SETBACKGROUNDIMAGE, _T("���ȭ�� �ҷ�����(&I)"));
		pMenu->ModifyMenuA(ID_VIEW_LOADSOUNDFONT, MF_BYCOMMAND, ID_VIEW_LOADSOUNDFONT, _T("���� (&E)"));
		pMenu->ModifyMenuA(ID_SETTINGS_VIEWSETTINGS, MF_BYCOMMAND, ID_SETTINGS_VIEWSETTINGS, _T("�׷��� (&G)"));
		pMenu->ModifyMenuA(ID_HELP_VIEWREADME, MF_BYCOMMAND, ID_HELP_VIEWREADME, _T("��뼳�� ���� (&V)"));
		pMenu->ModifyMenuA(ID_HELP_GETCURRENTVERSIONOFFIRMWARE, MF_BYCOMMAND, ID_HELP_GETCURRENTVERSIONOFFIRMWARE, _T("������ ������ (&i)"));
		pMenu->ModifyMenuA(0x3, MF_BYPOSITION, 0x3, _T("����(&H)"));
		pMenu->ModifyMenuA(0x2, MF_BYPOSITION, 0x2, _T("���� (&S)"));
		pMenu->ModifyMenuA(0x1, MF_BYPOSITION, 0x1, _T("��� (&P)"));
		pMenu->ModifyMenuA(0x0, MF_BYPOSITION, 0x0, _T("���� (&F)"));
		pMenu->ModifyMenuA(ID_RECENT_CLEARALL, MF_BYCOMMAND, ID_RECENT_CLEARALL, _T("����� (&C)"));
		pMenu->ModifyMenuA(ID_HELP_GETCURRENTVERSIONOFFIRMWARE, MF_BYCOMMAND, ID_HELP_GETCURRENTVERSIONOFFIRMWARE, _T("������ ������ (&i)"));
		pMenu->ModifyMenuA(ID_FILE_UNINSTALL, MF_BYCOMMAND, ID_FILE_UNINSTALL, _T("���α׷� ����"));
	}
	
	return 0;
}


/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

void CMainFrame::OnPaletteChanged(CWnd* pFocusWnd)
{
	CFrameWnd::OnPaletteChanged(pFocusWnd);

	if(pFocusWnd != this)
		OnQueryNewPalette();
}

BOOL CMainFrame::OnQueryNewPalette()
{
	int        i;
	CPalette    *pOldPal;
	CCubeView   *pView = (CCubeView *)GetActiveView();
	CClientDC   dc(pView);


	pOldPal = dc.SelectPalette(&pView->m_cPalette, FALSE);
	i = dc.RealizePalette();
	dc.SelectPalette(pOldPal, FALSE);

	if(i > 0)
		InvalidateRect(NULL);





	return CFrameWnd::OnQueryNewPalette();
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// remove this flag to remove " - Untitled" from the frame's caption

	cs.style &= ~ FWS_ADDTOTITLE;

	return CFrameWnd::PreCreateWindow(cs);
}


void CMainFrame::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.


	CFrameWnd::OnGetMinMaxInfo(lpMMI);
}


void CMainFrame::OnHelpGetcurrentversionoffirmware()
{
	ShellExecute(NULL, "open", "http://cielab.uy.to", NULL, NULL, SW_SHOW);
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
}


void CMainFrame::OnHelpViewreadme()
{
	
}


void CMainFrame::OnMenuSelect(UINT nItemID, UINT nFlags, HMENU hSysMenu)
{
	CFrameWnd::OnMenuSelect(nItemID, nFlags, hSysMenu);
	CMenu* pMenu = GetMenu();
	HWAVEOUT device_;
	WAVEOUTCAPS pwoc;
	waveOutGetDevCaps((UINT)device_, &pwoc, sizeof(WAVEOUTCAPS));

	char t[10000];
	for (int i = 0; i < 10; i++) {
		if (strcmp(recentfilelist[i], "\0") != 0) {
			sprintf(t, "&%d:%s", i, recentfilelist[i]);
			////printf(t);
			
			pMenu->ModifyMenuA(ID_RECENT_11 + i, MF_BYCOMMAND, ID_RECENT_11 + i, t); 
			pMenu->EnableMenuItem(ID_RECENT_11 + i, MF_ENABLED);
		}
		else {
			sprintf(t, "&%d:", i);
			
			pMenu->ModifyMenuA(ID_RECENT_11 + i, MF_BYCOMMAND, ID_RECENT_11 + i, t);
			pMenu->EnableMenuItem(ID_RECENT_11 + i, MF_GRAYED);
		}
	}
	pMenu->EnableMenuItem(ID_RECENT_CLEARALL, MF_ENABLED);
	if (g3446 == false) {
		pMenu->EnableMenuItem(ID_VIEW_LOADSOUNDFONT, MF_GRAYED);
	}
	if (g3446 == false && !midiout) {
		
		pMenu->EnableMenuItem(ID_PLAY_PITCHUP, MF_ENABLED);
		pMenu->EnableMenuItem(ID_PLAY_PITCHDOWN, MF_ENABLED);
		pMenu->EnableMenuItem(ID_PLAY_SPEEDUP, MF_ENABLED);
		pMenu->EnableMenuItem(ID_PLAY_SPEEDDOWN, MF_ENABLED);

	}
	else {
		
		pMenu->EnableMenuItem(ID_PLAY_PITCHUP, MF_GRAYED);
		pMenu->EnableMenuItem(ID_PLAY_PITCHDOWN, MF_GRAYED);
		pMenu->EnableMenuItem(ID_PLAY_SPEEDUP, MF_GRAYED);
		pMenu->EnableMenuItem(ID_PLAY_SPEEDDOWN, MF_GRAYED);
	}
	if(monce&&mode!=14)pMenu->EnableMenuItem(ID_PLAY_PITCHSPEED, MF_ENABLED);
	else pMenu->EnableMenuItem(ID_PLAY_PITCHSPEED, MF_GRAYED);
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
}


//void CMainFrame::OnSettingsViewsettings()
//{
//	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
//
//}


void CMainFrame::OnMove(int x, int y)
{
	CFrameWnd::OnMove(x, y);
	UpdateData(1);
	//MessageBox("A", "A", MB_OK);
	if (sq1) {
		RECT rect;
		GetWindowRect(&rect);
		windv = rect.bottom - rect.top;
		windh = rect.right - rect.left;
		windowx = rect.left;
		windowy = rect.top;
		printf("%^@#@^%d %d\n", windv, windh);
		putini();
	}
	UpdateData(0);
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
}


void CMainFrame::OnMoving(UINT fwSide, LPRECT pRect)
{
	CFrameWnd::OnMoving(fwSide, pRect);
	UpdateData(1);
	//MessageBox("A", "A", MB_OK);
	if (sq1) {
		RECT rect;
		
		GetWindowRect(&rect);
		windv = rect.bottom - rect.top;
		windh = rect.right - rect.left;
		windowx = rect.left;
		windowy = rect.top;
		printf("%^@#@^%d %d\n", windv, windh);
		putini();
	}
	UpdateData(0);
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
}
