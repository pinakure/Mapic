//
// Ball.c: Ball code
//
#include "Common.h"

ball_t Balls[MAX_BALLS];
static BITMAP *BallGFX[BT_MAX];
static float TiltTimer	= 0.0f;
float BallSpeed			= 2.0f;
int Kamikaze = 0;

static int LoadBallGFX(BITMAP *bmp, int idx)
{
	BITMAP *b;
	int x;

	b = create_bitmap(BALL_WIDTH, BALL_HEIGHT);
	if(!b) {
		alert("Can't create", "ball bitmap", NULL, "OK", NULL, 0, 0);
		return 0;
	}

	x = idx*BALL_WIDTH;

	blit(bmp, b, x, 0, 0, 0, BALL_WIDTH, BALL_HEIGHT);
	BallGFX[idx] = b;

	return 1;
}

void BallExplode(void)
{
	if(TheBall.Type==BT_Normal) {
		if(Kamikaze==0)Kamikaze=1;
	}
}

int InitBalls(void)
{
	BITMAP *bmp;
	char *path;
	int i;

	path = GetStaticChar1024();
	strcpy(path, GameDir);
	strcat(path, "gfx/balls.bmp");

	bmp = load_bitmap(path, NULL);

	if(!bmp) {
		alert("Can't load", "gfx/balls.bmp", NULL, "OK", NULL, 0, 0);
		return 0;
	}

	for(i=0; i<BT_MAX; i++) {
		if(!LoadBallGFX(bmp, i)) {
			return 0;
		}
	}

	KillBall(-1);

	destroy_bitmap(bmp);
	return 1;
}

void DeinitBalls(void)
{
	int i;

	for(i=0; i<BT_MAX; i++) {
		destroy_bitmap(BallGFX[i]);
		BallGFX[i] = NULL;
	}
}

int NewBall(float X, float Y, float DX, float DY, enum E_BallTypes Type, int Moving)
{
	int i;

	for(i=0; i<MAX_BALLS; i++) {
		if(Balls[i].Type == BT_Free) {
			Balls[i].Type	= Type;
			Balls[i].Move   = Moving;
			Balls[i].X		= X;
			Balls[i].Y		= Y;
			Balls[i].LX		= X;
			Balls[i].LY		= Y;
			Balls[i].NX		= X;
			Balls[i].NY		= Y;
			Balls[i].DX		= DX;
			Balls[i].DY		= DY;
			return i;
		}
	}

	return -1;
}

void KillBall(int idx)
{
	int i;

	if(idx == -1) {
		for(i=0; i<MAX_BALLS; i++) {
			Balls[i].Type = BT_Free;
			Balls[i].Move = 0;
		}
	} else {
		Balls[idx].Type = BT_Free;
		Balls[idx].Move = 0;
	}
}

void DrawBall(int X, int Y, enum E_BallTypes Type)
{
	masked_blit(BallGFX[Type], VidBuffer, 0, 0, X - (BALL_WIDTH/2), Y - (BALL_HEIGHT/2), BALL_WIDTH, BALL_HEIGHT);
}

void DrawBalls(void)
{
	int i;

	for(i=0; i<MAX_BALLS; i++) {
		if(Balls[i].Type != BT_Free) {
			DrawBall(Balls[i].X, Balls[i].Y, Balls[i].Type);
		}
	}
}

int CheckBall(int idx)
{
	if(Balls[idx].X < 6) {
		Balls[idx].DX = BallSpeed;
		SndPlaySound(SND_Border);
	} else if(Balls[idx].X > ((MAP_WIDTH*BRICK_WIDTH))) {
		Balls[idx].DX = -BallSpeed;
		SndPlaySound(SND_Border);
	}

	if(Balls[idx].Y < 3) {
		Balls[idx].DY = BallSpeed;
		SndPlaySound(SND_Border);
	} else if(Balls[idx].Y > 239) {
		if(idx == 0) {
			GameBallOut();
			return 0;
		} else {
			KillBall(idx);
		}
	}

	return 1;
}

void DrawBallVel(ball_t *b, float Scale)
{
	int cx, cy;
	int dx, dy;

	cx = b->X ;
	cy = b->Y ;

	dx = (int)((float)cx + (Scale*b->DX));
	dy = (int)((float)cy + (Scale*b->DY));

	line(VidBuffer, cx, cy, dx, dy, makecol(0,255,0));
}

