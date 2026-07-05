//
// Hud.c: HUD code
//
#include "Common.h"
#define HUD_TITLE_FPS		2.0f
static int Score=0, PadLeft=0;
static FONT *HudFontDigi;
FONT *HudFontRed, *HudFontGreen, *HudFontWhite, *HudFont;
static BITMAP *HudGfx, *HudGfxHi, *HudGfxHiBad, *HudGfxHiGood, *HudMapInfo, *HudMapEdit, *HudTitle, *HudPretitle;
static int TxtFgCol = 0;
static int HighlightHud = 0;
static int TiltHud = 0;
//static int HudBlink = 0;
static char MapTitle[32];
static int	MapTitlePos=0;
static int	MapTitleLen=20;
static float IncFrame=0.0f, MarqueeFrame=0.0f;

static char Credits[MAX_CREDITS][30] = {
									"",
									"",
									"M4GEEK Corporation",
									"Copyright 2011",
									"",
									"CREDITS",
									"",
									"",
									"CODE",
									"Mitikoro",
									"Smiker",
									"",
									"",
									"MUSIC",
									"Smiker",
									"",
									"",
									"SFX",
									"Smiker",
									"",
									"",
									"GFX",
									"Smiker",
									"",
									"",
									""
							 };


static float titletime = 5.0f;


void HudPreTitleDraw(void)
{
	blit(HudPretitle, VidBuffer, 0, 0, 0, 0, SCR_WIDTH, SCR_HEIGHT);
}


int HudPreTitleTick(void)
{
	titletime-=TimeDT;
	if(titletime > 0) return 1;

	return 0;
}

