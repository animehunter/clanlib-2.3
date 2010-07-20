
#include "precomp.h"
#include "game_terrain.h"
#include "3D/light_model.h"

GameTerrain::GameTerrain(
	CL_GraphicContext &gc, 
	const CL_String &heightmap_png, 
	const CL_String &texture_png, 
	const CL_String &areas_png, 
	const CL_String &borders_png, 
	int area_count, 
	float vertical_scale)
: num_vertices(0)
{
	color_areas = CL_PNGProvider::load(areas_png).to_format(cl_r8);

	CL_PixelBuffer pb = CL_PNGProvider::load(heightmap_png).to_format(cl_rgba8);
	int width = pb.get_width()-1;
	int height = pb.get_height()-1;

	num_vertices = width*height*3*4 /*+ width*6*2 + height*6*2 + 6*/;
	vertex_buffer = CL_VertexArrayBuffer(gc, num_vertices*sizeof(Vertex));
	vertex_buffer.lock(cl_access_write_only);
	Vertex *vertex_data = reinterpret_cast<Vertex *>(vertex_buffer.get_data());

	int pitch = pb.get_pitch();
	unsigned char *data = reinterpret_cast<unsigned char *>(pb.get_data());
	for (int y = 0; y < height; y++)
	{
		unsigned int *line1 = reinterpret_cast<unsigned int *>(data + y*pitch);
		unsigned int *line2 = reinterpret_cast<unsigned int *>(data + (y+1)*pitch);
		Vertex *vertex_line = vertex_data + 3*4*width*y;
		for (int x = 0; x < width; x++)
		{
			float height1 = (line1[x] >> 24) * vertical_scale;
			float height2 = (line1[x+1] >> 24) * vertical_scale;
			float height3 = (line2[x] >> 24) * vertical_scale;
			float height4 = (line2[x+1] >> 24) * vertical_scale;
			float height5 = (height1 + height2 + height3 + height4) / 4.0f;
			Vertex *vertex_quad = vertex_line + x*3*4;

			CL_Vec3f positions[12] =
			{
				CL_Vec3f(x+0.0f, height1, y+0.0f),
				CL_Vec3f(x+1.0f, height2, y+0.0f),
				CL_Vec3f(x+0.5f, height5, y+0.5f),

				CL_Vec3f(x+1.0f, height2, y+0.0f),
				CL_Vec3f(x+1.0f, height4, y+1.0f),
				CL_Vec3f(x+0.5f, height5, y+0.5f),

				CL_Vec3f(x+1.0f, height4, y+1.0f),
				CL_Vec3f(x+0.0f, height3, y+1.0f),
				CL_Vec3f(x+0.5f, height5, y+0.5f),

				CL_Vec3f(x+0.0f, height3, y+1.0f),
				CL_Vec3f(x+0.0f, height1, y+0.0f),
				CL_Vec3f(x+0.5f, height5, y+0.5f)
			};

			for (int i = 0; i < 12; i++)
				vertex_quad[i].position = positions[i];

			CL_Vec3f normal1 = calc_normal(x, y, data, width, height, pitch);
			CL_Vec3f normal2 = calc_normal(x+1, y, data, width, height, pitch);
			CL_Vec3f normal3 = calc_normal(x, y+1, data, width, height, pitch);
			CL_Vec3f normal4 = calc_normal(x+1, y+1, data, width, height, pitch);
			CL_Vec3f normal5 = (normal1+normal2+normal3+normal4)/4.0;

			vertex_quad[0].normal = normal1;
			vertex_quad[1].normal = normal2;
			vertex_quad[2].normal = normal5;
			vertex_quad[3].normal = normal2;
			vertex_quad[4].normal = normal4;
			vertex_quad[5].normal = normal5;
			vertex_quad[6].normal = normal4;
			vertex_quad[7].normal = normal3;
			vertex_quad[8].normal = normal5;
			vertex_quad[9].normal = normal3;
			vertex_quad[10].normal = normal1;
			vertex_quad[11].normal = normal5;
		}
	}

	vertex_data += width*height*3*4;
/*
	for (int y = 0; y < height; y++)
	{
		unsigned int *line1 = reinterpret_cast<unsigned int *>(data + y*pitch);
		unsigned int *line2 = reinterpret_cast<unsigned int *>(data + (y+1)*pitch);
		float scale = 0.4f;
		float height1 = (line1[0] >> 24)*scale;
		float height2 = (line2[0] >> 24)*scale;
		float height3 = (line1[width] >> 24)*scale;
		float height4 = (line2[width] >> 24)*scale;

		vertex_data[y*6*2+0].position = CL_Vec3f(0, -0.1f, (float)y);
		vertex_data[y*6*2+1].position = CL_Vec3f(0, (float)height1, (float)y);
		vertex_data[y*6*2+2].position = CL_Vec3f(0, (float)height2, (float)y+1);
		vertex_data[y*6*2+3].position = CL_Vec3f(0, (float)height2, (float)y+1);
		vertex_data[y*6*2+4].position = CL_Vec3f(0, -0.1f, (float)y+1);
		vertex_data[y*6*2+5].position = CL_Vec3f(0, -0.1f, (float)y);

		vertex_data[y*6*2+11].position = CL_Vec3f((float)width+1, -0.1f, (float)y);
		vertex_data[y*6*2+10].position = CL_Vec3f((float)width+1, (float)height3, (float)y);
		vertex_data[y*6*2+9].position = CL_Vec3f((float)width+1, (float)height4, (float)y+1);
		vertex_data[y*6*2+8].position = CL_Vec3f((float)width+1, (float)height4, (float)y+1);
		vertex_data[y*6*2+7].position = CL_Vec3f((float)width+1, -0.1f, (float)y+1);
		vertex_data[y*6*2+6].position = CL_Vec3f((float)width+1, -0.1f, (float)y);

		for (int i = 0; i < 6; i++)
			vertex_data[y*6*2+i].normal = CL_Vec3f(-1, 0, 0);
		for (int i = 0; i < 6; i++)
			vertex_data[y*6*2+6+i].normal = CL_Vec3f(1, 0, 0);
	}

	vertex_data += height*6*2;

	unsigned int *line1 = reinterpret_cast<unsigned int *>(data);
	unsigned int *line2 = reinterpret_cast<unsigned int *>(data + height*pitch);

	for (int x = 0; x < width; x++)
	{
		float scale = 0.4f;
		float height1 = (line1[x] >> 24)*scale;
		float height2 = (line1[x+1] >> 24)*scale;
		float height3 = (line2[x] >> 24)*scale;
		float height4 = (line2[x+1] >> 24)*scale;

		vertex_data[x*6*2+5].position = CL_Vec3f((float)x, -0.1f, 0);
		vertex_data[x*6*2+4].position = CL_Vec3f((float)x, (float)height1, 0);
		vertex_data[x*6*2+3].position = CL_Vec3f((float)x+1, (float)height2, 0);
		vertex_data[x*6*2+2].position = CL_Vec3f((float)x+1, (float)height2, 0);
		vertex_data[x*6*2+1].position = CL_Vec3f((float)x+1, -0.1f, 0);
		vertex_data[x*6*2+0].position = CL_Vec3f((float)x, -0.1f, 0);

		vertex_data[x*6*2+6].position = CL_Vec3f((float)x, -0.1f, (float)height+1);
		vertex_data[x*6*2+7].position = CL_Vec3f((float)x, (float)height3, (float)height+1);
		vertex_data[x*6*2+8].position = CL_Vec3f((float)x+1, (float)height4, (float)height+1);
		vertex_data[x*6*2+9].position = CL_Vec3f((float)x+1, (float)height4, (float)height+1);
		vertex_data[x*6*2+10].position = CL_Vec3f((float)x+1, -0.1f, (float)height+1);
		vertex_data[x*6*2+11].position = CL_Vec3f((float)x, -0.1f, (float)height+1);

		for (int i = 0; i < 6; i++)
			vertex_data[x*6*2+i].normal = CL_Vec3f(0, 0, -1);
		for (int i = 0; i < 6; i++)
			vertex_data[x*6*2+6+i].normal = CL_Vec3f(0, 0, 1);
	}

	vertex_data += width*6*2;

	vertex_data[0].position = CL_Vec3f(0, -0.1f, 0);
	vertex_data[1].position = CL_Vec3f((float)width, -0.1f, 0);
	vertex_data[2].position = CL_Vec3f((float)width, -0.1f, (float)height);
	vertex_data[3].position = CL_Vec3f((float)width, -0.1f, (float)height);
	vertex_data[4].position = CL_Vec3f(0, -0.1f, (float)height);
	vertex_data[5].position = CL_Vec3f(0, -0.1f, 0);
	for (int i = 0; i < 6; i++)
		vertex_data[i].normal = CL_Vec3f(0, -1, 0);
*/
	vertex_buffer.unlock();

	size = CL_Size(width+1, height+1);

	this->area_count = area_count;

	CL_VirtualDirectory vdir;
	shader_program = CL_ProgramObject::load(gc, "Resources/map_vertex.glsl", "Resources/map_fragment.glsl", vdir);
	shader_program.bind_attribute_location(0, "in_position");
	shader_program.bind_attribute_location(1, "in_normal");
	if (!shader_program.link())
		throw CL_Exception("Map shader program failed to link: " + shader_program.get_info_log());

	texture_base = CL_Texture(gc, texture_png);
	texture_base.set_min_filter(cl_filter_linear);
	texture_base.set_mag_filter(cl_filter_linear);

	// Change the values between area colors to avoid GPUs merging them together
	CL_PixelBuffer image = CL_ImageProviderFactory::load(areas_png).to_format(cl_rgba8);
	unsigned int* image_data = reinterpret_cast<unsigned int*>(image.get_data());
	int image_total_pixels = image.get_width()*image.get_height();
	for (int i = 0; i < image_total_pixels; i++)
	{
		unsigned int area_color = (image_data[i]>>24)*255/area_count;
		image_data[i] = (area_color<<24)+(area_color<<16)+(area_color<<8)+area_color;
	}
	texture_areas = CL_Texture(gc, image.get_size());
	texture_areas.set_image(image);
	texture_areas.set_min_filter(cl_filter_nearest);
	texture_areas.set_mag_filter(cl_filter_nearest);

	texture_borders = CL_Texture(gc, borders_png);
	texture_borders.set_min_filter(cl_filter_linear);
	texture_borders.set_mag_filter(cl_filter_linear);
}

