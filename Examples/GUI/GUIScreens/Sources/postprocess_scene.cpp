
#include "precomp.h"
#include "postprocess_scene.h"
#include "postprocess_effect.h"
#include "ui_screen.h"

PostProcessScene::PostProcessScene(CL_DisplayWindow display_window)
: display_window(display_window), gc(display_window.get_gc()), alpha(0.0f)
{
	t.func_expired().set(this, &PostProcessScene::on_timer);
	t.start(50);
}

void PostProcessScene::begin_scene()
{
	gc.clear();

	// Our textures are in pre-multiplied alpha
	CL_BlendMode blendmode;
	blendmode.enable_blending(true);
	blendmode.set_blend_function(cl_blend_one, cl_blend_one_minus_src_alpha, cl_blend_one, cl_blend_one_minus_src_alpha);
	gc.set_blend_mode(blendmode);
}

void PostProcessScene::render_component(CL_GUIComponent *component, CL_Texture &texture, CL_Rect geometry)
{
	UIScreen *uiscreen = dynamic_cast<UIScreen*>(component);
	PostProcessEffect *effect = uiscreen ? uiscreen->get_postprocess_effect() : 0;
	if (effect)
	{
		effect->render(gc, texture, geometry);
	}
	else
	{
		gc.set_texture(0, texture);
		CL_Draw::texture(gc, geometry);
		gc.reset_texture(0);
	}
}

void PostProcessScene::end_scene()
{
	gc.reset_blend_mode();
	display_window.flip();
}

void PostProcessScene::set_transparency(float alpha)
{
	if(alpha >= 0.0f && alpha <= 1.0f)
		this->alpha = alpha;
}

void PostProcessScene::on_timer()
{
	set_transparency(get_transparency() + 0.01f);
}
