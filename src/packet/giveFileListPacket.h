/*
*	Class representing a GiveFileList-type Packet
*	Author: Damian Rakowski
*	Date created: 31-05-2014
*/

#ifndef TIN_GIVEFILELISTPACKET
#define TIN_GIVEFILELISTPACKET

#include "packet/packet.h"

//! Klasa opisująca pakiet typu GiveFileList
class GiveFileListPacket : public Packet
{
public:
	//! Pole oznaczające nadawcę
	std::string name;

	//! Metoda zwracająca pakiet JSON na podstawie pakietu
	std::string getData();
	//! Konstruktor
	GiveFileListPacket(void);
	//! Destruktor
	~GiveFileListPacket(void);
};

#endif
