#pragma once


// csettings4 대화 상자입니다.

class csettings4 : public CDialogEx
{
	DECLARE_DYNAMIC(csettings4)

public:
	csettings4(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~csettings4();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG2 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CString ht;
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};
