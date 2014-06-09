/*
*	Class representing a IHave-type Packet
*	Author: Damian Rakowski
*	Date created: 31-05-2014
*/
#ifndef TIN_IHAVEPACKET
#define TIN_IHAVEPACKET

#include "packet/packet.h"
#include <vector>

//! Struktura opisująca plik wykorzystywana przez pakiet typu IHave
struct IHavePacketFile
{
	//! Pole oznaczające nazwę pliku
	std::string name;
	//! Pole oznaczające MD5 pliku
	std::string md5;
	//! Pole mówiące o własności pliku
	bool isOwner;
	//! Pole mówiące, kiedy plik straci ważność
	long long expires;
};

//! Klasa opisująca pakiet typu IHave
class IHavePacket :	public Packet
{
public:
	//! Pole oznaczające nadawcę
	std::string name;
	//! Wektor z listą plików
	std::vector<IHavePacketFile> files;

	//! Metoda zwracająca pakiet JSON na podstawie pakietu
	std::string getData();
	//! Konstruktor
	IHavePacket(void);
	//! Destruktor
	~IHavePacket(void);
};

#endif

