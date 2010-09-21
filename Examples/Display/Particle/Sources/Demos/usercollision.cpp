/*
////////////////////////////////////////////////////////////////////////////
// LinearParticle Example - UserCollision
////////////////////////////////////////////////////////////////////////////

This example demostrates using user callback function to implement collision
detection for particles.


 - 06/06/2007

////////////////////////////////////////////////////////////////////////////
*/

#include "precomp.h"
#include "usercollision.h"
#include "framerate_counter.h"

int DemoUserCollision::run(CL_DisplayWindow &window)
{
	window.set_title("LinearParticle Example - UserCollision ");

	CL_Slot slot_quit = window.sig_window_close().connect(this, &DemoUserCollision::on_window_close);
	CL_GraphicContext gc = window.get_gc();

	CL_Slot slot_key_up = (window.get_ic().get_keyboard()).sig_key_up().connect(this, &DemoUserCollision::on_key_up);

	// initialize LinearParticle
	srand(CL_System::get_time());
	L_ParticleSystem::init();

	// create surface to be used for particle and set the alignment
	CL_Sprite particle_1_sur(gc, "Resources/sketch.png");
	particle_1_sur.set_alignment(origin_center);

	CL_Sprite particle_2_sur(gc, "Resources/explosion.png");
	particle_2_sur.set_alignment(origin_center);

	/* If L_INFINITE_LIFE is used, the system will not remove
	the particle automatically. */
	particle_1 = new L_Particle(&particle_1_sur, L_INFINITE_LIFE);
	particle_1->set_color(L_Color(255,255,150,70));

	// Set callback function.
	particle_1->set_prerun_callback(&check_collision, (void*)this);


	// Setup shooting effect.
	L_Vector shooting_vector;
	shooting_vector.set2( 0.4, L_DEGREE_TO_RADIAN(-90) );
	shooting_eff = new L_ShootingEffect(320,400,shooting_vector,40,1);
	shooting_eff->add(particle_1);
	shooting_eff->set_angle_interval(L_DEGREE_TO_RADIAN(25));
	shooting_eff->set_follow_shooting();
	shooting_eff->initialize();


	// Setup explosion effect
	motion_ctrl.set_1d_acceleration(-0.0004);
	particle_2 = new L_Particle(&particle_2_sur,500);
	particle_2->set_color( L_Color(255,110,60,255) );
	particle_2->coloring2( L_Color(255,255,255,100), L_Color(0,255,60,60) );
	particle_2->sizing2( 0.8, 1.5 );
	particle_2->set_motion_controller(&motion_ctrl);

	exp_effect = new L_ExplosionEffect(0,0,16,4,5,0.15f);
	exp_effect->add(particle_2);
	exp_effect->set_life(100); //set life of this effect
	exp_effect->set_rotation_distortion(L_2PI);
	exp_effect->set_size_distortion(0.7);
	exp_effect->set_life_distortion(100); //set life distortion for particles
	exp_effect->set_speed_distortion(0.1f);
	exp_effect->initialize();

	exp_emitter  = new L_EffectEmitter(exp_effect);

	circle_pos.set( 320, 100 );
	CL_Image circle_surface(gc, "Resources/circle.png");
	circle_surface.set_alignment(origin_center);

	CL_Image cflight_surface(gc, "Resources/cflight.png");
	cflight_surface.set_color( CL_Color(255,255,255,220) );
	cflight_surface.set_alignment(origin_center);

	quit = false;
	show_menu = true;

	CL_Font font(gc,  "Arial", 16 );
	FramerateCounter frameratecounter;
	unsigned int last_time = CL_System::get_time();

	CL_InputDevice &keyboard = window.get_ic().get_keyboard();
	while(!quit)
	{
		triggered = false;

		gc.clear();

		L_Vector vel;
		if( keyboard.get_keycode(CL_KEY_UP) )
			vel.y = -0.2;

		else if( keyboard.get_keycode(CL_KEY_DOWN))
			vel.y  = 0.2;

		if( keyboard.get_keycode(CL_KEY_LEFT))
			vel.x = -0.2;

		else if( keyboard.get_keycode(CL_KEY_RIGHT))
			vel.x = 0.2;

		if( keyboard.get_keycode(CL_KEY_Z))
			triggered = true;

		shooting_eff->set_velocity(vel);

		unsigned int current_time = CL_System::get_time();
		int time_run = current_time - last_time;
		last_time = current_time;

		while( time_run > 30 )
		{
			run_a_step(30);
			time_run -= 30;
		}

		if( time_run > 0 )
			run_a_step(time_run);

		circle_surface.draw(gc, circle_pos.x, circle_pos.y);
		L_DrawParticle(gc, shooting_eff);
		L_DrawParticle(gc, exp_emitter);
		cflight_surface.draw(gc, shooting_eff->x_pos, shooting_eff->y_pos);

		if( show_menu )
		{
			frameratecounter.show_fps(gc, font);

			font.draw_text(gc, 10,395,"F1 : hide/show menu");
			font.draw_text(gc, 10,410,"Space : trigger random sleep");
			font.draw_text(gc, 10,425,"Z Key : trigger shooting");
			font.draw_text(gc, 10,440,"Arrow keys : move the shooting source");
		}

		window.flip(0);	// Set to "1" to lock to screen refresh rate
		frameratecounter.frame_shown();

		CL_KeepAlive::process(0);
	}


	// deinitialize LinearParticle
	L_ParticleSystem::deinit();

	delete exp_emitter;
	delete exp_effect;
	delete particle_2;
	delete shooting_eff;
	delete particle_1;

	return 0;
}

void DemoUserCollision::on_key_up(const CL_InputEvent& key, const CL_InputState&)
{
	if( key.id == CL_KEY_ESCAPE )
		quit = true;

	else if( key.id == CL_KEY_F1)
		show_menu = !show_menu;

	else if( key.id == CL_KEY_SPACE)
		CL_System::sleep(rand()%200+50);
}

void DemoUserCollision::on_window_close()
{
	quit = true;
}

void DemoUserCollision::run_a_step(int time)
{
	if( triggered )
		shooting_eff->trigger();

	shooting_eff->run(time);
	exp_emitter->run(time);
}

void DemoUserCollision::check_collision(L_Particle& particle, const void* user_data)
{
	DemoUserCollision* app_oject = (DemoUserCollision*)user_data;

	// Remove the particle when its position is less than -8
	if( particle.y_pos < -8 )
		particle.remove();

	else
	{
		float circle_radius_sqr = 4096;
		float x_d = particle.x_pos - app_oject->circle_pos.x;
		float y_d = particle.y_pos - app_oject->circle_pos.y;

		/* Remove the projectile particle when it's collided with the big
		circle. Point-circle collision detection is used in this example
		for simplicity reason. */
		if( x_d*x_d + y_d*y_d < circle_radius_sqr )
		{
			app_oject->exp_emitter->emit(particle.x_pos, particle.y_pos);
			particle.remove();
		}
	}
}

