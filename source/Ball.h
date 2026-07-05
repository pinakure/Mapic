//
// Ball.h: Ball code, header file
//
#ifndef __BALL_H
#define __BALL_H

// define to show velocity vectors
//#define BALL_SHOW_DIR

#define MAX_BALLS	32
#define TheBall		(Balls[0])

// define to sync the ball's velocity to the current framerate
//#define BALL_SYNC

#ifndef BALL_SYNC
#define BALL_INITIAL_DX		-BallSpeed
#define BALL_INITIAL_DY		-BallSpeed

#define BALL_INITIAL_Y		Pad.Y - (BALL_HEIGHT / 2.0f)

#else
#define BALL_INITIAL_DX		-100
#define BALL_INITIAL_DY		-100
#endif

#define BALL_WIDTH			4
#define BALL_HEIGHT			4

enum E_BallTypes {
	BT_Free = -1,
	BT_Normal = 0, 
	BT_Power, 
	BT_Hollow, 
	BT_Mini, 
	BT_Debug,
	BT_MAX
};

typedef struct ball_s {
	enum E_BallTypes Type;
	//int Type;		// -1=ball is free for NewBall
	int Move;		// 0: no move
	float X, Y;

	// DX,DY: velocity (when BALL_SYNC in pix/s)
	float DX, DY;

	// LX,LY: last ball position
	int	  LX, LY;
	
	// NX,NY: next ball position
	int	  NX, NY;
} ball_t;

extern ball_t Balls[MAX_BALLS];
extern float BallSpeed;
extern int Kamikaze;

/*
==========
InitBalls
==========
	Initializes all ball graphics. Returns 0 on error.
*/
int InitBalls(void);

/*
============
DeinitBalls
============
	Frees all resources allocated with InitBalls.
*/
void DeinitBalls(void);

/*
========
NewBall
========
	Creates a new ball with the given parameters. Returns the ball
	index on success, or -1 if no free balls are avaiable.
*/
int NewBall(float X, float Y, float DX, float DY, enum E_BallTypes Type, int Moving);

/*
=========
KillBall
=========
	Frees the given ball, so it can be allocated with NewBall again.
*/
void KillBall(int idx);

/*
=========
DrawBall
=========
	Draws the ball sprite to (X,Y).
*/
void DrawBall(int X, int Y, int Type);

/*
============
DrawBallVel
============
	Draws a vector representing the ball's direction. The vector
	is scaled by the parameter Scale.
*/
void DrawBallVel(ball_t *b, float Scale);

/*
==========
DrawBalls
==========
	Draws all active balls.
*/
void DrawBalls(void);

/*
==========
MoveBalls
==========
	Moves all active balls.
*/
void MoveBalls(void);

/*
==========
TiltBalls
==========
	Tilts balls.
*/
void TiltBalls(int axis);

/*
============
BallExplode
============
	Kills the ball and blasts everything around it.
*/
void BallExplode(void);

#endif
