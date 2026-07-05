#include "Common.h"

#define COLOR_WIDTH 2
#define HANDLE_WIDTH 2

static BITMAP *ColorSect;
static BITMAP *BarSect;
static float ShotTimer = 0.0f;

pad_t Pad;

static void CheckBall(ball_t *b, int len)
{
	
	if((b->Y > Pad.Y-4)&&(b->Y < Pad.Y+(PAD_HEIGHT-4))) {
		if((b->X >= Pad.X) && (b->X < (Pad.X+len))) {
			if((Pad.Mode == PM_Glue)&&(b == &TheBall)) {
				GameSetChooseDir();
				TheBall.Y = BALL_INITIAL_Y;
				TheBall.DY = -BallSpeed;
			} else {
				b->DY = -BallSpeed;
				SndPlaySound(SND_Pad);
			}
		}
	}
}

static void InitBarSect(void)
{
	putpixel(BarSect, 0, 0, makecol(190, 190, 195));
	putpixel(BarSect, 0, 1, makecol(220, 220, 255));
	putpixel(BarSect, 0, 2, makecol(120, 120, 125));
	putpixel(BarSect, 0, 3, makecol( 60,  60,  85));
}

static void DrawBarSect(int SX, int SY, int Len)
{
	int x;

	for(x=0; x<Len; x++) {
		blit(BarSect, VidBuffer, 0, 0, SX+x, SY, 1, PAD_HEIGHT);
	}
}

static void DrawHandle(int X, int Y, int Right)
{
	if(!Right) {
		rectfill(VidBuffer, X, Y+1, X+1, (Y+PAD_HEIGHT)-2, makecol(255, 255, 255));
		rectfill(VidBuffer, X+1, Y, X+(HANDLE_WIDTH-1), Y+PAD_HEIGHT-1, makecol(255, 255, 255));
	} else {
		rectfill(VidBuffer, X, Y, X+(HANDLE_WIDTH-2), Y+PAD_HEIGHT-1, makecol(255, 255, 255));
		rectfill(VidBuffer, X+(HANDLE_WIDTH-2), Y+1, X+HANDLE_WIDTH-1, Y+(PAD_HEIGHT-2), makecol(255, 255, 255));
	}
}

int PadInit(void)
{
	ColorSect = create_bitmap(COLOR_WIDTH, PAD_HEIGHT);
	if(!ColorSect) {
		return 0;
	}

	BarSect = create_bitmap(1, PAD_HEIGHT);
	if(!ColorSect) {
		destroy_bitmap(ColorSect);
		return 0;
	}

	InitBarSect();
	PadReset();
	Pad.AutoChase = 0;
	return 1;
}

void PadReset(void)
{
	PadSetColor(0, 0, 0);
	
	Pad.X = 3+(((MAP_WIDTH*BRICK_WIDTH)/2)-(PAD_MINLEN/2));
	Pad.Y = 220;
	Pad.ExtLen = 0;		
	Pad.Mode = PM_Normal;
}

void PadMove(int dx)
{
	Pad.X += dx;
	
	if(Pad.X < 3) Pad.X = 3;

	if(Pad.X > ((3+(MAP_WIDTH*BRICK_WIDTH))-PadGetLen()))
		Pad.X = ((3+(MAP_WIDTH*BRICK_WIDTH))-PadGetLen());

}

void PadDeinit(void)
{
	destroy_bitmap(ColorSect);
	destroy_bitmap(BarSect);
}

void PadSetColor(int r, int g, int b)
{
	static int hi[4] = { 196, 128, 64, 32 };
	static int lo[4] = {  48,  32, 16,  8 };
	int rr, gg, bb;

#define SET(id) \
	if(!r) rr = lo[id]; else rr = hi[id]; \
	if(!g) gg = lo[id]; else gg = hi[id]; \
	if(!b) bb = lo[id]; else bb = hi[id];

	SET(1); hline(ColorSect, 0, 0, COLOR_WIDTH-1, makecol(rr, gg, bb));
	SET(0); hline(ColorSect, 0, 1, COLOR_WIDTH-1, makecol(rr, gg, bb));
	SET(2); hline(ColorSect, 0, 2, COLOR_WIDTH-1, makecol(rr, gg, bb));
	SET(3); hline(ColorSect, 0, 3, COLOR_WIDTH-1, makecol(rr, gg, bb));
#undef SET
}

void PadDrawSprite(int X, int Y, int ExtLen)
{
	int l;

	l = PAD_MINLEN + ExtLen;
	if(l <= 0) return;

	DrawHandle(X, Y, 0);
	DrawHandle((X+l)-HANDLE_WIDTH, Y, 1);
	DrawBarSect(X+HANDLE_WIDTH+COLOR_WIDTH, Y, l-(2*(HANDLE_WIDTH+COLOR_WIDTH)));

	blit(ColorSect, VidBuffer, 0, 0, X+HANDLE_WIDTH, Y, COLOR_WIDTH, PAD_HEIGHT);
	blit(ColorSect, VidBuffer, 0, 0, (X+l)-(COLOR_WIDTH+HANDLE_WIDTH), Y, COLOR_WIDTH, PAD_HEIGHT);
}

void PadDraw(void)
{	
	PadDrawSprite(Pad.X, Pad.Y, Pad.ExtLen);	
}

void PadTick(void)
{
	int len, i;

	len = PAD_MINLEN + Pad.ExtLen;

	if(Pad.AutoChase>0) {
		Pad.X = TheBall.X-(PadGetLen()/2);
		if(Pad.X < 2) Pad.X = 2;
		if(Pad.X >(BRICK_WIDTH * MAP_WIDTH) - (PadGetLen() - 3)) {
			Pad.X = (BRICK_WIDTH * MAP_WIDTH) - (PadGetLen() - 3);		
		}
		Pad.AutoChase--;
	}
	
	if(ShotTimer > 0) {
		ShotTimer -= TimeDT;
		if(ShotTimer < 0)ShotTimer = 0.0f;
	}
	for(i=0; i<MAX_BALLS; i++) {
		if(Balls[i].Type != -1) {
			CheckBall(&Balls[i], len);
		}
	}	
}

void PadAddLen(int add)
{
	Pad.ExtLen += add*2;
	Pad.X -= add;

	if(Pad.X < 3) Pad.X = 3;

	if(Pad.X > ((3+(MAP_WIDTH*BRICK_WIDTH))-PadGetLen()))
		Pad.X = ((3+(MAP_WIDTH*BRICK_WIDTH))-PadGetLen());

	if(PadGetLen() >= (MAP_WIDTH*BRICK_WIDTH)) {
		Pad.X = 3;
		Pad.ExtLen = (MAP_WIDTH*BRICK_WIDTH)-PAD_MINLEN;
	}

	if(Pad.ExtLen < -12)Pad.ExtLen = -12;
}

void PadResetLen(void)
{
	Pad.X += Pad.ExtLen;
	Pad.ExtLen = 0;
}

int PadGetLen(void)
{
	return PAD_MINLEN + Pad.ExtLen;
}

void PadFire(void)
{
	if(Pad.Mode == PM_Shot) {		
		if(ShotTimer <= 0) {		
			ShotTimer = PAD_FIRE_TIME;
			NewShot(Pad.X + (PadGetLen() / 2), Pad.Y - SHOT_LENGTH, SD_North); 
		} 
	}
}