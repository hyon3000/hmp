
#ifndef SOUNDFONTCPP
#define SOUNDFONTCPP
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <math.h>
#include <omp.h>
#define ontimeresample( val)  (valtt = (unsigned long)val,valt = val-(double)valtt,valt*smpl[valtt + 1] - (valt - 1)*smpl[valtt])
inline double pow10(double b);
struct pgen;
struct igen;
struct _gen;
class parsetmpgen;
class sample;
struct inst;
struct preset;
struct sfont;
struct _gen {
	long samplestartoffset = 0, sampleendoffset = 0, startloopoffset = 0, endloopoffset = 0, loopplayback = -1, rootkey = -1, tuningsemitones = INT_MIN, tuningcents = INT_MIN, tuningscale = INT_MIN, keyvolenvholdtmp = -1, keyvolenvdecaytmp = -1, volenvholdtmp = -1, volenvdecaytmp = -1, modenvholdtmp = -1, modenvdecaytmp = -1, keymodenvholdtmp = -1, fixedkey = -1, fixedvelocity = -1, exclusiveclass = -1, keymodenvdecaytmp = -1;
	char keyrangehi = -1, keyrangelo = -1, velocityrangehi = -1, velocityrangelo = -1;
	double attenuation = -1, filterfrequency = -1, filterresonance = 10e306, volenvdelay = -1, volenvattack = -1, volenvsustain = -1, volenvrelease = -1, modlfodelay = -1, modlfofreq = -1, modlfopitch = 0, modlfovolume = -1, modlfofilter = 0, modenvdelay = -1, modenvattack = -1, modenvsustain = -1, modenvrelease = -1, modenvpitch = 0, modenvfilter = 0, chorus = 10e306, reverb = 10e306, pan = 10e306, viblfodelay = -1, viblfofreq = -1, viblfopitch = 0;
};
class sample {
public:
	char achSampleName[20];
	long dwSampleRate = -1;
	char byOriginalPitch = -1;//unused
	short sfSampleType;//0:both 1:left 2:right
	long dwStart = 0;
	long dwEnd = 0;
	long dwStartloop = 0;
	long dwEndloop = 0;
	char chPitchCorrection = 0;
	double *smpl;
	BOOL loaded = 0;
};
struct igen :public _gen {
	int sampletmp = -1;
	sample *smpl;
};
struct pgen :public _gen {
	int insttmp = -1;
	inst *inst;
};
struct inst {
	char instname[20];
	igen *igen;
	unsigned short *igenptr;
	int igencount;
};
struct preset {
	int _bank,_preset,pgencount;
	char presetname[20];
	pgen* pgen;
	unsigned short *pgenptr;
};
struct sfont {
	preset* preset;
	inst* inst;
	sample *smpl;
	unsigned short *presetptr, *instptr;
	int presetcount, instcount, samplecount;
};
union fp{double d;int x[2];};
union genAmountType {
	short wordval;
	char byteval[2];
};
fp fastpow; 
sfont soundfont;
FILE *sf;
unsigned int soundfontcppsiz, tmpsize;
int bfrsize,bfsd50,bf2, aptr, *sineval, getboth_righttmp, aptrc1, aptrc2, aptrc3, aptrrv, chkytmp;
char volumeptrcount[2048][2], volumeptr2[2048][128][2], volumeptr3[2048][2], volumeptr3mono[2048];
double 
*poutbuffer[16],*pcoutbuffer[16]
,aptrp, getbotht, readtemp, getlefttmp,getrighttmp, 

