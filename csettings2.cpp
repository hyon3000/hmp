// csettings2.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Cube.h"
#include "csettings2.h"
#include "afxdialogex.h"
#include "extern.h"
extern char p[100];
#define notusekeyname
// csettings2 ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(csettings2, CDialogEx)
bool focrd2 = false, focrd3 = false, focl=false,focr = false, focg = false, focb = false, focrd = false, focx = false, focy = false, focr2 = false, focg2 = false, focb2 = false, foca2 = false, foctf = false, foctf2 = false, foctf3 = false, foct = false,focxstr=false,focystr=false,foczstr=false,focrd7 = false, focrd8 = false;
csettings2::csettings2(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_viewsettings, pParent)
	, croll(_T(""))
	, cyaw(_T(""))
	, cpit(_T(""))
	, cradi(_T(""))
	, clrrandom(FALSE)
	//, idc_clr_num(_T(""))
	, clrnum(_T(""))
	, cred(_T(""))
	, cgreen(_T(""))
	, cblue(_T(""))
	//, clrrandom(FALSE)
	, clrsmp1(_T(""))
	, textx(_T(""))
	, texty(_T(""))
	, clrrand(FALSE)
	, blu2(_T(""))
	, grn2(_T(""))
	, red2(_T(""))
	, a2(_T(""))
	, kfreq(FALSE)
	, kname(FALSE)
	, extkpd(FALSE)
	, radi2(_T(""))
	, radi3(_T(""))
	, txtfnt(_T(""))
	, txtfnt2(_T(""))
	, txtfnt3(_T(""))
	, ctra(_T(""))
	, radi4(_T(""))
	, radi5(_T(""))
	, radi6(_T(""))
	, radi7(_T(""))
	, radi8(_T(""))
{

}

csettings2::~csettings2()
{
}

void csettings2::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//  DDX_Control(pDX, IDC_ROL, eroll);
	DDX_Text(pDX, IDC_ROL, croll);
	DDX_Text(pDX, IDC_YAW, cyaw);
	//  DDX_Control(pDX, IDC_YAW, eyaw);
	//  DDX_Control(pDX, IDC_PIT, epit);
	DDX_Text(pDX, IDC_PIT, cpit);
	DDX_Text(pDX, IDC_RADI, cradi);
	//  DDX_Check(pDX, IDC_clrrandom, clrrandom);

	//  DDX_Text(pDX, IDC_clr_num, idc_clr_num);
	DDX_Text(pDX, IDC_clr_num, clrnum);
	DDX_Text(pDX, IDC_red, cred);
	DDX_Text(pDX, IDC_grn, cgreen);
	DDX_Text(pDX, IDC_blu, cblue);
	//  DDX_Control(pDX, IDC_blu, eblue);
	//  DDX_Control(pDX, IDC_grn, egreen);
	//  DDX_Control(pDX, IDC_red, ered);
	DDX_Check(pDX, IDC_clrrandom, clrrandom);
	DDX_Text(pDX, IDC_CLR_SAMPLE, clrsmp1);
	DDX_Text(pDX, IDC_x, textx);
	DDX_Text(pDX, IDC_y, texty);
	DDX_Check(pDX, IDC_clrrandom2, clrrand);
	DDX_Text(pDX, IDC_blu2, blu2);
	//  DDX_Text(pDX, IDC_clr_red2, red2);
	DDX_Text(pDX, IDC_grn2, grn2);
	DDX_Text(pDX, IDC_red2, red2);
	DDX_Text(pDX, IDC_a, a2);
	DDX_Control(pDX, IDC_blu, eblue);
	DDX_Control(pDX, IDC_grn, egreen);
	DDX_Control(pDX, IDC_red, ered);
#ifndef notusekeyname
	DDX_Check(pDX, IDC_keyfreq, kfreq);
	DDX_Check(pDX, IDC_keyname, kname);
#endif
	////printf("%d ", hortxti);
	//	putini();
	DDX_Check(pDX, IDC_extkpd, extkpd);
	DDX_Text(pDX, IDC_RADI2, radi2);
	DDX_Text(pDX, IDC_RADI3, radi3);
	DDX_Text(pDX, IDC_TXTFONT, txtfnt);
	DDX_Text(pDX, IDC_TXTFONT2, txtfnt2);
	DDX_Text(pDX, IDC_TXTFONT3, txtfnt3);
	//  DDX_Control(pDX, IDC_blu3, eblue2);
	DDX_Control(pDX, IDC_blu3, etra);
	DDX_Text(pDX, IDC_blu3, ctra);
	DDX_Text(pDX, IDC_RADI4, radi4);
	DDX_Text(pDX, IDC_RADI5, radi5);
	DDX_Text(pDX, IDC_RADI6, radi6);
	DDX_Text(pDX, IDC_RADI7, radi7);
	DDX_Text(pDX, IDC_RADI8, radi8);
}


BEGIN_MESSAGE_MAP(csettings2, CDialogEx)
	ON_EN_KILLFOCUS(IDC_YAW, &csettings2::OnKillfocusYaw)
	ON_EN_KILLFOCUS(IDC_PIT, &csettings2::OnKillfocusPit)
	ON_EN_KILLFOCUS(IDC_ROL, &csettings2::OnKillfocusRol)
	ON_WM_SHOWWINDOW()
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_posup, &csettings2::OnClickedPosup)
	ON_BN_CLICKED(IDC_posfront, &csettings2::OnClickedPosfront)
