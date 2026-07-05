//
// Sound.c: Sound code
//
#include "Common.h"

#ifndef NO_SOUND

#define MAX_CHANNELS	128
#define MAX_POLY		2

static FMOD_SYSTEM *Sys;
static FMOD_SOUND *Sounds[SND_MAX];
static int Channel = 0;

static int LoadSound(char *fname, int idx)
{
	char *str;

	if( (!ValidFilename(fname, "WAV"))&&(!ValidFilename(fname, "XM")) ) {
		alert("Invalid wav filename", fname, NULL, "OK", NULL, 0, 0);
		return 0;
	}

	str = GetStaticChar1024();
	strcpy(str, GameDir);
	strcat(str, "sounds/");
	strcat(str, fname);

	if(FMOD_System_CreateSound(Sys, str, FMOD_DEFAULT, NULL, &Sounds[idx]) != FMOD_OK) {
		alert("Can't load", fname, NULL, "OK", NULL, 0, 0);
		return 0;
	}

	return 1;
}

static void Free(void)
{
	int i;

	for(i=0; i<SND_MAX; i++) {
		if(Sounds[i]) {
			FMOD_Sound_Release(Sounds[i]);
			Sounds[i] = NULL;
		}
	}
}

FMOD_DSP*			DSP_Echo;
FMOD_CHANNEL*		DSP_Chan;
FMOD_RESULT			DSP_Error;

int SndInit(void)
{
	int i;

	if(FMOD_System_Create(&Sys) != FMOD_OK) {
		alert("Can't initialize FMOD", NULL, NULL, "OK", NULL, 0, 0);
		return 0;
	}

	if(FMOD_System_Init(Sys, MAX_CHANNELS, FMOD_DEFAULT, NULL) != FMOD_OK) {
		FMOD_System_Release(Sys);
		alert("Can't initialize FMOD", NULL, NULL, "OK", NULL, 0, 0);
		return 0;
	}

	for(i=0; i<SND_MAX; i++) {
		Sounds[i] = NULL;
	}


#define Load(fname, idx) \
	if(!LoadSound(fname, idx)) { Free(); return 0; }

	// sound loading
	Load("silence.wav",		SND_Mute)
	Load("brick.wav",		SND_Brick)
	Load("bricknb.wav",		SND_Brick_NonBreak)
	Load("brickm.wav",		SND_Brick_Multi)
	Load("border.wav",		SND_Border)
	Load("pad.wav",			SND_Pad)
	Load("shot.wav",		SND_Shot)
	Load("explode1.wav",	SND_Explosion)
	Load("explode2.wav",	SND_Explosion_Alt1)
	Load("explode3.wav",	SND_Explosion_Alt2)
	Load("tokeng.wav",		SND_Token_Good)
	Load("tokenb.wav",		SND_Token_Bad)
	Load("tokenx.wav",		SND_Token_Extra)
	Load("tilt.wav",		SND_Tilt)
	Load("ballout.wav",		SND_BallOut)
	Load("menu.wav",		SND_Menu)
	Load("back.wav",		SND_Back)
	Load("confirm.wav",		SND_Confirm)
	Load("title.xm",		SND_TitleMusic)
	Load("hiscore.xm",		SND_HiscoreMusic)
	Load("jingle1.wav",		SND_Jingle1)
	Load("jingle2.wav",		SND_Jingle2)
#undef Load


	/* Add audio DSP's */
	FMOD_System_GetChannel(Sys, MAX_POLY + SND_Brick_NonBreak, &DSP_Chan);

		if(FMOD_System_CreateDSPByType(Sys, FMOD_DSP_TYPE_ECHO, &DSP_Echo) == FMOD_OK) {
			//if(FMOD_Channel_AddDSP(DSP_Chan, DSP_Echo, 0) == FMOD_OK) {
			if(FMOD_System_AddDSP(Sys, DSP_Echo, NULL) == FMOD_OK) {
				FMOD_DSP_SetParameter(DSP_Echo, FMOD_DSP_ECHO_DECAYRATIO, 0.6f);
				FMOD_DSP_SetParameter(DSP_Echo, FMOD_DSP_ECHO_DELAY,      80.0f);
				FMOD_DSP_SetParameter(DSP_Echo, FMOD_DSP_ECHO_DRYMIX,     1.0f);
				FMOD_DSP_SetParameter(DSP_Echo, FMOD_DSP_ECHO_WETMIX,     0.5f);
			} else {
				alert("Can't add DSP", "to channel", NULL, "OK", NULL, 0, 0);
			}
		} else {
			alert("Can't create DSP", "DSP_ECHO", NULL, "OK", NULL, 0, 0);
		}

	return 1;
}

void SndDeinit(void)
{
	Free();
	FMOD_System_Release(Sys);
}

void SndPlayMusic(int Sound)
{
	if(Options[OPT_Music]) {
		switch(Sound) {
			case SND_TitleMusic:
			case SND_HiscoreMusic:
				FMOD_System_PlaySound(Sys, MAX_POLY + Sound, Sounds[Sound], FALSE, NULL);
				break;
		}
	}
}

void SndSetVolume(int Sound, float volume)
{
	FMOD_CHANNEL* channel;

	FMOD_System_GetChannel(Sys, MAX_POLY + Sound, &channel);

	FMOD_Channel_SetVolume(channel, volume);
}

void SndStop(int Sound)
{
	FMOD_System_PlaySound(Sys, MAX_POLY + Sound, Sounds[SND_Mute], FALSE, NULL);
}

void SndPlaySound(int Sound)
{
	if(Options[OPT_Sound]){
			switch(Sound)
		{
			//Non Trigerable sounds
			case SND_HiscoreMusic:
			case SND_TitleMusic:
				break;
			//Dedicated channels
			case SND_Brick:
			case SND_Brick_NonBreak:
			case SND_Jingle1:
			case SND_Jingle2:
			case SND_Brick_Multi:
			case SND_BallOut:
			case SND_Border:
			case SND_Pad:
			case SND_Shot:
			case SND_Token_Good:
			case SND_Token_Bad:
			case SND_Token_Extra:
			case SND_Menu:
			case SND_Back:
			case SND_Confirm:
			case SND_Tilt:
				FMOD_System_PlaySound(Sys, MAX_POLY + Sound, Sounds[Sound], FALSE, NULL);
				break;

			//Polyphonic
			case SND_Explosion:
				FMOD_System_PlaySound(Sys, Channel, Sounds[Sound + Channel], FALSE, NULL);
				Channel = (Channel+1)%MAX_POLY;
				break;
		}
	}
}




void SndUpdate(void)
{
	FMOD_System_Update(Sys);
}

#else

void SndStop(enum E_Sounds Sound)
{}

void SndSetVolume(enum E_Sounds Sound, float volume)
{}

void SndPlayMusic(enum E_Sounds Sound)
{}

void SndDeinit(void)
{}

int SndInit(void)
{ return 1; }

void SndPlaySound(enum E_Sounds Sound)
{}

void SndUpdate(void)
{}

#endif
