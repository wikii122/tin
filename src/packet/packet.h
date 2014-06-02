/*
*	Class representing an abstract class Packet upon which are the other Packet-type classes based.
*	Author: Damian Rakowski
*	Date created: 24-05-2014
*/

#ifndef TIN_PACKET
#define TIN_PACKET

#include <string>
#include <memory>


enum class PacketType
{
	Hello,
	GiveFileList,
	IHave,
	GiveMe,
	IGot,
	Objection,
	IForgot,
	Forget,
};

class Packet
{
public:
	Packet(void);
	~Packet(void);

	virtual std::string getData() = 0;
	static auto getPacket(std::string data) -> std::shared_ptr<Packet>;
	PacketType getType() const;

protected:
	PacketType type;
};

#endif
