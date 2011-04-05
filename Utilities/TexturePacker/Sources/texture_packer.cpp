/*
**  ClanLib SDK
**  Copyright (c) 1997-2011 The ClanLib Team
**
**  This software is provided 'as-is', without any express or implied
**  warranty.  In no event will the authors be held liable for any damages
**  arising from the use of this software.
**
**  Permission is granted to anyone to use this software for any purpose,
**  including commercial applications, and to alter it and redistribute it
**  freely, subject to the following restrictions:
**
**  1. The origin of this software must not be misrepresented; you must not
**     claim that you wrote the original software. If you use this software
**     in a product, an acknowledgment in the product documentation would be
**     appreciated but is not required.
**  2. Altered source versions must be plainly marked as such, and must not be
**     misrepresented as being the original software.
**  3. This notice may not be removed or altered from any source distribution.
**
**  Note: Some of the libraries ClanLib may link to may have additional
**  requirements or restrictions.
**
**  File Author(s):
**
**    Kenneth Gangstoe
*/

#include "precomp.h"
#include "texture_packer.h"
#include <iostream>

TexturePacker::TexturePacker()
{
}

TexturePacker::~TexturePacker()
{
}

void TexturePacker::load_resources(CL_GraphicContext &gc, const CL_String &filename)
{
	resources = CL_ResourceManager(filename);

	// TODO: Delete items before clearing
	resource_items.clear();

	std::vector<CL_String> resource_names = resources.get_resource_names();
	std::vector<CL_String>::iterator resource_it;
	for(resource_it = resource_names.begin(); resource_it != resource_names.end(); ++resource_it)
	{
		CL_String resource_id = (*resource_it);
		CL_Resource resource = resources.get_resource(resource_id);

		resource_items.push_back(load_resource(gc, resource_id, resource, resources));
	}
}

ResourceItem *TexturePacker::load_resource(CL_GraphicContext &gc, CL_String &resource_id, CL_Resource &resource, CL_ResourceManager &resources)
{
	ResourceItem *item = 0;

	try
	{
		CL_String type = resource.get_type();
		if(type == "sprite")
		{
			item = load_sprite(gc, resource_id, resource, resources);
		}
		else if(type == "image")
		{
			item = load_image(gc, resource_id, resource, resources);
		}
		else
		{
			throw CL_Exception(cl_format("Resourcetype %1 is not supported", type));
		}
	}
	catch(CL_Exception ex)
	{
		item = new NotSupportedResourceItem(resource, ex.message);
	}

	// Find resource path by traversing parents
	CL_DomElement &dom_element = resource.get_element();
	CL_DomNode parent = dom_element.get_parent_node();
	while (!parent.is_null())
	{
		CL_DomElement parent_element = parent.to_element();
		CL_String parent_name = parent_element.get_attribute("name");
		if(parent_name.length() > 0)
			item->resource_path = cl_format("%1/%2", parent_name, item->resource_path);
		parent = parent.get_parent_node();
	}

	return item;
}

ResourceItem *TexturePacker::load_sprite(CL_GraphicContext &gc, CL_String &resource_id, CL_Resource &resource, CL_ResourceManager &resources)
{
	CL_SpriteDescription desc(gc, resource_id, &resources);

	CL_Sprite sprite(gc, resource_id, &resources);
	sprite.set_play_loop(true);
	sprite.set_alignment(origin_top_left);

	SpriteResourceItem *item = new SpriteResourceItem(resource);
	item->sprite_description = desc;
	item->sprite = sprite;

	return item;
}

ResourceItem *TexturePacker::load_image(CL_GraphicContext &gc, CL_String &resource_id, CL_Resource &resource, CL_ResourceManager &resources)
{
	CL_SpriteDescription desc(gc, resource_id, &resources);

	CL_Image image(gc, resource_id, &resources);
	image.set_alignment(origin_top_left);

	ImageResourceItem *item = new ImageResourceItem(resource);
	item->sprite_description = desc;
	item->image = image;

	return item;
}

