//
// Game.c: Game logic
//
#include "Common.h"

enum E_GameStates
{
	GS_Start,		// during the map desc. screen
	GS_ChooseDir,	// choosing ball direction
	GS_InGame,		// gameplay
};
unsigned int HiScores[10];
char *HiScoreNames[10];
static int GameMode=GS_Start;
static int PadLeft = 6;
static unsigned int Score = 0;
int GameToken = -1;
static int CurMap = 0;
int GameIsOver = 0;

static void ResetToken(int tok)
{
	if(tok == -1) {
		// reset all
		PadResetLen();
		BallSpeed = 2.0f;			
	} else {
		switch(tok) {
			case TOK_Plus:
			case TOK_Minus:
				PadResetLen();
				break;
			
			case TOK_Invert:
				PadSetColor(0,0,0);
				break;
			
			case TOK_Slow:
				BallSpeed = 2.0f;			
				break;
			
			case TOK_Glue:
				if(GameIsInChooseDir()) {
					GameSetInGame();
				}
	
				// fallthru
			case TOK_Shot:
				Pad.Mode = PM_Normal;
				PadSetColor(0,0,0);
				break;

			case TOK_BoomBall:
			case TOK_Power:
				TheBall.Type = BT_Normal;
				break;
		}
	}
}

void GameJumpMap(int Map, int Rel)
{
	if(Rel) CurMap += Map;
	else CurMap = Map;
	
	if(CurMap < 0) CurMap = 0;
	if(CurMap >= MapListCount) CurMap = MapListCount-1;
	GameChangeMap(MapList[CurMap]);
	GameToken = -1;
	ResetToken(-1);
	PadReset();
	TokenKill(-1);
	KillBall(-1);
	ShotKill(-1);
	ExplosionKill(-1);
	NewBall(MAP_PIXCENTER+3, BALL_INITIAL_Y, BALL_INITIAL_DX, BALL_INITIAL_DY, BT_Normal, 0);
	
}

void GameTest(void) {
	GameToken = -1;
	ResetToken(-1);
	TokenKill(-1);
	KillBall(-1);
	ShotKill(-1);
	ExplosionKill(-1);
	CurMap = 0;
	NewBall(MAP_PIXCENTER+3, BALL_INITIAL_Y, BALL_INITIAL_DX, BALL_INITIAL_DY, BT_Normal, 0);
	GameIsTesting = 1;
	GameHasStarted = 0;
	GameSetChooseDir();
	PadLeft = 0;
	Score = 0;	
}

void GameNew(char *map)
{
	// reset all to default values
	PadLeft = 6;
	Score = 0;
	GameToken = -1;
	ResetToken(-1);
	PadReset();
	TokenKill(-1);
	KillBall(-1);
	ShotKill(-1);
	ExplosionKill(-1);
	CurMap = 0;
	NewBall(MAP_PIXCENTER+3, BALL_INITIAL_Y, BALL_INITIAL_DX, BALL_INITIAL_DY, BT_Normal, 0);
	GameHasStarted = 1;
	GameIsTesting = 0;
	// change map
	GameChangeMap(map);
}

int IsHighScore(void)
{
	int i;
	for(i=0; i<10; i++) {
		if(Score > HiScores[i])return i;
	}
	return -1;
}

void GameAddScore(int add)
{
	if(GameToken == TOK_Double) {	
		add*=2;
	} else if(GameToken == TOK_Half) {	
		add/=2;
	} 

	if(add < 0) {
		if((Score+add)>Score) {
			Score = 0;
		} else {
			Score += add;
		}
	} else {
		if((Score+add)<Score) {
			Score = (unsigned int) -1;
		} else {
			Score += add;
		}
	}
}

void GameEndLevel(void)
{
	// TODO: end of level
	
	if(!GameIsTesting) {
		//alert("Level finished!", NULL, NULL, "OK", NULL, 0, 0);
		show_mouse(NULL);
		SndPlaySound(SND_Jingle1);
			
		GameToken = -1;
		ResetToken(-1);
		PadReset();
		TokenKill(-1);
		KillBall(-1);
		ShotKill(-1);
		ExplosionKill(-1);
		NewBall(MAP_PIXCENTER+3, BALL_INITIAL_Y, BALL_INITIAL_DX, BALL_INITIAL_DY, BT_Normal, 0);
		TheBall.Move = 0;
		GameJumpMap(1, 1);
	} else {
		
	}
}

