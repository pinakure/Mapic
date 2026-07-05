//
// Effect.c: GFX Routines.
//
#include "Common.h"


static int GFXTick = 0;
static float sin_t1[SCR_WIDTH * SCR_HEIGHT];
static float sin_t2[SCR_WIDTH * SCR_HEIGHT];
static float sin_t3[SCR_WIDTH * SCR_HEIGHT];
//static PALLETE pal;

float FadeIn	= 0.0f;
float FadeOut	= 0.0f;
int FadeStatus = FV_Quiet;


void GFXEffectTick(void) {
	GFXTick = 1;
}

/*
Plasma code
*/

static void InitSin(void)
{
    int i;

    for(i=0;i<(SCR_WIDTH * SCR_HEIGHT);i++) {
        sin_t1[i] = sin((i * PI) / 270) * 256;
        sin_t2[i] = sin((i * PI) / 45) * 32;
        sin_t3[i] = sin((i * PI) / 90) * 64;
    }
}
/*
END: Plasma code.
*/

void GFXEffect(enum E_EffectTypes type)
{
	static BITMAP *Grid = NULL;
	static BITMAP *GFX  = NULL;
	static BITMAP *VLine = NULL;
	static BITMAP *HLine = NULL;

	//static int PlasmaVariation = 0;

	int		x = 0,
			y = 0;
	static int  c = 0,
				i = 0;

	if(!VLine) {
		VLine = create_bitmap(1, SCR_HEIGHT);
	}
	if(!HLine) {
		HLine = create_bitmap(SCR_WIDTH, 1);
	}

	if(!GFX) {
		GFX = create_bitmap(SCR_WIDTH, SCR_HEIGHT);
		clear_to_color(GFX, makecol(255,0,255));
		InitSin();
	}

	if(!Grid) {
		Grid = create_bitmap(SCR_WIDTH, SCR_HEIGHT);
		clear_to_color(Grid, makecol(255,0,255));
		for(x=0;x<SCR_WIDTH;x++) {
			for(y=0;y<SCR_HEIGHT;y++) {
				if(x%2) {
					if( y%2)putpixel(Grid, x, y, makecol(0,0,0));
				}
				else
				{
					if(!(y%2))putpixel(Grid, x, y, makecol(0,0,0));
				}
			}
		}
	}


	switch(type)
	{
		case FX_Grid:
			masked_blit(Grid, VidBuffer, 0, 0, 0 ,0, SCR_WIDTH, SCR_HEIGHT);
			break;
		case FX_GFX:
			if(GFXTick)
			{
				if(i==(SCR_WIDTH * SCR_HEIGHT)) i=0;

				for(y = 0; y < SCR_HEIGHT; y+=2) {
					for(x = 0; x < SCR_WIDTH; x+=3) {
						c = sin_t1[x] + sin_t2[y] + sin_t3[x + y + i];

						putpixel(GFX, x, y, (c&31)+176);

					}
				}
				i+=SPEED;

				blit(GFX,  VLine, 0, 0, 0, 0, 1, SCR_HEIGHT);
				blit(GFX,   GFX, 1, 0, 0, 0, SCR_WIDTH-1, SCR_HEIGHT);
				blit(VLine,  GFX, 0, 0, SCR_WIDTH-1, 0, SCR_WIDTH-1, SCR_HEIGHT);

				blit(GFX,  HLine, 0, 0, 0, 0, SCR_WIDTH, 1);
				 blit(GFX,   GFX, 0, 1, 0, 0, SCR_WIDTH, SCR_HEIGHT-1);
				blit(HLine,  GFX, 0, 0, 0, SCR_HEIGHT-1, SCR_WIDTH, SCR_HEIGHT-1);

				GFXTick = 0;
			}
			masked_blit(GFX, VidBuffer, 0, 0, 0 ,0, SCR_WIDTH, SCR_HEIGHT);
			masked_blit(Grid, VidBuffer, 0, 0, 0 ,0, SCR_WIDTH, SCR_HEIGHT);

			break;
        default:
            break;
	}
}
