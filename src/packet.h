#ifndef TIN_PACKET
#define TIN_PACKET

#include <string>

enum class PacketType
{
	Hello,
};

class Packet
{
public:
	Packet(void);
	~Packet(void);

	virtual std::string getData() = 0;
	static Packet* getPacket(std::string data);
	const PacketType getType();

private:
	PacketType type;
};

#endif