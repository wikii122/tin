/*
*	Class representing a Local-type Packet
*	Author: Damian Rakowski
*	Date created: 01-06-2014
*/
#ifndef TIN_LOCALPACKET
#define TIN_LOCALPACKET

#include "packet/packet.h"
#include <vector>

//! Klasa opisująca pakiet typu Local
class LocalPacket: public Packet
{
public:
	//! Pole oznaczające nadawcę
	std::string name;
	//! Pole oznaczające nazwę pliku
	std::string file;
	//! Pole oznaczające komendę
	std::string command;

	//! Metoda zwracająca pakiet JSON na podstawie pakietu
	auto getData() -> std::string override;

	//! Konstruktor
	LocalPacket();
	//! Destruktor
	~LocalPacket();
};

#endif

