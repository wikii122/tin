/*
*	Class representing a GiveMe-type Packet
*	Author: Damian Rakowski
*	Date created: 01-06-2014
*/
#ifndef TIN_GIVEMEPACKET
#define TIN_GIVEMEPACKET

#include "packet/packet.h"
#include <vector>

//! Klasa opisująca pakiet typu GiveMe
class GiveMePacket :	public Packet
{
public:
	//! Pole oznaczające nadawcę
	std::string name;
	//! Pole oznaczające nazwę pliku
	std::string filename;
	//! Pole oznaczające MD5 pliku
	std::string md5;
	//! Pole oznaczające to, czy plik jest przesyłany w wyniku transferu posiadacza
	bool original;

	//! Metoda zwracająca pakiet JSON na podstawie pakietu
	std::string getData();
	//! Konstruktor
	GiveMePacket(void);
	//! Destruktor
	~GiveMePacket(void);
};

#endif
