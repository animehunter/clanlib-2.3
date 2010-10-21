
#include "precomp.h"
#include "postprocess_effect.h"
#include "postprocess_scene.h"

void PostProcessEffectDarken::render(CL_GraphicContext &gc, CL_Texture &texture, CL_Rect geometry)
{
	gc.set_texture(0, texture);
	CL_Draw::texture(gc, geometry, CL_Colorf(0.5f, 0.5f, 0.5f));
	gc.reset_texture(0);
}

void PostProcessEffectTransparency::render(CL_GraphicContext &gc, CL_Texture &texture, CL_Rect geometry)
{
	float alpha = scene->get_transparency();
	gc.set_texture(0, texture);
	CL_Draw::texture(gc, geometry, CL_Colorf(alpha, alpha, alpha, alpha));
	gc.reset_texture(0);
}
