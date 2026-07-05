//
// Brick.c: Brick rendering code
//
#include  "Common.h"

static BITMAP *Bricks[64];

static int LoadBricks(void)
{
	char *str;
	BITMAP *bmp;
	int x, y, i;

	str = GetStaticChar1024();
	strcpy(str, GameDir);
	strcat(str, "gfx/bricks.bmp");

	bmp = load_bitmap(str, NULL);
	if(!bmp) {
		alert("Can't open", "gfx/bricks.bmp", NULL, "OK", NULL, 0, 0);
		return 0;
	}

	for(i=0; i<64; i++) {
		Bricks[i] = create_bitmap(BRICK_WIDTH, BRICK_HEIGHT);
		if(!Bricks[i]) {
			alert("No mem for bricks!", NULL, NULL, "OK", NULL, 0, 0);
			return 0;
		}

		x = BRICK_WIDTH * (i%16);
		y = BRICK_HEIGHT * (i/16);

		blit(bmp, Bricks[i], x, y, 0, 0, BRICK_WIDTH, BRICK_HEIGHT);
	}

	destroy_bitmap(bmp);
	return 1;
}

int InitBricks(void)
{
	if(!LoadBricks()) {
		return 0;
	}

	return 1;
}

void DeinitBricks(void)
{
	int i;

	for(i=0; i<64; i++) {
		destroy_bitmap(Bricks[i]);
		Bricks[i] = NULL;
	}
}

void DrawBrick(int X, int Y, int Type)
{
	blit(Bricks[Type], VidBuffer, 0, 0, X, Y, BRICK_WIDTH, BRICK_HEIGHT);
}