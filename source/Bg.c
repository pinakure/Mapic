//
// Bg.c: Background code.
//
#include "Common.h"

static BITMAP *Bg = NULL;
static float IncFrame=0.0f;
static int CurFrame=0;

static void DrawFrame(int X, int Y)
{
	int sx;

	sx = CurFrame * BG_TILE_W;
	blit(Bg, VidBuffer, sx, 0, X, Y, BG_TILE_W, BG_TILE_H);
}

int BgLoad(char *fname)
{
	char *str;

	if(Bg) {
		BgUnload();
	}

	if(!ValidFilename(fname, "BMP")) {
		return 0;
	}
	str = GetStaticChar1024();
	strcpy(str, GameDir);
	strcat(str, "gfx/backdrop/");
	strcat(str, fname);

	Bg = load_bitmap(str, NULL);
	if(!Bg) return 0;

	return 1;
}

void BgUnload(void)
{
	if(Bg) {
		destroy_bitmap(Bg);
		Bg = NULL;
	}
}

void BgDraw(void)
{
	int x, y;
	int w;

	w = (MAP_WIDTH*BRICK_WIDTH) / BG_TILE_W;

	if(Bg) {
		y = 3;

		while(y < SCR_HEIGHT) {
			for(x=0; x<w; x++) {
				DrawFrame(3+(x*BG_TILE_W), y);
			}

			y += BG_TILE_H;
		}
	}
}

void BgTick(void)
{
	IncFrame -= TimeDT;
	if(IncFrame <= 0.0f) {
		IncFrame = 1.0f / BG_FPS;
		CurFrame = (CurFrame+1) % BG_FRAMES;
	}
}

