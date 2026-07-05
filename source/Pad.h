#ifndef __PAD_H
#define __PAD_H

#define PAD_MINLEN 20
#define PAD_HEIGHT 4

#define PAD_MAX_SHOTS		4			// max number of active shots
#define PAD_FIRE_TIME		0.25f		// time between shots
#define PAD_FIRE_SPEED		1
#define PAD_AUTOCHASE_TIME  1000;
typedef struct pad_s
{
	int X, Y;
	int Mode;
	int ExtLen;
	int AutoChase;
	int Enabled;
} pad_t;

enum E_PadModes
{
	PM_Normal = 0,
	PM_Glue,
	PM_Shot
};

int PadInit(void);
void PadDeinit(void);
void PadReset(void);
void PadMove(int dx);
void PadSetColor(int r, int g, int b);
void PadTick(void);
void PadDraw(void);
void PadFire(void);
void PadAddLen(int add);
void PadResetLen(void);
int PadGetLen(void);
void PadDrawSprite(int X, int Y, int ExtLen);

extern pad_t Pad;

#endif
