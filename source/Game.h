//
// Game.h: Game logic
//
#ifndef __GAME_H
#define __GAME_H

extern int GameIsQuiet;
extern int GameHasStarted;
extern int GameIsTesting;
extern int GameIsOver;

/*
========
GameNew
========
	This function loads the given map, resets the game score,
	lives...
*/
void GameNew(char *map);

/*
=========
GameTest
=========
	This function sets the lives to 1 and prepares to return to the editor when level is over.
*/
void GameTest(void);


/*
==============
GameChangeMap
==============
	This function loads a new map.
*/
void GameChangeMap(char *map);

/*
=============
GameEndLevel
=============
	This function ends the current level and jumps to the
	next one.
*/
void GameEndLevel(void);

/*
=============
GameAddScore
=============
	Increments the score counter. add may be negative.
*/
void GameAddScore(int add);

/*
============
GameBallOut
============
	This function signals the game that the player's ball
	has exited the map.
*/
void GameBallOut(void);

/*
=============
GameGetToken
=============
	When this function is called, the game will set the new
	powerup to tok (disabling the old one).
*/
void GameGetToken(int tok);

/*
===============
GameBreakBrick
===============
	This function is called when a brick is broken; it increments
	the score counter.
*/
void GameBreakBrick(int type);

/*
=================
GameSetChooseDir
=================
	Makes the game go to choose dir mode and the ball stands still.
*/
void GameSetChooseDir(void);

/*
============
GameJumpMap
============
	This function changes the current map.
	If Rel==1, then Map is an offset from the current map.
*/
void GameJumpMap(int Map, int Rel);

/*
=========
GameTick
=========
	Advances game logic.
*/
int GameTick(void);

/*
=========
GameTilt
=========
	Produces a random ball tilt and gives a score penalty.
*/
void GameTilt(int axis);

/*
=================
GameCurrentLevel
=================
	Returns current level number.
*/
int GameCurrentLevel(void);

/*
=========
GameDraw
=========
	Draws the game.
*/
void GameDraw(void);

/*
==============
GameSetInGame
==============
	Makes the game enter ingame mode.
*/
void GameSetInGame(void);



/*
==================
GameIsInChooseDir
==================
	Returns 1 if the game is in choose dir mode.
*/
int GameIsInChooseDir(void);

/*
===============
GameIsStarting
===============
	Returns 1 if the game is starting.
*/
int GameIsStarting(void);

//
// GameToken
// When -1, no token is in effect
// If >= 0, then that token is in effect (one value of the TOK_<xxx> enum)
//
extern int GameToken;
extern unsigned int HiScores[10];
extern char *HiScoreNames[10];

#endif
