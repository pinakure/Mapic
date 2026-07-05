//
// Edit.c: Editor code
//
#include "Common.h"
//
//static int InfoSize = 0;
static int Help     = 0;

char InfoLine[6][MAX_MAPINFO_LINE+1];

static DIALOG MapDlg[] = {
	/* (dialog proc)     (x)   (y)   (w)   (h) (fg)(bg) (key) (flags)			   (d1) (d2)    (dp)                   (dp2) (dp3) */

	{ d_clear_proc,        0,   0,  0,    0,   0,  0,    0,      0,					0,   0,    NULL,                   NULL, NULL  },

	{ d_text_proc,       20, 10,    0,    0,   0,  0,    0,      0,					0,   0,    "Map Properties",          NULL, NULL  },

	{ d_text_proc,       20, 30,    0,    0,   0,  0,    0,      0,					0,   0,    "   Title:",          NULL, NULL  },
	{ d_text_proc,       20, 40,    0,    0,   0,  0,    0,      0,					0,   0,    "Backdrop:",          NULL, NULL  },
	{ d_text_proc,       20, 50,    0,    0,   0,  0,    0,      0,					0,   0,    "Map Info:",          NULL, NULL  },

	{ d_edit_proc,       140, 30,  160,    8,   0,  0,    0,      0,		       32,   0,    Map.Name,            NULL, NULL  },
	{ d_edit_proc,       140, 40,  160,    8,   0,  0,    0,      0,			   32,   0,    Map.Bg,              NULL, NULL  },

	{ d_edit_proc,		 140,  50,  160,    0,   0,  0,   0,      0, MAX_MAPINFO_LINE,   0,    InfoLine[0],            NULL, NULL  },
	{ d_edit_proc,		 140,  60,  160,    0,   0,  0,   0,      0, MAX_MAPINFO_LINE,   0,    InfoLine[1],            NULL, NULL  },
	{ d_edit_proc,		 140,  70,  160,    0,   0,  0,   0,      0, MAX_MAPINFO_LINE,   0,    InfoLine[2],            NULL, NULL  },
	{ d_edit_proc,		 140,  80,  160,    0,   0,  0,   0,      0, MAX_MAPINFO_LINE,   0,    InfoLine[3],            NULL, NULL  },
	{ d_edit_proc,		 140,  90,  160,    0,   0,  0,   0,      0, MAX_MAPINFO_LINE,   0,    InfoLine[4],            NULL, NULL  },
	{ d_edit_proc,		 140, 100,  160,    0,   0,  0,   0,      0, MAX_MAPINFO_LINE,   0,    InfoLine[5],            NULL, NULL  },

	{ d_button_proc,     20,   200,  80,   20,   0,  0,  'e', D_EXIT,				0,   0,    "&Exit",                NULL, NULL  },

	{ NULL,                0,   0,    0,    0,   0,  0,    0,      0,				0,   0,    NULL,                   NULL, NULL  }
};

static int CsrX=0, CsrY=0;
static int ShowGrid = 1, InGrid=0, ShowBg=1;
static int CurTile = 0, CurToken=0;
static jmp_buf JmpBuf;

static void SafeWrite(FILE *fp, void *ptr, size_t count)
{
	size_t l;
	char *s2=NULL;

	l = count;
	while(count > 0) {
		l = fwrite(ptr, 1, l, fp);
		if(l < count) {
			if(ferror(fp)) {
				if(fclose(fp)) {
					s2 = "can't close file!";
				}

				alert("Error during write", "disk full?", s2, "OK", NULL, 0, 0);
				longjmp(JmpBuf, 1);
			}
		}

		count -= l;
	}
}

static void WriteStr(FILE *fp, char *str)
{
	SafeWrite(fp, str, strlen(str) + 1);
}

static void WriteByte(FILE *fp, unsigned char val)
{
	SafeWrite(fp, &val, 1);
}

static void WriteDword(FILE *fp, unsigned int val)
{
	SafeWrite(fp, &val, 4);
}

static int SaveMap(char *str)
{
	FILE *fp;
	int i;

	fp = fopen(str, "wb");
	if(!fp) {
		alert("Cannot open", str, "for writing", "OK", NULL, 0, 0);
		return 0;
	}

	if(setjmp(JmpBuf) == 1) {
		return 0;
	}

	WriteDword(fp, MAP_MAGIC);
	WriteStr(fp, Map.Name);
	WriteStr(fp, Map.Bg);

	for(i=0; i<(MAP_WIDTH*MAP_HEIGHT); i++) {
		WriteByte(fp, Map.Bricks[i].Type);
		WriteByte(fp, Map.Bricks[i].Token);
	}

	if(strlen(Map.Info)){
		WriteStr(fp, Map.Info);
	}

	if(fclose(fp)) {
		alert("Cannot close", str, NULL, "OK", NULL, 0, 0);
		return 0;
	}

	return 1;
}

