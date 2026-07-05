//
// Hiscore.h: HiScore system
//
#include "Common.h"

int HiScoreSetting = 0;


static void NewHiScore(void)
{
	int i;
	
	strcpy(HiScoreNames[0], "Mitikoro");
	strcpy(HiScoreNames[1], "Smiker");
	HiScores[0] = 0xFFFFFFFF;
	HiScores[1] = 37359270;	

	for(i=2;i<10;i++)
	{
		strcpy(HiScoreNames[i], "NoName");
		HiScores[i] = (10-i)*100;
	}
	
}

void HiScoreAddEntry(int idx)
{
	int i;

	for(i=9; i > idx; i--){
		strcpy(HiScoreNames[i], HiScoreNames[i-1]);
		HiScores[i] = HiScores[i-1];
	}
}

int SaveHiScore(void)
{
	FILE *fp;
	char *str;
	int i;

	str = GetStaticChar1024();
	strcpy(str, GameDir);
	strcat(str, "hiscore.dat");

	fp = fopen(str, "wb");
	*str = 0;
	if(!fp) {		
		return 0;		
	}

	for(i=0; i<10; i++) {		
		fprintf(fp, "%u,%s\n", HiScores[i], HiScoreNames[i]);
	}
	
	fclose(fp);
	
	return 1;
}

int LoadHiScore(void)
{
	FILE *fp;
	char *str;
	int i;

	str = GetStaticChar1024();
	strcpy(str, GameDir);
	strcat(str, "hiscore.dat");

	fp = fopen(str, "rb");
	*str = 0;
	if(!fp) {		
		return 0;		
	}
	for(i=0;i<10;i++)
	{		
		fscanf (fp, "%u,%s\n", &HiScores[i], HiScoreNames[i]);				
	}	
	fclose(fp);
	
	return 1;
}

void HiScoreDeinit(void)
{
	int i;
	for(i=0;i<10;i++)
	{
		if(HiScoreNames[i])free(HiScoreNames[i]);
		HiScoreNames[i] = NULL;
	}
}

int HiScoreInit(void)
{
	int i;
	
	for(i=0;i<10;i++)
	{
		HiScoreNames[i]	 = malloc(32);
		*HiScoreNames[i] = 0;
		HiScores[i]		 = 0;
	}

	if(!LoadHiScore()) {
		NewHiScore();
		if(!SaveHiScore())
		{
			alert("Can't create", "HiScore file!", NULL, "OK", NULL, 0, 0);
			return 0;
		};		
		HiScoreDeinit();
		return(HiScoreInit());
	}	
	return 1;
}