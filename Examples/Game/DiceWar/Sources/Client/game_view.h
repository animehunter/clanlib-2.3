
#pragma once

#include "game_map_selection.h"

class Client;
class MapArea;
class GameTerrain;
class GamePlayer;
class ModelAssimp;
class LightModel;
class GameViewPlayerList;
class GameViewBattle;
class GameViewEnd;
class GameViewChat;
class GameViewActionBar;

class GameView : public CL_GUIComponent
{
public:
	GameView(Client *client);
	~GameView();

	void load_map();
	void player_joined_game(GamePlayer *player);
	void player_left_game(GamePlayer *player);
	void start_game();

	void update_map();

	void set_active_turn();
	void set_waiting_turn();

	void add_player_message(GamePlayer *player, const CL_String &message);
	void add_system_message(const CL_String &message);

	void invalid_attack();
	void attacked_area(int map_area_from_id, int map_area_to_id);
	void attack_dice_result(const std::vector<int> &dice_results);
	void defense_dice_result(const std::vector<int> &dice_results);
	void game_over(const CL_String &title, const CL_String &message);

	void battle_over();

	void end_turn();

	Client *get_client() { return client; }

private:
	bool is_active_turn;
	bool is_in_battle;

	void on_render(CL_GraphicContext &gc, const CL_Rect &clip_rect);
	void on_message(CL_GUIMessage &message);

	void on_message_pointer_moved();
	void on_message_mouse_left_pressed();

	void render_map(CL_GraphicContext &gc);
	void render_armies(CL_GraphicContext &gc, const LightModel &light_model);
	void render_army(CL_GraphicContext &gc, const LightModel &light_model, MapArea *area);

	CL_Mat4f get_projection();
	CL_Mat4f get_modelview();

	int get_area_id_at_mousepos(CL_GraphicContext &gc, CL_Mat4f &projection, CL_Mat4f &modelview);

	void set_battle_state(bool in_battle);
	void set_playing_state(bool active_turn);

	void update_actionbar();

	Client *client;

	std::auto_ptr<GameTerrain> terrain;

	std::auto_ptr<ModelAssimp> army_model;

	std::auto_ptr<MapSelection> map_selection;

	CL_Sprite sprite_waiting_for_turn;
	CL_Font font_large;

	std::auto_ptr<GameViewPlayerList> view_playerlist;
	std::auto_ptr<GameViewBattle> view_battle;
	std::auto_ptr<GameViewEnd> view_gameend;
	std::auto_ptr<GameViewChat> view_chat;
	std::auto_ptr<GameViewActionBar> view_actionbar;

	float view_angle_x;
	float view_angle_y;
	float view_angle_z;
	float view_distance;
	bool camera_drag_in_progress;
	CL_Point camera_drag_last_mouse_pos;

	static CL_GUITopLevelDescription get_toplevel_description();
};