//	ON_WM_KILLFOCUS()
//ON_WM_SETFOCUS()
ON_WM_LBUTTONDOWN()
//ON_WM_MOUSEACTIVATE()
//ON_WM_ENABLE()
//ON_EN_KILLFOCUS(IDC_RADI, &csettings2::OnKillfocusRadi)
ON_BN_CLICKED(IDC_clrrandom, &csettings2::OnClickedClrrandom)
ON_WM_TIMER()
ON_EN_CHANGE(IDC_clr_num, &csettings2::OnChangeClrNum)
ON_EN_CHANGE(IDC_red, &csettings2::OnChangeRed)
ON_EN_CHANGE(IDC_grn, &csettings2::OnChangeGrn)
ON_EN_CHANGE(IDC_blu, &csettings2::OnChangeBlu)
ON_BN_CLICKED(IDC_clr_up, &csettings2::OnClickedClrUp)
ON_BN_CLICKED(IDC_clr_down, &csettings2::OnClickedClrDown)
ON_EN_KILLFOCUS(IDC_x, &csettings2::OnKillfocusX)
ON_EN_SETFOCUS(IDC_x, &csettings2::OnSetfocusX)
ON_EN_KILLFOCUS(IDC_y, &csettings2::OnKillfocusY)
ON_EN_SETFOCUS(IDC_y, &csettings2::OnSetfocusY)
ON_EN_KILLFOCUS(IDC_RADI, &csettings2::OnKillfocusRadi)
ON_EN_SETFOCUS(IDC_RADI, &csettings2::OnSetfocusRadi)
ON_EN_SETFOCUS(IDC_YAW, &csettings2::OnSetfocusYaw)
ON_EN_SETFOCUS(IDC_ROL, &csettings2::OnSetfocusRol)
ON_EN_SETFOCUS(IDC_PIT, &csettings2::OnSetfocusPit)
ON_WM_LBUTTONDOWN()
ON_BN_CLICKED(IDC_clrrandom2, &csettings2::OnClickedClrrandom2)
ON_EN_SETFOCUS(IDC_red2, &csettings2::OnSetfocusRed2)
ON_EN_KILLFOCUS(IDC_red2, &csettings2::OnKillfocusRed2)
ON_EN_KILLFOCUS(IDC_grn2, &csettings2::OnKillfocusGrn2)
ON_EN_SETFOCUS(IDC_grn2, &csettings2::OnSetfocusGrn2)
ON_EN_KILLFOCUS(IDC_blu2, &csettings2::OnKillfocusBlu2)
ON_EN_SETFOCUS(IDC_blu2, &csettings2::OnSetfocusBlu2)
ON_EN_KILLFOCUS(IDC_a, &csettings2::OnKillfocusA)
ON_EN_SETFOCUS(IDC_a, &csettings2::OnSetfocusA)
ON_COMMAND(IDC_RADIO_hor, &csettings2::OnRadioHor)
ON_COMMAND(IDC_RADIO_ver, &csettings2::OnRadioVer)
#ifndef notusekeyname
ON_BN_CLICKED(IDC_keyname, &csettings2::OnClickedKeyname)
ON_BN_CLICKED(IDC_keyfreq, &csettings2::OnClickedKeyfreq)
#endif
ON_BN_CLICKED(IDC_extkpd, &csettings2::OnClickedExtkpd)
//ON_EN_CHANGE(IDC_RADI2, &csettings2::OnChangeRadi2)
//ON_EN_CHANGE(IDC_RADI3, &csettings2::OnChangeRadi3)
ON_EN_SETFOCUS(IDC_RADI2, &csettings2::OnSetfocusRadi2)
ON_EN_KILLFOCUS(IDC_RADI2, &csettings2::OnKillfocusRadi2)
ON_EN_SETFOCUS(IDC_RADI3, &csettings2::OnSetfocusRadi3)
ON_EN_KILLFOCUS(IDC_RADI3, &csettings2::OnKillfocusRadi3)
ON_EN_KILLFOCUS(IDC_TXTFONT, &csettings2::OnKillfocusTxtfont)
ON_EN_SETFOCUS(IDC_TXTFONT, &csettings2::OnSetfocusTxtfont)
//ON_EN_CHANGE(IDC_TXTFONT2, &csettings2::OnEnChangeTxtfont2)
ON_EN_KILLFOCUS(IDC_TXTFONT2, &csettings2::OnKillfocusTxtfont2)
ON_EN_SETFOCUS(IDC_TXTFONT2, &csettings2::OnSetfocusTxtfont2)
ON_EN_KILLFOCUS(IDC_TXTFONT3, &csettings2::OnKillfocusTxtfont3)
ON_EN_SETFOCUS(IDC_TXTFONT3, &csettings2::OnSetfocusTxtfont3)
ON_EN_CHANGE(IDC_blu3, &csettings2::OnChangeBlu3)
ON_COMMAND(IDC_RADIO_ver2, &csettings2::OnRadioVer2)
ON_EN_KILLFOCUS(IDC_RADI4, &csettings2::OnKillfocusRadi4)
ON_EN_SETFOCUS(IDC_RADI4, &csettings2::OnSetfocusRadi4)
ON_EN_KILLFOCUS(IDC_RADI5, &csettings2::OnKillfocusRadi5)
ON_EN_SETFOCUS(IDC_RADI5, &csettings2::OnSetfocusRadi5)
ON_EN_KILLFOCUS(IDC_RADI6, &csettings2::OnKillfocusRadi6)
ON_EN_SETFOCUS(IDC_RADI6, &csettings2::OnSetfocusRadi6)
ON_EN_CHANGE(IDC_RADI4, &csettings2::OnChangeRadi4)
ON_EN_CHANGE(IDC_RADI5, &csettings2::OnChangeRadi5)
ON_EN_CHANGE(IDC_RADI6, &csettings2::OnChangeRadi6)
ON_EN_CHANGE(IDC_RADI7, &csettings2::OnChangeRadi7)
ON_EN_CHANGE(IDC_RADI8, &csettings2::OnChangeRadi8)
ON_EN_KILLFOCUS(IDC_RADI7, &csettings2::OnKillfocusRadi7)
ON_EN_SETFOCUS(IDC_RADI7, &csettings2::OnSetfocusRadi7)
ON_EN_KILLFOCUS(IDC_RADI8, &csettings2::OnKillfocusRadi8)
ON_EN_SETFOCUS(IDC_RADI8, &csettings2::OnSetfocusRadi8)
ON_BN_CLICKED(IDC_CHECK1, &csettings2::OnClickedCheck1)
ON_BN_CLICKED(IDC_CHECK2, &csettings2::OnClickedCheck2)
//ON_STN_CLICKED(IDC_POS3, &csettings2::OnStnClickedPos3)
ON_CBN_SELCHANGE(IDC_COMBO2, &csettings2::OnSelchangeCombo2)
ON_CBN_KILLFOCUS(IDC_COMBO2, &csettings2::OnKillfocusCombo2)
ON_CBN_SETFOCUS(IDC_COMBO2, &csettings2::OnSetfocusCombo2)
END_MESSAGE_MAP()


