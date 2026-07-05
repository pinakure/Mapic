//
// Token.h: Token system
//
#ifndef __TOKEN_H
#define __TOKEN_H

#define TOKEN_WIDTH		11
#define TOKEN_HEIGHT	7
#define TOKEN_FRAMES	6		// 6 frames for each line

// max number of falling tokens
#define MAX_FTOKENS			32
#define TOKEN_FSPEED		40.0f	// pix/s
#define TOKEN_FPS			25.0f	// frames/s

enum E_Tokens
{
	TOK_Plus = 0,
	TOK_Minus,
	TOK_Invert,
	TOK_Slow,
	TOK_Shot,
	TOK_Multi,
	TOK_Power,
	TOK_Glue,
	TOK_Extra,
	TOK_Double,
	TOK_Half,
	TOK_Next,
	TOK_FarNext,
	TOK_AutoChase,
	TOK_BoomBall,
	TOK_MAX
};

typedef struct token_s {
	int tok;	// -1: free entry
	int X, Y;
} token_t;

/*
==========
TokenInit
==========
	Initializes the token system.
*/
int TokenInit(void);

/*
============
TokenDeinit
============
	Frees resources allocated by TokenInit.
*/
void TokenDeinit(void);

/*
==========
TokenAnim
==========
	Animates all falling tokens.
*/
void TokenAnim(void);

/*
=========
TokenNew
=========
	Spawns a new token, falling from (X,Y). Returns -1 if no tokens
	were avaiable; otherwise, returns the token index (for TokenKill).
*/
int TokenNew(int X, int Y, int Type);

/*
==========
TokenKill
==========
	Kills a falling token. If given -1, then all falling tokens will be
	killed.
*/
void TokenKill(int idx);

/*
==========
TokenDraw
==========
	Draws one token, at the given pixel position.
	Frame is the frame to be drawn (from 0 to TOKEN_FRAMES-1).
*/
void TokenDraw(int X, int Y, int Frame, enum E_Tokens Type);

/*
==============
TokenGetFrame
==============
	Returns current rendered frame for the token.
*/
int TokenGetFrame(void);

/*
============
TokenRender
============
	Renders all falling tokens.
*/
void TokenRender(void);

#endif
