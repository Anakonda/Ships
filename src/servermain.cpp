#include "net.h"
#include "utils.h"
#include "objects/ship.h"
#include "objects/star.h"
#include "objects/planet.h"

#include "timer.h"
#include "client.h"

#include <sstream>
#include <map>
#include <signal.h>

#define CALC_INTERVAL 10

bool closing;

std::map<unsigned short, Client> clients;
std::map<unsigned short, Object*> objects;

void signalHandler(int signal)
{
	closing = true;
}

std::string objectToString(Object *object)
{
	Net::Packet packet;
	Object::Type type = object->getType();
	packet.writeShort(object->getID());
	packet.writeChar((char)type);
	packet.writePoint(object->getPosition());
	packet.writePoint(object->getHeading());
	packet.writePoint(object->getUp());
	packet.writePoint(object->getVelocity());
	switch(type)
	{
		case Object::Type::Star:
		{
			Star *star = (Star*)object;
			packet.writePoint(star->getColor());
			packet.writeFloat(star->getScale());
			packet.writeInt(star->getLightID());
			break;
		}
		case Object::Type::Planet:
		{
			Planet *planet = (Planet*)object;
			packet.writeFloat(planet->getScale());
			packet.writeString(planet->getTexture());
			break;
		}
	}
	return packet.getData();
}

void CreateUniverse(void)
{
	unsigned short id = utils::firstUnusedKey(objects);
	Object *star = new Star(id, Point3(200, 0, 0), Point3(0, 0, 1), Point3(0, 1, 0), Point3(1, 0.909, 0.15), 50, 0);
	objects.insert(std::pair<unsigned short, Object*>(id, star));

	id = utils::firstUnusedKey(objects);
	star = new Star(id, Point3(-200, 0, 0), Point3(0, 0, 1), Point3(0, 1, 0), Point3(0.9, 0.360, 0.15), 40, 1);
	objects.insert(std::pair<unsigned short, Object*>(id, star));

	id = utils::firstUnusedKey(objects);
	Object *planet = new Planet(id, Point3(-100, 7, 0), Point3(0, 0, 1), Point3(0, 1, 0), 10, "res/planeetta.tga");
	objects.insert(std::pair<unsigned short, Object*>(id, planet));
}

void handlePacket(ENetEvent event)
{
	Net::Packet packet(std::string((char*)event.packet->data, event.packet->dataLength));
	switch(packet.readChar())
	{
		case (char)Net::Header::ObjectData:
		{
			unsigned short id = packet.readShort();
			Object::Type type = (Object::Type)packet.readChar();
			if(clients.find((size_t)event.peer->data) != clients.end())
			{
				Ship *ship = clients.find((size_t)event.peer->data)->second.ship;
				if(ship != nullptr && id == ship->getID())
				{
					ship->setPosition(packet.readPoint());
					ship->setHeading(packet.readPoint());
					ship->setUp(packet.readPoint());
					ship->setVelocity(packet.readPoint());
					ship->needsSending = true;
				}
			}
			break;
		}
		case (char)Net::Header::HP:
		{
			unsigned short ID = packet.readShort();
			short hp = packet.readShort();
			auto client = clients.find((size_t)event.peer->data);
			if(client != clients.end() && client->second.ship != nullptr)
			{
				Ship *ship = client->second.ship;
				if(ID == ship->getID())
				{
					if(ship->getHP() >= hp)
					{
						ship->setHP(hp);
						if(hp <= 0)
						{
							delete ship;
							objects.erase(ID);
							client->second.ship = nullptr;

							Net::Packet packet;
							packet.writeChar((char)Net::Header::RemoveObject);
							packet.writeShort(ID);
							Net::Send(packet);
						}
					}
				}
			}
			break;
		}
	}
}

int main(int argc, char* argv[])
{
	closing = false;
	signal(SIGINT, signalHandler);

	unsigned int port = 8555;
	if(argc > 1)
	{
		std::istringstream stream(argv[1]);
		stream >> port;
	}

	al_init();

	CreateUniverse();
	Net::Listen(port);

	Timer timer(10);
	while(!closing)
	{
		timer.wait();
		ENetEvent event;
		do
		{
			event = Net::getEvent();
			switch(event.type)
			{
				case ENET_EVENT_TYPE_CONNECT:
				{
					unsigned short shipID = utils::firstUnusedKey(objects);
					Object *ship = new Ship(shipID, Point3(0, 0, 0), Point3(0, 0, 1), Point3(0, 1, 0));
					unsigned short clientID = utils::firstUnusedKey<Client>(clients);
					Client client(event.peer, dynamic_cast<Ship*>(ship), clientID);
					clients.insert(std::pair<unsigned short, Client>(clientID, client));
					objects.insert(std::pair<unsigned short, Object*>(shipID, ship));
					event.peer->data = (void*)((size_t)clientID);
					std::cout<<"New connection"<< std::endl;
					Net::Packet packet;
					packet.writeChar((char)Net::Header::ShipID);
					packet.writeShort(shipID);
					Net::Send(packet, event.peer);
					std::string data;
					data.push_back((char)Net::Header::CreateObject);
					for(auto &object : objects)
					{
						data = data + objectToString(object.second);
					}
					Net::Packet objectData(data);
					Net::Send(objectData, event.peer);

					data = "";
					data.push_back((char)Net::Header::CreateObject);
					data = data + objectToString(ship);
					Net::Send(Net::Packet(data));
					break;
				}
				case ENET_EVENT_TYPE_RECEIVE:
					handlePacket(event);
					enet_packet_destroy(event.packet);
					break;
				case ENET_EVENT_TYPE_DISCONNECT:
				{
					std::cout<<"Disconnect"<<std::endl;
					unsigned long long clientID = reinterpret_cast<long long>(event.peer->data);
					if(clients.find(clientID)->second.ship != nullptr)
					{
						unsigned short shipID = clients.find(clientID)->second.ship->getID();
						delete objects.find(shipID)->second;
						objects.erase(shipID);

						Net::Packet packet;
						packet.writeChar((char)Net::Header::RemoveObject);
						packet.writeShort(shipID);

						Net::Send(packet);
					}
					clients.erase(clientID);
					break;
				}
				case ENET_EVENT_TYPE_NONE:
				{
					break;
				}
			}
		}
		while(event.type != ENET_EVENT_TYPE_NONE);

		for(auto &object : objects)
		{
			object.second->simulateFrame(CALC_INTERVAL);
		}

		for(auto &client : clients)
		{
			Ship *ship = client.second.ship;
			if(ship != nullptr)
			{
				for(auto &object : objects)
				{
					if(ship->testCollision(object.second))
					{
						break;
					}
				}
				if(ship->getHP() <= 0)
				{
					//Do something
				}
			}

		}

		char temp = static_cast<char>(Net::Header::ObjectData);
		std::string data(1, temp);
		for(auto &object : objects)
		{
			if(!object.second->needsSending)
			{
				continue;
			}
			data = data + objectToString(object.second);

			object.second->needsSending = false;
		}
		if(data.length() > 1)
		{
			Net::Send(Net::Packet(data));
		}
	}

	Net::Disconnect();
	//clean
	for(auto &object : objects)
	{
		delete object.second;
	}
	return 0;
}