CL_TextureGroup *TexturePacker::pack(CL_GraphicContext &gc, const CL_Size &texture_size, int border_size)
{
	CL_TextureGroup *group = new CL_TextureGroup(gc, texture_size);

	std::vector<ResourceItem *> &items = get_resource_items();
	std::vector<ResourceItem *>::size_type item_index, item_size;
	item_size = items.size();
	for(item_index = 0; item_index < item_size; ++item_index)
	{
		SpriteResourceItem *sprite_item = dynamic_cast<SpriteResourceItem *>(items[item_index]);
		if(sprite_item)
		{
			sprite_item->packed_sub_textures.clear();

			const std::vector<CL_SpriteDescriptionFrame> &frames = sprite_item->sprite_description.get_frames();
			std::vector<CL_SpriteDescriptionFrame>::size_type index, size;
			size = frames.size();
			for(index = 0; index < size; ++index)
			{
				CL_Rect frame_rect = frames[index].rect;

				CL_Subtexture sub_texture = group->add(gc, CL_Size(frame_rect.get_width() + border_size*2, frame_rect.get_height() + border_size*2));
				sprite_item->packed_sub_textures.push_back(sub_texture);

				CL_Texture texture = frames[index].texture;
				const CL_PixelBuffer &pb = texture.get_pixeldata();
				last_border_size = border_size;
				if (last_border_size < 0) last_border_size= 0;
				CL_PixelBuffer new_pb = CL_PixelBufferHelp::add_border(pb, border_size, pb.get_size());
				sub_texture.get_texture().set_subimage(sub_texture.get_geometry().get_top_left(), new_pb, new_pb.get_size());
			}
		}

		ImageResourceItem *image_item = dynamic_cast<ImageResourceItem *>(items[item_index]);
		if(image_item)
		{
			image_item->packed_sub_textures.clear();

			const std::vector<CL_SpriteDescriptionFrame> &frames = image_item->sprite_description.get_frames();
			std::vector<CL_SpriteDescriptionFrame>::size_type index, size;
			size = frames.size();
			for(index = 0; index < size; ++index)
			{
				CL_Rect frame_rect = frames[index].rect;

				CL_Subtexture sub_texture = group->add(gc, CL_Size(frame_rect.get_width() + border_size*2, frame_rect.get_height() + border_size*2));
				image_item->packed_sub_textures.push_back(sub_texture);

				CL_Texture texture = frames[index].texture;
				const CL_PixelBuffer &pb = texture.get_pixeldata();
				last_border_size = border_size;
				if (last_border_size < 0) last_border_size = 0;
				CL_PixelBuffer new_pb = CL_PixelBufferHelp::add_border(pb, border_size, pb.get_size());
				sub_texture.get_texture().set_subimage(sub_texture.get_geometry().get_top_left(), new_pb, new_pb.get_size());
			}
		}

		if(!func_pack_progress.is_null())
			func_pack_progress.invoke((int)item_index + 1, (int)item_size);
	}

	return group;
}

void TexturePacker::save_resources(const CL_String &filename)
{
	// Map containing generated texture filenames for packed CL_Textures
	std::map<CL_Texture, CL_String> generated_texture_filenames;
	int generated_texture_index = 0;

	CL_String images_pathname = CL_PathHelp::get_fullpath(filename);

	// Loop through all resource items
	std::vector<ResourceItem *> &items = get_resource_items();
	std::vector<ResourceItem *>::size_type item_index, item_size;
	item_size = items.size();
	for(item_index = 0; item_index < item_size; ++item_index)
	{
		SpriteResourceItem *sprite_item = dynamic_cast<SpriteResourceItem *>(items[item_index]);
		if (sprite_item)
			process_resource(sprite_item->resource, sprite_item->packed_sub_textures, generated_texture_filenames, generated_texture_index, images_pathname);

		ImageResourceItem *image_item = dynamic_cast<ImageResourceItem *>(items[item_index]);
		if (image_item)
			process_resource(image_item->resource, image_item->packed_sub_textures, generated_texture_filenames, generated_texture_index, images_pathname);
	}

	// Save the entire resource DOM
	resources.save(filename);
}

void TexturePacker::process_resource(CL_Resource &item_resource, std::vector<CL_Subtexture> &packed_sub_textures, std::map<CL_Texture, CL_String> &generated_texture_filenames, int &generated_texture_index, const CL_String &image_pathname )
{
	// Found a sprite resource, lets modify its content!
	CL_Resource resource = item_resource;

	// Iterate through all nodes, and remove all previous image tags
	CL_DomElement &element = resource.get_element();
	CL_DomNode cur = element.get_first_child();
	while (!cur.is_null())
	{
		CL_DomNode next = cur.get_next_sibling();
		CL_DomNode::NodeType nodeType = (CL_DomNode::NodeType)cur.get_node_type();

		// Only remove the <image> tag, as we want to keep the other sprite attributes
		if (cur.get_node_name() == "image")
			element.remove_child(cur);

		cur = next;
	}

	// Add new image tag to resource DOM
	std::vector<CL_Subtexture>::size_type index, size;
	size = packed_sub_textures.size();
	for(index = 0; index < size; ++index)
	{
		CL_Subtexture subtexture = packed_sub_textures[index];

		// Try to find out if we already have created a texture-on-disk for this subtexture
		CL_String texture_filename;
		CL_Texture texture = subtexture.get_texture();
		std::map<CL_Texture, CL_String>::iterator it;
		it = generated_texture_filenames.find(texture);
		if(it == generated_texture_filenames.end())
		{
			// Texture not found, generate a filename and dump texture to disk
			texture_filename = cl_format("texture%1.png", ++generated_texture_index);
			CL_PNGProvider::save(texture.get_pixeldata(), image_pathname + texture_filename);
			generated_texture_filenames[texture] = texture_filename;
		}
		else
		{
			// Previously dumped textures, lets reuse the filename
			texture_filename = (*it).second;
		}

		// Add <grid> DOM element
		CL_DomElement new_grid_element = element.get_owner_document().create_element("grid");
		new_grid_element.set_attribute("pos", cl_format("%1,%2", subtexture.get_geometry().left + last_border_size, subtexture.get_geometry().top + last_border_size));
		new_grid_element.set_attribute("size", cl_format("%1,%2", subtexture.get_geometry().get_width()- last_border_size*2, subtexture.get_geometry().get_height()- last_border_size*2));

		// Add <image> DOM element
		CL_DomElement new_image_element = element.get_owner_document().create_element("image");
		new_image_element.set_attribute("file", texture_filename);
		new_image_element.append_child(new_grid_element);

		// Add <image> element under <sprite> element
		element.append_child(new_image_element);
	}
}

