/*
*	Class representing an abstract class Packet upon which are the other Packet-type classes based.
*	Author: Damian Rakowski
*	Date created: 24-05-2014
*/

#ifndef TIN_PACKET
#define TIN_PACKET

#include <string>
#include <memory>

//! Klasa wyliczeniowa wymieniająca typy pakietów
enum class PacketType
{
	Hello,
	GiveFileList,
	IHave,
	GiveMe,
	IGot,
	Objection,
	IForgot,
	Forget,
	Local,
};

//! Klasa abstrakcyjna opisująca pakiet
class Packet
{
public:
	//! Pusty konstruktor
	Packet(void);
	//! Pusty destruktor
	~Packet(void);

	//! Metoda czysto wirtualna zwracająca pakiet JSON na podstawie pakietu
	virtual std::string getData() = 0;
	//! Statyczna metoda zwracająca odpowiedni pakiet na podstawie tekstu JSON. Zwrócony pakiet nalezy rzutowac
	static auto getPacket(std::string data) -> std::shared_ptr<Packet>;
	//! Metoda zwracająca typ pakietu
	PacketType getType() const;

protected:
	PacketType type;
};

#endif
