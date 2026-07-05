//
//	Common.h: Ark main header
//

#ifndef __COMMON_H
#define __COMMON_H

//
// System includes
//
#define _CRT_SECURE_NO_DEPRECATE

#ifndef NO_SOUND
#include <fmod.h>
#endif

#include <allegro.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <setjmp.h>

// TODO: enable ASM only in i386
// #define ASM

#define FPS		60
#define TimeDT	(1.0f / (float) FPS)

// SCR_WIDTH and SCR_HEIGHT are the virtual dimensions of the screen.
#define SCR_WIDTH	320
#define SCR_HEIGHT	240

#define SCR_DEPTH	VidDepth
#define SCR_FULLSCREEN

#define SCR_SCALE	VidScale

// When SCR_SCALE is defined, the actual screen size is multiplied by SCR_SCALE
#ifndef SCR_FULLSCREEN
#endif

// This define enables cheats, debug info and hacks to debug the code.
#define DEBUG

// If defined, the system will fill the settings file with junk.
#define JUNK_CFG

// Breakpoint.
#ifdef DEBUG
#define BREAK()	do { __asm int 3; } while(0)
#else
#define BREAK()
#endif

// Main backbuffer: size is SCR_WIDTH*SCR_HEIGHT
#define VidBuffer VidPages[VidCurPage]
extern BITMAP *VidPages[3];
extern int VidCurPage;
extern int VidScale, VidDepth, VidTripleBuffer;

// Mouse position: range=0 to SCR_WIDTH/SCR_HEIGHT - 1
extern int MouseX, MouseY;
extern int MouseDX, MouseDY;
extern int bQuit;
// MouseB: &1=LMB, &2=RMB, &4=MMB
// MouseDZ: mouse wheel relative movement
extern int MouseB, MouseDZ;

extern char GameDir[];

enum E_Options {
	OPT_Sound,
	OPT_Music,
	OPT_Physics,
	OPT_Powerups,
	OPT_Cheats,
	OPT_Fullscreen,
	OPT_VSync,
	OPT_VideoScale,
	OPT_TripleBuffer,
	OPT_ColorDepth,
	OPT_MAX
};

extern int Options[];
/*
==================
GetStaticChar1024
==================
	This function returns a static array of characters, 1024 chars
	long. This function only uses one buffer and is unsafe to call.
*/
char *GetStaticChar1024(void);

/*
=============
ChopString
=============
	This function returns a piece of string of the requested size.
*/
void ChopString(char *source, char *dest, int start, unsigned int len);

/*
==============
ValidFilename
==============
	This function returns 1 if fname is a valid filename. ext is a string
	with the expected file extension (in uppercase).

	Valid filenames:
		- Must have at least one letter before period
		- Must have only one period, and exactly 3 characters after it.
		- Can't be >= 64 characters, including extension.
		- Only contain:
			- Alphanumeric characters
			- Space (0x20, ' ')
			- One dot (for extension)
*/
int ValidFilename(char *fname, char *ext);

/*
===========
BlitSprite
===========
	Draws a sprite, masking full pink colors.
*/
void BlitSprite(BITMAP *bmp, int SX, int SY);

//
// Game includes
//
#include "Sound.h"		// Sound code
#include "Ball.h"		// Ball code
#include "Pad.h"		// Pad code
#include "Brick.h"		// Brick rendering code
#include "Map.h"		// Map code
#include "Edit.h"		// Editor code
#include "Token.h"		// Token code
#include "Game.h"		// Game logic code
#include "Hud.h"		// HUD code
#include "Bg.h"			// Background code
#include "Shot.h"		// Projectile code
#include "Menu.h"		// Menu code
#include "Explosion.h"	// Explosion code
#include "Hiscore.h"	// Hi-Score code
#include "Effect.h"		// GFX Effect Code
#endif
