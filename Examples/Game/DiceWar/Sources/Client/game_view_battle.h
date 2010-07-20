
#pragma once

class GameView;
class GameModel;
class GamePlayer;
class Client;

class GameViewBattle : public CL_GUIComponent
{
public:
	GameViewBattle(GameView *view, Client *client);
	~GameViewBattle();

	void attacked_area(int map_area_from_id, int map_area_to_id);
	void attack_dice_result(const std::vector<int> &dice_results);
	void defense_dice_result(const std::vector<int> &dice_results);

private:
	void close();

	void on_render(CL_GraphicContext &gc, const CL_Rect &clip_rect);
	void on_message(CL_GUIMessage &message);
	void on_timer_hide_expired();

private:
	Client *client;
	GameView *view;
	
	CL_Timer timer_hide;

	CL_Sprite sprite_dices;

	CL_Font font_large;
	CL_Font font_small;

	std::vector<int> attack_results;
	std::vector<int> defense_results;
	int current_dice_index;

	GamePlayer *player_from;
	GamePlayer *player_to;
};
