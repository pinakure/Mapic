//
//	Main.c: Initialization, main loop and timing code
//
#include "Common.h"

#ifndef LINUX
#include <winalleg.h>
#include <windows.h>
#include <direct.h>
#endif

#define GetCurrentDir _getcwd
/*#ifdef DEBUG
char GameDir[256] = "C:\\Ark\\";
#else
char GameDir[256];
#endif*/
#ifdef LINUX
char GameDir[256] = "/home/offsetzero/m4g/Ark/";
#else
char GameDir[256] = ".\\";
#endif
//char GameDir[256] = "C:\\Ark\\";

int MouseX=SCR_WIDTH/2, MouseY=SCR_HEIGHT/2;
int MouseDX, MouseDY;
int MouseB, MouseDZ;
int bQuit = 0;
int VidScale, VidDepth, VidTripleBuffer;

BITMAP *VidPages[3];
PALETTE Palette;
int VidCurPage = 0;

enum E_GameModes {
	GM_Game,
	GM_Editor,
	GM_Menu,
	GM_Pretitle
};

void BlitSprite(BITMAP *bmp, int SX, int SY)
{
	static int pink=0;
	int x, y, c;

	if(!pink) {
		pink = makecol(255, 0, 255);
	}

	for(y=0; y<bmp->h; y++) {
		for(x=0; x<bmp->w; x++) {
			c = getpixel(bmp, x, y);
			if(c != pink) {
				putpixel(VidBuffer, SX+x, SY+y, c);
			}
		}
	}
}

int ValidFilename(char *fname, char *ext)
{
	int i, dc;

	// len < 64?
	if(strlen(fname) >= 64) {
		return 0;
	}

	// >=1 char before period?
	if(!isalnum(fname[0])) {
		return 0;
	}

	// only alnum, spaces and period
	for(dc=0, i=0; fname[i]; i++) {
		if(isalnum(fname[i])) {
			continue;
		} else if(fname[i] == ' ') {
			continue;
		} else if(fname[i] == '.') {
			dc++;
			continue;
		} else {
			return 0;
		}
	}
	if(dc != 1) {
		return 0;
	}

	// extension = .map?
	for(i=0; fname[i]; i++) {
		if(fname[i] == '.') {
			i++;

			while(*ext) {
				if(toupper(fname[i]) != *ext) return 0;
				i++;
				ext++;
			}

			if(fname[i] != 0) return 0;
			return 1;
		}
	}

	return 0;
}

char *GetStaticChar1024(void)
{
	static char str[1024];

	str[0] = 0;
	return str;
}

void ChopString(char *source, char *dest, int start, unsigned int len)
{
#ifndef ASM
	source += start;
	while(*source && len) {
		*dest++ = *source++;
		len--;
	}
	*dest = 0;
#else
	__asm {
		mov esi, source
		add esi, start

		mov edi, dest
		mov ecx, len

		cld
lp:
		lodsb
		or al, al
		jz lp2
		stosb
		loop lp

		xor eax, eax
lp2:
		stosb
	}
#endif
}

static volatile unsigned int frames=0, fps=0, peak_fps=0, drive=0, ticks=0;
static void ticker(void)
{
	ticks++;

	if((ticks%FPS)==0) {
		fps = frames;
		frames = 0;
		if(fps > peak_fps) peak_fps = fps;
	}
}
END_OF_STATIC_FUNCTION(ticker);

static void UpdateMouse(void)
{
	static int ox=-1, oz;

	if(ox == -1) {
#ifdef SCR_SCALE
		position_mouse((SCR_SCALE*SCR_WIDTH)/2, (SCR_SCALE*SCR_HEIGHT)/2);
#else
		position_mouse(SCR_WIDTH/2, SCR_HEIGHT/2);
#endif
		ox = 0;
	} else {
		MouseB = mouse_b;
		MouseDZ = mouse_z - oz;
		oz = mouse_z;

#ifndef SCR_SCALE
		MouseDX = mouse_x - (SCR_WIDTH/2);
		MouseDY = mouse_y - (SCR_HEIGHT/2);
#else
		MouseDX = mouse_x - ((SCR_SCALE*SCR_WIDTH)/2);
		MouseDY = mouse_y - ((SCR_SCALE*SCR_HEIGHT)/2);
		MouseDX /= SCR_SCALE;
		MouseDY /= SCR_SCALE;
#endif

		MouseX += MouseDX;
		MouseY += MouseDY;

		if(MouseX < 0) MouseX = 0;
		if(MouseY < 0) MouseY = 0;
		if(MouseX >= SCR_WIDTH) MouseX = SCR_WIDTH-1;
		if(MouseY >= SCR_HEIGHT) MouseY = SCR_HEIGHT-1;

#ifdef SCR_SCALE
		position_mouse((SCR_SCALE*SCR_WIDTH)/2, (SCR_SCALE*SCR_HEIGHT)/2);
#else
		position_mouse(SCR_WIDTH/2, SCR_HEIGHT/2);
#endif
	}
}

