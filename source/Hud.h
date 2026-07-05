//
// Hud.h: HUD code (header)
//
#ifndef __HUD_H
#define __HUD_H

// x, ex: start and end of pad counter (x axis)
// y: start of pad counter (y axis)
#define HUD_PAD_X		242 
#define HUD_PAD_EX		(242 + ((3 + PAD_MINLEN)*3))
#define HUD_PAD_Y		51

// text color
#define HUD_TEXT_COL	(100, 245, 0)

// active token coordinates
#define HUD_TOKEN_X		269
#define HUD_TOKEN_Y		117

// Remaining Pads coordinates
#define HUD_PADLEFT_X	240
#define HUD_PADLEFT_Y	97

// LCD Screen Coordinates
#define HUD_LCD_X		240
#define HUD_LCD1_Y		10
#define HUD_LCD2_Y		98

// Credit Marquee FPS
#define HUD_MARQUEE_FPS			12.5f

// Credit Max Phrases
#define MAX_CREDITS				26

enum E_HighlightTypes {
	HL_None = 0,
	HL_Good,
	HL_Neutral,
	HL_Bad
};

extern FONT *HudFontRed, *HudFontGreen, *HudFontWhite, *HudFont;

/*
========
HudInit
========
	Initializes the HUD graphics. Returns 0 on error.
*/
int HudInit(void);

/*
==========
HudDeinit
==========
	Frees HUD graphics.
*/
void HudDeinit(void);

/*
=============
HudHighlight
=============
	Makes the hud highlight ARK letters to give bounce feedback.
*/
void HudHighlight(int type);

/*
============
HudMapTitle
============
	Draws the Map Title on screen.
*/
void HudMapTitle(void);

/*
============
HudLCDPrint
============
	Displays a string in the LCD screen at given line index.
*/
void HudLCDPrint(int line, int HOffset, char* fmt, ...);

/*
==============
HudDrawWindow
==============
	Draws a window.
*/
void HudDrawWindow(void);

/*
========
HudDraw
========
	Draws the HUD.
*/
void HudDraw(void);

/*
==============
HudDrawString
==============
	Draws a string parsing \n at the given coordinates.
*/
void HudDrawString(int sx, int sy, char *str);

/*
==============
HudDrawEditor
==============
	Draws the HUD for the Editor Screen.
*/
void HudDrawEditor(void);

/*
=============
HudDrawTitle
=============
	Draws the title screen.
*/
void HudDrawTitle(void);
/*
==========
HudUpdate
==========
	Updates the HUD. Called by the game code.
*/
void HudUpdate(int score, int pads);

/*
============
HudPreTitle
============
	Displays a pre title screen.
*/
void HudPreTitleDraw(void);

/*
================
HudPreTitleTick
================
	returns 0 when pre title display time is over.
*/
int HudPreTitleTick(void);



/*
===========
HudCredits
===========
	Shows credits.
*/
void HudCredits(void);

/*
========
HudTilt
========
	Reports tilt message on the hud and gives lighting feedback
*/
void HudTilt(void);
#endif
