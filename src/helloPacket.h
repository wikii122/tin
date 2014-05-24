#ifndef TIN_HELLOPACKET
#define TIN_HELLOPACKET

#include "packet.h"

class HelloPacket : public Packet
{
public:
	std::string name;

	std::string getData();
	HelloPacket(void);
	~HelloPacket(void);
};

#endif