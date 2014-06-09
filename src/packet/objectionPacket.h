/*
*	Class representing a Objection-type Packet
*	Author: Damian Rakowski
*	Date created: 01-06-2014
*/
#ifndef TIN_OBJECTIONPACKET
#define TIN_OBJECTIONPACKET

#include "packet/packet.h"
#include <vector>

//! Klasa opisująca pakiet typu Objection
class ObjectionPacket :	public Packet
{
public:
	//! Pole oznaczające nadawcę
	std::string name;
	//! Pole oznaczające nazwę pliku
	std::string filename;
	//! Pole oznaczające MD5 pliku
	std::string md5;

	//! Metoda zwracająca pakiet JSON na podstawie pakietu
	std::string getData();
	//! Konstruktor
	ObjectionPacket(void);
	//! Destruktor
	~ObjectionPacket(void);
};

#endif

