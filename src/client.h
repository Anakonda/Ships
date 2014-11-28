#ifndef CLIENT_H
#define CLIENT_H

#include "objects/ship.h"

#include <enet/enet.h>

struct Client
{
	ENetPeer *peer;
	Ship *ship;
	unsigned short clientID;

	Client(ENetPeer *peer, Ship *ship, unsigned short clientID);
	~Client();
};




#endif // CLIENT_H
