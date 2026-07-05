//
// Menu.c: Menu system
//
#include "Common.h"

enum E_MenuOptions {
#define _(en, str) MO_##en,
#include "menuopt.inc"
#undef _
	MAX_OPTIONS
};

enum E_SubMenus {
	MENU_Main,
	MENU_Options,
	MENU_HiScore
};

static char *Strings[] = {
#define _(en, str) str,
#include "menuopt.inc"
#undef _
};

int Options[OPT_MAX];

static int Option = 0;
static int SubMenu = MENU_Main;


static void MenuPrint(int y, char *str, FONT *fnt)
{
	textprintf_centre_ex(VidBuffer, fnt, SCR_WIDTH / 2, (SCR_HEIGHT / 2)+y, -1, -1, "%s", str);
}

static void SubMenuDraw(void)
{
	FONT *Highlight[16];
	int i;

	switch(SubMenu) {
		case MENU_Main:
			for(i=0; i<OPT_MAX; i++) {
				Highlight[i] = HudFontWhite;
			}
			Highlight[Option] = HudFontGreen;
			if(Option==MO_Exit)Highlight[Option] = HudFontRed;

			if(GameHasStarted)MenuPrint(-55, "Paused",	HudFontRed);
			MenuPrint(-45+(GameHasStarted*10), "Start Game",	Highlight[0]);
			MenuPrint(-25+(GameHasStarted*10), "Hall Of Fame",	Highlight[1]);
			MenuPrint( -5+(GameHasStarted*10), "Editor",		Highlight[2]);
			MenuPrint(+15+(GameHasStarted*10), "Options",		Highlight[3]);
			MenuPrint(+35+(GameHasStarted*10), "Quit Game",		Highlight[4]);

			break;
		case MENU_Options:

			for(i=0; i<OPT_MAX; i++) {
				Highlight[i] = HudFontWhite;
			}
			Highlight[Option] = HudFontGreen;



			MenuPrint(-55, "Options", HudFontRed);
			MenuPrint(-45, "Sound Effects:             ", HudFontWhite);
			MenuPrint(-35, "     Music:           ", HudFontWhite);
			MenuPrint(-25, "   Physics:           ", HudFontWhite);
			MenuPrint(-15, "  Powerups:           ", HudFontWhite);
			MenuPrint( -5, "    Cheats:           ", HudFontWhite);
			MenuPrint(  5, "Fullscreen:           ", HudFontWhite);
			MenuPrint( 15, "     VSync:           ", HudFontWhite);
			MenuPrint( 25, "   Scaling:           ", HudFontWhite);
			MenuPrint( 35, "Triple Buffer:              ", HudFontWhite);
			MenuPrint( 45, "Color depth:            ", HudFontWhite);

			if(Options[OPT_Sound]) {
				MenuPrint(-45, "               Enabled", Highlight[OPT_Sound]);
			} else {
				MenuPrint(-45, "              Disabled", Highlight[OPT_Sound]);
			}
			if(Options[OPT_Music]) {
				MenuPrint(-35, "               Enabled", Highlight[OPT_Music]);
			} else {
				MenuPrint(-35, "              Disabled", Highlight[OPT_Music]);
			}
			if(Options[OPT_Physics]) {
				MenuPrint(-25, "                Simple", Highlight[OPT_Physics]);
			} else {
				MenuPrint(-25, "               Complex", Highlight[OPT_Physics]);
			}
			if(Options[OPT_Powerups]) {
				MenuPrint(-15, "               Enabled", Highlight[OPT_Powerups]);
			} else {
				MenuPrint(-15, "              Disabled", Highlight[OPT_Powerups]);
			}
			if(Options[OPT_Cheats]) {
				MenuPrint( -5, "               Enabled", HudFontRed);
			} else {
				MenuPrint( -5, "              Disabled", Highlight[OPT_Cheats]);
			}
			if(Options[OPT_Fullscreen]) {
				MenuPrint(  5, "               Enabled", Highlight[OPT_Fullscreen]);
			} else {
				MenuPrint(  5, "              Disabled", Highlight[OPT_Fullscreen]);
			}
			if(Options[OPT_VSync]) {
				MenuPrint(15, "               Enabled", Highlight[OPT_VSync]);
			} else {
				MenuPrint(15, "              Disabled", Highlight[OPT_VSync]);
			}
			if(Options[OPT_TripleBuffer]) {
				MenuPrint(35, "               Enabled", Highlight[OPT_TripleBuffer]);
			} else {
				MenuPrint(35, "              Disabled", Highlight[OPT_TripleBuffer]);
			}
			switch(Options[OPT_VideoScale]) {
						case 1: MenuPrint(25, "                  None", Highlight[OPT_VideoScale]); break;
						case 2: MenuPrint(25, "                    2x", Highlight[OPT_VideoScale]); break;
						case 3: MenuPrint(25, "                    3x", Highlight[OPT_VideoScale]); break;
						case 4: MenuPrint(25, "                    4x", Highlight[OPT_VideoScale]); break;
					}
			if(Options[OPT_VideoScale]) {

			} else {
				MenuPrint(25, "                    2x", Highlight[OPT_VideoScale]);
			}
			switch(Options[OPT_ColorDepth]) {
				case  8: MenuPrint(45, "                 8 Bit", Highlight[OPT_ColorDepth]); break;
				case 15: MenuPrint(45, "                15 Bit", Highlight[OPT_ColorDepth]); break;
				case 16: MenuPrint(45, "                16 Bit", Highlight[OPT_ColorDepth]); break;
				case 24: MenuPrint(45, "                24 Bit", Highlight[OPT_ColorDepth]); break;
				case 32: MenuPrint(45, "                32 Bit", Highlight[OPT_ColorDepth]); break;
			}
			break;
		case MENU_HiScore:
			if(HiScoreSetting) {
				MenuPrint(-55, "You got a new Hi-Score!", HudFontRed);
			} else {
				MenuPrint(-55, "Hall Of Fame", HudFontRed);
			}

			for(i=0;i<10;i++) {
				if(Option == i) {
					textprintf_centre_ex(VidBuffer, HudFontRed,
										 (SCR_WIDTH / 2), ((SCR_HEIGHT / 2)-43) + (i*10),
										 -1, -1,
										 "% 8s% 19u", HiScoreNames[i], HiScores[i]);
				} else {
					textprintf_centre_ex(VidBuffer, HudFontWhite,
										 (SCR_WIDTH / 2), ((SCR_HEIGHT / 2)-43) + (i*10),
										 -1, -1,
										 "% 8s% 19u", HiScoreNames[i], HiScores[i]);
				}
			}
			break;
		/*case MENU_LoadGame:
		case MENU_SaveGame:*/
	}
}