static void ComputeNextBallPos(int idx)
{
	float px, py;
	int bx, by;
	int tx, ty;

	px = Balls[idx].X;
	py = Balls[idx].Y;

	bx = (px - 3) / BRICK_WIDTH;
	by = (py - 3) / BRICK_HEIGHT;

	while(1) {
		px += Balls[idx].DX;
		py += Balls[idx].DY;

		tx = (px - 3) / BRICK_WIDTH;
		ty = (py - 3) / BRICK_HEIGHT;

		if((bx != tx)||(by != ty)) {
			Balls[idx].NX = tx;
			Balls[idx].NY = ty;
			return;
		}
	}
}

int GetBallDir(int idx)
{
	int ret;

	ret = 0;

	if(Balls[idx].NX > Balls[idx].LX) {
		ret |= 1;
	} else if(Balls[idx].NX < Balls[idx].LX) {
		ret |= 2;
	}

	if(Balls[idx].NY > Balls[idx].LY) {
		ret |= 4;
	} else if(Balls[idx].NY < Balls[idx].LY) {
		ret |= 8;
	}

	return(ret);
}

static void BallBounce(int idx, int ox, int oy)
{
	int dir;

	dir = GetBallDir(idx);

	switch(dir)
	{
		case 1: // left
			Balls[idx].DX = -BallSpeed;
			break;
		case 2: // right
			Balls[idx].DX = BallSpeed;
			break;
		case 4: // down
			Balls[idx].DY = -BallSpeed;
			break;
		case 8: // up
			Balls[idx].DY = BallSpeed;
			break;

		case 5: //down + right
			if(MapTestBrick(ox, oy-1)&&MapTestBrick(ox-1, oy))
			{
				Balls[idx].DX = -BallSpeed;
				Balls[idx].DY = -BallSpeed;

				Balls[idx].X += 0.9f;
				Balls[idx].Y -= 0.9f;
				break;
			}
			if(!MapTestBrick(ox - 1, oy)) {
				Balls[idx].DX = -BallSpeed;
			} else if(!MapTestBrick(ox, oy - 1)) {
				Balls[idx].DY = -BallSpeed;
			}
			break;
		case 6: //down + left
			if(MapTestBrick(ox, oy-1)&&MapTestBrick(ox+1, oy))
			{
				Balls[idx].DX = BallSpeed;
				Balls[idx].DY = -BallSpeed;

				Balls[idx].X += 0.9f;
				Balls[idx].Y -= 0.9f;
				break;
			}

			if(!MapTestBrick(ox + 1, oy)) {
				Balls[idx].DX = BallSpeed;
			} else if(!MapTestBrick(ox, oy - 1)) {
				Balls[idx].DY = -BallSpeed;
			}
			break;
		case 9: //up + right
			if(MapTestBrick(ox, oy+1)&&MapTestBrick(ox-1, oy)) {
				Balls[idx].DX = -BallSpeed;
				Balls[idx].DY = BallSpeed;
				/* ! original -*/
				Balls[idx].X += 0.9f;
				Balls[idx].Y += 0.9f;
				break;
			}
			if(!MapTestBrick(ox - 1, oy)) {
				Balls[idx].DX = -BallSpeed;
			} else if(!MapTestBrick(ox, oy + 1)) {
				Balls[idx].DY = BallSpeed;
			}
			break;
		case 10: //up + left
			if(MapTestBrick(ox, oy+1)&&MapTestBrick(ox+1, oy)) {
				Balls[idx].DY = BallSpeed;
				Balls[idx].DX = BallSpeed;

				Balls[idx].X += 0.9f;
				Balls[idx].Y += 0.9f;
				break;
			} else if(MapTestBrick(ox, oy)&&MapTestBrick(ox+1, oy)) {
				Balls[idx].DY = BallSpeed;
				Balls[idx].Y += 1.0f;
				break;
			}
			if(!MapTestBrick(ox + 1, oy)) {
				Balls[idx].DX = BallSpeed;
			} else if(!MapTestBrick(ox, oy - 1)) {
				Balls[idx].DY = BallSpeed;
			}
			break;
		default:
			//textprintf(VidBuffer,font, 240, 64, makecol(255,255,255), "DIR: %d", dir);
			break;
	}
}

