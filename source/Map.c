//
// Map.c: Map system
//
#include "Common.h"

#define MAP_TSIZE	(MAP_WIDTH*MAP_HEIGHT)
map_t Map;
char **MapList=NULL;
int MapListCount;

static jmp_buf JmpBuf;

static int BlinkIdx = -1;

int GameIsQuiet = 0;
int GameIsTesting = 0;
int GameHasStarted = 0;

static void SafeRead(FILE *fp, void *ptr, size_t count)
{
	size_t l;
	char *s2=NULL;

	l = count;
	while(count > 0) {
		l = fread(ptr, 1, l, fp);
		if(l < count) {
			if(ferror(fp)) {
				if(fclose(fp)) {
					s2 = "can't close file!";
				}

				alert("Error during read", s2, NULL, "OK", NULL, 0, 0);
				longjmp(JmpBuf, 1);
			}

			if(feof(fp)) {
				if(fclose(fp)) {
					s2 = "can't close file!";
				}

				alert("Unexpected EOF", s2, NULL, "OK", NULL, 0, 0);
				longjmp(JmpBuf, 1);
			}
		}

		count -= l;
	}
}

static unsigned char ReadByte(FILE *fp)
{
	unsigned char ret;

	SafeRead(fp, &ret, 1);
	return ret;
}

static unsigned int ReadDword(FILE *fp)
{
	unsigned int ret;

	SafeRead(fp, &ret, 4);
	return ret;
}

static void ReadStr(FILE *fp, char *str, int count)
{
	int i;

	for(i=0; i<count; i++) {
		str[i] = ReadByte(fp);
		if(!str[i]) {
			return;
		}
	}

	alert("String too big", NULL, NULL, "OK", NULL, 0, 0);
	if(fclose(fp)) {
		alert("Can't close file", NULL, NULL, "OK", NULL, 0, 0);
	}

	longjmp(JmpBuf, 1);
}

int MapLoad(char *fname)
{
	char *str;
	int ch;
	FILE *fp;
	unsigned int magic;
	int i;

	if(!ValidFilename(fname, "MAP")) {
		alert("Map filename not valid", fname, NULL, "OK", NULL, 0, 0);
		return 0;
	}

	str = GetStaticChar1024();
	strcpy(str, GameDir);
	strcat(str, "maps/");
	strcat(str, fname);

	fp = fopen(str, "rb");
	if(!fp) {
		alert("Can't open", str, NULL, "OK", NULL, 0, 0);
		return 0;
	}

	if(setjmp(JmpBuf) == 1) {
		return 0;
	}

	magic = ReadDword(fp);
	if(magic != MAP_MAGIC) {
		alert("Map header incorrect", "(invalid magic)", NULL, "OK", NULL, 0, 0);
		if(fclose(fp)) {
			alert("Can't close", str, NULL, "OK", NULL, 0, 0);
		}

		return 0;
	}

	ReadStr(fp, Map.Name, sizeof(Map.Name));
	ReadStr(fp, Map.Bg, sizeof(Map.Bg));

	Map.BricksLeft = 0;
	for(i=0; i<MAP_TSIZE; i++) {
		Map.Bricks[i].Type = (int) ((signed char)ReadByte(fp));
		Map.Bricks[i].Token = (int) ((signed char)ReadByte(fp));
		
		if(Map.Bricks[i].Type) {
			switch(Map.Bricks[i].Type-1) {
				case BRICK_NonBreak:
				case BRICK_ShotPassV:
				case BRICK_ShotPassH:
				case BRICK_ShotPassM:
					break;
				default:
					Map.BricksLeft++;
					break;
			}		

			if(Map.Bricks[i].Type == (BRICK_Break1+1))
				Map.Bricks[i].HitLeft = 1;
			else if(Map.Bricks[i].Type == (BRICK_Break2+1))
				Map.Bricks[i].HitLeft = 2;
			else if(Map.Bricks[i].Type == (BRICK_Break3+1))
				Map.Bricks[i].HitLeft = 3;
		}
	}

	/* get mapinfo */
	i = 0;
	while(i < MAX_MAPINFO_SIZE) {
		ch = fgetc(fp);
		if(!ch || (ch == EOF)) {
			break;
		} else {
			Map.Info[i] = ch;
			i++;
		}
	}
	Map.Info[i] = 0;
	
	if(!strlen(Map.Info)) {
		strcpy(Map.Info, "Map doesn't contain any info.\nPlease add some tips or credits.\n");
	}

	if(fclose(fp)) {
		alert("Can't close", str, NULL, "OK", NULL, 0, 0);
		return 0;
	}

	BgUnload();
	
	if(strlen(Map.Bg)) {
		BgLoad(Map.Bg);
	}

	return 1;
}

