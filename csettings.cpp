// csettings.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Cube.h"
#include "csettings.h"
#include "afxdialogex.h"
#include "extern.h"
#include <mmsystem.h>

extern char p[100];
bool csetinit = false;
long long midifilesize;
// csettings 대화 상자입니다.

IMPLEMENT_DYNAMIC(csettings, CDialogEx)

csettings::csettings(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_settings, pParent)
	, sfname(_T(""))
	, sprate(_T("Sampling rate"))
	, sfont(_T("Soundfont"))
	, automod(FALSE)
	, stemp(_T("Temperament"))
	, vmode(_T("Viewmode"))
	, mrmode(FALSE)
	, rev(FALSE)
	, tfreq(_T(""))
	, tfval(_T(""))
	, tfwan(_T(""))
{

}

csettings::~csettings()
{
}

void csettings::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	printf("@");
	DDX_Text(pDX, IDC_STATIC_SFNAME, sfname);
	DDX_Text(pDX, IDC_STATIC_SAMPRATE, sprate);
	DDX_Text(pDX, IDC_STATIC_SOUNDFONT, sfont);
	DDX_Control(pDX, IDC_erase, btners);
	DDX_Control(pDX, IDC_loadsf, btnldsf);
	DDX_Check(pDX, IDC_automod, automod);
	//DDX_Control(pDX, IDC_SAMPCOMBO, smpcombo);
	//DDX_Control(pDX, IDC_defmp, defmp);
	printf("^");
	DDX_Text(pDX, IDC_STATIC_TEMPERAMENT, stemp);
	DDX_Control(pDX, IDCANCEL, btncc);
	//DDX_Text(pDX, IDC_STATIC_VIEWMODE, vmode);
	printf("&");
	DDX_Check(pDX, IDC_mrmode, mrmode);
	DDX_Check(pDX, IDC_rev, rev);
	DDX_Text(pDX, IDC_STATIC_SAMPRATE2, tfreq);
	DDX_Text(pDX, IDC_EDIT1, tfval);
	printf("#");
	if (csetinit == false) {
		csetinit = true;
		LANGID LangType = PRIMARYLANGID(GetSystemDefaultLangID());
		if (LangType == LANG_KOREAN) kj768 = true;
		UpdateData(1);
		sfname = _T(recentfilelist[11]);
		if(!strlen(recentfilelist[11])) sfname = _T("hmp.exe\\gm.sf2");
		////printf("!");
		if (turfreq < 1)turfreq = 440;
		mrmode = mirror;
		rev = reverse;
		if (kj768 == true) {
			sprate = _T("샘플링 레이트");
			tfreq = _T("튜닝 주파수");
			sfont = _T("사운드폰트");
			stemp = _T("조율방식");
			vmode = _T("화면 모드");

			GetDlgItem(IDC_erase)->SetWindowText("지우기");
			GetDlgItem(IDC_loadsf)->SetWindowText("불러오기");
			GetDlgItem(IDCANCEL)->SetWindowText("닫기");
			GetDlgItem(IDC_equal)->SetWindowText("평균율");
			GetDlgItem(IDC_pyt)->SetWindowText("피타고라스 음율");
			GetDlgItem(IDC_just)->SetWindowText("순정률");
			GetDlgItem(IDC_automod)->SetWindowText("자동 조바꿈");
			//GetDlgItem(IDC_USINGPREVIEW)->SetWindowText("미리보기 사용");
			//GetDlgItem(IDC_FB)->SetWindowText("건반+블록");
			//GetDlgItem(IDC_STF)->SetWindowText("악보");
			GetDlgItem(IDC_mrmode)->SetWindowText("거울 모드");
			GetDlgItem(IDC_rev)->SetWindowText("역재생");
			SetWindowText("설정");
		}////printf("!");
		switch (jhj56) {
		case 0:
			automod = true;
			((CButton*)GetDlgItem(IDC_automod))->EnableWindow(0);
			((CButton*)GetDlgItem(IDC_equal))->SetCheck(1);
			((CButton*)GetDlgItem(IDC_pyt))->SetCheck(0);
			((CButton*)GetDlgItem(IDC_just))->SetCheck(0);
			break;
		case 1:
			automod = false;
			((CButton*)GetDlgItem(IDC_automod))->EnableWindow(1);
			((CButton*)GetDlgItem(IDC_equal))->SetCheck(0);
			((CButton*)GetDlgItem(IDC_pyt))->SetCheck(1);
			((CButton*)GetDlgItem(IDC_just))->SetCheck(0);
			break;
		case 2:
			automod = true;
			((CButton*)GetDlgItem(IDC_automod))->EnableWindow(1);
			((CButton*)GetDlgItem(IDC_equal))->SetCheck(0);
			((CButton*)GetDlgItem(IDC_pyt))->SetCheck(1);
			((CButton*)GetDlgItem(IDC_just))->SetCheck(0);
			break;
		case 3:
			automod = false;
			((CButton*)GetDlgItem(IDC_automod))->EnableWindow(1);
			((CButton*)GetDlgItem(IDC_equal))->SetCheck(0);
			((CButton*)GetDlgItem(IDC_pyt))->SetCheck(0);
			((CButton*)GetDlgItem(IDC_just))->SetCheck(1);
			break;
		case 4:
			automod = true;
			((CButton*)GetDlgItem(IDC_automod))->EnableWindow(1);
			((CButton*)GetDlgItem(IDC_equal))->SetCheck(0);
			((CButton*)GetDlgItem(IDC_pyt))->SetCheck(0);
			((CButton*)GetDlgItem(IDC_just))->SetCheck(1);
			break;
		}
		/*switch (ghkj6) {
		case 0:
			((CButton*)GetDlgItem(IDC_FB))->SetCheck(1);
			((CButton*)GetDlgItem(IDC_STF))->SetCheck(0);
			break;
		default:
			((CButton*)GetDlgItem(IDC_FB))->SetCheck(0);
			((CButton*)GetDlgItem(IDC_STF))->SetCheck(1);
			break;
		}
		if (j65j8 == 1)((CButton*)GetDlgItem(IDC_USINGPREVIEW))->SetCheck(1);
		else((CButton*)GetDlgItem(IDC_USINGPREVIEW))->SetCheck(0);*/
		////printf("!");
		////printf("%d ", h54ui);
		int t;
		if (h54ui == 0)t = 0;
		else if (h54ui == 22050) t = 1;
		else if (h54ui == 44100)t = 2;
		else if (h54ui == 88200)t = 3;
		else if (h54ui == 176400)t = 4;
		else if (h54ui == 352800)t = 5;
		////printf("%d ", t);//printf("!");
		((CComboBox*)(GetDlgItem(IDC_SAMPCOMBO)))->SetCurSel(t); ////printf("!");
		tfval = flt2charp(turfreq);
		UpdateData(0);
	}
	DDX_Text(pDX, IDC_tfwarn, tfwan);
	//  DDX_Control(pDX, IDC_STATIC_MMP, mmps);

}


