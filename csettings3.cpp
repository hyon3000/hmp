// csettings3.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Cube.h"
#include "csettings3.h"
#include "afxdialogex.h"
#include "extern.h"
#include <iostream>

int vchn = -1, pchn = -1,vbchn=-1,vmchn=-1,vtchn=-1,ichn=-1,mchn=-1,rchn=-1,bchn=-1,cchn=-1;
// csettings3 대화 상자입니다.
bool qq = 0;
IMPLEMENT_DYNAMIC(csettings3, CDialogEx)

csettings3::csettings3(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG1, pParent)
	, cpc(_T(""))
	, cpv(_T(""))
	, csc(_T(""))
	, cvc(_T(""))
	, cvv(_T(""))
	, ist(_T(""))
	, cpv2(_T(""))
	, cvv_mast(_T(""))
	, cvv_pan(_T(""))
	, cposit(_T(""))
	, _oks(FALSE)
	, cvvbass(_T(""))
	, cvvmid(_T(""))
	, cvvtrib(_T(""))
	, cvcbass(_T(""))
	, cvcmid(_T(""))
	, cvctrib(_T(""))
	
	
	, cbc(_T(""))
	, cbv(_T(""))
	, ccc(_T(""))
	, ccv(_T(""))
	, cic(_T(""))
	, cmc(_T(""))
	, cmv(_T(""))
	, crc(_T(""))
	, crv(_T(""))
	, cev1(_T(""))
	, cev2(_T(""))
	, cev3(_T(""))
{

}
bool cs3init = false;
csettings3::~csettings3()
{
}

void csettings3::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_d1_pc, cpc);
	DDX_Text(pDX, IDC_d1_pv, cpv);
	DDX_Text(pDX, IDC_d1_sc, csc);
	DDX_Text(pDX, IDC_d1_vc, cvc);
	DDX_Text(pDX, IDC_d1_vv, cvv);
	DDX_Text(pDX, IDC_static_1, ist);
	DDX_Text(pDX, IDC_d1_pv2, cpv2);
	DDX_Text(pDX, IDC_vol_master, cvv_mast);
	//DDX_Text(pDX, IDC_vol_pan, cvv_pan);
	DDX_Text(pDX, IDC_d1_sc2, cposit);

	DDX_Check(pDX, IDC_oks, _oks);
	DDX_Text(pDX, IDC_d1_vvbass, cvvbass);
	DDX_Text(pDX, IDC_d1_vvmid, cvvmid);
	DDX_Text(pDX, IDC_d1_vvtrib, cvvtrib);
	DDX_Text(pDX, IDC_d1_vcbass, cvcbass);
	DDX_Text(pDX, IDC_d1_vcmid, cvcmid);
	DDX_Text(pDX, IDC_d1_vctrib, cvctrib);
	//  DDX_Slider(pDX, IDC_SLIDER1, sldr1);
	DDX_Control(pDX, IDC_SLIDER1, sldrctrl);
	//  DDX_CBIndex(pDX, IDC_COMBO2, civ);
	DDX_Text(pDX, IDC_d1_bc, cbc);
	DDX_Text(pDX, IDC_d1_bv, cbv);
	DDX_Text(pDX, IDC_d1_cc, ccc);
	DDX_Text(pDX, IDC_d1_cv, ccv);
	DDX_Text(pDX, IDC_d1_ic, cic);
	DDX_Text(pDX, IDC_d1_mc, cmc);
	DDX_Text(pDX, IDC_d1_mv, cmv);
	DDX_Text(pDX, IDC_d1_rc, crc);
	DDX_Text(pDX, IDC_d1_rv, crv);
//	DDX_Text(pDX, IDC_ev1, cev1);
//	DDX_Text(pDX, IDC_ev2, cev2);
//	DDX_Text(pDX, IDC_ev3, cev3);
	DDX_Control(pDX, IDC_COMBO2, _civ);
}


BEGIN_MESSAGE_MAP(csettings3, CDialogEx)
	ON_BN_CLICKED(IDC_d1_pcdw, &csettings3::OnClickedD1Pcdw)
	ON_BN_CLICKED(IDC_d1_pcup, &csettings3::OnClickedD1Pcup)
	ON_EN_CHANGE(IDC_d1_pc, &csettings3::OnChangeD1Pc)
	ON_EN_CHANGE(IDC_d1_pv, &csettings3::OnChangeD1Pv)
	ON_BN_CLICKED(IDC_d1_vcup, &csettings3::OnClickedD1Vcup)
	ON_BN_CLICKED(IDC_d1_vcdw, &csettings3::OnClickedD1Vcdw)
	ON_EN_CHANGE(IDC_d1_vv, &csettings3::OnChangeD1Vv)
	ON_EN_CHANGE(IDC_d1_sc, &csettings3::OnChangeD1Sc)
	ON_EN_CHANGE(IDC_d1_vc, &csettings3::OnChangeD1Vc)
	//	ON_WM_SHOWWINDOW()
	ON_WM_SHOWWINDOW()
	//ON_WM_CREATE()
	ON_BN_CLICKED(IDCANCEL, &csettings3::OnBnClickedCancel)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_BUTTON1, &csettings3::OnClickedButton1)
	ON_EN_KILLFOCUS(IDC_d1_pv, &csettings3::OnKillfocusD1Pv)
	ON_EN_KILLFOCUS(IDC_d1_pc, &csettings3::OnKillfocusD1Pc)
	ON_EN_KILLFOCUS(IDC_d1_vc, &csettings3::OnKillfocusD1Vc)
	ON_EN_KILLFOCUS(IDC_d1_vv, &csettings3::OnKillfocusD1Vv)
	ON_EN_KILLFOCUS(IDC_d1_sc, &csettings3::OnKillfocusD1Sc)
