/*
////////////////////////////////////////////////////////////////////////////
// LinearParticle Example - Circle
////////////////////////////////////////////////////////////////////////////

This example demostrates coloring effect and using dropping effect to make
circular light trail.


 - 26/09/2006

////////////////////////////////////////////////////////////////////////////
*/

#include "precomp.h"
#include "circle.h"
#include "framerate_counter.h"

int DemoCircle::run(CL_DisplayWindow &window)
{
	window.set_title("LinearParticle Example - Circle ");
	CL_Slot slot_quit = window.sig_window_close().connect(this, &DemoCircle::on_window_close);
	CL_GraphicContext gc = window.get_gc();
	CL_Slot slot_input_up = (window.get_ic().get_keyboard()).sig_key_up().connect(this, &DemoCircle::on_key_up);

	// initialize LinearParticle
	L_ParticleSystem::init();

	// create surface to be used for particle and set the alignment
	CL_Sprite surface(gc, "Resources/light16p.png");
	surface.set_alignment(origin_center);

	// create a sample of particle with life of 2000
	L_Particle particle(&surface,2000);
	particle.set_size(2.0);
	particle.coloring2( L_Color(255,130,255,20), L_Color(0,255,10,10) );

	dropping_period = 16;
	// create dropping effect with period of 16
	dropper = new L_DroppingEffect(480,240,dropping_period);

	// add the particle to dropper effect
	dropper->add(&particle);

	// initialize particle effect
	dropper->initialize();

	char str[64];
	quit = false;
	show_menu = true;

	CL_Font font(gc, "tahoma", 16 );

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


		L_DrawParticle(gc, dropper);

		if( show_menu )
		{
			sprintf(str,"Period (millisecs) : %d",dropping_period);
			font.draw_text(gc,10,30,str);

			font.draw_text(gc,10,410,"F1 : hide/show menu");
			font.draw_text(gc,10,425,"Space : trigger random sleep");
			font.draw_text(gc,10,440,"Up/Down : change dropping period");
		}

		frameratecounter.show_fps(gc, font);
		window.flip(0);	// Set to "1" to lock to screen refresh rate
		frameratecounter.frame_shown();

		CL_KeepAlive::process(0);
	}

	delete dropper;

	// deinitialize LinearParticle
	L_ParticleSystem::deinit();

	return 0;
 }

// A key was pressed
void DemoCircle::on_key_up(const CL_InputEvent &key, const CL_InputState &state)
{
	if( key.id == CL_KEY_ESCAPE )
		quit = true;

	else if( key.id == CL_KEY_UP )
	{
		dropping_period++;
		if( dropping_period > 80 )
			dropping_period = 80;

		dropper->set_period(dropping_period);
	}

	else if( key.id == CL_KEY_DOWN )
	{
		dropping_period--;
		if( dropping_period < 5 )
			dropping_period = 5;

		dropper->set_period(dropping_period);
	}

	else if( key.id == CL_KEY_F1)
		show_menu = !show_menu;

	else if( key.id == CL_KEY_SPACE)
		CL_System::sleep(rand()%200+50);

}

// The window was closed
void DemoCircle::on_window_close()
{
	quit = true;
}

void DemoCircle::run_a_step(int time)
{
	static double rad = 0.0;
	static CL_Vec2<float> current_pos(480, 240);
	static CL_Vec2<float> prev_pos;

	rad += 0.0026*time;

	if( rad > L_2PI )
		rad -= L_2PI;

	prev_pos = current_pos;
	current_pos.x = 160*cos(rad)+320;
	current_pos.y = 160*sin(rad)+240;

	L_Vector vel;
	vel.set( (current_pos.x-prev_pos.x)/time,
				(current_pos.y-prev_pos.y)/time );

	/* it's recommended to use L_ParticleEffect::set_velocity() than just
	to use L_ParticleEffect::set_position() if the desired position of effect
	is not static or jumping. */
	dropper->set_velocity(vel);

	dropper->trigger();

	dropper->run(time);

	/* set position(although velocity has been set before) to avoid error
	being accumulated.*/
	dropper->set_position(current_pos.x, current_pos.y);
}
