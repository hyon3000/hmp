#pragma once
#include <gl\gl.h>
#include <gl\glu.h>
#include <winapifamily.h>
#include <windows.h>
#include "extern.h"
HFONT fnt; HDC strgrphdc;
bool bf = false; int fsize = 20;
void buildfont() {
	strgrphdc = wglGetCurrentDC();
	fnt = CreateFontA(fsize, 0, 0, 0, (txtmd2&0x01)?FW_EXTRABOLD:FW_NORMAL, (txtmd2&0x02)==0x02, FALSE, FALSE, ANSI_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS,
		ANTIALIASED_QUALITY, FF_DONTCARE | DEFAULT_PITCH, recentfilelist[13]);
	SelectObject(strgrphdc, fnt);
}
void pc(float x, float y, float z, char *dest) {
	if (bf == false) {
		bf = true; buildfont();
	}
	for (int i = 0; i < strlen(dest); i++) {
		if (dest[i] == 0x10) dest[i] = 0x20;
	}
	wchar_t *wchar;

	int wcharlength = MultiByteToWideChar(CP_ACP, 0, dest, strlen(dest), NULL, NULL);
	wchar = new wchar_t[wcharlength];
	MultiByteToWideChar(CP_ACP, 0, dest, strlen(dest), wchar, wcharlength);
	glRasterPos3f(x, y,z);
	float t = 0; int ListNum = glGenLists(1);
	for (int i = 0; i < wcharlength; i++) {

		wglUseFontBitmapsW(strgrphdc, wchar[i], 1, ListNum);
		glCallList(ListNum);

	}glDeleteLists(ListNum, 1);
	delete[]wchar;
}
void printchr2(float ptrx, float ptry, char* dest, int shf) {
	//glColor4f(1.0,1.0,0.0,1.0);
	if (bf == false) {
		bf = true; buildfont();
	}
	for (int i = 0; i < strlen(dest); i++) {
		if (dest[i] == 0x10) dest[i] = 0x20;
	}
	
	wchar_t *wchar;
	

	int wcharlength = MultiByteToWideChar(CP_ACP, 0, dest, strlen(dest), NULL, NULL);
	wchar = new wchar_t[wcharlength];
	MultiByteToWideChar(CP_ACP, 0, dest, strlen(dest), wchar, wcharlength);
	
	glRasterPos3f(ptrx, ptry,10.f);
	
	float t = 0;int ListNum = glGenLists(1);
	for (int i = 0; i < wcharlength; i++) {
		
		wglUseFontBitmapsW(strgrphdc, wchar[i], 1, ListNum);
		glCallList(ListNum);
		
	}
	
	glDeleteLists(ListNum, 1);
	delete[]wchar;
}
void printchr(int asfaq, int brebe, long width, long height, char *dest, int shf) {

	float ptrx = 1.*x1n1*f346h*(float)(asfaq - width / 2) / (float)width;
	float ptry = -1.*y1n1*((float)((brebe+shf) - height / 2) / (float)height-ypn1);
	////printf("%d %d %d %d\n", asfaq, brebe, width, height);
	printchr2(ptrx, ptry, dest, shf);
}
