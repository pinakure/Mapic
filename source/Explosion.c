//
// Explosion.c: Explosions code.
//
#include "Common.h"

typedef struct explosion_s
{
	int Enabled;
	int X, Y;
	int Frame;
} explosion_t;

static explosion_t Explosions[MAX_EXPLOSIONS];
static BITMAP *ExplosionGfx;
static float IncFrame=0.0f;

int InitExplosions(void)
{
	char *str;
	str = GetStaticChar1024();
	strcpy(str, GameDir);
	strcat(str, "gfx/explode.bmp");

	ExplosionGfx = load_bitmap(str, NULL);
	if(!ExplosionGfx) {
		alert("Can't open", "gfx/explode.bmp", NULL, "OK", NULL, 0, 0);
		return 0;
	}
	return 1;
}

void DeinitExplosions(void)
{
	destroy_bitmap(ExplosionGfx);
}

void ExplosionTick(void)
{
	int i, j;

	IncFrame -= TimeDT;
	if(IncFrame <= 0) {
		IncFrame = 1.0f / EXPLOSION_FPS;

		for(i=0; i<MAX_EXPLOSIONS; i++) {
			if(Explosions[i].Enabled) {
				Explosions[i].Frame++;
				if(Explosions[i].Frame == EXPLOSION_FRAMES) {
					ExplosionKill(i);

#define a(n, x, y) \
	case n: if(MapTestBrick((x), (y))) { \
		if(!MapTouchBrick((x), (y))) return; } \
		break;

					for(j=0; j<9; j++) {
						switch(j) {
							a(1, Explosions[i].X  , Explosions[i].Y  );
							a(2, Explosions[i].X+1, Explosions[i].Y  );
							a(3, Explosions[i].X+2, Explosions[i].Y  );
							a(4, Explosions[i].X  , Explosions[i].Y+1);
							a(5, Explosions[i].X+1, Explosions[i].Y+1);
							a(6, Explosions[i].X+2, Explosions[i].Y+1);
							a(7, Explosions[i].X  , Explosions[i].Y+2);
							a(8, Explosions[i].X+1, Explosions[i].Y+2);
							a(9, Explosions[i].X+2, Explosions[i].Y+2);
						}
					}
#undef a
				}
			}
		}
	}
}

void ExplosionDraw(void)
{
	int i;

	for(i=0; i<MAX_EXPLOSIONS; i++) {
		if(Explosions[i].Enabled) {
			masked_blit(ExplosionGfx, VidBuffer,
				Explosions[i].Frame * EXPLOSION_WIDTH, 0,
				3+(Explosions[i].X*BRICK_WIDTH), 3+(Explosions[i].Y*BRICK_HEIGHT),
				EXPLOSION_WIDTH, EXPLOSION_HEIGHT);
		}
	}
}

void ExplosionKill(int idx)
{
	int i;

	if(idx == -1) {
		for(i=0; i<MAX_EXPLOSIONS; i++) {
			Explosions[i].Enabled = 0;
			GameIsQuiet = 0;
		}
	} else {
		Explosions[idx].Enabled = 0;
		GameIsQuiet--;
	}
}

int NewExplosion(int x, int y)
{
	int i;

	for(i=0; i<MAX_EXPLOSIONS; i++) {
		if(!Explosions[i].Enabled) {
			SndPlaySound(SND_Explosion);
			Explosions[i].Enabled = 1;
			Explosions[i].X = x-1;
			Explosions[i].Y = y-1;
			Explosions[i].Frame = 0;
			GameIsQuiet++;
			return i;
		}
	}
	return -1;
}