BEGIN_MESSAGE_MAP(csettings, CDialogEx)
//	ON_BN_CLICKED(IDC_USINGPREVIEW, &csettings::OnClickedUsingpreview)
	ON_BN_CLICKED(IDC_loadsf, &csettings::OnClickedLoadsf)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_erase, &csettings::OnClickedErase)
	ON_WM_SHOWWINDOW()
	ON_COMMAND(IDC_equal, &csettings::OnEqual)
	ON_COMMAND(IDC_pyt, &csettings::OnPyt)
	ON_BN_CLICKED(IDC_automod, &csettings::OnClickedAutomod)
	ON_CBN_SELCHANGE(IDC_SAMPCOMBO, &csettings::OnSelchangeSampcombo)
//ON_COMMAND(IDC_FB, &csettings::OnFb)
//ON_COMMAND(IDC_STF, &csettings::OnStf)
//ON_BN_CLICKED(IDC_USINGPREVIEW, &csettings::OnClickedUsingpreview)
//ON_WM_LBUTTONDOWN()
ON_BN_CLICKED(IDC_mrmode, &csettings::OnClickedMrmode)
//ON_BN_CLICKED(IDC_rev, &csettings::OnClickedRev)
//ON_COMMAND(IDC_RADIO_hor, &csettings::OnRadioHor)
//ON_BN_CLICKED(IDC_equal2, &csettings::OnBnClickedequal2)
ON_COMMAND(IDC_just, &csettings::OnJust)
ON_EN_KILLFOCUS(IDC_EDIT1, &csettings::OnKillfocusEdit1)
ON_EN_CHANGE(IDC_EDIT1, &csettings::OnChangeEdit1)
//ON_BN_CLICKED(IDC_rev2, &csettings::OnClickedRev2)
ON_BN_CLICKED(IDC_rev, &csettings::OnClickedRev)
//ON_COMMAND(IDC_hfbmps, &csettings::OnHfbmps)
//ON_COMMAND(IDC_tim, &csettings::OnTim)
ON_STN_CLICKED(IDC_STATIC_SAMPRATE3, &csettings::OnStnClickedStaticSamprate3)
ON_BN_CLICKED(IDC_RADIO1, &csettings::OnBnClickedRadio1)
ON_BN_CLICKED(IDC_RADIO2, &csettings::OnBnClickedRadio2)
ON_COMMAND(IDC_RADIO3, &csettings::OnBnClickedRadio3)
END_MESSAGE_MAP()


