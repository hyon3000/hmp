// csettings4.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Cube.h"
#include "csettings4.h"
#include "afxdialogex.h"
#include "extern.h"

// csettings4 ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(csettings4, CDialogEx)

csettings4::csettings4(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG2, pParent)
	, ht(_T(""))
{

}

csettings4::~csettings4()
{
}

void csettings4::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, ht);
}


BEGIN_MESSAGE_MAP(csettings4, CDialogEx)
	ON_WM_CREATE()
END_MESSAGE_MAP()


// csettings4 �޽��� ó�����Դϴ�.


int csettings4::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;
	if (kj768 == true)ht = f2f23;
	else ht = f2f24;
	// TODO:  ���⿡ Ư��ȭ�� �ۼ� �ڵ带 �߰��մϴ�.

	return 0;
}
