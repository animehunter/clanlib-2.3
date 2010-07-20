#include <ClanLib/core.h>
#include <ClanLib/application.h>
#include <ClanLib/display.h>

#define USE_GDI_BECAUSE_IT_IS_COOL

#ifdef USE_GDI_BECAUSE_IT_IS_COOL
#include <ClanLib/swrender.h>
#else
#include <ClanLib/gl.h>
#endif

class App
{
private:
	bool quit;

public:
	int start(const std::vector<CL_String> &args)
	{
		CL_ConsoleWindow console("Console");

		try
		{
			CL_DisplayWindow window("Image test", 1024, 768);
			CL_GraphicContext gc = window.get_gc();

			// Connect the Window close event
			CL_Slot slot_quit = window.sig_window_close().connect(this, &App::on_window_close);

			CL_BlendMode blend_mode1;
			blend_mode1.enable_blending(true);
			gc.set_blend_mode(blend_mode1);

			quit = false;

			CL_ResourceManager resources("resources.xml");

			CL_Texture texture(gc, "Images/square.png");

			CL_Image image_texture(gc, texture, CL_Rect(0, 0, texture.get_size()));
			CL_Image image_loaded(gc, "Images/square.png");
			CL_Image image_resources(gc, "entire_image", &resources);
			CL_Image image_copy(image_texture);
			CL_Image image_top_right(gc, "image_top_right", &resources);
			CL_Image image_bottom_right(gc, "image_bottom_right", &resources);
			CL_Image image_black(gc, "image_black", &resources);

			CL_Font small_font = CL_Font(gc, "Tahoma", 12);

			//CL_Console::write_line("Color: %1,%2,%3,%4", image_resources.get_color().r, image_resources.get_color().g, image_resources.get_color().b, image_resources.get_color().a);
			//CL_Console::write_line("Scale: %1,%2", image_resources.get_scale_x(), image_resources.get_scale_y());
			//CL_Console::write_line("Translation: %1,%2,%3", image_resources.get_alignment());

			while((!quit) && (!window.get_ic().get_keyboard().get_keycode(CL_KEY_ESCAPE)))
			{
				gc.clear(CL_Colorf(0.5f,0.5f,0.5f));

				small_font.draw_text(gc, 10, 40, "Image From Texture (10,60)");
				image_texture.draw(gc, 10, 60);

				small_font.draw_text(gc, 150, 40, "Image From Load (150,60)");
				image_loaded.draw(gc, 150, 60);

				small_font.draw_text(gc, 300, 40, "Image From Resources (300,60)");
				image_resources.draw(gc, 300, 60);

				small_font.draw_text(gc, 450, 40, "Image Copied (450,60)");
				image_copy.draw(gc, 450, 60);

				small_font.draw_text(gc, 10, 190, "Image - Top Right (10,200)");
				image_top_right.draw(gc, 10, 200);

				small_font.draw_text(gc, 150, 190, "Image - Top Right (150,200)");
				image_texture.draw(gc, CL_Rect(32, 0, CL_Size(32, 32)), CL_Rect(150, 200, CL_Size(32, 32)));

				small_font.draw_text(gc, 300, 190, "Image - Bottom Right (300,200)");
				image_bottom_right.draw(gc, 300, 200);

				small_font.draw_text(gc, 450, 190, "Image - Bottom Right (450,200)");
				image_texture.draw(gc, CL_Rect(32, 32, CL_Size(32, 32)), CL_Rect(450, 200, CL_Size(32, 32)));

				small_font.draw_text(gc, 10, 290, "700 Images (10,300)");
				for(int i=0;i<700;i++)
					image_texture.draw(gc, 10, 300);

				small_font.draw_text(gc, 150, 290, "br image (150,400) Size(128,256)");
				image_bottom_right.draw(gc, CL_Rect(150, 300, CL_Size(128, 256)));

				small_font.draw_text(gc, 300, 290, "Image - black");
				image_black.draw(gc, 300, 300);

				small_font.draw_text(gc, 300, 490, "Image - Scale (1.5, 2.5)");
				image_texture.set_scale(1.5f, 2.5f);
				image_texture.draw(gc, 300, 500);
				image_texture.set_scale(1.0f, 1.0f);

				small_font.draw_text(gc, 450, 460, "Image - Alignment (4 images with 8 pixel offset)");
				small_font.draw_text(gc, 450, 475, "(top left, top right, bottom left, bottom right)");
				small_font.draw_text(gc, 450, 490, "(Circle denotes the draw origin)");
				const int offset = 96;

				image_texture.set_alignment(origin_top_left, 8, 8);
				image_texture.draw(gc, 450+offset, 500+offset);
				image_texture.set_alignment(origin_top_right, -8, 8);
				image_texture.draw(gc, 450+offset, 500+offset);

				image_texture.set_alignment(origin_bottom_left, 8, -8);
				image_texture.draw(gc, 450+offset, 500+offset);
				image_texture.set_alignment(origin_bottom_right, -8, -8);
				image_texture.draw(gc, 450+offset, 500+offset);

				CL_Draw::circle(gc, 450+offset, 500+offset, 4, CL_Colorf(1.0f, 1.0f, 1.0f, 0.9f));

				small_font.draw_text(gc, 700, 460, "Image - Center Alignment (4 images with 8 pixel offset)");
				small_font.draw_text(gc, 700, 475, "(top center, right center, bottom center, left center)");
				small_font.draw_text(gc, 700, 490, "(Circle denotes the draw origin)");

				image_texture.set_alignment(origin_top_center, 0, 8);
				image_texture.draw(gc, 700+offset, 500+offset);
				image_texture.set_alignment(origin_bottom_center, 0, -8);
				image_texture.draw(gc, 700+offset, 500+offset);

				image_texture.set_alignment(origin_center_left, 8, 0);
				image_texture.draw(gc, 700+offset, 500+offset);
				image_texture.set_alignment(origin_center_right, -8, 0);
				image_texture.draw(gc, 700+offset, 500+offset);

				CL_Draw::circle(gc, 700+offset, 500+offset, 4, CL_Colorf(1.0f, 1.0f, 1.0f, 0.9f));

				small_font.draw_text(gc, 700, 160, "Image - Center Align (4 images with 64 pixel offset)");
				small_font.draw_text(gc, 700, 175, "Also Includes a centered image (Without offset)");
				small_font.draw_text(gc, 700, 190, "(Circle denotes the draw origin)");

				const int center_image_offset = 64;

				image_texture.set_alignment(origin_center, 0, 0);
				image_texture.draw(gc, 700+offset, 200+offset);

				image_texture.set_alignment(origin_center, 0, center_image_offset);
				image_texture.draw(gc, 700+offset, 200+offset);
				image_texture.set_alignment(origin_center, 0, -center_image_offset);
				image_texture.draw(gc, 700+offset, 200+offset);

				image_texture.set_alignment(origin_center, center_image_offset, 0);
				image_texture.draw(gc, 700+offset, 200+offset);
				image_texture.set_alignment(origin_center, -center_image_offset, 0);
				image_texture.draw(gc, 700+offset, 200+offset);

				CL_Draw::circle(gc, 700+offset, 200+offset, 4, CL_Colorf(1.0f, 1.0f, 1.0f, 0.9f));

				// Restore alignment
				image_texture.set_alignment(origin_top_left, 0, 0);

				dump_fps();

				window.flip(1);
				CL_KeepAlive::process();
			}

			return 0;
		}
		catch(CL_Exception error)
		{
			CL_Console::write_line("Exception caught:");
			CL_Console::write_line(error.message);
			console.display_close_message();

			return -1;
		}

		return 0;
	}