INT_PTR csettings::DoModal()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	return CDialogEx::DoModal();
}


void csettings::OnClickedLoadsf()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CFileDialog dlg(TRUE, NULL, NULL, OFN_FILEMUSTEXIST | OFN_LONGNAMES,
		"SOUNDFONT2 FILES(*.sf2)|*.sf2|All Files(*.*)|*.*||");
	CString strPathName;
	if (IDOK == dlg.DoModal()) {
		FILE *t;
		strPathName = dlg.GetPathName();
		if ((t = fopen(strPathName.GetBuffer(), "r")) == NULL) return;
		strcpy(recentfilelist[11], strPathName.GetBuffer());
		j5j58 = true;
	}
	UpdateData(1);
	sfname = _T(recentfilelist[11]); if (!strlen(recentfilelist[11])) sfname = _T("hmp.exe\\gm.sf2");
	UpdateData(0);
}




int csettings::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	printf("a");
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;
	printf("_");
	// TODO:  여기에 특수화된 작성 코드를 추가합니다.

	return 0;
}




void csettings::OnClickedErase()
{
	
	strcpy(recentfilelist[11], "\0");
	j5j58 = true;
	UpdateData(1);
	sfname = _T(recentfilelist[11]); if (!strlen(recentfilelist[11])) sfname = _T("hmp.exe\\gm.sf2");
	UpdateData(0);
}

