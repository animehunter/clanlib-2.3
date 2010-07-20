
#include "precomp.h"
#include "server_player.h"

ServerPlayer::ServerPlayer(CL_NetGameConnection *connection)
: connection(connection), lobby_player(0), game_player(0), id(0)
{
	if(connection)
		connection->set_data("serverplayer", this);
}

ServerPlayer *ServerPlayer::get_player(CL_NetGameConnection *connection)
{
	if(connection)
		return reinterpret_cast<ServerPlayer *>(connection->get_data("serverplayer"));
	else
		return 0;
}

void ServerPlayer::send_event(const CL_NetGameEvent &game_event)
{
	if(connection)
		connection->send_event(game_event);
}
