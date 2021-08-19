

#include "extern.h"
#include "stdafx.h"
#include "cube.h"
#include "cubedoc.h"
#include "cubeview.h"
#include "mainfrm.h"
#include <gl\GL.h>
#include "strgrp.h"
#include <stdio.h>
#include <string.h>
#include <Windows.h>
#include <math.h>
#include <time.h>
#include <omp.h>
#include <io.h>
#include "resource.h"
#include "csettings.h"
#include "soundfont.h"
#include "gm.h"
#include <gdiplus.h>
#include <mmsystem.h>
#include <locale.h>
#include <assert.h>
#pragma comment(lib,"Gdiplus.lib")
#pragma comment(lib,"winmm")
#pragma warning(disable:4996)
#pragma warning(disable:4244)

#ifndef printf
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#endif
#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;

void CCubeView::AssertValid() const
{
	CView::AssertValid();
}

void CCubeView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CCubeDoc* CCubeView::GetDocument() // non-debug version is inline
{
	return STATIC_DOWNCAST(CCubeDoc, m_pDocument);
}
#endif 
#define del 0
#define MIN(a,b) (a<b)?a:b
#define MAX(a,b) (a>b)?a:b
#define getr(x) ((x&0x00FF0000) >> 0x10)
#define getg(x) ((x&0x0000FF00) >> 0x08)
#define getb(x) ((x&0x000000FF))
#define wjputs(f) fwrite(opbuf, 1, opbufcnt, f)
#define cc(x,lv) ((lv != 0) ? x + (1 - x) / (float)(lv + 1):x)
#define print2st(val) (((CMainFrame*)AfxGetApp()->m_pMainWnd)->m_wndStatusBar).SetPaneText(0, val);
#define glBegin2(id) (glcounter[id]=0)
#define glBegin3(id) (glVertexPointer(3, GL_FLOAT, 0, glbuffer[id]))
#define glBegin_common2() glEnableClientState(GL_VERTEX_ARRAY)
#define glVertex3f2(x,y,z,id) (glbt=(float*)glbuffer[id][glcounter[id]++],glbt[0]=x,glbt[1]=y,glbt[2]=z)
#define glEnd2(id)  glDrawArrays(GL_QUADS,0,glcounter[id])
#define glEnd_common2() glDisableClientState(GL_VERTEX_ARRAY)
inline void pauseall();
void DrawScene();
int exit2(int iex);
void mtinit2();



struct chbuf { char c[8000]; char cb; };
struct wavFileHeader
{
	long chunkId;           //"RIFF" (0x52,0x49,0x46,0x46)
	long chunkSize;         // (fileSize - 8)  - could also be thought of as bytes of data in file following this field (bytesRemaining)
	long riffType;          // "WAVE" (0x57415645)
};
struct fmtChunk
{
	long chunkId;                       // "fmt " - (0x666D7420)
	long chunkDataSize;                 // 16 + extra format bytes
	short compressionCode;              // 1 - 65535
	short numChannels;                  // 1 - 65535
	long sampleRate;                    // 1 - 0xFFFFFFFF
	long avgBytesPerSec;                // 1 - 0xFFFFFFFF
	short blockAlign;                   // 1 - 65535
	short significantBitsPerSample;     // 2 - 65535
	short extraFormatBytes;             // 0 - 65535
};
struct wavChunk
{
	long chunkId;
	long chunkDataSize;
};
union int642char { unsigned long long i = 0; char c[8]; };
union int2char { unsigned int i = 0; char c[4]; };
union _playmidit { unsigned char bf[4]; int me; };

	char mdtmpvel[2048];
	char mdtmpvel8[2048];
	char mdtmpoff[2048];
	char mdtmpwrite[12288];
	int mdtmpsize;

	void mdtmpreset() {
		mdtmpsize = 0;
		memset(mdtmpvel, 0, 2048);
		memset(mdtmpvel8, 0, 2048);
		memset(mdtmpoff, 0, 2048);
	}
	
	void mdtmpsort() {
		
		mdtmpsize = 0;
		for (int i = 0; i < 2048; i++) {
			if (mdtmpoff[i]) {
				mdtmpwrite[mdtmpsize++] = mdtmpoff[i] + (i >> 7);
				mdtmpwrite[mdtmpsize++] = (i & 127);
				mdtmpwrite[mdtmpsize++] = 0;
			}
		}
		for (int i = 0; i < 2048; i++) {
			if (mdtmpvel[i] > 80) {
				mdtmpwrite[mdtmpsize++] = mdtmpvel8[i] + (i >> 7);
				mdtmpwrite[mdtmpsize++] = (i & 127);
				mdtmpwrite[mdtmpsize++] = mdtmpvel[i];
			}
		}
		for (int i = 0; i < 2048; i++) {
			if (mdtmpvel[i] > 40&&80>= mdtmpvel[i]) {
				mdtmpwrite[mdtmpsize++] = mdtmpvel8[i] + (i >> 7);
				mdtmpwrite[mdtmpsize++] = (i & 127);
				mdtmpwrite[mdtmpsize++] = mdtmpvel[i];
			}
		}
		for (int i = 0; i < 2048; i++) {
			if ( 40 >= mdtmpvel[i] && mdtmpvel[i]) {
				mdtmpwrite[mdtmpsize++] = mdtmpvel8[i] + (i >> 7);
				mdtmpwrite[mdtmpsize++] = (i & 127);
				mdtmpwrite[mdtmpsize++] = mdtmpvel[i];
			}
		}
		
		

	}

class AUX_RGBImageRec {

	void convertBGRtoRGB()
	{
		const DWORD BitmapLength = sizeX * sizeY * 3;
		byte Temp;
		for (DWORD i = 0; i< BitmapLength; i += 3)
		{
			Temp = data[i];
			data[i] = data[i + 2];
			data[i + 2] = Temp;
		}
	}
public:
	byte *data;
	DWORD sizeX;
	DWORD sizeY;
	bool NoErrors;
	AUX_RGBImageRec() : NoErrors(false), data(NULL) {};
	AUX_RGBImageRec(const char *FileName) : data(NULL), NoErrors(false)
	{
		loadFile(FileName);
	}
	~AUX_RGBImageRec()
	{
		if (data != NULL) delete data;
		data = NULL;
	}
	bool loadFile(const char* Filename)
	{
		BITMAPINFO BMInfo;								// need the current OpenGL device contexts in order to make use of windows DIB utilities  
		const HDC gldc = wglGetCurrentDC();   			// a handle for the current OpenGL Device Contexts
														// assume there are errors until file is loaded successfully into memory  
		NoErrors = false;  								// release old data since this object could be used to load multiple Textures  
		if (data != NULL) delete data;					// windows needs this info to determine what header info we are looking for  
		BMInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);  // Get windows to determine color bit depth in the file for us  
		BMInfo.bmiHeader.biBitCount = 0;				// Get windows to open and load the BMP file and handle the messy decompression if the file is compressed  
														// assume perfect world and no errors in reading file, Ha Ha  
		HANDLE DIBHandle = LoadImage(0, Filename, IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR | LR_CREATEDIBSECTION | LR_LOADFROMFILE);  // use windows to get header info of bitmap - assume no errors in header format 

		GetDIBits(gldc, (HBITMAP)DIBHandle, 0, 0, NULL, &BMInfo, DIB_RGB_COLORS);
		sizeX = BMInfo.bmiHeader.biWidth;
		sizeY = BMInfo.bmiHeader.biHeight;				// change color depth to 24 bits (3 bytes (BGR) / pixel)  
		BMInfo.bmiHeader.biBitCount = 24;				// don't want the data compressed  
		BMInfo.bmiHeader.biCompression = BI_RGB;
		const DWORD BitmapLength = sizeX * sizeY * 3;	// 3 bytes (BGR) per pixel (24bp)  
														// allocate enough memory to hold the pixel data in client memory  
		data = new byte[BitmapLength];					// Get windows to do the dirty work of converting the BMP into the format needed by OpenGL  
														// if file is already 24 bit color then this is a waste of time but makes for short code  
														// Get the actual Texel data from the BMP object  

		if (GetDIBits(gldc, (HBITMAP)DIBHandle, 0, sizeY, data, &BMInfo, DIB_RGB_COLORS))
		{
			NoErrors = true;
			convertBGRtoRGB();							// NOTE: BMP is in BGR format but OpenGL needs RGB unless you use GL_BGR_EXT
		}

		DeleteObject(DIBHandle);						// don't need the BMP Object anymore  
		return NoErrors;
	}
	friend AUX_RGBImageRec *auxDIBImageLoad(const char *FileName);
};

AUX_RGBImageRec *pTextureImage;
wavChunk mDataChunk;
WAVEFORMATEX wf;
_playmidit playmidit;
WAVEHDR wh;
HWAVEOUT hWaveOut;
int642char offs;
HANDLE thread1;
HMIDIOUT playmidiout;
csettings4 st4;
int2char wjct2;
chbuf *chbuffer;
FILE *wj, *wj2, *wj3, *wj6, *wj7, *i, *wj4, *ch, *lrcf, *playmidif, *playmidig;
int fp2,speedupdate=0;
char afqwfq[384] = { 0 };
bool ab=0,bt, gtmp, gmea, mov, dbc, lrcb, ps, fs, skip, ppmy, txtoff, playrd100t, playrd100t2, *buf, keysig2, stop, damppad[16], softpad[16], sti3, stop2, g3446 = true, atmod, wjaa, updta, vol, strclrc, ply = true, f, fnm, pb, quit, htit, mmp, forcenomidi, sdfsdf, exbl;
char t[10000], filename1[10000], filename[10000], filename2[10000], filename3[10000], filename4[10000], filename6[10000], filename5[10000], filename8[1025], filename9[400], *buffer, opbuf[8000], mdstrings[2000], chrinp[100], ktemp[1056], ktt, lrcc1[2000], lrcc2[2000], lrcc3[2000], filename7[10000], argv[1000], tj[1000], imgname[1000], mdt[500][268][16], p[100], *midname, *midname2, str[10000], midstr[1000], ofilename2[1000], filename0[1000];
short pc2[16], *key5[128], stm, midipan;
int windh, windv, ox, oy, frqt, gmetmp2, midipitchbend[16][2], mdtpnt2, owindx, owindy, monce, onelp, pitchint[16], q, ips, ro, midipitchbend2[16], midipitchbend_[16], pitchint2[16], midipitch, tmpmode, lt, glcounter[8], wavestate, windx, windy, ocl, bfps, fps, txtys, key3[128][16], keysust[128][16], *oldstate, midibx[128][16], playrd10, chn = -1, yryer, opbufcnt, oks, ipaused[16], bpaused[16], cpaused[16], rpaused[16], mpaused[16], j5gyj, chrcnt, bth = 4, btl = 4, keysig = 8, *chbuffercnt, textmode, windowx, windowy, mode, ii, stringcount, mdtpnt, brebe, txtmd2, tpi, knamei, kfreqi, hortxti, mirror, reverse, trand, asfaq, mpoffset, setmidivol, fspace = 100, isplay, midibank[16], fmbank[16], midipreset[16], fmpreset[16], ombank[16], ompreset[16], bpshf;
unsigned int tex, playrate = 22050, tempo = 120, deltatime;
long gtc, ldpdt, oldpdt, doldpdt, playrd100, modulationwheel[16], fmodulationwheel[16], fchorus[16], freverb[16], pbendratio[16], chnvolt[16], footcontr[16], breathcontr[16], pan[16], owptr, owptr6, chorus[16], reverb[16];
unsigned long thread1d, thread1e, clock_start;
float glbuffer[8][2008][3], *glbt, h45h7 , gasfw , hg4h5, stretchx = 1.0f, stretchy = 1.0f, stretchz = 1.0f, x , y, turfreq = 440, strclr[4], j5jgj = 1, or , og, tr = 0.5, tg = 0.5, tb = 0.5, ta = 1, mdtpnt3, upbl = 0.1, dwbl = 0.3, erhwh[16], iyybt[16], trity[16], sadae[16], erhwhw2[16][8], iyybts2[16][8], trityu2[16][8];
long long offs2, lrct3, lrct4, midisize, pnote, wptr, owptr4, owptr5, mptr, *le, *le2, *chlnt, *fileptr, nplayoffset, playoffset, onp1;
unsigned long long oplaymidii, gmeo,   lud, playmidin0, playlength, wj6startpos2, playmidii, playmidij, playmidini, playmidinj, note, owptr2;
double lrct1, lrct2, _clock2, clocks, clocks2, f346h = 1., playvolume = 1.0f, pitcherror[16][128], chvol[16], key[128][16], dchorus[16], dreverb[16], rmodulationwheel[16], volume2[16], spd2=100,spd = 100, loop = 0., pitch[16], rpitch[16], pc3[16], dwp, mdvol2[3][16], mdvol3[128][16], dplayrate, pitcherr[128][16], pdt, dpdt, wjct, wjcid2[128], wjca;
volatile unsigned long long gmeaptr, gmeaptr2;
AUX_RGBImageRec *auxDIBImageLoad(const char *FileName)
{
	return new AUX_RGBImageRec(FileName);
}
AUX_RGBImageRec * LoadBMPFile(char *filename)
{
	FILE *hFile = NULL;
	if (!filename) return NULL;

	hFile = fopen(filename, "r");
	if (hFile) {
		fclose(hFile);
		return auxDIBImageLoad(filename);
	}

	return NULL;

}
inline void revfile(char* filename) {
	char y[10000];
	FILE *r, *w;
	strcpy(y, filename);
	r = fopen(y, "rb");
	sprintf(y, "%st", filename);
	w = fopen(y, "wb");
	for (int i = 0; i < 4; i++) {
		fputc(fgetc(r), w);
	}
	fputc(fgetc(r) == 1 ? 0 : 1, w);
	unsigned long long t;
	_fseeki64(r, -8, SEEK_END);
	fread(&t, 8, 1, r);
	_fseeki64(r, t, SEEK_SET);
	long long rt = t;
	_fseeki64(r, -1056, SEEK_CUR); rt -= 1056;
	while (1) {
		fread(y, 1, 1056, r);
		rt += 1056;
		fwrite(y, 1, 1056, w);
		_fseeki64(r, -2112, SEEK_CUR);
		rt -= 2112;
		if (rt<0)break;
	}
	//("?");
	_fseeki64(r, -12, SEEK_END);
	rt = _ftelli64(r);
	while (1) {
		int ti = 0, ti2 = 0;
		while (1) {
			//_fseeki64(r, -1, SEEK_CUR);
			if (rt < t) break;
			rt++;
			if (fgetc(r) == 0x0A) ti++;

			if (ti == 6) break;
			//
			rt -= 2;
			_fseeki64(r, -2, SEEK_CUR);
			ti2++;
		}
		fread(y, 1, ti2, r);
		fwrite(y, 1, ti2, w);
		_fseeki64(r, -ti2 - 1, SEEK_CUR);
		rt--;
		if (_ftelli64(r) < t) break;
	}
	fputc(0x0a, w);
	fwrite("eof", 1, 3, w);
	fwrite(&t, 8, 1, w);
	fclose(r);
	fclose(w);
	sprintf(y, "del \"%s\"", filename);
	system(y);
	Sleep(100);
	char *t2;
	for (int i = strlen(filename); i >= 0; i--) {
		if (filename[i] == '\\') {
			t2 = &(filename[i + 1]);

			break;
		}
	}
	sprintf(y, "ren \"%st\" \"%s\"", t2, t2);
	//("%s", y);
	system(y);
}
inline void revfile2(char* filename) {
	char y[10000];
	FILE *r, *w;
	strcpy(y, filename);
	r = fopen(y, "rb");
	sprintf(y, "%st", filename);
	w = fopen(y, "wb");
	for (int i = 0; i < 0x2c; i++) {
		fputc(fgetc(r), w);
	}
	fseek(r, -2, SEEK_END);
	while (1) {
		fputc(fgetc(r), w);
		fputc(fgetc(r), w);
		fseek(r, -4, SEEK_CUR);
		if (ftell(r) < 0x2c) break;
	}
	fclose(r);
	fclose(w);
	sprintf(y, "del \"%s\"", filename);
	system(y);
	Sleep(100);
	char *t2;
	for (int i = strlen(filename); i >= 0; i--) {
		if (filename[i] == '\\') {
			t2 = &(filename[i + 1]);

			break;
		}
	}
	sprintf(y, "ren \"%st\" \"%s\"", t2, t2);
	system(y);
}
void putini() {
	FILE *ini;
	intfloat t[16];
	t[0].a = h45h7;
	t[1].a = gasfw;
	t[2].a = hg4h5;
	t[3].a = tr;
	t[4].a = tg;
	t[5].a = tb;
	t[6].a = ta;
	t[7].a = turfreq;
	t[8].a = x;
	t[9].a = y;
	t[10].a = stretchx;
	t[11].a = stretchy;
	t[12].a = stretchz;
	t[13].a = upbl;
	t[14].a = dwbl;
	t[15].a = j5jgj;
	char stf[1000];
	GetModuleFileNameA(NULL, stf, 1000);
	for (int i = strlen(stf) - 1; i >= 0; i--)
		if (stf[i] == '\\') { stf[i] = 0; break; }
	strcat(stf, "\\hmp6.ini");
	if ((ini = fopen(stf, "w")) != NULL) {
		sprintf(recentfilelist[12], "%d %d %d %d %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x", windowx, windowy, windh, windv, mmp, t[0].b, t[1].b, t[2].b, t[3].b, t[4].b, t[5].b, t[6].b, asfaq, brebe, textmode, h54ui, jhj56, ghkj6, j65j8, trand, mirror, reverse, knamei, kfreqi, hortxti, t[7].b, t[8].b, t[9].b, t[10].b, fsize, fspace, t[11].b, t[12].b, t[13].b, t[14].b, txtmd2, t[15].b);
		for (int i = 0; i < 13; i++) { fprintf(ini, "%s_\n", recentfilelist[i]); }
		fprintf(ini, "%d\n", j5gyj);
		for (int i = 0; i < 16; i++) { t[0].a = erhwh[i]; t[1].a = iyybt[i]; t[2].a = trity[i]; t[3].a = sadae[i]; fprintf(ini, "%x %x %x %x\n", t[0].b, t[1].b, t[2].b, t[3].b); }
		fprintf(ini, "%s_\n", recentfilelist[13]);
		fclose(ini);
	}
}
inline void getini() {
	FILE *ini;
	char stf[1000];
	GetModuleFileNameA(NULL, stf, 1000);
	for (int i = strlen(stf) - 1; i >= 0; i--)
		if (stf[i] == '\\') { stf[i] = 0; break; }
	strcat(stf, "\\hmp6.ini");
	if (access(stf, 0) != 0) {
	
		st4.ShowWindow(SW_SHOW);
		
		windh = 1024;
		windv = 768;
		
		turfreq = 440.0f;
		fsize = 20; fspace = 100;
		strcpy(recentfilelist[13], "Arial");
		
		ini = fopen(stf, "w");
		fprintf(ini, "_\n_\n_\n_\n_\n_\n_\n_\n_\n_\n_\n_\n100 100 1024 768 0 0 0 0 3f000000 3f000000 3f000000 3f800000 15e 64 0 0 2 0 64 0 0 0 0 0 0 43dc0000 0 0 3f800000 14 64 3f800000 3f800000 3dcccccd 3e99999a 0 3f800000_\n1\n3f7f0000 0 3efe0000 3f7f0000\n3eee0000 3f350000 3f7e0000 3f7f0000\n3f7f0000 3f5f0000 0 3f7f0000\n3f3f0000 3f7f0000 0 3f7f0000\n3efe0000 0 3f7f0000 3f7f0000\n0 3f7f0000 3f7f0000 3f7f0000\n3f600000 3f300000 3f7f0000 3f7f0000\n3f7f0000 3f650000 3f340000 3f7f0000\n3f230000 3f410000 3f2d0000 3f7f0000\n3f400000 3f400000 3f400000 3f7f0000\n3f410000 3f1a0000 3ed60000 3f7f0000\n3f7e0000 3e9c0000 3f5a0000 3f7f0000\n3f700000 3f7f0000 3f700000 3f7f0000\n3ea00000 3f480000 3ef00000 3f7f0000\n3f7d0000 3f7f0000 0 3f7f0000\n3f670000 3f7e0000 3f7f0000 3f7f0000\nArial_");
		fclose(ini);
		Sleep(10);
		ini = fopen(stf, "r");

	}

	if ((ini = fopen(stf, "r")) == NULL) return;

	for (int i = 0; i < 13; i++) {
		fscanf(ini, " %[^\n]", recentfilelist[i]);
		recentfilelist[i][strlen(recentfilelist[i]) - 1] = 0;
	}
	fscanf(ini, " %d", &j5gyj);
	intfloat t[16];
	if (j5gyj == 1) {
		for (int i = 0; i < 16; i++) {
			fscanf(ini, " %x %x %x %x", &t[0].b, &t[1].b, &t[2].b, &t[3].b);   erhwh[i] = t[0].a;  iyybt[i] = t[1].a; trity[i] = t[2].a; sadae[i] = t[3].a;
		}
	}
	else {
		int j;
		for (int i = 0; i < 16; i++) {
			fscanf(ini, " %*x %*x %*x %*x");
		}
	}
	fscanf(ini, " %[^\n]", recentfilelist[13]);
	recentfilelist[13][strlen(recentfilelist[13]) - 1] = 0;
	FILE *tmp;
	int a = 0;
	for (int i = 0; i < 12; i++) {
		if ((tmp = fopen(recentfilelist[i], "r")) == NULL) {
			a = 1;
			recentfilelist[i][0] = 0;
		}
		else fclose(tmp);
	}
	fclose(ini);

	fsize = 0; fspace = 0;
	sscanf(recentfilelist[12], " %d %d %d %d %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x", &windowx, &windowy, &windh, &windv, &mmp, &t[0].b, &t[1].b, &t[2].b, &t[3].b, &t[4].b, &t[5].b, &t[6].b, &asfaq, &brebe, &textmode, &h54ui, &jhj56, &ghkj6, &j65j8, &trand, &mirror, &reverse, &knamei, &kfreqi, &hortxti, &t[7].b, &t[8].b, &t[9].b, &t[10].b, &fsize, &fspace, &t[11].b, &t[12].b, &t[13].b, &t[14].b, &txtmd2, &t[15].b);
	if (a)putini();
	h45h7 = t[0].a;
	gasfw = t[1].a;
	hg4h5 = t[2].a;
	tr = t[3].a;
	tg = t[4].a;
	tb = t[5].a;
	ta = t[6].a;
	turfreq = t[7].a;
	x = t[8].a;
	y = t[9].a;
	stretchx = t[10].a;
	stretchy = t[11].a;
	stretchz = t[12].a;
	upbl = t[13].a;
	dwbl = t[14].a;
	j5jgj = t[15].a;

	if (stretchx < 0.00099 || stretchx >1001. || stretchy < 0.00099 || stretchy >1001.|| stretchz < 0.00099 || stretchz >1001.) {
		Sleep(10);
		
		char p[1000];
		
		sprintf(p, "del \"%s\"", stf);
		system(p);
		Sleep(100);
		getini();
	}
}
inline int LoadGLTextures(void) {
	AUX_RGBImageRec *texRec; //<1>
	if ((texRec = LoadBMPFile(imgname))) {
		glGenTextures(3, &tex); 
		glBindTexture(GL_TEXTURE_2D, tex);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //<7>
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 
		glTexImage2D(GL_TEXTURE_2D, 0, 3, texRec->sizeX, texRec->sizeY, 0,GL_RGB, GL_UNSIGNED_BYTE, texRec->data); //<9>
	}
	else return FALSE;
	if (texRec) {
		if (texRec->data) free(texRec->data);
		free(texRec);
	}
	else return FALSE;
	glEnable(GL_TEXTURE_2D);
	return TRUE;
}
inline void wjput(char* g, FILE *f) {

	opbuf[opbufcnt++] = g[2];

	opbuf[opbufcnt++] = g[3];
	if (opbufcnt == 8000) {
		opbufcnt = 0; fwrite(opbuf, 1, 8000, f);
	}
}
inline int sjgf(FILE* file, int id) {
	int temp;
	if (chbuffercnt[id] >= 0) temp = chbuffer[id].c[chbuffercnt[id]];
	else if (chbuffercnt[id] == -1) temp = chbuffer[id].cb;
	else {
		_fseeki64(file, fileptr[id], SEEK_SET);

		long long loadsize = midisize - fileptr[id];
		if (loadsize > 8000)loadsize = 8000;
		fread(chbuffer[id].c, 1, loadsize, file);
		fileptr[id] += loadsize;
		chbuffercnt[id] = 0;
		temp = chbuffer[id].c[0] & 0x00FF;
	}


	if (chbuffercnt[id] == 7999) {
		chbuffercnt[id] = -1;
		chbuffer[id].cb = chbuffer[id].c[7999]; _fseeki64(file, fileptr[id], SEEK_SET);
		long long loadsize = midisize - fileptr[id];
		if (loadsize > 8000)loadsize = 8000;
		fread(chbuffer[id].c, 1, loadsize, file);
		fileptr[id] += loadsize;
	}
	chbuffercnt[id]++;
	chlnt[id]--;
	if (chlnt[id] < 0) buf[id] = true;
	temp &= 0x00FF;
	return temp;
}
inline double freq2(int a) {
	int b = a - 0x45;
	double c = 440;
	while (1) {
		if (b > 12) { b -= 12; c *= 2; }
		else if (b < -12) { b += 12; c /= 2; }
		else if (b > 4) { b -= 4; c *= 1.2599210498948731647672106072782; }
		else if (b < -4) { b += 4; c /= 1.2599210498948731647672106072782; }
		else if (b > 0) { b--; c *= 1.0594630943592952645618252949463; }
		else if (b < 0) { b++; c /= 1.0594630943592952645618252949463; }
		else break;
	}
	return c;

}
inline double freq(int a) {
	if (jhj56 == 2 || jhj56 == 4) atmod = true;
	else atmod = false;
	if (jhj56 == 0) {

		int b = a - 0x45;
		double c = 440.;
		while (1) {
			if (b > 12) { b -= 12; c *= 2; }
			else if (b < -12) { b += 12; c /= 2; }
			else if (b > 4) { b -= 4; c *= 1.2599210498948731647672106072782; }
			else if (b < -4) { b += 4; c /= 1.2599210498948731647672106072782; }
			else if (b > 0) { b--; c *= 1.0594630943592952645618252949463; }
			else if (b < 0) { b++; c /= 1.0594630943592952645618252949463; }
			else break;
		}
		return c*turfreq / 440.;
	}
	else if (jhj56>2) {
		frqt = a % 12;
		if (frqt == 9) {
			int b = a - 69;
			double c = pure[0][(atmod == true) ? keysig : 8];
			while (1) {
				if (b >= 12) { b -= 12; c *= 2; }
				else if (b <= -12) { b += 12; c /= 2; }
				else break;
			}
			return c*turfreq / 440.;
		}
		else if (frqt == 10) {
			int b = a - 70;
			double c = pure[1][(atmod == true) ? keysig : 8];

			while (1) {
				if (b >= 12) { b -= 12; c *= 2; }
				else if (b <= -12) { b += 12; c /= 2; }
				else break;
			}
			return c*turfreq / 440.;
		}
		else if (frqt == 11) {
			int b = a - 71;
			double c = pure[2][(atmod == true) ? keysig : 8];
			while (1) {
				if (b >= 12) { b -= 12; c *= 2; }
				else if (b <= -12) { b += 12; c /= 2; }
				else break;
			}
			return c*turfreq / 440.;
		}
		else if (frqt == 0) {
			int b = a - 72;
			double c = pure[3][(atmod == true) ? keysig : 8];
			while (1) {
				if (b >= 12) { b -= 12; c *= 2; }
				else if (b <= -12) { b += 12; c /= 2; }
				else break;
			}
			return c*turfreq / 440.;
		}
		else if (frqt == 1) {
			int b = a - 73;
			double c = pure[4][(atmod == true) ? keysig : 8];
			while (1) {
				if (b >= 12) { b -= 12; c *= 2; }
				else if (b <= -12) { b += 12; c /= 2; }
				else break;
			}
			return c*turfreq / 440.;
		}
		else if (frqt == 2) {
			int b = a - 74;
			double c = pure[5][(atmod == true) ? keysig : 8];
			while (1) {
				if (b >= 12) { b -= 12; c *= 2; }
				else if (b <= -12) { b += 12; c /= 2; }
				else break;
			}
			return c*turfreq / 440.;
		}
		else if (frqt == 3) {
			int b = a - 75;
			double c = pure[6][(atmod == true) ? keysig : 8];
			while (1) {
				if (b >= 12) { b -= 12; c *= 2; }
				else if (b <= -12) { b += 12; c /= 2; }
				else break;
			}
			return c*turfreq / 440.;
		}
		else if (frqt == 4) {
			int b = a - 76;
			double c = pure[7][(atmod == true) ? keysig : 8];
			while (1) {
				if (b >= 12) { b -= 12; c *= 2; }
				else if (b <= -12) { b += 12; c /= 2; }
				else break;
			}
			return c*turfreq / 440.;
		}
		else if (frqt == 5) {
			int b = a - 77;
			double c = pure[8][(atmod == true) ? keysig : 8];
			while (1) {
				if (b >= 12) { b -= 12; c *= 2; }
				else if (b <= -12) { b += 12; c /= 2; }
				else break;
			}
			return c*turfreq / 440.;
		}
		else if (frqt == 6) {
			int b = a - 78;
			double c = pure[9][(atmod == true) ? keysig : 8];
			while (1) {
				if (b >= 12) { b -= 12; c *= 2; }
				else if (b <= -12) { b += 12; c /= 2; }
				else break;
			}
			return c*turfreq / 440.;
		}
		else if (frqt == 7) {
			int b = a - 79;
			double c = pure[10][(atmod == true) ? keysig : 8];
			while (1) {
				if (b >= 12) { b -= 12; c *= 2; }
				else if (b <= -12) { b += 12; c /= 2; }
				else break;
			}
			return c*turfreq / 440.;
		}
		else {
			int b = a - 80;
			double c = pure[11][(atmod == true) ? keysig : 8];
			while (1) {
				if (b >= 12) { b -= 12; c *= 2; }
				else if (b <= -12) { b += 12; c /= 2; }
				else break;
			}
			return c*turfreq / 440.;
		}
		//
	}
	else {//69번키가 라
		frqt = a % 12;
		if (frqt == 9) {
			int b = a - 69;
			double c = pyt[0][(atmod == true) ? keysig : 8];
			while (1) {
				if (b >= 12) { b -= 12; c *= 2; }
				else if (b <= -12) { b += 12; c /= 2; }
				else break;
			}
			return c*turfreq / 440.;
		}
		else if (frqt == 10) {
			int b = a - 70;
			double c = pyt[1][(atmod == true) ? keysig : 8];

			while (1) {
				if (b >= 12) { b -= 12; c *= 2; }
				else if (b <= -12) { b += 12; c /= 2; }
				else break;
			}
			return c*turfreq / 440.;
		}
		else if (frqt == 11) {
			int b = a - 71;
			double c = pyt[2][(atmod == true) ? keysig : 8];
			while (1) {
				if (b >= 12) { b -= 12; c *= 2; }
				else if (b <= -12) { b += 12; c /= 2; }
				else break;
			}
			return c*turfreq / 440.;
		}
		else if (frqt == 0) {
			int b = a - 72;
			double c = pyt[3][(atmod == true) ? keysig : 8];
			while (1) {
				if (b >= 12) { b -= 12; c *= 2; }
				else if (b <= -12) { b += 12; c /= 2; }
				else break;
			}
			return c*turfreq / 440.;
		}
		else if (frqt == 1) {
			int b = a - 73;
			double c = pyt[4][(atmod == true) ? keysig : 8];
			while (1) {
				if (b >= 12) { b -= 12; c *= 2; }
				else if (b <= -12) { b += 12; c /= 2; }
				else break;
			}
			return c*turfreq / 440.;
		}
		else if (frqt == 2) {
			int b = a - 74;
			double c = pyt[5][(atmod == true) ? keysig : 8];
			while (1) {
				if (b >= 12) { b -= 12; c *= 2; }
				else if (b <= -12) { b += 12; c /= 2; }
				else break;
			}
			return c*turfreq / 440.;
		}
		else if (frqt == 3) {
			int b = a - 75;
			double c = pyt[6][(atmod == true) ? keysig : 8];
			while (1) {
				if (b >= 12) { b -= 12; c *= 2; }
				else if (b <= -12) { b += 12; c /= 2; }
				else break;
			}
			return c*turfreq / 440.;
		}
		else if (frqt == 4) {
			int b = a - 76;
			double c = pyt[7][(atmod == true) ? keysig : 8];
			while (1) {
				if (b >= 12) { b -= 12; c *= 2; }
				else if (b <= -12) { b += 12; c /= 2; }
				else break;
			}
			return c*turfreq / 440.;
		}
		else if (frqt == 5) {
			int b = a - 77;
			double c = pyt[8][(atmod == true) ? keysig : 8];
			while (1) {
				if (b >= 12) { b -= 12; c *= 2; }
				else if (b <= -12) { b += 12; c /= 2; }
				else break;
			}
			return c*turfreq / 440.;
		}
		else if (frqt == 6) {
			int b = a - 78;
			double c = pyt[9][(atmod == true) ? keysig : 8];
			while (1) {
				if (b >= 12) { b -= 12; c *= 2; }
				else if (b <= -12) { b += 12; c /= 2; }
				else break;
			}
			return c*turfreq / 440.;
		}
		else if (frqt == 7) {
			int b = a - 79;
			double c = pyt[10][(atmod == true) ? keysig : 8];
			while (1) {
				if (b >= 12) { b -= 12; c *= 2; }
				else if (b <= -12) { b += 12; c /= 2; }
				else break;
			}
			return c*turfreq / 440.;
		}
		else {
			int b = a - 80;
			double c = pyt[11][(atmod == true) ? keysig : 8];
			while (1) {
				if (b >= 12) { b -= 12; c *= 2; }
				else if (b <= -12) { b += 12; c /= 2; }
				else break;
			}
			return c*turfreq / 440.;
		}
		//
	}
}
inline void sppmy() {

	for (int i = 0; i < 16; i++) {
		for (int k = 0; k < 128; k++) {
			pitcherr[k][i] = freq(k)*rpitch[i] / freq2(k);
		}
	}


}
inline void noteoff(int wyr, int midichennel) {
	if (mirror)wyr = 0x7F - wyr;
	if (midichennel != 9)releasenote(midichennel, wyr);
	key3[wyr][midichennel] = 0;

}
inline void noteon(int wyr, int vel, int chennel, int midichennel) {
	ombank[midichennel] = 1;
	if (mirror)wyr = 0x7F - wyr;
	note++;
	setnote(midichennel, wyr, vel, (double)vel / 128.*mdvol3[wyr][midichennel]);
	key3[wyr][midichennel] = ((chennel & 7) != 0) ? (chennel & 7) : 7;
}
inline void noteon2(int wyr, int midichennel) {
	if (mirror)wyr = 0x7F - wyr;
	key3[wyr][midichennel] = 0;
}

