#include "point3.h"

#include <string>

#include <enet/enet.h>

namespace Net
{
	enum class Header : unsigned char
	{
		ObjectData       = 0x00,
		CreateObject     = 0x01,
		RemoveObject     = 0x02,
		RemoveProjectile = 0x03,

		ShipID   = 0x10,
		HP       = 0x11,
		Shoot    = 0x12,
		cooldown = 0x13
	};

	bool Connect(std::string address, unsigned int port);
	void Listen(unsigned int port);

	int getPing(void);

	void Disconnect(void);

	class Packet
	{
	private:
		std::string data;
		unsigned int pos;

	public:
		Packet();
		Packet(std::string packet);

		void writeString(std::string arg);
		void writeInt(int arg);
		void writeChar(unsigned char arg);
		void writeFloat(float arg);
		void writePoint(Point3 arg);
		void writeShort(short arg);

		unsigned char readChar(void);
		int readInt(void);
		float readFloat(void);
		std::string readString(void);
		Point3 readPoint(void);
		short readShort(void);

		bool good(void) const;

		unsigned int getPos(void) const;

		std::string getData(void) const;
	};

	void Send(Packet packet);
	void Send(Packet packet, ENetPeer *peer);

	ENetEvent getEvent(void);
	ENetEvent waitForEvent(unsigned int timeout);

};
