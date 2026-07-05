//
// Effect.h: GFX Routines.
//
#ifndef EFFECT_H
#define EFFECT_H

#define PI 3.141592
#define LINEAR(a, b) ( (b * SCR_X) + a )
#define SPEED 2


enum E_FadeValues {
	FV_Quiet = 0,
	FV_StartFadeIn,
	FV_FadeIn,
	FV_EndFadeIn,
	FV_StartFadeOut,
	FV_FadeOut,
	FV_EndFadeOut
};

enum E_EffectTypes {
	FX_GFX=0,
	FX_Grid=1,
	FX_MAX
};

extern float FadeIn, FadeOut;
extern int FadeStatus;

/*
==============
GFXEffectDraw
==============
	Copy desired GFX Canvas to main video buffer.
*/
void GFXEffect(enum E_EffectTypes type);

/*
==============
GFXEffectTick
==============
	Enables frame advance on GFX Canvas.
*/
void GFXEffectTick(void);
	
#endif