void GameBallOut(void)
{
	int s = -1;
	PadLeft--;
	SndPlaySound(SND_BallOut);
	
	if(!GameIsTesting)
	if(PadLeft < 0) {		
		SndPlaySound(SND_Jingle2);
		//TODO: Game Over state wait
		alert("Game over", NULL, NULL, "OK", NULL, 0, 0);
		s = IsHighScore();
		if(s!=-1){
			if(!Options[OPT_Cheats]) {
				//go highscore
				MenuSetHiScore(s, Score); 
				SndPlayMusic(SND_HiscoreMusic);				
			}
		}
		show_mouse(NULL);		
		PadLeft = 6;
		Score = 0;
		GameToken = -1;
		ResetToken(-1);
		PadReset();
		TokenKill(-1);
		KillBall(-1);
		ShotKill(-1);
		ExplosionKill(-1);
		NewBall(MAP_PIXCENTER+3, BALL_INITIAL_Y, BALL_INITIAL_DX, BALL_INITIAL_DY, BT_Normal, 0);
		
		// change map
		GameJumpMap(0, 0);
		GameIsOver = 1;
		GameHasStarted = 0;
	}

	ResetToken(-1);
	PadReset();
	TokenKill(-1);
	KillBall(-1);
	ShotKill(-1);
	ExplosionKill(-1);
	NewBall(MAP_PIXCENTER+3, BALL_INITIAL_Y, BALL_INITIAL_DX, BALL_INITIAL_DY, BT_Normal, 0);
	GameSetChooseDir();
	GameToken = -1;
}

void GameChangeMap(char *map)
{
	// TODO: change music
	if(!MapLoad(map)) {
		// TODO: handle MapLoad()==0 correctly
		MapLoadDef();
	}
	GameMode = GS_Start;
}

void GameGetToken(int tok)
{
	if(GameToken != tok) {
		ResetToken(GameToken);
	}

	GameToken = tok;
	switch(tok) {
		case TOK_Plus:
			SndPlaySound(SND_Token_Good);
			PadAddLen(10);
			break;

		case TOK_Minus:
			SndPlaySound(SND_Token_Bad);
			PadAddLen(-3);
			break;

		case TOK_Invert:
			SndPlaySound(SND_Token_Bad);			
			PadSetColor(0,1,1);			
			break;

		case TOK_Slow:
			SndPlaySound(SND_Token_Good);			
			BallSpeed = 1.0f;
			break;
		
		case TOK_Shot:
			SndPlaySound(SND_Token_Good);			
			PadSetColor(1,0,0);
			Pad.Mode = PM_Shot;
			break;

		case TOK_Multi:
			SndPlaySound(SND_Token_Good);			
			NewBall(TheBall.X, TheBall.Y, -TheBall.DX,  TheBall.DY, BT_Hollow, 1);  
			NewBall(TheBall.X, TheBall.Y,  TheBall.DX, -TheBall.DY, BT_Hollow, 1);
			NewBall(TheBall.X, TheBall.Y, -TheBall.DX, -TheBall.DY, BT_Hollow, 1);
			GameToken = -1;
			break;
		
		case TOK_Power:
			SndPlaySound(SND_Token_Good);			
			TheBall.Type = BT_Power;
			break;
		
		case TOK_Glue:
			SndPlaySound(SND_Token_Good);			
			PadSetColor(1,0,1);
			Pad.Mode = PM_Glue;
			break;

		case TOK_Extra:
			SndPlaySound(SND_Token_Extra);
			PadLeft++;
			GameToken = -1;
			break;

		case TOK_Next:
			SndPlaySound(SND_Token_Extra);
			GameJumpMap(1, 1);
			GameToken = -1;
			break;

		case TOK_FarNext:
			SndPlaySound(SND_Token_Extra);
			GameJumpMap(5, 1);
			GameToken = -1;
			break;
		
		case TOK_AutoChase:
			if(Pad.AutoChase == 0) {
				Pad.AutoChase += PAD_AUTOCHASE_TIME;
			}
			else
			{
				Pad.AutoChase--;	
				if(Pad.AutoChase==1) {
					GameToken=-1;
					Pad.AutoChase = 0;
				}
			}
			break;
		
		case TOK_BoomBall:
			TheBall.Type = BT_Mini;
			break;
		
		case TOK_Half:
			SndPlaySound(SND_Token_Bad);
			break;

		default:
			SndPlaySound(SND_Token_Good);			
			return;
	}
}

void GameBreakBrick(int type)
{
	HudHighlight(HL_Good);
	GameAddScore((type%32) * ( ((rand() + 1) % 10)*(CurMap+1)));
}

static void Kamikaze_Tick(void)
{
	int x,y;

	x = TheBall.X / BRICK_WIDTH;
	y = TheBall.Y / BRICK_HEIGHT;
	
	if(Kamikaze == 1) {		
		NewExplosion(x  , y);
		NewExplosion((Pad.X+(PadGetLen()/2))/BRICK_WIDTH  , Pad.Y/BRICK_HEIGHT);
		Pad.Enabled = 0;
		PadSetColor(0, 0, 0);
		KillBall(-1);
		Kamikaze = 5;
	}

	if(Kamikaze == 5) {
		NewExplosion(x  , y-2);
		NewExplosion(x  , y+2);
		NewExplosion(x-2, y);
		NewExplosion(x+2, y);
		Kamikaze=10;
	}

	if(Kamikaze == 15) {
		NewExplosion(x+2, y-2);
		NewExplosion(x-2, y-2);
		NewExplosion(x-2, y+2);
		NewExplosion(x+2, y+2);
		Kamikaze=20;
	}
	
	Kamikaze++;
	
	if(Kamikaze >= 20) {
		if(!GameIsQuiet) {
			GameBallOut();
			Kamikaze = 0;
			if(!Map.BricksLeft){
				GameEndLevel();
			} else { 
				
			}
			
		}
	}	
}