int HudInit(void)
{
	char *str;

	MapTitle[0] = 0;
	str = GetStaticChar1024();

	strcpy(str, GameDir);
	strcat(str, "gfx/hud/hud.bmp");
	HudGfx = load_bitmap(str, NULL);
	if(!HudGfx) {
		alert("Can't load", str, NULL, "OK", NULL, 0, 0);
		return 0;
	}

	strcpy(str, GameDir);
	strcat(str, "gfx/hud/hud_hi.bmp");
	HudGfxHi = load_bitmap(str, NULL);
	if(!HudGfxHi) {
		alert("Can't load", str, NULL, "OK", NULL, 0, 0);

		destroy_bitmap(HudGfx);
		return 0;
	}

	strcpy(str, GameDir);
	strcat(str, "gfx/hud/hud_hig.bmp");
	HudGfxHiGood = load_bitmap(str, NULL);
	if(!HudGfxHiGood) {
		alert("Can't load", str, NULL, "OK", NULL, 0, 0);

		destroy_bitmap(HudGfx);
		destroy_bitmap(HudGfxHi);
		return 0;
	}

	strcpy(str, GameDir);
	strcat(str, "gfx/hud/hud_hib.bmp");
	HudGfxHiBad = load_bitmap(str, NULL);
	if(!HudGfxHiBad) {
		alert("Can't load", str, NULL, "OK", NULL, 0, 0);
		destroy_bitmap(HudGfx);
		destroy_bitmap(HudGfxHi);
		destroy_bitmap(HudGfxHiGood);
		return 0;
	}

	/*load fonts*/
	strcpy(str, GameDir);
	strcat(str, "gfx/font/fontmono.bmp");
	HudFont = load_bitmap_font(str, NULL, NULL);
	if(!HudFont) {
		destroy_bitmap(HudGfx);
		destroy_bitmap(HudGfxHi);
		destroy_bitmap(HudGfxHiGood);
		destroy_bitmap(HudGfxHiBad);
		alert("Can't load", str, NULL, "OK", NULL, 0, 0);
		return 0;
	}

	strcpy(str, GameDir);
	strcat(str, "gfx/font/fontdigi.bmp");
	HudFontDigi = load_bitmap_font(str, NULL, NULL);
	if(!HudFontDigi) {
		destroy_bitmap(HudGfx);
		destroy_bitmap(HudGfxHi);
		destroy_bitmap(HudGfxHiGood);
		destroy_bitmap(HudGfxHiBad);
		destroy_font(HudFont);
		alert("Can't load", str, NULL, "OK", NULL, 0, 0);
		return 0;
	}

	strcpy(str, GameDir);
	strcat(str, "gfx/font/fontr.bmp");
	HudFontRed = load_bitmap_font(str, NULL, NULL);
	if(!HudFontRed) {
		destroy_bitmap(HudGfx);
		destroy_bitmap(HudGfxHi);
		destroy_bitmap(HudGfxHiGood);
		destroy_bitmap(HudGfxHiBad);
		destroy_font(HudFont);
		destroy_font(HudFontDigi);
		alert("Can't load", str, NULL, "OK", NULL, 0, 0);
		return 0;
	}

	strcpy(str, GameDir);
	strcat(str, "gfx/font/fontg.bmp");
	HudFontGreen = load_bitmap_font(str, NULL, NULL);
	if(!HudFontGreen) {
		destroy_bitmap(HudGfx);
		destroy_bitmap(HudGfxHi);
		destroy_bitmap(HudGfxHiGood);
		destroy_bitmap(HudGfxHiBad);
		destroy_font(HudFont);
		destroy_font(HudFontDigi);
		destroy_font(HudFontRed);
		alert("Can't load", str, NULL, "OK", NULL, 0, 0);
		return 0;
	}

	strcpy(str, GameDir);
	strcat(str, "gfx/font/fontw.bmp");
	HudFontWhite = load_bitmap_font(str, NULL, NULL);
	if(!HudFontWhite) {
		destroy_bitmap(HudGfx);
		destroy_bitmap(HudGfxHi);
		destroy_bitmap(HudGfxHiGood);
		destroy_bitmap(HudGfxHiBad);
		destroy_font(HudFont);
		destroy_font(HudFontDigi);
		destroy_font(HudFontRed);
		destroy_font(HudFontGreen);
		alert("Can't load", str, NULL, "OK", NULL, 0, 0);
		return 0;
	}

	strcpy(str, GameDir);
	strcat(str, "gfx/hud/mapinfo.bmp");
	HudMapInfo = load_bitmap(str, NULL);
	if(!HudMapInfo) {
		destroy_bitmap(HudGfx);
		destroy_bitmap(HudGfxHi);
		destroy_bitmap(HudGfxHiGood);
		destroy_bitmap(HudGfxHiBad);
		destroy_font(HudFont);
		destroy_font(HudFontDigi);
		destroy_font(HudFontRed);
		destroy_font(HudFontGreen);
		destroy_font(HudFontWhite);
		alert("Can't load", str, NULL, "OK", NULL, 0, 0);
		return 0;
	}

	strcpy(str, GameDir);
	strcat(str, "gfx/hud/hudedit.bmp");
	HudMapEdit = load_bitmap(str, NULL);
	if(!HudMapEdit) {
		destroy_bitmap(HudGfx);
		destroy_bitmap(HudGfxHi);
		destroy_bitmap(HudGfxHiGood);
		destroy_bitmap(HudGfxHiBad);
		destroy_bitmap(HudMapInfo);
		destroy_font(HudFont);
		destroy_font(HudFontDigi);
		destroy_font(HudFontRed);
		destroy_font(HudFontGreen);
		destroy_font(HudFontWhite);
		alert("Can't load", str, NULL, "OK", NULL, 0, 0);
		return 0;
	}

	strcpy(str, GameDir);
	strcat(str, "gfx/hud/title.bmp");
	HudTitle = load_bitmap(str, NULL);
	if(!HudMapEdit) {
		destroy_bitmap(HudGfx);
		destroy_bitmap(HudGfxHi);
		destroy_bitmap(HudGfxHiGood);
		destroy_bitmap(HudGfxHiBad);
		destroy_bitmap(HudMapInfo);
		destroy_bitmap(HudMapEdit);
		destroy_font(HudFont);
		destroy_font(HudFontDigi);
		destroy_font(HudFontRed);
		destroy_font(HudFontGreen);
		destroy_font(HudFontWhite);
		alert("Can't load", str, NULL, "OK", NULL, 0, 0);
		return 0;
	}

	strcpy(str, GameDir);
	strcat(str, "gfx/hud/pretitle.bmp");
	HudPretitle = load_bitmap(str, NULL);
	if(!HudPretitle) {
		destroy_bitmap(HudTitle);
		destroy_bitmap(HudGfx);
		destroy_bitmap(HudGfxHi);
		destroy_bitmap(HudGfxHiGood);
		destroy_bitmap(HudGfxHiBad);
		destroy_bitmap(HudMapInfo);
		destroy_bitmap(HudMapEdit);
		destroy_font(HudFont);
		destroy_font(HudFontDigi);
		destroy_font(HudFontRed);
		destroy_font(HudFontGreen);
		destroy_font(HudFontWhite);
		alert("Can't load", str, NULL, "OK", NULL, 0, 0);
		return 0;
	}

	TxtFgCol = makecol HUD_TEXT_COL;

	/*editor tunings*/
	//font = HudFont;
	//gui_bg_color = makecol(20,40,10);
	return 1;
}

