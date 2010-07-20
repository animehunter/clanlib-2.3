/*
////////////////////////////////////////////////////////////////////////////
// LinearParticle Example - CMotion
////////////////////////////////////////////////////////////////////////////

This example demostrates the effect made by using L_MotionController and
point gravitation (point acceleration).


 - 06/06/2007

////////////////////////////////////////////////////////////////////////////
*/

#include "precomp.h"
#include "cmotion.h"
#include "framerate_counter.h"

DemoCMotion::DemoCMotion() : fontColor(255.0f,255.0f,255.0f), blendingMode(L_ADDITIVE_BLENDING)
{
}

int DemoCMotion::run(CL_DisplayWindow &window)
{

	window.set_title("LinearParticle Example - CMotion ");
	CL_Slot slot_quit = window.sig_window_close().connect(this, &DemoCMotion::on_window_close);
	CL_GraphicContext gc = window.get_gc();

	CL_Slot slot_input_up = (window.get_ic().get_keyboard()).sig_key_up().connect(this, &DemoCMotion::on_key_up,gc);

	// initialize LinearParticle
	L_ParticleSystem::init();

	// create surface to be used for particle and set the alignment
	CL_Sprite surface(gc,"Resources/sketch.png");
	surface.set_alignment(origin_center);
	font = new CL_Font(gc, "Arial", 16 );

	motion_ctrl.set_speed_limit(0.65f);
	motion_ctrl.set_point_acceleration( 320, 240, 0.001f );

	particle = new L_Particle(&surface,3000);
	particle->rotating4();
	particle->set_motion_controller(&motion_ctrl);


	L_Vector shooting_vector;
	shooting_vector.set2( 0.4, L_DEGREE_TO_RADIAN(-90) );
	effect = new L_ShootingEffect(460,360,shooting_vector,16,6);
	effect->add(particle);
	effect->set_width_interval(20);
	effect->set_angle_interval(L_DEGREE_TO_RADIAN(40));
	effect->set_life_distortion(600);
	effect->set_size_distortion(0.4f);
	effect->set_speed_distortion(0.06f);
	effect->initialize();

	bg_color = CL_Colorf(0.0f,0.0f,0.0f);


	char str[32];
	quit = false;
	show_menu = true;


	random_ini_rotation = false;
	current_style = 0;
	set_style(gc);

	FramerateCounter frameratecounter;
	unsigned int last_time = CL_System::get_time();

	while(!quit)
	{
		gc.clear(bg_color);

		unsigned int current_time = CL_System::get_time();
		int time_run = current_time - last_time;
		last_time = current_time;

		/* the maximum time step is set to 50 milisecs to avoid artifacts
		and errors caused by low frame rate to be less noticeable. */
		while( time_run > 50 )
		{
			run_a_step(50);
			time_run -= 50;
		}

		if( time_run > 0 )
			run_a_step(time_run);

		
		if(blendingMode == L_ADDITIVE_BLENDING)
			L_DrawParticle(gc,effect);
		else
			L_DrawParticleMinusAlpha(gc,effect);

		if( show_menu )
		{
			frameratecounter.show_fps(gc, *font);

			sprintf(str,"#Particle : %d", effect->get_particle_num());
			font->draw_text(gc,10,30,str,fontColor);

			font->draw_text(gc,10,395,"F1 : hide/show menu",fontColor);
			font->draw_text(gc,10,410,"Space : trigger random sleep",fontColor);
			font->draw_text(gc,10,425,"Z key : change style",fontColor);
			font->draw_text(gc,10,440,"X Key : toggle randomization for initial rotation",fontColor);
		}


		window.flip(0);	// Set to "1" to lock to screen refresh rate
		frameratecounter.frame_shown();

		CL_KeepAlive::process(0);
	}

	delete font;
	delete effect;
	delete particle;

	// deinitialize LinearParticle
	L_ParticleSystem::deinit();
	return 0;
}

void DemoCMotion::set_style(CL_GraphicContext &gc)
{
	if( current_style == 0 )
	{
		bg_color = CL_Colorf(0.0f,0.0f,0.0f);

		particle->set_color(L_Color(255,110,60,255));
		particle->coloring2(L_Color(255,110,60,255),L_Color(0,200,100,255),0.8);
		blendingMode = L_ADDITIVE_BLENDING;

		fontColor = CL_Colorf(255.0f,255.0f,255.0f);
	}

	else
	{
		bg_color = CL_Colorf(255.0f,255.0f,255.0f);

		particle->set_color(L_Color(100,0,0,0));
		particle->coloring2(L_Color(100,0,0,0),L_Color(0,0,0,0),0.8);
		blendingMode = L_NORMAL_BLENDING;

		fontColor = CL_Colorf(0.0f,0.0f,0.0f);
	}

}

void DemoCMotion::on_key_up(const CL_InputEvent& key, const CL_InputState &, CL_GraphicContext gc)
{
	if( key.id == CL_KEY_ESCAPE )
		quit = true;

	else if( key.id == CL_KEY_Z)
	{
		current_style = (current_style+1)%2;
		set_style(gc);
	}

	else if( key.id == CL_KEY_X)
	{
		random_ini_rotation = !random_ini_rotation;
		if( random_ini_rotation )
			effect->set_rotation_distortion(L_DEGREE_TO_RADIAN(200));

		else
			effect->set_rotation_distortion(0);
	}

	else if( key.id == CL_KEY_F1)
		show_menu = !show_menu;

	else if( key.id == CL_KEY_SPACE)
		CL_System::sleep(rand()%200+50);
}

void DemoCMotion::on_window_close()
{
	quit = true;
}

void DemoCMotion::run_a_step(int time)
{
	static double rad = 0.0;
	static CL_Vec2<float> current_pos(460, 360);
	static CL_Vec2<float> prev_pos;

	rad -= 0.002*time;

	if( rad < L_2PI )
		rad += L_2PI;

	prev_pos = current_pos;
	current_pos.x = 140*cos(rad)+320;
	current_pos.y = 140*sin(rad)+360;

	L_Vector vel;
	vel.set( (current_pos.x-prev_pos.x)/time,
				(current_pos.y-prev_pos.y)/time );

	effect->trigger();

	/* it's recommended to use L_ParticleEffect::set_velocity() than just
	to use L_ParticleEffect::set_position() if the desired position of effect
	is not static or jumping. */
	effect->set_velocity(vel);
	effect->run(time);

	/* set position(although velocity has been set before) to avoid error
	being accumulated.*/
	effect->set_position(current_pos.x, current_pos.y);
}

