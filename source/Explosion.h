//
// Explosion.h: Explosions code.
//
#ifndef __EXPLOSION_H
#define __EXPLOSION_H

#define MAX_EXPLOSIONS		32
#define EXPLOSION_FPS		25.0f
#define EXPLOSION_FRAMES	8
#define EXPLOSION_WIDTH		48
#define EXPLOSION_HEIGHT	24
/*
=================
DeinitExplosions
=================
	Frees explosions' resources.
*/
void DeinitExplosions(void);

/*
===============
InitExplosions
===============
	Loads explosions' resources.
*/
int InitExplosions(void);

/*
==============
ExplosionTick
==============
	Process explosions.
*/
void ExplosionTick(void);

/*
==============
ExplosionDraw
==============
	Draws explosions.
*/
void ExplosionDraw(void);
	
/*
==============
ExplosionKill
==============
	Kills given explosion.
*/
void ExplosionKill(int idx);

/*
=============
NewExplosion
=============
	Spawns a explosion at given point in brick coordinates .
	Returns explosion index or -1 if error
*/
int NewExplosion(int x, int y);



#endif