void HudDeinit(void)
{
	destroy_bitmap(HudPretitle);
	destroy_bitmap(HudGfx);
	destroy_bitmap(HudGfxHi);
	destroy_bitmap(HudGfxHiGood);
	destroy_bitmap(HudGfxHiBad);
	destroy_bitmap(HudMapInfo);
	destroy_bitmap(HudMapEdit);
	destroy_bitmap(HudTitle);

	destroy_font(HudFont);
	destroy_font(HudFontDigi);
	destroy_font(HudFontRed);
	destroy_font(HudFontGreen);
	destroy_font(HudFontWhite);
}

void HudHighlight(int type)
{
	HighlightHud = type;
}

void HudDraw(void)
{
	int x, y, i;

	masked_blit(HudGfx, VidBuffer, 0, 0, 0, 0, HudGfx->w, HudGfx->h);

	HudLCDPrint(0, 0, "Score");
	HudLCDPrint(1, 0,"% 14u", Score);

	if(TiltHud) {
		HudLCDPrint(2, 0, "Tilt!    -100!");
	}

	x = HUD_PAD_X;
	y = HUD_PAD_Y;
	for(i=0; i<(PadLeft%18); i++) {
		PadDrawSprite(x, y, 0);

		x += PAD_MINLEN+3;
		if(x >= HUD_PAD_EX) {
			y += PAD_HEIGHT*2;
			x = HUD_PAD_X;
		}
	}

	if(PadLeft > 17) {
		textprintf_ex(VidBuffer, HudFontRed, HUD_PADLEFT_X, HUD_PADLEFT_Y, -1, -1, " +%d", PadLeft-(PadLeft%18));
	}



	if(GameToken != -1) {
		if(GameToken == TOK_Double) {
			HudLCDPrint(0, 0, "Score       x2");
		} else if(GameToken == TOK_Half) {
			HudLCDPrint(0, 0, "Score       /2");
		}
		TokenDraw(HUD_TOKEN_X, HUD_TOKEN_Y, TokenGetFrame(), GameToken);
	}

	ChopString(Map.Name, MapTitle, MapTitlePos, 14);
	MapTitleLen = (int)strlen(Map.Name);
	if(MapTitleLen > 13) {
		MapTitleLen -= 13;
	} else {
		MapTitleLen = 0;
	}

	HudLCDPrint(5, 0,"%s", MapTitle);
	HudLCDPrint(6, 0," Level  % 3d", GameCurrentLevel()+1);
	HudLCDPrint(7, 0,"Bricks  % 3d", Map.BricksLeft);
	HudLCDPrint(9, 0, "High Score");
	HudLCDPrint(10,0, "%  14s", HiScoreNames[0]);
	HudLCDPrint(11,0, "% 14u", HiScores[0]);

	switch(HighlightHud) {
		case HL_None:
			break;
		case HL_Good:
			blit(HudGfxHiGood, VidBuffer,
				0, 0,
				SCR_WIDTH - HudGfxHiGood->w, SCR_HEIGHT - HudGfxHiGood->h,
				HudGfxHiGood->w, HudGfxHiGood->h);
			break;
		case HL_Neutral:
			blit(HudGfxHi, VidBuffer,
				0, 0,
				SCR_WIDTH - HudGfxHi->w, SCR_HEIGHT - HudGfxHi->h,
				HudGfxHi->w, HudGfxHi->h);
			break;
		case HL_Bad:
			blit(HudGfxHiBad, VidBuffer,
				0, 0,
				SCR_WIDTH - HudGfxHiBad->w, SCR_HEIGHT - HudGfxHiBad->h,
				HudGfxHiBad->w, HudGfxHiBad->h);
			break;
	}
	if(TiltHud)TiltHud--;
	HighlightHud = HL_None;
}