// csettings2 �޽��� ó�����Դϴ�.





//BOOL csettings2::Create(LPCTSTR lpszTemplateName, CWnd* pParentWnd)
//{
//	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
//
//	return CDialogEx::Create(lpszTemplateName, pParentWnd);
//}


void csettings2::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);
	UpdateData(1);
	(j5gyj == 0) ? (clrrandom = true) : (clrrandom = false);
	(trand == 0) ? clrrand=false:clrrand=true;
	////printf("%d", j5gyj);
	kname = knamei;
	kfreq = kfreqi;
	extkpd = exbl; 
	LANGID LangType = PRIMARYLANGID(GetSystemDefaultLangID());
	if (LangType == LANG_KOREAN) kj768 = true;
	else kj768 = false;
	if (kj768 == true) {
		GetDlgItem(IDC_posup)->SetWindowText("����");
		GetDlgItem(IDC_posfront)->SetWindowText("�ո�");
		GetDlgItem(IDC_RADIO_hor)->SetWindowText("���η�");
		GetDlgItem(IDC_RADIO_ver)->SetWindowText("���η�");
		GetDlgItem(IDC_RADIO_ver2)->SetWindowText("�۾� ǥ���ٿ�");
		GetDlgItem(IDCANCEL)->SetWindowText("�ݱ�");
		GetDlgItem(IDC_clrrandom)->SetWindowText("���Ƿ�");
		GetDlgItem(IDC_clrrandom2)->SetWindowText("���Ƿ�");
#ifndef notusekeyname
		GetDlgItem(IDC_keyname)->SetWindowText("�ǹ��̸� ����");
		GetDlgItem(IDC_keyfreq)->SetWindowText("�ǹ����ļ� ����");
#endif

		GetDlgItem(IDC_extkpd)->SetWindowText("Ȯ�� �ǹ� ����");
		GetDlgItem(IDC_POS)->SetWindowText("���� ����");
		GetDlgItem(IDC_srol)->SetWindowText("������");
		GetDlgItem(IDC_spit)->SetWindowText("Ű����");
		GetDlgItem(IDC_syaw)->SetWindowText("������");
		GetDlgItem(IDC_srad2)->SetWindowText("������ x");
		GetDlgItem(IDC_srad3)->SetWindowText("������ y");
		GetDlgItem(IDC_srad)->SetWindowText("������ z");
		GetDlgItem(IDC_srad5)->SetWindowText("x �ø���");
		GetDlgItem(IDC_srad6)->SetWindowText("y �ø���");
		GetDlgItem(IDC_srad4)->SetWindowText("z �ø���");
		GetDlgItem(IDC_a1)->SetWindowText("�ǹ� ����");
		GetDlgItem(IDC_a2)->SetWindowText("��");
		GetDlgItem(IDC_a3)->SetWindowText("�Ʒ�");
		GetDlgItem(IDC_clr_snum)->SetWindowText("ä��\n*:��ü");
		GetDlgItem(IDC_clr_red)->SetWindowText("����");
		GetDlgItem(IDC_clr_grn)->SetWindowText("�ʷ�");
		GetDlgItem(IDC_clr_blu)->SetWindowText("�Ķ�");
		GetDlgItem(IDC_clr_blu3)->SetWindowText("����");
		GetDlgItem(IDC_POS2)->SetWindowText("�۾� ��ġ");
		GetDlgItem(IDC_tx)->SetWindowText("����(%)");
		GetDlgItem(IDC_ty)->SetWindowText("����(%)");
		GetDlgItem(IDC_color2)->SetWindowText("�۾� ����");
		GetDlgItem(IDC_clr_red2)->SetWindowText("����");
		GetDlgItem(IDC_clr_grn2)->SetWindowText("�ʷ�");
		GetDlgItem(IDC_clr_blu2)->SetWindowText("�Ķ�");
		GetDlgItem(IDC_clr_snum2)->SetWindowText("����");
		GetDlgItem(IDC_color)->SetWindowText("��� ����");
		GetDlgItem(IDC_txtmode)->SetWindowText("�۾� ����");
		GetDlgItem(IDC_color3)->SetWindowText("�۾�ü");
		GetDlgItem(IDC_color4)->SetWindowText("ũ��");
		GetDlgItem(IDC_color5)->SetWindowText("����");
		GetDlgItem(IDC_CHECK1)->SetWindowText("���ϰ�");
		GetDlgItem(IDC_CHECK2)->SetWindowText("�����");
		GetDlgItem(IDC_POS3)->SetWindowText("�۾� ����");
		
		SetWindowText("����");
	}
	/*
	if (j5gyj ==0) {

	ered.SetReadOnly(1);
	egreen.SetReadOnly(1);
	eblue.SetReadOnly(1);
	}
	else {
	ered.SetReadOnly(0);
	egreen.SetReadOnly(0);
	eblue.SetReadOnly(0);
	}
	*/
	////printf("%d ", hortxti);
	if (hortxti == 1) {
		((CButton*)GetDlgItem(IDC_RADIO_hor))->SetCheck(1); ((CButton*)GetDlgItem(IDC_RADIO_ver))->SetCheck(0); ((CButton*)GetDlgItem(IDC_RADIO_ver2))->SetCheck(0);
	}
	else if(hortxti==0){
		((CButton*)GetDlgItem(IDC_RADIO_hor))->SetCheck(0); ((CButton*)GetDlgItem(IDC_RADIO_ver))->SetCheck(1); ((CButton*)GetDlgItem(IDC_RADIO_ver2))->SetCheck(0);
	}
	else {
		((CButton*)GetDlgItem(IDC_RADIO_hor))->SetCheck(0); ((CButton*)GetDlgItem(IDC_RADIO_ver))->SetCheck(0); ((CButton*)GetDlgItem(IDC_RADIO_ver2))->SetCheck(1);
	}
	((CComboBox*)(GetDlgItem(IDC_COMBO2)))->SetCurSel(textmode);
	cred =int2charp((int)(erhwh[0] * 256));
	cgreen = int2charp((int)(iyybt[0] * 256));
	cblue = int2charp((int)(trity[0] * 256));
	ctra = int2charp((int)(sadae[0] * 256));
	clrnum = _T("1");
	radi7 = flt2charp(upbl);
	radi8 = flt2charp(dwbl);
	UpdateData(0);
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
}


