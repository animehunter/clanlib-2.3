/*
////////////////////////////////////////////////////////////////////////////
// LinearParticle Example - Shooting
////////////////////////////////////////////////////////////////////////////

This example demostrates using L_ShootingEffect to make an effect.


 - 26/09/2006

////////////////////////////////////////////////////////////////////////////
*/

#include "precomp.h"
#include "shooting.h"
#include "framerate_counter.h"

int DemoShooting::run(CL_DisplayWindow &window)
{
	window.set_title("LinearParticle Example - Shooting ");

	CL_Slot slot_quit = window.sig_window_close().connect(this, &DemoShooting::on_window_close);
	CL_GraphicContext gc = window.get_gc();
	CL_Slot slot_input_up = (window.get_ic().get_keyboard()).sig_key_up().connect(this, &DemoShooting::on_key_up);

	// initialize LinearParticle
	L_ParticleSystem::init();

	// create surface to be used for particle and set the alignment
	CL_Sprite surface(gc,"Resources/star.png");
	surface.set_alignment(origin_center);

	motion_ctrl.set_1d_acceleration(-0.0003);

	L_Particle particle(&surface,3000);
	particle.set_color(L_Color(255,110,60,255));
	particle.coloring2(L_Color(255,110,60,255),L_Color(0,200,100,255),0.6);
	particle.rotating2(L_2PI);
	particle.set_motion_controller(&motion_ctrl);

	L_Vector shooting_vector;
	shooting_vector.set2( 0.4, L_DEGREE_TO_RADIAN(-90) );
	effect = new L_ShootingEffect(460,360,shooting_vector,16,4);
	effect->add(&particle);
	effect->set_width_interval(100);
	effect->set_angle_interval(L_DEGREE_TO_RADIAN(45));
	effect->set_life_distortion(600);
	effect->set_size_distortion(0.4f);
	effect->initialize();


	char str[32];
	quit = false;
	show_menu = true;

	CL_Font font(gc, "Arial", 16 );
	FramerateCounter frameratecounter;
	unsigned int last_time = CL_System::get_time();

	while(!quit)
	{
		gc.clear();

		unsigned int current_time = CL_System::get_time();
		int time_run = current_time - last_time;
		last_time = current_time;

		/* the maximum time step is set to 50milisecs to avoid artifacts
		and errors caused by low frame rate to be less noticeable. */
		while( time_run > 50 )
		{
			run_a_step(50);
			time_run -= 50;
		}

		if( time_run > 0 )
			run_a_step(time_run);


		L_DrawParticle(gc, effect);

		if( show_menu )
		{
			frameratecounter.show_fps(gc, font);

			sprintf(str,"#Particle : %d", effect->get_particle_num());
			font.draw_text(gc,10,30,str);

			font.draw_text(gc,10,425,"F1 : hide/show menu");
			font.draw_text(gc,10,440,"Space : trigger random sleep");
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

void DemoShooting::on_key_up(const CL_InputEvent& key, const CL_InputState &)
{
	if( key.id == CL_KEY_ESCAPE )
		quit = true;

	else if( key.id == CL_KEY_F1)
		show_menu = !show_menu;

	else if( key.id == CL_KEY_SPACE)
		CL_System::sleep(rand()%200+50);
}

void DemoShooting::on_window_close()
{
	quit = true;
}

void DemoShooting::run_a_step(int time)
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

