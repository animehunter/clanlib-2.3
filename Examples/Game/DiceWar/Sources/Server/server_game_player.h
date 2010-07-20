
#pragma once

class ServerGame;
class ServerPlayer;
class CL_NetGameEvent;

class ServerGamePlayer
{
public:
	ServerGamePlayer(ServerPlayer *player);

	virtual void send_event(const CL_NetGameEvent &game_event);

public:
	ServerGame *current_game;

	ServerPlayer *player;

	int visual_id;

	bool in_battle_mode;
};