inline void getmidieventacc() {

	for (int j = 0; j < chn; j++) {
		unsigned int tmp2;
		unsigned int tmp = 0, b = 0;
		unsigned int c = 0,e=0;
		unsigned long long d = 0;
		if (mptr == le[j] + le2[j] && buf[j] == false) {

			le[j] += le2[j];

			tmp = sjgf(ch, j);

		C2:

			gmetmp2 = tmp & 15;
			if (tmp <= 0x7F) { //("!"); 
				chbuffercnt[j]--; chlnt[j]++;  tmp = oldstate[j]; goto C2;
			}
			else if (tmp >= 0x80 && tmp <= 0x8F) {
				oldstate[j] = tmp;
				b = sjgf(ch, j);
				c = sjgf(ch, j);
				if (keysust[b][gmetmp2] == 0) noteoff(b, gmetmp2);
				else noteon2(b, gmetmp2);

			}
			else if (tmp >= 0x90 && tmp <= 0x9F) {
				oldstate[j] = tmp;
				b = sjgf(ch, j);
				c = sjgf(ch, j);
				if (c)   noteon(b, softpad[gmetmp2] ? (c >> 1) : c, j, gmetmp2);
				else if (damppad[gmetmp2] || keysust[b][gmetmp2] == 1)
					noteon2(b, gmetmp2);
				else noteoff(b, gmetmp2);
			}
			else if (tmp >= 0xF0 && tmp <= 0xFE) {
				if (tmp == 0xF0) {

					while (tmp != 0xF7) tmp = sjgf(ch, j);
				}
				else if (tmp == 0xF7) {

					while (tmp != 0xF7) { tmp = sjgf(ch, j); }
				}
				else if (tmp == 0xF6) {

				}
				else if (tmp >= 0xF1 || tmp <= 0xF3) {

					tmp = sjgf(ch, j);
				}
			}
			else if (tmp >= 0xE0 && tmp <= 0xEF) {
				oldstate[j] = tmp;
				b = sjgf(ch, j);
				c = sjgf(ch, j);
				//midipitchbend[gmetmp2][0] = b; midipitchbend[gmetmp2][1] = c;
				int t = ((((int)c << 7) + (int)b) - 8192);
				rpitch[gmetmp2] = pow(2, (double)t / (12.*4096. * 256. / (double)pbendratio[gmetmp2]))*pc3[gmetmp2];
				pc2[gmetmp2] = (int)(12 * log2(rpitch[gmetmp2]));
				sppmy();
			}
			else if (tmp >= 0xC0 && tmp <= 0xCF) {
				oldstate[j] = tmp;
				e = sjgf(ch, j);
				if ((fmpreset[gmetmp2] == 128 || ombank[gmetmp2] == 1))
					midipreset[gmetmp2] = e;
				if (midiout)reparsesf(gmetmp2, midibank[gmetmp2], midipreset[gmetmp2], playrate);
			}
			else if (tmp >= 0xD0 && tmp <= 0xDF) {
				oldstate[j] = tmp;
				b = sjgf(ch, j);

			}
			else if (tmp >= 0xB0 && tmp <= 0xBF) {
				oldstate[j] = tmp;
				c = sjgf(ch, j);
				b = sjgf(ch, j);
				if (c == 0x00 && (fmbank[gmetmp2] == -1||ombank[gmetmp2]==1)) {
					midibank[gmetmp2] = b;
					//system("pause");
					if (midiout)reparsesf(gmetmp2, midibank[gmetmp2], midipreset[gmetmp2], playrate);
				}
				else if (c == 0x01) {
					modulationwheel[gmetmp2] = (b << 7) + (modulationwheel[gmetmp2] & 0x7F); rmodulationwheel[gmetmp2] = (double)(modulationwheel[gmetmp2] + fmodulationwheel[gmetmp2]) / 16384.; if (rmodulationwheel[gmetmp2]>1) rmodulationwheel[gmetmp2] = 1; if (rmodulationwheel[gmetmp2]<0) rmodulationwheel[gmetmp2] = 0;
				}
				else if (c == 0x02) {
					breathcontr[gmetmp2] = (b << 7) + (breathcontr[gmetmp2] & 0x7F);
				}
				else if (c == 0x04) {
					footcontr[gmetmp2] = (b << 7) + (footcontr[gmetmp2] & 0x7F);
				}
				else if (c == 0x06) {
					pbendratio[gmetmp2] = (b << 7) + (pbendratio[gmetmp2] & 0x7F);
				}
				else if (c == 0x07) {
					chnvolt[gmetmp2] = (b << 7) + (chnvolt[gmetmp2] & 0x7F);
					chvol[gmetmp2] = volume2[gmetmp2] * chnvolt[gmetmp2] / 16383.;
				}
				else if (c == 0x08 || c == 0x0A) {
					pan[gmetmp2] = (b << 7) + (pan[gmetmp2] & 0x7F);
				}
				else if (c == 0x20 && (fmbank[gmetmp2] == -1||ombank[gmetmp2]==1)) {
					////midibank[gmetmp2] = b + (midibank[gmetmp2] & 0x3F80);
					//system("pause");
					//if (midiout)reparsesf(gmetmp2, midibank[gmetmp2], midipreset[gmetmp2], playrate);
				}
				else if (c == 0x21) {
					modulationwheel[gmetmp2] = b + (modulationwheel[gmetmp2] & 0x3F80); rmodulationwheel[gmetmp2] = (double)(modulationwheel[gmetmp2] + fmodulationwheel[gmetmp2]) / 16384.; if (rmodulationwheel[gmetmp2]>1) rmodulationwheel[gmetmp2] = 1; if (rmodulationwheel[gmetmp2]<0) rmodulationwheel[gmetmp2] = 0;
				}
				else if (c == 0x22) {
					breathcontr[gmetmp2] = b + (breathcontr[gmetmp2] & 0x3F80);
				}
				else if (c == 0x24) {
					footcontr[gmetmp2] = b + (footcontr[gmetmp2] & 0x3F80);
				}
				else if (c == 0x26) {
					pbendratio[gmetmp2] = b + (pbendratio[gmetmp2] & 0x3F80);
				}
				else if (c == 0x27) {
					chnvolt[gmetmp2] = b + (chnvolt[gmetmp2] & 0x3F80);
					chvol[gmetmp2] = volume2[gmetmp2] * chnvolt[gmetmp2] / 16383.;
				}
				else if (c == 0x28 || c == 0x2A) {
					pan[gmetmp2] = b + (pan[gmetmp2] & 0x3F80);
				}
				else if (c == 0x40) {
					if (b > 64) damppad[gmetmp2] = true;
					else {
						damppad[gmetmp2] = false;
						if (gmetmp2 != 9)
							for (int dj = 0; dj < 128; dj++) {
								if (!key3[dj][gmetmp2])releasenote(gmetmp2, dj);
							}

					}
				}
				else if (c == 0x41) {
					if (b > 64);
					else pbendratio[gmetmp2] = 256;
				}
				else if (c == 0x42) {
					if (b > 64) {
						for (int dj = 0; dj < 128; dj++)keysust[gmetmp2][dj] = (key3[gmetmp2][dj] != 0);
					}
					else {
						for (int dj = 0; dj < 128; dj++)
							if (keysust[gmetmp2][dj] == 1) {
								keysust[gmetmp2][dj] = 0;
								releasenote(gmetmp2, dj);
							}
					}
				}
				else if (c == 0x43) {
					if (b > 64) softpad[gmetmp2] = true;
					else softpad[gmetmp2] = false;
				}
				else if (c == 0x5B) {
					reverb[gmetmp2] = b; dreverb[gmetmp2] = (double)(reverb[gmetmp2] + freverb[gmetmp2]) / 140.; if (dreverb[gmetmp2]>1) dreverb[gmetmp2] = 1; if (dreverb[gmetmp2]<0) dreverb[gmetmp2] = 0;
				}
				else if (c == 0x5D) {
					chorus[gmetmp2] = b;
					dchorus[gmetmp2] = (double)(chorus[gmetmp2] + fchorus[gmetmp2]) / 128.; if (dchorus[gmetmp2]>1) dchorus[gmetmp2] = 1; if (dchorus[gmetmp2]<0) dchorus[gmetmp2] = 0;
				}
				else if (c == 0x5E) {

				}
				else if (c == 0x79) {
					damppad[gmetmp2] = false;
					if (gmetmp2 != 9)
						for (int dj = 0; dj < 128; dj++) {
							if (!key3[dj][gmetmp2])releasenote(gmetmp2, dj);
						}
					pbendratio[gmetmp2] = 256;
					for (int dj = 0; dj < 128; dj++)
						if (keysust[gmetmp2][dj] == 1) {
							keysust[gmetmp2][dj] = 0;
							releasenote(gmetmp2, dj);
						}
					softpad[gmetmp2] = false;
					reverb[gmetmp2] = 0; dreverb[gmetmp2] = (double)(reverb[gmetmp2] + freverb[gmetmp2]) / 140.; if (dreverb[gmetmp2]>1) dreverb[gmetmp2] = 1; if (dreverb[gmetmp2]<0) dreverb[gmetmp2] = 0;
					chorus[gmetmp2] = 0; dchorus[gmetmp2] = (double)(chorus[gmetmp2] + fchorus[gmetmp2]) / 128.; if (dchorus[gmetmp2]>1) dchorus[gmetmp2] = 1; if (dchorus[gmetmp2]<0) dchorus[gmetmp2] = 0;


					footcontr[gmetmp2] = 0;
					breathcontr[gmetmp2] = 0;
					pan[gmetmp2] = 0;
					modulationwheel[gmetmp2] = 0; rmodulationwheel[gmetmp2] = (double)(modulationwheel[gmetmp2] + fmodulationwheel[gmetmp2]) / 16384.; if (rmodulationwheel[gmetmp2]>1) rmodulationwheel[gmetmp2] = 1; if (rmodulationwheel[gmetmp2]<0) rmodulationwheel[gmetmp2] = 0;
					chnvolt[gmetmp2] = 16383;
				}
				else if (c == 0x7B) {
					for (int dj = 0; dj<128; dj++)
						if (keysust[dj][gmetmp2] == 0) noteoff(dj, gmetmp2);
						else noteon2(dj, gmetmp2);
				}
			}
			else if (tmp >= 0xA0 && tmp <= 0xAF) {
				if (tmp<0xF0) oldstate[j] = tmp;
				sjgf(ch, j); sjgf(ch, j);
			}
			else {

				b = sjgf(ch, j);
				if (b == 0x2F) {
					buf[j] = true; goto P2;
				}
				else if (b == 0x51) {
					sjgf(ch, j);
					int2char i2c;
					i2c.c[2] = sjgf(ch, j); i2c.c[1] = sjgf(ch, j); i2c.c[0] = sjgf(ch, j);
					tempo = 60000000 / i2c.i;
					pdt = (60 * (double)playrate) / ((double)deltatime * (double)tempo);
				}
				else if (b == 0x59) {
					sjgf(ch, j);
					keysig = sjgf(ch, j);
					keysig2 = (bool)sjgf(ch, j);
					keysig += 8;
					if (keysig > 15) keysig &= 0xFF;
					if (ppmy == true) sppmy();
				}
				else if (b == 0x01) {
					tmp = 0;
					tmp2 = sjgf(ch, j);
					while (1) {
						if (tmp2 >= 0x80) {

							tmp += tmp2 - 0x80;
							tmp2 = sjgf(ch, j);
							tmp = tmp << 7;
						}
						else {
							tmp += tmp2;
							break;
						}
					}
					for (int i = 0; i < tmp; i++) {
						if (i < 1999) {
							mdstrings[i] = sjgf(ch, j); mdstrings[i + 1] = 0;
							if (mdstrings[i] == 0x20) mdstrings[i] = 0x10;
							else if (mdstrings[i] == 0x0A) mdstrings[i] = 0x10;
							else if (mdstrings[i] == 0x0D) mdstrings[i] = 0x10;
						}
						else sjgf(ch, j);
					}

				}
				else if (b == 0x58) {
					sjgf(ch, j);
					bth = sjgf(ch, j);
					btl = sjgf(ch, j);
					btl = 1 << btl;
					sjgf(ch, j); sjgf(ch, j);
				}
				else {
					tmp = 0;
					tmp2 = sjgf(ch, j);
					while (1) {
						if (tmp2 >= 0x80) {

							tmp += tmp2 - 0x80;
							tmp2 = sjgf(ch, j);
							tmp = tmp << 7;
						}
						else {
							tmp += tmp2;
							break;
						}
					}
					for (int i = 0; i < tmp; i++) sjgf(ch, j);

				}
			}
			while (1) {
				tmp = sjgf(ch, j);
				if (tmp == 0) {
					tmp = sjgf(ch, j);
				C1:
					gmetmp2 = tmp & 15;
					if (tmp <= 0x7F) { //("!"); 
						chbuffercnt[j]--; chlnt[j]++;  tmp = oldstate[j]; goto C1;
					}
					else if (tmp >= 0x80 && tmp <= 0x8F) {
						oldstate[j] = tmp;
						b = sjgf(ch, j);
						c = sjgf(ch, j);
						if (keysust[b][gmetmp2] == 0) noteoff(b, gmetmp2);
						else noteon2(b, gmetmp2);

					}
					else if (tmp >= 0x90 && tmp <= 0x9F) {
						oldstate[j] = tmp;
						b = sjgf(ch, j);
						c = sjgf(ch, j);
						if (c)   noteon(b, softpad[gmetmp2] ? (c >> 1) : c, j, gmetmp2);
						else if (damppad[gmetmp2] || keysust[b][gmetmp2] == 1)
							noteon2(b, gmetmp2);
						else noteoff(b, gmetmp2);
					}
					else if (tmp >= 0xF0 && tmp <= 0xFE) {
						if (tmp == 0xF0) {

							while (tmp != 0xF7) tmp = sjgf(ch, j);
						}
						else if (tmp == 0xF7) {

							while (tmp != 0xF7) { tmp = sjgf(ch, j); }
						}
						else if (tmp == 0xF6) {

						}
						else if (tmp >= 0xF1 || tmp <= 0xF3) {

							tmp = sjgf(ch, j);
						}
					}
					else if (tmp >= 0xE0 && tmp <= 0xEF) {
						oldstate[j] = tmp;
						b = sjgf(ch, j);
						c = sjgf(ch, j);
						//midipitchbend[gmetmp2][0] = b; midipitchbend[gmetmp2][1] = c;
						int t = ((((int)c << 7) + (int)b) - 8192);
						rpitch[gmetmp2] = pow(2, (double)t / (12.*4096. * 256. / (double)pbendratio[gmetmp2]))*pc3[gmetmp2];
						pc2[gmetmp2] = (int)(12 * log2(rpitch[gmetmp2]));
						sppmy();
					}
					else if (tmp >= 0xC0 && tmp <= 0xCF) {
						oldstate[j] = tmp;
						e = sjgf(ch, j);
						if ((fmpreset[gmetmp2] == 128 || ombank[gmetmp2] == 1))
							midipreset[gmetmp2] = e;
						if (midiout)reparsesf(gmetmp2, midibank[gmetmp2], midipreset[gmetmp2], playrate);
					}
					else if (tmp >= 0xD0 && tmp <= 0xDF) {
						oldstate[j] = tmp;
						b = sjgf(ch, j);

					}
					else if (tmp >= 0xB0 && tmp <= 0xBF) {
						oldstate[j] = tmp;
						c = sjgf(ch, j);
						b = sjgf(ch, j);
						if (c == 0x00 && (fmbank[gmetmp2] == -1||ombank[gmetmp2]==1)) {
							midibank[gmetmp2] = b;
							if (midiout)reparsesf(gmetmp2, midibank[gmetmp2], midipreset[gmetmp2], playrate);
						}
						else if (c == 0x01) {
							modulationwheel[gmetmp2] = (b << 7) + (modulationwheel[gmetmp2] & 0x7F); rmodulationwheel[gmetmp2] = (double)(modulationwheel[gmetmp2] + fmodulationwheel[gmetmp2]) / 16384.; if (rmodulationwheel[gmetmp2]>1) rmodulationwheel[gmetmp2] = 1; if (rmodulationwheel[gmetmp2]<0) rmodulationwheel[gmetmp2] = 0;
						}
						else if (c == 0x02) {
							breathcontr[gmetmp2] = (b << 7) + (breathcontr[gmetmp2] & 0x7F);
						}
						else if (c == 0x04) {
							footcontr[gmetmp2] = (b << 7) + (footcontr[gmetmp2] & 0x7F);
						}
						else if (c == 0x06) {
							pbendratio[gmetmp2] = (b << 7) + (pbendratio[gmetmp2] & 0x7F);
						}
						else if (c == 0x07) {
							chnvolt[gmetmp2] = (b << 7) + (chnvolt[gmetmp2] & 0x7F);
							chvol[gmetmp2] = volume2[gmetmp2] * chnvolt[gmetmp2] / 16383.;
						}
						else if (c == 0x08 || c == 0x0A) {
							pan[gmetmp2] = (b << 7) + (pan[gmetmp2] & 0x7F);
						}
						else if (c == 0x20 && (fmbank[gmetmp2] == -1||ombank[gmetmp2]==1)) {
						}
						else if (c == 0x21) {
							modulationwheel[gmetmp2] = b + (modulationwheel[gmetmp2] & 0x3F80); rmodulationwheel[gmetmp2] = (double)(modulationwheel[gmetmp2] + fmodulationwheel[gmetmp2]) / 16384.; if (rmodulationwheel[gmetmp2]>1) rmodulationwheel[gmetmp2] = 1; if (rmodulationwheel[gmetmp2]<0) rmodulationwheel[gmetmp2] = 0;
						}
						else if (c == 0x22) {
							breathcontr[gmetmp2] = b + (breathcontr[gmetmp2] & 0x3F80);
						}
						else if (c == 0x24) {
							footcontr[gmetmp2] = b + (footcontr[gmetmp2] & 0x3F80);
						}
						else if (c == 0x26) {
							pbendratio[gmetmp2] = b + (pbendratio[gmetmp2] & 0x3F80);
						}
						else if (c == 0x27) {
							chnvolt[gmetmp2] = b + (chnvolt[gmetmp2] & 0x3F80);
							chvol[gmetmp2] = volume2[gmetmp2] * chnvolt[gmetmp2] / 16383.;
						}
						else if (c == 0x28 || c == 0x2A) {
							pan[gmetmp2] = b + (pan[gmetmp2] & 0x3F80);
						}
						else if (c == 0x40) {
							if (b > 64) damppad[gmetmp2] = true;
							else {
								damppad[gmetmp2] = false;
								if (gmetmp2 != 9)
									for (int dj = 0; dj < 128; dj++) {
										if (!key3[dj][gmetmp2])releasenote(gmetmp2, dj);
									}

							}
						}
						else if (c == 0x41) {
							if (b > 64);
							else pbendratio[gmetmp2] = 256;
						}
						else if (c == 0x42) {
							if (b > 64) {
								for (int dj = 0; dj < 128; dj++)keysust[gmetmp2][dj] = (key3[gmetmp2][dj] != 0);
							}
							else {
								for (int dj = 0; dj < 128; dj++)
									if (keysust[gmetmp2][dj] == 1) {
										keysust[gmetmp2][dj] = 0;
										releasenote(gmetmp2, dj);
									}
							}
						}
						else if (c == 0x43) {
							if (b > 64) softpad[gmetmp2] = true;
							else softpad[gmetmp2] = false;
						}
						else if (c == 0x5B) {
							reverb[gmetmp2] = b; dreverb[gmetmp2] = (double)(reverb[gmetmp2] + freverb[gmetmp2]) / 140.; if (dreverb[gmetmp2]>1) dreverb[gmetmp2] = 1; if (dreverb[gmetmp2]<0) dreverb[gmetmp2] = 0;
						}
						else if (c == 0x5D) {
							chorus[gmetmp2] = b; dchorus[gmetmp2] = (double)(chorus[gmetmp2] + fchorus[gmetmp2]) / 128.; if (dchorus[gmetmp2]>1) dchorus[gmetmp2] = 1; if (dchorus[gmetmp2]<0) dchorus[gmetmp2] = 0;
						}
						else if (c == 0x5E) {

						}
						else if (c == 0x79) {
							damppad[gmetmp2] = false;
							if (gmetmp2 != 9)
								for (int dj = 0; dj < 128; dj++) {
									if (!key3[dj][gmetmp2])releasenote(gmetmp2, dj);
								}
							pbendratio[gmetmp2] = 256;
							for (int dj = 0; dj < 128; dj++)
								if (keysust[gmetmp2][dj] == 1) {
									keysust[gmetmp2][dj] = 0;
									releasenote(gmetmp2, dj);
								}
							softpad[gmetmp2] = false;
							reverb[gmetmp2] = 0; dreverb[gmetmp2] = (double)(reverb[gmetmp2] + freverb[gmetmp2]) / 140.; if (dreverb[gmetmp2]>1) dreverb[gmetmp2] = 1; if (dreverb[gmetmp2]<0) dreverb[gmetmp2] = 0;
							chorus[gmetmp2] = 0; dchorus[gmetmp2] = (double)(chorus[gmetmp2] + fchorus[gmetmp2]) / 128.; if (dchorus[gmetmp2]>1) dchorus[gmetmp2] = 1; if (dchorus[gmetmp2]<0) dchorus[gmetmp2] = 0;
							footcontr[gmetmp2] = 0;
							breathcontr[gmetmp2] = 0;
							pan[gmetmp2] = 0;
							modulationwheel[gmetmp2] = 0; rmodulationwheel[gmetmp2] = (double)(modulationwheel[gmetmp2] + fmodulationwheel[gmetmp2]) / 16384.; if (rmodulationwheel[gmetmp2]>1) rmodulationwheel[gmetmp2] = 1; if (rmodulationwheel[gmetmp2]<0) rmodulationwheel[gmetmp2] = 0;
							chnvolt[gmetmp2] = 16383;
						}
						else if (c == 0x7B) {
							for (int dj = 0; dj<128; dj++)
								if (keysust[dj][gmetmp2] == 0) noteoff(dj, gmetmp2);
								else noteon2(dj, gmetmp2);
						}
					}
					else if (tmp >= 0xA0 && tmp <= 0xAF) {
						if (tmp<0xF0) oldstate[j] = tmp;
						sjgf(ch, j); sjgf(ch, j);
					}
					else {


						b = sjgf(ch, j);
						if (b == 0x2F) {
							buf[j] = true; break;
						}
						else if (b == 0x51) {
							sjgf(ch, j);
							int2char i2c;
							i2c.c[2] = sjgf(ch, j); i2c.c[1] = sjgf(ch, j); i2c.c[0] = sjgf(ch, j);

							tempo = 60000000 / i2c.i;
							pdt = (60 * (double)playrate) / ((double)deltatime * (double)tempo);
						}
						else if (b == 0x59) {
							sjgf(ch, j);
							keysig = sjgf(ch, j);
							keysig2 = (bool)sjgf(ch, j);
							keysig += 8;
							if (keysig > 15) keysig &= 0xFF;
							if (ppmy == true) sppmy();
						}
						else if (b == 0x01) {
							tmp = 0;
							tmp2 = sjgf(ch, j);
							while (1) {
								if (tmp2 >= 0x80) {

									tmp += tmp2 - 0x80;
									tmp2 = sjgf(ch, j);
									tmp = tmp << 7;
								}
								else {
									tmp += tmp2;
									break;
								}
							}
							for (int i = 0; i < tmp; i++) {
								if (i < 1999) {
									mdstrings[i] = sjgf(ch, j); mdstrings[i + 1] = 0;
									if (mdstrings[i] == 0x20) mdstrings[i] = 0x10;
									else if (mdstrings[i] == 0x0A) mdstrings[i] = 0x10;
									else if (mdstrings[i] == 0x0D) mdstrings[i] = 0x10;
								}
								else sjgf(ch, j);
							}

						}
						else if (b == 0x58) {
							sjgf(ch, j);
							bth = sjgf(ch, j);
							btl = sjgf(ch, j);
							btl = 1 << btl;

							sjgf(ch, j); sjgf(ch, j);
						}
						else {
							tmp = 0;
							tmp2 = sjgf(ch, j);
							while (1) {
								if (tmp2 >= 0x80) {

									tmp += tmp2 - 0x80;
									tmp2 = sjgf(ch, j);
									tmp = tmp << 7;
								}
								else {
									tmp += tmp2;
									break;
								}
							}
							for (int i = 0; i < tmp; i++) sjgf(ch, j);

						}
					}
				}
				else {
					b = tmp;
					while (1) {
						if (b >= 0x80) {

							d += b - 0x80;
							b = sjgf(ch, j);
							d = d << 7;
						}
						else {
							d += b;
							break;
						}
					}
					le2[j] = d;
					break;
				}
			}
		}
	P2:
		tmp = tmp;
	}
}
inline void getmidievent2() {
	//("!", pnotes);
	//system("pause");
	for (int j = 0; j < chn; j++) {

		unsigned int tmp2;
		unsigned int tmp = 0; unsigned int b = 0, c = 0,e=0; unsigned long long d = 0;
		if (buf[j] == false) {
			while (1) {
				tmp = sjgf(ch, j);
				if (tmp == 0) {
					tmp = sjgf(ch, j);
				C3:

					gmetmp2 = tmp & 15;
					if (tmp <= 0x7F) {
						chbuffercnt[j]--; chlnt[j]++;  tmp = oldstate[j]; goto C3;
					}
					else if (tmp >= 0x80 && tmp <= 0x8F) {
						oldstate[j] = tmp;
						b = sjgf(ch, j);
						c = sjgf(ch, j);
						if (keysust[b][gmetmp2] == 0) noteoff(b, gmetmp2);
						else noteon2(b, gmetmp2);

					}
					else if (tmp >= 0x90 && tmp <= 0x9F) {
						oldstate[j] = tmp;
						b = sjgf(ch, j);
						c = sjgf(ch, j);
						if (c)   noteon(b, softpad[gmetmp2] ? (c >> 1) : c, j, gmetmp2);
						else if (damppad[gmetmp2] || keysust[b][gmetmp2] == 1)
							noteon2(b, gmetmp2);
						else noteoff(b, gmetmp2);
					}
					else if (tmp >= 0xF0 && tmp <= 0xFE) {
						if (tmp == 0xF0) {

							while (tmp != 0xF7) tmp = sjgf(ch, j);
						}
						else if (tmp == 0xF7) {

							while (tmp != 0xF7) { tmp = sjgf(ch, j); }
						}
						else if (tmp == 0xF6) {

						}
						else if (tmp >= 0xF1 || tmp <= 0xF3) {

							tmp = sjgf(ch, j);
						}
					}
					else if (tmp >= 0xE0 && tmp <= 0xEF) {
						oldstate[j] = tmp;
						b = sjgf(ch, j);
						c = sjgf(ch, j);
						int t = ((((int)c << 7) + (int)b) - 8192);
						rpitch[gmetmp2] = pow(2, (double)t / (12.*4096. * 256. / (double)pbendratio[gmetmp2]))*pc3[gmetmp2];
						pc2[gmetmp2] = (int)(12 * log2(rpitch[gmetmp2]));
						sppmy();
					}
					else if (tmp >= 0xC0 && tmp <= 0xCF) {
						oldstate[j] = tmp;
						e = sjgf(ch, j);
						if ((fmpreset[gmetmp2] == 128 || ombank[gmetmp2] == 1))
							midipreset[gmetmp2] = e;
						if (midiout)reparsesf(gmetmp2, midibank[gmetmp2], midipreset[gmetmp2], playrate);
					}
					else if (tmp >= 0xD0 && tmp <= 0xDF) {
						oldstate[j] = tmp;
						b = sjgf(ch, j);

					}
					else if (tmp >= 0xB0 && tmp <= 0xBF) {
						oldstate[j] = tmp;
						c = sjgf(ch, j);
						b = sjgf(ch, j);
						if (c == 0x00 && (fmbank[gmetmp2] == -1||ombank[gmetmp2]==1)) {
							midibank[gmetmp2] = b;
							//system("pause");
							if (midiout)reparsesf(gmetmp2, midibank[gmetmp2], midipreset[gmetmp2], playrate);
						}
						else if (c == 0x01) {
							modulationwheel[gmetmp2] = (b << 7) + (modulationwheel[gmetmp2] & 0x7F); rmodulationwheel[gmetmp2] = (double)(modulationwheel[gmetmp2] + fmodulationwheel[gmetmp2]) / 16384.; if (rmodulationwheel[gmetmp2]>1) rmodulationwheel[gmetmp2] = 1; if (rmodulationwheel[gmetmp2]<0) rmodulationwheel[gmetmp2] = 0;
						}
						else if (c == 0x02) {
							breathcontr[gmetmp2] = (b << 7) + (breathcontr[gmetmp2] & 0x7F);
						}
						else if (c == 0x04) {
							footcontr[gmetmp2] = (b << 7) + (footcontr[gmetmp2] & 0x7F);
						}
						else if (c == 0x06) {
							pbendratio[gmetmp2] = (b << 7) + (pbendratio[gmetmp2] & 0x7F);
						}
						else if (c == 0x07) {
							chnvolt[gmetmp2] = (b << 7) + (chnvolt[gmetmp2] & 0x7F);
							chvol[gmetmp2] = volume2[gmetmp2] * chnvolt[gmetmp2] / 16383.;
						}
						else if (c == 0x08 || c == 0x0A) {
							pan[gmetmp2] = (b << 7) + (pan[gmetmp2] & 0x7F);
						}
						else if (c == 0x20 && (fmbank[gmetmp2] == -1||ombank[gmetmp2]==1)) {
							////midibank[gmetmp2] = b + (midibank[gmetmp2] & 0x3F80);
							//system("pause");
							//if (midiout)reparsesf(gmetmp2, midibank[gmetmp2], midipreset[gmetmp2], playrate);
						}
						else if (c == 0x21) {
							modulationwheel[gmetmp2] = b + (modulationwheel[gmetmp2] & 0x3F80); rmodulationwheel[gmetmp2] = (double)(modulationwheel[gmetmp2] + fmodulationwheel[gmetmp2]) / 16384.; if (rmodulationwheel[gmetmp2]>1) rmodulationwheel[gmetmp2] = 1; if (rmodulationwheel[gmetmp2]<0) rmodulationwheel[gmetmp2] = 0;
						}
						else if (c == 0x22) {
							breathcontr[gmetmp2] = b + (breathcontr[gmetmp2] & 0x3F80);
						}
						else if (c == 0x24) {
							footcontr[gmetmp2] = b + (footcontr[gmetmp2] & 0x3F80);
						}
						else if (c == 0x26) {
							pbendratio[gmetmp2] = b + (pbendratio[gmetmp2] & 0x3F80);
						}
						else if (c == 0x27) {
							chnvolt[gmetmp2] = b + (chnvolt[gmetmp2] & 0x3F80);
							chvol[gmetmp2] = volume2[gmetmp2] * chnvolt[gmetmp2] / 16383.;
						}
						else if (c == 0x28 || c == 0x2A) {
							pan[gmetmp2] = b + (pan[gmetmp2] & 0x3F80);
						}
						else if (c == 0x40) {
							if (b > 64) damppad[gmetmp2] = true;
							else {
								damppad[gmetmp2] = false;
								if (gmetmp2 != 9)
									for (int dj = 0; dj < 128; dj++) {
										if (!key3[dj][gmetmp2])releasenote(gmetmp2, dj);
									}

							}
						}
						else if (c == 0x41) {
							if (b > 64);
							else pbendratio[gmetmp2] = 256;
						}
						else if (c == 0x42) {
							if (b > 64) {
								for (int dj = 0; dj < 128; dj++)keysust[gmetmp2][dj] = (key3[gmetmp2][dj] != 0);
							}
							else {
								for (int dj = 0; dj < 128; dj++)
									if (keysust[gmetmp2][dj] == 1) {
										keysust[gmetmp2][dj] = 0;
										releasenote(gmetmp2, dj);
									}
							}
						}
						else if (c == 0x43) {
							if (b > 64) softpad[gmetmp2] = true;
							else softpad[gmetmp2] = false;
						}
						else if (c == 0x5B) {
							reverb[gmetmp2] = b; dreverb[gmetmp2] = (double)(reverb[gmetmp2] + freverb[gmetmp2]) / 140.; if (dreverb[gmetmp2]>1) dreverb[gmetmp2] = 1; if (dreverb[gmetmp2]<0) dreverb[gmetmp2] = 0;
						}
						else if (c == 0x5D) {
							chorus[gmetmp2] = b; dchorus[gmetmp2] = (double)(chorus[gmetmp2] + fchorus[gmetmp2]) / 128.; if (dchorus[gmetmp2]>1) dchorus[gmetmp2] = 1; if (dchorus[gmetmp2]<0) dchorus[gmetmp2] = 0;
						}
						else if (c == 0x5E) {

						}
						else if (c == 0x79) {
							damppad[gmetmp2] = false;
							if (gmetmp2 != 9)
								for (int dj = 0; dj < 128; dj++) {
									if (!key3[dj][gmetmp2])releasenote(gmetmp2, dj);
								}
							pbendratio[gmetmp2] = 256;
							for (int dj = 0; dj < 128; dj++)
								if (keysust[gmetmp2][dj] == 1) {
									keysust[gmetmp2][dj] = 0;
									releasenote(gmetmp2, dj);
								}
							softpad[gmetmp2] = false;
							reverb[gmetmp2] = 0; dreverb[gmetmp2] = (double)(reverb[gmetmp2] + freverb[gmetmp2]) / 140.; if (dreverb[gmetmp2]>1) dreverb[gmetmp2] = 1; if (dreverb[gmetmp2]<0) dreverb[gmetmp2] = 0;
							chorus[gmetmp2] = 0; dchorus[gmetmp2] = (double)(chorus[gmetmp2] + fchorus[gmetmp2]) / 128.; if (dchorus[gmetmp2]>1) dchorus[gmetmp2] = 1; if (dchorus[gmetmp2]<0) dchorus[gmetmp2] = 0;


							footcontr[gmetmp2] = 0;
							breathcontr[gmetmp2] = 0;
							pan[gmetmp2] = 0;
							modulationwheel[gmetmp2] = 0; rmodulationwheel[gmetmp2] = (double)(modulationwheel[gmetmp2] + fmodulationwheel[gmetmp2]) / 16384.; if (rmodulationwheel[gmetmp2]>1) rmodulationwheel[gmetmp2] = 1; if (rmodulationwheel[gmetmp2]<0) rmodulationwheel[gmetmp2] = 0;
							chnvolt[gmetmp2] = 16383;
						}
						else if (c == 0x7B) {
							for (int dj = 0; dj<128; dj++)
								if (keysust[dj][gmetmp2] == 0) noteoff(dj, gmetmp2);
								else noteon2(dj, gmetmp2);
						}
					}
					else if (tmp >= 0xA0 && tmp <= 0xAF) {
						if (tmp<0xF0) oldstate[j] = tmp;
						sjgf(ch, j); sjgf(ch, j);
					}
					else {

						b = sjgf(ch, j);
						if (b == 0x2F) {
							buf[j] = true; break;
						}
						else if (b == 0x51) {
							sjgf(ch, j);
							int2char i2c;
							i2c.c[2] = sjgf(ch, j); i2c.c[1] = sjgf(ch, j); i2c.c[0] = sjgf(ch, j);
							tempo = 60000000 / i2c.i; pdt = (60 * (double)playrate) / ((double)deltatime * (double)tempo);
						}
						else if (b == 0x59) {
							sjgf(ch, j);
							keysig = sjgf(ch, j);
							keysig2 = (bool)sjgf(ch, j);
							keysig += 8;
							if (keysig > 15) keysig &= 0xFF;
							if (ppmy == true) sppmy();
						}
						else if (b == 0x01) {
							tmp = 0;
							tmp2 = sjgf(ch, j);
							while (1) {
								if (tmp2 >= 0x80) {

									tmp += tmp2 - 0x80;
									tmp2 = sjgf(ch, j);
									tmp = tmp << 7;
								}
								else {
									tmp += tmp2;
									break;
								}
							}
							for (int i = 0; i < tmp; i++) {
								if (i < 1999) {
									mdstrings[i] = sjgf(ch, j); mdstrings[i + 1] = 0;
									if (mdstrings[i] == 0x20) mdstrings[i] = 0x10;
									else if (mdstrings[i] == 0x0A) mdstrings[i] = 0x10;
									else if (mdstrings[i] == 0x0D) mdstrings[i] = 0x10;
								}
								else sjgf(ch, j);
							}

						}
						else if (b == 0x58) {
							sjgf(ch, j);
							bth = sjgf(ch, j);
							btl = sjgf(ch, j);
							btl = 1 << btl;
							sjgf(ch, j); sjgf(ch, j);
						}
						else {
							tmp = 0;
							tmp2 = sjgf(ch, j);
							while (1) {
								if (tmp2 >= 0x80) {

									tmp += tmp2 - 0x80;
									tmp2 = sjgf(ch, j);
									tmp = tmp << 7;
								}
								else {
									tmp += tmp2;
									break;
								}
							}
							for (int i = 0; i < tmp; i++) sjgf(ch, j);

						}
					}
				}
				else {
					b = tmp;
					while (1) {
						if (b >= 0x80) {
							d += b - 0x80;
							b = sjgf(ch, j);
							d = d << 7;
						}
						else {
							d += b;
							break;
						}
					}
					le2[j] = d;
					break;
				}
			}
		}
	}

}
inline void getmidieventacc1() {
	gmea = 0;
	for (int j = 0; j < chn; j++) {
		unsigned int tmp2;
		unsigned int tmp = 0, b = 0; unsigned int c = 0,e=0; unsigned long long d = 0;
		//("%lld|%lld,%lld,%lld ", mptr, le[j] + le2[j],le[j] , le2[j]);
		if (mptr == le[j] + le2[j] && buf[j] == false) {
			if (gmea == 0) {

				gmea = 1;
				if (gmeo != (wptr * 1000ll / (unsigned long long)playrate)) {
					mdtmpsort();
					fwrite(mdtmpwrite, 1, mdtmpsize, wj);
					mdtmpreset();
					gmeo = (wptr * 1000ll / (unsigned long long)playrate);

					gmeaptr2 = _ftelli64(wj);
					_fseeki64(wj, gmeaptr - 8, SEEK_SET);
					fwrite((const void*)&gmeaptr2, 8, 1, wj);
					
					
					_fseeki64(wj, 0, SEEK_END);

					fwrite(&gmeo, 8, 1, wj);

					fwrite(&gmeo, 8, 1, wj);
					
					
					gmeaptr = _ftelli64(wj);
					
					_fseeki64(wj, 0, SEEK_END);
					if (lud != gmeo / 300) {

						lud = gmeo / 300;
						fputc(0x00, wj);
						fputc(0x00, wj);
						fputc(0x00, wj);
						for (int i = 0; i < 16; i++) {


							for (int j = 0; j<128; j++)
								if (midibx[j][i] != -1) {
									fputc(0xB0 + i, wj);
									fputc(j, wj);
									fputc((unsigned char)midibx[j][i], wj);

								}

							fputc(0xC0 + i, wj);
							fputc(midipreset[i], wj);
							fputc(0x00, wj);

							fputc(0xE0 + i, wj);
							fputc(midipitchbend[i][0], wj);
							fputc(midipitchbend[i][1], wj);


						}
					}
					
					
					mdtmpreset();
				}
			}
			le[j] += le2[j];

			tmp = sjgf(ch, j);

		C2:

			gmetmp2 = tmp & 15;
			if (tmp <= 0x7F) { //("!"); 
				chbuffercnt[j]--; chlnt[j]++;  tmp = oldstate[j]; goto C2;
			}
			else if (tmp >= 0x80 && tmp <= 0x8F) {
				oldstate[j] = tmp;
				b = sjgf(ch, j);
				c = sjgf(ch, j);
				if (keysust[b][gmetmp2] == 0) noteoff(b, gmetmp2);
				else noteon2(b, gmetmp2);
				
				tmp -= 0x80;
				if (c == 0) {

					mdtmpoff[(tmp << 7) + b] = 0x80;
				}
				else {
					if (mdtmpvel[(tmp << 7) + b] < c) {
						mdtmpvel[(tmp << 7) + b] = c;
						mdtmpvel8[(tmp << 7) + b] = 0x80;
					}
				}
				
			}
			else if (tmp >= 0x90 && tmp <= 0x9F) {
				oldstate[j] = tmp;
				b = sjgf(ch, j);
				c = sjgf(ch, j);
				if (c)   noteon(b, softpad[gmetmp2] ? (c >> 1) : c, j, gmetmp2);
				else if (damppad[gmetmp2] || keysust[b][gmetmp2] == 1)
					noteon2(b, gmetmp2);
				else noteoff(b, gmetmp2);
				tmp -= 0x90;
				if (c == 0) {

					mdtmpoff[(tmp << 7) + b] = 0x90;
				}
				else {
					if (mdtmpvel[(tmp << 7) + b] < c) {
						mdtmpvel[(tmp << 7) + b] = c;
						mdtmpvel8[(tmp << 7) + b] = 0x90;
					}
				}
			}
			else if (tmp >= 0xF0 && tmp <= 0xFE) {
				if (tmp == 0xF0) {

					while (tmp != 0xF7) tmp = sjgf(ch, j);
				}
				else if (tmp == 0xF7) {

					while (tmp != 0xF7) { tmp = sjgf(ch, j); }
				}
				else if (tmp == 0xF6) {

				}
				else if (tmp >= 0xF1 || tmp <= 0xF3) {

					tmp = sjgf(ch, j);
				}
			}
			else if (tmp >= 0xE0 && tmp <= 0xEF) {
				oldstate[j] = tmp;
				b = sjgf(ch, j);
				c = sjgf(ch, j);
				midipitchbend[gmetmp2][0] = b; midipitchbend[gmetmp2][1] = c;
				int t = ((((int)c << 7) + (int)b) - 8192);
				for (int i = 0; i < 128; i++)if (key3[i][gmetmp2] != 0) { oks = 1; break; }
				rpitch[gmetmp2] = pow(2, (double)t / (12.*4096. * 256. / (double)pbendratio[gmetmp2]))*pc3[gmetmp2];
				pc2[gmetmp2] = (int)(12 * log2(rpitch[gmetmp2]));

				sppmy();
				fwrite(&tmp, 1, 1, wj);
				fwrite(&b, 1, 1, wj);
				fwrite(&c, 1, 1, wj);
			}
			else if (tmp >= 0xC0 && tmp <= 0xCF) {
				oldstate[j] = tmp;
				e = sjgf(ch, j);
				if ((fmpreset[gmetmp2] == 128 || ombank[gmetmp2] == 1))
					midipreset[gmetmp2] = e;
				//system("pause");
				if (midiout)reparsesf(gmetmp2, midibank[gmetmp2], midipreset[gmetmp2], playrate);
				fwrite(&tmp, 1, 1, wj);
				fwrite(&e, 1, 1, wj);
				fwrite("\0", 1, 1, wj);
			}
			else if (tmp >= 0xD0 && tmp <= 0xDF) {
				oldstate[j] = tmp;
				b = sjgf(ch, j);
				fwrite(&tmp, 1, 1, wj);
				fwrite(&b, 1, 1, wj);
				fwrite("\0", 1, 1, wj);
			}
			else if (tmp >= 0xB0 && tmp <= 0xBF) {
				oldstate[j] = tmp;
				c = sjgf(ch, j);
				b = sjgf(ch, j);
				midibx[c][gmetmp2] = b;
				if (c == 0x00 && (fmbank[gmetmp2] == -1||ombank[gmetmp2]==1)) {
					midibank[gmetmp2] = b;
					//system("pause");
					if (midiout)reparsesf(gmetmp2, midibank[gmetmp2], midipreset[gmetmp2], playrate);
				}
				else if (c == 0x01) {
					modulationwheel[gmetmp2] = (b << 7) + (modulationwheel[gmetmp2] & 0x7F); rmodulationwheel[gmetmp2] = (double)(modulationwheel[gmetmp2] + fmodulationwheel[gmetmp2]) / 16384.; if (rmodulationwheel[gmetmp2]>1) rmodulationwheel[gmetmp2] = 1; if (rmodulationwheel[gmetmp2]<0) rmodulationwheel[gmetmp2] = 0;
				}
				else if (c == 0x02) {
					breathcontr[gmetmp2] = (b << 7) + (breathcontr[gmetmp2] & 0x7F);
				}
				else if (c == 0x04) {
					footcontr[gmetmp2] = (b << 7) + (footcontr[gmetmp2] & 0x7F);
				}
				else if (c == 0x06) {
					pbendratio[gmetmp2] = (b << 7) + (pbendratio[gmetmp2] & 0x7F);
				}
				else if (c == 0x07) {
					chnvolt[gmetmp2] = (b << 7) + (chnvolt[gmetmp2] & 0x7F);
					chvol[gmetmp2] = volume2[gmetmp2] * chnvolt[gmetmp2] / 16383.;
				}
				else if (c == 0x08 || c == 0x0A) {
					pan[gmetmp2] = (b << 7) + (pan[gmetmp2] & 0x7F);
				}
				else if (c == 0x20 && (fmbank[gmetmp2] == -1||ombank[gmetmp2]==1)) {
					//midibank[gmetmp2] = b + (midibank[gmetmp2] & 0x3F80);
					//system("pause");
					//if (midiout)reparsesf(gmetmp2, midibank[gmetmp2], midipreset[gmetmp2], playrate);
				}
				else if (c == 0x21) {
					modulationwheel[gmetmp2] = b + (modulationwheel[gmetmp2] & 0x3F80); rmodulationwheel[gmetmp2] = (double)(modulationwheel[gmetmp2] + fmodulationwheel[gmetmp2]) / 16384.; if (rmodulationwheel[gmetmp2]>1) rmodulationwheel[gmetmp2] = 1; if (rmodulationwheel[gmetmp2]<0) rmodulationwheel[gmetmp2] = 0;
				}
				else if (c == 0x22) {
					breathcontr[gmetmp2] = b + (breathcontr[gmetmp2] & 0x3F80);
				}
				else if (c == 0x24) {
					footcontr[gmetmp2] = b + (footcontr[gmetmp2] & 0x3F80);
				}
				else if (c == 0x26) {
					pbendratio[gmetmp2] = b + (pbendratio[gmetmp2] & 0x3F80);
				}
				else if (c == 0x27) {
					chnvolt[gmetmp2] = b + (chnvolt[gmetmp2] & 0x3F80);
					chvol[gmetmp2] = volume2[gmetmp2] * chnvolt[gmetmp2] / 16383.;
				}
				else if (c == 0x28 || c == 0x2A) {
					pan[gmetmp2] = b + (pan[gmetmp2] & 0x3F80);
				}
				else if (c == 0x40) {
					if (b > 64) damppad[gmetmp2] = true;
					else {
						damppad[gmetmp2] = false;
						if (gmetmp2 != 9)
							for (int dj = 0; dj < 128; dj++) {
								if (!key3[dj][gmetmp2])releasenote(gmetmp2, dj);
							}

					}
				}
				else if (c == 0x41) {
					if (b > 64);
					else pbendratio[gmetmp2] = 256;
				}
				else if (c == 0x42) {
					if (b > 64) {
						for (int dj = 0; dj < 128; dj++)keysust[gmetmp2][dj] = (key3[gmetmp2][dj] != 0);
					}
					else {
						for (int dj = 0; dj < 128; dj++)
							if (keysust[gmetmp2][dj] == 1) {
								keysust[gmetmp2][dj] = 0;
								releasenote(gmetmp2, dj);
							}
					}
				}
				else if (c == 0x43) {
					if (b > 64) softpad[gmetmp2] = true;
					else softpad[gmetmp2] = false;
				}
				else if (c == 0x5B) {
					reverb[gmetmp2] = b; dreverb[gmetmp2] = (double)(reverb[gmetmp2] + freverb[gmetmp2]) / 140.; if (dreverb[gmetmp2]>1) dreverb[gmetmp2] = 1; if (dreverb[gmetmp2]<0) dreverb[gmetmp2] = 0;
				}
				else if (c == 0x5D) {
					chorus[gmetmp2] = b; dchorus[gmetmp2] = (double)(chorus[gmetmp2] + fchorus[gmetmp2]) / 128.; if (dchorus[gmetmp2]>1) dchorus[gmetmp2] = 1; if (dchorus[gmetmp2]<0) dchorus[gmetmp2] = 0;
				}
				else if (c == 0x5E) {

				}
				else if (c == 0x79) {
					damppad[gmetmp2] = false;
					if (gmetmp2 != 9)
						for (int dj = 0; dj < 128; dj++) {
							if (!key3[dj][gmetmp2])releasenote(gmetmp2, dj);
						}
					pbendratio[gmetmp2] = 256;
					for (int dj = 0; dj < 128; dj++)
						if (keysust[gmetmp2][dj] == 1) {
							keysust[gmetmp2][dj] = 0;
							releasenote(gmetmp2, dj);
						}
					softpad[gmetmp2] = false;
					reverb[gmetmp2] = 0; dreverb[gmetmp2] = (double)(reverb[gmetmp2] + freverb[gmetmp2]) / 140.; if (dreverb[gmetmp2]>1) dreverb[gmetmp2] = 1; if (dreverb[gmetmp2]<0) dreverb[gmetmp2] = 0;
					chorus[gmetmp2] = 0; dchorus[gmetmp2] = (double)(chorus[gmetmp2] + fchorus[gmetmp2]) / 128.; if (dchorus[gmetmp2]>1) dchorus[gmetmp2] = 1; if (dchorus[gmetmp2]<0) dchorus[gmetmp2] = 0;


					footcontr[gmetmp2] = 0;
					breathcontr[gmetmp2] = 0;
					pan[gmetmp2] = 0;
					modulationwheel[gmetmp2] = 0; rmodulationwheel[gmetmp2] = (double)(modulationwheel[gmetmp2] + fmodulationwheel[gmetmp2]) / 16384.; if (rmodulationwheel[gmetmp2]>1) rmodulationwheel[gmetmp2] = 1; if (rmodulationwheel[gmetmp2]<0) rmodulationwheel[gmetmp2] = 0;
					chnvolt[gmetmp2] = 16383;
				}
				else if (c == 0x7B) {
					for (int dj = 0; dj<128; dj++)
						if (keysust[dj][gmetmp2] == 0) noteoff(dj, gmetmp2);
						else noteon2(dj, gmetmp2);
				}
				fwrite(&tmp, 1, 1, wj);
				fwrite(&c, 1, 1, wj);
				fwrite(&b, 1, 1, wj);
			}
			else if (tmp >= 0xA0 && tmp <= 0xAF) {
				if (tmp<0xF0) oldstate[j] = tmp;
				b = sjgf(ch, j); c = sjgf(ch, j);

				fwrite(&tmp, 1, 1, wj);
				fwrite(&b, 1, 1, wj);
				fwrite(&c, 1, 1, wj);
			}

			else {

				b = sjgf(ch, j);
				if (b == 0x2F) {
					buf[j] = true; goto P2;
				}
				else if (b == 0x51) {
					sjgf(ch, j);
					int2char i2c;
					i2c.c[2] = sjgf(ch, j); i2c.c[1] = sjgf(ch, j); i2c.c[0] = sjgf(ch, j);
					tempo = 60000000 / i2c.i;
					pdt = (60 * (double)playrate) / ((double)deltatime * (double)tempo);
				}
				else if (b == 0x59) {
					sjgf(ch, j);
					keysig = sjgf(ch, j);
					keysig2 = (bool)sjgf(ch, j);
					keysig += 8;
					if (keysig > 15) keysig &= 0xFF;
					if (ppmy == true) sppmy();
				}
				else if (b == 0x01) {
					tmp = 0;
					tmp2 = sjgf(ch, j);
					while (1) {
						if (tmp2 >= 0x80) {

							tmp += tmp2 - 0x80;
							tmp2 = sjgf(ch, j);
							tmp = tmp << 7;
						}
						else {
							tmp += tmp2;
							break;
						}
					}
					for (int i = 0; i < tmp; i++) {
						if (i < 1999) {
							mdstrings[i] = sjgf(ch, j); mdstrings[i + 1] = 0;
							if (mdstrings[i] == 0x20) mdstrings[i] = 0x10;
							else if (mdstrings[i] == 0x0A) mdstrings[i] = 0x10;
							else if (mdstrings[i] == 0x0D) mdstrings[i] = 0x10;
						}
						else sjgf(ch, j);
					}

				}
				else if (b == 0x58) {
					sjgf(ch, j);
					bth = sjgf(ch, j);
					btl = sjgf(ch, j);
					btl = 1 << btl;
					sjgf(ch, j); sjgf(ch, j);
				}
				else {
					tmp = 0;
					tmp2 = sjgf(ch, j);
					while (1) {
						if (tmp2 >= 0x80) {

							tmp += tmp2 - 0x80;
							tmp2 = sjgf(ch, j);
							tmp = tmp << 7;
						}
						else {
							tmp += tmp2;
							break;
						}
					}
					for (int i = 0; i < tmp; i++) sjgf(ch, j);

				}
			}
			while (1) {
				tmp = sjgf(ch, j);
				if (tmp == 0) {
					tmp = sjgf(ch, j);
				C1:
					gmetmp2 = tmp & 15;
					if (tmp <= 0x7F) { //("!"); 
						chbuffercnt[j]--; chlnt[j]++;  tmp = oldstate[j]; goto C1;
					}
					else if (tmp >= 0x80 && tmp <= 0x8F) {
						oldstate[j] = tmp;
						b = sjgf(ch, j);
						c = sjgf(ch, j);
						if (keysust[b][gmetmp2] == 0) noteoff(b, gmetmp2);
						else noteon2(b, gmetmp2);
						tmp -= 0x80;
						if (c == 0) {

							mdtmpoff[(tmp << 7) + b] = 0x80;
						}
						else {
							if (mdtmpvel[(tmp << 7) + b] < c) {
								mdtmpvel[(tmp << 7) + b] = c;
								mdtmpvel8[(tmp << 7) + b] = 0x80;
							}
						}
					}
					else if (tmp >= 0x90 && tmp <= 0x9F) {
						oldstate[j] = tmp;
						b = sjgf(ch, j);
						c = sjgf(ch, j);
						if (c)   noteon(b, softpad[gmetmp2] ? (c >> 1) : c, j, gmetmp2);
						else if (damppad[gmetmp2] || keysust[b][gmetmp2] == 1)
							noteon2(b, gmetmp2);
						else noteoff(b, gmetmp2);
						tmp -= 0x90;
						if (c == 0) {

							mdtmpoff[(tmp << 7) + b] = 0x90;
						}
						else {
							if (mdtmpvel[(tmp << 7) + b] < c) {
								mdtmpvel[(tmp << 7) + b] = c;
								mdtmpvel8[(tmp << 7) + b] = 0x90;
							}
						}
					}
					else if (tmp >= 0xF0 && tmp <= 0xFE) {
						if (tmp == 0xF0) {

							while (tmp != 0xF7) tmp = sjgf(ch, j);
						}
						else if (tmp == 0xF7) {

							while (tmp != 0xF7) { tmp = sjgf(ch, j); }
						}
						else if (tmp == 0xF6) {

						}
						else if (tmp >= 0xF1 || tmp <= 0xF3) {

							tmp = sjgf(ch, j);
						}
					}
					else if (tmp >= 0xE0 && tmp <= 0xEF) {
						oldstate[j] = tmp;
						b = sjgf(ch, j);
						c = sjgf(ch, j);
						midipitchbend[gmetmp2][0] = b; midipitchbend[gmetmp2][1] = c;
						int t = ((((int)c << 7) + (int)b) - 8192);
						for (int i = 0; i < 128; i++)if (key3[i][gmetmp2] != 0) { oks = 1; break; }
						rpitch[gmetmp2] = pow(2, (double)t / (12.*4096. * 256. / (double)pbendratio[gmetmp2]))*pc3[gmetmp2];
						pc2[gmetmp2] = (int)(12 * log2(rpitch[gmetmp2]));

						sppmy();
						fwrite(&tmp, 1, 1, wj);
						fwrite(&b, 1, 1, wj);
						fwrite(&c, 1, 1, wj);
					}
					else if (tmp >= 0xC0 && tmp <= 0xCF) {
						oldstate[j] = tmp;

						midipreset[gmetmp2] = sjgf(ch, j);
						//system("pause");
						if (midiout)reparsesf(gmetmp2, midibank[gmetmp2], midipreset[gmetmp2], playrate);
						fwrite(&tmp, 1, 1, wj);
						fwrite(&e, 1, 1, wj);
						fwrite("\0", 1, 1, wj);
					}
					else if (tmp >= 0xD0 && tmp <= 0xDF) {
						oldstate[j] = tmp;
						b = sjgf(ch, j);
						fwrite(&tmp, 1, 1, wj);
						fwrite(&b, 1, 1, wj);
						fwrite("\0", 1, 1, wj);
					}
					else if (tmp >= 0xB0 && tmp <= 0xBF) {

						oldstate[j] = tmp;
						c = sjgf(ch, j);
						b = sjgf(ch, j);
						midibx[c][gmetmp2] = b;
						if (c == 0x00 && (fmbank[gmetmp2] == -1||ombank[gmetmp2]==1)) {
							midibank[gmetmp2] = b;
							//system("pause");
							if (midiout)reparsesf(gmetmp2, midibank[gmetmp2], midipreset[gmetmp2], playrate);
						}
						else if (c == 0x01) {
							modulationwheel[gmetmp2] = (b << 7) + (modulationwheel[gmetmp2] & 0x7F); rmodulationwheel[gmetmp2] = (double)(modulationwheel[gmetmp2] + fmodulationwheel[gmetmp2]) / 16384.; if (rmodulationwheel[gmetmp2]>1) rmodulationwheel[gmetmp2] = 1; if (rmodulationwheel[gmetmp2]<0) rmodulationwheel[gmetmp2] = 0;
						}
						else if (c == 0x02) {
							breathcontr[gmetmp2] = (b << 7) + (breathcontr[gmetmp2] & 0x7F);
						}
						else if (c == 0x04) {
							footcontr[gmetmp2] = (b << 7) + (footcontr[gmetmp2] & 0x7F);
						}
						else if (c == 0x06) {
							pbendratio[gmetmp2] = (b << 7) + (pbendratio[gmetmp2] & 0x7F);
						}
						else if (c == 0x07) {
							chnvolt[gmetmp2] = (b << 7) + (chnvolt[gmetmp2] & 0x7F);
							chvol[gmetmp2] = volume2[gmetmp2] * chnvolt[gmetmp2] / 16383.;
						}
						else if (c == 0x08 || c == 0x0A) {
							pan[gmetmp2] = (b << 7) + (pan[gmetmp2] & 0x7F);
						}
						else if (c == 0x20 && (fmbank[gmetmp2] == -1||ombank[gmetmp2]==1)) {
							//midibank[gmetmp2] = b + (midibank[gmetmp2] & 0x3F80);
							//system("pause");
							//if (midiout)reparsesf(gmetmp2, midibank[gmetmp2], midipreset[gmetmp2], playrate);
						}
						else if (c == 0x21) {
							modulationwheel[gmetmp2] = b + (modulationwheel[gmetmp2] & 0x3F80); rmodulationwheel[gmetmp2] = (double)(modulationwheel[gmetmp2] + fmodulationwheel[gmetmp2]) / 16384.; if (rmodulationwheel[gmetmp2]>1) rmodulationwheel[gmetmp2] = 1; if (rmodulationwheel[gmetmp2]<0) rmodulationwheel[gmetmp2] = 0;
						}
						else if (c == 0x22) {
							breathcontr[gmetmp2] = b + (breathcontr[gmetmp2] & 0x3F80);
						}
						else if (c == 0x24) {
							footcontr[gmetmp2] = b + (footcontr[gmetmp2] & 0x3F80);
						}
						else if (c == 0x26) {
							pbendratio[gmetmp2] = b + (pbendratio[gmetmp2] & 0x3F80);
						}
						else if (c == 0x27) {
							chnvolt[gmetmp2] = b + (chnvolt[gmetmp2] & 0x3F80);
							chvol[gmetmp2] = volume2[gmetmp2] * chnvolt[gmetmp2] / 16383.;
						}
						else if (c == 0x28 || c == 0x2A) {
							pan[gmetmp2] = b + (pan[gmetmp2] & 0x3F80);
						}
						else if (c == 0x40) {
							if (b > 64) damppad[gmetmp2] = true;
							else {
								damppad[gmetmp2] = false;
								if (gmetmp2 != 9)
									for (int dj = 0; dj < 128; dj++) {
										if (!key3[dj][gmetmp2])releasenote(gmetmp2, dj);
									}

							}
						}
						else if (c == 0x41) {
							if (b > 64);
							else pbendratio[gmetmp2] = 256;
						}
						else if (c == 0x42) {
							if (b > 64) {
								for (int dj = 0; dj < 128; dj++)keysust[gmetmp2][dj] = (key3[gmetmp2][dj] != 0);
							}
							else {
								for (int dj = 0; dj < 128; dj++)
									if (keysust[gmetmp2][dj] == 1) {
										keysust[gmetmp2][dj] = 0;
										releasenote(gmetmp2, dj);
									}
							}
						}
						else if (c == 0x43) {
							if (b > 64) softpad[gmetmp2] = true;
							else softpad[gmetmp2] = false;
						}
						else if (c == 0x5B) {
							reverb[gmetmp2] = b; dreverb[gmetmp2] = (double)(reverb[gmetmp2] + freverb[gmetmp2]) / 140.; if (dreverb[gmetmp2]>1) dreverb[gmetmp2] = 1; if (dreverb[gmetmp2]<0) dreverb[gmetmp2] = 0;
						}
						else if (c == 0x5D) {
							chorus[gmetmp2] = b; dchorus[gmetmp2] = (double)(chorus[gmetmp2] + fchorus[gmetmp2]) / 128.; if (dchorus[gmetmp2]>1) dchorus[gmetmp2] = 1; if (dchorus[gmetmp2]<0) dchorus[gmetmp2] = 0;
						}
						else if (c == 0x5E) {

						}
						else if (c == 0x79) {
							damppad[gmetmp2] = false;
							if (gmetmp2 != 9)
								for (int dj = 0; dj < 128; dj++) {
									if (!key3[dj][gmetmp2])releasenote(gmetmp2, dj);
								}
							pbendratio[gmetmp2] = 256;
							for (int dj = 0; dj < 128; dj++)
								if (keysust[gmetmp2][dj] == 1) {
									keysust[gmetmp2][dj] = 0;
									releasenote(gmetmp2, dj);
								}
							softpad[gmetmp2] = false;
							reverb[gmetmp2] = 0; dreverb[gmetmp2] = (double)(reverb[gmetmp2] + freverb[gmetmp2]) / 140.; if (dreverb[gmetmp2]>1) dreverb[gmetmp2] = 1; if (dreverb[gmetmp2]<0) dreverb[gmetmp2] = 0;
							chorus[gmetmp2] = 0; dchorus[gmetmp2] = (double)(chorus[gmetmp2] + fchorus[gmetmp2]) / 128.; if (dchorus[gmetmp2]>1) dchorus[gmetmp2] = 1; if (dchorus[gmetmp2]<0) dchorus[gmetmp2] = 0;


							footcontr[gmetmp2] = 0;
							breathcontr[gmetmp2] = 0;
							pan[gmetmp2] = 0;
							modulationwheel[gmetmp2] = 0; rmodulationwheel[gmetmp2] = (double)(modulationwheel[gmetmp2] + fmodulationwheel[gmetmp2]) / 16384.; if (rmodulationwheel[gmetmp2]>1) rmodulationwheel[gmetmp2] = 1; if (rmodulationwheel[gmetmp2]<0) rmodulationwheel[gmetmp2] = 0;
							chnvolt[gmetmp2] = 16383;
						}
						else if (c == 0x7B) {
							for (int dj = 0; dj<128; dj++)
								if (keysust[dj][gmetmp2] == 0) noteoff(dj, gmetmp2);
								else noteon2(dj, gmetmp2);
						}
						fwrite(&tmp, 1, 1, wj);
						fwrite(&c, 1, 1, wj);
						fwrite(&b, 1, 1, wj);
					}
					else if (tmp >= 0xA0 && tmp <= 0xAF) {
						if (tmp<0xF0) oldstate[j] = tmp;
						b = sjgf(ch, j); c = sjgf(ch, j);

						fwrite(&tmp, 1, 1, wj);
						fwrite(&b, 1, 1, wj);
						fwrite(&c, 1, 1, wj);
					}
					else {


						b = sjgf(ch, j);
						if (b == 0x2F) {
							buf[j] = true; break;
						}
						else if (b == 0x51) {
							sjgf(ch, j);
							int2char i2c;
							i2c.c[2] = sjgf(ch, j); i2c.c[1] = sjgf(ch, j); i2c.c[0] = sjgf(ch, j);

							tempo = 60000000 / i2c.i;
							pdt = (60 * (double)playrate) / ((double)deltatime * (double)tempo);
						}
						else if (b == 0x59) {
							sjgf(ch, j);
							keysig = sjgf(ch, j);
							keysig2 = (bool)sjgf(ch, j);
							keysig += 8;
							if (keysig > 15) keysig &= 0xFF;
							if (ppmy == true) sppmy();
						}
						else if (b == 0x01) {
							tmp = 0;
							tmp2 = sjgf(ch, j);
							while (1) {
								if (tmp2 >= 0x80) {

									tmp += tmp2 - 0x80;
									tmp2 = sjgf(ch, j);
									tmp = tmp << 7;
								}
								else {
									tmp += tmp2;
									break;
								}
							}
							for (int i = 0; i < tmp; i++) {
								if (i < 1999) {
									mdstrings[i] = sjgf(ch, j); mdstrings[i + 1] = 0;
									if (mdstrings[i] == 0x20) mdstrings[i] = 0x10;
									else if (mdstrings[i] == 0x0A) mdstrings[i] = 0x10;
									else if (mdstrings[i] == 0x0D) mdstrings[i] = 0x10;
								}
								else sjgf(ch, j);
							}

						}
						else if (b == 0x58) {
							sjgf(ch, j);
							bth = sjgf(ch, j);
							btl = sjgf(ch, j);
							btl = 1 << btl;

							sjgf(ch, j); sjgf(ch, j);
						}
						else {
							tmp = 0;
							tmp2 = sjgf(ch, j);
							while (1) {
								if (tmp2 >= 0x80) {

									tmp += tmp2 - 0x80;
									tmp2 = sjgf(ch, j);
									tmp = tmp << 7;
								}
								else {
									tmp += tmp2;
									break;
								}
							}
							for (int i = 0; i < tmp; i++) sjgf(ch, j);

						}
					}
				}
				else {
					b = tmp;
					while (1) {
						if (b >= 0x80) {

							d += b - 0x80;
							b = sjgf(ch, j);
							d = d << 7;
						}
						else {
							d += b;
							break;
						}
					}
					le2[j] = d;
					break;
				}
			}
		}
	P2:
		tmp = tmp;
	}

}
inline void getmidievent21() {
	mdtmpreset();
	gmea = 0;
	//("!", pnotes);
	//system("pause");
	for (int j = 0; j < chn; j++) {

		unsigned int tmp2;
		unsigned int tmp = 0; unsigned int b = 0, c = 0, e = 0; unsigned long long d = 0;
		if (buf[j] == false) {
			if (gmea == 0) {

				gmea = 1;
				if (gmeo != (wptr * 1000ll / (unsigned long long)playrate)) {
					gmeo = (wptr * 1000ll / (unsigned long long)playrate);
					fwrite(&gmeo, 8, 1, wj);
					fwrite(&gmeo, 8, 1, wj);
					gmeaptr = _ftelli64(wj);
				}
			}
			while (1) {
				tmp = sjgf(ch, j);
				if (tmp == 0) {
					tmp = sjgf(ch, j);
				C3:

					gmetmp2 = tmp & 15;
					if (tmp <= 0x7F) { //("!"); 
						chbuffercnt[j]--; chlnt[j]++;  tmp = oldstate[j]; goto C3;
					}
					else if (tmp >= 0x80 && tmp <= 0x8F) {
						oldstate[j] = tmp;
						b = sjgf(ch, j);
						c = sjgf(ch, j);
						if (keysust[b][gmetmp2] == 0) noteoff(b, gmetmp2);
						else noteon2(b, gmetmp2);
						tmp -= 0x80;
						if (c == 0) {

							mdtmpoff[(tmp << 7) + b] = 0x80;
						}
						else {
							if (mdtmpvel[(tmp << 7) + b] < c) {
								mdtmpvel[(tmp << 7) + b] = c;
								mdtmpvel8[(tmp << 7) + b] = 0x80;
							}
						}
					}
					else if (tmp >= 0x90 && tmp <= 0x9F) {
						oldstate[j] = tmp;
						b = sjgf(ch, j);
						c = sjgf(ch, j);
						if (c)   noteon(b, softpad[gmetmp2] ? (c >> 1) : c, j, gmetmp2);
						else if (damppad[gmetmp2] || keysust[b][gmetmp2] == 1)
							noteon2(b, gmetmp2);
						else noteoff(b, gmetmp2);
						tmp -= 0x90;
						if (c == 0) {

							mdtmpoff[(tmp << 7) + b] = 0x90;
						}
						else {
							if (mdtmpvel[(tmp << 7) + b] < c) {
								mdtmpvel[(tmp << 7) + b] = c;
								mdtmpvel8[(tmp << 7) + b] = 0x90;
							}
						}
					}
					else if (tmp >= 0xF0 && tmp <= 0xFE) {
						if (tmp == 0xF0) {

							while (tmp != 0xF7) tmp = sjgf(ch, j);
						}
						else if (tmp == 0xF7) {

							while (tmp != 0xF7) { tmp = sjgf(ch, j); }
						}
						else if (tmp == 0xF6) {

						}
						else if (tmp >= 0xF1 || tmp <= 0xF3) {

							tmp = sjgf(ch, j);
						}
					}
					else if (tmp >= 0xE0 && tmp <= 0xEF) {
						oldstate[j] = tmp;
						b = sjgf(ch, j);
						c = sjgf(ch, j);
						midipitchbend[gmetmp2][0] = b; midipitchbend[gmetmp2][1] = c;
						int t = ((((int)c << 7) + (int)b) - 8192);
						for (int i = 0; i < 128; i++)if (key3[i][gmetmp2] != 0) { oks = 1; break; }
						rpitch[gmetmp2] = pow(2, (double)t / (12.*4096. * 256. / (double)pbendratio[gmetmp2]))*pc3[gmetmp2];
						pc2[gmetmp2] = (int)(12 * log2(rpitch[gmetmp2]));

						sppmy();
						fwrite(&tmp, 1, 1, wj);
						fwrite(&b, 1, 1, wj);
						fwrite(&c, 1, 1, wj);
					}
					else if (tmp >= 0xC0 && tmp <= 0xCF) {
						oldstate[j] = tmp;
						e = sjgf(ch, j);
						if ((fmpreset[gmetmp2] == 128||ombank[gmetmp2]==1))
							midipreset[gmetmp2] = e;

						//system("pause");
						//if (midiout)reparsesf(gmetmp2, midibank[gmetmp2], midipreset[gmetmp2], playrate);
						fwrite(&tmp, 1, 1, wj);
						fwrite(&e, 1, 1, wj);
						fwrite("\0", 1, 1, wj);
					}
					else if (tmp >= 0xD0 && tmp <= 0xDF) {
						oldstate[j] = tmp;
						b = sjgf(ch, j);
						fwrite(&tmp, 1, 1, wj);
						fwrite(&b, 1, 1, wj);
						fwrite("\0", 1, 1, wj);
					}
					else if (tmp >= 0xB0 && tmp <= 0xBF) {
						oldstate[j] = tmp;
						c = sjgf(ch, j);
						b = sjgf(ch, j);
						midibx[c][gmetmp2] = b;
						if (c == 0x00 && (fmbank[gmetmp2] == -1||ombank[gmetmp2]==1)) {
							midibank[gmetmp2] = b;
							//system("pause");
							//	if (midiout)reparsesf(gmetmp2, midibank[gmetmp2], midipreset[gmetmp2], playrate);
						}
						else if (c == 0x01) {
							modulationwheel[gmetmp2] = (b << 7) + (modulationwheel[gmetmp2] & 0x7F); rmodulationwheel[gmetmp2] = (double)(modulationwheel[gmetmp2] + fmodulationwheel[gmetmp2]) / 16384.; if (rmodulationwheel[gmetmp2]>1) rmodulationwheel[gmetmp2] = 1; if (rmodulationwheel[gmetmp2]<0) rmodulationwheel[gmetmp2] = 0;
						}
						else if (c == 0x02) {
							breathcontr[gmetmp2] = (b << 7) + (breathcontr[gmetmp2] & 0x7F);
						}
						else if (c == 0x04) {
							footcontr[gmetmp2] = (b << 7) + (footcontr[gmetmp2] & 0x7F);
						}
						else if (c == 0x06) {
							pbendratio[gmetmp2] = (b << 7) + (pbendratio[gmetmp2] & 0x7F);
						}
						else if (c == 0x07) {
							chnvolt[gmetmp2] = (b << 7) + (chnvolt[gmetmp2] & 0x7F);
							chvol[gmetmp2] = volume2[gmetmp2] * chnvolt[gmetmp2] / 16383.;
						}
						else if (c == 0x08 || c == 0x0A) {
							pan[gmetmp2] = (b << 7) + (pan[gmetmp2] & 0x7F);
						}
						else if (c == 0x20 && (fmbank[gmetmp2] == -1||ombank[gmetmp2]==1)) {
							//midibank[gmetmp2] = b + (midibank[gmetmp2] & 0x3F80);
							//system("pause");
							//if (midiout)reparsesf(gmetmp2, midibank[gmetmp2], midipreset[gmetmp2], playrate);
						}
						else if (c == 0x21) {
							modulationwheel[gmetmp2] = b + (modulationwheel[gmetmp2] & 0x3F80); rmodulationwheel[gmetmp2] = (double)(modulationwheel[gmetmp2] + fmodulationwheel[gmetmp2]) / 16384.; if (rmodulationwheel[gmetmp2]>1) rmodulationwheel[gmetmp2] = 1; if (rmodulationwheel[gmetmp2]<0) rmodulationwheel[gmetmp2] = 0;
						}
						else if (c == 0x22) {
							breathcontr[gmetmp2] = b + (breathcontr[gmetmp2] & 0x3F80);
						}
						else if (c == 0x24) {
							footcontr[gmetmp2] = b + (footcontr[gmetmp2] & 0x3F80);
						}
						else if (c == 0x26) {
							pbendratio[gmetmp2] = b + (pbendratio[gmetmp2] & 0x3F80);
						}
						else if (c == 0x27) {
							chnvolt[gmetmp2] = b + (chnvolt[gmetmp2] & 0x3F80);
							chvol[gmetmp2] = volume2[gmetmp2] * chnvolt[gmetmp2] / 16383.;
						}
						else if (c == 0x28 || c == 0x2A) {
							pan[gmetmp2] = b + (pan[gmetmp2] & 0x3F80);
						}
						else if (c == 0x40) {
							if (b > 64) damppad[gmetmp2] = true;
							else {
								damppad[gmetmp2] = false;
								if (gmetmp2 != 9)
									for (int dj = 0; dj < 128; dj++) {
										if (!key3[dj][gmetmp2])releasenote(gmetmp2, dj);
									}

							}
						}
						else if (c == 0x41) {
							if (b > 64);
							else pbendratio[gmetmp2] = 256;
						}
						else if (c == 0x42) {
							if (b > 64) {
								for (int dj = 0; dj < 128; dj++)keysust[gmetmp2][dj] = (key3[gmetmp2][dj] != 0);
							}
							else {
								for (int dj = 0; dj < 128; dj++)
									if (keysust[gmetmp2][dj] == 1) {
										keysust[gmetmp2][dj] = 0;
										releasenote(gmetmp2, dj);
									}
							}
						}
						else if (c == 0x43) {
							if (b > 64) softpad[gmetmp2] = true;
							else softpad[gmetmp2] = false;
						}
						else if (c == 0x5B) {
							reverb[gmetmp2] = b; dreverb[gmetmp2] = (double)(reverb[gmetmp2] + freverb[gmetmp2]) / 140.; if (dreverb[gmetmp2]>1) dreverb[gmetmp2] = 1; if (dreverb[gmetmp2]<0) dreverb[gmetmp2] = 0;
						}
						else if (c == 0x5D) {
							chorus[gmetmp2] = b; dchorus[gmetmp2] = (double)(chorus[gmetmp2] + fchorus[gmetmp2]) / 128.; if (dchorus[gmetmp2]>1) dchorus[gmetmp2] = 1; if (dchorus[gmetmp2]<0) dchorus[gmetmp2] = 0;
						}
						else if (c == 0x5E) {

						}
						else if (c == 0x79) {
							damppad[gmetmp2] = false;
							if (gmetmp2 != 9)
								for (int dj = 0; dj < 128; dj++) {
									if (!key3[dj][gmetmp2])releasenote(gmetmp2, dj);
								}
							pbendratio[gmetmp2] = 256;
							for (int dj = 0; dj < 128; dj++)
								if (keysust[gmetmp2][dj] == 1) {
									keysust[gmetmp2][dj] = 0;
									releasenote(gmetmp2, dj);
								}
							softpad[gmetmp2] = false;
							reverb[gmetmp2] = 0; dreverb[gmetmp2] = (double)(reverb[gmetmp2] + freverb[gmetmp2]) / 140.; if (dreverb[gmetmp2]>1) dreverb[gmetmp2] = 1; if (dreverb[gmetmp2]<0) dreverb[gmetmp2] = 0;
							chorus[gmetmp2] = 0; dchorus[gmetmp2] = (double)(chorus[gmetmp2] + fchorus[gmetmp2]) / 128.; if (dchorus[gmetmp2]>1) dchorus[gmetmp2] = 1; if (dchorus[gmetmp2]<0) dchorus[gmetmp2] = 0;


							footcontr[gmetmp2] = 0;
							breathcontr[gmetmp2] = 0;
							pan[gmetmp2] = 0;
							modulationwheel[gmetmp2] = 0; rmodulationwheel[gmetmp2] = (double)(modulationwheel[gmetmp2] + fmodulationwheel[gmetmp2]) / 16384.; if (rmodulationwheel[gmetmp2]>1) rmodulationwheel[gmetmp2] = 1; if (rmodulationwheel[gmetmp2]<0) rmodulationwheel[gmetmp2] = 0;
							chnvolt[gmetmp2] = 16383;
						}
						else if (c == 0x7B) {
							for (int dj = 0; dj<128; dj++)
								if (keysust[dj][gmetmp2] == 0) noteoff(dj, gmetmp2);
								else noteon2(dj, gmetmp2);
						}
						fwrite(&tmp, 1, 1, wj);
						fwrite(&c, 1, 1, wj);
						fwrite(&b, 1, 1, wj);
					}
					else if (tmp >= 0xA0 && tmp <= 0xAF) {
						if (tmp<0xF0) oldstate[j] = tmp;
						b = sjgf(ch, j); c = sjgf(ch, j);

						fwrite(&tmp, 1, 1, wj);
						fwrite(&b, 1, 1, wj);
						fwrite(&c, 1, 1, wj);
					}
					else {

						b = sjgf(ch, j);
						if (b == 0x2F) {
							buf[j] = true; break;
						}
						else if (b == 0x51) {
							sjgf(ch, j);
							int2char i2c;
							i2c.c[2] = sjgf(ch, j); i2c.c[1] = sjgf(ch, j); i2c.c[0] = sjgf(ch, j);
							tempo = 60000000 / i2c.i; pdt = (60 * (double)playrate) / ((double)deltatime * (double)tempo);
						}
						else if (b == 0x59) {
							sjgf(ch, j);
							keysig = sjgf(ch, j);
							keysig2 = (bool)sjgf(ch, j);
							keysig += 8;
							if (keysig > 15) keysig &= 0xFF;
							if (ppmy == true) sppmy();
						}
						else if (b == 0x01) {
							tmp = 0;
							tmp2 = sjgf(ch, j);
							while (1) {
								if (tmp2 >= 0x80) {

									tmp += tmp2 - 0x80;
									tmp2 = sjgf(ch, j);
									tmp = tmp << 7;
								}
								else {
									tmp += tmp2;
									break;
								}
							}
							for (int i = 0; i < tmp; i++) {
								if (i < 1999) {
									mdstrings[i] = sjgf(ch, j); mdstrings[i + 1] = 0;
									if (mdstrings[i] == 0x20) mdstrings[i] = 0x10;
									else if (mdstrings[i] == 0x0A) mdstrings[i] = 0x10;
									else if (mdstrings[i] == 0x0D) mdstrings[i] = 0x10;
								}
								else sjgf(ch, j);
							}

						}
						else if (b == 0x58) {
							sjgf(ch, j);
							bth = sjgf(ch, j);
							btl = sjgf(ch, j);
							btl = 1 << btl;
							sjgf(ch, j); sjgf(ch, j);
						}
						else {
							tmp = 0;
							tmp2 = sjgf(ch, j);
							while (1) {
								if (tmp2 >= 0x80) {

									tmp += tmp2 - 0x80;
									tmp2 = sjgf(ch, j);
									tmp = tmp << 7;
								}
								else {
									tmp += tmp2;
									break;
								}
							}
							for (int i = 0; i < tmp; i++) sjgf(ch, j);

						}
					}
				}
				else {
					b = tmp;
					while (1) {
						if (b >= 0x80) {
							d += b - 0x80;
							b = sjgf(ch, j);
							d = d << 7;
						}
						else {
							d += b;
							break;
						}
					}
					le2[j] = d;
					break;
				}
			}
		}
	}


}
inline void pauseall() {
	for (int p = 0; p < 0x10; p++)
		midiOutShortMsg(playmidiout, 0x00007BB0 + p);
}
inline void waveOutSetPosition(int milliseconds) {

	wh.lpData = buffer + 2 * (milliseconds*playrate / 1000); // 소리 데이터가 있는 위치
	wh.dwBufferLength = mDataChunk.chunkDataSize - 2 * (milliseconds*playrate / 1000); // 소리 데이터의 크기
	wh.dwFlags = 0;
	wh.dwLoops = 0;
	waveOutReset(hWaveOut);
	waveOutUnprepareHeader(hWaveOut, &wh, sizeof(wh));
	waveOutClose(hWaveOut);

	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wf, 0, 0, CALLBACK_NULL);

	waveOutPrepareHeader(hWaveOut, &wh, sizeof(wh));
	waveOutWrite(hWaveOut, &wh, sizeof(wh));
	if (!isplay) waveOutPause(hWaveOut);
}
inline int clock2_init() {
	_clock2 = 0.; clock_start = clock(); return 0;
}
inline int clock2_save() {
	unsigned long clocktemp = clock();
	_clock2 = (int)(_clock2 + (spd / 100. * (double)(clocktemp - clock_start)));
	clock_start = clocktemp;
	return 0;
}
inline int clock2() {
	return (int)(_clock2 + (spd / 100.*(double)(clock() - clock_start)));
}
inline void MOSM(HMIDIOUT hmo,DWORD dwMsg) {
	//while
		(midiOutShortMsg(hmo, dwMsg) != MMSYSERR_NOERROR);

}
DWORD WINAPI playmidi(LPVOID unused)
{
	//MessageBox(0, "A", "A", MB_OK);

	double t;
	double mpbt;
	unsigned char k;
	short a, b;
	int vt;
	_playmidit playt;
	ro = 0;
	playmidif = fopen(filename0, "rb");
	playmidig = fopen(filename0, "rb");
	midiOutOpen(&playmidiout, 0, 0, 0, CALLBACK_NULL);
	Sleep(1);
	midiOutSetVolume(playmidiout, (int)(playvolume * 65535.) + ((int)(playvolume * 65535.) << 16));
	unsigned long long len;
	playmidii = 0;
	playmidin0 = clock2();

	playmidit.bf[3] = 0;
	fread(&playmidii, 8, 1, playmidif);
	fread(&playmidij, 8, 1, playmidif);
	fseek(playmidig, 16, SEEK_CUR);
	fseek(playmidif, playmidij, SEEK_SET);

	while (1)
	{


		if (!isplay) {
			midiOutClose(playmidiout);
			while (!isplay) {

				//Sleep(1);
			}
			midiOutOpen(&playmidiout, 0, 0, 0, CALLBACK_NULL);
			//Sleep(1);
			for (int i = 0; i < 16; i++) {
				ipaused[i] = 1, bpaused[i] = 1, mpaused[i] = 1, cpaused[i] = 1, rpaused[i] = 1;
				if (bpaused[i] == 1) {
					MOSM(playmidiout, 0x000000B0 + i + (fmbank[i] << 16));
					bpaused[i] = 0;
				}
			}
		}
		if (ro) {
			ro = 0;
			//	for (int p = 0; p < 0x10; p++)
			//MOSM(playmidiout, 0x00007BB0 + p);
			midiOutClose(playmidiout);
			midiOutOpen(&playmidiout, 0, 0, 0, CALLBACK_NULL);
			//Sleep(1);
			for (int i = 0; i < 16; i++) {
				ipaused[i] = 1, bpaused[i] = 1, mpaused[i] = 1, cpaused[i] = 1, rpaused[i] = 1;
				if (bpaused[i] == 1) {
					MOSM(playmidiout, 0x000000B0 + i + (fmbank[i] << 16));
					bpaused[i] = 0;
				}
			}
		}
		fread(&playmidini, 8, 1, playmidif);

		fread(&playmidinj, 8, 1, playmidif);
		fseek(playmidif, playmidinj, SEEK_SET);
		len = (playmidij - ftell(playmidig)) / 3;
		k = 12;
		if (oks == 1) {
			for (a = 0; a<16; a++) if (a != 9) pitchint[a] = pitchint2[a] = (int)(12.*log2(pitch[a] / 100.) + 0.5);
		}
		for (int i = 0; i < 16; i++) {
			MOSM(playmidiout, 0x000000C0 + i + (fmpreset[i] << 8));
			if (rpaused[i]) {
				a = freverb[i] + reverb[i];
				if (a > 127) a = 127;
				else if (a < 0)a = 0;
				MOSM(playmidiout, 0x00005BB0 + i + (a << 16));
				rpaused[i] = 0;

			}
			if (cpaused[i]) {
				a = fchorus[i] + chorus[i];
				if (a > 127) a = 127;
				else if (a < 0)a = 0;
				MOSM(playmidiout, 0x00005DB0 + i + (a << 16));
				cpaused[i] = 0;
			}
			if (mpaused[i]) {
				a = fmodulationwheel[i] + modulationwheel[i];
				if (a > 16383) a = 16383;
				else if (a < 0)a = 0;
				MOSM(playmidiout, 0x000001B0 + i + ((a >> 7) << 16));
				MOSM(playmidiout, 0x000021B0 + i + ((a & 127) << 16));

				mpaused[i] = 0;

			}
		}
		while (clock2() - playmidin0 < playmidii) {  }
		while (len--)
		{
			if (fread(playmidit.bf, 1, 3, playmidig) < 3) goto Y;
			playt.me = playmidit.me;
			if (playmidit.bf[0] >= 0xA0) {
				if ((playmidit.bf[0] & 0xF0) == 0xB0 && playmidit.bf[1] == 0x06) {
					a = (playmidit.bf[0] & 0x0F);
					pbendratio[a] = ((int)playmidit.bf[2] << 7) + (pbendratio[a] & 0x7F);
				}
				else if ((playmidit.bf[0] & 0xF0) == 0xB0 && playmidit.bf[1] == 0x26) {
					a = (playmidit.bf[0] & 0x0F);
					pbendratio[a] = (int)playmidit.bf[2] + (pbendratio[a] & 0x3F80);
				}
				if ((playmidit.bf[0] & 0xF0) == 0xE0 && (playmidit.bf[0] & 0x0F) != 0x09 && oks == 0) {
					a = (playmidit.bf[0] & 0x0F);

					if (pbendratio[a] >= 128) {
						pitchint[a] = 0; pitchint2[a] = 0;
						mpbt = (double)((((int)playmidit.bf[2] << 7) + (int)playmidit.bf[1]) - 8192) +
							(log2(pitch[a] / 100.)*12.*4096.*256. / (double)pbendratio[a]);
						if (pbendratio[a] >= 256) {
							if (mpbt >= 8192.*256. / (double)pbendratio[a]) {
								int lp = (int)((mpbt - 4096. * 256. / (double)pbendratio[a]) / (4096. * 256. / (double)pbendratio[a]));

								{
									mpbt -= (4096 * 256. / (double)pbendratio[a]) * (double)lp;
									pitchint[a] += lp;
									pitchint2[a] += lp;
								}
							}
							if (mpbt >= 2048.*3. * 256. / (double)pbendratio[a]) {

								pitchint[a]++;

							}
							if (mpbt >= 2048. * 256. / (double)pbendratio[a]) {

								pitchint[a]++;

							}
							if (mpbt < -8192. *256. / (double)pbendratio[a]) {
								int lp = (int)((-mpbt - 4096. * 256. / (double)pbendratio[a]) / (4096. * 256. / (double)pbendratio[a]));

								{
									mpbt += 4096. * 256. / (double)pbendratio[a] * (double)lp;
									pitchint[a] -= lp;
									pitchint2[a] -= lp;
								}
							}
							if (mpbt < -2048.*3. * 256. / (double)pbendratio[a]) {

								pitchint[a]--;
							}
							if (mpbt < -2048. * 256. / (double)pbendratio[a]) {

								pitchint[a]--;
							}
						}
						else {
							if (mpbt >= 4096.*256. / (double)pbendratio[a]) {
								int lp = (int)((mpbt) / (4096. * 256. / (double)pbendratio[a]));

								{
									mpbt -= (4096 * 256. / (double)pbendratio[a]) * (double)lp;
									pitchint[a] += lp;
									pitchint2[a] += lp;
								}
							}
							if (mpbt >= 2048. * 256. / (double)pbendratio[a]) {
								pitchint[a]++;

							}
							if (mpbt < -4096. * 256. / (double)pbendratio[a]) {
								int lp = (int)((-mpbt) / (4096. * 256. / (double)pbendratio[a]));

								{
									mpbt += 4096. * 256. / (double)pbendratio[a] * (double)lp;
									pitchint[a] -= lp;
									pitchint2[a] -= lp;
								}
							}
							if (mpbt < -2048. * 256. / (double)pbendratio[a]) {

								pitchint[a]--;
							}
						}
						midipitchbend2[a] = (int)mpbt;
						playmidit.bf[2] = (midipitchbend2[a] + 8192) >> 7;
						playmidit.bf[1] = ((midipitchbend2[a] + 8192) & 0x7F);

					}
					else {
						pitchint[a] = pitchint2[a] = (int)(12.*log2(pitch[a] / 100.) + 0.5);
					}
				}
				if ((playmidit.bf[0] & 0xF0) == 0xC0) {
					a = (playmidit.bf[0] & 0x0F);
					ompreset[a] = midipreset[a];
					midipreset[a] = playmidit.bf[1];
					if (ompreset[a] != midipreset[a]) { fmpreset[a] = midipreset[a]; ompreset[a] = midipreset[a]; }
					playmidit.me = 0;
					for (int i = 0; i < 16; i++) {
						MOSM(playmidiout, 0x000000C0 + i + (fmpreset[i] << 8));
					}
				}
				if ((playmidit.bf[0] & 0xF0) == 0xB0 && playmidit.bf[1] == 0x00) {
					a = (playmidit.bf[0] & 0x0F);
					ombank[a] = midibank[a];
					midibank[a] = (int)playmidit.bf[2];
					if (ombank[a] != midibank[a]) { fmbank[a] = midibank[a]; ombank[a] = midibank[a]; }
					playmidit.me = 0;
				}
				if ((playmidit.bf[0] & 0xF0) == 0xB0 && playmidit.bf[1] == 0x5B) {
					b = (playmidit.bf[0] & 0x0F);
					reverb[b] = (int)playmidit.bf[2];
					playmidit.me = 0;
					a = freverb[b] + reverb[b];
					if (a > 127) a = 127;
					else if (a < 0)a = 0;
					MOSM(playmidiout, 0x00005BB0 + b + (a << 16));
				}
				if ((playmidit.bf[0] & 0xF0) == 0xB0 && playmidit.bf[1] == 0x5D) {
					b = (playmidit.bf[0] & 0x0F);

					chorus[b] = (int)playmidit.bf[2];

					playmidit.me = 0;
					a = fchorus[b] + chorus[b];
					if (a > 127) a = 127;
					else if (a < 0)a = 0;
					MOSM(playmidiout, 0x00005DB0 + b + (a << 16));



				}
				if ((playmidit.bf[0] & 0xF0) == 0xB0 && playmidit.bf[1] == 0x01) {
					b = (playmidit.bf[0] & 0x0F);

					modulationwheel[b] = ((int)playmidit.bf[2] << 7) + (modulationwheel[b] & 0x7F);


					playmidit.me = 0;
					a = fmodulationwheel[b] + modulationwheel[b];
					if (a > 16383) a = 16383;
					else if (a < 0)a = 0;
					MOSM(playmidiout, 0x000001B0 + b + ((a >> 7) << 16));
					MOSM(playmidiout, 0x000021B0 + b + ((a & 127) << 16));



				}
				if ((playmidit.bf[0] & 0xF0) == 0xB0 && playmidit.bf[1] == 0x21) {
					b = (playmidit.bf[0] & 0x0F);

					modulationwheel[b] = (int)playmidit.bf[2] + (modulationwheel[b] & 0x3F80);

					playmidit.me = 0;
					a = fmodulationwheel[b] + modulationwheel[b];
					if (a > 16383) a = 16383;
					else if (a < 0)a = 0;
					MOSM(playmidiout, 0x000001B0 + b + ((a >> 7) << 16));
					MOSM(playmidiout, 0x000021B0 + b + ((a & 127) << 16));



				}
				if (playmidit.me != 0)MOSM(playmidiout, playmidit.me);
			}
			else {
				b = playmidit.bf[0] & 0x0F;

				a = (short)playmidit.bf[1] + pitchint2[b];
				if (a > 127)playmidit.bf[2] = 0;
				else if (a < 0) playmidit.bf[2] = 0;
				else {
					playmidit.bf[1] = (char)a;
					vt = (int)((double)playmidit.bf[2] * volume2[b] / 100.) * mdvol3[a][b];
					if (vt < 128 && 0 <= vt) playmidit.bf[2] = vt;
					else if (vt >= 128) playmidit.bf[2] = 127;
					else playmidit.bf[2] = 0;
				}
			
				MOSM(playmidiout, playmidit.me);
			}
			
			k++;
			if (k == 16) {
				k = 0;
				unsigned long long t = clock2() - playmidin0;
				if (t>playmidini + 2 || (t > playmidini && !(playt.bf[0] <= 0x9F && playt.bf[2]<60))) {
					for (int p = 0; p < 0x10; p++)
						MOSM(playmidiout, 0x00007BB0 + p);
					fseek(playmidig, playmidij, SEEK_SET);
					break;
				}
			}
			
		}

		playmidii = playmidini; playmidij = playmidinj;
		fseek(playmidig, 16, SEEK_CUR);
	}
Y:
	pauseall();
	midiOutClose(playmidiout);
	return 0;
}
void GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
	UINT  wyr = 0;		  // wyrber of image encoders
	UINT  size = 0;		 // size of the image encoder array in bytes
	Gdiplus::ImageCodecInfo* pImageCodecInfo = NULL;
	Gdiplus::GetImageEncodersSize(&wyr, &size);
	pImageCodecInfo = (Gdiplus::ImageCodecInfo*)(malloc(size));
	GetImageEncoders(wyr, size, pImageCodecInfo);
	for (UINT j = 0; j < wyr; j++) {
		if (wcscmp(pImageCodecInfo[j].MimeType, format) == 0) {
			*pClsid = pImageCodecInfo[j].Clsid;
			free(pImageCodecInfo);
			return;
		}
	}
	free(pImageCodecInfo);

}
void loadimg() {
	getini();
	//
	FILE *u;
	if (strcmp(recentfilelist[10], "\0") != 0 && (u = fopen(recentfilelist[10], "r")) != 0) {
		fclose(u);
		tpi = 1;
		//("%s\n", recentfilelist[10]);
		strcpy(imgname, recentfilelist[10]);
		//("%s\n",imgname);
		if (strcmp(&imgname[strlen(imgname) - 3], "bmp") != 0) {

			wchar_t imgnamet[1000]; MultiByteToWideChar(CP_ACP, 0, imgname, -1, imgnamet, 1000);
			Gdiplus::GdiplusStartupInput startupInput;
			ULONG_PTR token;
			GdiplusStartup(&token, &startupInput, NULL);
			Gdiplus::Image testImg(imgnamet, false);
			CLSID clsid;
			GetEncoderClsid(L"image/bmp", &clsid);
			strcat(imgname, ".bmp"); MultiByteToWideChar(CP_ACP, 0, imgname, -1, imgnamet, 1000);
			testImg.Save(imgnamet, &clsid);
		}
		//("! %s", imgname);
		LoadGLTextures();
	}
	else {
		tpi = 0;
	}

}
void mtinit2() {
	_fseeki64(wj6, wj6startpos2, SEEK_SET);
	for (int i = 0; i < playoffset / 2; i++) {
		fscanf(wj6, "%s", filename1); fscanf(wj6, "%s", filename1); fscanf(wj6, "%s", filename1); fscanf(wj6, "%s", filename1); fscanf(wj6, "%s", filename1);
	}
	unsigned long long npos = 5 + (5 * playoffset) * 1056;
	if (npos >= 5) _fseeki64(wj7, npos, SEEK_SET);
	int r;
	for (int z = 0; z < 500; z++) {
		//if (npos == 5) _fseeki64(wj7, 5, SEEK_SET);
		if (npos >= 5) fread(pc2, 2, 16, wj7);
		for (int x = 0; x < 16; x++) {
			bt = false; r = 0;
			for (int yi2 = 0; yi2 < 268; yi2++) {
				if (npos >= 5) {

					mdt[z][yi2][x] =
						((yi2 >= 60 + pc2[x]) && (yi2 < 188 + pc2[x])) ?
						(_ftelli64(wj7) < offs.i ? ((bt == false) ? ((r++, q = fgetc(wj7))&(bt = true) & 0) + (q & 0x0000000F) : ((bt = false) & 0) + ((q & 0x000000F0) >> 4)) : 0)
						: 0;
				}
				else {
					mdt[z][yi2][x] = 0;

				}
			}
			if (npos >= 5) {
				while (r < 64) {
					r++; fgetc(wj7);
				}
			}
		}
		if (npos < 5)npos += 1056;

	}


	mdtpnt = 0;

	if (midiout) waveOutSetPosition(playoffset * 50);
	else {
		unsigned long long len;
		fseek(playmidif, 0, SEEK_SET);
		fread(&playmidii, 8, 1, playmidif);
		fread(&playmidij, 8, 1, playmidif);
		if (playmidii >= playoffset * 50) {
			playmidini = playmidii; playmidinj = playmidij;
			fseek(playmidig, 0x10, SEEK_SET);
			fseek(playmidif, playmidij, SEEK_SET);
			goto TM;
		}
		fseek(playmidif, playmidij, SEEK_SET);
		while (1)
		{
			fread(&playmidini, 8, 1, playmidif);
			fread(&playmidinj, 8, 1, playmidif);
			fseek(playmidif, playmidinj, SEEK_SET);
			fseek(playmidig, playmidij + 0x10, SEEK_SET);
			playmidii = playmidini; playmidij = playmidinj;

			if (playmidii >= playoffset * 50) break;
		}
	TM:;
	}

	//alSourcef(isource, AL_SEC_OFFSET, (float)playoffset / 20);
	loop = 50. / (spd / 100.); ii = clock(); skip = false;
	//	system("pause");

}
int exit2(int iex) {
	if (!midiout) {
		//	MessageBox("B", "B", MB_OK);
		//cmidi.Pause();
		TerminateThread(thread1, 0);
		CloseHandle(thread1);
		Sleep(100);
		pauseall();
		midiOutClose(playmidiout);

		FILE *t = fopen("a.bat", "w");
		fprintf(t, "ping 127.0.0.1 -n 1\ndel \"%s\"\ndel \"%s\"\ndel \"%s\"\ndel a.bat", filename0, filename2, filename3);
		fclose(t);
		ShellExecute(0, "open", "a.bat", 0, 0, 0);
	}
	else {

		FILE *t = fopen(filename3, "rb");
		if (t == NULL) goto IUI2O;
		_fseeki64(t, -11, SEEK_END);
		if (fgetc(t) != 'e') {
			fclose(t);
			t = fopen("a.bat", "w");
			fprintf(t, "ping 127.0.0.1 -n 1\ndel \"%s\"\ndel \"%s\"\ndel \"%s\"\ndel a.bat", filename, filename2, filename3);
			fclose(t);
			ShellExecute(0, "open", "a.bat", 0, 0, 0);

		}
		else fclose(t);

	}
IUI2O:
	exit(iex);
	return 0;
}
IMPLEMENT_DYNCREATE(CCubeView, CView)
BEGIN_MESSAGE_MAP(CCubeView, CView)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
	ON_WM_DROPFILES()
	ON_WM_CHAR()
	ON_WM_KEYDOWN()
	ON_WM_LBUTTONDOWN()
	ON_WM_MBUTTONDOWN()
	ON_WM_HELPINFO()
	ON_WM_MOUSEWHEEL()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONDBLCLK()
	ON_WM_MOUSEMOVE()
	ON_WM_NCLBUTTONUP()
	ON_WM_MOUSELEAVE()
	ON_WM_SIZING()
	//ON_WM_NCMBUTTONDBLCLK()
	ON_WM_MBUTTONDBLCLK()
	ON_COMMAND(ID_FILE_OPEN32773, &CCubeView::OnFileOpen32773)
	ON_COMMAND(ID_VIEW_SETBACKGROUNDIMAGE, &CCubeView::OnViewSetbackgroundimage)
	ON_COMMAND(ID_VIEW_LOADSOUNDFONT, &CCubeView::OnViewLoadsoundfont)

	ON_COMMAND(ID_RECENT_11, &CCubeView::OnRecent11)
	ON_COMMAND(ID_RECENT_12, &CCubeView::OnRecent12)
	ON_COMMAND(ID_RECENT_13, &CCubeView::OnRecent13)
	ON_COMMAND(ID_RECENT_14, &CCubeView::OnRecent14)
	ON_COMMAND(ID_RECENT_15, &CCubeView::OnRecent15)
	ON_COMMAND(ID_RECENT_16, &CCubeView::OnRecent16)
	ON_COMMAND(ID_RECENT_17, &CCubeView::OnRecent17)
	ON_COMMAND(ID_RECENT_18, &CCubeView::OnRecent18)
	ON_COMMAND(ID_RECENT_19, &CCubeView::OnRecent19)
	ON_COMMAND(ID_RECENT_20, &CCubeView::OnRecent20)
	ON_COMMAND(ID_FILE_RESET, &CCubeView::OnFileReset)
	ON_COMMAND(ID_SETTINGS_VIEWSETTINGS, &CCubeView::OnSettingsViewsettings)
	ON_COMMAND(ID_PLAY_PAUSE, &CCubeView::OnPlayPause)
	ON_COMMAND(ID_PLAY_PITCHDOWN, &CCubeView::OnPlayPitchdown)
	ON_COMMAND(ID_PLAY_PITCHUP, &CCubeView::OnPlayPitchup)
	ON_COMMAND(ID_PLAY_PLAY, &CCubeView::OnPlayPlay)
	ON_COMMAND(ID_PLAY_REPLAY, &CCubeView::OnPlayReplay)
	ON_COMMAND(ID_PLAY_SKIPBACK, &CCubeView::OnPlaySkipback)
	ON_COMMAND(ID_PLAY_SKIPFORWARD, &CCubeView::OnPlaySkipforward)
	ON_COMMAND(ID_PLAY_SPEEDDOWN, &CCubeView::OnPlaySpeeddown)
	ON_COMMAND(ID_PLAY_SPEEDUP, &CCubeView::OnPlaySpeedup)
	ON_COMMAND(ID_PLAY_STOP, &CCubeView::OnPlayStop)
	ON_COMMAND(ID_PLAY_VOLUMEDOWN, &CCubeView::OnPlayVolumedown)
	ON_COMMAND(ID_PLAY_VOLUMEUP, &CCubeView::OnPlayVolumeup)
	ON_COMMAND(ID_PLAY_RESET, &CCubeView::OnPlayReset)
	ON_COMMAND(ID_PLAY_PITCHSPEED, &CCubeView::OnPlayPitchspeed)
	ON_COMMAND(ID_HELP_VIEWREADME, &CCubeView::OnHelpViewreadme)
	ON_COMMAND(ID_EDIT_CLEAR_ALL, &CCubeView::OnEditClearAll)
	ON_COMMAND(ID_RECENT_CLEARALL, &CCubeView::OnRecentClearall)
	ON_COMMAND(ID_FILE_UNINSTALL, &CCubeView::OnFileUninstall)
	ON_WM_MOVE()
	ON_WM_MOVING()
