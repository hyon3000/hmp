#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// csettings3 대화 상자입니다.
extern int speedupdate;
extern double spd2;
class csettings3 : public CDialogEx
{
	DECLARE_DYNAMIC(csettings3)

public:
	csettings3(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~csettings3();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClickedD1Pcdw();
	afx_msg void OnClickedD1Pcup();
	afx_msg void OnChangeD1Pc();
	afx_msg void OnChangeD1Pv();
	afx_msg void OnClickedD1Vcup();
	afx_msg void OnClickedD1Vcdw();
	afx_msg void OnChangeD1Vv();
	afx_msg void OnChangeD1Sc();
	afx_msg void OnChangeD1Vc();
//	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
//	virtual INT_PTR DoModal();
	CString cpc;
	CString cpv;
	CString csc;
	CString cvc;
	CString cvv;
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
//	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnBnClickedCancel();
//	virtual BOOL Create(LPCTSTR lpszTemplateName, CWnd* pParentWnd = NULL);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual INT_PTR DoModal();
	CString ist;
	afx_msg void OnClickedButton1();
	afx_msg void OnClickedCheck1();
	afx_msg void OnKillfocusD1Pv();
	afx_msg void OnKillfocusD1Pc();
	afx_msg void OnKillfocusD1Vc();
	afx_msg void OnKillfocusD1Vv();
	afx_msg void OnKillfocusD1Sc();
	afx_msg void OnStnClickedd1pit2();
	afx_msg void OnStnClickedd1spd3();
	afx_msg void OnBnClickedd1vcup9();
	afx_msg void OnChangeD1Pv2();
	afx_msg void OnKillfocusD1Pv2();
	CString cpv2;
	afx_msg void OnChangeVolMaster();
	//afx_msg void OnChangeVolPan();
	CString cvv_mast;
	CString cvv_pan;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnChangeD1Sc2();
	CString cposit;
	afx_msg void OnKillfocusD1Sc2();
	afx_msg void OnBnClickedoks();
	BOOL _oks;
	CString cvvbass;
	CString cvvmid;
	CString cvvtrib;
	CString cvcbass;
	CString cvcmid;
	CString cvctrib;
	afx_msg void OnChangeD1Vcbass();
	afx_msg void OnChangeD1Vcmid();
	afx_msg void OnChangeD1Vctrib();
	afx_msg void OnChangeD1Vvbass();
	afx_msg void OnChangeD1Vvmid();
	afx_msg void OnChangeD1Vvtrib();
	afx_msg void OnClickedD1VcupBass();
	afx_msg void OnClickedD1VcupMid();
	afx_msg void OnClickedD1VcupTrib();
	afx_msg void OnClickedD1VcdwBass();
	afx_msg void OnClickedD1VcdwMid();
	afx_msg void OnClickedD1VcdwTrib();
//	int sldr1;
	CSliderCtrl sldrctrl;
	afx_msg void OnReleasedcaptureSlider1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnSelchangeCombo2();
//	int civ;
	CString cbc;
	CString cbv;
	CString ccc;
	CString ccv;
	CString cic;
	CString cmc;
	CString cmv;
	CString crc;
	CString crv;
	CString cev1;
	CString cev2;
	CString cev3;
	afx_msg void OnClickedD1Bcup();
	afx_msg void OnClickedD1Ccup();
	afx_msg void OnClickedD1Icup();
	afx_msg void OnClickedD1Mcup();
	afx_msg void OnClickedD1Rcup();
	afx_msg void OnClickedD1Bcdw();
	afx_msg void OnClickedD1Ccdw();
	afx_msg void OnClickedD1Icdw();
	afx_msg void OnClickedD1Mcdw();
	afx_msg void OnClickedD1Rcdw();
	afx_msg void OnChangeD1Bc();
//	afx_msg void OnChangeD1Bv();
//	afx_msg void OnKillfocusD1Bc();
	afx_msg void OnKillfocusD1Bv();
	afx_msg void OnChangeD1Cc();
	afx_msg void OnChangeD1Cv();
	afx_msg void OnChangeD1Ic();
	afx_msg void OnChangeD1Mc();
	afx_msg void OnChangeD1Mv();
	afx_msg void OnChangeD1Rc();
	afx_msg void OnChangeD1Rv();
	afx_msg void OnClickedSnd();
	afx_msg void OnKillfocusCombo2();
	CComboBox _civ;

	//afx_msg void OnClickedButton2();
	afx_msg void OnDestroy();
	virtual BOOL DestroyWindow();
};