void GameTerrain::set_area_colors(CL_GraphicContext &gc, const std::vector<unsigned int> &colors)
{
	CL_PixelBuffer buffer_colors(colors.size(), 1, cl_rgba8);
	unsigned int *col = reinterpret_cast<unsigned int *>(buffer_colors.get_data());
	for(unsigned int i=0;i<colors.size();++i)
		col[i] = colors[i];

	texture_colors = CL_Texture(gc, area_count+1, 1);
	texture_colors.set_image(buffer_colors);
	texture_colors.set_wrap_mode(cl_wrap_clamp_to_edge, cl_wrap_clamp_to_edge);
	texture_colors.set_min_filter(cl_filter_nearest);
	texture_colors.set_mag_filter(cl_filter_nearest);
}

inline CL_Vec3f GameTerrain::calc_normal(int x, int y, unsigned char *data, int width, int height, int pitch)
{
	CL_Vec3f vectors[9];
	for (int yy = 0; yy < 3; yy++)
	{
		for (int xx = 0; xx < 3; xx++)
		{
			vectors[xx+yy*3] = get_vector(x, y, xx-1, yy-1, data, width, height, pitch);
		}
	}

	CL_Vec3f normals[8] =
	{
		CL_Vec3f::cross(vectors[1], vectors[0]),
		CL_Vec3f::cross(vectors[2], vectors[1]),
		CL_Vec3f::cross(vectors[5], vectors[2]),
		CL_Vec3f::cross(vectors[8], vectors[5]),
		CL_Vec3f::cross(vectors[7], vectors[8]),
		CL_Vec3f::cross(vectors[6], vectors[7]),
		CL_Vec3f::cross(vectors[3], vectors[6]),
		CL_Vec3f::cross(vectors[0], vectors[3])
	};
	for (int i = 0; i < 8; i++)
		normals[i].normalize();

	CL_Vec3f normal(0.0f, 0.0f, 0.0f);
	for (int i = 0; i < 8; i++)
		normal += normals[i];
	normal /= 8.0f;
	normal.normalize();
	return normal;
}

