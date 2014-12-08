#include <thread>
#include <sstream>

#include "render.h"
#include "input.h"
#include "objects/ship.h"
#include "objects/star.h"
#include "objects/planet.h"

#include "weapons/laser.h"

#include "projectiles/laser.h"

#include "net.h"
#include "timer.h"

#define CALC_INTERVAL 10

bool closing;

Renderer renderer;

struct
{
	unsigned short ID;
	bool stuck;
} shipData;

std::map<unsigned short, Object*> objects;
std::map<unsigned short, Projectile*> projectiles;

bool screenNeedsResizing = false;

void calculate(void)
{
	Timer timer(CALC_INTERVAL);
	bool sendInfo = true;
	while(!closing)
	{
		timer.wait();
		if(controllerStatus.connected && shipData.ID != (unsigned short)-1 && objects.find(shipData.ID) != objects.end())
		{
			Ship *ship = (Ship*)objects.find(shipData.ID)->second;

			ship->accelerate(controllerStatus.ThumbLY/32767.0 * 0.001 * CALC_INTERVAL);
			if(controllerStatus.ThumbLY == 0)
			{
				ship->slow();
			}

			if(!shipData.stuck)
			{
				ship->RotateRelative(controllerStatus.ThumbLX/32767.0 * 0.0025 * CALC_INTERVAL,
									controllerStatus.ThumbRY/32767.0 * -0.0025 * CALC_INTERVAL,
									controllerStatus.ThumbRX/32767.0 * -0.0025 * CALC_INTERVAL);
			}
			if(controllerStatus.A)
			{
				Net::Packet packet;
				packet.writeChar((char)Net::Header::Shoot);
				packet.writeShort(shipData.ID);

				Net::Send(packet);
			}

			if(sendInfo)
			{
				Net::Packet packet;
				packet.writeChar((char)Net::Header::ObjectData);
				packet.writeShort(shipData.ID);
				packet.writeChar((char)Object::Type::Ship);

				packet.writePoint(ship->getPosition());
				packet.writePoint(ship->getHeading());
				packet.writePoint(ship->getUp());
				packet.writePoint(ship->getVelocity());

				Net::Send(packet);

				sendInfo = false;
			}
			else
			{
				sendInfo = true;
			}
		}
		for(auto &object : objects)
		{
			object.second->simulateFrame(CALC_INTERVAL);
		}
		for(auto &projectile : projectiles)
		{
			projectile.second->simulateFrame(CALC_INTERVAL);
		}
		bool colliding = false;
		if(shipData.ID != (unsigned short)-1 && objects.find(shipData.ID) != objects.end())
		{
			Object *ship = objects.find(shipData.ID)->second;
			for(auto &object : objects)
			{
				if(objects.find(shipData.ID)->second->testCollision(object.second))
				{
					colliding = true;
				}
			}
			if(colliding)
			{
				//tell server
				Net::Packet packet;
				packet.writeChar((char)Net::Header::HP);
				packet.writeShort(shipData.ID);
				packet.writeShort(((Ship*)ship)->getHP());
				Net::Send(packet);
			}
		}
		shipData.stuck = colliding;
		if(shipData.stuck)
		{
			((Ship*)objects.find(shipData.ID)->second)->setSpeed(0);
		}
	}
}