//	ON_STN_CLICKED(IDC_d1_spd3, &csettings3::OnStnClickedd1spd3)
	ON_EN_CHANGE(IDC_d1_pv2, &csettings3::OnChangeD1Pv2)
	ON_EN_KILLFOCUS(IDC_d1_pv2, &csettings3::OnKillfocusD1Pv2)
	ON_EN_CHANGE(IDC_vol_master, &csettings3::OnChangeVolMaster)
	//ON_EN_CHANGE(IDC_vol_pan, &csettings3::OnChangeVolPan)
	ON_WM_TIMER()
	ON_EN_CHANGE(IDC_d1_sc2, &csettings3::OnChangeD1Sc2)
	ON_EN_KILLFOCUS(IDC_d1_sc2, &csettings3::OnKillfocusD1Sc2)
	ON_BN_CLICKED(IDC_oks, &csettings3::OnBnClickedoks)
	ON_EN_CHANGE(IDC_d1_vcbass, &csettings3::OnChangeD1Vcbass)
	ON_EN_CHANGE(IDC_d1_vcmid, &csettings3::OnChangeD1Vcmid)
	ON_EN_CHANGE(IDC_d1_vctrib, &csettings3::OnChangeD1Vctrib)
	ON_EN_CHANGE(IDC_d1_vvbass, &csettings3::OnChangeD1Vvbass)
	ON_EN_CHANGE(IDC_d1_vvmid, &csettings3::OnChangeD1Vvmid)
	ON_EN_CHANGE(IDC_d1_vvtrib, &csettings3::OnChangeD1Vvtrib)
	ON_BN_CLICKED(IDC_d1_vcup_bass, &csettings3::OnClickedD1VcupBass)
	ON_BN_CLICKED(IDC_d1_vcup_mid, &csettings3::OnClickedD1VcupMid)
	ON_BN_CLICKED(IDC_d1_vcup_trib, &csettings3::OnClickedD1VcupTrib)
	ON_BN_CLICKED(IDC_d1_vcdw_bass, &csettings3::OnClickedD1VcdwBass)
	ON_BN_CLICKED(IDC_d1_vcdw_mid, &csettings3::OnClickedD1VcdwMid)
	ON_BN_CLICKED(IDC_d1_vcdw_trib, &csettings3::OnClickedD1VcdwTrib)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER1, &csettings3::OnReleasedcaptureSlider1)
	ON_WM_HSCROLL()
	ON_CBN_SELCHANGE(IDC_COMBO2, &csettings3::OnSelchangeCombo2)
	ON_BN_CLICKED(IDC_d1_bcup, &csettings3::OnClickedD1Bcup)
	ON_BN_CLICKED(IDC_d1_ccup, &csettings3::OnClickedD1Ccup)
	ON_BN_CLICKED(IDC_d1_icup, &csettings3::OnClickedD1Icup)
	ON_BN_CLICKED(IDC_d1_mcup, &csettings3::OnClickedD1Mcup)
	ON_BN_CLICKED(IDC_d1_rcup, &csettings3::OnClickedD1Rcup)
	ON_BN_CLICKED(IDC_d1_bcdw, &csettings3::OnClickedD1Bcdw)
	ON_BN_CLICKED(IDC_d1_ccdw, &csettings3::OnClickedD1Ccdw)
	ON_BN_CLICKED(IDC_d1_icdw, &csettings3::OnClickedD1Icdw)
	ON_BN_CLICKED(IDC_d1_mcdw, &csettings3::OnClickedD1Mcdw)
	ON_BN_CLICKED(IDC_d1_rcdw, &csettings3::OnClickedD1Rcdw)
	ON_EN_CHANGE(IDC_d1_bc, &csettings3::OnChangeD1Bc)
//	ON_EN_CHANGE(IDC_d1_bv, &csettings3::OnChangeD1Bv)
	ON_EN_KILLFOCUS(IDC_d1_bv, &csettings3::OnKillfocusD1Bv)
	ON_EN_CHANGE(IDC_d1_cc, &csettings3::OnChangeD1Cc)
	ON_EN_CHANGE(IDC_d1_cv, &csettings3::OnChangeD1Cv)
	ON_EN_CHANGE(IDC_d1_ic, &csettings3::OnChangeD1Ic)
	ON_EN_CHANGE(IDC_d1_mc, &csettings3::OnChangeD1Mc)
	ON_EN_CHANGE(IDC_d1_mv, &csettings3::OnChangeD1Mv)
	ON_EN_CHANGE(IDC_d1_rc, &csettings3::OnChangeD1Rc)
	ON_EN_CHANGE(IDC_d1_rv, &csettings3::OnChangeD1Rv)
//	ON_BN_CLICKED(IDC_SND, &csettings3::OnClickedSnd)
	ON_CBN_KILLFOCUS(IDC_COMBO2, &csettings3::OnKillfocusCombo2)
//	ON_BN_CLICKED(IDC_CHECK3, &csettings3::OnBnClickedCheck3)
//ON_BN_CLICKED(IDC_BUTTON2, &csettings3::OnClickedButton2)
ON_WM_DESTROY()
END_MESSAGE_MAP()


// csettings3 메시지 처리기입니다.


void csettings3::OnClickedD1Pcdw()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(1);
	if (pchn < 0)pchn = 0;
	pchn > 0 ? pchn-- : 0;
	cpc = int2charp(pchn + 1);
	char p[50];
	sprintf(p, "%.5lf", pitch[pchn]);
	cpv = p;
	sprintf(p, "%+.5lf", 12.*log2(pitch[pchn] / 100.));
	cpv2 = p;
	UpdateData(0);
}


void csettings3::OnClickedD1Pcup()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(1);
	pchn <15 ? pchn++ : 0;
	cpc = int2charp(pchn + 1);
	char p[50];
	sprintf(p, "%.5lf", pitch[pchn]);
	cpv = p;
	sprintf(p, "%+.5lf", 12.*log2(pitch[pchn] / 100.));
	cpv2 = p;
	UpdateData(0);
}


void csettings3::OnChangeD1Pc()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.
	UpdateData(1);
	if (strcmp(cpc, "*") != 0) {
		if (atoi(cpc) - 1 < 0) cpc = "1";
		else if (atoi(cpc) - 1 > 15)cpc = "16";
		pchn = atoi(cpc) - 1;

		char p[50];
		sprintf(p, "%.5lf", pitch[pchn]);
		cpv = p;
		sprintf(p, "%+.5lf", 12.*log2(pitch[pchn] / 100.));
		cpv2 = p;
	}
	else {
		pchn = -1;
		for (int i = 0; i < 16; i++) if (pitch[i] != pitch[0] && i != 9)goto T2;
		char p[50];
		sprintf(p, "%.5lf", pitch[0]);
		cpv = p;
		sprintf(p, "%+.5lf", 12.*log2(pitch[0] / 100.));
		cpv2 = p;
		goto T21;
	T2:
		cpv = "?";
		cpv2 = "?";
	T21:;
	}
	UpdateData(0);
	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void csettings3::OnChangeD1Pv()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	UpdateData(1);
	char cpvt[100]; double cpvt2;
	sprintf(cpvt, "%s", cpv);
	//printf("%s\n", cpvt);

	cpvt2 = atof(cpvt);

	ro = 1;
	if (cpvt2 > 1e-10 && cpvt2 < 3.5076939009667905669797907802474) cpvt2 = 3.5076939009667905669797907802474;
	if (cpvt2 > 2850.8758980490857751687238578896)cpvt2 = 2850.8758980490857751687238578896;
	if (cpvt2 <= 1e-10) cpvt2 = 4.2093406495766567999961709912834e-50;
	if (oks == 1) {
		cpvt2 = pow(2, (
			floor(12.*log2(cpvt2 / 100.) + 0.5)
			) / 12.0)*100.0;
	}
	if (pchn != -1) pitch[pchn] = cpvt2;
	else for (int i = 0; i < 16; i++)  if (i != 9) pitch[i] = cpvt2;

	if (g3446 == 0) pitch[9] = 100.;
	char p[50];

	sprintf(p, "%+.5lf", 12.*log2(pitch[pchn] / 100.));
	cpv2 = p;
	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(0);
}