void csettings::OnShowWindow(BOOL bShow, UINT nStatus)
{
	printf("b");
	csetinit = false;
	printf("c");
	LANGID LangType = PRIMARYLANGID(GetSystemDefaultLangID());
	printf("d");
	if (LangType == LANG_KOREAN) kj768 = true;
	printf("e");
	UpdateData(1);
	////printf("!");
	printf("f");
	mrmode = mirror;

	rev = reverse;
	
	printf("g");
	if (kj768 == true) {
		sprate = _T("샘플링 레이트");
		tfreq = _T("튜닝 주파수");
		sfont = _T("사운드폰트");
		stemp = _T("조율방식");
		vmode = _T("화면 모드");

		btners.SetWindowText("지우기");
		btnldsf.SetWindowText("불러오기");
		btncc.SetWindowText("닫기");
		//GetDlgItem(IDC_rev2)->SetWindowText("음표 겹침 허용");
		//GetDlgItem(IDC_hfbmps)->SetWindowText("하미플 사인파 생성기");
		//GetDlgItem(IDC_tim)->SetWindowText("티미디티");
		GetDlgItem(IDC_equal)->SetWindowText("평균율");
		GetDlgItem(IDC_pyt)->SetWindowText("피타고라스 음율");
		GetDlgItem(IDC_just)->SetWindowText("순정률");
		GetDlgItem(IDC_automod)->SetWindowText("자동 조바꿈");
		//GetDlgItem(IDC_USINGPREVIEW)->SetWindowText("미리보기 사용");
		//GetDlgItem(IDC_FB)->SetWindowText("건반+블록");
		//GetDlgItem(IDC_STF)->SetWindowText("악보");
		GetDlgItem(IDC_mrmode)->SetWindowText("거울 모드");
		GetDlgItem(IDC_rev)->SetWindowText("역재생");
		GetDlgItem(IDC_STATIC_SAMPRATE3)->SetWindowText("미디 출력 장치");
		MIDIOUTCAPS moc;
		midiOutGetDevCaps(0, &moc, sizeof(moc));
		GetDlgItem(IDC_RADIO2)->SetWindowText(moc.szPname);

		SetWindowText("설정");
	}////printf("!");
	else {
		
		MIDIOUTCAPS moc;
		midiOutGetDevCaps(0, &moc, sizeof(moc));
		GetDlgItem(IDC_RADIO2)->SetWindowText(moc.szPname);
	}
	//((CButton*)GetDlgItem(IDC_rev2))->SetCheck(allownoteoverlap);
	printf("h");

	switch (midiout) {
	case 0:
		if (tfnm == 0) {
			((CButton*)GetDlgItem(IDC_RADIO1))->SetCheck(0);
			((CButton*)GetDlgItem(IDC_RADIO2))->SetCheck(0);
			((CButton*)GetDlgItem(IDC_RADIO3))->SetCheck(1);
		}
		else {
			((CButton*)GetDlgItem(IDC_RADIO1))->SetCheck(0);
			((CButton*)GetDlgItem(IDC_RADIO2))->SetCheck(1);
			((CButton*)GetDlgItem(IDC_RADIO3))->SetCheck(0);
		}

		mirror = 0;
		reverse = 0;
		mrmode = mirror;

		rev = reverse;
		((CButton*)GetDlgItem(IDC_mrmode))->SetCheck(0);
		((CButton*)GetDlgItem(IDC_rev))->SetCheck(0);
		((CButton*)GetDlgItem(IDC_mrmode))->EnableWindow(0);
		((CButton*)GetDlgItem(IDC_rev))->EnableWindow(0);
		((CButton*)GetDlgItem(IDC_loadsf))->EnableWindow(0);
		((CButton*)GetDlgItem(IDC_erase))->EnableWindow(0);
		GetDlgItem(IDC_STATIC_SFNAME)->EnableWindow(0);
		GetDlgItem(IDC_SAMPCOMBO)->EnableWindow(0);
		GetDlgItem(IDC_EDIT1)->EnableWindow(0);
		GetDlgItem(IDC_STATIC_SOUNDFONT)->EnableWindow(0);
		GetDlgItem(IDC_STATIC_TEMPERAMENT)->EnableWindow(0);
		GetDlgItem(IDC_STATIC_SAMPRATE2)->EnableWindow(0);
		GetDlgItem(IDC_STATIC_SAMPRATE)->EnableWindow(0);
		((CButton*)GetDlgItem(IDC_automod))->EnableWindow(0);
		((CButton*)GetDlgItem(IDC_equal))->EnableWindow(0);
		((CButton*)GetDlgItem(IDC_pyt))->EnableWindow(0);
		((CButton*)GetDlgItem(IDC_just))->EnableWindow(0);
		break;
	default:
		((CButton*)GetDlgItem(IDC_RADIO2))->SetCheck(0);
		((CButton*)GetDlgItem(IDC_RADIO1))->SetCheck(1);
		((CButton*)GetDlgItem(IDC_RADIO3))->SetCheck(0);
		((CButton*)GetDlgItem(IDC_mrmode))->EnableWindow(1);
		((CButton*)GetDlgItem(IDC_rev))->EnableWindow(1);
		((CButton*)GetDlgItem(IDC_loadsf))->EnableWindow(1);
		((CButton*)GetDlgItem(IDC_erase))->EnableWindow(1);
		GetDlgItem(IDC_STATIC_SFNAME)->EnableWindow(1);
		GetDlgItem(IDC_SAMPCOMBO)->EnableWindow(1);
		GetDlgItem(IDC_EDIT1)->EnableWindow(1);
		GetDlgItem(IDC_STATIC_SOUNDFONT)->EnableWindow(1);
		GetDlgItem(IDC_STATIC_TEMPERAMENT)->EnableWindow(1);
		GetDlgItem(IDC_STATIC_SAMPRATE2)->EnableWindow(1);
		GetDlgItem(IDC_STATIC_SAMPRATE)->EnableWindow(1);
		switch (jhj56) {
		case 0:
			((CButton*)GetDlgItem(IDC_automod))->EnableWindow(0);
			break;
		default:
			((CButton*)GetDlgItem(IDC_automod))->EnableWindow(1);
			break;
		}
		((CButton*)GetDlgItem(IDC_equal))->EnableWindow(1);
		((CButton*)GetDlgItem(IDC_pyt))->EnableWindow(1);
		((CButton*)GetDlgItem(IDC_just))->EnableWindow(1);
		break;
	}
	switch (jhj56) {
	case 0:
		automod = true;
		((CButton*)GetDlgItem(IDC_automod))->EnableWindow(0);
		((CButton*)GetDlgItem(IDC_equal))->SetCheck(1);
		((CButton*)GetDlgItem(IDC_pyt))->SetCheck(0);
		((CButton*)GetDlgItem(IDC_just))->SetCheck(0);
		break;
	case 1:
		automod = false;
		((CButton*)GetDlgItem(IDC_automod))->EnableWindow(1);
		((CButton*)GetDlgItem(IDC_equal))->SetCheck(0);
		((CButton*)GetDlgItem(IDC_pyt))->SetCheck(1);
		((CButton*)GetDlgItem(IDC_just))->SetCheck(0);
		break;
	case 2:
		automod = true;
		((CButton*)GetDlgItem(IDC_automod))->EnableWindow(1);
		((CButton*)GetDlgItem(IDC_equal))->SetCheck(0);
		((CButton*)GetDlgItem(IDC_pyt))->SetCheck(1);
		((CButton*)GetDlgItem(IDC_just))->SetCheck(0);
		break;
	case 3:
		automod = false;
		((CButton*)GetDlgItem(IDC_automod))->EnableWindow(1);
		((CButton*)GetDlgItem(IDC_equal))->SetCheck(0);
		((CButton*)GetDlgItem(IDC_pyt))->SetCheck(0);
		((CButton*)GetDlgItem(IDC_just))->SetCheck(1);
		break;
	case 4:
		automod = true;
		((CButton*)GetDlgItem(IDC_automod))->EnableWindow(1);
		((CButton*)GetDlgItem(IDC_equal))->SetCheck(0);
		((CButton*)GetDlgItem(IDC_pyt))->SetCheck(0);
		((CButton*)GetDlgItem(IDC_just))->SetCheck(1);
		break;
	}
	
	int t;
	if (h54ui == 0)t = 0;
	else if (h54ui == 22050) t = 1;
	else if (h54ui == 44100)t = 2;
	else if (h54ui == 88200)t = 3;
	else if (h54ui == 176400)t = 4;
	else if (h54ui == 352800)t = 5;
	////printf("%d ", t);//printf("!");
	printf("m");
	((CComboBox*)(GetDlgItem(IDC_SAMPCOMBO)))->SetCurSel(t); ////printf("!");
	printf("n");
	tfval = flt2charp(turfreq);
	printf("o");
	UpdateData(0);
	printf("p");
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CDialogEx::OnShowWindow(bShow, nStatus);
}