static int HandleKeys(int k)
{
	if(Options[OPT_Cheats])
	{
		switch(k) {
			case KEY_TAB:
				clear_keybuf();
				if(GameIsTesting) {
					return GM_Editor;
				}
				break;

			case KEY_2:
				TokenNew(Pad.X+(PadGetLen()/2)-(TOKEN_WIDTH/2), Pad.Y-16, TOK_Next);
				break;

			case KEY_3:
				TokenNew(Pad.X+(PadGetLen()/2)-(TOKEN_WIDTH/2), Pad.Y-16, TOK_Extra);
				break;

			case KEY_4:
				TokenNew(Pad.X+(PadGetLen()/2)-(TOKEN_WIDTH/2), Pad.Y-16, TOK_Plus);
				break;

			case KEY_5:
				TokenNew(Pad.X+(PadGetLen()/2)-(TOKEN_WIDTH/2), Pad.Y-16, TOK_Power);
				break;

			case KEY_6:
				TokenNew(Pad.X+(PadGetLen()/2)-(TOKEN_WIDTH/2), Pad.Y-16, TOK_Glue);
				break;

			case KEY_7:
				TokenNew(Pad.X+(PadGetLen()/2)-(TOKEN_WIDTH/2), Pad.Y-16, TOK_Shot);
				break;

			case KEY_8:
				TokenNew(Pad.X+(PadGetLen()/2)-(TOKEN_WIDTH/2), Pad.Y-16, TOK_Multi);
				break;
		}
	}
	switch(k){
		case KEY_ESC:
			SndPlayMusic(SND_TitleMusic);
			clear_keybuf();
			return GM_Menu;
	}
	return GM_Game;
}

static int SaveOptions(void)
{
	FILE *fp;
	char *str;
	int i, len;

	str = GetStaticChar1024();
	strcpy(str, GameDir);
	strcat(str, "config.dat");

	fp = fopen(str, "wb");
	if(!fp) {
		alert("Can't open", str, NULL, "OK", NULL, 0, 0);
		return 0;
	}

	fwrite(Options, sizeof(int), OPT_MAX, fp);

#ifdef JUNK_CFG
	len = (rand() % 0xCAFE) + 0x1000;
	for(i=0; i<len; i++) {
		fputc(rand() % 0xFF, fp);
	}
#endif

	fclose(fp);
	return 1;
}

static void NewOptions(void)
{
	Options[OPT_Music]		= 1;
	Options[OPT_Cheats]		= 0;
	Options[OPT_ColorDepth]	= 8;
	Options[OPT_Fullscreen] = 0;
	Options[OPT_Physics]    = 0;
	Options[OPT_Powerups]   = 1;
	Options[OPT_Sound]	    = 1;
	Options[OPT_TripleBuffer]=0;
	Options[OPT_VideoScale] = 1;
	Options[OPT_VSync]		= 0;
}

static int LoadOptions(void)
{
	FILE *fp;
	char *str;

	str = GetStaticChar1024();
	strcpy(str, GameDir);
	strcat(str, "config.dat");

	fp = fopen(str, "rb");
	if(!fp) {
		NewOptions();
		return 1;
	}

	fread(Options, sizeof(int), OPT_MAX, fp);

	fclose(fp);
	return 1;
}