void csettings3::OnChangeD1Sc()
{
}

void csettings3::OnClickedD1Vcdw()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(1);
	if (vchn < 0)vchn = 0;
	vchn > 0 ? vchn-- : 0;
	cvc = int2charp(vchn + 1);
	cvv = dbl2charp(volume2[vchn]);
	UpdateData(0);
}


void csettings3::OnClickedD1Vcup()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(1);
	vchn <15 ? vchn++ : 0;
	cvc = int2charp(vchn + 1);
	cvv = dbl2charp(volume2[vchn]);
	UpdateData(0);
}



void csettings3::OnChangeD1Vv()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.
	UpdateData(1);
	if (vchn != -1) volume2[vchn] = atof(cvv);
	else for (int i = 0; i<16; i++)  volume2[i] = atof(cvv);
	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(0);
}
void csettings3::OnChangeD1Vc()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.
	UpdateData(1);
	if (strcmp(cvc, "*") != 0) {
		if (atoi(cvc) - 1 < 0) cvc = "1";
		else if (atoi(cvc) - 1 > 15)cvc = "16";
		vchn = atoi(cvc) - 1;
		cvv = dbl2charp(volume2[vchn]);
	}
	else {
		vchn = -1;
		for (int i = 0; i < 16; i++) if (volume2[i] != volume2[0])goto T1;
		cvv = dbl2charp(volume2[0]);
		goto T11;
	T1:
		cvv = "?";
	T11:;
	}
	UpdateData(0);
	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}