void csettings::OnEqual()
{
	((CButton*)GetDlgItem(IDC_pyt))->SetCheck(0);
	((CButton*)GetDlgItem(IDC_just))->SetCheck(0);
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	GetDlgItem(IDC_automod)->EnableWindow(false);
	jhj56 = 0;
	automod = true;
	////printf("%d ",jhj56);
}


void csettings::OnPyt()
{
	((CButton*)GetDlgItem(IDC_equal))->SetCheck(0);
	((CButton*)GetDlgItem(IDC_just))->SetCheck(0);
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	GetDlgItem(IDC_automod)->EnableWindow(true);
	if(automod==true)
		jhj56 = 2;
	else jhj56 = 1;
	automod = true;
	////printf("%d ", jhj56);
}


void csettings::OnClickedAutomod()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	
	automod == true?automod = false:automod = true;
	if (((CButton*)GetDlgItem(IDC_pyt))->GetCheck() == BST_CHECKED) {
		if (automod == true)
			jhj56 = 2;
		else jhj56 = 1;
	}
	else {
		if (automod == true)
			jhj56 = 4;
		else jhj56 = 3;
	}
}


void csettings::OnSelchangeSampcombo()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(1);
	char t[20]; 
	((CComboBox *)(GetDlgItem(IDC_SAMPCOMBO)))->GetLBText(((CComboBox *)(GetDlgItem(IDC_SAMPCOMBO)))->GetCurSel(), t);
	h54ui = atoi(t);
	UpdateData(0);
}



