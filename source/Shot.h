//
// Shot.h: Projectile code.
//
#ifndef __SHOT_H
#define __SHOT_H

#define SHOT_SPEED	2.5f
#define MAX_SHOTS	32
#define SHOT_LENGTH 3
#define SHOT_COLOR		(128, 50, 30)
#define SHOT_COLOR_2	(255, 50, 0)

enum E_ShotDirections 
{
	SD_North,
	SD_East,
	SD_South,
	SD_West
};

typedef struct shot_s
{
	int Enabled;
	float X, Y;
	int Dir;
} shot_t;

/*
=========
ShotTick
=========
	Moves projectiles
*/
void ShotTick(void);

/*
=========
ShotDraw
=========
	Draws all enabled shots.
*/
void ShotDraw(void);

/*
=========
ShotKill
=========
	Kills given shot, all shots if -1 is specified.
*/
void ShotKill(int idx);

/*
========
NewShot
========
	Creates a shot and initializes it at the given position.
	Returns shot index, -1 if error.
*/
int NewShot(float x, float y, int dir);

#endif