void csettings3::OnShowWindow(BOOL bShow, UINT nStatus)
{

	CDialogEx::OnShowWindow(bShow, nStatus);
	UpdateData(1);
	if (cs3init == false) {
		

		//((CButton*)GetDlgItem(IDC_CHECK1))->SetCheck(0);
		cs3init = true;
		LANGID LangType = PRIMARYLANGID(GetSystemDefaultLangID());
		if (LangType == LANG_KOREAN) kj768 = true;

		char p[50];
		{
			bool a = 0, b = 0,c=0,d=0,e=0,
				f=0,g=0,h=0,y=0,z=0
				;
			for (int i = 1; i < 16; i++) {
				if (pitch[i] != pitch[0] && i != 9) a = 1;
				if (volume2[i] != volume2[0]) b = 1;
				if (mdvol2[0][i] != mdvol2[0][0]) c = 1;
				if (mdvol2[1][i] != mdvol2[1][0]) d = 1;
				if (mdvol2[2][i] != mdvol2[2][0]) e = 1;
				if (fmbank[i] != fmbank[0] && i != 9) f = 1;
				if (fmpreset[i] != fmpreset[0] && i != 9) g = 1;
				if (fchorus[i] != fchorus[0] && i != 9) h = 1;
				if (freverb[i] != freverb[0] && i != 9) y = 1;
				if (fmodulationwheel[i] != fmodulationwheel[0] && i != 9) z = 1;
			}
			if (a == 0) {
				sprintf(p, "%.5lf", pitch[0]); cpv = p;
				sprintf(p, "%+.5lf", log2(pitch[0] / 100.)*12.); cpv2 = p;
			}
			else {
				strcpy(p, "?"); cpv = p; strcpy(p, "?"); cpv2 = p;
			}
			if (b == 0)
				cvv = dbl2charp(volume2[0]);
			else cvv = "?";
			if (c == 0)
				cvvbass = dbl2charp(mdvol2[0][0]);
			else cvvbass = "?";
			if (d == 0)
				cvvmid = dbl2charp(mdvol2[1][0]);
			else cvvmid = "?"; 
			
			if (e == 0)
				cvvtrib = dbl2charp(mdvol2[2][0]);
			else cvvtrib = "?";

			if (f == 0)
				cbv = dbl2charp(fmbank[0]);
			else cbv = "?";
			if (g == 0)
				_civ.SetCurSel(fmpreset[0]);
				
			else _civ.SetCurSel(-1);
			if (h == 0)
				ccv = dbl2charp(fchorus[0]);
			else ccv = "?";
			if (y == 0)
				crv = dbl2charp(freverb[0]);
			else crv = "?";
			if (z == 0)
				cmv = dbl2charp(fmodulationwheel[0]);
			else cmv = "?";
			sprintf(p, "%.2lf / %.2lf", (double)playoffset / 20., (double)playlength / 100.);
			cposit = p;
		}
		cbc = "*";
		cic = "*";
		cmc = "*";
		ccc = "*";
		crc = "*";
		cpc = "*";  cvc = "*"; cvcbass = "*"; cvcmid = "*"; cvctrib = "*"; 	csc = dbl2charp(spd); cvv_mast = dbl2charp(playvolume*100.); 
		//printf("%lf %d!!\n", playvolume*100., midipan);
		//system("pause");
		char a[1000];
		strcpy(a, midname);
		int y = 0, y2 = 0;
		bool y3 = false;
		while (1) {
			if (a[y] == 0) break;
			else if (a[y] >0) y2++;
			else if (y3 == false) y3 = true;
			else { y3 = false; y2 += 2; }
			y++;
		}
		if (y2 > 50) {
			y = 0; y2 = 0;
			y3 = false;
			while (1) {
				if (a[y] == 0) break;
				else if (a[y] >0) y2++;
				else if (y3 == false) y3 = true;
				else { y3 = false; y2 += 2; }
				if (y2 >= 50) break;
				y++;
			}
			a[y + 1] = '.'; a[y + 2] = '.'; a[y + 3] = '.'; a[y + 4] = 0;
		}

		ist = a;
		_oks = oks;
		//system("cls");
		//	printf("%d", oks);
	}
	if (kj768 == true) {
		GetDlgItem(IDC_d1_chn)->SetWindowText("채널\n*:전체");
		GetDlgItem(IDC_d1_val)->SetWindowText("값");
		GetDlgItem(IDC_d1_chn2)->SetWindowText("채널\n*:전체");
		GetDlgItem(IDC_d1_val2)->SetWindowText("값");
		GetDlgItem(IDC_d1_pit)->SetWindowText("음높이 (키 밀기+피치 벤드)");
		GetDlgItem(IDC_d1_spd)->SetWindowText("속도 (10~500)");
		GetDlgItem(IDC_d1_vol)->SetWindowText("소리 크기 (≥0)");
		GetDlgItem(IDC_d1_vol3)->SetWindowText("낮은 소리 크기");
		GetDlgItem(IDC_d1_vol5)->SetWindowText("중간 소리 크기");
		GetDlgItem(IDC_d1_vol7)->SetWindowText("높은 소리 크기");
		GetDlgItem(IDC_d1_spd4)->SetWindowText("전체 소리 크기 (0~100)");
//		GetDlgItem(IDC_d1_spd2)->SetWindowText("소리 좌우 비율 (-100~100)");
		GetDlgItem(IDC_d1_bnk)->SetWindowText("뱅크 \n(지정안함 -1,타악기 127)");
		GetDlgItem(IDC_d1_inst)->SetWindowText("프로그램 (지정안함 129)");
		GetDlgItem(IDC_d1_rev)->SetWindowText("잔향 (-127~127)");
		GetDlgItem(IDC_d1_vol9)->SetWindowText("코러스 (-127~127)");
		GetDlgItem(IDC_d1_vol8)->SetWindowText("모듈레이션 (-16383~16383)");
		GetDlgItem(IDC_d1_spd8)->SetWindowText("위치");
//		GetDlgItem(IDC_d1_ev)->SetWindowText("미디 이벤트 보내기");
//		GetDlgItem(IDC_d1_ev1)->SetWindowText("상태");
//		GetDlgItem(IDC_d1_ev2)->SetWindowText("값 1");
//		GetDlgItem(IDC_d1_ev3)->SetWindowText("값 2");
//		GetDlgItem(IDC_SND)->SetWindowText("보내기");
		GetDlgItem(IDC_oks)->SetWindowText("키 밀기만");
		GetDlgItem(IDCANCEL)->SetWindowText("확인");
		GetDlgItem(IDC_BUTTON1)->SetWindowText("취소");
		//		GetDlgItem(IDC_d1_pit2)->SetWindowText("+/- 사용:반음 단위 (+0:변화 없음)\n부호 없음:백분율 (0:채널 음소거)");
	}


	
	GetDlgItem(IDC_BUTTON1)->EnableWindow(g3446);
	GetDlgItem(IDC_vol_master)->EnableWindow(!g3446);
	GetDlgItem(IDC_d1_pc)->EnableWindow(!g3446 && !midiout || g3446&&midiout);
	GetDlgItem(IDC_d1_pcup)->EnableWindow(!g3446 && !midiout || g3446&&midiout);
	GetDlgItem(IDC_d1_pcdw)->EnableWindow(!g3446 && !midiout || g3446&&midiout);
	GetDlgItem(IDC_d1_pv)->EnableWindow(!g3446 && !midiout || g3446&&midiout);
	GetDlgItem(IDC_d1_pv2)->EnableWindow(!g3446 && !midiout || g3446&&midiout);
	GetDlgItem(IDC_d1_sc)->EnableWindow(!g3446 && !midiout || g3446&&midiout);
	GetDlgItem(IDC_d1_vc)->EnableWindow(!g3446 && !midiout || g3446&&midiout);
	GetDlgItem(IDC_d1_vcup)->EnableWindow(!g3446 && !midiout || g3446&&midiout);
	GetDlgItem(IDC_d1_vcdw)->EnableWindow(!g3446 && !midiout || g3446&&midiout);
	GetDlgItem(IDC_d1_vv)->EnableWindow(!g3446 && !midiout || g3446&&midiout);
	GetDlgItem(IDC_d1_vcbass)->EnableWindow(!g3446 && !midiout || g3446&&midiout);
	GetDlgItem(IDC_d1_vcup_bass)->EnableWindow(!g3446 && !midiout || g3446&&midiout);
	GetDlgItem(IDC_d1_vcdw_bass)->EnableWindow(!g3446 && !midiout || g3446&&midiout);
	GetDlgItem(IDC_d1_vvbass)->EnableWindow(!g3446 && !midiout || g3446&&midiout);
	GetDlgItem(IDC_d1_vcmid)->EnableWindow(!g3446 && !midiout || g3446&&midiout);
	GetDlgItem(IDC_d1_vcup_mid)->EnableWindow(!g3446 && !midiout || g3446&&midiout);
	GetDlgItem(IDC_d1_vcdw_mid)->EnableWindow(!g3446 && !midiout || g3446&&midiout);
	GetDlgItem(IDC_d1_vvmid)->EnableWindow(!g3446 && !midiout || g3446&&midiout);
	GetDlgItem(IDC_d1_vctrib)->EnableWindow(!g3446 && !midiout || g3446&&midiout);
	GetDlgItem(IDC_d1_vcup_trib)->EnableWindow(!g3446 && !midiout || g3446&&midiout);
	GetDlgItem(IDC_d1_vcdw_trib)->EnableWindow(!g3446 && !midiout || g3446&&midiout);
	GetDlgItem(IDC_d1_vvtrib)->EnableWindow(!g3446 && !midiout || g3446&&midiout);
	GetDlgItem(IDC_SLIDER1)->EnableWindow(!g3446&&owptr6 > 24 * 2);
	GetDlgItem(IDC_d1_sc2)->EnableWindow(!g3446&&owptr6 > 24 * 2);
//	GetDlgItem(IDC_ev1)->EnableWindow(!g3446 && !midiout);
//	GetDlgItem(IDC_ev2)->EnableWindow(!g3446 && !midiout);
//	GetDlgItem(IDC_ev3)->EnableWindow(!g3446 && !midiout);
//	GetDlgItem(IDC_SND)->EnableWindow(!g3446 && !midiout);
	GetDlgItem(IDC_oks)->EnableWindow(!g3446 && !midiout);
	GetDlgItem(IDC_d1_bc)->EnableWindow(!g3446 && !midiout || g3446&&midiout);
	GetDlgItem(IDC_d1_bcup)->EnableWindow(!g3446 && !midiout || g3446&&midiout);
	GetDlgItem(IDC_d1_bcdw)->EnableWindow(!g3446 && !midiout || g3446&&midiout);
	GetDlgItem(IDC_d1_bv)->EnableWindow(!g3446 && !midiout || g3446&&midiout);
	GetDlgItem(IDC_d1_ic)->EnableWindow(!g3446 && !midiout || g3446&&midiout);
	GetDlgItem(IDC_d1_icup)->EnableWindow(!g3446 && !midiout || g3446&&midiout);
	GetDlgItem(IDC_d1_icdw)->EnableWindow(!g3446 && !midiout || g3446&&midiout);
	GetDlgItem(IDC_COMBO2)->EnableWindow(!g3446 && !midiout || g3446&&midiout);
	GetDlgItem(IDC_d1_rc)->EnableWindow(!g3446 && !midiout || g3446&&midiout);
	GetDlgItem(IDC_d1_rcup)->EnableWindow(!g3446 && !midiout || g3446&&midiout);
	GetDlgItem(IDC_d1_rcdw)->EnableWindow(!g3446 && !midiout || g3446&&midiout);
	GetDlgItem(IDC_d1_rv)->EnableWindow(!g3446 && !midiout || g3446&&midiout);
	GetDlgItem(IDC_d1_cc)->EnableWindow(!g3446 && !midiout || g3446&&midiout);
	GetDlgItem(IDC_d1_ccup)->EnableWindow(!g3446 && !midiout || g3446&&midiout);
	GetDlgItem(IDC_d1_ccdw)->EnableWindow(!g3446 && !midiout || g3446&&midiout);
	GetDlgItem(IDC_d1_cv)->EnableWindow(!g3446 && !midiout || g3446&&midiout);
	GetDlgItem(IDC_d1_mc)->EnableWindow(!g3446 && !midiout || g3446&&midiout);
	GetDlgItem(IDC_d1_mcup)->EnableWindow(!g3446 && !midiout || g3446&&midiout);
	GetDlgItem(IDC_d1_mcdw)->EnableWindow(!g3446 && !midiout || g3446&&midiout);
	GetDlgItem(IDC_d1_mv)->EnableWindow(!g3446 && !midiout || g3446&&midiout);


	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	UpdateData(0);
}