inline CL_Vec3f GameTerrain::get_vector(int x, int y, int xx, int yy, unsigned char *data, int width, int height, int pitch)
{
	CL_Vec3f v((float) xx, 0.0f, (float) yy);
	int px = x+xx;
	int py = y+yy;
	if (px < 0)
		px = 0;
	if (px > width)
		px = width;
	if (py < 0)
		py = 0;
	if (py > height)
		py = height;

	unsigned int *pixel1 = reinterpret_cast<unsigned int *>(data+y*pitch+x*4);
	unsigned int *pixel2 = reinterpret_cast<unsigned int *>(data+py*pitch+px*4);
	float h1 = ((*pixel1) >> 24) * 0.4f;
	float h2 = ((*pixel2) >> 24) * 0.4f;
	v.y = h2 - h1;
	return v;
}

void GameTerrain::render(CL_GraphicContext &gc, const LightModel &light_model)
{
	CL_Rectf rect(0, 0, (float)size.width-1, (float)size.height-1);
	render_sprite(gc, light_model, rect);
}

void GameTerrain::render_sprite(CL_GraphicContext &gc, const LightModel &light_model, CL_Rectf dest)
{
	gc.push_modelview();
	gc.mult_scale(4.0, 1.0, 4.0);
	gc.mult_modelview(CL_Mat4f::translate(-size.width/2.0f, 0.0f, -size.height/2.0f));

	CL_PolygonRasterizer polygon_rasterizer;
	polygon_rasterizer.set_culled(true);
	polygon_rasterizer.set_front_face(cl_face_side_counter_clockwise);
	gc.set_polygon_rasterizer(polygon_rasterizer);

	CL_PrimitivesArray prim_array(gc);
	prim_array.set_attributes(0, vertex_buffer, 3, cl_type_float, &static_cast<Vertex*>(0)->position, sizeof(Vertex));
	prim_array.set_attributes(1, vertex_buffer, 3, cl_type_float, &static_cast<Vertex*>(0)->normal, sizeof(Vertex));

	gc.set_texture(0, texture_base);
	gc.set_texture(1, texture_areas);
	gc.set_texture(2, texture_colors);
	gc.set_texture(3, texture_borders);

	gc.set_program_object(shader_program);

	Material material;
	// material.shininess = 25.0f;

	shader_program.set_uniform4f("area", CL_Vec4f(dest.left, dest.top, dest.get_width(), dest.get_height()));
	shader_program.set_uniform1i("texture_base", 0);
	shader_program.set_uniform1i("texture_areas", 1);
	shader_program.set_uniform1i("texture_colors", 2);
	shader_program.set_uniform1i("texture_borders", 3);
/*
	shader_program.set_uniform("lightSourcePosition", light_model.light_sources[0].position);
	shader_program.set_uniform("frontLightProductAmbient", light_model.get_light_ambient(material, light_model.light_sources[0]));
	shader_program.set_uniform("frontLightProductDiffuse", light_model.get_light_diffuse(material, light_model.light_sources[0]));
	shader_program.set_uniform("frontLightProductSpecular", light_model.get_light_specular(material, light_model.light_sources[0]));
	shader_program.set_uniform("frontMaterialShininess", material.shininess);
	shader_program.set_uniform("frontLightModelProductSceneColor", light_model.get_scene_color(material));
*/
	gc.draw_primitives(cl_triangles, num_vertices, prim_array);

	gc.reset_program_object();
	gc.reset_texture(0);
	gc.reset_texture(1);
	gc.reset_texture(2);
	gc.reset_texture(3);

	gc.reset_polygon_rasterizer();

	gc.pop_modelview();
}

