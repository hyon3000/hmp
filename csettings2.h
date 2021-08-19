#pragma once
#include "afxwin.h"


// csettings2 대화 상자입니다.

class csettings2 : public CDialogEx
{
	DECLARE_DYNAMIC(csettings2)

public:
	csettings2(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~csettings2();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_viewsettings };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
//	CEdit eroll;
	CString croll;
	CString cyaw;
//	CEdit eyaw;
//	CEdit epit;
	CString cpit;
	afx_msg void OnKillfocusYaw();
	afx_msg void OnKillfocusPit();
	afx_msg void OnKillfocusRol();
//	virtual BOOL Create(LPCTSTR lpszTemplateName, CWnd* pParentWnd = NULL);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClickedPosup();
	afx_msg void OnClickedPosfront();
//	afx_msg void OnKillFocus(CWnd* pNewWnd);
//	afx_msg void OnSetFocus(CWnd* pOldWnd);
//	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
//	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
//	afx_msg void OnEnable(BOOL bEnable);
	CString cradi;
//	afx_msg void OnKillfocusRadi();
	afx_msg void OnClickedClrrandom();
//	BOOL clrrandom;
//	CString idc_clr_num;
	CString clrnum;
	CString cred;
	CString cgreen;
	CString cblue;
//	CEdit eblue;
//	CEdit egreen;
//	CEdit ered;
	BOOL clrrandom;
	CString clrsmp1;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnChangeClrNum();
	afx_msg void OnChangeRed();
	afx_msg void OnChangeGrn();
	afx_msg void OnChangeBlu();
	afx_msg void OnClickedClrUp();
	afx_msg void OnClickedClrDown();
//	afx_msg void OnChangeY();
//	afx_msg void OnChangeX();
	CString textx;
	CString texty;
//	afx_msg void OnSetfocusRed();
//	afx_msg void OnKillfocusRed();
//	afx_msg void OnKillfocusGrn();
//	afx_msg void OnSetfocusGrn();
//	afx_msg void OnSetfocusBlu();
//	afx_msg void OnKillfocusBlu();
//	afx_msg void OnSetfocusRadi();
//	afx_msg void OnChangeRadi();
	afx_msg void OnKillfocusX();
	afx_msg void OnSetfocusX();
	afx_msg void OnKillfocusY();
	afx_msg void OnSetfocusY();
	afx_msg void OnKillfocusRadi();
	afx_msg void OnSetfocusRadi();
	afx_msg void OnSetfocusYaw();
	afx_msg void OnSetfocusRol();
	afx_msg void OnSetfocusPit();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnClickedClrrandom2();
	BOOL clrrand;
	afx_msg void OnSetfocusRed2();
	afx_msg void OnKillfocusRed2();
	afx_msg void OnKillfocusGrn2();
	afx_msg void OnSetfocusGrn2();
	afx_msg void OnKillfocusBlu2();
	afx_msg void OnSetfocusBlu2();
	afx_msg void OnKillfocusA();
	afx_msg void OnSetfocusA();
	CString blu2;
//	CString red2;
	CString grn2;
	CString red2;
	CString a2;
	CEdit eblue;
	CEdit egreen;
	CEdit ered;
	afx_msg void OnRadioHor();
	afx_msg void OnRadioVer();
	afx_msg void OnClickedKeyname();
	afx_msg void OnClickedKeyfreq();
	BOOL kfreq;
	BOOL kname;
	BOOL extkpd;
	afx_msg void OnClickedExtkpd();
//	afx_msg void OnChangeRadi2();
//	afx_msg void OnChangeRadi3();
	CString radi2;
	CString radi3;
	afx_msg void OnSetfocusRadi2();
	afx_msg void OnKillfocusRadi2();
	afx_msg void OnSetfocusRadi3();
	afx_msg void OnKillfocusRadi3();
	CString txtfnt;
	afx_msg void OnKillfocusTxtfont();
	afx_msg void OnSetfocusTxtfont();
//	afx_msg void OnEnChangeTxtfont2();
	afx_msg void OnKillfocusTxtfont2();
	afx_msg void OnSetfocusTxtfont2();
	afx_msg void OnKillfocusTxtfont3();
	afx_msg void OnSetfocusTxtfont3();
	CString txtfnt2;
	CString txtfnt3;
	afx_msg void OnChangeBlu3();
//	CEdit eblue2;
	CEdit etra;
	CString ctra;
	afx_msg void OnRadioVer2();
	afx_msg void OnKillfocusRadi4();
	afx_msg void OnSetfocusRadi4();
	afx_msg void OnKillfocusRadi5();
	afx_msg void OnSetfocusRadi5();
	afx_msg void OnKillfocusRadi6();
	afx_msg void OnSetfocusRadi6();
	CString radi4;
	CString radi5;
	CString radi6;
	afx_msg void OnChangeRadi4();
	afx_msg void OnChangeRadi5();
	afx_msg void OnChangeRadi6();
	afx_msg void OnChangeRadi7();
	afx_msg void OnChangeRadi8();
	CString radi7;
	CString radi8;
	afx_msg void OnKillfocusRadi7();
	afx_msg void OnSetfocusRadi7();
	afx_msg void OnKillfocusRadi8();
	afx_msg void OnSetfocusRadi8();
	afx_msg void OnClickedCheck1();
	afx_msg void OnClickedCheck2();
//	afx_msg void OnStnClickedPos3();
	afx_msg void OnSelchangeCombo2();
	afx_msg void OnKillfocusCombo2();
	afx_msg void OnSetfocusCombo2();
};