void HudDrawEditor(void)
{
	masked_blit(HudGfx, VidBuffer, 0, 0, 0, 0, HudGfx->w, HudGfx->h);
	masked_blit(HudMapEdit, VidBuffer, 0, 0, SCR_WIDTH - HudMapEdit->w, SCR_HEIGHT - HudMapEdit->h, HudMapEdit->w, HudMapEdit->h);

	ChopString(Map.Name, MapTitle, MapTitlePos, 14);
	MapTitleLen = (int)strlen(Map.Name);
	if(MapTitleLen > 13) {
		MapTitleLen -= 13;
	} else {
		MapTitleLen = 0;
	}

	if(strlen(MapTitle)>0) {
		HudLCDPrint(0, 0, "%s", MapTitle);
	} else {
		HudLCDPrint(0, 0, "Untitled");
	}

	if(strlen(Map.Bg)>0) {
		HudLCDPrint(1, 0,"BG: % 6s", Map.Bg);
	} else {
		HudLCDPrint(1, 0,"BG: None");
	}

	HudLCDPrint(2, 0,"Bricks % 7d", Map.BricksLeft);

	textprintf_ex(VidBuffer, HudFont, HUD_TOKEN_X - 17, HUD_TOKEN_Y, -1, -1, "Q       E");
}

void HudDrawString(int sx, int sy, char *str)
{
	int i;
	char *line;
	char *lp;

	line = GetStaticChar1024();

	i = 0;
	lp = line;
	while(*str) {
		if(!*str || (*str == '\n')) {
			*lp = 0;
			if(*str == '\n') str++;

			textprintf_centre_ex(VidBuffer, HudFontWhite, sx, sy + (i * 10), -1, -1,
				"%s", line);

			lp = line;
			i++;
		} else {
			*lp++ = *str++;
		}
	}
}

void HudLCDPrint(int line, int HOffset, char* fmt, ...)
{
    va_list ap;
	char *buffer;
	buffer = GetStaticChar1024();

	va_start(ap, fmt);
	vsprintf(buffer, fmt, ap);
    va_end(ap);

	line = line % 12;
	switch(line) {
		case 0:
		case 1:
		case 2:
			textprintf_ex(VidBuffer, HudFontDigi, HUD_LCD_X + HOffset, HUD_LCD1_Y + (line * 8), -1, -1, "%s", buffer);
			break;

		default:
			textprintf_ex(VidBuffer, HudFontDigi, HUD_LCD_X + HOffset, HUD_LCD2_Y + (line * 8), -1, -1, "%s", buffer);
			break;
	}
}

