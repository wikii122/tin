/*
*	Class representing a IForgot-type Packet
*	Author: Damian Rakowski
*	Date created: 02-06-2014
*/
#ifndef TIN_IFORGOTPACKET
#define TIN_IFORGOTPACKET

#include "packet/packet.h"
#include <vector>

//! Klasa opisująca pakiet typu IForgot
class IForgotPacket :	public Packet
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
	IForgotPacket(void);
	//! Destruktor
	~IForgotPacket(void);
};

#endif

