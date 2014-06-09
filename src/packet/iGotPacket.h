/*
*	Class representing a IGot-type Packet
*	Author: Damian Rakowski
*	Date created: 01-06-2014
*/
#ifndef TIN_IGOTPACKET
#define TIN_IGOTPACKET

#include "packet/packet.h"
#include <vector>

//! Klasa opisująca pakiet typu IGot
class IGotPacket :	public Packet
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
	IGotPacket(void);
	//! Destruktor
	~IGotPacket(void);
};

#endif