	void on_window_close()
	{
		quit = true;
	}

	int dump_fps()
	{
		// Calculate time since last update
		static int start_time = CL_System::get_time();
		static int fps_dump_time = 0;

		int cur_time = CL_System::get_time();
		int delta_time = cur_time - start_time;
		start_time = cur_time;

		int fps = calc_fps(delta_time);

		fps_dump_time += delta_time;
		if(fps_dump_time >= 1000)
		{
			CL_Console::write_line("fps:" + CL_StringHelp::int_to_text(fps));
			fps_dump_time = 0;
		}

		return delta_time;
	}

	int calc_fps(int frame_time)
	{
		static int fps_result = 0;
		static int fps_counter = 0;
		static int total_time = 0;

		total_time += frame_time;
		if(total_time >= 1000)	// One second has passed
		{
			fps_result = fps_counter + 1;
			fps_counter = 0;
			total_time -= 1000;
		}
		fps_counter++;	// Increase fps

		return fps_result;
	}
};

class Program
{
public:
	static int main(const std::vector<CL_String> &args)
	{
		CL_SetupCore setup_core; 
		CL_SetupDisplay setup_disp;

#ifdef USE_GDI_BECAUSE_IT_IS_COOL
		CL_SetupSWRender setup_swrender;
#else
		CL_SetupGL setup_gl;
#endif
		App app;
		return app.start(args);
	}

};

CL_ClanApplication app(&Program::main);