//	ON_MESSAGE(a, &CCubeView::a)
//ON_MESSAGE(WM_A, &CCubeView::OnA)
END_MESSAGE_MAP()
CCubeView::CCubeView()
{
	m_pDC = NULL;
	m_pOldPalette = NULL;

}
CCubeView::~CCubeView()
{
}
inline void CCubeView::main2(char *argv2) {
	monce = 1;
	onelp = 0;
	note = 0;
	spd = 100.;
	fnm = 1;
	onp1 = 0;
	if (strstr(argv2, "_p_") != 0) {
		*strstr(argv2, "_p_") = 0;
		ply = false;
		f = true;
		onelp = 1;
	}
	forcenomidi = 0;
	if (strstr(argv2, "_f_") != 0) {
		*strstr(argv2, "_f_") = 0;
		forcenomidi = 1;
		char a[1000];
		sprintf(a, "del \"*.nc\"", argv2);
		system(a);
		sprintf(a, "del \"%s.wav.hmp\"", argv2);
		system(a);
		sprintf(a, "del \"%s.mev\"", argv2);
		system(a);
	}
#pragma omp parallel for
	for (int i = 0; i < 16; i++) {
		damppad[i] = 0;
		softpad[i] = 0;


		chorus[i] = 0;
		dchorus[i] = 0;
		reverb[i] = 0;
		dreverb[i] = 0;
		fchorus[i] = 0;
		freverb[i] = 0;
		fmodulationwheel[i] = 0;
		footcontr[i] = 0;
		breathcontr[i] = 0;
		pan[i] = 0;
		modulationwheel[i] = 0; rmodulationwheel[i] = 0;
		midibank[i] = 0;
		midipreset[i] = 0;
		fmbank[i] = -1;
		fmpreset[i] = 128;
		ombank[i] = 0;
		ompreset[i] = 0;
		pbendratio[i] = 256;
		chnvolt[i] = 16383;

		volume2[i] = 100.; pc2[i] = 0; pc3[i] = 0; pitch[i] = 100.0;  midipitchbend[i][0] = 0; midipitchbend[i][1] = 64;
		mdvol2[0][i] = 100.;
		mdvol2[1][i] = 100.;
		mdvol2[2][i] = 100.;
		for (int j = 0; j < 128; j++) {
			mdvol3[j][i] = 1.;
			midibx[j][i] = -1;
			key[j][i] = 0.;
			keysust[j][i] = 0;
			key3[j][i] = 0;
		}
	}
	isplay = 0;
	for (int t = 0; t < strlen(argv2); t++) {
		if (argv2[t] == '?') argv2[t] = ' ';
	}
	srand(time(0));
	if (j5gyj == 0)
		for (int i = 0; i < 16;) {
			erhwh[i] = (float)rand() / 32768; iyybt[i] = (float)rand() / 32768;
			trity[i] = (float)rand() / 32768; sadae[i] = 1.;
			if (erhwh[i] + iyybt[i] + trity[i] > 0.5f&&erhwh[i] + iyybt[i] + trity[i] < 2.8) i++;
		}
	chn = strlen(argv2);
	strcpy(argv, argv2);
	if (argv[chn - 4] == '.'&&argv[chn - 3] == 'w'&&argv[chn - 2] == 'a'&&argv[chn - 1] == 'v')argv[chn - 4] = 0;
	if (argv[chn - 4] == '.'&&argv[chn - 3] == 'h'&&argv[chn - 2] == 'm'&&argv[chn - 1] == 'p')argv[chn - 8] = 0;
	{
		FILE *y;
		y = fopen(argv, "rb");
		if (y == NULL) {
			if (kj768 == true)
				MessageBox("에러-파일을 열 수 없음", "하이온의 미디 플레이어 v5.4rev2", MB_OK | MB_ICONSTOP);
			else
				MessageBox("error-can't open file", "hyon's midi player v5.4rev2", MB_OK | MB_ICONSTOP);
			exit(1);
		}
		char t[10];
		fread((char*)t, 1, 4, y);
		t[4] = 0;
		if (strcmp((char*)t, "MThd")) {
			if (kj768 == true)
				MessageBox("에러-파일을 열 수 없음", "하이온의 미디 플레이어 v5.4rev2", MB_OK | MB_ICONSTOP);
			else
				MessageBox("error-can't open file", "hyon's midi player v5.4rev2", MB_OK | MB_ICONSTOP);
			exit(1);

		}
		fclose(y);
	}
	getini();
	for (int i = 9; i > 0; i--) {
		strcpy(recentfilelist[i], recentfilelist[i - 1]);
	}

	strcpy(recentfilelist[0], argv);
	{
		char tmplst[10][1000];
		for (int it = 0; it < 10; it++)strcpy(tmplst[it], "\0");
		int u = 0, v = 0;
		FILE* w;
		while (u != 10) {
			if ((w = fopen(recentfilelist[u], "r")) != NULL) {
				for (int uv = 0; uv <= v; uv++) if (strcmp(tmplst[uv], recentfilelist[u]) == 0) goto UTV;
				strcpy(tmplst[v], recentfilelist[u]); v++;
			}
		UTV:
			u++;
		}
		for (u = 0; u < 10; u++) strcpy(recentfilelist[u], tmplst[u]);
	}
	putini();
	strcpy(t, argv);
	strcpy(filename7, t);
	filename7[strlen(filename7) - 3] = 0;
	strcat(filename7, "lrc");
	strcpy(filename, t);
	midname = new char[strlen(t) + 2];
	strcpy(midname, t);
	midname2 = new char[strlen(t) + 2];
	strcpy(midname2, midname);
	{
		int t2 = 0;
		for (int t = 0; t < strlen(midname); t++) {
			if (midname[t] == '\\') t2 = t + 1;
		}
		midname = midname + t2;
	}
	strcpy(filename0, filename);
	strcat(filename0, ".mev");
	strcat(filename, ".wav");
	strcpy(filename2, filename);
	strcat(filename2, ".nc");
	strcpy(filename3, filename);
	strcat(filename3, ".hmp");
	int fasfafafqw = 0;
	if ((i = fopen(filename3, "r")) != NULL) {
		char temp;
		fgetc(i); fgetc(i); temp = fgetc(i);
		if (temp == 'X')fasfafafqw = 1;
		fclose(i);
	}
	
	i = fopen(filename, "r");

	if (fasfafafqw == 1 || i != NULL) {//error?
		if (fasfafafqw == 0) fclose(i);
		i = fopen(filename3, "r");
		if (i != NULL) {
			char temp;
			fgetc(i); fgetc(i); temp = fgetc(i);

			if (temp == 'W' || temp == 'X') {
				if (temp == 'W') midiout = 1;
				else midiout = 0;
				if (!midiout) {
					FILE* j = fopen(filename0, "r");
					if (!j) goto M;
					else fclose(j);
				}
				if (fgetc(i) != mirror) goto M;

				_fseeki64(i, -11, SEEK_END);
				if (fgetc(i) == 'e') {
					_fseeki64(i, 4, SEEK_SET);
					if (fgetc(i) != reverse) {
						fclose(i);
						revfile(filename3);
						revfile2(filename);

					}
					fclose(i);
					SetTimer(1, 20, NULL);
					mtinit();
					return;
				}
				else fclose(i);
			}
			else fclose(i);

		}

	}
M:
	if (mode == 12 || mode == 13) {
		for (int t = 0; t < strlen(argv2); t++) {
			if (argv2[t] == 0x20) argv2[t] = '?';
		}

		char st[1000];
		char st2[1000];
		char st3[1000];
		GetModuleFileNameA(NULL, st, 1000);
		sprintf(st2, "\"%s\"", st);
		sprintf(st3, "\"%s\"_p_", argv);
		for (int i = 2; i < __argc; i++) {
			strcat(st3, " \"");
			strcat(st3, __argv[i]);
			strcat(st3, "\"");
		}
		ShellExecuteA(NULL, "OPEN", st2, st3, NULL, SW_SHOW);
		if (!midiout) {
			TerminateThread(thread1, 0);
			CloseHandle(thread1);
			Sleep(100);
			pauseall();
			midiOutClose(playmidiout);

			char a[1000];
			fclose(wj6);
			fclose(wj7);
		}
		exit2(1);
	}
	midipan = 0;
	mode = 0;
	ch = fopen(t, "rb");
	_fseeki64(ch, 10, SEEK_CUR);
	chn = ((int)((unsigned char)fgetc(ch)) << 8) + (int)((unsigned char)fgetc(ch));
	deltatime = ((int)((unsigned char)fgetc(ch)) << 8) + (int)((unsigned char)fgetc(ch));
	_fseeki64(ch, 0, SEEK_END);
	midifilesize = _ftelli64(ch);
	if (forcenomidi) midiout = 1;
	else { midiout = 0; mirror = 0; reverse = 0; }
	_fseeki64(ch, 0, SEEK_SET);
	chbuffer = new chbuf[chn];
	chbuffercnt = new int[chn];
	le = new long long[chn];
	le2 = new long long[chn];
	fileptr = new long long[chn];
	oldstate = new int[chn];
	//ch = new FILE*[chn];
	chlnt = new long long[chn];
	buf = new bool[chn];
	pnote = 0;
	for (int i = 0; i < 128; i++)key5[i] = new short[chn];
#pragma omp parallel for
	for (int i = 0; i < 1056; i++) ktemp[i] = 0;
#pragma omp parallel for
	for (int i = 0; i < chn; i++) {
		for (int j = 0; j< 128; j++) key5[j][i] = 0;
		chbuffercnt[i] = -100;
		le[i] = 0; le2[i] = 0;  oldstate[i] = 0;

		buf[i] = 0;
	}
	_fseeki64(ch, 0, SEEK_END);
	midisize = _ftelli64(ch);
	_fseeki64(ch, 0x12, SEEK_SET);
	{
		int2char rt;
		for (int j = 1; j < chn; j++) {
			rt.i = 0;
			rt.c[3] = fgetc(ch);
			rt.c[2] = fgetc(ch);
			rt.c[1] = fgetc(ch);
			rt.c[0] = fgetc(ch);
			fileptr[j - 1] = _ftelli64(ch);
			//rt.c[0] = fgetc(ch[j - 1]);
			_fseeki64(ch, (long long)rt.i + 4, SEEK_CUR);
			chlnt[j - 1] = rt.i;

		}
	rt.c[3] = fgetc(ch);
	rt.c[2] = fgetc(ch);
	rt.c[1] = fgetc(ch);
	rt.c[0] = fgetc(ch);
	fileptr[chn - 1] = _ftelli64(ch);
	chlnt[chn - 1] = rt.i;
	}
	if (kj768 == true)
		sprintf(t, "트랙 수:%d", chn);
	else
		sprintf(t, "Tracks:%d", chn);
	print(t);
	SetTimer(1, 20, NULL);
	if (chn == 0) {
		if (kj768 == true) {
			MessageBox("에러-깨진 미디 파일임", "하이온의 미디 플레이어 v5.4rev2", MB_OK | MB_ICONSTOP); Sleep(1000); if (!midiout) {
				TerminateThread(thread1, 0);
				CloseHandle(thread1);
				Sleep(100);
				pauseall();
				midiOutClose(playmidiout);

				char a[1000];
				fclose(wj6);
				fclose(wj7);

			}exit2(1);
		}
		else {
			MessageBox("error-broken midi file", "hyon's midi player v5.4rev2", MB_OK | MB_ICONSTOP); Sleep(1000); if (!midiout) {
				TerminateThread(thread1, 0);
				CloseHandle(thread1);
				Sleep(100);
				pauseall();
				midiOutClose(playmidiout);

				char a[1000];
				fclose(wj6);
				fclose(wj7);

			}exit2(1);
		}
	}
	else if (sdfsdf == 0) {
		tfnm = 1;
		midiout = 0;
		st.DoModal();
		if (midiout) st3.DoModal();
		if (tfnm == 0) fnm = 0;
		if (quit == true) {
			if (!midiout) {
				TerminateThread(thread1, 0);
				CloseHandle(thread1);
				Sleep(100);
				pauseall();
				midiOutClose(playmidiout);

				char a[1000];
				fclose(wj6);
				fclose(wj7);

			}exit2(1);
		}
	}


	dpdt = 0; ldpdt = 0;
	if (h54ui == 0) {
		int hi = 0;
		for (int y = 0; y < 16; y++) {
			if ((int)pitch[y] > hi) hi = (int)pitch[y];
		}
		hi *= 14;
		while (1) {
			if (playrate < UINT_MAX / 4 && (hi * 2) > playrate) playrate *= 2;
			else break;
		}
	}
	else {
		playrate = h54ui;
	}
	dplayrate = 1 / (double)playrate;
	ppmy = (bool)jhj56;
	resetarray();
	for (int i = 0; i < 16; i++) {
		volume2[i] *= 100.;
		pc3[i] = pitch[i] / 100;
		rpitch[i] = pc3[i];
		chvol[i] = volume2[i];
		pc2[i] = (short)(12 * log2(pc3[i]));
	}
	sppmy();
	if (midiout) wj = fopen(filename, "wb");
	else wj = fopen(filename0, "wb+");
	wj2 = fopen(filename2, "w");
	wj3 = fopen(filename3, "wb");
	fwrite(&midipan, 2, 1, wj3);
	if (midiout)fputs("W", wj3);
	else fputs("X", wj3);
	fputc(mirror, wj3);
	fputc(0, wj3);
	if (midiout) for (int i = 0; i < 40; i++) {
		fputc(opts[i], wj);
	}
	playrd10 = playrate / 10; playrd100 = playrate / 100;
	if (playrate == 22050) playrd100t = true;
	else playrd100t = false;
	playrd100t2 = false;
	if ((lrcf = fopen(filename7, "r")) == NULL) {
		lrcb = false;
	}
	else {
		lrcb = true;
		lrct3 = 0;
		strcpy(lrcc1, "\0");
		while (1) {
			lrct1 = fscanf(lrcf, " %[^\n]", lrcc3);
			if (lrct1 == EOF) break;
			else if (0x30 <= lrcc3[1] && lrcc3[1] <= 0x39)break;
		}
		if (lrct1 != EOF) {
			sscanf(lrcc3, "[%lf:%lf]%[^\n]", &lrct1, &lrct2, lrcc2);
			lrct4 = (int)((double)playrate*(lrct1*60. + lrct2));
			for (int i = 0; i < strlen(lrcc2); i++)
				if (lrcc2[i] == 0x20) lrcc2[i] = 0x10;
		}
		else {
			lrct4 = INT64_MAX;
			strcpy(lrcc2, "\0");
		}
	}
	mode = 7;
	if (strlen(recentfilelist[11]) == 0) {
		FILE *tmp2 = fopen("gm.sf2", "r");
		if (tmp2 == 0) {
			tmp2 = fopen("gm.sf2", "wb");
			fwrite(q141r, 1, 3245590, tmp2);
			fclose(tmp2);
		}
		else fclose(tmp2);
		readsf("gm.sf2");
	}
	else {
		FILE *tmp = fopen(recentfilelist[11], "r");
		if (tmp == 0) {
			if (kj768 == true)
				MessageBoxA("에러-사운드폰트 파일을 찾을 수 없음");
			else
				MessageBoxA("error-can't find soundfont file");
			FILE *tmp2 = fopen("gm.sf2", "r");
			if (tmp2 == 0) {
				tmp2 = fopen("gm.sf2", "wb");
				fwrite(q141r, 1, 3245590, tmp2);
				fclose(tmp2);
			}
			else fclose(tmp2);
			readsf("gm.sf2");
		}
		else {
			fclose(tmp);
			readsf(recentfilelist[11]);
		}
	}
	lud = 0;
	gmeo = ULONGLONG_MAX;
	oks = 0;
	for (int i = 0; i < 16; i++) {
		if (fmbank[i] == -1)
		{
			if (i == 9)midibank[i] = 127;
			else midibank[i] = 0;
		}
		else midibank[i] = fmbank[i];
		if (fmpreset[i] == 128)
			midipreset[i] = 0;
		else midipreset[i] = fmpreset[i];
		dchorus[i] = (double)(chorus[i] + fchorus[i]) / 128.; if (dchorus[i]>1) dchorus[i] = 1; if (dchorus[i]<0) dchorus[i] = 0;
		dreverb[i] = (double)(reverb[i] + freverb[i]) / 140.; if (dreverb[i]>1) dreverb[i] = 1; if (dreverb[i]<0) dreverb[i] = 0;
		rmodulationwheel[i] = (double)(modulationwheel[i] + fmodulationwheel[i]) / 16384.; if (rmodulationwheel[i]>1) rmodulationwheel[i] = 1; if (rmodulationwheel[i]<0) rmodulationwheel[i] = 0;

		if (midiout)reparsesf(i, midibank[i], midipreset[i], playrate);
	}
	if (midiout)getmidievent2();
	else getmidievent21();
	SetTimer(1, 20, NULL);
	owptr2 = GetTickCount64();
	pdt = (60 * (double)playrate) / ((double)deltatime * (double)tempo);
	owptr = 0;
	sdfsdf = 0;
}
inline void CCubeView::mtinit() {
	int r;
	mode = 12;
	system("del *.nc");
	wj6 = fopen(filename3, "rb");
	wj7 = fopen(filename3, "rb");
	fread(&midipan, 2, 1, wj6);
	_fseeki64(wj6, 0, SEEK_END);
	offs2 = _ftelli64(wj6);
	_fseeki64(wj6, -8, SEEK_END);
	_fseeki64(wj7, 0x05, SEEK_SET);
	for (int q = 0; q < 8; q++) offs.c[q] = fgetc(wj6);
	wj6startpos2 = offs.i;
	_fseeki64(wj6, offs.i, SEEK_SET);
	playlength = (offs.i - 5) / 1056;
	for (int z = 250; z < 500; z++) {
		(_ftelli64(wj7) < offs.i) ? (void)fread(pc2, 2, 16, wj7) : (void)memset(pc2, 0, 2 * 16);
		for (int x = 0; x < 16; x++) {
			r = 0; bt = false;
			for (int yi2 = 0; yi2 < 268; yi2++) {
				mdt[z - 250][yi2][x] = 0;
				mdt[z][yi2][x] =
					((yi2 >= 60 + pc2[x]) && (yi2<188 + pc2[x])) ?
					(_ftelli64(wj7) < offs.i ? ((bt == false) ? ((r++, q = fgetc(wj7))&(bt = true) & 0) + (q & 0x0000000F) : ((bt = false) & 0) + ((q & 0x000000F0) >> 4)) : 0)
					: 0;
			}
			while (r < 64) {
				r++; fgetc(wj7);
			}
		}
	}
	owptr6 = 0;
}
inline void CCubeView::arecloop() {
	if (lrcb)fclose(lrcf);
	if (playrate < 44100) wptr = (wptr * 440) / 441;
	if (midiout) {
		wjputs(wj);
		mode = 11;
		_fseeki64(wj, 0x18, SEEK_SET);
		wjct2.i = playrate;
		fputc(wjct2.c[0], wj); fputc(wjct2.c[1], wj); fputc(wjct2.c[2], wj); fputc(wjct2.c[3], wj);
		wjct2.i = playrate * 2;
		fputc(wjct2.c[0], wj); fputc(wjct2.c[1], wj); fputc(wjct2.c[2], wj); fputc(wjct2.c[3], wj);
		unsigned long long tmnn;
		_fseeki64(wj, 0, SEEK_END);
		tmnn = _ftelli64(wj) - 0x2c;
		_fseeki64(wj, 0x28, SEEK_SET);
		//int factt;
		wjct2.i = tmnn;
		fputc(wjct2.c[0], wj); fputc(wjct2.c[1], wj); fputc(wjct2.c[2], wj); fputc(wjct2.c[3], wj);
		_fseeki64(wj, 0x00, SEEK_END);
		tmnn = _ftelli64(wj) - 8;
		_fseeki64(wj, 0x04, SEEK_SET);
		wjct2.i = tmnn;
		fputc(wjct2.c[0], wj); fputc(wjct2.c[1], wj); fputc(wjct2.c[2], wj); fputc(wjct2.c[3], wj);
	}
	fclose(wj);
	fclose(wj2); fclose(wj3);
	owptr4 = note;
	int timers = 0;
	int t;
	wptr = 0;
	wj4 = fopen(filename2, "r");
	while ((t = fgetc(wj4)) != EOF) if (t == 0x0A) wptr++;
	wptr = (wptr / 35) * 10 + (wptr % 35 - 5) / 3;
	wptr *= 4410;
	fseek(wj4,0,SEEK_SET);
	owptr4 = note;
	
	unsigned long long ct2[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	int gergafrvh = 0, gergafrvh2 = wptr / 158760000;
	int gergafrvm = 0, gergafrvm2 = wptr / 2646000 - gergafrvh2 * 60;
	double gergafrv = 0, gergafrv2 = ((double)wptr) / 44100 - gergafrvm2 * 60 - gergafrvh2 * 360;

	int iiii = 0; unsigned long long owptr23 = 0;
	FILE *wj5;
	wj5 = fopen(filename3, "ab");
	_fseeki64(wj5, 0x00, SEEK_END);
	long long filename3l = _ftelli64(wj5);


	if (kj768 == false)
		fprintf(wj5, "0:0:0.0/%d:%d:%.1f\n0/%llu\n0notes/sec,0polyphony\n%ubpm,%d/%dbeat,%s\n%c\n", gergafrvh2, gergafrvm2, gergafrv2, owptr4, tempo, bth, btl, ekeysigm[keysig2][keysig], 0x10);
	else
		fprintf(wj5, "0:0:0.0/%d:%d:%.1f\n0/%llu\n0음표/초,0눌림\n%ubpm,%d/%d박자,%s\n%c\n", gergafrvh2, gergafrvm2, gergafrv2, owptr4, tempo, bth, btl, kkeysigm[keysig2][keysig], 0x10);
	while (1) {

		if (gergafrv >= 60.0) {
			gergafrv -= 60;
			gergafrvm++;
		}
		if (gergafrvm == 60) {
			gergafrvm -= 60;
			gergafrvh++;
		}


		if (timers % 10 == 0) {
			fscanf(wj4, "%u", &tempo); fscanf(wj4, "%d", &bth); fscanf(wj4, "%d", &btl); fscanf(wj4, "%d", &keysig); fscanf(wj4, "%d", &keysig2);
		}
		if (fscanf(wj4, "%llu", &note) == EOF) {
			note = owptr4;
			fscanf(wj4, "%lld", &pnote);
			fscanf(wj4, "%s", mdstrings);
			for (int i = 0; i < 9; i++) {
				ct2[i] = ct2[i + 1];
			}
			ct2[9] = note;
			if (kj768 == false) {
				fprintf(wj5, "%d:%d:%.1f/%d:%d:%.1f\n%llu/%llu\n%llunotes/sec,%lldpolyphony\n%ubpm,%d/%dbeat,%s\n%s\n", gergafrvh, gergafrvm, gergafrv, gergafrvh2, gergafrvm2, gergafrv2, note, owptr4, note - ct2[0], pnote, tempo, bth, btl, ekeysigm[keysig2][keysig], mdstrings);
			}
			else {
				fprintf(wj5, "%d:%d:%.1f/%d:%d:%.1f\n%llu/%llu\n%llu음표/초,%lld눌림\n%ubpm,%d/%d박자,%s\n%s\n", gergafrvh, gergafrvm, gergafrv, gergafrvh2, gergafrvm2, gergafrv2, note, owptr4, note - ct2[0], pnote, tempo, bth, btl, kkeysigm[keysig2][keysig], mdstrings);

			}
			gergafrv += 0.1;

			timers++;
			break;
		}
		fscanf(wj4, "%lld", &pnote);
		fscanf(wj4, "%s", mdstrings);
		for (int i = 0; i < 9; i++) {
			ct2[i] = ct2[i + 1];
		}
		ct2[9] = note;
		if (kj768 == false) {
			fprintf(wj5, "%d:%d:%.1f/%d:%d:%.1f\n%llu/%llu\n%llunotes/sec,%lldpolyphony\n%ubpm,%d/%dbeat,%s\n%s\n", gergafrvh, gergafrvm, gergafrv, gergafrvh2, gergafrvm2, gergafrv2, note, owptr4, note - ct2[0], pnote, tempo, bth, btl, ekeysigm[keysig2][keysig], mdstrings);
		}
		else {
			fprintf(wj5, "%d:%d:%.1f/%d:%d:%.1f\n%llu/%llu\n%llu음표/초,%lld눌림\n%ubpm,%d/%d박자,%s\n%s\n", gergafrvh, gergafrvm, gergafrv, gergafrvh2, gergafrvm2, gergafrv2, note, owptr4, note - ct2[0], pnote, tempo, bth, btl, kkeysigm[keysig2][keysig], mdstrings);

		}
		gergafrv += 0.1;

		timers++;
	}
	fwrite("eof", 1, 3, wj5);
	fwrite(&filename3l, 8, 1, wj5);

	fclose(wj4); fclose(wj5);
	{
		char tempstr[1000];
		sprintf(tempstr, "del %s", filename2);
		system(tempstr);
	}
	if (reverse == true) {
		revfile(filename3); revfile2(filename);
	}
	if (midiout)for (int i = 0; i < 16; i++) { pitch[i] = 100; pitchint[i] = 0; }
	mtinit();

}
void CCubeView::OnDraw(CDC* /*pDC*/)
{
	CCubeDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	DrawScene();
}
inline void CCubeView::print(char *val) {
	CClientDC dc(this);
	dc.SetTextColor(RGB(0, 0, 0));
	if (stringcount >= 20) {
		stringcount = 0;
		for (int i = 0; i < 22; i++) {
			dc.TextOut(asfaq, brebe + 20 * i, "                                                                                                                                                                                 ");
		}
	}
	strcpy(t, val);
	strcat(t, "            ");
	dc.TextOut(asfaq, brebe + 20 * stringcount, t);
	stringcount++;
}
BOOL CCubeView::PreCreateWindow(CREATESTRUCT& cs)
{
	// An OpenGL window must be created with the following flags and must not
	// include CS_PARENTDC for the class style. Refer to SetPixelFormat
	// documentation in the "Comments" section for further information.
	cs.style |= WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

	return CView::PreCreateWindow(cs);
}
int CCubeView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	st.Create(IDD_settings);
	st2.Create(IDD_viewsettings);
	st3.Create(IDD_DIALOG1);
	st4.Create(IDD_DIALOG2);
	getini();
	{
		char tmplst[10][1000];
		int u = 0, v = 0;
		FILE* w; for (int it = 0; it < 10; it++)strcpy(tmplst[it], "\0");
		while (u != 10) {
			if ((w = fopen(recentfilelist[u], "r")) != NULL) {
				for (int uv = 0; uv <= v; uv++) if (strcmp(tmplst[uv], recentfilelist[u]) == 0) goto UTV;
				strcpy(tmplst[v], recentfilelist[u]); v++;
			}
		UTV:
			u++;
		}
		for (u = 0; u < 10; u++) strcpy(recentfilelist[u], tmplst[u]);
	}
	putini();
	Init(); // initialize OpenGL
	loadimg();

	//("*");
	if (kj768 == true)
		GetParent()->SetWindowTextA("하이온의 미디 플레이어 v5.4rev2");
	else
		GetParent()->SetWindowTextA("hyon's midi player v5.4rev2");
	CRect rct; GetClientRect(rct);
	windx = rct.Width();
	windy = rct.Height();
	if (__argc > 1) {

		if (strcmp(__argv[1], "(null)") != 0) {

			sdfsdf = 1;

			main2(__argv[1]);


		}
	}
	SetTimer(1, 20, NULL);
	return 0;
}
void CCubeView::OnDestroy()
{
	HGLRC   hrc;
	KillTimer(1);
	if (!midiout) {
		TerminateThread(thread1, 0);
		CloseHandle(thread1);
		Sleep(100);
		pauseall();
		midiOutClose(playmidiout);

		
	}
	else {

		FILE *t = fopen(filename3, "rb");
		if (t == NULL) goto IUIO;
		_fseeki64(t, -11, SEEK_END);
		if (fgetc(t) != 'e') {
			fclose(t);
			

		}
		else fclose(t);

	}
IUIO:
	hrc = ::wglGetCurrentContext();

	::wglMakeCurrent(NULL, NULL);

	if (hrc)
		::wglDeleteContext(hrc);

	if (m_pOldPalette)
		m_pDC->SelectPalette(m_pOldPalette, FALSE);

	if (m_pDC)
		delete m_pDC;
	putini();
	CView::OnDestroy();
}
void CCubeView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);
	if (cy > 0)
	{
		glViewport(0, 0, cx, cy);


		if ((m_oldRect.right > cx) || (m_oldRect.bottom > cy))
			RedrawWindow();

		m_oldRect.right = cx;
		m_oldRect.bottom = cy;
		f346h = (double)cx / (double)cy;
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity(); glOrtho(-1.3*f346h, 1.3*f346h, -1.3, 1.3, -100, 100);		//gluPerspective(45.0f, (GLdouble)cx / cy, 3.0f, 7.0f);
		glMatrixMode(GL_MODELVIEW);
		if (sq1) {
			RECT rect;
			CMainFrame *frame = (CMainFrame*)AfxGetMainWnd();
			frame->GetWindowRect(&rect);
			windv = rect.bottom - rect.top;
			windh = rect.right - rect.left;
			windowx = rect.left;
			windowy = rect.top;
			putini();
		}
	}
}
void CCubeView::OnTimer(UINT_PTR nIDEvent)
{
	CRect rct; GetClientRect(rct);
	windx = rct.Width();
	windy = rct.Height();
	//	putini();
	DrawScene();

	CView::OnTimer(nIDEvent);

	// Eat spurious WM_TIMER messages
	MSG msg;
	while (::PeekMessage(&msg, m_hWnd, WM_TIMER, WM_TIMER, PM_REMOVE));
}
void CCubeView::Init()
{
	PIXELFORMATDESCRIPTOR pfd;
	int         n;
	HGLRC       hrc;
	GLfloat     fMaxObjSize, fAspect;
	GLfloat     fNearPlane, fFarPlane;

	m_pDC = new CClientDC(this);

	ASSERT(m_pDC != NULL);

	if (!bSetupPixelFormat())
		return;

	n = ::GetPixelFormat(m_pDC->GetSafeHdc());
	::DescribePixelFormat(m_pDC->GetSafeHdc(), n, sizeof(pfd), &pfd);



	hrc = wglCreateContext(m_pDC->GetSafeHdc());
	wglMakeCurrent(m_pDC->GetSafeHdc(), hrc);

	GetClientRect(&m_oldRect);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);

	if (m_oldRect.bottom)
		fAspect = (GLfloat)m_oldRect.right / m_oldRect.bottom;
	else    // don't divide by zero, not that we should ever run into that...
		fAspect = 1.0f;
	fNearPlane = 3.0f;
	fFarPlane = 7.0f;
	fMaxObjSize = 3.0f;
	m_fRadius = fNearPlane + fMaxObjSize / 2.0f;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, fAspect, fNearPlane, fFarPlane);
	glMatrixMode(GL_MODELVIEW);

}
BOOL CCubeView::bSetupPixelFormat()
{
	static PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),  // size of this pfd
		1,                              // version wyrber
		PFD_DRAW_TO_WINDOW |            // support window
		PFD_SUPPORT_OPENGL |          // support OpenGL
		PFD_DOUBLEBUFFER,             // double buffered
		PFD_TYPE_RGBA,                  // RGBA type
		24,                             // 24-bit color depth
		0, 0, 0, 0, 0, 0,               // color bits ignored
		0,                              // no alpha buffer
		0,                              // shift bit ignored
		0,                              // no accumulation buffer
		0, 0, 0, 0,                     // accum bits ignored
		32,                             // 32-bit z-buffer
		0,                              // no stencil buffer
		0,                              // no auxiliary buffer
		PFD_MAIN_PLANE,                 // main layer
		0,                              // reserved
		0, 0, 0                         // layer masks ignored
	};
	int pixelformat;

	if ((pixelformat = ChoosePixelFormat(m_pDC->GetSafeHdc(), &pfd)) == 0)
	{
		return FALSE;
	}

	if (SetPixelFormat(m_pDC->GetSafeHdc(), pixelformat, &pfd) == FALSE)
	{
		return FALSE;
	}

	return TRUE;
}
inline void CCubeView::DrawScene(void)
{
	if (ab == 1) {
		ab = 0;
		//st3.OnInitDialog();
	}
	UpdateData(1);
	::SendMessage(st3.GetSafeHwnd(), WM_TIMER, 1, 0);
	sq1 = 1;
	if (ply == false && f == false && onelp == 0) goto T2;

	if (skip == true)goto T2;
	if (j5j58 == true) {
		j5j58 = false;
		putini();

	}
	//(")");
	{
		
		CClientDC dc(this);
		if (mode < 0) {
			if (chrcnt == -1) { chrcnt = 0; main2(0); }
		}

		else if (mode == 7) {
			if (stop == true) { mode = 8; goto M8P; }
			dc.SetTextColor(RGB(0, 0, 0));
			KillTimer(1);
			while (1) {
				owptr++; owptr6++;
				if (owptr >= ldpdt) {
					dpdt += pdt;
					if (dpdt > 300000) { dpdt -= 200000; owptr -= 200000; }
					ldpdt = (long)dpdt;

					mptr++;
					//if(chn<5) getmidievent();
					//else 
					if (midiout)getmidieventacc();
					else getmidieventacc1();
					tmpmode = mode;
					for (int i = 0; i < chn; i++) {
						if (buf[i] != true) goto AP;
					}
					mode = 8; wptr += (long long)owptr6; break;
				}AP:
				if (midiout) {
					wjct2.i = getleft();

					wjput(wjct2.c, wj);
				}
				if (owptr6 == playrd100) {


					if (playrd100t) {
						if (playrd100t2) {
							owptr6 = 0; playrd100t2 = false; wptr += (playrd100 + 1);
						}
						else {
							owptr6 = -1; playrd100t2 = true; wptr += playrd100;
						}
					}
					else {
						owptr6 = 0; wptr += playrd100;
					}
					owptr4++;
					if (owptr4==10) {
						if (GetTickCount64() - owptr2 > 2000) {

							//owptr3 += 2;
							owptr2 = GetTickCount64();
							clocks2 = (double)wptr / (double)playrate;


							if (kj768 == true)
								sprintf(t, "불러오는 중-%.1lfsec,%llunote", clocks2, note);
							else
								sprintf(t, "Loading - %.1lf sec,%llunote", clocks2, note);

							dc.TextOut(0, 0 + 20 * stringcount, t);


							if (kj768 == true)
								sprintf(t, "변환 속도 %.1lfx               ", (clocks2 - clocks));
							else
								sprintf(t, "Convert Speed %.1lfx               ", (clocks2 - clocks));


							dc.TextOut(0, 0 + 20 * (stringcount + 1), t);
							if (kj768 == true)
								sprintf(t, "하이온의 미디 플레이어 v5.4rev2 -불러오는 중:%.1lfx", (clocks2 - clocks));
							else
								sprintf(t, "hyon's midi player v5.4rev2 -Loading:%.1lfx", (clocks2 - clocks));
							clocks = clocks2;
							GetParent()->SetWindowTextA(t);
							mode = 10;
						}
						if (fnm);
						else if (note - onp1 > 2000 && !midiout) {
							char st[1000];
							char st2[1000];
							char st3[1000];
							GetModuleFileNameA(NULL, st, 1000);
							sprintf(st2, "\"%s\"", st);
							if (ply == false) sprintf(st3, "\"%s\"_f__p_", midname2);
							else sprintf(st3, "\"%s\"_f_", midname2);
							for (int i = 2; i < __argc; i++) {
								strcat(st3, " \"");
								strcat(st3, __argv[i]);
								strcat(st3, "\"");
							}
							if (!midiout) {
								TerminateThread(thread1, 0);
								CloseHandle(thread1);
								Sleep(100);
								pauseall();
								midiOutClose(playmidiout);
							}
							ShellExecuteA(NULL, "OPEN", st2, st3, NULL, SW_SHOW);

							exit2(1);

						}
						onp1 = note;

						owptr5--; if (owptr5 == -1) {
							owptr5 = 9; fprintf(wj2, "%u\n%d\n%d\n%d\n%d\n", tempo, bth, btl, keysig, keysig2);
						}
						owptr4 = 0;
						pnote = 0;
						for (int i = 0; i < 128; i++)
							for (int j = 0; j < 16; j++) pnote += (key3[i][j] != 0);
						if (lrcb) {
							if (wptr > lrct4) {
								lrct3 = lrct4;
								strcpy(lrcc1, lrcc2);

								while (1) {

									lrct1 = fscanf(lrcf, " %[^\n]", lrcc3);

									if (lrct1 == EOF) break;
									else if (0x30 <= lrcc3[1] && lrcc3[1] <= 0x39)break;
								}
								if (lrct1 != EOF) {
									sscanf(lrcc3, "[%lf:%lf]%[^\n]", &lrct1, &lrct2, lrcc2);
									lrct4 = (int)((double)playrate*(lrct1*60. + lrct2));
									for (int i = 0; i < strlen(lrcc2); i++)
										if (lrcc2[i] == 0x20) lrcc2[i] = 0x10;
										else if (lrcc2[i] == 0x0A) lrcc2[i] = 0x10;
										else if (lrcc2[i] == 0x0D) lrcc2[i] = 0x10;
								}
								else {
									lrct4 = INT64_MAX;
									strcpy(lrcc2, "\0");

								}
							}
							fprintf(wj2, "%llu\n%lld\n%s%c\n", note, pnote, lrcc1, 0x10);
						}
						else fprintf(wj2, "%llu\n%lld\n%s%c\n", note, pnote, mdstrings, 0x10);
					}

#pragma omp parallel for
					for (int j = 0; j < 16; j++) {
						ktemp[j << 1] = pc2[j] & 0x00FF;
						ktemp[(j << 1) + 1] = (pc2[j] & 0xFF00) >> 8;
					}

					for (int i = 0; i < 16; i++) {
						for (int j = 0; j < 64; j++) {
							ktemp[(i << 6) + j + 32] = (key3[(j << 1) + 1][i] << 4) | key3[j << 1][i];
						}
					}
					fwrite(ktemp, 1, 1056, wj3);
#pragma omp parallel for
					for (int j = 0; j < 1056; j++) {
						ktemp[j] = 0;
					}
					if (mode == 10) { mode = tmpmode; break; }
				}
			}M8P:
			SetTimer(1, 20, NULL);
		}
		else if (mode == 8) {
			arecloop();
		}
		else if (mode == 12 || mode == 13) {
		if (speedupdate) {
			speedupdate = 0;
			
			unsigned long clocktemp = clock();
			_clock2 = (int)(_clock2 + (spd / 100. * (double)(clocktemp - clock_start)));
			clock_start = clocktemp;
			spd = spd2;
			UpdateData(1);
			KillTimer(1);
			SetTimer(1, (int)(17. / (spd / 100.)), 0);
			//SetTimer(1, playspeed / 5 * 2, 0);
			if (kj768 == true)
				sprintf(t, "하이온의 미디 플레이어 v5.4rev2 -재생 속도 %d%c", (int)(spd), '%');
			else
				sprintf(t, "hyon's midi player v5.4rev2 -speed %d%c", (int)spd, '%');
			GetParent()->SetWindowTextA(t);
			UpdateData(0);
		}
			if (setmidivol) {
				setmidivol = 0;
				waveOutSetVolume(0, (int)(playvolume * 65535.) + ((int)(playvolume * 65535.) << 16));
				midiOutSetVolume(playmidiout, (int)(playvolume * 65535.) + ((int)(playvolume * 65535.) << 16));
			}
			if (mode == 13 && mpoffset == 1) {
				
				mpoffset = 0;
				unsigned int oclo = clock();
				ro = 1;
				int oisplay;
				if (!midiout) {
					oisplay = isplay;
					isplay = 0;
				}
				if (skip == false) {
					skip = true;
					playmidin0 += (playoffset - nplayoffset) * 50; playoffset -= playoffset - nplayoffset;
					mtinit2();
				}
				if (!midiout) { isplay = oisplay; playmidin0 += clock() - oclo; }
			}
			if (htit == true && hortxti != 2) {
				htit = false; print2st("\0");
			}
			if (mode == 12) {
				spd = 100.;

				for (int i = 0; i < 16; i++) {
					midipitchbend2[i] = 0; pbendratio[i] = 256;
					midipitchbend_[i] = 0; ipaused[i] = 0; bpaused[i] = 0; cpaused[i] = 0; rpaused[i] = 0; mpaused[i] = 0;
					fmpreset[i] = 0; fmbank[i] = 0;
					ompreset[i] = 0; ombank[i] = 0;

					midipreset[i] = 0; midibank[i] = 0; pitch[i] = volume2[i] = 100.;
					if (i == 9) {
						fmbank[i] = 127;
						ombank[i] = 127;
						midibank[i] = 127;
					}
				}
				ocl = clock();
				playoffset = 0;
				mpoffset = 0;
				fs = true;
				if (!midiout) {
				}
				else {
					
					long fileSize;
					FILE *fp = fopen(filename, "rb");
					long len;
					fseek(fp, 0, SEEK_END);
					len = ftell(fp);
					fseek(fp, 0, SEEK_SET);
					buffer = (char*)calloc(1, len + 1);
					fread(buffer, 1, len, fp);
					fclose(fp);
					fileSize = len;
					long *mPtr;
					char *tmpPtr;
					fmtChunk mFmtChunk;
					mPtr = (long*)buffer;
					mPtr += 3;
					tmpPtr = (char*)mPtr;
					memcpy(&mFmtChunk, tmpPtr, sizeof(mFmtChunk));
					tmpPtr += 8;
					tmpPtr += mFmtChunk.chunkDataSize;
					mPtr = (long*)tmpPtr;
					tmpPtr = (char*)mPtr;
					memcpy(&mDataChunk, tmpPtr, sizeof(mDataChunk));
					mPtr += 2;
					buffer = (char*)malloc(mDataChunk.chunkDataSize);
					memcpy(buffer, mPtr, mDataChunk.chunkDataSize);
					wf.wFormatTag = mFmtChunk.compressionCode;
					wf.nChannels = mFmtChunk.numChannels;
					wf.nSamplesPerSec = mFmtChunk.sampleRate;
					playrate = mFmtChunk.sampleRate;
					wf.nAvgBytesPerSec = mFmtChunk.avgBytesPerSec;
					wf.nBlockAlign = mFmtChunk.blockAlign;
					wf.wBitsPerSample = mFmtChunk.significantBitsPerSample;
					wf.cbSize = mFmtChunk.extraFormatBytes;
					wh.lpData = buffer;
					wh.dwBufferLength = mDataChunk.chunkDataSize;
					wh.dwFlags = 0;
					wh.dwLoops = 0;
					if (wavestate) {
						waveOutReset(hWaveOut);
						waveOutUnprepareHeader(hWaveOut, &wh, sizeof(wh));
						waveOutClose(hWaveOut);
					}
					waveOutOpen(&hWaveOut, WAVE_MAPPER, &wf, 0, 0, CALLBACK_NULL);
					waveOutSetVolume(0, (int)(playvolume * 65535.) + ((int)(playvolume * 65535.) << 16));
					wavestate = 1;
					waveOutPrepareHeader(hWaveOut, &wh, sizeof(wh));
					waveOutWrite(hWaveOut, &wh, sizeof(wh));
					waveOutPause(hWaveOut);
				}

				g3446 = false;
				mode = 13;
				SetTimer(1, 17, NULL);
				getini();
				ii = clock();
				loop = 50. / (spd / 100.);
				mdtpnt = 0;
				filename[2] = 0;
			}
			if (f == true && onelp == 0) {
				f = false;
				if (ply == true) {
					if (midiout)waveOutSetPosition(50 * playoffset);
					isplay = 1;
					if (midiout)waveOutRestart(hWaveOut);
					ii += clock() - gtc;
					playmidin0 += clock() - gtc;
				}
				else {
					gtc = clock();
					isplay = 0;
					if (midiout)waveOutPause(hWaveOut);
					goto T;

				}
			}
			else if (onelp !=0) onelp ++;
			if (onelp == 3) onelp = 0;
			if (owptr6 < 24 * 2 && !midiout) {
				playoffset = 0;
				if (isplay) {
					isplay = 0;
				}
			}
			if (owptr6 == 24 * 2 && !midiout) {
				playoffset = 0;
				gtc = clock();
				isplay = 1;
				thread1 = CreateThread(NULL, 0, playmidi, &thread1d, 0, &thread1e);
				clock2_init();
				ii += clock() - gtc;
				oplaymidii = 0;
			}
			if (owptr6 < 24 * 2 && midiout) {
				playoffset = 0;
				if (isplay) {
					if (midiout)waveOutSetPosition(0), waveOutPause(hWaveOut);

					isplay = 0;
				}
			}
			if (owptr6 > 24 * 2 && !midiout && oplaymidii != playmidii) {
				oplaymidii = playmidii;
				if (playmidii > playoffset * 50 + 150 + 200) {

					ii -= (int)(20. / (spd / 100.));
				}
				else if (playmidii + 150< playoffset * 50 + 200) {
					ii += (int)(20. / (spd / 100.));
				}
			}
			if (owptr6 == 24 * 2 && midiout) {
				playoffset = 0;
				gtc = clock();
				isplay = 1;
				if (midiout)waveOutRestart(hWaveOut);

				system("cls");

				ii += clock() - gtc;
			}
			if (owptr6 < 80) owptr6++;
			if (clock() < (int)loop + ii) {
				while (clock() < (int)loop + ii) { Sleep(1); }
				ps = false;
			}
			else ps = true;

			pb = false;
			loop += 50. / (spd / 100.);
			lt++;
			if (lt == 2) lt = 0;
			short ph = 0;
			short pl = 0;
			for (int z = 0; z <5; z++) {
				ph = 0; pl = 0;
				if (_ftelli64(wj7) < offs.i)
					fread(pc2, 2, 16, wj7);

				for (int i = 0; i < 16; i++) {
					if (ph < pc2[i])ph = pc2[i];
					else if (pl > pc2[i])pl = pc2[i];
				}
				ph += 188;
				pl += 60;
				if (ph < 1) ph = 1;
				if (ph > 267) ph = 267;
				if (pl < 0) pl = 0;
				if (pl > 266) pl = 266;
				//("%d %d \n", pl, ph);
				if (_ftelli64(wj7) < offs.i)
					fread(filename8, 1, 1024, wj7);
				else
					memset(filename8, 0, 1024);
				q = -1;

				for (int x = 0; x < 16; x++) {
					bt = 0;
					q = x * 64 - 1;
					for (int yi2 = 0; yi2 < 268; yi2++) {
						if ((mdtpnt + z) < 500)
							mdt[mdtpnt + z][yi2][x] =
							((yi2 >= 60 + pc2[x]) && (yi2 < 188 + pc2[x])) ?
							(((bt = !bt) == true) ? (filename8[++q] & 0x0F) : (filename8[q] >> 4))
							: 0;
						else
							mdt[mdtpnt + z - 500][yi2][x] =
							((yi2 >= 60 + pc2[x]) && (yi2 < 188 + pc2[x])) ?
							(((bt = !bt) == true) ? (filename8[++q] & 0x0F) : (filename8[q] >> 4))
							: 0;
					}
				}
			}playoffset += 1; mdtpnt += 100 / 20;
			if (mdtpnt == 500) mdtpnt = 0;

			if (lt == 0) {
				if (filename7[0] != 0) {
					if (kj768 == true)
						sprintf(t, "하이온의 미디 플레이어 v5.4rev2 -%s/%dfps/%s/%s", midname, fps, midstr, filename7);
					else
						sprintf(t, "hyon's midi player v5.4rev2 -%s/%dfps/%s/%s", midname, fps, midstr, filename7);

				}
				else {
					if (kj768 == true)
						sprintf(t, "하이온의 미디 플레이어 v5.4rev2 -%s/%dfps/%s", midname, fps, midstr);
					else
						sprintf(t, "hyon's midi player v5.4rev2 -%s/%dfps/%s", midname, fps, midstr);
				}
				GetParent()->SetWindowTextA(t);

			}
			if (ps == true && owptr6 > 25 * 2) {

				if (lt == 0) { fscanf(wj6, "%s", filename1); fscanf(wj6, "%s", filename4); fscanf(wj6, "%s", filename6); fscanf(wj6, "%s", midstr); fscanf(wj6, "%s", filename7); }
				for (int i = 0; i < strlen(filename7); i++) {
					if (filename7[i] == 0x10) filename7[i] = 0x20;
				}
				goto EX;
			}
			if (clock() - ocl>4000) {

				ocl = clock();

				putini();
				fps = bfps / 4;
				bfps = 0;
				//waveOutSetPosition(50 * playoffset);
			}
			bfps++;
			if (tpi == 0) {
				glClearColor(0, 0, 0, 0);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE);
				glEnable(GL_BLEND);

				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				glPushMatrix();
				glTranslatef(0.0f, 0.0f, -3);
				glColor4f(1, 1, 1, 1);
			}
			else {
				glClearColor(1, 1, 1, 1);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE);
				glEnable(GL_BLEND);

				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				glPushMatrix();
				glTranslatef(0.0f, 0.0f, -3);
				glColor4f(1, 1, 1, 1);
				glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D, tex);
				glBegin(GL_QUADS);
				glTexCoord3f(0.0f, 0.0f, -1.0f); glVertex3f(-3.33f, -1.3f, -10);
				glTexCoord3f(1.0f, 0.0f, -1.0f); glVertex3f(3.33f, -1.3f, -10);
				glTexCoord3f(1.0f, 1.0f, -1.0f); glVertex3f(3.33f, 1.3f, -10);
				glTexCoord3f(0.0f, 1.0f, -1.0f); glVertex3f(-3.33f, 1.3f, -10);
				glEnd();
				glDisable(GL_TEXTURE_2D);
			}
			//trand = true;
			if (trand == true) {
				tr = (float)rand() / 32768;
				tg = (float)rand() / 32768;
				tb = (float)rand() / 32768;
			}
			glColor4f(tr, tg, tb, ta);
			glLineWidth(2);
			float txtystmp;
			if (owptr6 > 25 * 2 || fs == true) {

				strcpy(ofilename2, filename1);
				if (lt == 0) {
					fs = false;
					fscanf(wj6, "%s", filename1);
				}
				txtys = 0;

				if (_ftelli64(wj6) >= offs2 - 12) {
					//Sleep(1000);
					fclose(wj6);
					fclose(wj7);
					putini();
					if (midiout) {
						waveOutUnprepareHeader(hWaveOut, &wh, sizeof(wh));
						wavestate = 0;
						free(buffer);
					}
					else {
						Sleep(100);
						Sleep(100);
					}
					mode = 14; glPopMatrix();
					glFinish();
					SwapBuffers(wglGetCurrentDC()); //("%d", mode); goto T;
				}
				if (hortxti == 0) {
					txtystmp = fsize*fspace / 143;
					if (textmode % 2 == 0) { printchr(asfaq, brebe, windx, windy, filename1, 0);  txtys += txtystmp; }
					if (lt == 0)	fscanf(wj6, "%s", filename4);
					if (textmode != -1) {
						printchr(asfaq, brebe, windx, windy, filename4, txtys);  txtys += txtystmp;
					}
					if (lt == 0)	fscanf(wj6, "%s", filename6);
					if (textmode == 0 || textmode > 2) { printchr(asfaq, brebe, windx, windy, filename6, txtys);  txtys += txtystmp; }
					if (lt == 0)	fscanf(wj6, "%s", midstr);
					//if (textmode == 0) { printchr(asfaq, brebe, windx, windy, , txtys);  txtys += fsize*fspace/143; }
					if (lt == 0)	fscanf(wj6, "%s", filename7);
					if (textmode == 0 || textmode > 2) { printchr(asfaq, brebe, windx, windy, filename7, txtys);  txtys += txtystmp; }
				}
				else if (hortxti == 1) {
					txtystmp = fsize*fspace / 1000;
					if (textmode % 2 == 0) { printchr(asfaq + windx / 15 * txtys, brebe, windx, windy, filename1, 0); txtys += txtystmp; }
					if (lt == 0)	fscanf(wj6, "%s", filename4);
					if (textmode != -1) { printchr(asfaq + windx / 15 * txtys, brebe, windx, windy, filename4, 0); txtys += txtystmp; }
					if (lt == 0)	fscanf(wj6, "%s", filename6);
					if (textmode == 0 || textmode > 2) { printchr(asfaq + windx / 15 * txtys, brebe, windx, windy, filename6, 0); txtys += txtystmp; }
					if (lt == 0)	fscanf(wj6, "%s", midstr);
					//if (textmode == 0) { printchr(asfaq + windx / 15 * txtys, brebe, windx, windy, , 0); txtys += 3; }
					if (lt == 0)	fscanf(wj6, "%s", filename7);
					if (textmode == 0 || textmode > 2) { printchr(asfaq + windx / 15 * txtys, brebe, windx, windy, filename7, 0); txtys += txtystmp; }
				}
				else {
					htit = true;
					memset(filename9, ' ', 400);
					if (textmode % 2 == 0) { strcpy(filename9 + txtys, filename1); filename9[strlen(filename9)] = ' '; txtys += 30; }
					if (lt == 0)	fscanf(wj6, "%s", filename4);
					if (textmode != -1) { strcpy(filename9 + txtys, filename4); filename9[strlen(filename9)] = ' '; txtys += 20; }
					if (lt == 0)	fscanf(wj6, "%s", filename6);
					if (textmode == 0 || textmode > 2) { strcpy(filename9 + txtys, filename6); filename9[strlen(filename9)] = ' '; txtys += 20; }
					if (lt == 0)	fscanf(wj6, "%s", midstr);
					//if (textmode == 0) { printchr(asfaq + windx / 15 * txtys, brebe, windx, windy, , 0); txtys += 3; }
					if (lt == 0)	fscanf(wj6, "%s", filename7);
					if (textmode == 0 || textmode > 2) { strcpy(filename9 + txtys, filename7); filename9[strlen(filename9)] = ' '; txtys += 20; }
					print2st(filename9);
				}
				for (int i = 0; i < strlen(filename7); i++) {
					if (filename7[i] == 0x10) filename7[i] = 0x20;
				}
			}

			else {
				txtys = 0;
				CRect rct; GetClientRect(rct);
				if (hortxti == 0) {
					txtystmp = fsize*fspace / 143;
					if (textmode % 2 == 0) { printchr(asfaq, brebe, windx, windy, filename1, 0);  txtys += txtystmp; }
					if (textmode != -1) {
						printchr(asfaq, brebe, windx, windy, filename4, txtys);  txtys += txtystmp;
					}
					if (textmode == 0 || textmode > 2) { printchr(asfaq, brebe, windx, windy, filename6, txtys);  txtys += txtystmp; }
					if (textmode == 0 || textmode > 2) { printchr(asfaq, brebe, windx, windy, filename7, txtys);  txtys += txtystmp; }
				}
				else if (hortxti == 1) {
					txtystmp = fsize*fspace / 1000;
					if (textmode % 2 == 0) { printchr(asfaq + windx / 15 * txtys, brebe, windx, windy, filename1, 0); txtys += txtystmp; }
					if (textmode != -1) { printchr(asfaq + windx / 15 * txtys, brebe, windx, windy, filename4, 0); txtys += txtystmp; }
					if (textmode == 0 || textmode > 2) { printchr(asfaq + windx / 15 * txtys, brebe, windx, windy, filename6, 0); txtys += txtystmp; }
					if (textmode == 0 || textmode > 2) { printchr(asfaq + windx / 15 * txtys, brebe, windx, windy, filename7, 0); txtys += txtystmp; }
				}
				else {
					htit = true;
					memset(filename9, ' ', 400);
					if (textmode % 2 == 0) { strcpy(filename9 + txtys, filename1); filename9[strlen(filename9)] = ' '; txtys += 30; }
					if (textmode != -1) { strcpy(filename9 + txtys, filename4); filename9[strlen(filename9)] = ' '; txtys += 20; }
					if (textmode == 0 || textmode > 2) { strcpy(filename9 + txtys, filename6); filename9[strlen(filename9)] = ' '; txtys += 20; }
					if (textmode == 0 || textmode > 2) { strcpy(filename9 + txtys, filename7); filename9[strlen(filename9)] = ' '; txtys += 20; }
					print2st(filename9);
				}
			}
			glPopMatrix();
			glPushMatrix();
			glOrtho(-1 / j5jgj, 1 / j5jgj, -1 / j5jgj, 1 / j5jgj, -100 / j5jgj, 100 / j5jgj);
			glTranslatef(0.0f + x / 10, 0.0f + y / 10, -3);

			glRotatef(h45h7, 1, 0, 0);
			glRotatef(gasfw, 0, 1, 0);
			glRotatef(hg4h5, 0, 0, 1);
			mdtpnt2 = mdtpnt;
			int t = 0;
			for (int x = 0; x<16; x++)
				for (int i = 1; i < 8; i++) {
					erhwhw2[x][i] = cc(erhwh[x], i - 1);
					iyybts2[x][i] = cc(iyybt[x], i - 1);
					trityu2[x][i] = cc(trity[x], i - 1);
				}


			float fltx3625, fltx3375, fltx325, fltx375;
			float fltx270105, fltx280105;
			float cxm3625 = -3.3625*stretchx;
			float cxm3375 = -3.3375*stretchx;
			float cxm325 = -3.325*stretchx;
			float cxm375 = -3.375*stretchx;
			float cx025 = 0.025*stretchx;
			fltx3625 = cxm3625;
			fltx3375 = cxm3375;
			fltx325 = cxm325;
			fltx375 = cxm375;
			float cz76190476190476190476190476190477 = 0.76190476190476190476190476190477*stretchz;
			float cz85714285714285714285714285714286 = 0.85714285714285714285714285714286*stretchz;
			float cz0952380952380952380952380952381 = 0.0952380952380952380952380952381*stretchz;
			float cym1d0 = -1 * stretchy;
			float cym999 = -0.999*stretchy;
			float cym1d1 = (-1 - upbl)*stretchy;
			float cym1d3 = (-1 - dwbl)*stretchy;
			float czm749 = -0.75*stretchz + 0.01;
			float cz749 = 0.75*stretchz - 0.01;
			float cz75 = 0.75*stretchz;
			float czm75 = -0.75*stretchz;
			if (pb == false && clock() < (int)loop + ii + 25) {
				pb = true;

			}

			for (int x = 0; x < 268; x++) {
				bool bpshftmp;
				if (x - bpshf >= 0 && x - bpshf<268)bpshftmp = 1;
				else bpshftmp = 0;
				fltx3625 += cx025;
				fltx3375 += cx025;
				fltx325 += cx025;
				fltx375 += cx025;
				if (t == 1 || t == 3 || t == 6 || t == 8 || t == 10) {
					fltx270105 = cz76190476190476190476190476190477;
					fltx280105 = cz85714285714285714285714285714286;
					glBegin(GL_QUADS);
					for (int x2 = 0; x2 < 16; x2++) {
						fltx270105 -= cz0952380952380952380952380952381;
						fltx280105 -= cz0952380952380952380952380952381;
						gtmp = false;
						if (bpshftmp&&mdt[mdtpnt2][((x - bpshf - pitchint[x2])<0 || (x - bpshf - pitchint[x2]) >= 268) ? 0 : (x - bpshf - pitchint[x2])][x2] != 0) { glColor4f(erhwhw2[x2][mdt[mdtpnt2][((x - bpshf - pitchint[x2])<0 || (x - bpshf - pitchint[x2]) >= 268) ? 0 : (x - bpshf - pitchint[x2])][x2]], iyybts2[x2][mdt[mdtpnt2][((x - bpshf - pitchint[x2])<0 || (x - bpshf - pitchint[x2]) >= 268) ? 0 : (x - bpshf - pitchint[x2])][x2]], trityu2[x2][mdt[mdtpnt2][((x - bpshf - pitchint[x2])<0 || (x - bpshf - pitchint[x2]) >= 268) ? 0 : (x - bpshf - pitchint[x2])][x2]], 1); gtmp = true; }
						else glColor4f(0, 0, 0, 1);
						if (exbl == true || (x >= 60 && x < 188) || gtmp == true) {

							glVertex3f(fltx3625, cym999, fltx270105);
							glVertex3f(fltx3375, cym999, fltx270105);
							glVertex3f(fltx3375, cym999, fltx280105);
							glVertex3f(fltx3625, cym999, fltx280105);

						}
					}
					glEnd();
					glColor4f(0, 0, 0, 1);
					gtmp = false;
					if (x < ph || x >= pl) {
						for (int x2 = 15; x2 >-1; x2--)
							if (bpshftmp&&mdt[mdtpnt2][((x - bpshf - pitchint[x2])<0 || (x - bpshf - pitchint[x2]) >= 268) ? 0 : (x - bpshf - pitchint[x2])][x2] != 0) {
								glColor3f(erhwhw2[x2][mdt[mdtpnt2][((x - bpshf - pitchint[x2])<0 || (x - bpshf - pitchint[x2]) >= 268) ? 0 : (x - bpshf - pitchint[x2])][x2]], iyybts2[x2][mdt[mdtpnt2][((x - bpshf - pitchint[x2])<0 || (x - bpshf - pitchint[x2]) >= 268) ? 0 : (x - bpshf - pitchint[x2])][x2]], trityu2[x2][(bpshftmp ? mdt[mdtpnt2][((x - bpshf - pitchint[x2])<0 || (x - bpshf - pitchint[x2]) >= 268) ? 0 : (x - bpshf - pitchint[x2])][x2] : 0)]);
								gtmp = true;
								break;
							}
					}

					if (x >= 60 && x < 188) {
						glBegin(GL_QUAD_STRIP);
						glVertex3f(fltx3625, cym1d1, -cz75);
						glVertex3f(fltx3625, cym1d0, -cz75);
						glVertex3f(fltx3375, cym1d1, -cz75);
						glVertex3f(fltx3375, cym1d0, -cz75);
						glVertex3f(fltx3375, cym1d1, cz75);
						glVertex3f(fltx3375, cym1d0, cz75);
						glVertex3f(fltx3625, cym1d1, cz75);
						glVertex3f(fltx3625, cym1d0, cz75);
						glEnd();

					}
					else if (exbl == true || gtmp == true) {
						glBegin(GL_QUADS);
						glVertex3f(fltx3625, cym1d1, -cz75);
						glVertex3f(fltx3375, cym1d1, -cz75);
						glVertex3f(fltx3375, cym1d0, -cz75);
						glVertex3f(fltx3625, cym1d0, -cz75);
						glEnd();
						glBegin(GL_QUADS);
						glVertex3f(fltx3625, cym1d1, cz75);
						glVertex3f(fltx3375, cym1d1, cz75);
						glVertex3f(fltx3375, cym1d0, cz75);
						glVertex3f(fltx3625, cym1d0, cz75);
						glEnd();
					}

				}

				else if (t == 0 || t == 5) {
					fltx270105 = cz76190476190476190476190476190477;
					fltx280105 = cz85714285714285714285714285714286;
					for (int x2 = 0; x2 < 16; x2++) {
						fltx270105 -= cz0952380952380952380952380952381;
						fltx280105 -= cz0952380952380952380952380952381;
						if (x >= 60 && x <= 188) glColor4f(1, 1, 1, 1);
						else glColor4f(1, 1, 1, 0.1);
						gtmp = false;
						if (bpshftmp&&mdt[mdtpnt2][((x - bpshf - pitchint[x2])<0 || (x - bpshf - pitchint[x2]) >= 268) ? 0 : (x - bpshf - pitchint[x2])][x2] != 0) { glColor3f(erhwhw2[x2][mdt[mdtpnt2][((x - bpshf - pitchint[x2])<0 || (x - bpshf - pitchint[x2]) >= 268) ? 0 : (x - bpshf - pitchint[x2])][x2]], iyybts2[x2][mdt[mdtpnt2][((x - bpshf - pitchint[x2])<0 || (x - bpshf - pitchint[x2]) >= 268) ? 0 : (x - bpshf - pitchint[x2])][x2]], trityu2[x2][(bpshftmp ? mdt[mdtpnt2][((x - bpshf - pitchint[x2])<0 || (x - bpshf - pitchint[x2]) >= 268) ? 0 : (x - bpshf - pitchint[x2])][x2] : 0)]); gtmp = true; }
						if (exbl == true || (x >= 60 && x < 188) || gtmp == true) {
							glBegin(GL_QUADS);
							glVertex3f(fltx3625, cym1d3, fltx270105);
							glVertex3f(fltx325, cym1d3, fltx270105);
							glVertex3f(fltx325, cym1d3, fltx280105);
							glVertex3f(fltx3625, cym1d3, fltx280105);
							glEnd();
							glBegin(GL_QUADS);
							glVertex3f(fltx3625, cym1d0, fltx270105);
							glVertex3f(fltx325, cym1d0, fltx270105);
							glVertex3f(fltx325, cym1d0, fltx280105);
							glVertex3f(fltx3625, cym1d0, fltx280105);
							glEnd();
						}
					}
					if (x >= 60 && x <= 188) glColor4f(1, 1, 1, 1);
					else glColor4f(1, 1, 1, 0.1);
					gtmp = false;
					if (x < ph || x >= pl) {
						for (int x2 = 15; x2 >-1; x2--)
							if (bpshftmp&&mdt[mdtpnt2][((x - bpshf - pitchint[x2])<0 || (x - bpshf - pitchint[x2]) >= 268) ? 0 : (x - bpshf - pitchint[x2])][x2] != 0) {
								glColor3f(erhwhw2[x2][mdt[mdtpnt2][((x - bpshf - pitchint[x2])<0 || (x - bpshf - pitchint[x2]) >= 268) ? 0 : (x - bpshf - pitchint[x2])][x2]], iyybts2[x2][mdt[mdtpnt2][((x - bpshf - pitchint[x2])<0 || (x - bpshf - pitchint[x2]) >= 268) ? 0 : (x - bpshf - pitchint[x2])][x2]], trityu2[x2][(bpshftmp ? mdt[mdtpnt2][((x - bpshf - pitchint[x2])<0 || (x - bpshf - pitchint[x2]) >= 268) ? 0 : (x - bpshf - pitchint[x2])][x2] : 0)]);
								gtmp = true;
								break;
							}
					}
					if (x >= 60 && x < 188) {

						glBegin(GL_QUAD_STRIP);
						glVertex3f(fltx3625, cym1d3, czm749);
						glVertex3f(fltx3625, cym1d0, czm749);
						glVertex3f(fltx325, cym1d3, czm749);
						glVertex3f(fltx325, cym1d0, czm749);
						glVertex3f(fltx325, cym1d3, cz749);
						glVertex3f(fltx325, cym1d0, cz749);
						glVertex3f(fltx3625, cym1d3, cz749);
						glVertex3f(fltx3625, cym1d0, cz749);
						glEnd();


					}
					else if (exbl == true || gtmp == true) {
						glBegin(GL_QUADS);
						glVertex3f(fltx3625, cym1d3, cz749);
						glVertex3f(fltx325, cym1d3, cz749);
						glVertex3f(fltx325, cym1d0, cz749);
						glVertex3f(fltx3625, cym1d0, cz749);

						glVertex3f(fltx3625, cym1d3, czm749);
						glVertex3f(fltx325, cym1d3, czm749);
						glVertex3f(fltx325, cym1d0, czm749);
						glVertex3f(fltx3625, cym1d0, czm749);
						glEnd();
					}
				}
				else if (t == 2 || t == 7 || t == 9) {
					fltx270105 = cz76190476190476190476190476190477;
					fltx280105 = cz85714285714285714285714285714286;
					for (int x2 = 0; x2 < 16; x2++) {
						fltx270105 -= cz0952380952380952380952380952381;
						fltx280105 -= cz0952380952380952380952380952381;
						if (x >= 60 && x <= 188) glColor4f(1, 1, 1, 1);
						else glColor4f(1, 1, 1, 0.1);
						gtmp = false;
						if (bpshftmp&&mdt[mdtpnt2][((x - bpshf - pitchint[x2])<0 || (x - bpshf - pitchint[x2]) >= 268) ? 0 : (x - bpshf - pitchint[x2])][x2] != 0) {
							glColor3f(erhwhw2[x2][mdt[mdtpnt2][((x - bpshf - pitchint[x2])<0 || (x - bpshf - pitchint[x2]) >= 268) ? 0 : (x - bpshf - pitchint[x2])][x2]], iyybts2[x2][mdt[mdtpnt2][((x - bpshf - pitchint[x2])<0 || (x - bpshf - pitchint[x2]) >= 268) ? 0 : (x - bpshf - pitchint[x2])][x2]], trityu2[x2][(bpshftmp ? mdt[mdtpnt2][((x - bpshf - pitchint[x2])<0 || (x - bpshf - pitchint[x2]) >= 268) ? 0 : (x - bpshf - pitchint[x2])][x2] : 0)]); gtmp = true;
						}
						if (exbl == true || (x >= 60 && x < 188) || gtmp == true) {
							glBegin(GL_QUADS);
							glVertex3f(fltx375, cym1d3, fltx270105);
							glVertex3f(fltx325, cym1d3, fltx270105);
							glVertex3f(fltx325, cym1d3, fltx280105);
							glVertex3f(fltx375, cym1d3, fltx280105);

							glVertex3f(fltx375, cym1d0, fltx270105);
							glVertex3f(fltx325, cym1d0, fltx270105);
							glVertex3f(fltx325, cym1d0, fltx280105);
							glVertex3f(fltx375, cym1d0, fltx280105);
							glEnd();
						}
					}
					if (x >= 60 && x <= 188) glColor4f(1, 1, 1, 1);
					else glColor4f(1, 1, 1, 0.1);
					gtmp = false;
					if (x < ph || x >= pl) {
						for (int x2 = 15; x2 >-1; x2--)
							if (bpshftmp&&mdt[mdtpnt2][((x - bpshf - pitchint[x2])<0 || (x - bpshf - pitchint[x2]) >= 268) ? 0 : (x - bpshf - pitchint[x2])][x2] != 0) {
								glColor3f(erhwhw2[x2][mdt[mdtpnt2][((x - bpshf - pitchint[x2])<0 || (x - bpshf - pitchint[x2]) >= 268) ? 0 : (x - bpshf - pitchint[x2])][x2]], iyybts2[x2][mdt[mdtpnt2][((x - bpshf - pitchint[x2])<0 || (x - bpshf - pitchint[x2]) >= 268) ? 0 : (x - bpshf - pitchint[x2])][x2]], trityu2[x2][(bpshftmp ? mdt[mdtpnt2][((x - bpshf - pitchint[x2])<0 || (x - bpshf - pitchint[x2]) >= 268) ? 0 : (x - bpshf - pitchint[x2])][x2] : 0)]);
								gtmp = true;
								break;
							}
					}
					if (x >= 60 && x < 188) {

						glBegin(GL_QUAD_STRIP);
						glVertex3f(fltx375, cym1d3, czm749);
						glVertex3f(fltx375, cym1d0, czm749);
						glVertex3f(fltx325, cym1d3, czm749);
						glVertex3f(fltx325, cym1d0, czm749);
						glVertex3f(fltx325, cym1d3, cz749);
						glVertex3f(fltx325, cym1d0, cz749);
						glVertex3f(fltx375, cym1d3, cz749);
						glVertex3f(fltx375, cym1d0, cz749);
						glEnd();


					}
					if (exbl == true || gtmp == true) {
						glBegin(GL_QUADS);
						glVertex3f(fltx375, cym1d3, cz749);
						glVertex3f(fltx325, cym1d3, cz749);
						glVertex3f(fltx325, cym1d0, cz749);
						glVertex3f(fltx375, cym1d0, cz749);

						glVertex3f(fltx375, cym1d3, czm749);
						glVertex3f(fltx325, cym1d3, czm749);
						glVertex3f(fltx325, cym1d0, czm749);
						glVertex3f(fltx375, cym1d0, czm749);
						glEnd();

					}
				}
				else if (t == 4 || t == 11) {
					fltx270105 = cz76190476190476190476190476190477;
					fltx280105 = cz85714285714285714285714285714286;
					for (int x2 = 0; x2 < 16; x2++) {
						fltx270105 -= cz0952380952380952380952380952381;
						fltx280105 -= cz0952380952380952380952380952381;
						if (x >= 60 && x <= 188) glColor4f(1, 1, 1, 1);
						else glColor4f(1, 1, 1, 0.1);
						gtmp = false;
						if (bpshftmp&&mdt[mdtpnt2][((x - bpshf - pitchint[x2])<0 || (x - bpshf - pitchint[x2]) >= 268) ? 0 : (x - bpshf - pitchint[x2])][x2] != 0) {
							glColor3f(erhwhw2[x2][mdt[mdtpnt2][((x - bpshf - pitchint[x2])<0 || (x - bpshf - pitchint[x2]) >= 268) ? 0 : (x - bpshf - pitchint[x2])][x2]], iyybts2[x2][mdt[mdtpnt2][((x - bpshf - pitchint[x2])<0 || (x - bpshf - pitchint[x2]) >= 268) ? 0 : (x - bpshf - pitchint[x2])][x2]], trityu2[x2][(bpshftmp ? mdt[mdtpnt2][((x - bpshf - pitchint[x2])<0 || (x - bpshf - pitchint[x2]) >= 268) ? 0 : (x - bpshf - pitchint[x2])][x2] : 0)]); gtmp = true;
						}
						if (exbl == true || (x >= 60 && x < 188) || gtmp == true) {
							glBegin(GL_QUADS);
							glVertex3f(fltx375, cym1d3, fltx270105);
							glVertex3f(fltx3375, cym1d3, fltx270105);
							glVertex3f(fltx3375, cym1d3, fltx280105);
							glVertex3f(fltx375, cym1d3, fltx280105);

							glVertex3f(fltx375, cym1d0, fltx270105);
							glVertex3f(fltx3375, cym1d0, fltx270105);
							glVertex3f(fltx3375, cym1d0, fltx280105);
							glVertex3f(fltx375, cym1d0, fltx280105);
							glEnd();
						}
					}
					if (x >= 60 && x <= 188) glColor4f(1, 1, 1, 1);
					else glColor4f(1, 1, 1, 0.1);
					gtmp = false;
					if (x < ph || x >= pl) {
						for (int x2 = 15; x2 >-1; x2--)
							if (bpshftmp&&mdt[mdtpnt2][((x - bpshf - pitchint[x2])<0 || (x - bpshf - pitchint[x2]) >= 268) ? 0 : (x - bpshf - pitchint[x2])][x2] != 0) {
								glColor3f(erhwhw2[x2][mdt[mdtpnt2][((x - bpshf - pitchint[x2])<0 || (x - bpshf - pitchint[x2]) >= 268) ? 0 : (x - bpshf - pitchint[x2])][x2]], iyybts2[x2][mdt[mdtpnt2][((x - bpshf - pitchint[x2])<0 || (x - bpshf - pitchint[x2]) >= 268) ? 0 : (x - bpshf - pitchint[x2])][x2]], trityu2[x2][(bpshftmp ? mdt[mdtpnt2][((x - bpshf - pitchint[x2])<0 || (x - bpshf - pitchint[x2]) >= 268) ? 0 : (x - bpshf - pitchint[x2])][x2] : 0)]);
								gtmp = true;
								break;
							}
					}
					if (x >= 60 && x < 188) {


						glBegin(GL_QUAD_STRIP);
						glVertex3f(fltx375, cym1d3, czm749);
						glVertex3f(fltx375, cym1d0, czm749);
						glVertex3f(fltx3375, cym1d3, czm749);
						glVertex3f(fltx3375, cym1d0, czm749);
						glVertex3f(fltx3375, cym1d3, cz749);
						glVertex3f(fltx3375, cym1d0, cz749);
						glVertex3f(fltx375, cym1d3, cz749);
						glVertex3f(fltx375, cym1d0, cz749);
						glEnd();


					}
					else if (exbl == true || gtmp == true) {
						glBegin(GL_QUADS);
						glVertex3f(fltx375, cym1d3, cz749);
						glVertex3f(fltx3375, cym1d3, cz749);
						glVertex3f(fltx3375, cym1d0, cz749);
						glVertex3f(fltx375, cym1d0, cz749);

						glVertex3f(fltx375, cym1d3, czm749);
						glVertex3f(fltx3375, cym1d3, czm749);
						glVertex3f(fltx3375, cym1d0, czm749);
						glVertex3f(fltx375, cym1d0, czm749);
						glEnd();

					}
				}
				t++;
				if (t == 12)t = 0;
			}
			/*
			fltx270105 = cz76190476190476190476190476190477;
			fltx280105 = cz85714285714285714285714285714286;
			for (int x2 = 0; x2 < 16; x2++) {
			fltx270105 -= cz0952380952380952380952380952381;
			fltx280105 -= cz0952380952380952380952380952381;
			*/
			if (pb == false && clock() < (int)loop + ii + 25) {
				pb = true;

				//alSourcef(isource, AL_SEC_OFFSET, 0.025 + (float)playoffset / 20);
			}

			float fltx134540, fltx133540, fltz7510;
			float mdtpnt31038636, mdtpnt310340909;
			float stretchy1038636363636364 = 1.038636363636364f*stretchy, stretchy1034090909090909 = 1.034090909090909f*stretchy;
			float cz0d85 = 0.85*stretchz;
			float cz0d1 = 0.1*stretchz;
			char mdtvtmp;
			glBegin_common2();
			double bpshf2[16];
			for (int x = 0; x<16; x++) bpshf2[x] = 0.025*((double)pitchint[x])*stretchx;
			if (mdtpnt2 != 0) {
				fltx134540 = (-3.365 + 0.025*(pl + bpshf))*stretchx;
				fltx133540 = (-3.34 + 0.025*(pl + bpshf))*stretchx;
				for (int x = pl; x < ph; x++) {
					fltx134540 += cx025;
					fltx133540 += cx025;
					fltz7510 = cz0d85;
					for (int z = 0; z < 16; z++) {
						while (pitchint[z] < -257) z++;
						fltz7510 -= cz0d1;

						//glBegin2( 0);
						glBegin2(1);
						glBegin2(2);
						glBegin2(3);
						glBegin2(4);
						glBegin2(5);
						glBegin2(6);
						glBegin2(7);

						for (int y = mdtpnt2; y < 500; y++) {

							if (mdt[y][x][z]) {
								mdtpnt3 = (float)(y - mdtpnt2) / 220.f*stretchy;
								mdtpnt31038636 = (mdtpnt3 - stretchy1038636363636364);
								mdtpnt310340909 = (mdtpnt3 - stretchy1034090909090909);
								if (y == mdtpnt2 || mdt[y][x][z] != mdt[y - 1][x][z]) {
									mdtvtmp = mdt[y][x][z];

									glVertex3f2(fltx134540 + bpshf2[z], mdtpnt31038636, fltz7510, mdtvtmp);
									glVertex3f2(fltx133540 + bpshf2[z], mdtpnt31038636, fltz7510, mdtvtmp);
								}
								if (mdt[y][x][z] != mdt[(y == 499) ? 0 : (y + 1)][x][z]) {

									glVertex3f2(fltx133540 + bpshf2[z], mdtpnt310340909, fltz7510, mdtvtmp);
									glVertex3f2(fltx134540 + bpshf2[z], mdtpnt310340909, fltz7510, mdtvtmp);

								}
							}
						}

						for (int y = 0; y < mdtpnt2; y++) {
							if (mdt[y][x][z]) {
								mdtpnt3 = (float)(y - mdtpnt2 + 500) / 220.f * stretchy;
								mdtpnt31038636 = (mdtpnt3 - stretchy1038636363636364);
								mdtpnt310340909 = (mdtpnt3 - stretchy1034090909090909);
								if (mdt[y][x][z] != mdt[(y == 0) ? 499 : (y - 1)][x][z]) {
									mdtvtmp = mdt[y][x][z];

									glVertex3f2(fltx134540 + bpshf2[z], mdtpnt31038636, fltz7510, mdtvtmp);
									glVertex3f2(fltx133540 + bpshf2[z], mdtpnt31038636, fltz7510, mdtvtmp);
								}
								if (y == mdtpnt2 - 1 || mdt[y][x][z] != mdt[y + 1][x][z]) {

									glVertex3f2(fltx133540 + bpshf2[z], mdtpnt310340909, fltz7510, mdtvtmp);
									glVertex3f2(fltx134540 + bpshf2[z], mdtpnt310340909, fltz7510, mdtvtmp);

								}



							}
						}
						glBegin3(1);
						glColor4f(erhwhw2[z][1], iyybts2[z][1], trityu2[z][1], sadae[z]);
						glEnd2(1); glBegin3(2);
						glColor4f(erhwhw2[z][2], iyybts2[z][2], trityu2[z][2], sadae[z]);
						glEnd2(2); glBegin3(3);
						glColor4f(erhwhw2[z][3], iyybts2[z][3], trityu2[z][3], sadae[z]);
						glEnd2(3); glBegin3(4);
						glColor4f(erhwhw2[z][4], iyybts2[z][4], trityu2[z][4], sadae[z]);
						glEnd2(4); glBegin3(5);
						glColor4f(erhwhw2[z][5], iyybts2[z][5], trityu2[z][5], sadae[z]);
						glEnd2(5); glBegin3(6);
						glColor4f(erhwhw2[z][6], iyybts2[z][6], trityu2[z][6], sadae[z]);
						glEnd2(6); glBegin3(7);
						glColor4f(erhwhw2[z][7], iyybts2[z][7], trityu2[z][7], sadae[z]);
						glEnd2(7);

					}
				}
			}
			else {

				fltx134540 = (-3.365 + 0.025*(pl + bpshf))*stretchx;
				fltx133540 = (-3.34 + 0.025*(pl + bpshf))*stretchx;
				for (int x = pl; x < ph; x++) {
					fltx134540 += cx025;
					fltx133540 += cx025;
					fltz7510 = cz0d85;
					for (int z = 0; z < 16; z++) {
						while (pitchint[z] < -257) z++;
						//glBegin2( 0);
						glBegin2(1);
						glBegin2(2);
						glBegin2(3);
						glBegin2(4);
						glBegin2(5);
						glBegin2(6);
						glBegin2(7);
						fltz7510 -= cz0d1;
						for (int y = 0; y < 500; y++) {

							if (mdt[y][x][z]) {
								mdtpnt3 = (float)(y - mdtpnt2) / 220.f * stretchy;
								mdtpnt31038636 = (mdtpnt3 - stretchy1038636363636364);
								mdtpnt310340909 = (mdtpnt3 - stretchy1034090909090909);
								if (y == 0 || mdt[y][x][z] != mdt[y - 1][x][z]) {
									mdtvtmp = mdt[y][x][z];
									glVertex3f2(fltx134540 + bpshf2[z], mdtpnt31038636, fltz7510, mdtvtmp);
									glVertex3f2(fltx133540 + bpshf2[z], mdtpnt31038636, fltz7510, mdtvtmp);
								}
								if (y == 499 || mdt[y][x][z] != mdt[y + 1][x][z]) {
									glVertex3f2(fltx133540 + bpshf2[z], mdtpnt310340909, fltz7510, mdtvtmp);
									glVertex3f2(fltx134540 + bpshf2[z], mdtpnt310340909, fltz7510, mdtvtmp);

								}
							}
						}
						glBegin3(1);
						glColor4f(erhwhw2[z][1], iyybts2[z][1], trityu2[z][1], sadae[z]);
						glEnd2(1); glBegin3(2);
						glColor4f(erhwhw2[z][2], iyybts2[z][2], trityu2[z][2], sadae[z]);
						glEnd2(2); glBegin3(3);
						glColor4f(erhwhw2[z][3], iyybts2[z][3], trityu2[z][3], sadae[z]);
						glEnd2(3); glBegin3(4);
						glColor4f(erhwhw2[z][4], iyybts2[z][4], trityu2[z][4], sadae[z]);
						glEnd2(4); glBegin3(5);
						glColor4f(erhwhw2[z][5], iyybts2[z][5], trityu2[z][5], sadae[z]);
						glEnd2(5); glBegin3(6);
						glColor4f(erhwhw2[z][6], iyybts2[z][6], trityu2[z][6], sadae[z]);
						glEnd2(6); glBegin3(7);
						glColor4f(erhwhw2[z][7], iyybts2[z][7], trityu2[z][7], sadae[z]);
						glEnd2(7);

					}
				}

			}
			glEnd_common2();

			if (pb == false && clock() < (int)loop + ii + 25) {
				pb = true;

			}
			glColor4f(tr, tg, tb, ta);

			glPopMatrix();
			glFinish();
			SwapBuffers(wglGetCurrentDC());
		EX:
			;

		}
		else if (mode == 14) {
			if (__argc > 2) {

				char playl[1000];
				strcpy(playl, "\0");
				for (int i = 0; i < __argc - 2; i++) {
					strcat(playl, "\"");
					strcat(playl, __argv[i + 2]);
					strcat(playl, "\" ");

				}
				playl[strlen(playl) - 1] = 0;


				char st[1000];
				char st2[1000];
				GetModuleFileNameA(NULL, st, 1000);
				sprintf(st2, "\"%s\"", st);
				ShellExecuteA(NULL, "OPEN", st2, playl, NULL, SW_SHOW);
				if (!midiout) {
					//	MessageBox("B", "B", MB_OK);
					//cmidi.Pause();
					TerminateThread(thread1, 0);
					CloseHandle(thread1);
					Sleep(100);
					pauseall();
					midiOutClose(playmidiout);

					char a[1000];
					fclose(wj6);
					fclose(wj7);
					
				}
				exit2(1);


			}
			if (htit == true && hortxti != 2) {
				htit = false; print2st("\0");
			}
			//("?");
			if (tpi == 0) {
				glClearColor(0, 0, 0, 0);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE);
				glEnable(GL_BLEND);

				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				glPushMatrix();
				glTranslatef(0.0f, 0.0f, -3);
				glColor4f(1, 1, 1, 1);
			}
			else {
				glClearColor(1, 1, 1, 1);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE);
				glEnable(GL_BLEND);

				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				glPushMatrix();
				glTranslatef(0.0f, 0.0f, -3);
				glColor4f(1, 1, 1, 1);
				glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D, tex);
				glBegin(GL_QUADS);
				glTexCoord3f(0.0f, 0.0f, -1.0f); glVertex3f(-3.33f, -1.3f, -10);
				glTexCoord3f(1.0f, 0.0f, -1.0f); glVertex3f(3.33f, -1.3f, -10);
				glTexCoord3f(1.0f, 1.0f, -1.0f); glVertex3f(3.33f, 1.3f, -10);
				glTexCoord3f(0.0f, 1.0f, -1.0f); glVertex3f(-3.33f, 1.3f, -10);
				glEnd();
				glDisable(GL_TEXTURE_2D);
			}
			//trand = true;
			if (trand == true) {
				tr = (float)rand() / 32768;
				tg = (float)rand() / 32768;
				tb = (float)rand() / 32768;
			}
			glColor4f(tr, tg, tb, ta);
			glLineWidth(2);
			//("%d %d\n", rct.Width(), rct.Height());
			txtys = 0;
			int txtystmp;
			if (hortxti == 0) {
				txtystmp = fsize*fspace / 143;
				if (textmode % 2 == 0) { printchr(asfaq, brebe, windx, windy, ofilename2, 0);  txtys += txtystmp; }
				if (textmode != -1) { printchr(asfaq, brebe, windx, windy, filename4, txtys);  txtys += txtystmp; }
				if (textmode == 0 || textmode > 2) { printchr(asfaq, brebe, windx, windy, filename6, txtys);  txtys += txtystmp; }
				//if (textmode == 0) { printchr(asfaq, brebe, windx, windy, filename1, txtys);  txtys += fsize*fspace/143; }
				if (textmode == 0 || textmode > 2) { printchr(asfaq, brebe, windx, windy, filename7, txtys);  txtys += txtystmp; }
			}
			else if (hortxti == 1) {
				txtystmp = fsize*fspace / 1000;
				if (textmode % 2 == 0) { printchr(asfaq + windx / 15 * txtys, brebe, windx, windy, ofilename2, 0); txtys += txtystmp; }
				//	if (lt == 0)	fscanf(wj6, "%s", filename4);
				if (textmode != -1) { printchr(asfaq + windx / 15 * txtys, brebe, windx, windy, filename4, 0); txtys += txtystmp; }
				//	if (lt == 0)	fscanf(wj6, "%s", filename6);
				if (textmode == 0 || textmode > 2) { printchr(asfaq + windx / 15 * txtys, brebe, windx, windy, filename6, 0); txtys += txtystmp; }
				//	if (lt == 0)	fscanf(wj6, "%s", midstr);
				//if (textmode == 0) { printchr(asfaq + windx / 15 * txtys, brebe, windx, windy, filename1, 0); txtys += 3; }
				//	if (lt == 0)	fscanf(wj6, "%s", filename7);
				if (textmode == 0 || textmode > 2) { printchr(asfaq + windx / 15 * txtys, brebe, windx, windy, filename7, 0); txtys += txtystmp; }
			}
			else {
				htit = true;
				memset(filename9, ' ', 400);
				if (textmode % 2 == 0) { strcpy(filename9 + txtys, ofilename2); filename9[strlen(filename9)] = ' '; txtys += 30; }
				//		if (lt == 0)	fscanf(wj6, "%s", filename4);
				if (textmode != -1) { strcpy(filename9 + txtys, filename4); filename9[strlen(filename9)] = ' '; txtys += 20; }
				//		if (lt == 0)	fscanf(wj6, "%s", filename6);
				if (textmode == 0 || textmode > 2) { strcpy(filename9 + txtys, filename6); filename9[strlen(filename9)] = ' '; txtys += 20; }
				//		if (lt == 0)	fscanf(wj6, "%s", midstr);
				//if (textmode == 0) { printchr(asfaq + windx / 15 * txtys, brebe, windx, windy, 1, 0); txtys += 3; }
				//		if (lt == 0)	fscanf(wj6, "%s", filename7);
				if (textmode == 0 || textmode > 2) { strcpy(filename9 + txtys, filename7); filename9[strlen(filename9)] = ' '; txtys += 20; }
				print2st(filename9);
			}

			glPopMatrix();
			glFinish();
			glPushMatrix();
			glOrtho(-1 / j5jgj, 1 / j5jgj, -1 / j5jgj, 1 / j5jgj, -100 / j5jgj, 100 / j5jgj);
			glTranslatef(0.0f + x / 10, 0.0f + y / 10, -3);

			glRotatef(h45h7, 1, 0, 0);
			glRotatef(gasfw, 0, 1, 0);
			glRotatef(hg4h5, 0, 0, 1);
			mdtpnt2 = mdtpnt + 10;
			if (mdtpnt2 == 500) mdtpnt2 = 0;
			int t = 0;

			float fltx3625, fltx3375, fltx325, fltx375;
			float fltx270105, fltx280105;
			float cxm3625 = -3.3625*stretchx;
			float cxm3375 = -3.3375*stretchx;
			float cxm325 = -3.325*stretchx;
			float cxm375 = -3.375*stretchx;
			float cx025 = 0.025*stretchx;
			fltx3625 = cxm3625;
			fltx3375 = cxm3375;
			fltx325 = cxm325;
			fltx375 = cxm375;
			float cz76190476190476190476190476190477 = 0.76190476190476190476190476190477*stretchz;
			float cz85714285714285714285714285714286 = 0.85714285714285714285714285714286*stretchz;
			float cz0952380952380952380952380952381 = 0.0952380952380952380952380952381*stretchz;
			float cym1d0 = -1 * stretchy;
			float cym999 = -0.999*stretchy;
			float cym1d1 = (-1 - upbl)*stretchy;
			float cym1d3 = (-1 - dwbl)*stretchy;
			float czm749 = -0.75*stretchz + 0.01;
			float cz749 = 0.75*stretchz - 0.01;
			float cz75 = 0.75*stretchz;
			float czm75 = -0.75*stretchz;
			for (int x = 0; x < 268; x++) {
				fltx3625 += cx025;
				fltx3375 += cx025;
				fltx325 += cx025;
				fltx375 += cx025;
				if (t == 1 || t == 3 || t == 6 || t == 8 || t == 10) {
					fltx270105 = cz76190476190476190476190476190477;
					fltx280105 = cz85714285714285714285714285714286;
					for (int x2 = 0; x2 < 16; x2++) {
						fltx270105 -= cz0952380952380952380952380952381;
						fltx280105 -= cz0952380952380952380952380952381;
						gtmp = false;
						if (mdt[mdtpnt2][x][x2] != 0) { glColor4f(erhwhw2[x2][mdt[mdtpnt2][x][x2]], iyybts2[x2][mdt[mdtpnt2][x][x2]], trityu2[x2][mdt[mdtpnt2][x][x2]], 1); gtmp = true; }
						else glColor4f(0, 0, 0, 1);
						if (exbl == true || (x >= 60 && x < 188) || gtmp == true) {
							glBegin(GL_QUADS);
							glVertex3f(fltx3625, cym999, fltx270105);
							glVertex3f(fltx3375, cym999, fltx270105);
							glVertex3f(fltx3375, cym999, fltx280105);
							glVertex3f(fltx3625, cym999, fltx280105);
							glEnd();
						}
					}
					glColor4f(0, 0, 0, 1);
					gtmp = false;
					if (x < 268 || x >= 0) {
						for (int x2 = 15; x2 >-1; x2--)
							if (mdt[mdtpnt2][x][x2] != 0) {
								glColor3f(erhwhw2[x2][mdt[mdtpnt2][x][x2]], iyybts2[x2][mdt[mdtpnt2][x][x2]], trityu2[x2][mdt[mdtpnt2][x][x2]]);
								gtmp = true;
								break;
							}
					}

					if (x >= 60 && x < 188) {
						glBegin(GL_QUAD_STRIP);
						glVertex3f(fltx3625, cym1d1, -cz75);
						glVertex3f(fltx3625, cym1d0, -cz75);
						glVertex3f(fltx3375, cym1d1, -cz75);
						glVertex3f(fltx3375, cym1d0, -cz75);
						glVertex3f(fltx3375, cym1d1, cz75);
						glVertex3f(fltx3375, cym1d0, cz75);
						glVertex3f(fltx3625, cym1d1, cz75);
						glVertex3f(fltx3625, cym1d0, cz75);
						glEnd();

					}
					else if (exbl == true || gtmp == true) {
						glBegin(GL_QUADS);
						glVertex3f(fltx3625, cym1d1, -cz75);
						glVertex3f(fltx3375, cym1d1, -cz75);
						glVertex3f(fltx3375, cym1d0, -cz75);
						glVertex3f(fltx3625, cym1d0, -cz75);
						glEnd();
						glBegin(GL_QUADS);
						glVertex3f(fltx3625, cym1d1, cz75);
						glVertex3f(fltx3375, cym1d1, cz75);
						glVertex3f(fltx3375, cym1d0, cz75);
						glVertex3f(fltx3625, cym1d0, cz75);
						glEnd();
					}

				}

				else if (t == 0 || t == 5) {
					fltx270105 = cz76190476190476190476190476190477;
					fltx280105 = cz85714285714285714285714285714286;
					for (int x2 = 0; x2 < 16; x2++) {
						fltx270105 -= cz0952380952380952380952380952381;
						fltx280105 -= cz0952380952380952380952380952381;
						if (x >= 60 && x <= 188) glColor4f(1, 1, 1, 1);
						else glColor4f(1, 1, 1, 0.1);
						gtmp = false;
						if (mdt[mdtpnt2][x][x2] != 0) { glColor3f(erhwhw2[x2][mdt[mdtpnt2][x][x2]], iyybts2[x2][mdt[mdtpnt2][x][x2]], trityu2[x2][mdt[mdtpnt2][x][x2]]); gtmp = true; }
						if (exbl == true || (x >= 60 && x < 188) || gtmp == true) {
							glBegin(GL_QUADS);
							glVertex3f(fltx3625, cym1d3, fltx270105);
							glVertex3f(fltx325, cym1d3, fltx270105);
							glVertex3f(fltx325, cym1d3, fltx280105);
							glVertex3f(fltx3625, cym1d3, fltx280105);
							glEnd();
							glBegin(GL_QUADS);
							glVertex3f(fltx3625, cym1d0, fltx270105);
							glVertex3f(fltx325, cym1d0, fltx270105);
							glVertex3f(fltx325, cym1d0, fltx280105);
							glVertex3f(fltx3625, cym1d0, fltx280105);
							glEnd();
						}
					}
					if (x >= 60 && x <= 188) glColor4f(1, 1, 1, 1);
					else glColor4f(1, 1, 1, 0.1);
					gtmp = false;
					if (x < 268 || x >= 0) {
						for (int x2 = 15; x2 >-1; x2--)
							if (mdt[mdtpnt2][x][x2] != 0) {
								glColor3f(erhwhw2[x2][mdt[mdtpnt2][x][x2]], iyybts2[x2][mdt[mdtpnt2][x][x2]], trityu2[x2][mdt[mdtpnt2][x][x2]]);
								gtmp = true;
								break;
							}
					}
					if (x >= 60 && x < 188) {

						glBegin(GL_QUAD_STRIP);
						glVertex3f(fltx3625, cym1d3, czm749);
						glVertex3f(fltx3625, cym1d0, czm749);
						glVertex3f(fltx325, cym1d3, czm749);
						glVertex3f(fltx325, cym1d0, czm749);
						glVertex3f(fltx325, cym1d3, cz749);
						glVertex3f(fltx325, cym1d0, cz749);
						glVertex3f(fltx3625, cym1d3, cz749);
						glVertex3f(fltx3625, cym1d0, cz749);
						glEnd();


					}
					else if (exbl == true || gtmp == true) {
						glBegin(GL_QUADS);
						glVertex3f(fltx3625, cym1d3, cz749);
						glVertex3f(fltx325, cym1d3, cz749);
						glVertex3f(fltx325, cym1d0, cz749);
						glVertex3f(fltx3625, cym1d0, cz749);
						glEnd();
						glBegin(GL_QUADS);
						glVertex3f(fltx3625, cym1d3, czm749);
						glVertex3f(fltx325, cym1d3, czm749);
						glVertex3f(fltx325, cym1d0, czm749);
						glVertex3f(fltx3625, cym1d0, czm749);
						glEnd();
					}
				}
				else if (t == 2 || t == 7 || t == 9) {
					fltx270105 = cz76190476190476190476190476190477;
					fltx280105 = cz85714285714285714285714285714286;
					for (int x2 = 0; x2 < 16; x2++) {
						fltx270105 -= cz0952380952380952380952380952381;
						fltx280105 -= cz0952380952380952380952380952381;
						if (x >= 60 && x <= 188) glColor4f(1, 1, 1, 1);
						else glColor4f(1, 1, 1, 0.1);
						gtmp = false;
						if (mdt[mdtpnt2][x][x2] != 0) {
							glColor3f(erhwhw2[x2][mdt[mdtpnt2][x][x2]], iyybts2[x2][mdt[mdtpnt2][x][x2]], trityu2[x2][mdt[mdtpnt2][x][x2]]); gtmp = true;
						}
						if (exbl == true || (x >= 60 && x < 188) || gtmp == true) {
							glBegin(GL_QUADS);
							glVertex3f(fltx375, cym1d3, fltx270105);
							glVertex3f(fltx325, cym1d3, fltx270105);
							glVertex3f(fltx325, cym1d3, fltx280105);
							glVertex3f(fltx375, cym1d3, fltx280105);
							glEnd();
							glBegin(GL_QUADS);
							glVertex3f(fltx375, cym1d0, fltx270105);
							glVertex3f(fltx325, cym1d0, fltx270105);
							glVertex3f(fltx325, cym1d0, fltx280105);
							glVertex3f(fltx375, cym1d0, fltx280105);
							glEnd();
						}
					}
					if (x >= 60 && x <= 188) glColor4f(1, 1, 1, 1);
					else glColor4f(1, 1, 1, 0.1);
					gtmp = false;
					if (x < 268 || x >= 0) {
						for (int x2 = 15; x2 >-1; x2--)
							if (mdt[mdtpnt2][x][x2] != 0) {
								glColor3f(erhwhw2[x2][mdt[mdtpnt2][x][x2]], iyybts2[x2][mdt[mdtpnt2][x][x2]], trityu2[x2][mdt[mdtpnt2][x][x2]]);
								gtmp = true;
								break;
							}
					}
					if (x >= 60 && x < 188) {

						glBegin(GL_QUAD_STRIP);
						glVertex3f(fltx375, cym1d3, czm749);
						glVertex3f(fltx375, cym1d0, czm749);
						glVertex3f(fltx325, cym1d3, czm749);
						glVertex3f(fltx325, cym1d0, czm749);
						glVertex3f(fltx325, cym1d3, cz749);
						glVertex3f(fltx325, cym1d0, cz749);
						glVertex3f(fltx375, cym1d3, cz749);
						glVertex3f(fltx375, cym1d0, cz749);
						glEnd();


					}
					if (exbl == true || gtmp == true) {
						glBegin(GL_QUADS);
						glVertex3f(fltx375, cym1d3, cz749);
						glVertex3f(fltx325, cym1d3, cz749);
						glVertex3f(fltx325, cym1d0, cz749);
						glVertex3f(fltx375, cym1d0, cz749);
						glEnd();
						glBegin(GL_QUADS);
						glVertex3f(fltx375, cym1d3, czm749);
						glVertex3f(fltx325, cym1d3, czm749);
						glVertex3f(fltx325, cym1d0, czm749);
						glVertex3f(fltx375, cym1d0, czm749);
						glEnd();

					}
				}
				else if (t == 4 || t == 11) {
					fltx270105 = cz76190476190476190476190476190477;
					fltx280105 = cz85714285714285714285714285714286;
					for (int x2 = 0; x2 < 16; x2++) {
						fltx270105 -= cz0952380952380952380952380952381;
						fltx280105 -= cz0952380952380952380952380952381;
						if (x >= 60 && x <= 188) glColor4f(1, 1, 1, 1);
						else glColor4f(1, 1, 1, 0.1);
						gtmp = false;
						if (mdt[mdtpnt2][x][x2] != 0) {
							glColor3f(erhwhw2[x2][mdt[mdtpnt2][x][x2]], iyybts2[x2][mdt[mdtpnt2][x][x2]], trityu2[x2][mdt[mdtpnt2][x][x2]]); gtmp = true;
						}
						if (exbl == true || (x >= 60 && x < 188) || gtmp == true) {
							glBegin(GL_QUADS);
							glVertex3f(fltx375, cym1d3, fltx270105);
							glVertex3f(fltx3375, cym1d3, fltx270105);
							glVertex3f(fltx3375, cym1d3, fltx280105);
							glVertex3f(fltx375, cym1d3, fltx280105);
							glEnd();
							glBegin(GL_QUADS);
							glVertex3f(fltx375, cym1d0, fltx270105);
							glVertex3f(fltx3375, cym1d0, fltx270105);
							glVertex3f(fltx3375, cym1d0, fltx280105);
							glVertex3f(fltx375, cym1d0, fltx280105);
							glEnd();
						}
					}
					if (x >= 60 && x <= 188) glColor4f(1, 1, 1, 1);
					else glColor4f(1, 1, 1, 0.1);
					gtmp = false;
					if (x < 268 || x >= 0) {
						for (int x2 = 15; x2 >-1; x2--)
							if (mdt[mdtpnt2][x][x2] != 0) {
								glColor3f(erhwhw2[x2][mdt[mdtpnt2][x][x2]], iyybts2[x2][mdt[mdtpnt2][x][x2]], trityu2[x2][mdt[mdtpnt2][x][x2]]);
								gtmp = true;
								break;
							}
					}
					if (x >= 60 && x < 188) {


						glBegin(GL_QUAD_STRIP);
						glVertex3f(fltx375, cym1d3, czm749);
						glVertex3f(fltx375, cym1d0, czm749);
						glVertex3f(fltx3375, cym1d3, czm749);
						glVertex3f(fltx3375, cym1d0, czm749);
						glVertex3f(fltx3375, cym1d3, cz749);
						glVertex3f(fltx3375, cym1d0, cz749);
						glVertex3f(fltx375, cym1d3, cz749);
						glVertex3f(fltx375, cym1d0, cz749);
						glEnd();


					}
					else if (exbl == true || gtmp == true) {
						glBegin(GL_QUADS);
						glVertex3f(fltx375, cym1d3, cz749);
						glVertex3f(fltx3375, cym1d3, cz749);
						glVertex3f(fltx3375, cym1d0, cz749);
						glVertex3f(fltx375, cym1d0, cz749);
						glEnd();
						glBegin(GL_QUADS);
						glVertex3f(fltx375, cym1d3, czm749);
						glVertex3f(fltx3375, cym1d3, czm749);
						glVertex3f(fltx3375, cym1d0, czm749);
						glVertex3f(fltx375, cym1d0, czm749);
						glEnd();

					}
				}
				t++;
				if (t == 12)t = 0;
			}



			glColor4f(tr, tg, tb, ta);
			
			
			glPopMatrix();
			glFinish();
			SwapBuffers(wglGetCurrentDC());

			;

		}
		//("%d ", mode);
	T:
		;
		
	}