void HudDrawWindow(void)
{
	masked_blit(HudMapInfo, VidBuffer, 0, 0,
		(SCR_WIDTH/2) - (HudMapInfo->w/2), (SCR_HEIGHT/2) - (HudMapInfo->h/2),
		HudMapInfo->w, HudMapInfo->h);
}

void HudMapTitle(void)
{
	HudDrawWindow();

	textprintf_centre_ex(VidBuffer, HudFontWhite, SCR_WIDTH/2, (SCR_HEIGHT/2)-50, -1, -1,
		"Level: %d", GameCurrentLevel()+1);

	if(Map.Name[0]) {
		textprintf_centre_ex(VidBuffer, HudFontGreen, SCR_WIDTH/2, (SCR_HEIGHT/2)-40, -1, -1,
			"%s", Map.Name);
	} else {
		textprintf_centre_ex(VidBuffer, HudFontRed, SCR_WIDTH/2, (SCR_HEIGHT/2)-40, -1, -1,
			"Untitled");
	}
	if(strlen(Map.Info)>1) {
		HudDrawString(SCR_WIDTH / 2, (SCR_HEIGHT / 2)-23, Map.Info);
	}

	textprintf_centre_ex(VidBuffer, HudFontWhite, SCR_WIDTH/2, (SCR_HEIGHT/2)+45, -1, -1,
		"Press button to start");
}

static int MarqueeStatus = 0;
static int MarqueePhrase = 0;
//static int HudGFXTick = 1;

void HudUpdate(int score, int pads)
{

	Score = score;
	PadLeft = pads;

	/* rotate title effect */
	IncFrame -= TimeDT;
	if(IncFrame <= 0.0f) {
		IncFrame = 1.0f / HUD_TITLE_FPS;
		if(MapTitleLen) {
			MapTitlePos = (MapTitlePos + 1) % MapTitleLen;
		} else {
			MapTitlePos = 0;
		}
	}

	MarqueeFrame -= TimeDT;
	if(MarqueeFrame <= 0.0f) {
		MarqueeFrame = 1/HUD_MARQUEE_FPS;

		MarqueeStatus++;
		if(MarqueeStatus == 8) {
			MarqueePhrase = (MarqueePhrase+1) % (MAX_CREDITS-3);
			MarqueeStatus = 0;
		}
	}
	GFXEffectTick();

}

void HudTilt(void)
{
	HudHighlight(HL_Bad);
	TiltHud = 30;
}



void HudCredits(void)
{
	static BITMAP  *Marquee  = NULL;
	int x, y;

	if(!Marquee) {
		Marquee = create_bitmap(160, 32);

	}
	clear_to_color(Marquee, makecol(255,0,255));

	x = Marquee->w / 2;


	textprintf_centre_ex(Marquee, HudFontWhite, x, 8 - MarqueeStatus    , -1, -1,  "%s", Credits[MarqueePhrase]);
	textprintf_centre_ex(Marquee, HudFontWhite, x, 16 - MarqueeStatus, -1, -1,  "%s", Credits[MarqueePhrase+1]);
	textprintf_centre_ex(Marquee, HudFontWhite, x, 24 - MarqueeStatus, -1, -1, "%s", Credits[MarqueePhrase+2]);

	rectfill(Marquee, x - 80,  0, x + 80,  7, makecol(255,0,255));
	rectfill(Marquee, x - 80, 24, x + 80, 32, makecol(255,0,255));

	x = SCR_WIDTH / 2;
	y = SCR_HEIGHT - (SCR_HEIGHT / 6);
	masked_blit(Marquee, VidBuffer, 0, 0, x-(Marquee->w)/2,y, Marquee->w, Marquee->h);
}

void HudDrawTitle(void)
{
	masked_blit(HudTitle, VidBuffer, 0, 0, (SCR_WIDTH /2) - (HudTitle->w / 2), 0, HudTitle->w, HudTitle->h);
}
