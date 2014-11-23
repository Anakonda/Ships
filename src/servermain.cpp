#include "net.h"
#include "utils.h"
#include "objects/ship.h"
#include "objects/star.h"
#include "objects/planet.h"

#include "timer.h"

#include <sstream>
#include <map>
#include <signal.h>

bool closing;

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

std::map<unsigned short, Object*> objects;

void CreateUniverse(void)
{
	unsigned short id = utils::firstUnusedKey(objects);
	Object *star = new Star(id, Point3(200, 0, 0), Point3(0, 0, 1), Point3(0, 1, 0), Point3(1, 0.909, 0.25), 50, 0);
	objects.insert(std::pair<unsigned short, Object*>(id, star));

	id = utils::firstUnusedKey(objects);
	star = new Star(id, Point3(-200, 0, 0), Point3(0, 0, 1), Point3(0, 1, 0), Point3(0.9, 0.360, 0.25), 40, 1);
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
		case (int)Net::Header::ObjectData:
		{
			unsigned short id = packet.readShort();
			Object::Type type = (Object::Type)packet.readChar();
			if(id == (size_t)event.peer->data && type == Object::Type::Ship)
			{
				Object *ship = objects.find(id)->second;
				ship->setPosition(packet.readPoint());
				ship->setHeading(packet.readPoint());
				ship->setUp(packet.readPoint());
				ship->needsSending = true;
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
					unsigned short id = utils::firstUnusedKey(objects);
					Object *ship = new Ship(id, Point3(0, 0, 0), Point3(0, 0, 1), Point3(0, 1, 0));
					objects.insert(std::pair<unsigned short, Object*>(id, ship));
					event.peer->data = (void*)((long long)id);
					std::cout<<"New connection"<< std::endl;
					Net::Packet packet;
					packet.writeChar((char)Net::Header::ShipID);
					packet.writeShort(id);
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
					unsigned long long id = reinterpret_cast<long long>(event.peer->data);
					delete objects.find(id)->second;
					objects.erase(id);

					Net::Packet packet;
					packet.writeChar((char)Net::Header::RemoveObject);
					packet.writeShort(id);

					Net::Send(packet);

					break;
				}
				case ENET_EVENT_TYPE_NONE:
					break;
			}
		}
		while(event.type != ENET_EVENT_TYPE_NONE);

		for(auto &object : objects)
		{
			object.second->simulateFrame();
		}

		Net::Packet objectData;
		objectData.writeChar((char)Net::Header::ObjectData);
		for(auto &object : objects)
		{
			if(!object.second->needsSending)
			{
				continue;
			}
			objectData.writeShort(object.second->getID());
			Object::Type type = object.second->getType();
			objectData.writeChar((char)type);

			objectData.writePoint(object.second->getPosition());
			objectData.writePoint(object.second->getHeading());
			objectData.writePoint(object.second->getUp());

			if(type == Object::Type::Ship)
			{
				objectData.writeFloat(((Ship*)object.second)->getSpeed());
			}
			else
			{
				objectData.writePoint(object.second->getVelocity());
			}
			object.second->needsSending = false;
		}
		Net::Send(objectData);
	}

	Net::Disconnect();
	//clean
	for(auto &object : objects)
	{
		delete object.second;
	}
	return 0;
}