int csettings2::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  ���⿡ Ư��ȭ�� �ۼ� �ڵ带 �߰��մϴ�.
	SetTimer(1, 20, 0);
	return 0;
}


void csettings2::OnClickedPosup()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(1);
	h45h7 = -90;
	gasfw = 0;
	hg4h5 = 0;
	croll = flt2charp(h45h7);
	cpit = flt2charp(gasfw);
	cyaw = flt2charp(hg4h5);
	cradi = flt2charp(j5jgj);
	radi2 = flt2charp(x);
	radi3 = flt2charp(y);
	radi4 = flt2charp(stretchz);
	radi5 = flt2charp(stretchx);
	radi6 = flt2charp(stretchy);
	red2 = int2charp((int)(tr * 256));
	grn2 = int2charp((int)(tg * 256));
	blu2 = int2charp((int)(tb * 256));
	a2 =int2charp((int)(ta * 256));
	UpdateData(0);
}


void csettings2::OnClickedPosfront()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(1);
	h45h7 = 0;
	gasfw = 0;
	hg4h5 = 0;
	croll = flt2charp(h45h7);
	cpit = flt2charp(gasfw);
	cyaw = flt2charp(hg4h5);
	cradi = flt2charp(j5jgj);
	radi2 = flt2charp(x);
	radi3 = flt2charp(y);
	radi4 = flt2charp(stretchz);
	radi5 = flt2charp(stretchx);
	radi6 = flt2charp(stretchy);
	red2 = int2charp((int)(tr * 256));
	grn2 = int2charp((int)(tg * 256));
	blu2 = int2charp((int)(tb * 256));
	a2 = int2charp((int)(ta * 256));
	UpdateData(0);
}


void csettings2::OnClickedClrrandom()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(1);
	(j5gyj == 0) ? (j5gyj = 1) : (j5gyj = 0);
	////printf("%d", j5gyj);
	/*
if (j5gyj ==0) {
		
		ered.SetReadOnly(1);
		egreen.SetReadOnly(1);
		eblue.SetReadOnly(1);
	}
	else {
		ered.SetReadOnly(0);
		egreen.SetReadOnly(0);
		eblue.SetReadOnly(0);
	}
	*/
	UpdateData(0);
}

void csettings2::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	UpdateData(1);
	{		CRect cr;
	CClientDC ccdc(GetDlgItem(IDC_CLR_SAMPLE));
	CStatic *cs = (CStatic*)GetDlgItem(IDC_CLR_SAMPLE);
	cs->GetClientRect(cr);
	ccdc.FillSolidRect(cr, RGB(((char)atoi(cred)), ((char)atoi(cgreen)), ((char)atoi(cblue))));
	cs->ValidateRect(cr); }
	{		CRect cr;
	CClientDC ccdc(GetDlgItem(IDC_CLR_SAMPLE2));
	CStatic *cs = (CStatic*)GetDlgItem(IDC_CLR_SAMPLE2);
	cs->GetClientRect(cr);
	ccdc.FillSolidRect(cr, RGB(((char)atoi(red2)), ((char)atoi(grn2)), ((char)atoi(blu2))));
	cs->ValidateRect(cr); }
	if (focrd2 == false)
		radi2 = flt2charp(x);
	if (focrd3 == false)
		radi3 = flt2charp(y);

	if (foczstr == false)
	radi4 = flt2charp(stretchz);
	if (focxstr == false)
	radi5 = flt2charp(stretchx);
	if (focystr == false)
	radi6 = flt2charp(stretchy);
	if (focrd7 == false)
		radi7 = flt2charp(upbl);
	if (focrd8 == false)
		radi8 = flt2charp(dwbl);
	if (focr == false) 
		croll = flt2charp(h45h7);
	if (focg == false) 
		cpit = flt2charp(gasfw);
	if (focb == false) 
		cyaw = flt2charp(hg4h5);
	if (focrd == false) 
		cradi = flt2charp(j5jgj);
	if (focx == false) 
		textx = flt2charp(((float)asfaq * 100 / (float)windx));
	if (focy == false) 
		texty =flt2charp(((float)brebe * 100 / (float)windy));
	if (focr2 == false) 
		red2 = int2charp((int)(tr * 256));
	if (focg2 == false) 
		grn2 = int2charp((int)(tg * 256));
	if (focb2 == false) 
		blu2 = int2charp((int)(tb * 256));
	if (foca2 == false) 
		a2 = int2charp((int)(ta * 256));
	if (foctf == false)
		txtfnt = recentfilelist[13];
	if (foctf2 == false)
		txtfnt2 =int2charp(fsize);
	if (foctf3 == false)
		txtfnt3 = int2charp(fspace);
	if(txtmd2&0x01)
		((CButton*)GetDlgItem(IDC_CHECK1))->SetCheck(1);
	else
		((CButton*)GetDlgItem(IDC_CHECK1))->SetCheck(0);
	if (txtmd2 & 0x02)
		((CButton*)GetDlgItem(IDC_CHECK2))->SetCheck(1);
	else
		((CButton*)GetDlgItem(IDC_CHECK2))->SetCheck(0);
	////printf("%d %d %d %d\n", asfaq, windx, brebe, windy);
	if(focl==false)((CComboBox*)(GetDlgItem(IDC_COMBO2)))->SetCurSel(textmode);
	UpdateData(0);
	CDialogEx::OnTimer(nIDEvent);
}
void csettings2::OnKillfocusYaw()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	hg4h5 = atof(cyaw); 
	focb = false;
	
}