void csettings3::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//sti3 = false;//printf("%d! %d! ", spd, deltatime);
	spd = spd2;
	deltatime = (int)((double)(deltatime)* spd / 100);
	////printf("%d! %d! ", spd, deltatime);
	for (int i = 0; i < 16; i++) if (pitch[i] != pitch[0])goto T3;
	wjaa = false;
	goto T31;
T3:
	wjaa = true;
T31:;
	CDialogEx::OnCancel();
}




int csettings3::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;
	//SetTimer(2, 45, 0);
	// TODO:  여기에 특수화된 작성 코드를 추가합니다.

	return 0;
}


INT_PTR csettings3::DoModal()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	return CDialogEx::DoModal();
}


void csettings3::OnClickedButton1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	quit = true;
	exit(1);
}


void csettings3::OnClickedCheck1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.




}


void csettings3::OnKillfocusD1Pv()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void csettings3::OnKillfocusD1Pc()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void csettings3::OnKillfocusD1Vc()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void csettings3::OnKillfocusD1Vv()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void csettings3::OnKillfocusD1Sc()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(1);
	// 이 알림 메시지를 보내지 않습니다.
	double tmp = atof(csc);
	if (tmp < 10.)
		csc = "10";
	else if (tmp > 300.)
		csc = "300";
	spd2 = atof(csc);
	speedupdate = 1;
	UpdateData(0);
}
void csettings3::OnStnClickedd1pit2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void csettings3::OnStnClickedd1spd3()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void csettings3::OnBnClickedd1vcup9()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void csettings3::OnChangeD1Pv2()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.
	UpdateData(1);
	char cpvt[100]; double cpvt2;
	sprintf(cpvt, "%s", cpv2);
	//printf("%s\n", cpvt);
	double cpt;
	sscanf(cpvt, "%lf", &cpt);
	if (oks == 1) {
		cpvt2 = pow(2, (
			floor(cpt + 0.5)
			) / 12.0)*100.0;
	}
	else cpvt2 = pow(2, (cpt) / 12.0)*100.0;


	ro = 1;
	if (cpvt2 > 1e-10 && cpvt2 < 3.5076939009667905669797907802474) cpvt2 = 3.5076939009667905669797907802474;
	if (cpvt2 > 2850.8758980490857751687238578896)cpvt2 = 2850.8758980490857751687238578896;
	if (cpvt2 <= 1e-10) cpvt2 = 4.2093406495766567999961709912834e-50;
	if (pchn != -1) pitch[pchn] = cpvt2;
	else for (int i = 0; i < 16; i++)  if (i != 9) pitch[i] = cpvt2;

	if (g3446 == 0) pitch[9] = 100.;
	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	char p[50];
	sprintf(p, "%.5lf", pitch[pchn]);
	cpv = p;

	UpdateData(0);
	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void csettings3::OnKillfocusD1Pv2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void csettings3::OnChangeVolMaster()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.
	UpdateData(1);
	setmidivol = 1;
	playvolume = atof(cvv_mast) / 100.;
	if (playvolume > 1.) {
		playvolume = 1.;
		cvv_mast = "100";
	}
	else if (playvolume < 0.) {
		playvolume = 0.;
		cvv_mast = "0";
	}
	UpdateData(0);
	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}





void csettings3::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (mode == 14)OnCancel();
	UpdateData(1);
		GetDlgItem(IDC_SLIDER1)->EnableWindow(!g3446&&owptr6 > 24 * 2);
	GetDlgItem(IDC_d1_sc2)->EnableWindow(!g3446&&owptr6 > 24 * 2);
	char p[50];
	int handle = 0;
	if(GetFocus()!=nullptr) {
		handle = GetFocus()->GetDlgCtrlID();
	}

	if (handle != IDC_d1_pv) {
		if (pchn >= 0) {
			sprintf(p, "%.5lf", pitch[pchn]);
		}
		else {
			for (int i = 0; i < 16; i++) if (pitch[i] != pitch[0] && i != 9) { sprintf(p, "?"); goto TM1; }
			sprintf(p, "%.5lf", pitch[0]);
		TM1:;
		}
		cpv = p;
	}
	if (handle != IDC_d1_pv2) {
		if (pchn >= 0) {
			sprintf(p, "%+.5lf", 12.*log2(pitch[pchn] / 100.));
		}
		else {
			for (int i = 0; i < 16; i++) if (pitch[i] != pitch[0] && i != 9) { sprintf(p, "?"); goto TM2; }
			sprintf(p, "%+.5lf", 12.*log2(pitch[0] / 100.));
		TM2:;
		}

		cpv2 = p;
	}
	if (handle != IDC_d1_sc) {
		csc = dbl2charp(spd);
	}
	if (handle != IDC_COMBO2 && handle != 1001 ) {
		if (ichn >= 0) {
			_civ.SetCurSel(fmpreset[ichn]);
		}
		else {
			for (int i = 0; i < 16; i++) if (fmpreset[i] != fmpreset[0] && i != 9) { _civ.SetCurSel(-1); goto TM3; }
			_civ.SetCurSel(fmpreset[0]);
		TM3:;
		}
		
		
	}
	if (handle != IDC_d1_bv) {
		if (bchn >= 0) {
			cbv = int2charp(fmbank[bchn]);
		}
		else {
			for (int i = 0; i < 16; i++) if (fmbank[i] != fmbank[0] && i != 9) { cbv = "?"; goto TM4; }
			cbv = int2charp(fmbank[0]);
		TM4:;
		}
	}
	if (handle != IDC_vol_master) {
		cvv_mast = dbl2charp(playvolume*100.);
	}
	if (handle != IDC_d1_sc2&& handle != IDC_SLIDER1) {
		sprintf(p, "%.2lf / %.2lf", (double)playoffset / 20., (double)playlength / 100.);
		cposit = p;
	}
	if (handle != IDC_SLIDER1) {
			sldrctrl.SetRange(0, playlength / 5);
			sldrctrl.SetPos(playoffset);
	}
	UpdateData(0);
	CDialogEx::OnTimer(nIDEvent);
}