outbuffer[16];
long smplptr;
class parsetmpgen :public _gen {
public:
	unsigned long valtt;
	double valt;
	long veld,releasereserved = 0, key, vel, lr, dwSampleRate = -1, enablefilter, noteon = 0, noteoff2 = 1, ly = 0, looplength2 = 1, midichennel;
	double volenvhold = -1, volenvdecay = -1, modenvhold = -1, modenvdecay = -1, pitch = 1, volume = 1, outsamplerate, volenvattack2, volenvhold2, volenvdecay2, modenvattack2, modenvhold2, modenvdecay2, sustainvol, volenvsustaindvolenvdecayd20, volenvsustaindvolenvdecayd202, volenvsustaindvolenvdecayd202p, sustainmod, modenvsustaindmodenvdecayd20, modenvsustaindmodenvdecayd202, modenvsustaindmodenvdecayd202p, deltasecond, dsamplestartoffset, dsampleendoffset, dstartloopoffset, dendloopoffset, sustain2, outtmp, outtmpt, outtmp2, secondtmp, *smpl, centtmp, ptr, timeptr, modwh, portam, loadedvolume,lvatt,lvsus, attvol, w0, a0, a1, a2, b0, b1, b2, f0 = 0, fm1 = 0, fm2 = 0, setuplength, looplength = 0.;
	char achSampleName[20], instname[20], lrlo = -1, lrhi = -1;
	unsigned long reg = 0;
	void reset() {

		releasereserved = 0;
		lrlo = -1;
		lrhi = -1;

		samplestartoffset = 0;
		sampleendoffset = 0;
		startloopoffset = 0;
		endloopoffset = 0;

		loopplayback = -1;//sampleModes 0:noloop 1:loop
		keyrangehi = -1;
		keyrangelo = -1;
		velocityrangehi = -1;
		velocityrangelo = -1;
		attenuation = 10e306;

		rootkey = -1;
		tuningsemitones = INT_MIN;
		tuningcents = INT_MIN;
		tuningscale = INT_MIN;

		filterfrequency = -1;//hz
		filterresonance = 10e306;//dB

		volenvdelay = -1;
		volenvattack = -1;
		volenvholdtmp = -1;
		volenvdecaytmp = -1;
		volenvsustain = -1;
		volenvrelease = -1;
		keyvolenvholdtmp = -1;
		keyvolenvdecaytmp = -1;

		modlfodelay = -1;//sec
		modlfofreq = -1;//Hz
		modlfopitch = 0;//속도 변경
		modlfovolume = -1;
		modlfofilter = 0;

		modenvdelay = -1;
		modenvattack = -1;
		modenvholdtmp = -1;
		modenvdecaytmp = -1;
		modenvsustain = -1; //0~1
		modenvrelease = -1;
		keymodenvholdtmp = -1;
		keymodenvdecaytmp = -1;
		modenvpitch = 0;
		modenvfilter = 0;//x

		viblfodelay = -1;
		viblfofreq = -1;
		viblfopitch = 0;

		exclusiveclass = -1;//x
		chorus = 10e306;//x
		reverb = 10e306;//x
		pan = 10e306;//x,stereo only

		fixedkey = -1;
		fixedvelocity = -1;
	}
	void loadpgen(pgen p, int pgenval) {
		strcpy(instname, (p.inst)->instname);
		reg = pgenval + 1;



		if (p.keyrangehi != -1)keyrangehi = p.keyrangehi;
		if (p.keyrangelo != -1)keyrangelo = p.keyrangelo;
		if (p.velocityrangehi != -1)velocityrangehi = p.velocityrangehi;
		if (p.velocityrangelo != -1)velocityrangelo = p.velocityrangelo;
		if (p.attenuation != 10e306)attenuation = p.attenuation;


		if (p.tuningsemitones != INT_MIN)tuningsemitones = p.tuningsemitones;
		if (p.tuningcents != INT_MIN)tuningcents = p.tuningcents;
		if (p.tuningscale != INT_MIN)tuningscale = p.tuningscale;

		if (p.chorus != 10e306)chorus = p.chorus;
		if (p.reverb != 10e306)reverb = p.reverb;
		if (p.pan != 10e306)pan = p.pan;


	}
	void loadigen(igen p, int igenval) {
		strcpy(achSampleName, ((p.smpl)->achSampleName));

		reg |= ((unsigned long)igenval << 16);
		if (p.samplestartoffset != 0) samplestartoffset += p.samplestartoffset;
		if (p.sampleendoffset != 0)sampleendoffset += p.sampleendoffset;
		if (p.startloopoffset != 0)startloopoffset += p.startloopoffset;
		if (p.endloopoffset != 0)endloopoffset += p.endloopoffset;

		if (p.loopplayback != -1)loopplayback = p.loopplayback;
		if (p.keyrangehi != -1)keyrangehi = p.keyrangehi;
		if (p.keyrangelo != -1)keyrangelo = p.keyrangelo;
		if (p.velocityrangehi != -1)velocityrangehi = p.velocityrangehi;
		if (p.velocityrangelo != -1)velocityrangelo = p.velocityrangelo;
		if (p.attenuation != 10e306)attenuation = p.attenuation;

		if (p.rootkey != -1)rootkey = p.rootkey;
		if (p.tuningsemitones != INT_MIN)tuningsemitones = p.tuningsemitones;
		if (p.tuningcents != INT_MIN)tuningcents = p.tuningcents;
		if (p.tuningscale != INT_MIN)tuningscale = p.tuningscale;

		if (p.filterfrequency != -1)filterfrequency = p.filterfrequency;
		if (p.filterresonance != 10e306)filterresonance = p.filterresonance;

		if (p.volenvdelay != -1)volenvdelay = p.volenvdelay;
		if (p.volenvattack != -1)volenvattack = p.volenvattack;
		if (p.volenvholdtmp != -1)volenvholdtmp = p.volenvholdtmp;
		if (p.volenvdecaytmp != -1)volenvdecaytmp = p.volenvdecaytmp;
		if (p.volenvsustain != -1)volenvsustain = p.volenvsustain;
		if (p.volenvrelease != -1)volenvrelease = p.volenvrelease;
		if (p.keyvolenvholdtmp != -1)keyvolenvholdtmp = p.keyvolenvholdtmp;
		if (p.keyvolenvdecaytmp != -1)keyvolenvdecaytmp = p.keyvolenvdecaytmp;

		if (p.modlfodelay != -1)modlfodelay = p.modlfodelay;
		if (p.modlfofreq != -1)modlfofreq = p.modlfofreq;
		if (p.modlfopitch != 0)modlfopitch = p.modlfopitch;
		if (p.modlfovolume != -1)modlfovolume = p.modlfovolume;
		if (p.modlfofilter != 0)modlfofilter = p.modlfofilter;

		if (p.modenvdelay != -1)modenvdelay = p.modenvdelay;
		if (p.modenvattack != -1)modenvattack = p.modenvattack;
		if (p.modenvholdtmp != -1)modenvholdtmp = p.modenvholdtmp;
		if (p.modenvdecaytmp != -1)modenvdecaytmp = p.modenvdecaytmp;
		if (p.modenvsustain != -1)modenvsustain = p.modenvsustain;
		if (p.modenvrelease != -1)modenvrelease = p.modenvrelease;
		if (p.keymodenvholdtmp != -1)keymodenvholdtmp = p.keymodenvholdtmp;
		if (p.keymodenvdecaytmp != -1)keymodenvdecaytmp = p.keymodenvdecaytmp;
		if (p.modenvpitch != 0)modenvpitch = p.modenvpitch;
		if (p.modenvfilter != 0)modenvfilter = p.modenvfilter;

		if (p.viblfodelay != -1)viblfodelay = p.viblfodelay;
		if (p.viblfofreq != -1)viblfofreq = p.viblfofreq;
		if (p.viblfopitch != 0)viblfopitch = p.viblfopitch;

		if (p.exclusiveclass != -1)exclusiveclass = p.exclusiveclass;
		if (p.chorus != 10e306)chorus = p.chorus;
		if (p.reverb != 10e306)reverb = p.reverb;
		if (p.pan != 10e306)pan = p.pan;

		if (p.fixedkey != -1)fixedkey = p.fixedkey;
		if (p.fixedvelocity != -1)fixedvelocity = p.fixedvelocity;


		if ((p.smpl)->loaded == 0) {
			(p.smpl)->loaded = 1;
			int j = (p.smpl)->dwEnd - (p.smpl)->dwStart + 2;
			(p.smpl)->smpl = new double[j];
			short *smpltmp = new short[j];
			fseek(sf, smplptr + 2 * (p.smpl)->dwStart, SEEK_SET);
			fread(smpltmp, 2, j, sf);

			for (int i = 0; i < j; i++) {
				(p.smpl)->smpl[i] = (double)smpltmp[i];
			}

			delete[]smpltmp;

		}
		smpl = (p.smpl)->smpl;
		//if ((p.smpl)->dwStart != 0) samplestartoffset += (p.smpl)->dwStart;
		if ((p.smpl)->dwEnd != 0) sampleendoffset += ((p.smpl)->dwEnd) - ((p.smpl)->dwStart);
		if ((p.smpl)->dwStartloop != 0) startloopoffset += ((p.smpl)->dwStartloop) - ((p.smpl)->dwStart);
		if ((p.smpl)->dwEndloop != 0) endloopoffset += ((p.smpl)->dwEndloop) - ((p.smpl)->dwStart);
		if (rootkey == -1) rootkey = (p.smpl)->byOriginalPitch;
		dwSampleRate = (p.smpl)->dwSampleRate;
		if ((p.smpl)->chPitchCorrection != 0) {
			if (tuningcents == INT_MIN)tuningcents = 0;
			tuningcents += (p.smpl)->chPitchCorrection;
		}
		if (pan == 10e306)pan = 0;
		if (pan < -25.) { lrlo = 0; lrhi = 0; }
		else if (pan >25.) { lrlo = 1; lrhi = 1; }
		else { lrlo = 0; lrhi = 1; }

		if (keyrangehi == -1)keyrangehi = 127;
		if (keyrangelo == -1)keyrangelo = 0;
		if (velocityrangehi == -1)velocityrangehi = 127;
		if (velocityrangelo == -1)velocityrangelo = 0;

	}
	void loaddefault(int keyi, int veli, int lri, int outoutsamplerate2) {
		outsamplerate = (double)outoutsamplerate2;
		deltasecond = 1. / outsamplerate;
		key = keyi; vel = veli; lr = lri;
		if (loopplayback == -1)loopplayback = 0;

		if (attenuation == 10e306)attenuation = 0;

		if (rootkey == -1)rootkey = 60;
		if (tuningsemitones == INT_MIN)tuningsemitones = 0;
		if (tuningcents == INT_MIN)tuningcents = 0;
		if (tuningscale == INT_MIN)tuningscale = 100;


		if (filterresonance == 10e306)filterresonance = 0;

		if (volenvdelay == -1)volenvdelay = 0.001;
		if (volenvattack == -1)volenvattack = 0.001;
		if (volenvholdtmp == -1)volenvholdtmp = -12000;
		if (volenvdecaytmp == -1)volenvdecaytmp = -12000;
		if (volenvrelease == -1)volenvrelease = 1;
		if (volenvsustain == -1) volenvsustain = 0;
		if (keyvolenvholdtmp == -1)keyvolenvholdtmp = 0;
		if (keyvolenvdecaytmp == -1)keyvolenvdecaytmp = 0;

		if (modlfodelay == -1)modlfodelay = 0.001;
		if (modlfofreq == -1)modlfofreq = 8.176;
		if (modlfovolume == -1)modlfovolume = 0;

		if (modenvdelay == -1)modenvdelay = 0.001;
		if (modenvattack == -1)modenvattack = 0.001;
		if (modenvholdtmp == -1)modenvholdtmp = -12000;
		if (modenvdecaytmp == -1)modenvdecaytmp = -12000;
		if (modenvsustain == -1)modenvsustain = 1;
		if (modenvrelease == -1)modenvrelease = 0.001;
		if (keymodenvholdtmp == -1)keymodenvholdtmp = 0;
		if (keymodenvdecaytmp == -1)keymodenvdecaytmp = 0;

		if (viblfodelay == -1)viblfodelay = 0.001;
		if (viblfofreq == -1)viblfofreq = 8.176;

		if (exclusiveclass == -1)exclusiveclass = 0;
		if (chorus == 10e306)chorus = 0;
		if (reverb == 10e306)reverb = 0;

		if (lrlo == -1) {
			lrlo = 0; lrhi = 1;
		}
		attenuation += filterresonance;
		//printf("%lf ", attenuation);
		attvol = pow(2, -(attenuation - 15.) / 6.0205999132796239042747778944899);
		dsamplestartoffset = (double)samplestartoffset;
		dsampleendoffset = (double)sampleendoffset;
		dstartloopoffset = (double)startloopoffset;
		dendloopoffset = (double)endloopoffset;
		modenvhold = pow(2., (double)(modenvholdtmp + (60 - key)*keymodenvholdtmp) / 1200.);
		modenvdecay = pow(2., (double)(modenvdecaytmp + (60 - key)*keymodenvdecaytmp) / 1200.);
		volenvhold = pow(2., (double)(volenvholdtmp + (60 - key)*keyvolenvholdtmp) / 1200.);
		volenvdecay = pow(2., (double)(volenvdecaytmp + (60 - key)*keyvolenvdecaytmp) / 1200.);

		pitch = (double)dwSampleRate / outsamplerate*pow(2., (
			(double)tuningcents / 100.
			+ (double)tuningsemitones
			+ (double)(((fixedkey != -1) ? fixedkey : key) - rootkey)*(double)tuningscale / 100.
			) / 12.
		);

		volume = 1.
			*pow(10, -(double)0. / 20.)
			*(0.5 - (lr ? (-pan) : (pan)) / 100.)
			;
		if (loopplayback == 0) {
			setuplength =
				dsampleendoffset - dsamplestartoffset + 1.;
			looplength = 1.;
			dstartloopoffset = dsampleendoffset - 1;
			dendloopoffset = dsampleendoffset;
		}
		else {
			setuplength = dstartloopoffset - dsamplestartoffset + 1.;
			looplength = dendloopoffset - dstartloopoffset + 1.;
			dsampleendoffset = dendloopoffset;
		}
		enablefilter = (filterfrequency != -1.);
		filterresonance = pow(10., filterresonance / 20.);
		volenvattack2 = volenvdelay + volenvattack;
		volenvhold2 = volenvdelay + volenvattack + volenvhold;
		volenvdecay2 = volenvdelay + volenvattack + volenvhold + volenvdecay;
		modenvattack2 = modenvdelay + modenvattack;
		modenvhold2 = modenvdelay + modenvattack + modenvhold;
		modenvdecay2 = modenvdelay + modenvattack + modenvhold + modenvdecay;

		sustainvol = pow(10, -volenvsustain / 20.);
		volenvsustaindvolenvdecayd20 = volenvsustain / volenvdecay / -20.;
		volenvsustaindvolenvdecayd202 = -7. / volenvrelease;

		if (modenvsustain < 0.00001)modenvsustain = 0.00001;
		sustainmod = modenvsustain;
		modenvsustain = 20 * log10(modenvsustain);
		modenvsustaindmodenvdecayd20 = modenvsustain / modenvdecay / 20.;
		modenvsustaindmodenvdecayd202 = (modenvsustain - 100) / modenvrelease / 20.;
		modenvsustaindmodenvdecayd202p = modenvsustain / 20.;
		if (enablefilter) {
			w0 = 6.283185307179586476925286766559*filterfrequency;
			a0 = 4 * outsamplerate*outsamplerate + (2 * w0*outsamplerate) / (filterresonance)+w0*w0;
			a1 = (-8 * outsamplerate*outsamplerate + 2 * w0*w0) / a0;
			a2 = (4 * outsamplerate*outsamplerate - 2 * w0* outsamplerate / (filterresonance)+w0*w0) / a0;
			b0 = (w0*w0) / a0;
			b1 = 2 * b0;
			b2 = b0;
		}
		modlfofreq *= 6.283185307179586476925286766559;
		viblfofreq *= 6.283185307179586476925286766559;
		looplength2 = (looplength == 0);
		//modenvpitcht = (modenvpitch != 0);
		if (looplength >= 2)looplength--;
	}
	void loaddefault10() {
		if (key == 72) volenvrelease = 3.;
		else if (key == 80 || key == 86) volenvrelease = 0.5;
		else volenvrelease = 2.;
		volenvsustaindvolenvdecayd202 = -7. / volenvrelease;

		ly = 1;

	}
	inline void setup(int midichennel) {
		this->midichennel = midichennel;
	}
	inline void set(double volumei,int vd) {
		veld = vd;
		sustain2 = 0.;
		releasereserved = 0;
		loadedvolume = volumei*volume*attvol;
		lvatt = loadedvolume / volenvattack;
			lvsus = loadedvolume *sustainvol;
		ptr = dsamplestartoffset;
		timeptr = 0;
		noteon = 1;
		noteoff2 = 0;
	}