void csettings2::OnKillfocusPit()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	gasfw = atof(cpit); 
	focg = false;
	
}


void csettings2::OnKillfocusRol()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	
	h45h7 = atof(croll);
	focr = false;
}

void csettings2::OnChangeClrNum()
{
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CDialogEx::OnInitDialog() �Լ��� ������ 
	//�ϰ� ����ũ�� OR �����Ͽ� ������ ENM_CHANGE �÷��׸� �����Ͽ� CRichEditCtrl().SetEventMask()�� ȣ������ ������
	// �� �˸� �޽����� ������ �ʽ��ϴ�.
	UpdateData(1); 
	if (strcmp(clrnum, "*") == 1) {
		char tmp[10];
		if (atoi(clrnum) > 16) clrnum = _T("16");
		if (atoi(clrnum) < 1) clrnum = _T("0");
		cred = int2charp((int)(erhwh[atoi(clrnum) - 1] * 256));
		cgreen = int2charp((int)(iyybt[atoi(clrnum) - 1] * 256));
		cblue = int2charp((int)(trity[atoi(clrnum) - 1] * 256));
		ctra = int2charp((int)(sadae[atoi(clrnum) - 1] * 256));
		//clrnum = _T("1");
	}
	else {
		float tmp=erhwh[0];
		int i;
		for (i = 1; i < 16; i++) if (tmp != erhwh[i]) {
			cred = "?"; break;
		}
		if(i==17) cred = int2charp((int)(tmp*256));

		tmp = iyybt[0];
		for (i = 1; i < 16; i++) if (tmp != iyybt[i]) {
			cgreen = "?"; break;
		}
		if (i == 17)cgreen = int2charp((int)(tmp * 256));

		tmp = trity[0];
		for ( i = 1; i <= 16; i++) if (tmp !=trity[i]) {
			cblue = "?"; break;
		}
		if (i == 17)cblue = int2charp((int)(tmp * 256));

		tmp = sadae[0];
		for ( i = 1; i <= 16; i++) if (tmp !=sadae[i]) {
			ctra = "?"; break;
		}
		if (i == 17)ctra = int2charp((int)(tmp * 256));

	}
	UpdateData(0);
	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void csettings2::OnChangeRed()
{
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CDialogEx::OnInitDialog() �Լ��� ������ 
	//�ϰ� ����ũ�� OR �����Ͽ� ������ ENM_CHANGE �÷��׸� �����Ͽ� CRichEditCtrl().SetEventMask()�� ȣ������ ������
	// �� �˸� �޽����� ������ �ʽ��ϴ�.
	UpdateData(1);
	char tmp[10];
	if (atoi(cred) > 255) cred = _T("255");
	if (atoi(cred) <0) cred = _T("0");
	if (strcmp(clrnum, "*") == 1)
	erhwh[atoi(clrnum) - 1] = atof(cred) / 256;
	else for(int i=0;i<16;i++)erhwh[i] = atof(cred) / 256;
	UpdateData(0);
	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void csettings2::OnChangeGrn()
{
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CDialogEx::OnInitDialog() �Լ��� ������ 
	//�ϰ� ����ũ�� OR �����Ͽ� ������ ENM_CHANGE �÷��׸� �����Ͽ� CRichEditCtrl().SetEventMask()�� ȣ������ ������
	// �� �˸� �޽����� ������ �ʽ��ϴ�.
	UpdateData(1);
	char tmp[10];
	if (atoi(cgreen) > 255) cgreen = _T("255");
	if (atoi(cgreen) <0) cgreen = _T("0");
	if (strcmp(clrnum, "*") == 1)
		iyybt[atoi(clrnum) - 1] = atof(cgreen) / 256;
	else for(int i=0;i<16;i++)iyybt[i] = atof(cgreen) / 256;
	UpdateData(0);
	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void csettings2::OnChangeBlu()
{
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CDialogEx::OnInitDialog() �Լ��� ������ 
	//�ϰ� ����ũ�� OR �����Ͽ� ������ ENM_CHANGE �÷��׸� �����Ͽ� CRichEditCtrl().SetEventMask()�� ȣ������ ������
	// �� �˸� �޽����� ������ �ʽ��ϴ�.
	UpdateData(1);
	char tmp[10];
	if (atoi(cblue) > 255) cblue = _T("255");
	if (atoi(cblue) <0) cblue = _T("0");
	if (strcmp(clrnum, "*") == 1)
		trity[atoi(clrnum) - 1] = atof(cblue) / 256;
	else for(int i=0;i<16;i++)trity[i] = atof(cblue) / 256;
	UpdateData(0);
	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void csettings2::OnClickedClrUp()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(1);
	int i = atoi(clrnum); i++;
	clrnum = int2charp(i);

	if (atoi(clrnum) > 16) clrnum = _T("16");
	if (atoi(clrnum) <=0) clrnum = _T("*");
	if (strcmp(clrnum, "*") == 1) {
		char tmp[10];
		if (atoi(clrnum) > 16) clrnum = _T("16");
		if (atoi(clrnum) < 1) clrnum = _T("0");
		cred = int2charp((int)(erhwh[atoi(clrnum) - 1] * 256));
		cgreen = int2charp((int)(iyybt[atoi(clrnum) - 1] * 256));
		cblue = int2charp((int)(trity[atoi(clrnum) - 1] * 256));
		ctra = int2charp((int)(sadae[atoi(clrnum) - 1] * 256));
		//clrnum = _T("1");
	}
	else {
		float tmp = erhwh[0];
		int i;
		for (i = 1; i < 16; i++) if (tmp != erhwh[i]) {
			cred = "?"; break;
		}
		if (i == 17) cred = int2charp((int)(tmp * 256));

		tmp = iyybt[0];
		for (i = 1; i < 16; i++) if (tmp != iyybt[i]) {
			cgreen = "?"; break;
		}
		if (i == 17)cgreen = int2charp((int)(tmp * 256));

		tmp = trity[0];
		for (i = 1; i < 16; i++) if (tmp != trity[i]) {
			cblue = "?"; break;
		}
		if (i == 17)cblue = int2charp((int)(tmp * 256));

		tmp = sadae[0];
		for (i = 1; i < 16; i++) if (tmp != sadae[i]) {
			ctra = "?"; break;
		}
		if (i == 17)ctra = int2charp((int)(tmp * 256));

	}
	//clrnum = _T("1");
	UpdateData(0);
}


void csettings2::OnClickedClrDown()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(1);
	int i = atoi(clrnum); i--;
	clrnum = int2charp(i);

	if (atoi(clrnum) > 16) clrnum = _T("16");
	if (atoi(clrnum) <= 0) clrnum = _T("*");
	if (strcmp(clrnum, "*") == 1) {
		char tmp[10];
		if (atoi(clrnum) > 16) clrnum = _T("16");
		if (atoi(clrnum) < 1) clrnum = _T("0");
		cred = int2charp((int)(erhwh[atoi(clrnum) - 1] * 256));
		cgreen = int2charp((int)(iyybt[atoi(clrnum) - 1] * 256));
		cblue = int2charp((int)(trity[atoi(clrnum) - 1] * 256));
		ctra = int2charp((int)(sadae[atoi(clrnum) - 1] * 256));
		//clrnum = _T("1");
	}
	else {
		float tmp = erhwh[0];
		int i;
		for (i = 1; i < 16; i++) if (tmp != erhwh[i]) {
			cred = "?"; break;
		}
		if (i == 17) cred = int2charp((int)(tmp * 256));

		tmp = iyybt[0];
		for (i = 1; i < 16; i++) if (tmp != iyybt[i]) {
			cgreen = "?"; break;
		}
		if (i == 17)cgreen = int2charp((int)(tmp * 256));

		tmp = trity[0];
		for (i = 1; i < 16; i++) if (tmp != trity[i]) {
			cblue = "?"; break;
		}
		if (i == 17)cblue = int2charp((int)(tmp * 256));

		tmp = sadae[0];
		for (i = 1; i < 16; i++) if (tmp != sadae[i]) {
			ctra = "?"; break;
		}
		if (i == 17)ctra = int2charp((int)(tmp * 256));

	}
	UpdateData(0);
}


void csettings2::OnKillfocusX()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if (atoi(textx) > 100) textx = _T("100.000000");
	if (atoi(textx) < 0) textx = _T("0.000000");
	if (windx != 0)
		asfaq = windx*(atof(textx) / 100);
	focx = false;
}

void csettings2::OnSetfocusX()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	focx = true;
}


