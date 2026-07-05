//
// Sound.h: Sound code, header file
//
#ifndef __SOUND_H
#define __SOUND_H

enum E_Sounds
{
	SND_Brick,
	SND_Brick_NonBreak,
	SND_Brick_Multi,
	SND_Border,
	SND_Pad,
	SND_BallOut,
	SND_Shot,
	SND_Tilt,
	SND_Token_Good,
	SND_Token_Bad,
	SND_Token_Extra,
	SND_Explosion,
	SND_Explosion_Alt1,
	SND_Explosion_Alt2,
	SND_Menu,
	SND_Confirm,
	SND_Back,
	SND_TitleMusic,
	SND_HiscoreMusic,
	SND_Jingle1,
	SND_Jingle2,
	SND_Mute,
	SND_MAX
};

/*
========
SndInit
========
	Allocates all sounds. Returns 1 on success.
*/
int SndInit(void);

/*
==========
SndDeinit
==========
	Frees the sounds.
*/
void SndDeinit(void);

/*
=============
SndPlaySound
=============
	Starts playing the given sound.
*/
void SndPlaySound(int Sound);

/*
========
SndStop
========
	Stops a sound or bgm.
*/
void SndStop(int Sound);

/*
=============
SndSetVolume
=============
	Changes desired sound-s channel volume.
*/
void SndSetVolume(int Sound, float volume);

/*
=============
SndPlayMusic
=============
	Starts playing music.
*/
void SndPlayMusic(int Sound);

/*
==========
SndUpdate
==========
	Updates the sound system. Must be called once per frame.
*/
void SndUpdate(void);

#endif