void MenuDraw(void)
{
	if(GameHasStarted) {
		BgDraw();
		MapDraw(3, 3);
		HudDraw();
		GFXEffect(FX_Grid);
	} else {
		/*draw dynamic stuff*/
		GFXEffect(FX_GFX);
	}
	if(SubMenu == MENU_Main)HudCredits();
	HudDrawTitle();
	HudDrawWindow();
	SubMenuDraw();
}



static int HandleKeyMain(int k)
{
	switch(k)
	{
		case KEY_UP:
			if(Option == 0) Option = MAX_OPTIONS - 1;
			else Option--;
			SndPlaySound(SND_Menu);
			break;
		case KEY_DOWN:
			if(Option == (MAX_OPTIONS-1)) Option = 0;
			else Option++;
			SndPlaySound(SND_Menu);
			break;
		case KEY_ESC:
			if(GameHasStarted){
				SndPlaySound(SND_Back);
				clear_keybuf();
				SndStop(SND_TitleMusic);
				return 0;
			}
			break;
		case KEY_ENTER:
			SndPlaySound(SND_Confirm);
			switch(Option) {
				case MO_NewGame:
					// TODO: New Game menu (select mode {puzzle | time attack | classic | new } )
					GameNew(MapList[0]);
					SndStop(SND_TitleMusic);
					//SndStop(SND_TitleMusic);
					clear_keybuf();
					return 0;
				/*case MO_SaveGame:
				case MO_LoadGame:*/
				case MO_HiScore:
					Option=-1;
					SubMenu = MENU_HiScore;
					break;
				case MO_Editor:
					// FIX: Anti exploits ;)
					SndStop(SND_TitleMusic);
					GameHasStarted = 0;
					return -1;
				case MO_Options:
					Option=0;
					SubMenu = MENU_Options;
					break;
				case MO_Exit:
					bQuit = 1;
					return 0;
			}
	}
	clear_keybuf();
	return 1;
}

