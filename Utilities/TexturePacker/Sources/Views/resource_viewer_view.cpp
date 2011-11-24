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
#include "resource_viewer_view.h"
#include "../sprite_component.h"
#include "../texture_packer.h"
#include "../Dialogs/create_collision_data_dialog.h"

ResourceViewerView::ResourceViewerView(CL_GUIComponent *parent, MainWindow *mainwindow)
: View(parent, mainwindow, "Resource viewer")
{
	func_resized().set(this, &ResourceViewerView::on_resized);

	label_selected_resource_file = new CL_Label(this);
	label_selected_resource_file->set_text("Resource file:");

	lineedit_selected_resource_file = new CL_LineEdit(this);

	button_browse_resource_file = new CL_PushButton(this);
	button_browse_resource_file->set_text("...");
	button_browse_resource_file->func_clicked().set(this, &ResourceViewerView::on_button_browse_resource_file);

	button_edit_resource = new CL_PushButton(this);
	button_edit_resource->set_text("Edit resource");
	button_edit_resource->func_clicked().set(this, &ResourceViewerView::on_button_edit_resource);
	button_edit_resource->set_enabled(false);

	button_create_collision_data = new CL_PushButton(this);
	button_create_collision_data->set_text("Create collision data");
	button_create_collision_data->func_clicked().set(this, &ResourceViewerView::on_button_create_collision_data);
	button_create_collision_data->set_enabled(false);

	resource_list = new CL_ListView(this);
	CL_ListViewColumnHeader col_resource = resource_list->get_header()->append(resource_list->get_header()->create_column("Resource", "Resource"));
	col_resource.set_width(350);
	CL_ListViewColumnHeader col_message = resource_list->get_header()->append(resource_list->get_header()->create_column("Message", "Message"));

	CL_ListViewIconList icon_list = resource_list->get_icon_list();
	CL_ListViewIcon icon_disabled;
	icon_disabled.set_sprite(CL_Sprite(get_gc(), "Resources/sweetie-16-em-cross.png"), listview_mode_details);
	CL_ListViewIcon icon_enabled;
	icon_enabled.set_sprite(CL_Sprite(get_gc(), "Resources/sweetie-16-em-check.png"), listview_mode_details);
	icon_list.set_icon(1, icon_disabled);
	icon_list.set_icon(2, icon_enabled);

	resource_list->func_selection_changed().set(this, &ResourceViewerView::on_selection_changed);

	sprite_component = new SpriteComponent(this);

	on_resized();
}

ResourceViewerView::~ResourceViewerView()
{
}

void ResourceViewerView::on_resized()
{
	CL_Size size = get_size();

	label_selected_resource_file->set_geometry(CL_Rect(10, 13, 80, 30));
	lineedit_selected_resource_file->set_geometry(CL_Rect(80, 10, size.width - 20 - 20, 30));
	button_browse_resource_file->set_geometry(CL_Rect(10 + size.width - 20 - 30, 10, size.width - 10, 30));
	resource_list->set_geometry(CL_Rect(10, 40, size.width - 10, 200));
	sprite_component->set_geometry(CL_Rect(10, 205, size.width - 10, size.height - 35));
	button_edit_resource->set_geometry(CL_Rect(10, size.height - 30, 130, size.height - 10));
	button_create_collision_data->set_geometry(CL_Rect(140, size.height - 30, 260, size.height - 10));
}

void ResourceViewerView::on_button_browse_resource_file()
{
	CL_OpenFileDialog dlg(this);
	dlg.add_filter("Resource files", "*.xml", true);
	dlg.add_filter("All files", "*.*");

	dlg.set_initial_directory(CL_System::get_exe_path());

	if(dlg.show())
	{
		CL_String filename = dlg.get_filename();

		try
		{
			load_resource_file(filename);
			lineedit_selected_resource_file->set_text(filename);
		}
		catch(CL_Exception error)
		{
//			generation_result->set_text(cl_format("Error: %1", error.message));
		}
	}
}

void ResourceViewerView::on_button_edit_resource()
{
}

ResourceItem *ResourceViewerView::get_selected_resource_item()
{
	std::vector<ResourceItem *> &items = packer.get_resource_items();
	int index = resource_list->get_selected_item().get_id();
	ResourceItem *resource_item = items[index];

	return resource_item;
}

void ResourceViewerView::on_button_create_collision_data()
{
	ResourceItem *resource_item = get_selected_resource_item();
	if(resource_item)
	{
		SpriteResourceItem *sprite_item = dynamic_cast<SpriteResourceItem *>(resource_item);
		if(sprite_item)
		{
			CreateCollisionDataDialog dlg(this, sprite_item);
			dlg.exec();
		}
	}
}

void ResourceViewerView::on_selection_changed(CL_ListViewSelection selection)
{
	ResourceItem *resource_item = get_selected_resource_item();
	if(resource_item)
		show_resource(resource_item);
}

void ResourceViewerView::show_resource(ResourceItem *resource_item)
{
	SpriteResourceItem *sprite_item = dynamic_cast<SpriteResourceItem *>(resource_item);
	ImageResourceItem *image_item = dynamic_cast<ImageResourceItem *>(resource_item);
	if(sprite_item)
	{
		sprite_component->set_sprite(&sprite_item->sprite);
//		button_edit_resource->set_enabled(true);
		button_create_collision_data->set_enabled(true);
	}
	else if(image_item)
	{
		sprite_component->set_sprite_description(&image_item->sprite_description);
		//button_edit_resource->set_enabled(true);
		button_create_collision_data->set_enabled(false);
	}
	else
	{
		sprite_component->clear_sprite();
//		button_edit_resource->set_enabled(false);
		button_create_collision_data->set_enabled(false);
	}
}

void ResourceViewerView::load_resource_file(const CL_String &file)
{
	resource_list->clear();

	packer.load_resources(get_gc(), file);

	std::vector<ResourceItem *> &items = packer.get_resource_items();
	for(size_t i = 0; i < items.size(); ++i)
	{
		ResourceItem *resource_item = items[i];

		CL_ListViewItem item = resource_list->create_item();
		item.set_column_text("Resource", cl_format("%1%2", resource_item->resource_path, resource_item->resource.get_name()));

		item.set_id(i);

		NotSupportedResourceItem *not_supported_item = dynamic_cast<NotSupportedResourceItem *>(resource_item);
		if(not_supported_item)
		{
			item.set_icon(1);
			item.set_column_text("Message", not_supported_item->error);
		}
		else
		{
			item.set_icon(2);
		}

		resource_list->get_document_item().append_child(item);
	}
}
