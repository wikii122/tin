#include "helloPacket.h"
int main()
{
	HelloPacket packet;
	packet.name = "asd";
	std::string asd = packet.getData();
	Packet* packet2 = Packet::getPacket(asd);
	HelloPacket* packet3 = (HelloPacket*)packet2;
}