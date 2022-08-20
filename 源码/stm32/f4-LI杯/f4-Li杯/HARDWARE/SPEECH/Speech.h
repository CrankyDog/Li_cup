#ifndef _SPEECH_H
#define _SPEECH_H
#include "delay.h"

extern u8 location_flag,improve_flag,play_flag,denoising_flag,stop_flag2,Fc_flag,nothing_flag,ori_flag;

void Open_Equ(void);
void Open_Loc(void);
void Open_Spe(void);
void Spe_Org(void);
void Spe_Equ(void);
void Spe_Wit(void);
void Speech_JG(void);

void Speech_SP(void);

#endif

