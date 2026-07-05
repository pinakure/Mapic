//
// Edit.h: Editor header
//
#ifndef __EDIT_H
#define __EDIT_H

//#define MAP_MAGIC			' KRA'		// 'ARK '
#define MAP_MAGIC			0x204B5241

// Map list coordinates
#define BRICKS_X	
#define BRICKS_Y

/*
=========
EditInit
=========
	This function initializes the editor.
*/
void EditInit(void);

/*
=========
EditTick
=========
	Advances the editor logic, handles key events...
	Returns 0 on return to game.
*/
int EditTick(void);

/*
=========
EditDraw
=========
	Draws the editor to screen.
*/
void EditDraw(void);

#endif
