#pragma once


// csettings4 ��ȭ �����Դϴ�.

class csettings4 : public CDialogEx
{
	DECLARE_DYNAMIC(csettings4)

public:
	csettings4(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~csettings4();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG2 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	CString ht;
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};