static int InitVideo(void)
{
	BITMAP *ColorMap;
	char *str;
	int v=0;

	set_color_depth(Options[OPT_ColorDepth]);

	if(Options[OPT_TripleBuffer]) {
		if(Options[OPT_VideoScale] != 1) {
			Options[OPT_VideoScale] = 1;
		}

		if(!Options[OPT_Fullscreen]) {
			Options[OPT_Fullscreen] = 1;
		}

		if(set_gfx_mode(GFX_AUTODETECT_FULLSCREEN, SCR_WIDTH, SCR_HEIGHT, 0, 0)) {
			v = 1;
			set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
			allegro_message("Can't setup video mode");
			return 0;
		}

		if(gfx_capabilities & GFX_CAN_TRIPLE_BUFFER) {
			VidPages[0] = create_video_bitmap(SCR_WIDTH, SCR_HEIGHT);
			VidPages[1] = create_video_bitmap(SCR_WIDTH, SCR_HEIGHT);
			VidPages[2] = create_video_bitmap(SCR_WIDTH, SCR_HEIGHT);
			if(!VidPages[0] || !VidPages[1] || !VidPages[2]) {
				set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
				allegro_message("Can't setup video mode");
				return 0;
			}

			VidCurPage = 0;
			goto next;
		} else {
			Options[OPT_TripleBuffer] = 0;
		}
	}

	if(!v) {
		if(Options[OPT_Fullscreen]) {
			if(set_gfx_mode(GFX_AUTODETECT_FULLSCREEN, SCR_WIDTH*Options[OPT_VideoScale], SCR_HEIGHT*Options[OPT_VideoScale], 0, 0)) {
				allegro_message("Can't setup video mode");
				return 0;
			}
		} else {
			if(set_gfx_mode(GFX_AUTODETECT_WINDOWED, SCR_WIDTH*Options[OPT_VideoScale], SCR_HEIGHT*Options[OPT_VideoScale], 0, 0)) {
				allegro_message("Can't setup video mode");
				return 0;
			}
		}
	}

	VidCurPage = 0;
	VidPages[0] = create_bitmap(SCR_WIDTH, SCR_HEIGHT);
	if(!VidPages[0]) {
		set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
		allegro_message("Can't setup video mode");
		return 0;
	}

next:;
	VidScale = Options[OPT_VideoScale];
	VidDepth = Options[OPT_ColorDepth];
	VidTripleBuffer = Options[OPT_TripleBuffer];

	/*load palette*/
	str = GetStaticChar1024();

	strcpy(str, GameDir);
	strcat(str, "gfx/colormap.bmp");
	ColorMap = load_bitmap(str, Palette);
	if(!ColorMap) {
		alert("Can't load", str, NULL, "OK", NULL, 0, 0);
		return 0;
	}
	destroy_bitmap(ColorMap);
	set_palette(Palette);

	return 1;
}

static void DeinitVideo(void)
{
	destroy_bitmap(VidPages[0]);

	if(VidTripleBuffer) {
		destroy_bitmap(VidPages[1]);
		destroy_bitmap(VidPages[2]);
	}

	set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
}