void csettings2::OnKillfocusY()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	// �� �˸� �޽����� ������ �ʽ��ϴ�.
	if (atoi(texty) > 100) texty = _T("100.000000");
	if (atoi(texty) < 0) texty = _T("0.000000");
	if (windy != 0)
		brebe = windy * (atof(texty) / 100);
	focy = false;
}


void csettings2::OnSetfocusY()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	focy = true;
}


void csettings2::OnKillfocusRadi()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	j5jgj = atof(cradi);
	focrd = false;
	
}

void csettings2::OnSetfocusRadi()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	focrd = true;
}


void csettings2::OnSetfocusYaw()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	focb = true;
}


void csettings2::OnSetfocusRol()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	focr = true;
}


void csettings2::OnSetfocusPit()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	focg = true;
}


void csettings2::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	if (atoi(textx) > 100) textx = _T("100.000000");
	if (atoi(textx) < 0) textx = _T("0.000000");
	if (atoi(texty) > 100) texty = _T("100.000000");
	if (atoi(texty) < 0) texty = _T("0.000000");
	if (windx != 0)
		asfaq = windx*(atof(textx) / 100);
	if (windy != 0)
		brebe = windy*(atof(texty) / 100);
	j5jgj = atof(cradi);
	hg4h5 = atof(cyaw);
	gasfw = atof(cpit);
	h45h7 = atof(croll);
	if (atof(red2) > 255) red2 = _T("255");
	if (atof(red2) < 0) red2 = _T("0");
	tr = atof(red2) / 256;
	if (atof(grn2) > 255) grn2 = _T("255");
	if (atof(grn2) < 0) grn2 = _T("0");
	tg = atof(grn2) / 256;
	if (atof(blu2) > 255) blu2 = _T("255");
	if (atof(blu2) < 0) blu2 = _T("0");
	tb = atof(blu2) / 256;
	if (atof(a2) > 255) a2 = _T("255");
	if (atof(a2) < 0) a2 = _T("0");
	ta = atof(a2) / 256;
	focx = false;
	focy = false;
	focr = false;
	focg = false;
	focb = false;
	focrd = false;
	focrd2 = false;
	focrd3 = false;
	focr2 = false;
	focg2 = false;
	focb2 = false;
	foca2 = false;
	foctf = false;
	foctf2 = false;
	foctf3 = false;
	CDialogEx::OnLButtonDown(nFlags, point);
}


