#include "net.h"

ENetHost *connection;

bool Net::Connect(std::string address, unsigned int port)
{
	enet_initialize();

	connection = enet_host_create(nullptr, 1, 1, 0, 0);

	if(connection == nullptr)
	{
		std::cout<<"fail"<<std::endl;
	}

	ENetAddress enetAddress;
	enet_address_set_host(&enetAddress, address.c_str());
	enetAddress.port = port;

	ENetPeer *peer = enet_host_connect (connection, &enetAddress, 1, 0);
	ENetEvent event;

	if (enet_host_service(connection, &event, 5000) > 0 && event.type == ENET_EVENT_TYPE_CONNECT)
	{
		return true;
	}
	return false;
}

void Net::Listen(unsigned int port)
{
	enet_initialize();
	ENetAddress address;
	address.host = ENET_HOST_ANY;
	address.port = port;
	connection = enet_host_create(&address, 32, 1, 0, 0);
	std::cout<<"Listening in port " << port << std::endl;
}

int Net::getPing(void)
{
	if(connection->peerCount != 1)
	{
		return 0;
	}
	return connection->peers[0].roundTripTime;

}

void Net::Disconnect(void)
{
	for(size_t i = 0; i < connection->peerCount; i++)
	{
		enet_peer_disconnect(&connection->peers[i], 0);
	}
	enet_host_destroy(connection);
	enet_deinitialize();
}

void Net::Send(Packet packet)
{
	ENetPacket *enetPacket = enet_packet_create(packet.getData().c_str(), packet.getData().size(), ENET_PACKET_FLAG_RELIABLE);

	enet_host_broadcast(connection, 0, enetPacket);
}

void Net::Send(Packet packet, ENetPeer *peer)
{
	ENetPacket *enetPacket = enet_packet_create(packet.getData().c_str(), packet.getData().size(), ENET_PACKET_FLAG_RELIABLE);

	enet_peer_send(peer, 0, enetPacket);
}

ENetEvent Net::getEvent(void)
{
	ENetEvent event;
	enet_host_service(connection, &event, 0);
	return event;
}
