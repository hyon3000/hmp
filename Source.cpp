#include <stdio.h>
#include <string.h>
#include <Windows.h>
#include <math.h>
#include <time.h>
#include <conio.h>
#include <omp.h>
#define using_openmp
#ifndef sourcecpp
#define sourcecpp

#pragma warning (disable:4996)
#pragma comment(lib, "shell32.lib") 
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
const unsigned char opts[] = { 0x52, 0x49, 0x46, 0x46, 0xE8, 0xEA, 0x1A, 0x00, 0x57, 0x41, 0x56, 0x45, 0x66, 0x6D, 0x74, 0x20, 0x28, 0x00, 0x00, 0x00, 0xFE, 0xFF, 0x01, 0x00, 0x44, 0xAC, 0x00, 0x00, 0x10, 0xB1, 0x02, 0x00, 0x04, 0x00, 0x20, 0x00, 0x16, 0x00, 0x20, 0x00, 0x04, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x80, 0x00, 0x00, 0xAA, 0x00, 0x38, 0x9B, 0x71, 0x64, 0x61, 0x74, 0x61 };
const char keysigm[2][15][15] = { {"내림다장조","내림사장조","내림라장조","내림가장조","내림마장조","내림나장조","바장조","다장조","사장조","라장조","가장조","마장조","나장조","올림바장조","올림다장조" } 
, { "내림가단조", "내림마단조", "내림나단조", "바단조", "다단조", "사단조", "라단조", "가단조", "마단조", "나단조", "올림바단조", "올림다단조", "올림사단조", "올림라단조", "올림가단조" }
};
long long wptr = 0;
long long owptr = 0;
long long owptr2 = 0;
long long owptr3 = 0;
long long owptr4 = 0; long long owptr5 = 0;
long long mptr = 0; 
long pnotes = 0;
unsigned int deltatime; char t[10000], tbk[10000], tbk2[10000], tbk3[10000], tbk4[10000];
int bth=4, btl=4;
int keysig = 0; bool keysig2 = false;//f:장조
long long *le;//마지막 이벤트 시점의 적분 델타타임
long long *le2;//다음 이벤트 델타타임
int chvol[16] = { 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127 };//채널 볼륨
int *key[128];//키 볼륨
int *key2[128];//키 경과시간
unsigned char *key3[128];//채널
int *fg2;
int *oldstate;
unsigned int tempo = 120;
int spd = 100;
bool *buf;//종료 확인,안끝났으면 f
int *pitch;
FILE **ch;
unsigned long long note=0;
int chn=-1;
union int2char{ int i=0; char c[4]; };

inline int fgetc2(FILE* file,int id){
	int t2 = fgetc(file);
	return t2;
}
/*	if (t2 == -1){
			printf("!");
			
			fseek(file, fg2[id], SEEK_SET);
			while (1){
				
				if (ftell(file) < fg2[id]) fseek(file,1 , SEEK_CUR);
				else if (ftell(file) > fg2[id]) fseek(file, -1, SEEK_CUR);
				else break;
			}
			t2 = fgetc(file);
			printf("%x %x ", t2, ftell(file));
			system("pause");
	}*/