void csettings2::OnClickedClrrandom2()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if (trand == false) trand = true;
	else trand = false;
}


void csettings2::OnSetfocusRed2()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	focr2 = true;
}


void csettings2::OnKillfocusRed2()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
		if (atof(red2) > 255) red2 = _T("255");
	if (atof(red2) < 0) red2 = _T("0");
	tr = atof(red2) / 256;
	focr2 = false;
}

void csettings2::OnKillfocusGrn2()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if (atof(grn2) > 255) grn2 = _T("255");
	if (atof(grn2) < 0) grn2 = _T("0");
	tg = atof(grn2) / 256;
	focg2 = false;
}


void csettings2::OnSetfocusGrn2()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	focg2 = true;
}


void csettings2::OnKillfocusBlu2()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if (atof(blu2) > 255) blu2 = _T("255");
	if (atof(blu2) < 0) blu2 = _T("0");
	tb = atof(blu2) / 256;
	focb2 = false;
}


void csettings2::OnSetfocusBlu2()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	focb2 = true;
}


void csettings2::OnKillfocusA()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if (atof(a2) > 255) a2 = _T("255");
	if (atof(a2) < 0) a2 = _T("0");
	ta = atof(a2) / 256;
	foca2 = false;
}


void csettings2::OnSetfocusA()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	foca2 = true;
}


void csettings2::OnRadioHor()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	hortxti = 1;
	//printf("%d", hortxti);
}


void csettings2::OnRadioVer()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	hortxti = 0; //printf("%d", hortxti);
}


void csettings2::OnClickedKeyname()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	knamei == 0 ? knamei = 1 : knamei = 0;
}


void csettings2::OnClickedKeyfreq()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	kfreqi == 0 ? kfreqi = 1 : kfreqi = 0;

}



void csettings2::OnClickedExtkpd()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	exbl== 0 ? exbl = 1 : exbl = 0;
}


//void csettings2::OnChangeRadi2()
//{
//	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
//	// CDialogEx::OnInitDialog() �Լ��� ������ 
//	//�ϰ� ����ũ�� OR �����Ͽ� ������ ENM_CHANGE �÷��׸� �����Ͽ� CRichEditCtrl().SetEventMask()�� ȣ������ ������
//	// �� �˸� �޽����� ������ �ʽ��ϴ�.
//	
//	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
//}


//void csettings2::OnChangeRadi3()
//{
//	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
//	// CDialogEx::OnInitDialog() �Լ��� ������ 
//	//�ϰ� ����ũ�� OR �����Ͽ� ������ ENM_CHANGE �÷��׸� �����Ͽ� CRichEditCtrl().SetEventMask()�� ȣ������ ������
//	// �� �˸� �޽����� ������ �ʽ��ϴ�.
//	
//	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
//}


void csettings2::OnSetfocusRadi2()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	focrd2 = true;
}


void csettings2::OnKillfocusRadi2()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	x = atof(radi2); focrd2 = false;
}


void csettings2::OnSetfocusRadi3()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	focrd3 = true;
}


void csettings2::OnKillfocusRadi3()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	y = atof(radi3); focrd3 = false;
}


void csettings2::OnKillfocusTxtfont()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	foctf = false;
	strcpy(recentfilelist[13], txtfnt);
	bf = false;
}


void csettings2::OnSetfocusTxtfont()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	foctf = true;
}


//void csettings2::OnEnChangeTxtfont2()
//{
//	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
//	// CDialogEx::OnInitDialog() �Լ��� ������ 
//	//�ϰ� ����ũ�� OR �����Ͽ� ������ ENM_CHANGE �÷��׸� �����Ͽ� CRichEditCtrl().SetEventMask()�� ȣ������ ������
//	// �� �˸� �޽����� ������ �ʽ��ϴ�.
//
//	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
//}


void csettings2::OnKillfocusTxtfont2()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	foctf2= false;
	fsize = atoi(txtfnt2);
	bf = false;
}


void csettings2::OnSetfocusTxtfont2()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	foctf2 = true;
}


void csettings2::OnKillfocusTxtfont3()
{
	foctf3 = false;
	fspace = atoi(txtfnt3);
	bf = false;
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void csettings2::OnSetfocusTxtfont3()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	foctf3 = true;
}


void csettings2::OnChangeBlu3()
{
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CDialogEx::OnInitDialog() �Լ��� ������ 
	//�ϰ� ����ũ�� OR �����Ͽ� ������ ENM_CHANGE �÷��׸� �����Ͽ� CRichEditCtrl().SetEventMask()�� ȣ������ ������
	// �� �˸� �޽����� ������ �ʽ��ϴ�.
	UpdateData(1);
	char tmp[10];
	if (atoi(ctra) > 255) ctra = _T("255");
	if (atoi(ctra) <0) ctra = _T("0");
	if (strcmp(clrnum,"*")==1)
		sadae[atoi(clrnum) - 1] = atof(ctra) / 256;
	else for(int i=0;i<16;i++)sadae[i] = atof(ctra) / 256;
	UpdateData(0);
	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void csettings2::OnRadioVer2()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	hortxti = 2; //printf("%d", hortxti);
}


