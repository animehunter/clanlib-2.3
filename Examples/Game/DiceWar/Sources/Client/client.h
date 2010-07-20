
#pragma once

class CL_NetGameEvent;
class GameEvents;
class GameView;
class GameModel;
class LobbyView;
class LobbyPlayer;
class LobbyGame;
class LobbyEvents;
class LobbyModel;
class LoginEvents;
class ClientGUI;

class Client
{
public:
	enum State
	{
		not_logged_in,
		in_lobby,
		playing
	};

public:
	Client();
	~Client();

	void exec();

	State get_state() const { return state; }
	void set_state(const State &value) { state = value; }

	void set_player_id(int id) { player_id = id; }
	int get_player_id() const { return player_id; }

	CL_GUIManager *get_gui();

	LobbyView *get_lobby_view() { return lobby_view; }
	LobbyModel *get_lobby_model() { return lobby_model; }

	GameView *get_game_view() { return game_view; }
	GameModel *get_game_model() { return game_model; }

	CL_NetGameClient *get_network_client() { return &network_client; }

	void send_event(const CL_NetGameEvent &game_event) { network_client.send_event(game_event); }

	void request_create_game();
	void request_join_game(int game_id);
	void request_start_game();
	void request_leave_game();
	
	void request_attack_area(int map_area_from_id, int map_area_to_id);
	void request_end_turn();

	void load_map(const CL_String &mapname);
	void create_game();

private:
	void on_connected();
	void on_disconnected();

	void on_event_received(const CL_NetGameEvent &e);

	State state;

	CL_NetGameClient network_client;

	CL_String player_nick;
	int player_id;

	GameView *game_view;
	LobbyView *lobby_view;

	CL_AutoPtr<ClientGUI> gui;
	CL_AutoPtr<GameModel> game_model;
	CL_AutoPtr<GameEvents> game_events;
	CL_AutoPtr<LobbyModel> lobby_model;
	CL_AutoPtr<LobbyEvents> lobby_events;
	CL_AutoPtr<LoginEvents> login_events;

	CL_SlotContainer slots;
};