CL_Point GameTerrain::from_screen_point(CL_GraphicContext &gc, const CL_Point &screen_point, const CL_Mat4f &projection, const CL_Mat4f &modelview2, const CL_Rect &viewport)
{
	CL_OpenGL::set_active(gc);
	
	float depth;
	clReadPixels(screen_point.x, screen_point.y, 1, 1, CL_DEPTH_COMPONENT, CL_FLOAT, &depth);

	CL_Mat4f modelview = modelview2;
	modelview.multiply(CL_Mat4f::translate(-size.width*2.0f, 0.0f, -size.height*2.0f));
	CL_Vec3f pos = unproject(CL_Vec3f((float)screen_point.x, (float)screen_point.y, depth), modelview, projection, viewport);
	return CL_Point((int)(pos.x+0.5f),(int)(pos.z+0.5f));
}

unsigned int GameTerrain::get_area(const CL_Point &map_point)
{
	int x = map_point.x * 4;
	int y = map_point.y * 4;

	if (x >= 0 && x < color_areas.get_width())
	{
		if (y >= 0 && y < color_areas.get_height())
		{
			char *area_data = (char *)color_areas.get_data();
			
			char color = area_data[y * color_areas.get_width() + x];
			return (unsigned int) color;
		}
	}

	return 0;
}

CL_Vec3f GameTerrain::unproject(CL_Vec3f win_pos, const CL_Mat4f &modelview, const CL_Mat4f &projection, const CL_Rect &viewport)
{
	CL_Mat4f matrix = projection;
	matrix.multiply(modelview);
	matrix.inverse();
	CL_Vec4f v(
		2*(win_pos.x - (float)viewport.left)/(float)viewport.get_width() - 1,
		2*(win_pos.y - (float)(viewport.top))/(float)viewport.get_height() - 1,
		2*win_pos.z - 1,
		1.0f);
	CL_Vec4f result = matrix * v;
	return CL_Vec3f(result.x/result.w, result.y/result.w, result.z/result.w);
}