T2:
	UpdateData(0);

}
BOOL CCubeView::OnEraseBkgnd(CDC* /*pDC*/)
{
	return TRUE;
}
void CCubeView::OnDropFiles(HDROP hDropInfo)
{

	char c[1500]; int i;
	unsigned int j = DragQueryFile(hDropInfo, 0xFFFFFFFF, NULL, 0);
	if (j != 1 || mode >= 12) {
		char playl[1000];
		strcpy(playl, "\0");
		for (int i = 0; i < j; i++) {
			DragQueryFile(hDropInfo, i, (LPSTR)c, 255);
			strcat(playl, "\"");
			strcat(playl, c);
			strcat(playl, "\" ");
		}
		playl[strlen(playl) - 1] = 0;
		char st[1000];
		char st2[1000];
		GetModuleFileNameA(NULL, st, 1000);
		sprintf(st2, "\"%s\"", st);
		ShellExecuteA(NULL, "OPEN", st2, playl, NULL, SW_SHOW);
		exit2(1);
	}
	DragQueryFile(hDropInfo, 0, (LPSTR)c, 255);;
	::DragFinish(hDropInfo);
	if (strcmp(&c[strlen(c) - 3], "mid") == 0) {
		print2st(c);
		main2(c);
	}
	else if (strcmp(&c[strlen(c) - 3], "wav") == 0) {
		print2st(c);
		main2(c);
	}
	else if (strcmp(&c[strlen(c) - 5], "hmp") == 0) {
		print2st(c);
		main2(c);
	}
	else if (strcmp(&c[strlen(c) - 3], "bmp") == 0) {
		strcpy(recentfilelist[10], c);
		putini();
		loadimg();
	}
	else if (strcmp(&c[strlen(c) - 3], "jpg") == 0) {
		strcpy(recentfilelist[10], c);
		putini();
		loadimg();
	}
	else if (strcmp(&c[strlen(c) - 3], "gif") == 0) {
		strcpy(recentfilelist[10], c);
		putini();
		loadimg();
	}
	else if (strcmp(&c[strlen(c) - 3], "tif") == 0) {
		strcpy(recentfilelist[10], c);
		putini();
		loadimg();
	}
	else if (strcmp(&c[strlen(c) - 3], "png") == 0) {
		strcpy(recentfilelist[10], c);
		putini();
		loadimg();
	}
	else if (strcmp(&c[strlen(c) - 3], "sf2") == 0) {
		strcpy(recentfilelist[11], c);
		putini();

	}
	CView::OnDropFiles(hDropInfo);

}
void CCubeView::OnInitialUpdate()
{
	CView::OnInitialUpdate();
	DragAcceptFiles();

}
void CCubeView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{


	CView::OnChar(nChar, nRepCnt, nFlags);
}
void CCubeView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (mode >= 12 && mode < 50) {
		if (nChar == '1') OnPlayPlay();
		else if (nChar == '2') OnPlayPause();
		else if (nChar == '3') OnPlayStop();
		else if (nChar == '4') OnPlayReplay();
		else if (nChar == '5') OnPlayVolumeup();
		else if (nChar == '6') OnPlayVolumedown();
		else if (nChar == '8' && owptr6 > 24 * 2) OnPlaySkipback();
		else if (nChar == '7'&& owptr6 > 24 * 2)OnPlaySkipforward();
		else if (nChar == '9' && !midiout) OnPlayPitchup();
		else if (nChar == '0' && !midiout) OnPlayPitchdown();
		else if (nChar == 189 && !midiout) OnPlaySpeedup();
		else if (nChar == 187 && !midiout) OnPlaySpeeddown();
		else if (nChar == 'P')OnPlayPlay();
		else if (nChar == 'A')OnPlayPause();
		else if (nChar == 'S')OnPlayStop();
		else if (nChar == 'R')OnPlayReplay();
		else if (nChar == 'p')OnPlayPlay();
		else if (nChar == 'a')OnPlayPause();
		else if (nChar == 's')OnPlayStop();
		else if (nChar == 'r')OnPlayReplay();
		//else if (nChar == 189)OnPlayPitchup();
		//else if (nChar == 187)OnPlayPitchdown();
		//else if (nChar == 219)OnPlaySpeedup();
		//else if (nChar == 221)OnPlaySpeeddown();
		else if (nChar == 'I')OnViewSetbackgroundimage();
		else if (nChar == 'G')OnSettingsViewsettings();
		else if (nChar == 'i')OnViewSetbackgroundimage();
		else if (nChar == 'g')OnSettingsViewsettings();
		else if (nChar == VK_UP) OnPlayVolumeup();
		else if (nChar == VK_DOWN) OnPlayVolumedown();
		else if (nChar == VK_LEFT && owptr6 > 24 * 2) OnPlaySkipback();
		else if (nChar == VK_RIGHT&& owptr6 > 24 * 2)OnPlaySkipforward();
		else if (nChar == VK_TAB)OnPlayReset();
		else if (nChar == VK_SPACE) {
			if (mode == 14) {
				OnPlayReplay();
			}
			else {
				if (ply == true) {
					ply = false;
					f = true;

				}
				else {
					ply = true;
					f = true;
				}
			}
		}
	}


	else if (nChar == VK_ESCAPE) {
		stop = true;


	}
	CView::OnKeyDown(nChar, nRepCnt, nFlags);

}
void CCubeView::OnLButtonDown(UINT nFlags, CPoint point)
{

	mov = true;
	dbc = false;
	ox = point.x;
	oy = point.y;
	CView::OnLButtonDown(nFlags, point);
}
void CCubeView::OnMButtonDown(UINT nFlags, CPoint point)
{
	CRect rct;
	GetClientRect(rct);
	or = tr; og = tg;
	tr = (float)point.x / (float)rct.Width();
	tg = 1 - ((float)point.y / (float)rct.Height());

	CView::OnMButtonDown(nFlags, point);
}
BOOL CCubeView::OnHelpInfo(HELPINFO* pHelpInfo)
{

	st4.ShowWindow(SW_SHOW);
	return CView::OnHelpInfo(pHelpInfo);
}
BOOL CCubeView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (zDelta > 0) j5jgj *= 1.05;
	else j5jgj /= 1.05;
	return CView::OnMouseWheel(nFlags, zDelta, pt);
}
void CCubeView::OnLButtonUp(UINT nFlags, CPoint point)
{
	mov = false;
	CView::OnLButtonUp(nFlags, point);
}
void CCubeView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	dbc = true;
	mov = true;
	ox = point.x;
	oy = point.y;
	CView::OnLButtonDblClk(nFlags, point);
}
void CCubeView::OnRButtonDown(UINT nFlags, CPoint point)
{
	if (mode >= 12 && mode <50) {
		asfaq = point.x;
		brebe = point.y;
	}

	CView::OnRButtonDown(nFlags, point);
}
void CCubeView::OnRButtonDblClk(UINT nFlags, CPoint point)
{
	if (mode >= 12 && mode <50) {
		asfaq = point.x;
		brebe = point.y;
	}
	textmode++;

	if (textmode == 5)textmode = -1;
	CView::OnRButtonDblClk(nFlags, point);
}
void CCubeView::OnMouseMove(UINT nFlags, CPoint point)
{
	TRACKMOUSEEVENT        mouseEvent;
	mouseEvent.cbSize = sizeof(mouseEvent);
	mouseEvent.dwFlags = TME_LEAVE;
	mouseEvent.hwndTrack = m_hWnd;
	TrackMouseEvent(&mouseEvent);
	if (mov == true) {
		if (dbc == false) { gasfw -= (float)(point.x - ox) / 10; h45h7 -= (float)(point.y - oy) / 5; }
		else hg4h5 -= (float)(point.x - ox) / 10;


		//("%f %f %f\n", h45h7, gasfw, hg4h5);
		ox = point.x;
		oy = point.y;
	}
	CView::OnMouseMove(nFlags, point);
}
void CCubeView::OnNcLButtonUp(UINT nHitTest, CPoint point)
{
	//("up ");
	mov = false;
	CView::OnNcLButtonUp(nHitTest, point);
}
void CCubeView::OnMouseLeave()
{
	//("up ");
	mov = false;
	CView::OnMouseLeave();
}
void CCubeView::OnSizing(UINT fwSide, LPRECT pRect)
{
	CView::OnSizing(fwSide, pRect);

}
void CCubeView::OnMButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	tr = or ; tg = og;
	CRect rct;
	GetClientRect(rct);
	tb = (float)point.x / (float)rct.Width();
	ta = 1 - ((float)point.y / (float)rct.Height());
	CView::OnMButtonDblClk(nFlags, point);
}
void CCubeView::OnFileOpen32773()
{
	//MessageBox("A", "A", MB_OK);
	

	CFileDialog dlg(TRUE, NULL, NULL, OFN_ALLOWMULTISELECT | OFN_FILEMUSTEXIST | OFN_LONGNAMES,
		"MIDI FILES(*.mid)|*.mid|All Files(*.*)|*.*||");
	CString strPathName;
	CString strFileList;
	const int c_cMaxFiles = 400 ;	// 메모리 부족현상으로 확장 안해주면 몇개 못씀
	const int c_cbBuffSize = (c_cMaxFiles * (MAX_PATH + 1)) + 1;
	dlg.GetOFN().lpstrFile = strFileList.GetBuffer(c_cbBuffSize);
	dlg.GetOFN().nMaxFile = c_cbBuffSize;
	if (IDOK == dlg.DoModal()) {
		if (mode >= 12) goto G;
		FILE *t;
		strPathName = dlg.GetPathName();
		if ((t = fopen(strPathName.GetBuffer(), "r")) == NULL) goto G;
		print2st((strPathName.GetBuffer()));
		main2((strPathName.GetBuffer()));
	}
	return;
G:
	char playl[1000];
	strcpy(playl, "\0");
	for (POSITION pos = dlg.GetStartPosition(); pos != NULL;) {

		strcat(playl, "\"");
		strcat(playl, dlg.GetNextPathName(pos));
		strcat(playl, "\" ");
	}
	playl[strlen(playl) - 1] = 0;
	char st[1000];
	char st2[1000];
	GetModuleFileNameA(NULL, st, 1000);
	sprintf(st2, "\"%s\"", st);
	ShellExecuteA(NULL, "OPEN", st2, playl, NULL, SW_SHOW);
	exit2(1);
	return;
}
void CCubeView::OnViewSetbackgroundimage()
{
	
	CFileDialog dlg(TRUE, NULL, NULL, OFN_FILEMUSTEXIST | OFN_LONGNAMES,
		"IMAGE FILES(*.bmp,*.jpg,*.png,*.tif,*.gif)|*.bmp;*.jpg;*.png;*.tif;*.gif|All Files(*.*)|*.*||");
	CString strPathName;
	if (IDOK == dlg.DoModal()) {
		FILE *t;
		strPathName = dlg.GetPathName();
		if ((t = fopen(strPathName.GetBuffer(), "r")) == NULL) return;
		strcpy(recentfilelist[10], strPathName.GetBuffer());
		putini();
		loadimg();
	}

}
void CCubeView::OnViewLoadsoundfont()
{
	
	
	st.ShowWindow(SW_SHOW);
}
void CCubeView::OnRecent11()
{
	char st[1000];
	char st2[1000];
	char st3[1000];
	GetModuleFileNameA(NULL, st, 1000);
	sprintf(st2, "\"%s\"", st);
	sprintf(st3, "\"%s\"", recentfilelist[0]);

	ShellExecuteA(NULL, "OPEN", st2, st3, NULL, SW_SHOW);
	if (!midiout) {
		//	MessageBox("B", "B", MB_OK);
		//cmidi.Pause();
		TerminateThread(thread1, 0);
		CloseHandle(thread1);
		Sleep(100);
		pauseall();
		midiOutClose(playmidiout);

		char a[1000];
		fclose(wj6);
		fclose(wj7);

	}
	exit2(1);
}
void CCubeView::OnRecent12()
{
	char st[1000];
	char st2[1000];
	char st3[1000];
	GetModuleFileNameA(NULL, st, 1000);
	sprintf(st2, "\"%s\"", st);
	sprintf(st3, "\"%s\"", recentfilelist[1]);

	ShellExecuteA(NULL, "OPEN", st2, st3, NULL, SW_SHOW);
	if (!midiout) {
		//	MessageBox("B", "B", MB_OK);
		//cmidi.Pause();
		TerminateThread(thread1, 0);
		CloseHandle(thread1);
		Sleep(100);
		pauseall();
		midiOutClose(playmidiout);

		char a[1000];
		fclose(wj6);
		fclose(wj7);

	}
	exit2(1);
}
void CCubeView::OnRecent13()
{
	char st[1000];
	char st2[1000];
	char st3[1000];
	GetModuleFileNameA(NULL, st, 1000);
	sprintf(st2, "\"%s\"", st);
	sprintf(st3, "\"%s\"", recentfilelist[2]);

	ShellExecuteA(NULL, "OPEN", st2, st3, NULL, SW_SHOW);
	if (!midiout) {
		//	MessageBox("B", "B", MB_OK);
		//cmidi.Pause();
		TerminateThread(thread1, 0);
		CloseHandle(thread1);
		Sleep(100);
		pauseall();
		midiOutClose(playmidiout);

		char a[1000];
		fclose(wj6);
		fclose(wj7);

	}
	exit2(1);
}
void CCubeView::OnRecent14()
{
	char st[1000];
	char st2[1000];
	char st3[1000];
	GetModuleFileNameA(NULL, st, 1000);
	sprintf(st2, "\"%s\"", st);
	sprintf(st3, "\"%s\"", recentfilelist[3]);

	ShellExecuteA(NULL, "OPEN", st2, st3, NULL, SW_SHOW);
	if (!midiout) {
		//	MessageBox("B", "B", MB_OK);
		//cmidi.Pause();
		TerminateThread(thread1, 0);
		CloseHandle(thread1);
		Sleep(100);
		pauseall();
		midiOutClose(playmidiout);

		char a[1000];
		fclose(wj6);
		fclose(wj7);

	}
	exit2(1);
}
void CCubeView::OnRecent15()
{
	char st[1000];
	char st2[1000];
	char st3[1000];
	GetModuleFileNameA(NULL, st, 1000);
	sprintf(st2, "\"%s\"", st);
	sprintf(st3, "\"%s\"", recentfilelist[4]);

	ShellExecuteA(NULL, "OPEN", st2, st3, NULL, SW_SHOW);
	if (!midiout) {
		//	MessageBox("B", "B", MB_OK);
		//cmidi.Pause();
		TerminateThread(thread1, 0);
		CloseHandle(thread1);
		Sleep(100);
		pauseall();
		midiOutClose(playmidiout);

		char a[1000];
		fclose(wj6);
		fclose(wj7);

	}
	exit2(1);
}
void CCubeView::OnRecent16()
{
	char st[1000];
	char st2[1000];
	char st3[1000];
	GetModuleFileNameA(NULL, st, 1000);
	sprintf(st2, "\"%s\"", st);
	sprintf(st3, "\"%s\"", recentfilelist[5]);

	ShellExecuteA(NULL, "OPEN", st2, st3, NULL, SW_SHOW);
	if (!midiout) {
		//	MessageBox("B", "B", MB_OK);
		//cmidi.Pause();
		TerminateThread(thread1, 0);
		CloseHandle(thread1);
		Sleep(100);
		pauseall();
		midiOutClose(playmidiout);

		char a[1000];
		fclose(wj6);
		fclose(wj7);

	}
	exit2(1);
}
void CCubeView::OnRecent17()
{
	char st[1000];
	char st2[1000];
	char st3[1000];
	GetModuleFileNameA(NULL, st, 1000);
	sprintf(st2, "\"%s\"", st);
	sprintf(st3, "\"%s\"", recentfilelist[6]);

	ShellExecuteA(NULL, "OPEN", st2, st3, NULL, SW_SHOW);
	if (!midiout) {
		//	MessageBox("B", "B", MB_OK);
		//cmidi.Pause();
		TerminateThread(thread1, 0);
		CloseHandle(thread1);
		Sleep(100);
		pauseall();
		midiOutClose(playmidiout);

		char a[1000];
		fclose(wj6);
		fclose(wj7);

	}
	exit2(1);
}
void CCubeView::OnRecent18()
{
	char st[1000];
	char st2[1000];
	char st3[1000];
	GetModuleFileNameA(NULL, st, 1000);
	sprintf(st2, "\"%s\"", st);
	sprintf(st3, "\"%s\"", recentfilelist[7]);

	ShellExecuteA(NULL, "OPEN", st2, st3, NULL, SW_SHOW);
	if (!midiout) {
		//	MessageBox("B", "B", MB_OK);
		//cmidi.Pause();
		TerminateThread(thread1, 0);
		CloseHandle(thread1);
		Sleep(100);
		pauseall();
		midiOutClose(playmidiout);

		char a[1000];
		fclose(wj6);
		fclose(wj7);

	}
	exit2(1);
}
void CCubeView::OnRecent19()
{
	char st[1000];
	char st2[1000];
	char st3[1000];
	GetModuleFileNameA(NULL, st, 1000);
	sprintf(st2, "\"%s\"", st);
	sprintf(st3, "\"%s\"", recentfilelist[8]);

	ShellExecuteA(NULL, "OPEN", st2, st3, NULL, SW_SHOW);
	if (!midiout) {
		//	MessageBox("B", "B", MB_OK);
		//cmidi.Pause();
		TerminateThread(thread1, 0);
		CloseHandle(thread1);
		Sleep(100);
		pauseall();
		midiOutClose(playmidiout);

		char a[1000];
		fclose(wj6);
		fclose(wj7);

	}
	exit2(1);
}
void CCubeView::OnRecent20()
{
	char st[1000];
	char st2[1000];
	char st3[1000];
	GetModuleFileNameA(NULL, st, 1000);
	sprintf(st2, "\"%s\"", st);
	sprintf(st3, "\"%s\"", recentfilelist[9]);

	ShellExecuteA(NULL, "OPEN", st2, st3, NULL, SW_SHOW);
	if (!midiout) {
		//	MessageBox("B", "B", MB_OK);
		//cmidi.Pause();
		TerminateThread(thread1, 0);
		CloseHandle(thread1);
		Sleep(100);
		pauseall();
		midiOutClose(playmidiout);

		char a[1000];
		fclose(wj6);
		fclose(wj7);

	}
	exit2(1);
}
void CCubeView::OnFileReset()
{
	
	char st[1000];
	char st2[1000];

	GetModuleFileNameA(NULL, st, 1000);
	sprintf(st2, "\"%s\"", st);
	ShellExecuteA(NULL, "OPEN", st2, 0, NULL, SW_SHOW);
	if (!midiout) {
		//	MessageBox("B", "B", MB_OK);
		//cmidi.Pause();
		TerminateThread(thread1, 0);
		CloseHandle(thread1);
		Sleep(100);
		pauseall();
		midiOutClose(playmidiout);

		char a[1000];
		fclose(wj6);
		fclose(wj7);
		
	}
	exit2(1);
}
void CCubeView::OnSettingsViewsettings()
{
	
	st2.ShowWindow(SW_SHOW);
}
void CCubeView::OnPlayPause()
{
	
	if (ply == true) {
		ply = false;
		f = true;

	}

}
void CCubeView::OnPlayPitchdown()
{
	ro = 1;
	for (int i = 0; i < 16; i++) {
		pitch[i] /= 1.0594630943592952645618252949463;
		if (pitch[i] > 2850.8758980490857751687238578896)pitch[i] = 2850.8758980490857751687238578896;
		if (pitch[i] < 3.5076939009667905669797907802474) pitch[i] = 3.5076939009667905669797907802474;
	}
	pitch[9] = 100.;
	if (kj768 == true)
		sprintf(t, "하이온의 미디 플레이어 v5.4rev2 - 음높이 -1");
	else
		sprintf(t, "hyon's midi player v5.4rev2 - pitch -1");

	GetParent()->SetWindowTextA(t);
}
void CCubeView::OnPlayPitchup()
{

	ro = 1;
	for (int i = 0; i < 16; i++) {
		pitch[i] *= 1.0594630943592952645618252949463;
		if (pitch[i] > 2850.8758980490857751687238578896)pitch[i] = 2850.8758980490857751687238578896;
		if (pitch[i] < 3.5076939009667905669797907802474) pitch[i] = 3.5076939009667905669797907802474;
	}
	pitch[9] = 100.;
	if (kj768 == true)
		sprintf(t, "하이온의 미디 플레이어 v5.4rev2 - 음높이 +1");
	else
		sprintf(t, "hyon's midi player v5.4rev2 - pitch +1");

	GetParent()->SetWindowTextA(t);
}
void CCubeView::OnPlayPlay()
{
	
	if (ply == false) {
		ply = true;
		f = true;
	}
}
void CCubeView::OnPlayReplay()
{
	char st[1000];
	char st2[1000];
	char st3[1000];
	GetModuleFileNameA(NULL, st, 1000);
	sprintf(st2, "\"%s\"", st);
	sprintf(st3, "\"%s\"", midname2);

	ShellExecuteA(NULL, "OPEN", st2, st3, NULL, SW_SHOW);
	if (!midiout) {
		//	MessageBox("B", "B", MB_OK);
		//cmidi.Pause();
		TerminateThread(thread1, 0);
		CloseHandle(thread1);
		Sleep(100);
		pauseall();
		midiOutClose(playmidiout);

		char a[1000];
		fclose(wj6);
		fclose(wj7);
		
	}
	exit2(1);
}
void CCubeView::OnPlaySkipback()
{
	
	unsigned int oclo = clock();
	ro = 1;
	int oisplay;
	if (!midiout) {
		oisplay = isplay;
		isplay = 0;
	}
	if (playoffset >= 200) {
		if (skip == false) {
			skip = true;
			playoffset -= 200;
			playmidin0 += 200 * 50;
			//if (playoffset < 0) playoffset = 0;
			if (kj768 == true)
				sprintf(t, "하이온의 미디 플레이어 v5.4rev2 -오프셋 %.2lf초", (float)playoffset / 20, '%');
			else
				sprintf(t, "hyon's midi player v5.4rev2 -offset %.2lfsec", (float)playoffset / 20, '%');

			GetParent()->SetWindowTextA(t);
			mtinit2();
		}
	}

	if (!midiout) { isplay = oisplay; playmidin0 += clock() - oclo; }
}
void CCubeView::OnPlaySkipforward()
{
	

	unsigned int oclo = clock();
	ro = 1;
	int oisplay;
	if (!midiout) {
		oisplay = isplay;
		isplay = 0;
	}

	//playoffset += 200;
	if (playoffset + 300 <= playlength / 5)
	{
		if (skip == false) {
			skip = true;
			playoffset += 200;
			playmidin0 -= 200 * 50;
			if (kj768 == true)
				sprintf(t, "하이온의 미디 플레이어 v5.4rev2 -오프셋 %.2lf초", (float)playoffset / 20, '%');
			else
				sprintf(t, "hyon's midi player v5.4rev2 -offset %.2lfsec", (float)playoffset / 20, '%');
			GetParent()->SetWindowTextA(t);
			mtinit2();
		}
	}

	if (!midiout) { isplay = oisplay; playmidin0 += clock() - oclo; }
}
void CCubeView::OnPlaySpeeddown()
{
	
	spd2 = spd/1.005;
	if (spd2<10.)spd2 = 10.;
	unsigned long clocktemp = clock();
	_clock2 = (int)(_clock2 + (spd / 100. * (double)(clocktemp - clock_start)));
	clock_start = clocktemp;
	spd = spd2;
	UpdateData(1);
	KillTimer(1);
	SetTimer(1, (int)(17. / (spd / 100.)), 0);
	//SetTimer(1, playspeed / 5 * 2, 0);
	if (kj768 == true)
		sprintf(t, "하이온의 미디 플레이어 v5.4rev2 -재생 속도 %d%c", (int)(spd), '%');
	else
		sprintf(t, "hyon's midi player v5.4rev2 -speed %d%c", (int)spd, '%');
	GetParent()->SetWindowTextA(t);
	UpdateData(0);

}
void CCubeView::OnPlaySpeedup()
{
	
	spd2 = spd * 1.005;
	if (spd2 >300.)spd2 = 300.;
	unsigned long clocktemp = clock();
	_clock2 = (int)(_clock2 + (spd / 100. * (double)(clocktemp - clock_start)));
	clock_start = clocktemp;
	spd = spd2;
	UpdateData(1);
	KillTimer(1);
	SetTimer(1, (int)(17. / (spd / 100.)), 0);
	if (kj768 == true)
		sprintf(t, "하이온의 미디 플레이어 v5.4rev2 -재생 속도 %d%c", (int)(spd), '%');
	else
		sprintf(t, "hyon's midi player v5.4rev2 -speed %d%c", (int)spd, '%');
	GetParent()->SetWindowTextA(t);
	UpdateData(0);
}
void CCubeView::OnPlayStop()
{
	char st[1000];
	char st2[1000];
	char st3[1000];
	GetModuleFileNameA(NULL, st, 1000);
	sprintf(st2, "\"%s\"", st);
	sprintf(st3, "\"%s\"_p_", midname2);
	for (int i = 2; i < __argc; i++) {
		strcat(st3, " \"");
		strcat(st3, __argv[i]);
		strcat(st3, "\"");
	}
	ShellExecuteA(NULL, "OPEN", st2, st3, NULL, SW_SHOW);
	if (!midiout) {
		//	MessageBox("B", "B", MB_OK);
		//cmidi.Pause();
		TerminateThread(thread1, 0);
		CloseHandle(thread1);
		Sleep(100);
		pauseall();
		midiOutClose(playmidiout);

		char a[1000];
		fclose(wj6);
		fclose(wj7);
		
	}
	exit2(1);

}
void CCubeView::OnPlayVolumedown()
{
	
	playvolume /= 1.01;
	if (playvolume<0.01)playvolume = 0.01;

	waveOutSetVolume(0, (int)(playvolume * 65535.) + ((int)(playvolume * 65535.) << 16));
	midiOutSetVolume(playmidiout, (int)(playvolume * 65535.) + ((int)(playvolume * 65535.) << 16));
	//alSourcef(isource, AL_GAIN, playvolume);
	if (kj768 == true)
		sprintf(t, "하이온의 미디 플레이어 v5.4rev2 - 소리 크기 %d%c", (int)(playvolume * 100), '%');
	else
		sprintf(t, "hyon's midi player v5.4rev2 - volume %d%c", (int)(playvolume * 100), '%');
	GetParent()->SetWindowTextA(t);
}
void CCubeView::OnPlayVolumeup()
{
	
	playvolume *= 1.01;
	if (playvolume>1.0)playvolume = 1.0;
	waveOutSetVolume(0, (int)(playvolume * 65535.) + ((int)(playvolume * 65535.) << 16));
	midiOutSetVolume(playmidiout, (int)(playvolume * 65535.) + ((int)(playvolume * 65535.) << 16));
	//alSourcef(isource, AL_GAIN, playvolume);
	if (kj768 == true)
		sprintf(t, "하이온의 미디 플레이어 v5.4rev2 - 소리 크기 %d%c", (int)(playvolume * 100), '%');
	else
		sprintf(t, "hyon's midi player v5.4rev2 - volume %d%c", (int)(playvolume * 100), '%');
	GetParent()->SetWindowTextA(t);
}
void CCubeView::OnPlayReset()
{
	
	ro = 1;
	playvolume = 1.;
	spd = 100.;
	for (int i = 0; i < 16; i++) {
		volume2[i] = 100.;
		pitch[i] = 100.;
	}
	bpshf = 0;
	if (kj768 == true)
		sprintf(t, "하이온의 미디 플레이어 v5.4rev2 -속도 100%c,소리 크기 100%c", '%', '%');
	else
		sprintf(t, "hyon's midi player v5.4rev2 -speed 100%c,volume 100%c", '%', '%');
	GetParent()->SetWindowTextA(t);
	if (midiout)waveOutSetPosition(playoffset * 50);
	else {
		//cmidi.Pause();
		//cmidi.SetPos(playoffset * 50);
		//cmidi.RePlay();
	}
	//alSourcef(isource, AL_SEC_OFFSET, (float)playoffset / 20);
}
void CCubeView::OnPlayPitchspeed()
{
	
	
	st3.ShowWindow(SW_SHOW);
}
void CCubeView::OnHelpViewreadme()
{
	
	st4.ShowWindow(SW_SHOW);
}
void CCubeView::OnEditClearAll()
{
	
	UpdateData(1);

	for (int i = 0; i < 10; i++) {
		recentfilelist[i][0] = 0;
	}
	putini();
	UpdateData(0);
}
void CCubeView::OnRecentClearall()
{
	
	UpdateData(1);

	for (int i = 0; i < 10; i++) {
		recentfilelist[i][0] = 0;
	}
	putini();
	UpdateData(0);
}
void CCubeView::OnFileUninstall()
{
	
	char st[1000];
	GetModuleFileNameA(NULL, st, 1000);
	FILE *t = fopen("a.bat", "w");
	fprintf(t, "ping 127.0.0.1 -n 1\ndel \"%s\"\ndel hmp6.ini\ndel gm.sf2\ndel a.bat", st);
	fclose(t);
	ShellExecute(0, "open", "a.bat", 0, 0, 0);
	exit2(1);
}
void CCubeView::OnMove(int x, int y)
{
	CView::OnMove(x, y);
	UpdateData(1);
	//MessageBox("A", "A", MB_OK);
	if (sq1) {
		RECT rect;
		CMainFrame *frame = (CMainFrame*)AfxGetMainWnd();
		frame->GetWindowRect(&rect);
		windv = rect.bottom - rect.top;
		windh = rect.right - rect.left;
		windowx = rect.left;
		windowy = rect.top;
		putini();
	}
	UpdateData(0);
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}
void CCubeView::OnMoving(UINT fwSide, LPRECT pRect)
{
	CView::OnMoving(fwSide, pRect);
	UpdateData(1);
	//MessageBox("A", "A", MB_OK);
	if (sq1) {
		RECT rect;
		CMainFrame *frame = (CMainFrame*)AfxGetMainWnd();
		frame->GetWindowRect(&rect);
		windv = rect.bottom - rect.top;
		windh = rect.right - rect.left;
		windowx = rect.left;
		windowy = rect.top;
		putini();
	}
	UpdateData(0);
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