int GameTick(void)
{
	static int bs=0;
	static int k=0;
	BgTick();
	
	if(GameMode == GS_Start) {
		if((MouseB & 1) && !(bs & 1)) {
			bs |= 1;
			GameSetChooseDir();
		}

		if(keypressed())
		{
			k = (readkey()>>8);
			if( k ==  KEY_ENTER) {
				GameSetChooseDir();
			}
			if(k == KEY_ESC) {
				clear_keybuf();
			}
		}
	} else if(GameMode == GS_ChooseDir) {
		TheBall.Move = 0;
		if((MouseB & 1) && !(bs & 1)) {
			bs |= 1;
			GameMode = GS_InGame;	
			TheBall.Move = 1;
		} else if(!(MouseB&1) && (bs & 1)) {
			bs &= ~1;
		}

		if(MouseDX) PadMove(MouseDX);
		TheBall.X = Pad.X+(BALL_WIDTH /2) + (PadGetLen()/2)-(2);

		if((MouseB & 2) && !(bs & 2)) {
			bs |= 2;
			TheBall.DX *= -1;
		} else if(!(MouseB&2) && (bs & 2)) {
			bs &= ~2;
		}

		ShotTick();
		ExplosionTick();
		MoveBalls();
		//PadTick();
		TokenAnim();		
	} else {
		if((MouseB&1)&&!(bs&1)) bs|=1;
		else if(!(MouseB&1)&&(bs&1)) bs&=~1;

		if((MouseB&2)&&!(bs&2)) bs|=2;
		else if(!(MouseB&2)&&(bs&2)) bs&=~2;
		if(MouseDX) {
			if(GameToken == TOK_Invert){
				PadMove(-MouseDX);
			} else {
				PadMove(MouseDX);
			}		
		}
				
		if(key[KEY_K])BallExplode();
			
		if(key[KEY_SPACE])GameTilt(rand()%2);
		
		if(key[KEY_LEFT])GameTilt(0);
		if(key[KEY_RIGHT])GameTilt(1);
		if(key[KEY_UP])GameTilt(2);
		if(key[KEY_DOWN])GameTilt(3);
		
		if(MouseB & 1)PadFire();

		if(key[KEY_R]) {
			if(Options[OPT_Cheats]){		
				TheBall.X = MAP_PIXCENTER+3;
				TheBall.Y = BALL_INITIAL_Y;
				TheBall.DX = -BallSpeed;
				TheBall.DY = -BallSpeed;
			}
		}
		
		if(Map.BricksLeft == 0) {
			if(!GameIsTesting) {
				if(GameIsQuiet == 0)GameEndLevel();
			} else {
				return 0;
			}
		}
		
		if(Kamikaze) {
			Kamikaze_Tick();
		}		
		ShotTick();
		ExplosionTick();
		MoveBalls();
		PadTick();
		TokenAnim();


	}

	
	HudUpdate(Score, PadLeft);
	return 1;
}

void GameDraw(void)
{
	BgDraw();
	MapDraw(3, 3);

	if(GameMode == GS_Start) {
		HudDraw();
		if(Options[OPT_Cheats]){
			if(Pad.AutoChase) {
				GameMode = GS_InGame;
				TheBall.Move=1;
			}
		}
		HudMapTitle();		
	} else {
		if(GameMode == GS_ChooseDir) {
#ifdef BALL_SYNC
			DrawBallVel(&TheBall, 0.05f);
#else
			DrawBallVel(&TheBall, 5);
#endif
		}
		
		DrawBalls();
		if(Pad.Enabled)PadDraw();
		TokenRender();
		ShotDraw();
		ExplosionDraw();
		HudDraw();
	}
}

void GameTilt(int axis)
{
	if(Score>=100){		
		TiltBalls(axis);		
	}
}

int GameIsInChooseDir(void)
{
	return GameMode == GS_ChooseDir;
}

void GameSetInGame(void)
{
	TheBall.Move = 1;
	GameMode = GS_InGame;
}

void GameSetChooseDir(void)
{
	GameMode = GS_ChooseDir;
	Pad.Enabled = 1;
}

int GameCurrentLevel(void)
{
	return CurMap;
}

int GameIsStarting(void)
{
	return GameMode == GS_Start;
}