	inline double readval1() {

		if (noteoff2)return 0.;

		if (looplength2)return 0.;

		if (ly)
			if (timeptr > volenvdecay2&&noteon) {
				secondtmp = timeptr;
				noteon = 0;
			}



		timeptr += deltasecond;

		outtmp = pitch*pitcherr[key][midichennel];
		if (enablefilter) { outtmpt = filterfrequency; }
		if (modenvpitch != 0. || modenvfilter != 0.) {
			if (noteon) {
				if (timeptr < modenvdelay) centtmp = 0.;
				else if (timeptr < modenvattack2) centtmp = (timeptr - modenvdelay) / modenvattack;
				else if (timeptr < modenvhold2)centtmp = 1.;
				else if (timeptr < modenvdecay2) centtmp = pow(10, modenvsustaindmodenvdecayd20*(timeptr - modenvhold2));
				else centtmp = sustainmod;
			}
			else {

				centtmp = sustainmod*pow(10, modenvsustaindmodenvdecayd202*(timeptr - secondtmp));

			}
			outtmp *= pow(2, modenvpitch*centtmp);
			if (enablefilter)outtmpt *= pow(2, modenvfilter*centtmp);
		}
		if (modlfopitch != 0. || modlfofilter != 0.) {
			if (timeptr > modlfodelay) {

				outtmp2 = sin(modlfofreq*(timeptr - modlfodelay));
				outtmp *= pow(2., modlfopitch*outtmp2);
				if (enablefilter)outtmpt *= pow(2., modlfofilter*outtmp2);
			}

		}
		if (viblfopitch != 0.) {
			if (timeptr > viblfodelay) {
				outtmp *= pow(2., viblfopitch*sin(viblfofreq*(timeptr - viblfodelay)));
			}

		}
		if (enablefilter && (modenvfilter != 0. || modlfofilter != 0.)) {

			w0 = 6.283185307179586476925286766559*outtmpt;
			a0 = 4 * outsamplerate*outsamplerate + (2 * w0*outsamplerate) / (filterresonance)+w0*w0;
			a1 = (-8 * outsamplerate*outsamplerate + 2 * w0*w0) / a0;
			a2 = (4 * outsamplerate*outsamplerate - 2 * w0* outsamplerate / (filterresonance)+w0*w0) / a0;
			b0 = (w0*w0) / a0;
			b1 = 2 * b0;
			b2 = b0;
		}
		ptr += outtmp;
		while (ptr >= dendloopoffset)ptr -= looplength;

		if (noteon) {
			if (timeptr < volenvdelay) {
				if (enablefilter) {
					outtmp = 0.;
				}
				else return 0.;

			}
			else if (timeptr < volenvattack2) outtmp = loadedvolume*(timeptr - volenvdelay) / volenvattack;
			else if (timeptr < volenvhold2)outtmp = loadedvolume;
			else if (timeptr < volenvdecay2) outtmp = loadedvolume*pow10(volenvsustaindvolenvdecayd20*(timeptr - volenvhold2));
			else outtmp = loadedvolume*sustainvol;
		}
		else {

			outtmp = loadedvolume* pow10(volenvsustaindvolenvdecayd202p + volenvsustaindvolenvdecayd202*(timeptr - secondtmp));
			if (outtmp < 0.00001) noteoff2 = 1;

		}
		if (modlfovolume != 0.) {
			if (timeptr > modlfodelay) {
				outtmp2 = sin(modlfofreq*(timeptr - modlfodelay));
				outtmp2 = modlfovolume*(outtmp2 > 0. ? 0. : outtmp2);

				outtmp *= pow10(outtmp2 / 20.);
			}

		}

		outtmp *= chvol[midichennel];
		if (enablefilter)return outtmp*
			(fm2 = fm1, fm1 = f0, f0 = ontimeresample(ptr) - a1*fm1 - a2*fm2, b0*f0 + b1*fm1 + b2*fm2);
		else
			return outtmp*ontimeresample(ptr);
	}
	fp fastpow;
	inline double pow10(double b) {
		fastpow.d = 10.;
		fastpow.x[1] = (int)(b * (fastpow.x[1] - 1072632447) + 1072632447);
		fastpow.x[0] = 0;
		return fastpow.d;
	}
	inline double readval() {

		if (reg);
		else return 0.;
		if (noteoff2)return 0.;

		if (looplength2)return 0.;

		if (ly) {

			if (noteon&&timeptr > volenvattack2 + 0.1) {
				secondtmp = timeptr;
				noteon = 0;
			}
		}



		timeptr += deltasecond;
		//0.924450 1.000000 91 0 92

		outtmp = pitch*pitcherr[key][midichennel];
		//printf("%lf ", outtmp);
		//printf("%lf %lf %d %d %d %d\n", pitch, pitcherr[key][midichennel],tuningcents, key, midichennel,rootkey);
		if (enablefilter) { outtmpt = filterfrequency; }
		if (modenvpitch != 0. || modenvfilter != 0.) {
			if (noteon) {
				if (timeptr < modenvdelay) centtmp = 0.;
				else if (timeptr < modenvattack2) centtmp = (timeptr - modenvdelay) / modenvattack;
				else if (timeptr < modenvhold2)centtmp = 1.;
				else if (timeptr < modenvdecay2) centtmp = pow(10, modenvsustaindmodenvdecayd20*(timeptr - modenvhold2));
				else centtmp = sustainmod;
			}
			else {

				centtmp = sustainmod*pow(10, modenvsustaindmodenvdecayd202*(timeptr - secondtmp));

			}
			outtmp *= pow(2, modenvpitch*centtmp);
			if (enablefilter)outtmpt *= pow(2, modenvfilter*centtmp);
		}
		if (modlfopitch != 0. || modlfofilter != 0.) {
			if (timeptr > modlfodelay) {

				outtmp2 = sin(modlfofreq*(timeptr - modlfodelay));
				outtmp *= pow(2., modlfopitch*outtmp2);
				if (enablefilter)outtmpt *= pow(2., modlfofilter*outtmp2);
			}

		}
		if (viblfopitch != 0.) {
			if (timeptr > viblfodelay) {
				outtmp *= pow(2., viblfopitch*sin(viblfofreq*(timeptr - viblfodelay)));
			}

		}
		if (modulationwheel[midichennel]) {
			//	system("pause");
			outtmp *= pow(2., rmodulationwheel[midichennel] * 0.24*sin(25.132741228718345907701147066236*(timeptr)));


		}
		if (enablefilter && (modenvfilter != 0. || modlfofilter != 0.)) {

			w0 = 6.283185307179586476925286766559*outtmpt;
			a0 = 4 * outsamplerate*outsamplerate + (2 * w0*outsamplerate) / (filterresonance)+w0*w0;
			a1 = (-8 * outsamplerate*outsamplerate + 2 * w0*w0) / a0;
			a2 = (4 * outsamplerate*outsamplerate - 2 * w0* outsamplerate / (filterresonance)+w0*w0) / a0;
			b0 = (w0*w0) / a0;
			b1 = 2 * b0;
			b2 = b0;
		}
		//printf("%lf\n", outtmp);
		ptr += outtmp;
		//	ptr += 1.;
		//printf("%lf %lf %lf\n", ptr, dendloopoffset, looplength);

		while (ptr > dendloopoffset) {
			ptr -= looplength;
			if (!(ptr >= -DBL_MAX) || ptr>DBL_MAX || ptr<-DBL_MAX) return 0.;
		}

		//return 100000*ontimeresample(ptr);
		if (noteon) {
			//printf("%lf %lf %lf %lf %lf %lf\n", dsamplestartoffset, dsampleendoffset, dstartloopoffset, dendloopoffset,looplength,ptr);
			//system("pause");
			
			if (timeptr < volenvdelay) {
				if (enablefilter) {
					outtmp = 0., sustain2 = outtmp;
				}
				else return 0.;

			}
			else if (timeptr < volenvattack2) outtmp = lvatt*chvol[midichennel] * (timeptr - volenvdelay) , sustain2 = outtmp;

			else {
				//printf("%lf\n",timeptr);
				//Sleep(3);
				
				if (timeptr < volenvhold2)outtmp = loadedvolume*chvol[midichennel], sustain2 = outtmp;
				else if (timeptr < volenvdecay2)outtmp = loadedvolume*chvol[midichennel] * pow10(volenvsustaindvolenvdecayd20*(timeptr - volenvhold2)), sustain2 = outtmp;
				else outtmp = lvsus*chvol[midichennel], sustain2 = outtmp;
				if (releasereserved) {
					//printf("/"); system("pause");
					releasereserved = 0;
					secondtmp = timeptr;
					noteon = 0;
					if (timeptr < 1 || veld<24) sustain2 = 0;
				}
			}
			if (releasereserved&&midichennel != 9) {
				//printf("/"); system("pause");
				releasereserved = 0;
				secondtmp = timeptr;
				noteon = 0; if (timeptr < 1 || veld<24) sustain2 = 0;
			}
		}
		else {

			//system("pause");
			outtmp =  sustain2* pow10(volenvsustaindvolenvdecayd202*(timeptr - secondtmp));

			if (outtmp < 0.00001)
				noteoff2 = 1;

		}
		if (modlfovolume != 0.) {
			if (timeptr > modlfodelay) {
				outtmp2 = sin(modlfofreq*(timeptr - modlfodelay));
				outtmp2 = modlfovolume*(outtmp2 > 0. ? 0. : outtmp2);

				outtmp *= pow10(outtmp2 / 20.);
			}

		}
		//if (ly) outtmp = loadedvolume;
		//printf("%lf %lf\n", sustain2, outtmp);
		//Sleep(10);
		
		if (enablefilter)return outtmp*
			(fm2 = fm1, fm1 = f0, f0 = ontimeresample(ptr) - a1*fm1 - a2*fm2, b0*f0 + b1*fm1 + b2*fm2);
		else
			return outtmp*ontimeresample(ptr);
	}
	inline void release() {
		releasereserved = 1;

	}
};
parsetmpgen parsetmp[2048][128][2];
inline double fastPow(double a, double b) {
	fastpow.d = a;
	fastpow.x[1] = (int)(b * (fastpow.x[1] - 1072632447) + 1072632447);
	fastpow.x[0] = 0;
	return fastpow.d;
}
inline double pow10(double b) {
	fastpow.d = 10.;
	fastpow.x[1] = (int)(b * (fastpow.x[1] - 1072632447) + 1072632447);
	fastpow.x[0] = 0;
	return fastpow.d;
}
void resample(short* in, short* out, long long insize, long long inrate, long long outrate) {
	long long o = insize *outrate / inrate - 1, j = 0, t = ((insize - 2) << 16) / (o - 1);

	for (long long i = 0; ++i < o; j += t) {

		out[i] = ((j & 65535)*in[(j >> 16) + 1] + (65536 - (j & 0xffff))*in[(j >> 16)]) >> 16;

	}

}
void shiftpitch2(double pitch, long size, long fftsize, long overlap, double srate, double *in, double *out) {
	double *gi = (double*)malloc(344072 * 8), *go = gi + 8192 * 4, *gf = gi + 16384 * 4, *gl = gi + 32768 * 4,
		*gs = gi + 34817 * 4, *gu = gi + 36866 * 4, *ga = gi + 53250 * 4,
		*gn = gi + 61442 * 4, *gy = gi + 69634 * 4, *gm = gi + 77826 * 4,
		wr, wi, arg, *p1, *p2, tp, tr, ti, ur, ui, *p1r, *p1i, *p2r, *p2i;
	memset(gi, 0, 344072 * 8);
	double ma, ph, t1, wd, rl, ig, fb, ex;
	long gr = 0, i1, k1, qpd, i3, il, ss, f2, i2, bt, j2, le, le2, k2;
	f2 = fftsize / 2;
	ss = fftsize / overlap;
	fb = srate / (double)fftsize;
	ex = 6.28318530717958647692*(double)ss / (double)fftsize;
	il = fftsize - ss;
	gr = il;
	for (i1 = 0; i1 < size; i1++) {
		gi[gr] = in[i1];
		out[i1] = go[gr - il];
		gr++;
		if (gr >= fftsize) {
			gr = il; for (k1 = 0; k1 < fftsize; k1++) {
				wd = -.5*cos(6.28318530717958647692*(double)k1 / (double)fftsize) + 0.5;
				gf[2 * k1] = gi[k1] * wd;
				gf[2 * k1 + 1] = 0.;
			}
			for (i2 = 2; i2 < 2 * fftsize - 2; i2 += 2) {
				for (bt = 2, j2 = 0; bt < 2 * fftsize; bt <<= 1) {
					if (i2 & bt) j2++;
					j2 <<= 1;
				}
				if (i2 < j2)
				{
					p1 = gf + i2; p2 = gf + j2;
					tp = *p1; *(p1++) = *p2; *(p2++) = tp;
					tp = *p1; *p1 = *p2; *p2 = tp;
				}
			}
			for (k2 = 0, le = 2; k2 < (long)(log(fftsize) / 0.693147180559945309417 + 0.5); k2++) {
				le <<= 1;
				le2 = le >> 1;
				ur = 1.0;
				ui = 0.0;
				arg = 3.14159265358979323846 / (le2 >> 1);
				wr = cos(arg);
				wi = -sin(arg);
				for (j2 = 0; j2 < le2; j2 += 2) {
					p1r = gf + j2;
					p1i = p1r + 1;
					p2r = p1r + le2;
					p2i = p2r + 1;
					for (i2 = j2; i2 < 2 * fftsize; i2 += le) {
						tr = *p2r * ur - *p2i * ui;
						ti = *p2r * ui + *p2i * ur;
						*p2r = *p1r - tr;
						*p2i = *p1i - ti;
						*p1r += tr;
						*p1i += ti;
						p1r += le;
						p1i += le; p2r += le; p2i += le;
					}
					tr = ur*wr - ui*wi;
					ui = ur*wi + ui*wr;
					ur = tr;
				}
			}
			for (k1 = 0; k1 <= f2; k1++) {
				rl = gf[2 * k1];
				ig = gf[2 * k1 + 1];
				ma = 2.*sqrt(rl*rl + ig*ig);
				ph = (ig == 0.) ? 0. : ((rl == 0.) ? ((ig > 0.) ? 3.14159265358979323846 / 2 : -3.14159265358979323846 / 2) : atan2(ig, rl));
				t1 = ph - gl[k1];
				gl[k1] = ph;
				t1 -= (double)k1*ex;
				qpd = t1 / 3.14159265358979323846;
				(qpd >= 0) ? (qpd += qpd & 1) : (qpd -= qpd & 1);

				t1 -= 3.14159265358979323846*(double)qpd;
				t1 = overlap*t1 / (6.28318530717958647692);
				t1 = (double)k1*fb + t1*fb; gn[k1] = ma; ga[k1] = t1;
			}
			memset(gm, 0, fftsize * 4 * 4);
			memset(gy, 0, fftsize * 8 * 4);
			for (k1 = 0; k1 <= f2; k1++) {
				i3 = (long)((double)k1*pitch);
				if (i3 <= f2) {
					gm[i3] += gn[k1];
					gy[i3] = (long)((double)ga[k1] * pitch);
				}
			}
			for (k1 = 0; k1 <= f2; k1++) {
				ma = gm[k1]; t1 = gy[k1];
				t1 -= (double)k1*fb; t1 /= fb;
				t1 = 6.28318530717958647692*t1 / overlap;
				t1 += (double)k1*ex;
				gs[k1] += t1; ph = gs[k1];
				gf[2 * k1] = ma*cos(ph);
				gf[2 * k1 + 1] = ma*sin(ph);
			}
			for (k1 = fftsize + 2; k1 < 2 * fftsize; k1++) gf[k1] = 0.;
			for (i2 = 2; i2 < 2 * fftsize - 2; i2 += 2) {
				for (bt = 2, j2 = 0; bt < 2 * fftsize; bt <<= 1) {
					if (i2 & bt) j2++;
					j2 <<= 1;
				}
				if (i2 < j2) {
					p1 = gf + i2; p2 = gf + j2; tp = *p1; *(p1++) = *p2; *(p2++) = tp; tp = *p1; *p1 = *p2;
					*p2 = tp;
				}
			}
			for (k2 = 0, le = 2; k2 < (long)(log(fftsize) / 0.693147180559945309417 + 0.5); k2++) {
				le <<= 1;
				le2 = le >> 1;
				ur = 1.0;
				ui = 0.0;
				arg = 3.14159265358979323846 / (le2 >> 1);
				wr = cos(arg);
				wi = sin(arg);
				for (j2 = 0; j2 < le2; j2 += 2) {
					p1r = gf + j2;
					p1i = p1r + 1;
					p2r = p1r + le2;
					p2i = p2r + 1;
					for (i2 = j2; i2 < 2 * fftsize; i2 += le) {
						tr = *p2r * ur - *p2i * ui;
						ti = *p2r * ui + *p2i * ur;
						*p2r = *p1r - tr;
						*p2i = *p1i - ti;
						*p1r += tr;
						*p1i += ti;
						p1r += le;
						p1i += le;
						p2r += le;
						p2i += le;
					}
					tr = ur*wr - ui*wi;
					ui = ur*wi + ui*wr;
					ur = tr;
				}
			}
			for (k1 = 0; k1 < fftsize; k1++) {
				wd = -.5*cos(6.28318530717958647692*(double)k1 / (double)fftsize) + 0.5;
				gu[k1] += 2.*wd*gf[2 * k1] / (f2*overlap);
			}
			for (k1 = 0; k1 < ss; k1++) go[k1] = gu[k1];
			memmove(gu, gu + ss, fftsize * 4 * 4);
			for (k1 = 0; k1 < il; k1++) gi[k1] = gi[k1 + ss];
		}
	}
}
inline void setnote(int chennel, int key, int vel, double volume) {

	chkytmp = ((chennel << 7) | key);
	volumeptr3mono[chkytmp] = volumeptr2[chkytmp][vel][0];

	
	parsetmp[chkytmp][volumeptr3mono[chkytmp]][0].set(volume, vel);
}
inline void releasenote(int chennel, int key) {
	
	chkytmp = ((chennel << 7) | key);
	parsetmp[chkytmp][volumeptr3mono[chkytmp]][0].release();
}
inline void setnotestereo(int chennel,int key,int vel, double volume ) {
	chkytmp = ((chennel << 7) | key);
	volumeptr3[chkytmp][0] = volumeptr2[chkytmp][vel][0];
	volumeptr3[chkytmp][1] = volumeptr2[chkytmp][vel][1];
	parsetmp[chkytmp][volumeptr3[chkytmp][0]][0].set(volume, vel);
	parsetmp[chkytmp][volumeptr3[chkytmp][1]][1].set(volume, vel);
}
inline void releasenotestereo(int chennel, int key) {
	chkytmp = ((chennel << 7) | key);
	parsetmp[chkytmp][volumeptr3[chkytmp][0]][0].release();
	parsetmp[chkytmp][volumeptr3[chkytmp][1]][1].release();
}
inline int resetarray() {
	aptr = 0;
	bfrsize = playrate;
	bf2 = bfrsize / 10;
	bfsd50 = bfrsize / 2200;
	for (int i = 0; i < 16; i++) {
		poutbuffer[i] = new double[bfrsize];
		pcoutbuffer[i]= new double[bfrsize];
	}
	sineval = new int[bfrsize];
	for(int j=0;j<16;j++)
	for (int i = 0; i < bfrsize; i++) {
		poutbuffer[j][i] = 0; 
		pcoutbuffer[j][i] = 0;
		sineval[i] = 
		(
			((int)((double)bfsd50*(1. + sin(4 * 3.1415 * (double)i / (double)bfrsize))))
			);
	}
	return 0;
}
inline int getleft2() {

	getlefttmp = 0;
#pragma omp parallel for reduction(+:getlefttmp)
	for (int i = 0; i < 512; i++) {
		getlefttmp +=
			parsetmp[i][volumeptr3mono[i]][0].readval() +
			parsetmp[i + 0x0200][volumeptr3mono[i + 0x0200]][0].readval() +
			parsetmp[i + 0x0400][volumeptr3mono[i + 0x0400]][0].readval() +
			parsetmp[i + 0x0600][volumeptr3mono[i + 0x0600]][0].readval();

	}


	if (getlefttmp > 2147483647.)return INT_MAX;
	else if (getlefttmp <-2147483648.)return  INT_MIN;
	else return (int)getlefttmp;
}
//unsigned long long zt = 0;
double outbuffer_u[2048];
inline int getleft() {
	//zt++;
	//if (zt == 321064) system("pause");
	aptr++;
	if (aptr == bfrsize) aptr = 0;
	for (int i = 0; i < 16; i++)
		poutbuffer[i][aptr] = 0;
	getlefttmp = 0;
	
#pragma omp parallel 
	{
	double outbuffer_t[16] = { 0. };
#pragma omp for	
	for (int t = 0; t < 2048; t++)
		outbuffer_t[t >> 7] += parsetmp[t][volumeptr3mono[t]][0].readval();
#pragma omp critical
	{
		for (int t = 0; t < 16; t++)
			poutbuffer[t][aptr] += outbuffer_t[t];
	}

}
		


	
	
	aptrc1 = aptr - sineval[aptr];
	if (aptrc1 <0)aptrc1 += bfrsize;
	for (int i = 0; i<16; i++)
		pcoutbuffer[i][aptr] = poutbuffer[i][aptr] + dchorus[i] * (poutbuffer[i][aptrc1]);

	aptrrv = aptr -bf2;
	if(aptrrv <0) aptrrv += bfrsize;
	for(int i=0;i<16;i++)
		getlefttmp += pcoutbuffer[i][aptr] + dreverb[i] * pcoutbuffer[i][aptrrv];
	




	if (getlefttmp > 2147483647.)return INT_MAX;
	else if (getlefttmp <-2147483648.)return  INT_MIN;
	
	else return (int)getlefttmp;
}
inline int getboth() {

	getlefttmp = 0;
	getrighttmp = 0;
	
#pragma omp parallel for reduction(+:getlefttmp,getrighttmp),private(getbotht)
	for (int i = 0; i < 2048; i++) {

		if (parsetmp[i][volumeptr3[i][1]][1].lrlo) {
			getrighttmp += parsetmp[i][volumeptr3[i][1]][1].readval();
			getlefttmp += parsetmp[i][volumeptr3[i][0]][0].readval();
		}
		else {
			getbotht= parsetmp[i][volumeptr3[i][0]][0].readval();
			getrighttmp += getbotht;
			getlefttmp += getbotht;
		}
		 
	}
	
	if (getrighttmp > INT_MAX)getboth_righttmp= INT_MAX;
	else if (getrighttmp <INT_MIN)getboth_righttmp= INT_MIN;
	else getboth_righttmp=(int)getrighttmp;
	
	if (getlefttmp > INT_MAX)return INT_MAX;
	else if (getlefttmp <-INT_MIN)return  INT_MIN;
	else return (int)getlefttmp;
}
void readsf(char *sfname) {


	sf = fopen(sfname, "rb");
	fseek(sf, 0x10, SEEK_CUR);
	fread(&soundfontcppsiz, 4, 1, sf);
	fseek(sf, soundfontcppsiz, SEEK_CUR);
	//sfbk 건너뜀
	fseek(sf, 0x04, SEEK_CUR); //LIST
	unsigned int sdtasiz;
	fread(&sdtasiz, 4, 1, sf);
	sdtasiz += ftell(sf);//""LISTPÙ..pdtaphdr
	char sdta[5];
	fseek(sf, 4, SEEK_CUR);
	fread(sdta, 1, 4, sf);
	sdta[4] = 0;
	

	if (strcmp(sdta, "smpl") == 0) {
		fread(&tmpsize, 4, 1, sf);
		smplptr = ftell(sf);
		fseek(sf, tmpsize, SEEK_CUR);

	}
	fread(sdta, 1, 4, sf);
	sdta[4] = 0;
	
	if (strcmp(sdta, "sm24") == 0) {
		fread(&tmpsize, 4, 1, sf);
		fseek(sf, tmpsize, SEEK_CUR);
	}
	fseek(sf, sdtasiz + 0x0C, SEEK_SET);

	while (1) {
		char pdta[5]; pdta[4] = 0;
		fread(pdta, 1, 4, sf); 
		if (strcmp(pdta, "phdr") == 0) {//사운드샘플 이름,필요없음
			int phdrsize;
			fread(&phdrsize, 4, 1, sf);
			soundfont.presetcount = phdrsize / 38;
			soundfont.preset = new preset[phdrsize / 38];
			soundfont.presetptr = new unsigned short[phdrsize / 38];
			short bank, preset, wpresetbagndx = -1, owpresetbagndx;
			
			for (int i = 0; i < soundfont.presetcount; i++) {
				fread(soundfont.preset[i].presetname, 1, 20, sf);
				fread(&preset, 2, 1, sf);
				fread(&bank, 2, 1, sf);
				owpresetbagndx = wpresetbagndx;
				fread(&wpresetbagndx, 2, 1, sf);
				soundfont.preset[i]._bank = bank;
				soundfont.preset[i]._preset=preset;
				//printf("%d %d\n", bank, preset);
				if (i != 0) {
					soundfont.preset[i - 1].pgencount = wpresetbagndx - owpresetbagndx;
					soundfont.preset[i - 1].pgen = new pgen[wpresetbagndx - owpresetbagndx];
					soundfont.preset[i - 1].pgenptr = new unsigned short[wpresetbagndx - owpresetbagndx];
				}

				fseek(sf, 12, SEEK_CUR);

			}
			soundfont.preset[soundfont.presetcount - 1].pgencount = 1;
			soundfont.preset[soundfont.presetcount - 1].pgen = new pgen[1];
			soundfont.preset[soundfont.presetcount - 1].pgenptr = new unsigned short[1];
			
		}
		else if (strcmp(pdta, "pbag") == 0) {//개수만 필요

			int pbagsize;
			fread(&pbagsize, 4, 1, sf);
			for (int h = 0; h < soundfont.presetcount; h++) {
				for (int i = 0; i < soundfont.preset[h].pgencount; i++) {
					fread(&soundfont.preset[h].pgenptr[i], 1, 2, sf);

					fseek(sf, 2, SEEK_CUR);
					
				}
			}
			
		}
		else if (strcmp(pdta, "pmod") == 0) {
			//sf2.0에선 모듈레이터 데이터 없음
			int pmodsize;
			fread(&pmodsize, 4, 1, sf);

			fseek(sf, pmodsize, SEEK_CUR);
		}

		else if (strcmp(pdta, "pgen") == 0) {
			int pgensize;
			fread(&pgensize, 4, 1, sf);
			

			short sfGenOper;
			genAmountType genAmountType;
			int t = 0;
			for (int h = 0; h < soundfont.presetcount; h++) {

				for (int i = 0; i < soundfont.preset[h].pgencount; i++) {
					for (; (i + 1 < soundfont.preset[h].pgencount) ? (t < soundfont.preset[h].pgenptr[i + 1]) : ((h + 1<soundfont.presetcount) ? (t<soundfont.preset[h + 1].pgenptr[0]) : (t < pgensize / 4)); t++) {
						fread(&sfGenOper, 2, 1, sf);
						fread(&genAmountType, 2, 1, sf);
						switch (sfGenOper) {
						case 0:
							soundfont.preset[h].pgen[i].samplestartoffset += genAmountType.wordval;
							break;
						case 1:
							soundfont.preset[h].pgen[i].sampleendoffset += genAmountType.wordval;
							break;
						case 2:
							soundfont.preset[h].pgen[i].startloopoffset += genAmountType.wordval;
							break;
						case 3:
							soundfont.preset[h].pgen[i].endloopoffset += genAmountType.wordval;
							break;
						case 4:
							soundfont.preset[h].pgen[i].samplestartoffset += 32768 * (int)genAmountType.wordval;
							break;
						case 5:
							soundfont.preset[h].pgen[i].modlfopitch = (double)genAmountType.wordval / 1200.;
							break;
						case 6:
							soundfont.preset[h].pgen[i].viblfopitch = (double)genAmountType.wordval / 1200.;
							break;
						case 7:
							soundfont.preset[h].pgen[i].modenvpitch = (double)genAmountType.wordval / 1200.;
							break;
						case 8:
							soundfont.preset[h].pgen[i].filterfrequency = 440.*pow(2., ((double)genAmountType.wordval / 100. - 69.) / 12.);
							
							break;
						case 9:
							soundfont.preset[h].pgen[i].filterresonance = (double)genAmountType.wordval / 10.;
							if (soundfont.preset[h].pgen[i].filterresonance <= 0) soundfont.preset[h].pgen[i].filterresonance = -1.;
							break;
						case 10:
							soundfont.preset[h].pgen[i].modlfofilter = (double)genAmountType.wordval / 1200.;
							break;
						case 11:
							soundfont.preset[h].pgen[i].modenvfilter = (double)genAmountType.wordval / 1200.;
							break;
						case 12:
							soundfont.preset[h].pgen[i].sampleendoffset += 32768 * (int)genAmountType.wordval;
							break;
						case 13:
							soundfont.preset[h].pgen[i].modlfovolume = (double)genAmountType.wordval / 10.;
							break;
						case 15:
							soundfont.preset[h].pgen[i].chorus = (double)genAmountType.wordval / 10.;
							break;
						case 16:
							soundfont.preset[h].pgen[i].reverb = (double)genAmountType.wordval / 10.;
							break;
						case 17:
							soundfont.preset[h].pgen[i].pan = (double)genAmountType.wordval / 10.;
							break;
						case 21:
							soundfont.preset[h].pgen[i].modlfodelay = pow(2., (double)genAmountType.wordval / 1200.);
							break;
						case 22:
							soundfont.preset[h].pgen[i].modlfofreq = pow(2., (double)genAmountType.wordval / 1200.)*8.176;
							break;
						case 23:
							soundfont.preset[h].pgen[i].viblfodelay = pow(2., (double)genAmountType.wordval / 1200.);
							break;
						case 24:
							soundfont.preset[h].pgen[i].viblfofreq = pow(2., (double)genAmountType.wordval / 1200.)*8.176;
							break;
						case 25:
							soundfont.preset[h].pgen[i].modenvdelay = pow(2., (double)genAmountType.wordval / 1200.);
							break;
						case 26:
							soundfont.preset[h].pgen[i].modenvattack = pow(2., (double)genAmountType.wordval / 1200.);
							break;
						case 27:
							//soundfont.preset[h].pgen[i].modenvhold = pow(2., (double)genAmountType.wordval / 1200.)
							soundfont.preset[h].pgen[i].modenvholdtmp = genAmountType.wordval;
							break;
						case 28:
							//soundfont.preset[h].pgen[i].modenvdecay = pow(2., (double)genAmountType.wordval / 1200.)
							soundfont.preset[h].pgen[i].modenvdecaytmp = genAmountType.wordval;
							break;
						case 29:
							soundfont.preset[h].pgen[i].modenvsustain = 1. - (double)genAmountType.wordval / 1000.;
							if (soundfont.preset[h].pgen[i].modenvsustain < 0.) soundfont.preset[h].pgen[i].modenvsustain = 0.;
							else if (soundfont.preset[h].pgen[i].modenvsustain > 1.) soundfont.preset[h].pgen[i].modenvsustain = 1.;
							break;
						case 30:
							soundfont.preset[h].pgen[i].modenvrelease = pow(2., (double)genAmountType.wordval / 1200.);
							break;
						case 31:
							//soundfont.preset[h].pgen[i].modenvhold = pow(2., (double)genAmountType.wordval / 1200.)
							soundfont.preset[h].pgen[i].keymodenvholdtmp = genAmountType.wordval;
							break;
						case 32:
							//soundfont.preset[h].pgen[i].modenvdecay = pow(2., (double)genAmountType.wordval / 1200.)
							soundfont.preset[h].pgen[i].keymodenvdecaytmp = genAmountType.wordval;
							break;
						case 33:
							soundfont.preset[h].pgen[i].volenvdelay = pow(2., (double)genAmountType.wordval / 1200.);
							break;
						case 34:
							soundfont.preset[h].pgen[i].volenvattack = pow(2., (double)genAmountType.wordval / 1200.);
							break;
						case 35:
							//soundfont.preset[h].pgen[i].volenvhold = pow(2., (double)genAmountType.wordval / 1200.)
							soundfont.preset[h].pgen[i].volenvholdtmp = genAmountType.wordval;
							break;
						case 36:
							//soundfont.preset[h].pgen[i].volenvdecay = pow(2., (double)genAmountType.wordval / 1200.)
							soundfont.preset[h].pgen[i].volenvdecaytmp = genAmountType.wordval;
							break;
						case 37:
							soundfont.preset[h].pgen[i].volenvsustain = (double)genAmountType.wordval / 10.;
							break;							break;
						case 38:
							soundfont.preset[h].pgen[i].volenvrelease = pow(2., (double)genAmountType.wordval / 1200.);
							break;
						case 39:
							//soundfont.preset[h].pgen[i].volenvhold = pow(2., (double)genAmountType.wordval / 1200.)
							soundfont.preset[h].pgen[i].keyvolenvholdtmp = genAmountType.wordval;
							break;
						case 40:
							//soundfont.preset[h].pgen[i].volenvdecay = pow(2., (double)genAmountType.wordval / 1200.)
							soundfont.preset[h].pgen[i].keyvolenvdecaytmp = genAmountType.wordval;
							break;
						case 41://only pgen
								//soundfont.preset[h].pgen[i].inst = &(soundfont.inst[genAmountType.wordval]);
							soundfont.preset[h].pgen[i].insttmp = genAmountType.wordval;
							//unused
							break;
						case 43:
							soundfont.preset[h].pgen[i].keyrangelo = genAmountType.byteval[0];
							soundfont.preset[h].pgen[i].keyrangehi = genAmountType.byteval[1];
							break;
						case 44:
							soundfont.preset[h].pgen[i].velocityrangelo = genAmountType.byteval[0];
							soundfont.preset[h].pgen[i].velocityrangehi = genAmountType.byteval[1];
							break;
						case 45:
							soundfont.preset[h].pgen[i].startloopoffset += 32768 * (int)genAmountType.wordval;
							break;
						case 46:
							soundfont.preset[h].pgen[i].fixedkey = genAmountType.wordval;

							break;
						case 47:
							soundfont.preset[h].pgen[i].fixedvelocity = genAmountType.wordval;
							break;
						case 48:
							soundfont.preset[h].pgen[i].attenuation = (double)genAmountType.wordval / 10.;
							break;
						case 50:
							soundfont.preset[h].pgen[i].endloopoffset += 32768 * (int)genAmountType.wordval;
							break;
						case 51:
							soundfont.preset[h].pgen[i].tuningsemitones = genAmountType.wordval;
							break;
						case 52:
							soundfont.preset[h].pgen[i].tuningcents = genAmountType.wordval;
							break;
						case 53://igen only

							break;
						case 54://igen only

							break;
						case 56:
							soundfont.preset[h].pgen[i].tuningscale = genAmountType.wordval;
							break;
						case 57:
							soundfont.preset[h].pgen[i].exclusiveclass = genAmountType.wordval;
							break;
						case 58:
							soundfont.preset[h].pgen[i].rootkey = genAmountType.wordval;
							break;
						default:
							break;
						}
					}
				}
			}
			
		}
		

		else if (strcmp(pdta, "inst") == 0) {
			int instsize;
			fread(&instsize, 4, 1, sf);
			soundfont.instcount = instsize / 22;
			soundfont.inst = new inst[instsize / 22];
			soundfont.instptr = new unsigned short[instsize / 22];
			short winstbagndx = -1, owinstbagndx;
			
			for (int i = 0; i < soundfont.instcount; i++) {
				fread(soundfont.inst[i].instname, 1, 20, sf);
				owinstbagndx = winstbagndx;
				fread(&winstbagndx, 2, 1, sf);

				if (i != 0) {
					soundfont.inst[i - 1].igencount = winstbagndx - owinstbagndx;
					soundfont.inst[i - 1].igen = new igen[winstbagndx - owinstbagndx];
					soundfont.inst[i - 1].igenptr = new unsigned short[winstbagndx - owinstbagndx];
				}


			}
			soundfont.inst[soundfont.instcount - 1].igencount = 1;
			soundfont.inst[soundfont.instcount - 1].igen = new igen[1];
			soundfont.inst[soundfont.instcount - 1].igenptr = new unsigned short[1];
			
		}

		else if (strcmp(pdta, "ibag") == 0) {
			int ibagsize;
			fread(&ibagsize, 4, 1, sf);

			for (int h = 0; h < soundfont.instcount; h++) {
				for (int i = 0; i < soundfont.inst[h].igencount; i++) {
					fread(&soundfont.inst[h].igenptr[i], 1, 2, sf);

					fseek(sf, 2, SEEK_CUR);
					
				}
			}
			
		}
		else if (strcmp(pdta, "imod") == 0) {
			//sf2.0에선 모듈레이터 데이터 없음

			int imodsize;
			fread(&imodsize, 4, 1, sf);

			fseek(sf, imodsize, SEEK_CUR);
		}

		else if (strcmp(pdta, "igen") == 0) {
			int igensize;
			fread(&igensize, 4, 1, sf);
			

			short sfGenOper;
			genAmountType genAmountType;
			int t = 0;
			for (int h = 0; h < soundfont.instcount; h++) {

				for (int i = 0; i < soundfont.inst[h].igencount; i++) {
					for (; (i + 1 < soundfont.inst[h].igencount) ? (t < soundfont.inst[h].igenptr[i + 1]) : ((h + 1<soundfont.instcount) ? (t<soundfont.inst[h + 1].igenptr[0]) : (t < igensize / 4)); t++) {
						fread(&sfGenOper, 2, 1, sf);
						fread(&genAmountType, 2, 1, sf);
						switch (sfGenOper) {
						case 0:
							soundfont.inst[h].igen[i].samplestartoffset += genAmountType.wordval;
							break;
						case 1:
							soundfont.inst[h].igen[i].sampleendoffset += genAmountType.wordval;
							break;
						case 2:
							soundfont.inst[h].igen[i].startloopoffset += genAmountType.wordval;
							break;
						case 3:
							soundfont.inst[h].igen[i].endloopoffset += genAmountType.wordval;
							break;
						case 4:
							soundfont.inst[h].igen[i].samplestartoffset += 32768 * (int)genAmountType.wordval;
							break;
						case 5:
							soundfont.inst[h].igen[i].modlfopitch = (double)genAmountType.wordval / 1200.;
							break;
						case 6:
							soundfont.inst[h].igen[i].viblfopitch = (double)genAmountType.wordval / 1200.;
							break;
						case 7:
							soundfont.inst[h].igen[i].modenvpitch = (double)genAmountType.wordval / 1200.;
							break;
						case 8:
							soundfont.inst[h].igen[i].filterfrequency = 440.*pow(2., ((double)genAmountType.wordval / 100. - 69.) / 12.);
							
							break;
						case 9:
							soundfont.inst[h].igen[i].filterresonance = (double)genAmountType.wordval / 10.;
							if (soundfont.inst[h].igen[i].filterresonance <= 0) soundfont.inst[h].igen[i].filterresonance = -1.;
							break;
						case 10:
							soundfont.inst[h].igen[i].modlfofilter = (double)genAmountType.wordval / 1200.;
							break;
						case 11:
							soundfont.inst[h].igen[i].modenvfilter = (double)genAmountType.wordval / 1200.;
							break;
						case 12:
							soundfont.inst[h].igen[i].sampleendoffset += 32768 * (int)genAmountType.wordval;
							break;
						case 13:
							soundfont.inst[h].igen[i].modlfovolume = (double)genAmountType.wordval / 10.;
							break;
						case 15:
							soundfont.inst[h].igen[i].chorus = (double)genAmountType.wordval / 10.;
							break;
						case 16:
							soundfont.inst[h].igen[i].reverb = (double)genAmountType.wordval / 10.;
							break;
						case 17:
							soundfont.inst[h].igen[i].pan = (double)genAmountType.wordval / 10.;
							break;
						case 21:
							soundfont.inst[h].igen[i].modlfodelay = pow(2., (double)genAmountType.wordval / 1200.);
							break;
						case 22:
							soundfont.inst[h].igen[i].modlfofreq = pow(2., (double)genAmountType.wordval / 1200.)*8.176;
							break;
						case 23:
							soundfont.inst[h].igen[i].viblfodelay = pow(2., (double)genAmountType.wordval / 1200.);
							break;
						case 24:
							soundfont.inst[h].igen[i].viblfofreq = pow(2., (double)genAmountType.wordval / 1200.)*8.176;
							break;
						case 25:
							soundfont.inst[h].igen[i].modenvdelay = pow(2., (double)genAmountType.wordval / 1200.);
							break;
						case 26:
							soundfont.inst[h].igen[i].modenvattack = pow(2., (double)genAmountType.wordval / 1200.);
							break;
						case 27:
							//soundfont.inst[h].igen[i].modenvhold = pow(2., (double)genAmountType.wordval / 1200.);
							soundfont.inst[h].igen[i].modenvholdtmp = genAmountType.wordval;
							break;
						case 28:
							//soundfont.inst[h].igen[i].modenvdecay = pow(2., (double)genAmountType.wordval / 1200.);
							soundfont.inst[h].igen[i].modenvdecaytmp = genAmountType.wordval;
							break;
						case 29:
							soundfont.inst[h].igen[i].modenvsustain = 1. - (double)genAmountType.wordval / 1000.;
							if (soundfont.inst[h].igen[i].modenvsustain < 0.) soundfont.inst[h].igen[i].modenvsustain = 0.;
							else if (soundfont.inst[h].igen[i].modenvsustain > 1.) soundfont.inst[h].igen[i].modenvsustain = 1.;
							break;
						case 30:
							soundfont.inst[h].igen[i].modenvrelease = pow(2., (double)genAmountType.wordval / 1200.);
							break;
						case 31:
							//soundfont.inst[h].igen[i].modenvhold = pow(2., (double)genAmountType.wordval / 1200.);
							soundfont.inst[h].igen[i].keymodenvholdtmp = genAmountType.wordval;
							break;
						case 32:
							//soundfont.inst[h].igen[i].modenvdecay = pow(2., (double)genAmountType.wordval / 1200.);
							soundfont.inst[h].igen[i].keymodenvdecaytmp = genAmountType.wordval;
							break;
						case 33:
							soundfont.inst[h].igen[i].volenvdelay = pow(2., (double)genAmountType.wordval / 1200.);
							break;
						case 34:
							soundfont.inst[h].igen[i].volenvattack = pow(2., (double)genAmountType.wordval / 1200.);
							break;
						case 35:
							//soundfont.inst[h].igen[i].volenvhold = pow(2., (double)genAmountType.wordval / 1200.)
							soundfont.inst[h].igen[i].volenvholdtmp = genAmountType.wordval;
							break;
						case 36:
							//soundfont.inst[h].igen[i].volenvdecay = pow(2., (double)genAmountType.wordval / 1200.)
							soundfont.inst[h].igen[i].volenvdecaytmp = genAmountType.wordval;
							break;
						case 37:
							soundfont.inst[h].igen[i].volenvsustain = (double)genAmountType.wordval / 10.;
							break;
						case 38:
							soundfont.inst[h].igen[i].volenvrelease = pow(2., (double)genAmountType.wordval / 1200.);
							break;
						case 39:
							//soundfont.inst[h].igen[i].volenvhold = pow(2., (double)genAmountType.wordval / 1200.)
							soundfont.inst[h].igen[i].keyvolenvholdtmp = genAmountType.wordval;
							break;
						case 40:
							//soundfont.inst[h].igen[i].volenvdecay = pow(2., (double)genAmountType.wordval / 1200.)
							soundfont.inst[h].igen[i].keyvolenvdecaytmp = genAmountType.wordval;
							break;
						case 41://only igen
								//soundfont.inst[h].igen[i].inst = &soundfont.inst[genAmountType.wordval];
								//unused
							break;
						case 43:
							soundfont.inst[h].igen[i].keyrangelo = genAmountType.byteval[0];
							soundfont.inst[h].igen[i].keyrangehi = genAmountType.byteval[1];
							break;
						case 44:
							soundfont.inst[h].igen[i].velocityrangelo = genAmountType.byteval[0];
							soundfont.inst[h].igen[i].velocityrangehi = genAmountType.byteval[1];
							break;
						case 45:
							soundfont.inst[h].igen[i].startloopoffset += 32768 * (int)genAmountType.wordval;
							break;
						case 46:
							soundfont.inst[h].igen[i].fixedkey = genAmountType.wordval;

							break;
						case 47:
							soundfont.inst[h].igen[i].fixedvelocity = genAmountType.wordval;
							break;
						case 48:
							soundfont.inst[h].igen[i].attenuation = (double)genAmountType.wordval / 10.;
							break;
						case 50:
							soundfont.inst[h].igen[i].endloopoffset += 32768 * (int)genAmountType.wordval;
							break;
						case 51:
							soundfont.inst[h].igen[i].tuningsemitones = genAmountType.wordval;
							break;
						case 52:
							soundfont.inst[h].igen[i].tuningcents = genAmountType.wordval;
							break;
						case 53://igen only
								//soundfont.inst[h].igen[i].sample = &(soundfont.shdr[genAmountType.wordval]);
							soundfont.inst[h].igen[i].sampletmp = genAmountType.wordval;
							break;
						case 54://igen only
							soundfont.inst[h].igen[i].loopplayback = genAmountType.wordval;
							break;
						case 56:
							soundfont.inst[h].igen[i].tuningscale = genAmountType.wordval;
							break;
						case 57:
							soundfont.inst[h].igen[i].exclusiveclass = genAmountType.wordval;
							break;
						case 58:
							soundfont.inst[h].igen[i].rootkey = genAmountType.wordval;
							break;
						default:
							break;
						}
					}
				}
			}
			//system("pause");

		}
		else if (strcmp(pdta, "shdr") == 0) {
			int shdrsize;
			fread(&shdrsize, 4, 1, sf);
			soundfont.samplecount = shdrsize / 46;
			soundfont.smpl = new sample[shdrsize / 46];
			for (int i = 0; i < soundfont.samplecount; i++) {
				fread(soundfont.smpl[i].achSampleName, 1, 20, sf);
				fread(&soundfont.smpl[i].dwStart, 4, 1, sf);
				fread(&soundfont.smpl[i].dwEnd, 4, 1, sf);
				fread(&soundfont.smpl[i].dwStartloop, 4, 1, sf);
				fread(&soundfont.smpl[i].dwEndloop, 4, 1, sf);
				fread(&soundfont.smpl[i].dwSampleRate, 4, 1, sf);
				fread(&soundfont.smpl[i].byOriginalPitch, 1, 1, sf);
				fread(&soundfont.smpl[i].chPitchCorrection, 1, 1, sf);
				fseek(sf, 2, SEEK_CUR);
				fread(&soundfont.smpl[i].sfSampleType, 2, 1, sf);
				if (soundfont.smpl[i].sfSampleType & 1) soundfont.smpl[i].sfSampleType = 0;

				else if (soundfont.smpl[i].sfSampleType & 2) {
					if (soundfont.smpl[i].sfSampleType & 4) soundfont.smpl[i].sfSampleType = 0;
					else soundfont.smpl[i].sfSampleType = 2;
				}
				else if (soundfont.smpl[i].sfSampleType & 4) soundfont.smpl[i].sfSampleType = 1;
				else soundfont.smpl[i].sfSampleType = 0;
			}
			//system("pause");
			unsigned long t = ftell(sf);
			fseek(sf, 0, SEEK_END);
			if (ftell(sf) == t) {
				//printf("\nmapping\n");
				for (int i = 0; i < soundfont.presetcount; i++) {
					for (int j = 0; j < soundfont.preset[i].pgencount; j++) {
						if (soundfont.preset[i].pgen[j].insttmp != -1)
							soundfont.preset[i].pgen[j].inst = &(soundfont.inst[soundfont.preset[i].pgen[j].insttmp]);

					}
				}
				for (int i = 0; i < soundfont.instcount; i++) {
					for (int j = 0; j < soundfont.inst[i].igencount; j++) {
						if (soundfont.inst[i].igen[j].sampletmp != -1)
							soundfont.inst[i].igen[j].smpl = &(soundfont.smpl[soundfont.inst[i].igen[j].sampletmp]);

					}
				}
				//system("pause");
				break;
			}
		}

		else {
			break;
		}
	}
}
void reparsesf(int chennel, int bank, int preset, int samplerate) {

	parsetmpgen tmp;
	int i = 0;
	//printf("^%d", soundfont.presetcount);
	for (int g = 0; g < soundfont.presetcount; g++) {
		if (soundfont.preset[g]._bank == bank&&soundfont.preset[g]._preset == preset) {
			i = g;
			
			break;
		}

	}
	
	for (int j = 0; j < soundfont.preset[i].pgencount; j++) {
		if (soundfont.preset[i].pgen[j].insttmp != -1) {
			for (int k = 0; k < (soundfont.preset[i].pgen[j].inst)->igencount; k++) {
				if ((soundfont.preset[i].pgen[j].inst)->igen[k].sampletmp != -1) {
					//printf(">");
					tmp.reset();
					tmp.loadpgen(soundfont.preset[i].pgen[j], j);
					tmp.loadigen((soundfont.preset[i].pgen[j].inst)->igen[k], k);
					//printf("%s;", tmp.achSampleName);
					//
					for (int l = tmp.keyrangelo; l <= tmp.keyrangehi; l++) {
						for (int n = tmp.lrlo; n <= tmp.lrhi;n++) {
							for (int m = tmp.velocityrangelo; m <= tmp.velocityrangehi; m++) {
								
								////printf("?%d %d %d\n", tmp.lrlo, n, tmp.lrhi);
								memcpy(&(parsetmp[((chennel << 7) | l)][m][n]), &tmp, sizeof(parsetmpgen));
								parsetmp[((chennel << 7) | l)][m][n].setup(chennel);
								parsetmp[((chennel << 7) | l)][m][n].loaddefault(l, m, n, samplerate);
								if (chennel == 9) parsetmp[((chennel << 7) | l)][m][n].loaddefault10();
								//if(tmp.lrlo==tmp.lrhi)
								//printf("%lf %d %d;", tmp.pan, tmp.lrlo, tmp.lrhi);
							}
							
						}

					}

				}
			}
		}
	}
	//printf("**%s ", parsetmp[(chennel<< 7) + 50][2][0].achSampleName);
	int t = 0;
	/*
	volumeptr3mono[chkytmp] = volumeptr2[chkytmp][vel][0];
	("! %lf\n", pitcherr);
	parsetmp[chkytmp][volumeptr3mono[chkytmp]][0].set(volume, pitcherr);
	*/
	for (int l = 0; l < 128; l++) {
		for (int n = 0; n < 2; n++) {
			
			for (int m = 0; m < 128; m++) {



				if ((m == 0 || parsetmp[((chennel << 7) + l)][m][n].reg != parsetmp[((chennel << 7) + l)][m - 1][n].reg) && parsetmp[((chennel << 7) + l)][m][n].keyrangelo != -1) {
					t = parsetmp[((chennel << 7) + l)][m][n].velocityrangelo;

					
				}
				//if (t == -1) t = 0;
				volumeptr2[((chennel << 7) + l)][m][n] = t;
				
			}

		}

	}
	//system("pause");
}

#endif