void csettings3::OnChangeD1Sc2()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.


}


void csettings3::OnKillfocusD1Sc2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(1);
	char p[500];
	sprintf(p, "%s", cposit);

	nplayoffset = atof(strtok(p, " /")) *20.;
	if (nplayoffset < 0) {
		nplayoffset = 0;
		sprintf(p, "%.2lf / %.2lf", (double)playoffset / 20., (double)playlength / 100.);
		cposit = p;
	}
	if (nplayoffset > playlength / 5 - 5) {
		nplayoffset = playlength / 5 - 5;
		sprintf(p, "%.2lf / %.2lf", (double)playoffset / 20., (double)playlength / 100.);
		cposit = p;
	}
	mpoffset = 1;

	UpdateData(0);
}


void csettings3::OnBnClickedoks()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(1);
	unsigned int oclo = clock();
	ro = 1;
	int oisplay;
	if (!midiout) {
		oisplay = isplay;
		isplay = 0;
	}
	oks = !oks;
	_oks = oks;
	if (oks == 1) {
		for (int i = 0; i<16; i++)if (i != 9)pitch[i] = pow(2, (
			floor(12.*log2(pitch[i] / 100.) + 0.5)
			) / 12.0)*100.0;
	}
	if (!midiout) { isplay = oisplay; playmidin0 += clock() - oclo; }
	UpdateData(0);
}



void csettings3::OnClickedD1VcdwBass()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(1);
	if (vbchn < 0)vbchn = 0;
	vbchn > 0 ? vbchn-- : 0;
	cvcbass = int2charp(vbchn + 1);
	cvvbass = dbl2charp(mdvol2[0][vbchn]);
	UpdateData(0);
}


void csettings3::OnClickedD1VcupBass()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(1);
	vbchn <15 ? vbchn++ : 0;
	cvcbass = int2charp(vbchn + 1);
	cvvbass = dbl2charp(mdvol2[0][vbchn]);
	UpdateData(0);
}



void csettings3::OnChangeD1Vvbass()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.
	UpdateData(1);
	if (vbchn != -1) mdvol2[0][vbchn] = atof(cvvbass);
	else for (int i = 0; i<16; i++)  mdvol2[0][i] = atof(cvvbass);
	if (vbchn != -1) {
		for (int i = 0; i < 64; i++) { mdvol3[i][vbchn] = (mdvol2[0][vbchn] * (double)(64 - i) + mdvol2[1][vbchn] * (double)i) / 6400.; if (mdvol3[i][vbchn] > 1) mdvol3[i][vbchn] = 1; }
		for (int i = 64; i < 128; i++) { mdvol3[i][vbchn] = (mdvol2[1][vbchn] * (double)(128 - i) + mdvol2[2][vbchn] * (double)(i - 64)) / 6400.; if (mdvol3[i][vbchn] > 1) mdvol3[i][vbchn] = 1; }
	}
	else
		for (int vbchn = 0; vbchn < 16; vbchn++) {
			for (int i = 0; i < 64; i++) { mdvol3[i][vbchn] = (mdvol2[0][vbchn] * (double)(64 - i) + mdvol2[1][vbchn] * (double)i) / 6400.; if (mdvol3[i][vbchn] > 1) mdvol3[i][vbchn] = 1; }
			for (int i = 64; i < 128; i++) { mdvol3[i][vbchn] = (mdvol2[1][vbchn] * (double)(128 - i) + mdvol2[2][vbchn] * (double)(i - 64)) / 6400.; if (mdvol3[i][vbchn] > 1) mdvol3[i][vbchn] = 1; }

		}
	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(0);
}
void csettings3::OnChangeD1Vcbass()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.
	UpdateData(1);
	if (strcmp(cvcbass, "*") != 0) {
		if (atoi(cvcbass) - 1 < 0) cvcbass = "1";
		else if (atoi(cvcbass) - 1 > 15)cvcbass = "16";
		vbchn = atoi(cvcbass) - 1;
		cvvbass = dbl2charp(mdvol2[0][vbchn]);
	}
	else {
		vbchn = -1;
		for (int i = 0; i < 16; i++) if (mdvol2[0][i] != mdvol2[0][0])goto T1;
		cvvbass = dbl2charp(mdvol2[0][0]);
		goto T11;
	T1:
		cvvbass = "?";
	T11:;
	}
	
	UpdateData(0);
	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void csettings3::OnClickedD1VcdwTrib()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(1);
	if (vtchn < 0)vtchn = 0;
	vtchn > 0 ? vtchn-- : 0;
	cvctrib = int2charp(vtchn + 1);
	cvvtrib = dbl2charp(mdvol2[2][vtchn]);
	UpdateData(0);
}


void csettings3::OnClickedD1VcupTrib()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(1);
	vtchn <15 ? vtchn++ : 0;
	cvctrib = int2charp(vtchn + 1);
	cvvtrib = dbl2charp(mdvol2[2][vtchn]);
	UpdateData(0);
}



void csettings3::OnChangeD1Vvtrib()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.
	UpdateData(1);
	if (vtchn != -1) mdvol2[2][vtchn] = atof(cvvtrib);
	else for (int i = 0; i<16; i++)  mdvol2[2][i] = atof(cvvtrib);
	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (vtchn != -1) {
		for (int i = 0; i < 64; i++) { mdvol3[i][vtchn] = (mdvol2[0][vtchn] * (double)(64 - i) + mdvol2[1][vtchn] * (double)i) / 6400.; if (mdvol3[i][vtchn] > 1) mdvol3[i][vtchn] = 1; }
		for (int i = 64; i < 128; i++) { mdvol3[i][vtchn] = (mdvol2[1][vtchn] * (double)(128 - i) + mdvol2[2][vtchn] * (double)(i - 64)) / 6400.; if (mdvol3[i][vtchn] > 1) mdvol3[i][vtchn] = 1; }
	}
	else
		for (int vtchn = 0; vtchn < 16; vtchn++) {
			for (int i = 0; i < 64; i++) { mdvol3[i][vtchn] = (mdvol2[0][vtchn] * (double)(64 - i) + mdvol2[1][vtchn] * (double)i) / 6400.; if (mdvol3[i][vtchn] > 1) mdvol3[i][vtchn] = 1; }
			for (int i = 64; i < 128; i++) { mdvol3[i][vtchn] = (mdvol2[1][vtchn] * (double)(128 - i) + mdvol2[2][vtchn] * (double)(i - 64)) / 6400.; if (mdvol3[i][vtchn] > 1) mdvol3[i][vtchn] = 1; }

		}
	UpdateData(0);
}
void csettings3::OnChangeD1Vctrib()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.
	UpdateData(1);
	if (strcmp(cvctrib, "*") != 0) {
		if (atoi(cvctrib) - 1 < 0) cvctrib = "1";
		else if (atoi(cvctrib) - 1 > 15)cvctrib = "16";
		vtchn = atoi(cvctrib) - 1;
		cvvtrib = dbl2charp(mdvol2[2][vtchn]);
	}
	else {
		vtchn = -1;
		for (int i = 0; i < 16; i++) if (mdvol2[2][i] != mdvol2[2][0])goto T1;
		cvvtrib = dbl2charp(mdvol2[2][0]);
		goto T11;
	T1:
		cvvtrib = "?";
	T11:;
	}
	
	UpdateData(0);
	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void csettings3::OnClickedD1VcdwMid()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(1);
	if (vmchn < 0)vmchn = 0;
	vmchn > 0 ? vmchn-- : 0;
	cvcmid = int2charp(vmchn + 1);
	cvvmid = dbl2charp(mdvol2[1][vmchn]);
	UpdateData(0);
}


