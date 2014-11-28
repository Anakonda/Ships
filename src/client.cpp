#include "client.h"

Client::Client(ENetPeer *peer, Ship *ship, unsigned short clientID) : peer(peer), ship(ship), clientID(clientID)
{

}

Client::~Client()
{

}