static void RecountBricks(void)
{
	int i;

	Map.BricksLeft = 0;
	for(i=0; i<(MAP_WIDTH*MAP_HEIGHT); i++) {
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
		}
	}
}

static void DrawGrid(void)
{
	int x, y;

	for(y=0; y<MAP_HEIGHT; y++) {
		for(x=0; x<MAP_WIDTH; x++) {
			rect(VidBuffer, 3+(x*BRICK_WIDTH), 3+(y*BRICK_HEIGHT),
							2+((x+1)*BRICK_WIDTH), 2+((y+1)*BRICK_HEIGHT),
							makecol(255, 255, 255));
		}
	}
}

static void DrawBList(void)
{
	int x, y, i;

	x = 3;
	y = 205;

	for(i=0; i<64; i++) {
		DrawBrick(x, y, i);

		if(i == CurTile) {
			rect(VidBuffer, x, y, x+BRICK_WIDTH-1, y+BRICK_HEIGHT-1,
				makecol(255, 0, 0));
		}

		x += BRICK_WIDTH;
		if(x >= (BRICK_WIDTH*16)) {
			x = 3;
			y += BRICK_HEIGHT;
		}
	}

	rect(VidBuffer, 2, 204, 3+(BRICK_WIDTH*16), 205+(BRICK_HEIGHT*4), makecol(0,0,0));
}

static void MapInfoCat(void)
{
	int i;

	*Map.Info = 0;

	for(i=0; i<6; i++) {
		if(strlen(InfoLine[i])) {
			strcat(Map.Info, InfoLine[i]);
		}
		strcat(Map.Info, "\n");
	}
}

static void MapInfoSplit(void)
{
	char *p, *in;
	int i;

	i = 0;
	in = Map.Info;
	p = InfoLine[i];
	while(*in) {
		switch(*in) {
			case '\n':
				*p = 0;
				i++;
				p = InfoLine[i];
				break;

			default:
				*p++ = *in;
				break;
		}

		in++;
	}
	*p = 0;
}

static int HandleKey(int k)
{
	char *str, *p;

	switch(k) {
		case KEY_Q:
			if(CurToken > 0) CurToken--;
			break;
		case KEY_W:
			CurToken = 0;
			break;
		case KEY_E:
			if(CurToken < TOK_MAX) CurToken++;
			break;
		case KEY_R:
			CurToken = -1;
			break;
		case KEY_T:
			CurToken = Map.Bricks[(CsrY*MAP_WIDTH)+CsrX].Token;
			break;

		case KEY_B:
			ShowBg ^= 1;
			break;

		case KEY_G:
			ShowGrid ^= 1;
			break;

		case KEY_N:
			if(key_shifts & KB_CTRL_FLAG) {
				memset(&Map, 0, sizeof(map_t));
			}
			break;

		case KEY_S:
			if(key_shifts & KB_CTRL_FLAG) {
				str = GetStaticChar1024();
				strcpy(str, GameDir);
				strcat(str, "maps/");

				if(file_select("Save map...", str, "MAP")) {
					SaveMap(str);
				}
			}
			break;

		case KEY_ESC:
			SndPlayMusic(SND_TitleMusic);
			return -1;

		case KEY_O:
			if(key_shifts & KB_CTRL_FLAG) {
				str = GetStaticChar1024();
				strcpy(str, GameDir);
				strcat(str, "maps/");

				if(file_select("Open map...", str, "MAP")) {
					str = strdup(str);
					p = str + strlen(str);
					while(p >= str) {
						if(*p == '\\') {
							p++;
							break;
						}

						if(*p == '/') {
							p++;
							break;
						}
						p--;
					}

					MapLoad(p);
					free(str);
				}
			}
			break;

		case KEY_F4:
			MapInfoSplit();
			do_dialog(MapDlg, -1);
			MapInfoCat();
			BgLoad(Map.Bg);
			break;

		case KEY_TAB:
			RecountBricks();
			clear_keybuf();
			GameTest();
			return 0;
	}

	return 1;
}

void EditInit(void)
{
	int i;

	for(i=0; MapDlg[i].proc; i++) {
		MapDlg[i].bg = 1;
		MapDlg[i].fg = 233;

	}
}