void MapLoadDef(void)
{
#if 1
	memset(&Map, 0, sizeof Map);
#else
	int i, cnt=0;

	Map.BricksLeft = MAP_TSIZE;
	for(i=0; i<MAP_TSIZE; i++) {
		Map.Bricks[i].Type = (i%64)+1;
	}
#endif
}

static int BreakBrick(int idx, int X, int Y)
{
	
	GameBreakBrick(Map.Bricks[idx].Type);

	if(Map.Bricks[idx].Type == (BRICK_Bomb+1)) {
		NewExplosion(X, Y);
	}

	Map.Bricks[idx].Type = 0;
	Map.BricksLeft--;

	if(Map.BricksLeft > 0) {
		if(Map.Bricks[idx].Token) {
			if(Map.Bricks[idx].Token > 0) {
				TokenNew(
					3+(BRICK_WIDTH*(idx%MAP_WIDTH))+(BRICK_WIDTH/2)-(TOKEN_WIDTH/2),
					3+(BRICK_HEIGHT*(idx/MAP_WIDTH)),
					Map.Bricks[idx].Token-1);
			} else {
				if(rand() > ((6*RAND_MAX)/8)) {
					TokenNew(
						3+(BRICK_WIDTH*(idx%MAP_WIDTH))+(BRICK_WIDTH/2)-(TOKEN_WIDTH/2),
						3+(BRICK_HEIGHT*(idx/MAP_WIDTH)),
						rand() % TOK_MAX);
				}
			}
		}

		return 1;
	} else {
		if(GameIsQuiet == 0){
			GameEndLevel();
		}
		return 0;
	}
}

int MapTouchBrick(int X, int Y)
{
	int idx;

	if(X < 0) return 1;
	if(X >= MAP_WIDTH) return 1;
	if(Y < 0) return 1;
	if(Y >= MAP_HEIGHT) return 1;

	idx = (Y*MAP_WIDTH)+X;
	if(!Map.Bricks[idx].Type) return 1;

	switch(Map.Bricks[idx].Type - 1) {
		case BRICK_NonBreak:
		case BRICK_ShotPassH:
		case BRICK_ShotPassV:
		case BRICK_ShotPassM:
			GameAddScore(1);
			HudHighlight(HL_Neutral);
			BlinkIdx = idx;
			return 1;
		
		case BRICK_Break1:
		case BRICK_Break2:
		case BRICK_Break3:
			BlinkIdx = idx;
			GameAddScore(10);
			HudHighlight(HL_Good);
			Map.Bricks[idx].HitLeft--;
			if(Map.Bricks[idx].HitLeft >= 0)
				return 1;
			break;

		case BRICK_ShotLeft:
			NewShot( (X*BRICK_WIDTH) + (BRICK_WIDTH / 2), 2+(Y*BRICK_HEIGHT) + (BRICK_HEIGHT/2), SD_West); 
			return BreakBrick(idx, X, Y);			

		case BRICK_ShotUp:
			NewShot( 2+(X*BRICK_WIDTH) + (BRICK_WIDTH / 2), (Y*BRICK_HEIGHT) + (BRICK_HEIGHT/2), SD_North); 
			return BreakBrick(idx, X, Y);

		case BRICK_ShotRight:
			NewShot( (X*BRICK_WIDTH) + (BRICK_WIDTH / 2), 2+(Y*BRICK_HEIGHT) + (BRICK_HEIGHT/2), SD_East); 
			return BreakBrick(idx, X, Y);

		case BRICK_ShotDown:
			NewShot( 2+(X*BRICK_WIDTH) + (BRICK_WIDTH / 2), (Y*BRICK_HEIGHT) + (BRICK_HEIGHT/2), SD_South); 
			return BreakBrick(idx, X, Y);
			
		case BRICK_Multi1:
		case BRICK_Multi2:
			BlinkIdx = idx;
			GameAddScore(10);
			HudHighlight(HL_Good);
			Map.Bricks[idx].Type++;
			return 1;				
	}

	if(Map.Bricks[idx].Type) {
		if(Map.Bricks[idx].Type != (BRICK_NonBreak+1)) {
			if(Map.Bricks[idx].Type == (BRICK_Break1+1)) {
				Map.Bricks[idx].HitLeft--;
				if(Map.Bricks[idx].HitLeft <= 0)
					return BreakBrick(idx, X, Y);
				
			} else if(Map.Bricks[idx].Type == (BRICK_Break2+1)) {
				Map.Bricks[idx].HitLeft--;
				if(Map.Bricks[idx].HitLeft <= 0)
					return BreakBrick(idx, X, Y);

			} else if(Map.Bricks[idx].Type == (BRICK_Break3+1)) {
				Map.Bricks[idx].HitLeft--;
				if(Map.Bricks[idx].HitLeft <= 0)
					return BreakBrick(idx, X, Y);

			} else {
				return BreakBrick(idx, X, Y);
			}
		}
	}

	return 1;
}

