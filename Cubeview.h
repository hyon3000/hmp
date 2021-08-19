// cubeview.h : interface of the CCubeView class
//
// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.
#include "csettings.h"
#include "csettings2.h"
#include "csettings3.h"
#include "csettings4.h"

class CCubeView : public CView
{
	csettings st;
	csettings2 st2;
	csettings3 st3;
	
protected: // create from serialization only
	CCubeView();
	DECLARE_DYNCREATE(CCubeView)

// Attributes
public:
	CCubeDoc* GetDocument();

	CPalette    m_cPalette;
	CPalette    *m_pOldPalette;
	CRect       m_oldRect;
	float       m_fRadius;
	CClientDC   *m_pDC;
	
// Operations
public:
	inline void print(char* val);

	void Init();
	void CreateRGBPalette(void);
	BOOL bSetupPixelFormat(void);
	unsigned char ComponentFromIndex(int i, UINT nbits, UINT shift);
	//void DrawScene(void);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCubeView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCubeView();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CCubeView)
	afx_msg void OnFilePlay();
	afx_msg void OnUpdateFilePlay(CCmdUI* pCmdUI);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	inline void main2(char* argv);
	inline void mtinit();
	inline void arecloop();
	inline void DrawScene(void);
	afx_msg void OnDropFiles(HDROP hDropInfo);
	virtual void OnInitialUpdate();
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
//	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
//	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
//	afx_msg void OnMButtonDblClk(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnNcLButtonUp(UINT nHitTest, CPoint point);
	afx_msg void OnMouseLeave();
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
//	afx_msg void OnNcMButtonDblClk(UINT nHitTest, CPoint point);
	afx_msg void OnMButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnFileOpen32773();
	afx_msg void OnViewSetbackgroundimage();
	afx_msg void OnViewLoadsoundfont();
//	afx_msg void OnRecent1();
//	afx_msg void OnRecent10();
//	afx_msg void OnRecent2();
//	afx_msg void OnRecent3();
//	afx_msg void OnRecent4();
//	afx_msg void OnRecent5();
//	afx_msg void OnRecent6();
//	afx_msg void OnRecent7();
//	afx_msg void OnRecent8();
//	afx_msg void OnRecent9();
//	afx_msg void OnMenuSelect(UINT nItemID, UINT nFlags, HMENU hSysMenu);
//	afx_msg void OnMenuSelect(UINT nItemID, UINT nFlags, HMENU hSysMenu);
	afx_msg void OnRecent11();
	afx_msg void OnRecent12();
	afx_msg void OnRecent13();
	afx_msg void OnRecent14();
	afx_msg void OnRecent15();
	afx_msg void OnRecent16();
	afx_msg void OnRecent17();
	afx_msg void OnRecent18();
	afx_msg void OnRecent19();
	afx_msg void OnRecent20();
	afx_msg void OnFileReset();
	afx_msg void OnSettingsViewsettings();
	afx_msg void OnPlayPause();
	afx_msg void OnPlayPitchdown();
	afx_msg void OnPlayPitchup();
	afx_msg void OnPlayPlay();
	afx_msg void OnPlayReplay();
	afx_msg void OnPlaySkipback();
	afx_msg void OnPlaySkipforward();
	afx_msg void OnPlaySpeeddown();
	afx_msg void OnPlaySpeedup();
	afx_msg void OnPlayStop();
	afx_msg void OnPlayVolumedown();
	afx_msg void OnPlayVolumeup();
	afx_msg void OnPlayReset();
	afx_msg void OnPlayPitchspeed();
	afx_msg void OnHelpViewreadme();
	afx_msg void OnEditClearAll();
	afx_msg void OnRecentClearall();
	afx_msg void OnFileUninstall();
	afx_msg void OnMove(int x, int y);
	afx_msg void OnMoving(UINT fwSide, LPRECT pRect);
protected:
//	afx_msg LRESULT a(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnA(WPARAM wParam, LPARAM lParam);
};

#ifndef _DEBUG  // debug version in cubeview.cpp
inline CCubeDoc* CCubeView::GetDocument()
   { return (CCubeDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////