inline double freq(int a, int pit){
	int b = a - 0x45;
	double c = 440;
	while (1){
		if (b > 12){ b -= 12; c *= 2; }
		else if (b < -12){ b += 12; c /= 2; }
		else if (b>4) { b -= 4; c *= 1.2599210498948731647672106072782; }
		else if (b<-4) { b += 4; c /= 1.2599210498948731647672106072782; }
		else if (b>0) { b--; c *= 1.0594630943592952645618252949463; }
		else if (b<0) { b++; c /= 1.0594630943592952645618252949463; }
		else break;
	}
	if (pit == 1000) return c;
	else return c*(double)pit / 1000;
}
inline double soundfont(double f,double t){
	return sin(2 * 3.14159265358979323846 *f*t) / ((t + 1)*(t + 1)*(t + 1)*(t + 1)*(t + 1));
}
inline int outc(){
	double temp = 0;
	int t;
	int j;
#ifdef using_openmp
	if (pnotes < 50){
		for (int i = 0; i < chn; i++){
			for (j = 0; j < 128; j++){
				if (key[j][i] != 0){
					key2[j][i]++;
					//printf("%lf ", sin(2*3.14159265358979323846*freq(j,pitch[i])*(double)key2[j][i]/44100));
					temp += (double)(key[j][i]) * 3000 * chvol[key3[j][i]] * soundfont(freq(j, pitch[i]), (double)key2[j][i] / 44100) / ((double)(chn));
				}
			}
		}
	}
	else{
		double temp2[4] = { 0, 0, 0, 0 };
		
#pragma omp parallel private(t) num_threads(4)
			{
				t = omp_get_thread_num()+1;
				for (int i = 0; i < chn; i++){
				for (j = 32*t-32; j < 32*t; j++){
					if (key[j][i] != 0){
						key2[j][i]++;
						//printf("%lf ", sin(2*3.14159265358979323846*freq(j,pitch[i])*(double)key2[j][i]/44100));
						temp2[t] += (double)(key[j][i]) * 3000 * chvol[key3[j][i]] * soundfont(freq(j, pitch[i]), (double)key2[j][i] / 44100) / ((double)(chn));
					}
				}
			}
			
		}
		temp = temp2[0] + temp2[1] + temp2[3] + temp2[2];
	}
#else
for (int i = 0; i < chn; i++){
	for (j = 0; j < 128; j++){
		if (key[j][i] != 0){
			key2[j][i]++;
			//printf("%lf ", sin(2*3.14159265358979323846*freq(j,pitch[i])*(double)key2[j][i]/44100));
			temp += (double)(key[j][i]) * 3000 * chvol[key3[j][i]] * soundfont(freq(j, pitch[i]), (double)key2[j][i] / 44100) / ((double)(chn));
		}
	}
}
#endif
	return (int)temp;
}
inline void chnvol(int vol, int midichennel){
	//if(vol!=0) chvol[midichennel - 0xB0] = vol;
}
inline void noteoff(int num, int chennel, int midichennel){
	key[num][chennel] = 0; pnotes--;
	key2[num][chennel] = 0;
}
inline void noteon(int num, int vel,int chennel,int midichennel){
	note++; pnotes++;
	key[num][chennel] = vel;
	key2[num][chennel] = 0;
	if (midichennel >= 0x90) key3[num][chennel] = midichennel - 0x90;
	else  key3[num][chennel] = midichennel - 0x80;
}
inline void getmidievent(){
	for (int j = 0; j < chn; j++){
		unsigned int tmp = 0, b = 0;unsigned int c = 0; unsigned long long d = 0;
		//printf("%lld|%lld,%lld,%lld ", mptr, le[j] + le2[j],le[j] , le2[j]);
		if (mptr == le[j] + le2[j] && buf[j] == false){
			
			le[j] += le2[j];
			
			tmp = fgetc2(ch[j],j);
			C2:
			if (tmp >= 0x80 && tmp <= 0x9F){
				oldstate[j] = tmp;
				b = fgetc2(ch[j],j);
				c = fgetc2(ch[j],j);
				if (c != 0) noteon((int)b, c, j,tmp);
				else noteoff(b, j,tmp);
			}
			else if (tmp <= 0x7F) { fseek(ch[j], -1, SEEK_CUR); tmp = oldstate[j]; goto C2; }
			else if (tmp == 0xF0){
				oldstate[j] = tmp;
				while (tmp != 0xF7) tmp = fgetc2(ch[j], j);
			}
			else if (tmp == 0xF6){
				oldstate[j] = tmp;
			}
			else if (tmp == 0xF1 || tmp == 0xF3){
				oldstate[j] = tmp;
				tmp = fgetc2(ch[j], j);
			}
			else if (tmp >= 0xC0 && tmp <= 0xDF){
				oldstate[j] = tmp;
				fgetc2(ch[j], j);
			}
			else if (tmp >= 0xC0 && tmp <= 0xDF){
				oldstate[j] = tmp;
				fgetc2(ch[j],j); 
			}
			else if (tmp >= 0xB0 && tmp <= 0xBF){
				oldstate[j] = tmp;
				c = fgetc2(ch[j], j);
				if (c == 0x27 || c == 0x07) chnvol(fgetc2(ch[j], j), tmp);
				else fgetc2(ch[j], j);
			}
			else if (tmp <0xF4){
				oldstate[j] = tmp;
				fgetc2(ch[j],j); fgetc2(ch[j],j);
			}
			else if (tmp != 0xFF){ oldstate[j] = tmp; }
			else{
				oldstate[j] = tmp;
				b = fgetc2(ch[j], j);
				if (b == 0x2F){
					buf[j] = true; break;
				}
				else if (b == 0x51){
					fgetc2(ch[j], j);
					int2char i2c;
					i2c.c[2] = fgetc2(ch[j], j); i2c.c[1] = fgetc2(ch[j], j); i2c.c[0] = fgetc2(ch[j], j);
					tempo = 60000000 / i2c.i;
				}
				else if (b == 0x59){
					fgetc2(ch[j], j);
					keysig = fgetc2(ch[j], j);
					keysig2 = (bool)fgetc2(ch[j], j);
					keysig += 7;
					if (keysig > 15) keysig &= 0xFF;
				}
				
				else if (b == 0x58){
					fgetc2(ch[j], j);
					bth = fgetc2(ch[j], j);
					btl = fgetc2(ch[j], j);
					if (btl == 0) btl = 1;
					else if (btl == 1)btl = 2;
					else if (btl == 2) btl = 4;
					else if (btl == 3) btl = 8;
					else if (btl == 4)btl = 16;
					else if (btl == 5) btl = 32;
					else if (btl == 6) btl = 64;
					else if (btl == 7)btl = 128;
					else if (btl == 8) btl = 256;
					else if (btl == 9) btl = 512;
					else if (btl == 10)btl = 1024;
					else if (btl == 11) btl = 2048;
					else if (btl == 12) btl = 4196;
					else if (btl == 13) btl = 8192;
					else if (btl == 14) btl = 16384;
					else if (btl == 15) btl = 32768;
					fgetc2(ch[j], j); fgetc2(ch[j], j);
				}
				else{
					tmp = fgetc2(ch[j], j);
					for (int i = 0; i < tmp; i++) fgetc2(ch[j], j);

				}
			}
			while (1){
				tmp = fgetc2(ch[j],j); 
				if (tmp == 0) {
					tmp = fgetc2(ch[j],j);
				C1:
					if (tmp >= 0x80 && tmp <= 0x9F){
						oldstate[j] = tmp;
						b = fgetc2(ch[j],j);
						c = fgetc2(ch[j],j);
						if (c != 0) noteon(b, c, j,tmp);
						else noteoff(b, j,tmp);
					}
					else if (tmp <= 0x7F) { fseek(ch[j], -1, SEEK_CUR); tmp = oldstate[j]; goto C1; }
					else if (tmp == 0xF0){
						oldstate[j] = tmp;
						while (tmp != 0xF7) tmp = fgetc2(ch[j], j);
					}
					else if (tmp == 0xF6){
						oldstate[j] = tmp;
					}
					else if (tmp == 0xF1 || tmp == 0xF3){
						oldstate[j] = tmp;
						tmp = fgetc2(ch[j], j);
					}
					else if (tmp >= 0xC0 && tmp <= 0xDF){
						oldstate[j] = tmp;
						fgetc2(ch[j], j);
					}
					else if (tmp >= 0xC0 && tmp <= 0xDF){
						oldstate[j] = tmp;
						fgetc2(ch[j], j);
					}
					else if (tmp >= 0xB0 && tmp <= 0xBF){
						oldstate[j] = tmp;
						c = fgetc2(ch[j], j);
						if (c == 0x27 || c == 0x07) chnvol(fgetc2(ch[j], j), tmp);
						else fgetc2(ch[j], j);
					}
					else if (tmp <0xF4){
						oldstate[j] = tmp;
						fgetc2(ch[j],j); fgetc2(ch[j],j);
					}
					else if (tmp != 0xFF){ oldstate[j] = tmp; }
					else{
						oldstate[j] = tmp;
						b = fgetc2(ch[j], j);
						if (b == 0x2F){
							buf[j] = true; break;
						}
						else if (b == 0x51){
							fgetc2(ch[j], j);
							int2char i2c;
							i2c.c[2] = fgetc2(ch[j], j); i2c.c[1] = fgetc2(ch[j], j); i2c.c[0] = fgetc2(ch[j], j);
							
							tempo = 60000000 / i2c.i; 
						}
						else if (b == 0x59){
							fgetc2(ch[j], j);
							keysig = fgetc2(ch[j], j);
							keysig2 = (bool)fgetc2(ch[j], j);
							keysig += 7;
							if (keysig > 15) keysig &= 0xFF;
						}
						
						else if (b == 0x58){
							fgetc2(ch[j], j);
							bth = fgetc2(ch[j], j);
							btl = fgetc2(ch[j], j);
							if (btl == 0) btl = 1;
							else if (btl == 1)btl = 2;
							else if (btl == 2) btl = 4;
							else if (btl == 3) btl = 8;
							else if (btl == 4)btl = 16;
							else if (btl == 5) btl = 32;
							else if (btl == 6) btl = 64;
							else if (btl == 7)btl = 128;
							else if (btl == 8) btl = 256;
							else if (btl == 9) btl = 512;
							else if (btl == 10)btl = 1024;
							else if (btl == 11) btl = 2048;
							else if (btl == 12) btl = 4196;
							else if (btl == 13) btl = 8192;
							else if (btl == 14) btl = 16384;
							else if (btl == 15) btl = 32768;
							fgetc2(ch[j], j); fgetc2(ch[j], j);
						}
						else{
							tmp = fgetc2(ch[j], j);
							for (int i = 0; i < tmp; i++) fgetc2(ch[j], j);

						}
					}
				}
				else{
					b = tmp;
					while (1){
						if (b>0x80){
							
							d += b - 0x80;
							b = fgetc2(ch[j],j);
							d *= 128;
						}
						else{
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
inline void getmidievent2(){

	for (int j = 0; j < chn; j++){
		unsigned int tmp = 0; unsigned int b = 0, c = 0; unsigned long long d = 0;
		if (buf[j] == false){
			while (1){
			tmp = fgetc2(ch[j],j);
			if (tmp == 0) {
				C3:
				tmp = fgetc2(ch[j],j);
				
				if (tmp >= 0x80 && tmp <= 0x9F){
					oldstate[j] = tmp;
					b = fgetc2(ch[j],j);
					c = fgetc2(ch[j],j);
					if (c != 0) noteon((int)b, c, j,tmp);
					else noteoff((int)b, j,tmp);
				}
				else if (tmp <= 0x7F) { fseek(ch[j], -1, SEEK_CUR); tmp = oldstate[j]; goto C3; }
				else if (tmp == 0xF0){
					oldstate[j] = tmp;
					while (tmp != 0xF7) tmp = fgetc2(ch[j], j);
				}
				else if (tmp == 0xF6){
					oldstate[j] = tmp;
				}
				else if (tmp == 0xF1 || tmp == 0xF3){
					oldstate[j] = tmp;
					tmp = fgetc2(ch[j], j);
				}
				else if (tmp >= 0xC0 && tmp <= 0xDF){
					oldstate[j] = tmp;
					fgetc2(ch[j], j);
				}
				
				else if (tmp >= 0xB0 && tmp <= 0xBF){
					oldstate[j] = tmp;
					c = fgetc2(ch[j], j);
					if (c == 0x27 || c == 0x07) chnvol(fgetc2(ch[j], j), tmp);
					else fgetc2(ch[j], j);
				}
				else if (tmp <0xF4){
					oldstate[j] = tmp;
					fgetc2(ch[j],j); fgetc2(ch[j],j);
				}
				else if (tmp != 0xFF){ oldstate[j] = tmp; }
				else{
					oldstate[j] = tmp;
					b=fgetc2(ch[j],j);
					if (b == 0x2F){
						buf[j] = true; break;
					}
					else if (b == 0x51){
						fgetc2(ch[j], j);
						int2char i2c;
						i2c.c[2] = fgetc2(ch[j], j); i2c.c[1] = fgetc2(ch[j], j); i2c.c[0] = fgetc2(ch[j], j);
						tempo = 60000000 / i2c.i;
					}
					else if (b == 0x59){
						fgetc2(ch[j], j);
						keysig = fgetc2(ch[j], j);
						keysig2 = (bool)fgetc2(ch[j], j);
						keysig += 7;
						if (keysig > 15) keysig &= 0xFF;
					}
					else if (b == 0x58){
						fgetc2(ch[j], j);
						bth = fgetc2(ch[j], j);
						btl = fgetc2(ch[j], j);
						if (btl == 0) btl = 1;
						else if (btl == 1)btl = 2;
						else if (btl == 2) btl = 4;
						else if (btl == 3) btl = 8;
						else if (btl == 4)btl = 16;
						else if (btl == 5) btl = 32;
						else if (btl == 6) btl = 64;
						else if (btl == 7)btl = 128;
						else if (btl == 8) btl = 256;
						else if (btl == 9) btl = 512;
						else if (btl == 10)btl = 1024;
						else if (btl == 11) btl = 2048;
						else if (btl == 12) btl = 4196;
						else if (btl == 13) btl = 8192;
						else if (btl == 14) btl = 16384;
						else if (btl == 15) btl = 32768;
						fgetc2(ch[j], j); fgetc2(ch[j], j);
					}
					else{
						tmp = fgetc2(ch[j], j);
						for (int i = 0; i < tmp; i++) fgetc2(ch[j], j);
						
					}
				}
			}
			else{
				b = tmp;
				while (1){
					if (b>0x80){
						d += b - 0x80;
						b = fgetc2(ch[j],j);
						d *= 128;
					}
					else{
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
struct str { char s[150]; int t = -1; };
void main2(int argc, char *argv[]){
	SetConsoleTitle(TEXT("하이온의 X같은 검악 플레이어 1.0 베타"));
	FILE *chi;
		
		if (argc != 1) strcpy(t, argv[1]);
		else {
			printf("미디 파일을 드래그한 뒤 엔터를 누르시오\n");
			scanf("%s", t);
		}
		printf("!");
		strcpy(tbk, t); printf("!");
		strcat(tbk, ".wav"); printf(".");
		strcpy(tbk2, tbk); printf("!");
		strcat(tbk2, ".nc"); printf("!");
		strcpy(tbk3, tbk); printf(".");
		strcat(tbk3, ".gc"); printf("!");
		strcpy(tbk4, tbk); printf(".");
		strcat(tbk4, ".nnc"); printf("!");
		FILE *i;
		i = fopen(tbk, "r"); printf("!");
		if (i != NULL) { fclose(i); goto P; }printf(".");
		chi = fopen(t, "rb"); printf("!");
		fseek(chi, 10, SEEK_CUR); printf("!");
		chn = (int)((unsigned char)fgetc(chi)) * 256 + (int)((unsigned char)fgetc(chi)); printf(".");
		deltatime = (int)((unsigned char)fgetc(chi)) * 256 + (int)((unsigned char)fgetc(chi)); printf("!");
		fclose(chi); printf("!");
		le = new long long[chn]; printf(".");
		le2 = new long long[chn]; printf("!");
		fg2 = new int[chn]; printf("!");
		oldstate = new int[chn];
		ch = new FILE*[chn]; printf(".");

		for (int i = 0; i < 128; i++) { key[i] = new int[chn]; key2[i] = new int[chn]; key3[i] = new unsigned char[chn];  }printf("!");
		buf = new bool[chn]; printf("!");
		pitch = new int[chn]; printf(".");
		for (int i = 0; i < chn; i++){
			le[i] = 0; le2[i] = 0; pitch[i] = 1000; fg2[i] = 0; oldstate[i] = 0;
			for (int j = 0; j < 128; j++) { key[j][i] = 0; key2[j][i] = 0; key3[j][i] = 0; }
			buf[i] = 0;
		}printf("!");
		for (int i = 0; i <= chn; i++) { ch[i] = fopen(t, "rb"); fseek(ch[i], 0x12, SEEK_SET); }
		{int2char rt;
		for (int j = 1; j < chn; j++){
			rt.i = 0;
			rt.c[3] = fgetc(ch[j - 1]);
			rt.c[2] = fgetc(ch[j - 1]);
			rt.c[1] = fgetc(ch[j - 1]);
			rt.c[0] = fgetc(ch[j - 1]);
			for (int i = j; i < chn; i++) fseek(ch[i], rt.i + 8, SEEK_CUR);

		}}
		fgetc(ch[chn-1]);
		fgetc(ch[chn - 1]);
		fgetc(ch[chn - 1]);
		fgetc(ch[chn - 1]);
		printf("!");
	if (chn == 0) { printf("에러-잘못된 미디 파일임\n"); Sleep(1000); return; }
	else printf("걍 틀라면 엔터를,피치와 속도를 조절하려면 그 외의 키를 누르시오\n",chn);
	if (getch() != 13){
		printf("피치를 입력하시오\n몇 퍼밀로 돌릴지 입력하면 됨\n피치를 채널마다 모두 다르게 지정하려면 -1을 입력하시오\n");
		scanf("%d", &pitch[0]);
		if (pitch[0]==-1)
		for (int y = 0; y < chn; y++) {
			printf("%d번째 채널:", y+1);
			scanf("%d", &pitch[y]);
		}
		else{
			for (int y = 1; y < chn; y++) pitch[y] = pitch[0];
		}
		printf("재생 속도를 입력하시오\n몇 퍼센트로 돌릴지 입력하면 됨\n");
		scanf("%d", &spd);
		deltatime *= spd;
		deltatime /= 100;
	}
	printf("컴파일 중-얼마나 걸릴지는 모름\n");
	FILE *out,*out2,*out3;
	out = fopen(tbk, "wb");
	out2 = fopen(tbk2, "w");
	out3 = fopen(tbk3, "w");
	fprintf(out3,"%d ", chn);
	for (int i = 0; i < 0x40; i++){
		fputc(opts[i], out);
	}
	getmidievent2();
	//printf("%d %d %d\n", le2[0], le2[1], le2[2]);
	{
		int2char temp2; printf("%lld초만큼 완료-%llu개 음표 인식됨\n", owptr3, note);
		while (1){
			wptr++;
			if (wptr - owptr >= (60 * 44100) / (deltatime * tempo)) {
				mptr++;
				getmidievent();
				for (int i = 0; i < chn; i++){
					if (buf[i] != true) goto A;
				}
				break;
			A:
				owptr = wptr;
			}
			temp2.i = outc();
			if (wptr - owptr2 == 44100){
				owptr3++;
				owptr2 = wptr;
				printf("%lld초만큼 완료-%llu개 음표 인식됨\n", owptr3, note);
			}
			
			if (wptr - owptr4 == 4410){
				
				owptr5--; if (owptr5 == -1){ owptr5 = 9; fprintf(out2, "%u\n", tempo); fprintf(out2, "%d\n", bth); fprintf(out2, "%d\n", btl); fprintf(out2, "%d\n", keysig); fprintf(out2, "%d\n", keysig2); }
				owptr4 = wptr;
				fprintf(out2, "%llu\n", note);
				for (int i = 0; i < chn; i++){
					for (int j = 0; j < 128; j++){
						if (key[j][i] != 0) { fputc(key3[j][i], out3); fputc(key[j][i], out3); }
						else { fputc(0xFF, out3); fputc(0xFF, out3); }
					}
				}
			}
			//fprintf(out, "%c%c%c%c", temp2.c[0], temp2.c[1], temp2.c[2], temp2.c[3]);
			fputc(temp2.c[0], out); fputc(temp2.c[1], out); fputc(temp2.c[2], out); fputc(temp2.c[3], out);
			//printf("%x %x %x %x\n", temp2.c[0],temp2.c[1],temp2.c[2],temp2.c[3]);
		}
		fseek(out, 0x40, SEEK_SET);
		temp2.i = wptr * 4;
		fputc(temp2.c[0], out); fputc(temp2.c[1], out); fputc(temp2.c[2], out); fputc(temp2.c[3], out);
		fclose(out);
		fclose(out2); fclose(out3);

		
		owptr4 = note; 
	}
		/*
		for (int i = 0; i < chn; i++){
			le[i] = 0; le2[i] = 0; pitch[i] = 1000; fg2[i] = 0; 
			for (int j = 0; j < 128; j++) { key[j][i] = 0; key2[j][i] = 0; key3[j][i] = 0; }
			buf[i] = 0;
		}printf("!");*/

	int timers=0;
	FILE *out4;
	out4 = fopen(tbk2, "r");
	owptr4 = note;
	unsigned long long ct2[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	int counterh = 0, counterh2 = wptr / 158760000;
	int counterm = 0, counterm2 = wptr / 2646000 - counterh2 * 60;
	double counter = 0, counter2 = ((double)wptr) / 44100 - counterm2 * 60 - counterh2 * 360;
	int iiii = 0; unsigned long long owptr23 = 0;
	FILE *out5;
	out5 = fopen(tbk4, "w");
	while (1){
		if (counter >= 60.0){
			counter -= 60;
			counterm++;
		}
		if (counterm == 60){
			counterm -= 60;
			counterh++;
		}
		

		if (timers % 10 == 0){ fscanf(out4, "%u", &tempo); fscanf(out4, "%d", &bth); fscanf(out4, "%d", &btl); fscanf(out4, "%d", &keysig);fscanf(out4,"%d" , &keysig2); }
		if (fscanf(out4, "%llu", &note) == EOF) break;
		for (int i = 0; i < 9; i++){
			ct2[i] = ct2[i + 1];
		}
		ct2[9] = note;
		fprintf(out5, "%d:%d:%.1f/%d:%d:%.1f\n%llu/%llu\n%llu개/초\n템포%u,박자%d/%d\n%s\n", counterh2, counterm2, counter2, counterh, counterm, counter, owptr4, note, note-ct2[0], tempo, bth, btl, keysigm[keysig2][keysig]);
		counter += 0.1;
		timers++;
		if (timers % 1000 == 0) printf("마무리 작업 중:%d초만큼 완료\n", timers / 10);
	}fclose(out4);
	fclose(out5);
	printf("작성 완료\n");
	system("pause");
	//////
P:
	FILE *out6;
	out6 = fopen(tbk4, "rb");
	int t2imers = 0;
	ShellExecuteA(0, "open", tbk, 0, 0, 0);
	Sleep(300);
	int ii = clock();
	
	while (1){
		if (clock() - ii >= 100) {  system("pause"); return; }
		while (clock() - ii < 100){}
		ii = clock();
		system("cls");
		if (fscanf(out6, "%s", tbk2) == EOF) break;
		printf("%s\n", tbk2);
		if (fscanf(out6, "%s", tbk2) == EOF) break;
		printf("%s\n", tbk2);
		if (fscanf(out6, "%s", tbk2) == EOF) break;
		printf("%s\n", tbk2);
		if (fscanf(out6, "%s", tbk2) == EOF) break;
		printf("%s\n", tbk2);
		if (fscanf(out6, "%s", tbk2) == EOF) break;
		printf("%s\n", tbk2);
	}
		Sleep(200);
	system("pause");
	fclose(out6);
}
#endif