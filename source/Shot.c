//
// Shot.c: Projectile code.
//
#include "Common.h"

static shot_t Shots[MAX_SHOTS];

static void CheckShot(int idx)
{	
	int x,y;
	int c;

	x = (Shots[idx].X - 3) / BRICK_WIDTH;
	y = (Shots[idx].Y - 3) / BRICK_HEIGHT;
	
	switch(Shots[idx].Dir) {
		case SD_North:
			if(Shots[idx].Y <= 3) {
				ShotKill(idx);
				return;
			}			
			break;
		case SD_East:
			if(Shots[idx].X >= (3+(MAP_WIDTH*BRICK_WIDTH))) {
				ShotKill(idx);
				return;
			}
			break;
		case SD_South:
			if(Shots[idx].Y >= 239) {
				ShotKill(idx);
				return;
			}
			break;
		case SD_West:
			if(Shots[idx].X <= 3) {
				ShotKill(idx);
				return;
			}
			break;
	}	
	
	c = MapTestBrick(x, y);

	if(c) {
		if((Shots[idx].Dir == SD_North)||(Shots[idx].Dir == SD_South)) {
			if((c == BRICK_ShotPassV+1)||(c == BRICK_ShotPassM+1)) { 
			} else {
				MapTouchBrick(x, y);
				ShotKill(idx);
			}
		} 
		
		if((Shots[idx].Dir == SD_East)||(Shots[idx].Dir == SD_West)) {
			if((c == BRICK_ShotPassH+1)||(c == BRICK_ShotPassM+1)) {
			} else {
				MapTouchBrick(x, y);
				ShotKill(idx);
			}
		}
	}

}

static void MoveShot(int idx)
{	
	switch(Shots[idx].Dir) {
		case SD_North:
			Shots[idx].Y -= SHOT_SPEED;			
			break;
		case SD_East:
			Shots[idx].X += SHOT_SPEED;
			break;
		case SD_South:
			Shots[idx].Y += SHOT_SPEED;
			break;
		case SD_West:
			Shots[idx].X -= SHOT_SPEED;
			break;
	}
}

void ShotTick(void)
{
	int i; 
	
	for(i=0; i<MAX_SHOTS; i++) {
		if(Shots[i].Enabled) {
			MoveShot(i);	
			CheckShot(i);	
		}
	}
}

void ShotDraw(void)
{
	int i; 

	for(i=0; i<MAX_SHOTS; i++) {
		if(Shots[i].Enabled) {
			switch(Shots[i].Dir) {
				case SD_North:
					line(VidBuffer, Shots[i].X, Shots[i].Y, Shots[i].X, Shots[i].Y + SHOT_LENGTH, makecol SHOT_COLOR);					
					break;
				case SD_East:
					line(VidBuffer, Shots[i].X, Shots[i].Y, Shots[i].X - SHOT_LENGTH, Shots[i].Y, makecol SHOT_COLOR);
					break;
				case SD_South:
					line(VidBuffer, Shots[i].X, Shots[i].Y, Shots[i].X, Shots[i].Y - SHOT_LENGTH, makecol SHOT_COLOR);
					break;
				case SD_West:
					line(VidBuffer, Shots[i].X, Shots[i].Y, Shots[i].X + SHOT_LENGTH, Shots[i].Y, makecol SHOT_COLOR);
					break;
			}
			putpixel(VidBuffer, Shots[i].X, Shots[i].Y, makecol SHOT_COLOR_2);
		}
	}
}

void ShotKill(int idx)
{
	int i;

	if(idx == -1) {
		for(i=0; i<MAX_SHOTS; i++) {
			Shots[i].Enabled = 0;
			GameIsQuiet = 0;
		}
	} else {
		Shots[idx].Enabled = 0;
		GameIsQuiet--;
	}
}

int NewShot(float x, float y, int dir)
{
	int i;
	
	for(i=0; i<MAX_SHOTS; i++) {
		if(!Shots[i].Enabled) {
			Shots[i].Enabled = 1;
			Shots[i].X = x;
			Shots[i].Y = y;
			Shots[i].Dir = dir;
			SndPlaySound(SND_Shot);
			GameIsQuiet++;
			return i;
			
		}
	}
	
	return -1;
}

