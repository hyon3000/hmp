#pragma once
#include "afxwin.h"


// csettings 대화 상자입니다.

class csettings : public CDialogEx
{
	DECLARE_DYNAMIC(csettings)

public:
	csettings(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~csettings();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_settings };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnClickedUsingpreview();
	virtual INT_PTR DoModal();
	afx_msg void OnClickedLoadsf();
	CString sfname;
	CString sprate;
	CString sfont;
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//afx_msg void OnClickedUsingsinewave();
	afx_msg void OnClickedErase();
	CButton btners;
	CButton btnldsf;
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnEqual();
	afx_msg void OnPyt();
	afx_msg void OnClickedAutomod();
	BOOL automod;
	afx_msg void OnSelchangeSampcombo();
//	CButton defmp;
	CString stemp;
	CButton btncc;
	CString vmode;
//	afx_msg void OnFb();
	//afx_msg void OnStf();
	//afx_msg void OnClickedUsingpreview();
//	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnClickedMrmode();
//	afx_msg void OnClickedRev();
	BOOL mrmode;
	BOOL rev;
//	afx_msg void OnRadioHor();
//	afx_msg void OnBnClickedequal2();
	afx_msg void OnJust();
	CString tfreq;
	CString tfval;
	afx_msg void OnKillfocusEdit1();
	afx_msg void OnChangeEdit1();
	CString tfwan;
//	afx_msg void OnClickedRev2();
	afx_msg void OnClickedRev();
//	CStatic mmps;
//	CString mmps;
//	afx_msg void OnHfbmps();
//	afx_msg void OnTim();
	afx_msg void OnStnClickedStaticSamprate3();
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedRadio3();
};