void csettings2::OnKillfocusRadi4()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if (atof(radi4) <= 0.001) {
		stretchz = 0.001;
		UpdateData(1);
		radi4 = "0.001";
		UpdateData(0);
	}
	else if (atof(radi4) >= 1000) {
		stretchz = 1000;
		UpdateData(1);
		radi4 = "1000";
		UpdateData(0);
	}
	else stretchz = atof(radi4);
	foczstr = false;
	


}


void csettings2::OnSetfocusRadi4()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	foczstr = true;

	
}


void csettings2::OnKillfocusRadi5()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if (atof(radi5) <= 0.001) {
		stretchx = 0.001;
		UpdateData(1);
		radi5 = "0.001";
		UpdateData(0);
	}
	else if (atof(radi5) >= 1000) {
		stretchx = 1000;
		UpdateData(1);
		radi5 = "1000";
		UpdateData(0);
	}
	else stretchx = atof(radi5);
	focxstr = false;

	
}


void csettings2::OnSetfocusRadi5()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	focxstr = true;
	
}


void csettings2::OnKillfocusRadi6()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if (atof(radi6) <= 0.001) {
		stretchy = 0.001;
		UpdateData(1);
		radi6 = "0.001";
		UpdateData(0);
	}
	else if (atof(radi6) >= 1000) {
		stretchy = 1000;
		UpdateData(1);
		radi6 = "1000";
		UpdateData(0);
	}
	else stretchy = atof(radi6);
	focystr = false;
}


void csettings2::OnSetfocusRadi6()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	focystr = true;

	
}


void csettings2::OnChangeRadi4()
{
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CDialogEx::OnInitDialog() �Լ��� ������ 
	//�ϰ� ����ũ�� OR �����Ͽ� ������ ENM_CHANGE �÷��׸� �����Ͽ� CRichEditCtrl().SetEventMask()�� ȣ������ ������
	// �� �˸� �޽����� ������ �ʽ��ϴ�.
	if (atof(radi4) <0) {
		UpdateData(1);
		radi4 = "0.001";
		UpdateData(0);
	}
	else if (atof(radi4) > 1000) {
		//printf("!");
		UpdateData(1);
		radi4 = "1000";
		UpdateData(0);
	}
	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void csettings2::OnChangeRadi5()
{
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CDialogEx::OnInitDialog() �Լ��� ������ 
	//�ϰ� ����ũ�� OR �����Ͽ� ������ ENM_CHANGE �÷��׸� �����Ͽ� CRichEditCtrl().SetEventMask()�� ȣ������ ������
	// �� �˸� �޽����� ������ �ʽ��ϴ�.
	if (atof(radi5) <0) {
		UpdateData(1);
		radi5 = "0.001";
		UpdateData(0);
	}
	else if (atof(radi5) > 1000) {
		UpdateData(1);
		radi5 = "1000";
		UpdateData(0);
	}
	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void csettings2::OnChangeRadi6()
{
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CDialogEx::OnInitDialog() �Լ��� ������ 
	//�ϰ� ����ũ�� OR �����Ͽ� ������ ENM_CHANGE �÷��׸� �����Ͽ� CRichEditCtrl().SetEventMask()�� ȣ������ ������
	// �� �˸� �޽����� ������ �ʽ��ϴ�.
	if (atof(radi6) <0) {
		UpdateData(1);
		radi6 = "0.001";
		UpdateData(0);
	}
	else if (atof(radi6) > 1000) {
		UpdateData(1);
		radi6 = "1000";
		UpdateData(0);
	}
	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void csettings2::OnChangeRadi7()
{
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CDialogEx::OnInitDialog() �Լ��� ������ 
	//�ϰ� ����ũ�� OR �����Ͽ� ������ ENM_CHANGE �÷��׸� �����Ͽ� CRichEditCtrl().SetEventMask()�� ȣ������ ������
	// �� �˸� �޽����� ������ �ʽ��ϴ�.
	
	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void csettings2::OnChangeRadi8()
{
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CDialogEx::OnInitDialog() �Լ��� ������ 
	//�ϰ� ����ũ�� OR �����Ͽ� ������ ENM_CHANGE �÷��׸� �����Ͽ� CRichEditCtrl().SetEventMask()�� ȣ������ ������
	// �� �˸� �޽����� ������ �ʽ��ϴ�.
	
	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void csettings2::OnKillfocusRadi7()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	upbl = atof(radi7);
	focrd7 = false;
}


void csettings2::OnSetfocusRadi7()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	focrd7 = true;
}


void csettings2::OnKillfocusRadi8()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	dwbl = atof(radi8);
	focrd8 = false;
}


void csettings2::OnSetfocusRadi8()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	focrd8 = true;
}


void csettings2::OnClickedCheck1()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	
	if (txtmd2 & 0x01)
		txtmd2 &= 0xFE;
	else
		txtmd2 |= 0x01;
	bf = false;
}


void csettings2::OnClickedCheck2()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if (txtmd2 & 0x02)
		txtmd2 &= 0xFD;
	else
		txtmd2 |= 0x02;
	bf = false;
}


//void csettings2::OnStnClickedPos3()
//{
//	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
//}


void csettings2::OnSelchangeCombo2()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	textmode=((CComboBox*)(GetDlgItem(IDC_COMBO2)))->GetCurSel();
}


void csettings2::OnKillfocusCombo2()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	focl = false;
}


void csettings2::OnSetfocusCombo2()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	focl = true;
}
