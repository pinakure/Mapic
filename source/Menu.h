//
// Menu.h: Menu system
//
#ifndef MENU_H
#define MENU_H


/*
=========
MenuTick
=========
	Process the menu system.
*/
int MenuTick(void);

/*
===============
MenuSetHiScore
===============
	Sets menu system to hiscore submenu and highlights option given.
*/
void MenuSetHiScore(int idx, int Score);

/*
=========
MenuDraw
=========
	Draws the menu system.
*/
void MenuDraw(void);


#endif