void csettings3::OnClickedD1VcupMid()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(1);
	vmchn <15 ? vmchn++ : 0;
	cvcmid = int2charp(vmchn + 1);
	cvvmid = dbl2charp(mdvol2[1][vmchn]);
	UpdateData(0);
}



void csettings3::OnChangeD1Vvmid()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.
	UpdateData(1);
	if (vmchn != -1) mdvol2[1][vmchn] = atof(cvvmid);
	else for (int i = 0; i<16; i++)  mdvol2[1][i] = atof(cvvmid);
	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (vmchn != -1) {
		for (int i = 0; i < 64; i++) { mdvol3[i][vmchn] = (mdvol2[0][vmchn] * (double)(64 - i) + mdvol2[1][vmchn] * (double)i) / 6400.; if (mdvol3[i][vmchn] > 1) mdvol3[i][vmchn] = 1; }
		for (int i = 64; i < 128; i++) { mdvol3[i][vmchn] = (mdvol2[1][vmchn] * (double)(128 - i) + mdvol2[2][vmchn] * (double)(i - 64)) / 6400.; if (mdvol3[i][vmchn] > 1) mdvol3[i][vmchn] = 1; }
	}
	else
		for (int vmchn = 0; vmchn < 16; vmchn++) {
			for (int i = 0; i < 64; i++) { mdvol3[i][vmchn] = (mdvol2[0][vmchn] * (double)(64 - i) + mdvol2[1][vmchn] * (double)i) / 6400.; if (mdvol3[i][vmchn] > 1) mdvol3[i][vmchn] = 1; }
			for (int i = 64; i < 128; i++) { mdvol3[i][vmchn] = (mdvol2[1][vmchn] * (double)(128 - i) + mdvol2[2][vmchn] * (double)(i - 64)) / 6400.; if (mdvol3[i][vmchn] > 1) mdvol3[i][vmchn] = 1; }

		}
	UpdateData(0);
}
void csettings3::OnChangeD1Vcmid()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.
	UpdateData(1);
	if (strcmp(cvcmid, "*") != 0) {
		if (atoi(cvcmid) - 1 < 0) cvcmid = "1";
		else if (atoi(cvcmid) - 1 > 15)cvcmid = "16";
		vmchn = atoi(cvcmid) - 1;
		cvvmid = dbl2charp(mdvol2[1][vmchn]);
	}
	else {
		vmchn = -1;
		for (int i = 0; i < 16; i++) if (mdvol2[1][i] != mdvol2[1][0])goto T1;
		cvvmid = dbl2charp(mdvol2[1][0]);
		goto T11;
	T1:
		cvvmid = "?";
	T11:;
	}
	
	UpdateData(0);
	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void csettings3::OnReleasedcaptureSlider1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(1);
	char p[500];
	sprintf(p, "%s", cposit);

	nplayoffset = sldrctrl.GetPos();
	if (nplayoffset < 0) {
		nplayoffset = 0;
		sprintf(p, "%.2lf / %.2lf", (double)playoffset / 20., (double)playlength / 100.);
		cposit = p;
	}
	if (nplayoffset > playlength / 5 - 5) {
		nplayoffset = playlength / 5 - 5;
		sprintf(p, "%.2lf / %.2lf", (double)playoffset / 20., (double)playlength / 100.);
		cposit = p;
	}
	mpoffset = 1;
	
	GetDlgItem(IDCANCEL)->SetFocus();
	UpdateData(0);
	*pResult = 0;
}


void csettings3::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	UpdateData(1);
	sprintf(p, "%.2lf / %.2lf", (double)sldrctrl.GetPos() / 20., (double)playlength / 100.);
	cposit = p;
	UpdateData(0);
	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}

void csettings3::OnClickedD1Bcdw()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(1);
	if (bchn < 0)bchn = 0;
	bchn > 0 ? bchn-- : 0;
	cbc = int2charp(bchn + 1);
	cbv = int2charp(fmbank[bchn]);
	UpdateData(0);
}


void csettings3::OnClickedD1Bcup()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(1);
	bchn <15 ? bchn++ : 0;
	cbc = int2charp(bchn + 1);
	cbv = int2charp(fmbank[bchn]);
	UpdateData(0);
}

void csettings3::OnKillfocusD1Bv()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	UpdateData(1);
	ro = 1;
	int tp = atoi(cbv);
	if (tp<-1) tp = -1;
	else if (tp>127) tp = 127;
	if (tp == -1 && !g3446) { tp = 0; cbv = "0"; }
	if (bchn != -1) fmbank[bchn] = tp,bpaused[bchn]=1;
	else for (int i = 0; i<16; i++)  if(i != 9)fmbank[i] = tp,bpaused[bchn]=1;
	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(0);
}
void csettings3::OnChangeD1Bc()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.
	UpdateData(1);
	if (strcmp(cbc, "*") != 0) {
		if (atoi(cbc) - 1 < 0) cbc = "1";
		else if (atoi(cbc) - 1 > 15)cbc = "16";
		bchn = atoi(cbc) - 1;
		cbv = int2charp(fmbank[bchn]);
	}
	else {
		bchn = -1;
		for (int i = 0; i < 16; i++) if (fmbank[i] != fmbank[0] && i != 9)goto T1;
		cbv = int2charp(fmbank[0]);
		goto T11;
	T1:
		cbv = "?";
	T11:;
	}
	UpdateData(0);
	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void csettings3::OnClickedD1Icdw()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(1);
	if (ichn < 0)ichn = 0;
	ichn > 0 ? ichn-- : 0;
	cic = int2charp(ichn + 1);
	_civ.SetCurSel(fmpreset[ichn]);
	UpdateData(0);
}


