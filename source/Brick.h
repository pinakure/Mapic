//
// Brick.h: Brick rendering code
//
#ifndef __BRICK_H
#define __BRICK_H

// Brick size, in pixels
#define BRICK_WIDTH		16
#define BRICK_HEIGHT	8

//
// Bricks:
//	There are 3 sets of 16 normal bricks (48 bricks),
//	and there's space for 16 "special" bricks (like non-breakable).
//	The bricks.bmp file contains 4 rows of 16 bricks, which map to
//	these 3 sets + 1 special set.
//
enum E_SpecialBricks
{
	BRICK_NonBreak = 48,
	BRICK_Break1,
	BRICK_Break2,
	BRICK_Break3,
	BRICK_ShotLeft,
	BRICK_ShotUp,
	BRICK_ShotRight,
	BRICK_ShotDown,
	BRICK_Bomb,
	BRICK_Multi1,
	BRICK_Multi2,
	BRICK_Multi3,
	BRICK_ShotPassH,
	BRICK_ShotPassV,
	BRICK_ShotPassM
};

/*
===========
InitBricks
===========
	Initializes the brick renderer. Returns 0 on error.
*/
int InitBricks(void);

/*
=============
DeinitBricks
=============
	Frees all resources allocated by InitBricks.
*/
void DeinitBricks(void);

/*
==========
DrawBrick
==========
	Draws a single brick, of the given type, at pixel (X,Y).
*/
void DrawBrick(int X, int Y, int Type);

#endif