static int HandleKeyOptions(int k)
{
	switch(k)
	{
		case KEY_UP:
			if(Option == 0) Option = OPT_MAX-1;
			else Option--;
			SndPlaySound(SND_Menu);
			break;
		case KEY_DOWN:
			if(Option == OPT_MAX-1) Option = 0;
			else Option++;
			SndPlaySound(SND_Menu);
			break;
		case KEY_ESC:
			Option	= MO_Options;
			SubMenu = MENU_Main;
			SndPlaySound(SND_Back);
			break;
		case KEY_ENTER:
			switch(Option) {
				case OPT_Music:
					if(Options[Option]==1) {
						SndStop(SND_TitleMusic);
					} else {
						SndPlayMusic(SND_TitleMusic);
					}
					Options[Option]^=1;
					break;

				case OPT_Sound:
					Options[Option]^=1;
					break;

				case OPT_Cheats:
					GameAddScore(0x80000000);
					Pad.AutoChase = 0;

				case OPT_Physics:
				case OPT_Powerups:
				case OPT_TripleBuffer:
				case OPT_VSync:
				case OPT_Fullscreen:
					Options[Option] ^= 1;
					break;

				case OPT_VideoScale:
					switch(Options[OPT_VideoScale]) {
						case 1: Options[OPT_VideoScale] = 2; break;
						case 2: Options[OPT_VideoScale] = 3; break;
						case 3: Options[OPT_VideoScale] = 4; break;
						case 4: Options[OPT_VideoScale] = 1; break;
					}
					break;

				case OPT_ColorDepth:
					switch(Options[OPT_ColorDepth]) {
						case  8: Options[OPT_ColorDepth] = 15; break;
						case 15: Options[OPT_ColorDepth] = 16; break;
						case 16: Options[OPT_ColorDepth] = 24; break;
						case 24: Options[OPT_ColorDepth] = 32; break;
						case 32: Options[OPT_ColorDepth] =  8; break;
					}
					break;

				default:
					break;

			}
			SndPlaySound(SND_Confirm);
			break;
	}
	clear_keybuf();
	return 1;
}

static int HandleKeyHiScore(int ky)
{
	int i;
	int k = ky >> 8;
	int ascii = ky & 0xFF;


	if(Option != -1) {
		i = (int)strlen(HiScoreNames[Option]);
		if(i<8) {
			if( (k >= KEY_A) && (k <= KEY_Z) ) {
				HiScoreNames[Option][i] = ascii;
				HiScoreNames[Option][i+1] = 0;
			} else if( (ascii >= '0') && (ascii <= '9') ) {
				HiScoreNames[Option][i] = ascii;
				HiScoreNames[Option][i+1] = 0;
			}

		}
	}
	switch(k)
	{
		case KEY_BACKSPACE:
			if(HiScoreSetting) {
				if(i>0){
					i--;
					HiScoreNames[Option][i] = 0;
				}
			}
			break;
		case KEY_ESC:
			if(!HiScoreSetting) {
				SndPlaySound(SND_Back);
				SubMenu = MENU_Main;
				Option = MO_HiScore;
				SndStop(SND_HiscoreMusic);
				SndPlayMusic(SND_TitleMusic);
			}
			break;
		case KEY_ENTER:
			if(!HiScoreSetting) {
				SndPlaySound(SND_Confirm);
				SubMenu = MENU_Main;
				Option = MO_HiScore;
			} else {
				if(!strlen(HiScoreNames[Option])) {
					strcpy(HiScoreNames[Option], "noname");
				}
				SaveHiScore();
				SndPlaySound(SND_Confirm);
				SndStop(SND_HiscoreMusic);
				SndPlayMusic(SND_TitleMusic);
				SubMenu = MENU_Main;
				Option = MO_HiScore;
				HiScoreSetting = 0;
				GameHasStarted = 0;
			}
			break;
	}
	clear_keybuf();
	return 1;
}

void MenuSetHiScore(int idx, int Score)
{
	Option				= idx;
	SubMenu				= MENU_HiScore;
	HiScoreAddEntry(idx);
	HiScores[idx]		= Score;
	*HiScoreNames[idx]	= 0;
	HiScoreSetting		= 1;
}

int MenuTick(void)
{
	int key;
	while(keypressed()) {


		switch(SubMenu) {
			case MENU_Main:
				key = HandleKeyMain(readkey() >> 8);
				if(!key) {
					return 0;
				} else if(key == -1) {
					return -1;
				}
				break;

			case MENU_Options:
				if(!HandleKeyOptions(readkey() >> 8)) {
					return 0;
				}
				break;

			case MENU_HiScore:
				if(!HandleKeyHiScore(readkey())) {
					return 0;
				}
				break;
		}
	}
	HudUpdate(0, 0);
	return 1;
}