/*
void csettings::OnFb()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	ghkj6 = 0;
}


void csettings::OnStf()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	ghkj6 = 1;
}


void csettings::OnClickedUsingpreview()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (j65j8 == 0)j65j8 = 1;
	else j65j8 = 0;
}*/




void csettings::OnClickedMrmode()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	mirror = (mirror == 0 ? 1 : 0);
	//printf("%d ", mirror);
}


//void csettings::OnClickedRev()
//{
//	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
//	reverse = (reverse == 0 ? 1 : 0);
//}



void csettings::OnJust()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	((CButton*)GetDlgItem(IDC_equal))->SetCheck(0);
	((CButton*)GetDlgItem(IDC_pyt))->SetCheck(0);
	GetDlgItem(IDC_automod)->EnableWindow(true);
	if (automod == true)
		jhj56 = 4;
	else jhj56 = 3;
	automod = true;
}


void csettings::OnKillfocusEdit1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(1);
	turfreq = atof(tfval);
	if (turfreq <= 400) if (kj768 == true) tfwan = "경고-주파수가 너무 낮음";
	else tfwan = "warning-frequency is too low";
	else if (turfreq >= 480) if (kj768 == true) tfwan = "경고-주파수가 너무 높음";
	else tfwan = "warning-turing frequency is too high";
	else tfwan = "";
	putini();
	UpdateData(0);
}


void csettings::OnChangeEdit1()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.
	UpdateData(1);
	turfreq = atof(tfval);
	if (turfreq == 0)turfreq = 440;
	if (turfreq <= 400) if (kj768 == true) tfwan = "경고-주파수가 너무 낮음";
	else tfwan = "warning-frequency is too low";
	else if (turfreq >= 480) if (kj768 == true) tfwan = "경고-주파수가 너무 높음";
	else tfwan = "warning-turing frequency is too high";
	else tfwan = "";
	putini();
	UpdateData(0);
	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}



void csettings::OnClickedRev()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	reverse = (reverse == 0 ? 1 : 0);
}







void csettings::OnStnClickedStaticSamprate3()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void csettings::OnBnClickedRadio1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	((CButton*)GetDlgItem(IDC_RADIO1))->SetCheck(1);
	((CButton*)GetDlgItem(IDC_RADIO2))->SetCheck(0);
	((CButton*)GetDlgItem(IDC_RADIO3))->SetCheck(0);
	midiout = 1;
	((CButton*)GetDlgItem(IDC_mrmode))->EnableWindow(1);
	((CButton*)GetDlgItem(IDC_rev))->EnableWindow(1);
	((CButton*)GetDlgItem(IDC_loadsf))->EnableWindow(1);
	((CButton*)GetDlgItem(IDC_erase))->EnableWindow(1);
	GetDlgItem(IDC_STATIC_SFNAME)->EnableWindow(1);
	GetDlgItem(IDC_SAMPCOMBO)->EnableWindow(1);
	GetDlgItem(IDC_EDIT1)->EnableWindow(1);
	GetDlgItem(IDC_STATIC_SOUNDFONT)->EnableWindow(1);
	GetDlgItem(IDC_STATIC_TEMPERAMENT)->EnableWindow(1);
	GetDlgItem(IDC_STATIC_SAMPRATE2)->EnableWindow(1);
	GetDlgItem(IDC_STATIC_SAMPRATE)->EnableWindow(1);
	switch (jhj56) {
	case 0:
		((CButton*)GetDlgItem(IDC_automod))->EnableWindow(0);
		break;
	default:
		((CButton*)GetDlgItem(IDC_automod))->EnableWindow(1);
		break;
	}
	((CButton*)GetDlgItem(IDC_equal))->EnableWindow(1);
	((CButton*)GetDlgItem(IDC_pyt))->EnableWindow(1);
	((CButton*)GetDlgItem(IDC_just))->EnableWindow(1);
}


