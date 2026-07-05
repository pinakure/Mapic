//
// Bg.h: Background code (header)
//
#ifndef __BG_H
#define __BG_H

#define BG_TILE_W		16
#define BG_TILE_H		16
#define BG_FRAMES		8
#define BG_FPS			25.0f

/*
=======
BgLoad
=======
	Loads a background. If it fails, returns 0.
*/
int BgLoad(char *fname);

/*
=========
BgUnload
=========
	If a background was successfully loaded with BgLoad,
	BgUnload will unload it.
*/
void BgUnload(void);

/*
=======
BgDraw
=======
	Draws the background
*/
void BgDraw(void);

/*
=======
BgTick
=======
	Increments the background frame timer.
*/
void BgTick(void);


#endif
