//
// Token.c: Token implementation
//
#include "Common.h"

// TokGfx: each entry contains all token frames
static BITMAP *TokGfx[TOK_MAX];
static token_t Tokens[MAX_FTOKENS];
static int GlobalFrame = 0;
static float IncFrame=0, IncFall=0;

static int LoadGfx(BITMAP *bmp, int idx)
{
	int y;

	y = TOKEN_HEIGHT * idx;
	
	TokGfx[idx] = create_bitmap(TOKEN_WIDTH*TOKEN_FRAMES, TOKEN_HEIGHT);
	if(!TokGfx[idx]) {
		alert("Not enough memory", NULL, NULL, "OK", NULL, 0, 0);
		return 0;
	}

	blit(bmp, TokGfx[idx], 0, y, 0, 0, TOKEN_WIDTH*TOKEN_FRAMES, TOKEN_HEIGHT);
	return 1;
}

static int AllocToken(void)
{
	int i;

	for(i=0; i<MAX_FTOKENS; i++) {
		if(Tokens[i].tok == -1)
			return i;
	}

	return -1;
}

int TokenInit(void)
{
	BITMAP *bmp;
	char *str;
	int i;

	for(i=0; i<MAX_FTOKENS; i++) {
		Tokens[i].tok = -1;
	}

	str = GetStaticChar1024();
	strcpy(str, GameDir);
	strcat(str, "gfx/tokens.bmp");

	bmp = load_bitmap(str, NULL);
	if(!bmp) {
		alert("Can't open", "gfx/tokens.bmp", NULL, "OK", NULL, 0, 0);
		return 0;
	}

	for(i=0; i<TOK_MAX; i++) {
		if(!LoadGfx(bmp, i))
			return 0;
	}

	destroy_bitmap(bmp);
	return 1;
}

void TokenDeinit(void)
{
	int i;

	for(i=0; i<TOK_MAX; i++) {
		destroy_bitmap(TokGfx[i]);
		TokGfx[i] = NULL;
	}
}

void TokenAnim(void)
{
	int i, x;

	// animate token rotation
	IncFrame -= TimeDT;
	if(IncFrame <= 0.0f) {
		IncFrame = 1/TOKEN_FPS;
		GlobalFrame = (GlobalFrame+1) % 6;
	}

	// fall tokens
	IncFall -= TimeDT;
	if(IncFall <= 0.0f) {
		IncFall = 1/TOKEN_FSPEED;
	
		for(i=0; i<MAX_FTOKENS; i++) {
			if(Tokens[i].tok != -1) {
				Tokens[i].Y++;
				if(Tokens[i].Y >= SCR_HEIGHT) {
					Tokens[i].tok = -1;
				}

				if(((Tokens[i].Y+TOKEN_HEIGHT) >= Pad.Y)&&((Tokens[i].Y) <= Pad.Y + PAD_HEIGHT)) {
					x = Tokens[i].X;
					if((x >= Pad.X) && (x<= (Pad.X+PadGetLen()))) {
						GameGetToken(Tokens[i].tok);
						Tokens[i].tok = -1;
						break;
					}

					x = Tokens[i].X + TOKEN_WIDTH;
					if((x >= Pad.X) && (x<= (Pad.X+PadGetLen()))) {
						GameGetToken(Tokens[i].tok);
						Tokens[i].tok = -1;
						break;
					}
				}
			}
		}
	}
}

int TokenNew(int X, int Y, int Type)
{
	int idx;

	if(Options[OPT_Powerups])
	{
	
		idx = AllocToken();
		if(idx != -1) {
			Tokens[idx].X = X;
			Tokens[idx].Y = Y;
			Tokens[idx].tok = Type;
		}
		return idx;
	}
	
	return -1;
}

void TokenKill(int idx)
{
	int i;

	if(idx == -1) {
		for(i=0; i<MAX_FTOKENS; i++) {
			Tokens[i].tok = -1;
		}
	} else {
		Tokens[idx].tok = -1;
	}
}

void TokenDraw(int X, int Y, int Frame, enum E_Tokens Type)
{
	masked_blit(TokGfx[Type], VidBuffer, Frame*TOKEN_WIDTH, 0, X, Y, TOKEN_WIDTH, TOKEN_HEIGHT);
}

void TokenRender(void)
{
	int i;

	for(i=0; i<MAX_FTOKENS; i++) {
		if(Tokens[i].tok != -1) {
			TokenDraw(Tokens[i].X, Tokens[i].Y, GlobalFrame, Tokens[i].tok);
		}
	}
}

int TokenGetFrame(void)
{
	return GlobalFrame;
}
