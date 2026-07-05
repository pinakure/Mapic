//
// Hiscore.h: HiScore system
//
#ifndef __HISCORE_H
#define __HISCORE_H


extern int HiScoreSetting;

/*
============
SaveHiScore
============
	Saves the Hi-Score table to disk. Returns 0 if error.
*/
int SaveHiScore(void);

/*
================
HiScoreAddEntry
================
	Shifts all entries below the given index from the Hi-Score table.
*/
void HiScoreAddEntry(int idx);

/*
============
LoadHiScore
============
	Loads the Hi-Score table from disk.
*/
int LoadHiScore(void);

/*
============
HiScoreInit
============
	Initializes hi-score memory and loads hiscore from disk.
	If loading fails, it attempts to create a new file.
	Returns 0 if creating the file fails.
*/
int HiScoreInit(void);

/*
==============
HiScoreDeinit
==============
	Deinitializes hi-score memory.
*/
void HiScoreDeinit(void);

#endif