int MapTestBrick(int X, int Y)
{
	int idx;

	if(X < 0) return 0;
	if(X >= MAP_WIDTH) return 0;
	if(Y < 0) return 0;
	if(Y >= MAP_HEIGHT) return 0;

	idx = (Y*MAP_WIDTH)+X;
	return Map.Bricks[idx].Type;
}

void MapDraw(int SX, int SY)
{
	int x, y, i;

	x = SX;
	y = SY;

	for(i=0; i<MAP_TSIZE; i++) {
		if(Map.Bricks[i].Type > 0) {
			if(i == BlinkIdx) {
				rectfill(VidBuffer, x, y, x + (BRICK_WIDTH - 1), y + (BRICK_HEIGHT - 1), makecol(255,255,250));
				BlinkIdx=-1;
			} else {
				DrawBrick(x, y, Map.Bricks[i].Type-1);			
			}
		}

		x += BRICK_WIDTH;
		if(x >= ((MAP_WIDTH*BRICK_WIDTH)-SX)) {
			x = SX;
			y += BRICK_HEIGHT;
		}
	}
}

static int ReadLine(FILE *fp, char *buf, int sz)
{
	int ch, i;

	while(1) {
		ch = fgetc(fp);
		if(ch == EOF) {
			return 0;
		}

		if(isalnum(ch)) {
			break;
		}
	}

	for(i=0; i<sz; i++) {
		if(ch == EOF) {
			buf[i] = 0;
			return 1;
		} else if(isspace(ch)) {
			buf[i] = 0;
			return 1;
		} else {
			buf[i] = ch;
		}
		ch = fgetc(fp);
	}

	return -1;
}

static int AddMap(char *map)
{
	map = strdup(map);
	if(!map) {
		alert("Not enough memory", NULL, NULL, "OK", NULL, 0, 0);
		return 0;
	}

	if(!MapList) {
		MapList = (char **) malloc(sizeof(char *));
		if(!MapList) {
			alert("Not enough memory", NULL, NULL, "OK", NULL, 0, 0);
			return 0;
		}
		MapList[0] = map;
		MapListCount = 1;
	} else {
		MapList = (char **) realloc(MapList, (MapListCount+1)*sizeof(char *));
		if(!MapList) {
			alert("Not enough memory", NULL, NULL, "OK", NULL, 0, 0);
			return 0;
		}

		MapList[MapListCount] = map;
		MapListCount++;
	}

	return 1;
}

int MapLoadList(void)
{

	static char buf[1024];
	char *path;
	FILE *fp;
	int s;

	path = GetStaticChar1024();
	strcpy(path, GameDir);
	strcat(path, "maplist.txt");
	
	fp = fopen(path, "rb");
	if(!fp) {
		alert("Can't open", "maplist.txt", NULL, "OK", NULL, 0, 0);
		return 0;
	}

	while(1) {
		s = ReadLine(fp, buf, sizeof buf);
		if(!s) break;
		else if(s == -1) {
			alert("Invalid maplist entry", NULL, NULL, "OK", NULL, 0, 0);
			return 0;
		}

		if(!AddMap(buf)) {
			return 0;
		}
	}

	if(fclose(fp)) {
		alert("Can't close", "maplist.txt", NULL, "OK", NULL, 0, 0);
		return 0;
	}

	if(!MapListCount) {
		alert("No maps in maplist", NULL, NULL, "OK", NULL, 0, 0);
		return 0;
	}

	return 1;
}

void MapFreeList(void)
{
	while(MapListCount) {
		free(MapList[MapListCount-1]);
		MapListCount--;
	}
	free(MapList);
}