int EditTick(void)
{
	int x, y, k;

	if(key[KEY_F1]) {
		Help = 1;
	} else {
		Help = 0;
	}

	if(!Help)
	{
		if(MouseDX || MouseDY) {
			if((MouseX >= 3) && (MouseX < (3+(MAP_WIDTH*BRICK_WIDTH)))) {
				if((MouseY >= 3) && (MouseY < (3+(MAP_HEIGHT*BRICK_HEIGHT)))) {
					CsrX = (MouseX-3) / BRICK_WIDTH;
					CsrY = (MouseY-3) / BRICK_HEIGHT;
					InGrid = 1;
				} else {
					InGrid = 0;
				}
			} else {
				InGrid = 0;
			}
		}

		if((MouseX>=0) && (MouseX < (16*BRICK_WIDTH))) {
			if((MouseY >= (SCR_HEIGHT-(4*BRICK_HEIGHT)))) {
				x = MouseX / BRICK_WIDTH;
				y = (MouseY - (SCR_HEIGHT-(4*BRICK_HEIGHT))) / BRICK_HEIGHT;
				if(MouseB & 1) {
					CurTile = (y*16)+x;
				}
			}
		}

		if(InGrid) {
			if(MouseB & 1) {
				Map.Bricks[(CsrY*MAP_WIDTH)+CsrX].Type = CurTile+1;
				Map.Bricks[(CsrY*MAP_WIDTH)+CsrX].Token = CurToken;
				if((CurTile >= BRICK_Break1) && (CurTile <= BRICK_Break3)) {
					Map.Bricks[(CsrY*MAP_WIDTH)+CsrX].HitLeft = CurTile - BRICK_NonBreak;
				}
			} else if(MouseB & 2) {
				Map.Bricks[(CsrY*MAP_WIDTH)+CsrX].Type = 0;
			}
		}

		if(MouseDZ) {
			CurTile += MouseDZ;
			if(CurTile < 0) CurTile = (64 + CurTile) % 64;
			if(CurTile >= 64) CurTile = (CurTile-64)%64;
		}

		while(keypressed()) {
			k = HandleKey(readkey() >> 8);
				if(!k) {
				return 0;
			} else {
				if(k == -1)return -1;
			}
		}
	}
	return 1;
}

static char* GetTokenName(int idx)
{
	switch(idx) {
		case TOK_Double:	return "2x Bonus";
		case TOK_Half:		return "50%% Bogus";
		case TOK_Extra:		return "1 Up";
		case TOK_FarNext:	return "Big jump";
		case TOK_Next:		return "Next level";
		case TOK_Glue:		return "Glue pad";
		case TOK_Shot:		return "Shooter pad";
		case TOK_Invert:	return "Inverted pad";
		case TOK_Minus:		return "Short pad";
		case TOK_Plus:		return "Long pad";
		case TOK_Multi:		return "Multiball";
		case TOK_Slow:		return "Slow ball";
		case TOK_Power:		return "Power ball";
		case TOK_AutoChase: return "Auto Chase";
		case TOK_BoomBall:	return "Boom Ball";
	}
	return "Undefined";
}


void EditDraw(void)
{


	if(ShowBg) {
		BgDraw();
	}


	if(ShowGrid) {
		DrawGrid();
	}

	MapDraw(3, 3);

	HudDrawEditor();
	DrawBList();
	HudLCDPrint(11, 0, "F1: Help");
	if(CurToken == -1) {
		HudLCDPrint(5, 0, "Tok: Random");
		textprintf_ex(VidBuffer, font, HUD_TOKEN_X+3, HUD_TOKEN_Y-1, -1, -1, "?");
	} else if(!CurToken) {
		HudLCDPrint(5, 0, "Tok: None");
	} else {
		HudLCDPrint(5, 0, "%s", GetTokenName(CurToken-1));

		TokenDraw(HUD_TOKEN_X, HUD_TOKEN_Y, TokenGetFrame(), CurToken-1);
	}

	if(InGrid) {
		rect(VidBuffer,
				3+(CsrX*BRICK_WIDTH), 3+(CsrY*BRICK_HEIGHT),
				2+((CsrX+1)*BRICK_WIDTH), 2+((CsrY+1)*BRICK_HEIGHT),
				makecol(255, 0, 0));
	}

	if(Help)
	{
		HudDrawWindow();
		HudDrawString(SCR_WIDTH / 2, (SCR_HEIGHT / 2)-55, "HELP\n"
														  "------------------------------\n"
														  "C-o: Open map  C-s:  Save map \n"
														  "C-n: New map    F1:  This help\n"
														  "TAB: Test Map   F4: Map config\n"
														  " G : Grid       B :   Backdrop\n"
														  " E : No Token   R : Random Tok\n"
														  " T : Pick Token               \n"
														  "------------------------------\n"
														  "LMB: Set       RMB: Clear\n"
														  "Mouse Wheel: Select brick\n");
	}

}