void MoveBalls(void)
{
	int i, c;
	float tx, ty;
	int x, y;
	int lx, ly;

	if(TiltTimer > 0) {
		TiltTimer -= TimeDT;
		if(TiltTimer<0)TiltTimer = 0;
	}

	for(i=0; i<MAX_BALLS; i++) {
		if(Balls[i].Type != BT_Free) {
			if(Balls[i].Move) {
				tx = Balls[i].X += Balls[i].DX;
				ty = Balls[i].Y += Balls[i].DY;


				if(!CheckBall(i))return;

				x = (tx - 3) / BRICK_WIDTH;
				y = (ty - 3) / BRICK_HEIGHT;

				c = MapTestBrick(x, y);
				if(c) {
					if((c == BRICK_NonBreak+1)||(c >= BRICK_ShotPassH+1)) {
						BallBounce(i, x, y);
					}
					else if(Balls[i].Type != BT_Power){
						BallBounce(i, x, y);
						if(Balls[i].Type == BT_Mini)NewExplosion(x-1, y-1);

					}
					switch(c-1) {
						case BRICK_NonBreak:
						case BRICK_ShotPassH:
						case BRICK_ShotPassV:
						case BRICK_ShotPassM:
							SndPlaySound(SND_Brick_NonBreak);
							break;

						case BRICK_Break1:
						case BRICK_Break2:
						case BRICK_Break3:
						case BRICK_ShotUp:
						case BRICK_ShotDown:
						case BRICK_ShotLeft:
						case BRICK_ShotRight:
						case BRICK_Multi1:
						case BRICK_Multi2:
						case BRICK_Multi3:
						case BRICK_Bomb:
							SndPlaySound(SND_Brick_Multi);
							break;

						default:
							SndPlaySound(SND_Brick);
							break;
					}

					MapTouchBrick(x, y);
				} else {
					Balls[i].X = tx;
					Balls[i].Y = ty;
					lx = Balls[i].LX;
					ly = Balls[i].LY;


					if((x != Balls[i].NX)||(y != Balls[i].NY)) {
						Balls[i].LX = x;
						Balls[i].LY = y;
						lx = x;
						ly = y;
					}
					//rect(VidBuffer, (lx*BRICK_WIDTH)+3, (ly*BRICK_HEIGHT)+3, (lx*BRICK_WIDTH)+2+BRICK_WIDTH, (ly*BRICK_HEIGHT)+2+BRICK_HEIGHT, makecol(255,0,255));

				}
				ComputeNextBallPos(i);


				/* DEBUG INFO */
				/*
					rect(VidBuffer, (x*BRICK_WIDTH)+3, (y*BRICK_HEIGHT)+3, (x*BRICK_WIDTH)+2+BRICK_WIDTH, (y*BRICK_HEIGHT)+2+BRICK_HEIGHT, makecol(255,0,0));
					rect(VidBuffer, (Balls[i].NX*BRICK_WIDTH)+3, (Balls[i].NY*BRICK_HEIGHT)+3, (Balls[i].NX*BRICK_WIDTH)+2+BRICK_WIDTH, (Balls[i].NY*BRICK_HEIGHT)+2+BRICK_HEIGHT, makecol(0,255,0));
					DrawBallVel(&Balls[i], 5.0f);

				*/
#ifdef DEBUG
				if(key[KEY_S])rest(300);
#endif


			}
		}
	}
}

void TiltBalls(int axis)
{
	int i;

	if(TiltTimer <= 0) {
		TiltTimer = 0.50f;

		for(i=0; i<MAX_BALLS; i++) {
			if(Balls[i].Type != BT_Free) {
				switch(axis)
				{
					case 0://left
						if(Balls[i].DX>0) {
							Balls[i].DX = -BallSpeed;
							GameAddScore(-100);
							SndPlaySound(SND_Tilt);
							}
						break;
					case 1://right
						if(Balls[i].DX<0) {
							Balls[i].DX = BallSpeed;
							GameAddScore(-100);
							SndPlaySound(SND_Tilt);
						}
						break;

					case 2://up
						if(Balls[i].DY>0) {
							Balls[i].DY = -BallSpeed;
							GameAddScore(-100);
							SndPlaySound(SND_Tilt);
						}
						break;
					case 3://down
						if(Balls[i].DY<0) {
							Balls[i].DY = BallSpeed;
							GameAddScore(-100);
							SndPlaySound(SND_Tilt);
						}
						break;
				}
			}
		}
		HudTilt();
	}


}
