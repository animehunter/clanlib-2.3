/*
////////////////////////////////////////////////////////////////////////////
// LinearParticle Example - MSmall
////////////////////////////////////////////////////////////////////////////

This example shows a special effect could be made by using L_ExplosionEffect
and 1 dimentional acceleration.


 - 26/09/2006

////////////////////////////////////////////////////////////////////////////
*/

#include "precomp.h"
#include "msmall.h"
#include "framerate_counter.h"

int DemoMSmall::run(CL_DisplayWindow &window)
{
	window.set_title("LinearParticle Example - MSmall ");
	CL_Slot slot_quit = window.sig_window_close().connect(this, &DemoMSmall::on_window_close);
	CL_GraphicContext gc = window.get_gc();
	CL_Slot slot_input_up = (window.get_ic().get_keyboard()).sig_key_up().connect(this, &DemoMSmall::on_key_up);

	// initialize LinearParticle
	L_ParticleSystem::init();

	// create surface to be used for particle and set the alignment
	CL_Sprite surface(gc, "Resources/small.png");
	surface.set_alignment(origin_center);

	motion_ctrl.set_speed_limit(0.1); // set max speed of particle
	motion_ctrl.set_1d_acceleration(-0.0003); // set deceleration

	// create sample of particle
	L_Particle particle(&surface,2000);
	particle.set_color(L_Color(255,110,60,255));
	particle.coloring2(L_Color(255,110,60,255),L_Color(0,200,100,255),0.6);
	particle.set_motion_controller(&motion_ctrl); // assign motion cotroller


	// create explosion effect
	effect = new L_ExplosionEffect(320,240,16,10,12,0.1);
	effect->add(&particle);

	// apply random life distortion for each emitted particle
	effect->set_life_distortion(700);
	effect->initialize();

	char str[32];
	quit = false;
	show_menu = true;

	CL_Font font(gc, "Arial", 16 );
	FramerateCounter frameratecounter;
	unsigned int last_time = CL_System::get_time();

	CL_InputDevice &keyboard = window.get_ic().get_keyboard();
	while(!quit)
	{
		gc.clear();


		static L_REAL x_pos = 320;
		static L_REAL y_pos = 240;

		L_REAL x_vel = 0;
		L_REAL y_vel = 0;
		if( keyboard.get_keycode(CL_KEY_UP) )
			y_vel = -0.2;

		else if( keyboard.get_keycode(CL_KEY_DOWN))
			y_vel = 0.2;

		if( keyboard.get_keycode(CL_KEY_LEFT))
			x_vel = -0.2;

		else if( keyboard.get_keycode(CL_KEY_RIGHT))
			x_vel = 0.2;

		L_Vector vel;
		vel.set( x_vel, y_vel );

		unsigned int current_time = CL_System::get_time();
		int time_run = current_time - last_time;
		last_time = current_time;

		x_pos += x_vel*time_run;
		y_pos += y_vel*time_run;

		effect->trigger();
		effect->set_velocity(vel);
		effect->run(time_run);
		effect->set_position(x_pos, y_pos);
		L_DrawParticle(gc, effect);

		if( show_menu )
		{
			frameratecounter.show_fps(gc, font);

			sprintf(str,"#Particle : %d", effect->get_particle_num());
			font.draw_text(gc,10,30,str);

			font.draw_text(gc,10,410,"F1 : hide/show menu");
			font.draw_text(gc,10,425,"Space : trigger random sleep");
			font.draw_text(gc,10,440,"Arrow keys : move the effect");
		}


		window.flip(0);	// Set to "1" to lock to screen refresh rate
		frameratecounter.frame_shown();

		CL_KeepAlive::process(0);
	}

	delete effect;

	// deinitialize LinearParticle
	L_ParticleSystem::deinit();

	return 0;
}

void DemoMSmall::on_key_up(const CL_InputEvent& key, const CL_InputState&)
{
	if( key.id == CL_KEY_ESCAPE )
		quit = true;

	else if( key.id == CL_KEY_F1)
		show_menu = !show_menu;

	else if( key.id == CL_KEY_SPACE)
		CL_System::sleep(rand()%200+50);
}

void DemoMSmall::on_window_close()
{
	quit = true;
}