void csettings::OnBnClickedRadio2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	((CButton*)GetDlgItem(IDC_RADIO1))->SetCheck(0);
	((CButton*)GetDlgItem(IDC_RADIO2))->SetCheck(1);
	((CButton*)GetDlgItem(IDC_RADIO3))->SetCheck(0);
	tfnm = 1;
	midiout = 0;
	mirror = 0;
	reverse = 0;
	mrmode = mirror;

	rev = reverse;
	((CButton*)GetDlgItem(IDC_mrmode))->SetCheck(0);
	((CButton*)GetDlgItem(IDC_rev))->SetCheck(0);
	((CButton*)GetDlgItem(IDC_mrmode))->EnableWindow(0);
	((CButton*)GetDlgItem(IDC_rev))->EnableWindow(0);
	((CButton*)GetDlgItem(IDC_loadsf))->EnableWindow(0);
	((CButton*)GetDlgItem(IDC_erase))->EnableWindow(0);
	GetDlgItem(IDC_STATIC_SFNAME)->EnableWindow(0);
	GetDlgItem(IDC_SAMPCOMBO)->EnableWindow(0);
	GetDlgItem(IDC_EDIT1)->EnableWindow(0);
	GetDlgItem(IDC_STATIC_SOUNDFONT)->EnableWindow(0);
	GetDlgItem(IDC_STATIC_TEMPERAMENT)->EnableWindow(0);
	GetDlgItem(IDC_STATIC_SAMPRATE2)->EnableWindow(0);
	GetDlgItem(IDC_STATIC_SAMPRATE)->EnableWindow(0);
	((CButton*)GetDlgItem(IDC_automod))->EnableWindow(0);
	((CButton*)GetDlgItem(IDC_equal))->EnableWindow(0);
	((CButton*)GetDlgItem(IDC_pyt))->EnableWindow(0);
	((CButton*)GetDlgItem(IDC_just))->EnableWindow(0);
}


void csettings::OnBnClickedRadio3()
{
	((CButton*)GetDlgItem(IDC_RADIO1))->SetCheck(0);
	((CButton*)GetDlgItem(IDC_RADIO2))->SetCheck(0);
	((CButton*)GetDlgItem(IDC_RADIO3))->SetCheck(1);
	tfnm = 0;
	midiout = 0;
	mirror = 0;
	reverse = 0;
	mrmode = mirror;

	rev = reverse;
	((CButton*)GetDlgItem(IDC_mrmode))->SetCheck(0);
	((CButton*)GetDlgItem(IDC_rev))->SetCheck(0);
	((CButton*)GetDlgItem(IDC_mrmode))->EnableWindow(0);
	((CButton*)GetDlgItem(IDC_rev))->EnableWindow(0);
	((CButton*)GetDlgItem(IDC_loadsf))->EnableWindow(0);
	((CButton*)GetDlgItem(IDC_erase))->EnableWindow(0);
	GetDlgItem(IDC_STATIC_SFNAME)->EnableWindow(0);
	GetDlgItem(IDC_SAMPCOMBO)->EnableWindow(0);
	GetDlgItem(IDC_EDIT1)->EnableWindow(0);
	GetDlgItem(IDC_STATIC_SOUNDFONT)->EnableWindow(0);
	GetDlgItem(IDC_STATIC_TEMPERAMENT)->EnableWindow(0);
	GetDlgItem(IDC_STATIC_SAMPRATE2)->EnableWindow(0);
	GetDlgItem(IDC_STATIC_SAMPRATE)->EnableWindow(0);
	((CButton*)GetDlgItem(IDC_automod))->EnableWindow(0);
	((CButton*)GetDlgItem(IDC_equal))->EnableWindow(0);
	((CButton*)GetDlgItem(IDC_pyt))->EnableWindow(0);
	((CButton*)GetDlgItem(IDC_just))->EnableWindow(0);
}
