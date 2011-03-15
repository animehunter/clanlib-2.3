
#pragma once

class GamePlayerCollection;

#include "game_map.h"

class GameModel
{
public:
	GameModel();
	~GameModel();

	GamePlayerCollection *get_players() { return players; }

	ClientMap *get_map() { return &map; }

private:
	std::auto_ptr<GamePlayerCollection> players;

	ClientMap map;
};
