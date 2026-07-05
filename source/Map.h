//
// Map.h: Map system
//
#ifndef __MAP_H
#define __MAP_H

// Map maximum size, in bricks.
#define MAP_WIDTH		14
#define MAP_HEIGHT		25
#define MAP_PIXCENTER	((MAP_WIDTH*BRICK_WIDTH)/2)
#define MAX_MAPINFO_SIZE		256
#define MAX_MAPINFO_LINE		30

//
// mbrick_t, map_t
// These are in-mem structs representing the current map (in var Map)
//
typedef struct mbrick_s
{
	int Type;		// brick type
	int Token;		// -1: random, 0: no tok, >=1: give Token-1
	int HitLeft;	// hits left to break
} mbrick_t;

typedef struct map_s
{
	char Info[256];
	char Name[32];
	char Bg[32];

	int BricksLeft;
	mbrick_t Bricks[MAP_WIDTH*MAP_HEIGHT];
} map_t;

/*
========
MapLoad
========
	Loads the given map. If it can't be loaded, it returns 0.
*/
int MapLoad(char *fname);

/*
===========
MapLoadDef
===========
	Builds a default map (no file loaded).
*/
void MapLoadDef(void);

/*
============
MapLoadList
============
	Parses the map list.
*/
int MapLoadList(void);

/*
============
MapFreeList
============
	Frees the map list.
*/
void MapFreeList(void);

/*
==============
MapTouchBrick
==============
	Event that notifies the map that a ball, a projectile
	or something else touched the block at brick coords (X,Y).
	Returns 0 if no further collision processing shall occur.
*/
int MapTouchBrick(int X, int Y);

/*
=============
MapTestBrick
=============
	This function returns 1 if there's a brick at the given
	brick position.
*/
int MapTestBrick(int X, int Y);

/*
========
MapDraw
========
	Draws the map at pixel (SX,SY)
*/
void MapDraw(int SX, int SY);

extern map_t Map;
extern char **MapList;
extern int MapListCount;

#endif
