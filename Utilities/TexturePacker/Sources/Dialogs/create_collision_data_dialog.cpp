
#include "precomp.h"
#include "create_collision_data_dialog.h"
#include "../texture_packer.h"

CreateCollisionDataDialog::CreateCollisionDataDialog(CL_GUIComponent *owner, SpriteResourceItem *sprite_item)
: CL_Window(owner, get_description()), sprite_item(sprite_item)
{
	set_layout(layout);

	create_components("Resources/create_collision_data.gui");

	edit_directory = CL_LineEdit::get_named_item(this, "editDirectory");
	button_browse = CL_PushButton::get_named_item(this, "buttonBrowse");
	button_generate =  CL_PushButton::get_named_item(this, "buttonGenerate");
	button_close =  CL_PushButton::get_named_item(this, "buttonClose");
	radio_high = CL_RadioButton::get_named_item(this, "radiobuttonHigh");
	radio_medium = CL_RadioButton::get_named_item(this, "radiobuttonMedium");
	radio_low = CL_RadioButton::get_named_item(this, "radiobuttonLow");
	radio_poor = CL_RadioButton::get_named_item(this, "radiobuttonPoor");
	radio_raw = CL_RadioButton::get_named_item(this, "radiobuttonRaw");

	edit_directory->set_focus();

	radio_medium->set_selected(true);

	button_browse->func_clicked().set(this, &CreateCollisionDataDialog::on_browse);
	button_generate->func_clicked().set(this, &CreateCollisionDataDialog::on_generate);
	button_generate->set_default(true);
	button_close->func_clicked().set(this, &CreateCollisionDataDialog::on_close);
	button_close->set_cancel(true);
	
	func_close().set(this, &CreateCollisionDataDialog::on_window_close);

	set_visible(true, true);
}

CL_GUITopLevelDescription CreateCollisionDataDialog::get_description()
{
	CL_GUITopLevelDescription desc;
	desc.set_title("Create Collision Data");
	desc.set_allow_resize(false);
	desc.set_visible(false);
	desc.show_minimize_button(false);
	desc.show_maximize_button(false);
	desc.show_sysmenu(false);
	return desc;
}

bool CreateCollisionDataDialog::on_window_close()
{
	exit_with_code(0);
	return true;
}

void CreateCollisionDataDialog::on_browse()
{
	CL_BrowseFolderDialog dlg(this);

	if(dlg.show())
		edit_directory->set_text(dlg.get_selected_path());
}

void CreateCollisionDataDialog::on_generate()
{
	CL_String directory = CL_StringHelp::trim(edit_directory->get_text());
	if(directory.length() == 0)
	{
		cl_message_box(this, "Missing output directory", "You need to specify an output directory.", cl_mb_buttons_ok, cl_mb_icon_error);
		return;
	}

	CL_OutlineAccuracy accuracy;
	if(radio_high->is_selected())
		accuracy = accuracy_high;
	else if(radio_low->is_selected())
		accuracy = accuracy_low;
	else if(radio_medium->is_selected())
		accuracy = accuracy_medium;
	else if(radio_poor->is_selected())
		accuracy = accuracy_poor;
	else if(radio_raw->is_selected())
		accuracy = accuracy_raw;

	generate_collision(directory, accuracy);

	exit_with_code(1);
}

void CreateCollisionDataDialog::on_close()
{
	exit_with_code(0);
}

void CreateCollisionDataDialog::generate_collision(const CL_String &directory, CL_OutlineAccuracy accuracy)
{
	const std::vector<CL_SpriteDescriptionFrame> &frames = sprite_item->sprite_description.get_frames();

	CL_String filename = sprite_item->resource.get_name();

	for(size_t i = 0; i < frames.size(); ++i)
	{
		CL_PixelBuffer pb;

		if(frames[i].type == CL_SpriteDescriptionFrame::type_pixelbuffer)
			pb = frames[i].pixelbuffer;
		else if(frames[i].type == CL_SpriteDescriptionFrame::type_texture)
			pb = frames[i].texture.get_pixeldata();

		if(frames[i].rect != pb.get_size())
			pb = pb.copy(frames[i].rect);
		
		CL_CollisionOutline generated(pb, 128, accuracy);

		CL_StringFormat f("%1\\%2-%3.col"); 
		f.set_arg(1, directory); 
		f.set_arg(2, filename);
		f.set_arg(3, i, 3);
		f.get_result();

		generated.save(f.get_result());
	}

	CL_String msg = cl_format("%1 collision outlines generated as %3\\%2-xxx.col", frames.size(), filename, directory);

	cl_message_box(this, "Collision outlines generated", msg, cl_mb_buttons_ok, cl_mb_icon_info);
}