void handlePacket(Net::Packet packet)
{
	switch(packet.readChar())
	{
		case (char)Net::Header::ObjectData:
		{
            while(packet.good())
			{
				unsigned short id = packet.readShort();
				auto it = objects.find(id);
				if(it != objects.end())
				{
					char type = packet.readChar();
					Point3 pos = packet.readPoint();
					Point3 heading = packet.readPoint();
					Point3 up = packet.readPoint();
					Point3 velocity = packet.readPoint();

					switch(type)
					{
						case (char)Object::Type::Star:
						{
							packet.readPoint();
							packet.readFloat();
							packet.readInt();
							break;
						}
						case (char)Object::Type::Planet:
						{
							packet.readFloat();
							packet.readString();
							break;
						}
						case (char)Object::Type::Projectile:
						{
							packet.readChar();
							break;
						}
					}
					if(it->first == shipData.ID)
					{
						continue;
					}
					it->second->setPosition(pos);
					it->second->setHeading(heading);
					it->second->setUp(up);
					it->second->setVelocity(velocity);
				}
			}
			break;
		}
		case (char)Net::Header::CreateObject:
		{
			while(packet.good())
			{
				unsigned short id = packet.readShort();
				bool exists = (objects.find(id) != objects.end());
				bool insertObject = false;
				char type = packet.readChar();
				Point3 pos = packet.readPoint();
				Point3 heading = packet.readPoint();
				Point3 up = packet.readPoint();
				Point3 velocity = packet.readPoint();
				Object *object;
				switch(type)
				{
					case (char)Object::Type::Ship:
					{
						if(!exists)
						{
							Weapon *weapon = new WeaponLaser();
							object = new Ship(id, pos, heading, up, weapon);
							insertObject = true;
						}
						break;
					}
					case (char)Object::Type::Star:
					{
						Point3 color = packet.readPoint();
						float scale = packet.readFloat();
						unsigned int light = packet.readInt();
						if(!exists)
						{
							object = new Star(id, pos, heading, up, color, scale, light);
							insertObject = true;
						}
						break;
					}
					case (char)Object::Type::Planet:
					{
						float scale = packet.readFloat();
						std::string texture = packet.readString();
						if(!exists)
						{
							object = new Planet(id, pos, heading, up, scale, texture);
							insertObject = true;
						}
						break;
					}
					case (char)Object::Type::Projectile:
					{
						char projectileType = packet.readChar();
						Point3 velocity = packet.readPoint();
						Projectile *projectile;
						bool insertProjectile = false;
						exists = (projectiles.find(id) != projectiles.end());
						switch(projectileType)
						{
							case (char)Projectile::Type::Laser:
							{
								if(!exists)
								{
									projectile = new ProjectileLaser(id, pos, heading, up, velocity);
									insertProjectile = true;
								}
								break;
							}
						}
						if(insertProjectile)
						{
							projectiles.insert(std::pair<unsigned short, Projectile*>(id, projectile));
						}
						break;
					}
				}
				if(insertObject)
				{
					objects.insert(std::pair<unsigned short, Object*>(id, object));
				}
			}
			break;
		}
		case (char)Net::Header::RemoveObject:
		{
			unsigned short id = packet.readShort();
			objects.erase(id);
			break;
		}
		case (char)Net::Header::RemoveProjectile:
		{
			unsigned short id = packet.readShort();
			projectiles.erase(id);
			break;
		}
		case (char)Net::Header::ShipID:
		{
			shipData.ID = (unsigned short)packet.readShort();
			break;
		}
	}
}

void networking(void)
{
	ENetEvent event;
	while(!closing)
	{
		unsigned int waitTimeout = 1000;
		event = Net::waitForEvent(waitTimeout);
		switch(event.type)
		{
			case ENET_EVENT_TYPE_RECEIVE:
			{
				handlePacket(Net::Packet(std::string((char*)event.packet->data, event.packet->dataLength)));
				enet_packet_destroy(event.packet);
				break;
			}

			case ENET_EVENT_TYPE_DISCONNECT:
			{
				std::cout<<"Server closed."<<std::endl;
				closing = true;
				break;
			}
			case ENET_EVENT_TYPE_NONE:
			{
				//std::cout<<"No packets in last "<<waitTimeout/1000<<" second(s)"<<std::endl;
			}
		}
	}
}