void csettings3::OnClickedD1Icup()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(1);
	ichn <15 ? ichn++ : 0;
	cic = int2charp(ichn + 1); 
	_civ.SetCurSel(fmpreset[ichn]);
	
	UpdateData(0);
}

void csettings3::OnSelchangeCombo2()
{

	UpdateData(1);

	int tp = _civ.GetCurSel();
	if (tp<0) tp = 0;
	else if (tp>128) tp = 128;
	if (tp == 128 && !g3446) { tp = 0; _civ.SetCurSel(0); }
	if (ichn != -1) fmpreset[ichn] = tp, ipaused[ichn] = 1;
	else for (int i = 0; i<16; i++)  if (i != 9)fmpreset[i] = tp, ipaused[ichn] = 1;
	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(0);
}
void csettings3::OnChangeD1Ic()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.
	UpdateData(1);
	if (strcmp(cic, "*") != 0) {
		if (atoi(cic) - 1 < 0) cic = "1";
		else if (atoi(cic) - 1 > 15)cic = "16";
		ichn = atoi(cic) - 1;
		_civ.SetCurSel(fmpreset[ichn]);
	
	}
	else {
		ichn = -1;
		for (int i = 0; i < 16; i++) if (fmpreset[i] != fmpreset[0] && i != 9)goto T1;
		_civ.SetCurSel(fmpreset[0]);
		goto T11;
	T1:
		_civ.SetCurSel(-1);
	T11:;
	}
	UpdateData(0);
	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void csettings3::OnClickedD1Mcdw()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(1);
	if (mchn < 0)mchn = 0;
	mchn > 0 ? mchn-- : 0;
	cmc = int2charp(mchn + 1);
	cmv = int2charp(fmodulationwheel[mchn]);
	UpdateData(0);
}


void csettings3::OnClickedD1Mcup()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(1);
	mchn <15 ? mchn++ : 0;
	cmc = int2charp(mchn + 1);
	cmv = int2charp(fmodulationwheel[mchn]);
	UpdateData(0);
}

void csettings3::OnChangeD1Mv()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	UpdateData(1);
	int tp = atoi(cmv);
	if (tp<-16383) tp = -16383;
	else if (tp>16383) tp = 16383;
	if (mchn != -1) { fmodulationwheel[mchn] = tp;  mpaused[mchn] = 1; }
	else for (int i = 0; i < 16; i++) if (i != 9) { fmodulationwheel[i] = tp; mpaused[i] = 1; }
	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(0);
}
void csettings3::OnChangeD1Mc()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.
	UpdateData(1);
	if (strcmp(cmc, "*") != 0) {
		if (atoi(cmc) - 1 < 0) cmc = "1";
		else if (atoi(cmc) - 1 > 15)cmc = "16";
		mchn = atoi(cmc) - 1;
		cmv = int2charp(fmodulationwheel[mchn]);
	}
	else {
		mchn = -1;
		for (int i = 0; i < 16; i++) if (fmodulationwheel[i] != fmodulationwheel[0] && i != 9)goto T1;
		cmv = int2charp(fmodulationwheel[0]);
		goto T11;
	T1:
		cmv = "?";
	T11:;
	}
	UpdateData(0);
	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void csettings3::OnClickedD1Rcdw()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(1);
	if (rchn < 0)rchn = 0;
	rchn > 0 ? rchn-- : 0;
	crc = int2charp(rchn + 1);
	crv = int2charp(freverb[rchn]);
	UpdateData(0);
}


void csettings3::OnClickedD1Rcup()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(1);
	rchn <15 ? rchn++ : 0;
	crc = int2charp(rchn + 1);
	crv = int2charp(freverb[rchn]);
	UpdateData(0);
}

void csettings3::OnChangeD1Rv()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	UpdateData(1);
	int tp = atoi(crv);
	if (tp<-127) tp = -127;
	else if (tp>127) tp = 127;
	if (rchn != -1) { freverb[rchn] = tp; rpaused[rchn] = 1; }
	else for (int i = 0; i < 16; i++) if (i != 9) { freverb[i] = tp;  rpaused[i] = 1; }
	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(0);
}
void csettings3::OnChangeD1Rc()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	UpdateData(1);
	if (strcmp(crc, "*") != 0) {
		if (atoi(crc) - 1 < 0) crc = "1";
		else if (atoi(crc) - 1 > 15)crc = "16";
		rchn = atoi(crc) - 1;
		crv = int2charp(freverb[rchn]);
	}
	else {
		rchn = -1;
		for (int i = 0; i < 16; i++) if (freverb[i] != freverb[0] && i != 9)goto T1;
		crv = int2charp(freverb[0]);
		goto T11;
	T1:
		crv = "?";
	T11:;
	}
	UpdateData(0);
	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void csettings3::OnClickedD1Ccdw()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(1);
	if (cchn < 0)cchn = 0;
	cchn > 0 ? cchn-- : 0;
	ccc = int2charp(cchn + 1);
	ccv = int2charp(fchorus[cchn]);
	UpdateData(0);
}


void csettings3::OnClickedD1Ccup()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(1);
	cchn <15 ? cchn++ : 0;
	ccc = int2charp(cchn + 1);
	ccv = int2charp(fchorus[cchn]);
	UpdateData(0);
}

void csettings3::OnChangeD1Cv()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	UpdateData(1);
	int tp = atoi(ccv);
	if (tp<-127) tp = -127;
	else if (tp>127) tp = 127;
	if (cchn != -1) { fchorus[cchn] = tp;  cpaused[cchn] = 1; }
	else for (int i = 0; i < 16; i++) if (i != 9) { fchorus[i] = tp; cpaused[i] = 1; }
	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(0);
}
void csettings3::OnChangeD1Cc()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.
	UpdateData(1);
	if (strcmp(ccc, "*") != 0) {
		if (atoi(ccc) - 1 < 0) ccc = "1";
		else if (atoi(ccc) - 1 > 15)ccc = "16";
		cchn = atoi(ccc) - 1;
		ccv = int2charp(fchorus[cchn]);
	}
	else {
		cchn = -1;
		for (int i = 0; i < 16; i++) if (fchorus[i] != fchorus[0] && i != 9)goto T1;
		ccv = int2charp(fchorus[0]);
		goto T11;
	T1:
		ccv = "?";
	T11:;
	}
	UpdateData(0);
	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}



void csettings3::OnClickedSnd()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void csettings3::OnKillfocusCombo2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

}






void csettings3::OnDestroy()
{
	

	CDialogEx::OnDestroy();
	//delete this;
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


BOOL csettings3::DestroyWindow()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	
	return CDialogEx::DestroyWindow();
}
