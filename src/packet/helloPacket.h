/*
*	Class representing a Hello-type Packet
*	Author: Damian Rakowski
*	Date created: 24-05-2014
*/

#ifndef TIN_HELLOPACKET
#define TIN_HELLOPACKET

#include "packet/packet.h"

//! Klasa opisująca pakiet typu HelloPacket
class HelloPacket : public Packet
{
public:
	//! Pole oznaczające nadawcę
	std::string name;

	//! Metoda zwracająca pakiet JSON na podstawie pakietu
	std::string getData();
	//! Konstruktor
	HelloPacket(void);
	//! Destruktor
	~HelloPacket(void);
};

#endif