void render(void)
{
	if(screenNeedsResizing)
	{
		glViewport(0, 0, renderer.screenWidth, renderer.screenHeight);
		screenNeedsResizing = false;
	}
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(75, (float)renderer.screenWidth/(float)renderer.screenHeight,  0.1,  500);
	//gluPerspective(75, 4.0/3.0,  0.1,  500);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	if(shipData.ID != (unsigned short)-1 && objects.find(shipData.ID) != objects.end())
	{
		Object *ship = objects.find(shipData.ID)->second;
		Point3 campos = ship->getPosition() - 9 * ship->getHeading() + 3 * ship->getUp() - 2 * ship->getHeading() * ((Ship*)ship)->getSpeed();
		gluLookAt(campos.x, campos.y, campos.z, ship->getPosition().x, ship->getPosition().y, ship->getPosition().z, ship->getUp().x, ship->getUp().y, ship->getUp().z);
		ship->draw(&renderer);
	}

	for(auto &object : objects)
	{
		if(object.second->getID() != shipData.ID)
		{
			object.second->draw(&renderer);
		}
	}

	for(auto &projectile : projectiles)
	{
		projectile.second->draw(&renderer);
	}

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0.0,  renderer.screenWidth, 0.0, renderer.screenHeight, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);

	glColor4f(1,1,1,1);

	if(shipData.ID != (unsigned short)-1 && objects.find(shipData.ID) != objects.end())
	{
		Object *ship = objects.find(shipData.ID)->second;
		short hp = ((Ship*)ship)->getHP();
		if(hp > 0)
		{
			glBegin(GL_QUADS);
				glVertex3f(0,0,0);
				glVertex3f(0,10,0);
				glVertex3f(renderer.screenWidth * hp / 1000, 10, 0);
				glVertex3f(renderer.screenWidth * hp / 1000, 0, 0);
			glEnd();
		}
	}

	renderer.renderText(std::string("Ping: ") + utils::toString(Net::getPing()), 2, 30);

	glEnable(GL_TEXTURE_2D);

	glEnable(GL_LIGHTING);

	// Display the result
	al_flip_display();
}

void handleEvents(void)
{
	ALLEGRO_EVENT_QUEUE *eventQueue = al_create_event_queue();
	al_register_event_source(eventQueue, al_get_display_event_source(renderer.window));
	while(!closing)
	{
		ALLEGRO_EVENT event;
		al_wait_for_event(eventQueue, &event);

		switch(event.type)
		{
			case ALLEGRO_EVENT_DISPLAY_CLOSE:
			{
				closing = true;
				break;
			}
			case ALLEGRO_EVENT_DISPLAY_RESIZE:
			{
				renderer.screenWidth = event.display.width;
				renderer.screenHeight = event.display.height;
				screenNeedsResizing = true;
				break;
			}
		}
	}
	al_destroy_event_queue(eventQueue);
}

int main(int argc, char* argv[])
{
	//renderer = Renderer();
	shipData.ID = -1;
	renderer = Renderer();
	if(!renderer.Init(argc, argv))
	{
		return 1;
	}

	glShadeModel(GL_SMOOTH);

	renderer.LoadModel("res/ship.obj");
	renderer.LoadTexture("res/ship.tga");

	renderer.LoadModel("res/sphere.obj");
	renderer.LoadTexture("res/valkoinen.tga");

	renderer.LoadTexture("res/planeetta.tga");

	renderer.LoadModel("res/laser.obj");
	renderer.LoadTexture("res/red.tga");

	std::string address = "localhost";
	unsigned int port = 8555;
	if(argc > 1)
	{
		address = argv[1];
	}
	if(argc > 2)
	{
		std::istringstream stream(argv[2]);
		stream >> port;
	}

	if(!Net::Connect(address, port))
	{
		std::cout << "Could not connect to " << address << " port " << port << ". Closing." << std::endl;
		return 1;
	}

	initializeControls();

	closing = false;

	std::thread networkThread(networking);
	std::thread calcThread(calculate);
	std::thread eventsThread(handleEvents);

	while (!closing)
	{
		render();
	}

	Net::Disconnect();

	deinitializeControls();

	calcThread.join();
	networkThread.join();
	eventsThread.join();

	renderer.deInit();

	//clean
	for(auto &object : objects)
	{
		delete object.second;
	}

	return 0;
}