int main(void)
{
	int i, ot;
	int mode=GM_Menu;

	srand(time(NULL));
/*
#ifndef DEBUG
	GetCurrentDir(GameDir,256);
	strcat(GameDir, "\\");
#endif*/
	LoadOptions();
	allegro_init();
	install_keyboard();
	install_timer();
	install_mouse();

	if(!InitVideo()) {
		return -1;
	}

	if(Options[OPT_ColorDepth] == 8) {
		gui_fg_color = 1;
		gui_bg_color = 233;
	}

	if(!SndInit()) {
		return -1;
	}

	if(!MapLoadList()) {
		SndDeinit();
		return -1;
	}

	if(!InitExplosions()) {
		SndDeinit();
		MapFreeList();
		return -1;
	}

	if(!HudInit()) {
		SndDeinit();
		MapFreeList();
		DeinitExplosions();
		return -1;
	}

	if(!InitBalls()) {
		SndDeinit();
		MapFreeList();
		DeinitExplosions();
		HudDeinit();
		return -1;
	}

	if(!TokenInit()) {
		SndDeinit();
		MapFreeList();
		DeinitExplosions();
		HudDeinit();
		DeinitBalls();
		return -1;
	}

	if(!PadInit()) {
		SndDeinit();
		MapFreeList();
		DeinitExplosions();
		HudDeinit();
		TokenDeinit();
		DeinitBalls();
		return -1;
	}

	if(!InitBricks()) {
		SndDeinit();
		MapFreeList();
		DeinitExplosions();
		HudDeinit();
		TokenDeinit();
		PadDeinit();
		DeinitBalls();
		return -1;
	}

	if(!HiScoreInit()) {
		SndDeinit();
		MapFreeList();
		DeinitExplosions();
		HudDeinit();
		TokenDeinit();
		PadDeinit();
		DeinitBricks();
		DeinitBalls();
		return -1;
	}
	SndPlayMusic(SND_TitleMusic);
	EditInit();
	//GameNew(MapList[0]);




	install_int_ex(ticker, BPS_TO_TIMER(FPS));

	UpdateMouse();
	ot = ticks;
	bQuit = 0;
	while(!bQuit) {
		show_mouse(NULL);
		if(VidTripleBuffer) {
			if(!poll_scroll()) {
				request_video_bitmap(VidBuffer);
				VidCurPage = (VidCurPage+1) % 3;
			}
		}

		if(ot != ticks) {
			// logic here
			ot = ticks;

			UpdateMouse();

			if(mode == GM_Game) {
				// game mode
				if(keypressed()) {
					if(!GameIsStarting()) {
						mode = HandleKeys(readkey() >> 8);
					}
				}

				if( !GameIsTesting) {
					if(GameIsOver) {
						mode = GM_Menu;
						HiScoreSetting = 0;
						GameIsOver = 0;
					}
				}

				i = GameTick();
				if(!i) {
					mode = GM_Editor;
				}
			} else if(mode == GM_Editor) {
				i = EditTick();
				HudUpdate(0, 6);
				if(!i) {
					mode = GM_Game;
				} else if(i == -1) {
					mode = GM_Menu;
					continue;
				}
			} else if(mode == GM_Menu) {
				i = MenuTick();
				if(!i) {
					clear_keybuf();
					mode = GM_Game;
				} else if(i == -1) {
					clear_keybuf();
					mode = GM_Editor;
				}
			} else if(mode == GM_Pretitle) {
				i = HudPreTitleTick();
				if(!i) {
					mode = GM_Menu;
				}
			}
		}

		// draw here
		if(VidDepth == 8) {
			clear_to_color(VidBuffer, 1);
		} else {
			clear_bitmap(VidBuffer);
		}

		if(mode == GM_Game) {
			GameDraw();
		} else if(mode == GM_Editor) {
			EditDraw();
			DrawBall(MouseX, MouseY, BT_Debug);
		} else if(mode == GM_Menu) {
			MenuDraw();
		} else if(mode == GM_Pretitle) {
			HudPreTitleDraw();
		}

		if(key[KEY_F])
		{
			textprintf_right_ex(VidBuffer, font, SCR_WIDTH, 0, makecol(255, 255, 255), -1, "FPS: %d", fps);
			textprintf_right_ex(VidBuffer, font, SCR_WIDTH, 8, makecol(255, 255, 255), -1, "Peak FPS: %d", peak_fps);
			textprintf_right_ex(VidBuffer, font, SCR_WIDTH, 16, makecol(255, 255, 255), -1, "Ticks: %u", ticks);
			textprintf_right_ex(VidBuffer, font, SCR_WIDTH, 24, makecol(255, 255, 255), -1, "CPage: %u", VidCurPage);
			textprintf_right_ex(VidBuffer, font, SCR_WIDTH, 32, makecol(255, 255, 255), -1, "VSync: %u", Options[OPT_VSync]);
		}

		if(!VidTripleBuffer)
		{

			stretch_blit(VidBuffer, screen,
				0, 0, SCR_WIDTH, SCR_HEIGHT,
				0, 0, VidScale * SCR_WIDTH, VidScale * SCR_HEIGHT);

		}

		SndUpdate();
		frames++;

		if(Options[OPT_VSync]) {
			vsync();
		}
	}

	SndDeinit();
	DeinitExplosions();
	MapFreeList();
	HudDeinit();
	TokenDeinit();
	DeinitBricks();
	PadDeinit();
	DeinitBalls();
	HiScoreDeinit();
	DeinitVideo();
	SaveOptions();
	return 0;
}
END_OF_MAIN()
