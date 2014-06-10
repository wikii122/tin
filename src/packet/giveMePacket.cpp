#include <jsoncpp/json/json.h>
#include "packet/giveMePacket.h"
#include "server.h"

//! Konstruktor
GiveMePacket::GiveMePacket(void)
{
	type = PacketType::GiveMe;
}

//! Destruktor
GiveMePacket::~GiveMePacket(void)
{
}

/*!
 * Metoda konwertująca pakiet na pakiet w formacie JSON
 * \return Pakiet w formacie JSON, gotowy do wysłania
 * \warning filename powinien zostać ustawiony albo zostanie rzucony wyjątek
 */
std::string GiveMePacket::getData()
{
	if(name == "")
		name = Server::get().get_name();
	if(filename == "")
		throw "GiveMePacket::getData(): No filename entry";

	Json::Value root;

	root["type"] = "GiveMe";
	root["name"] = name;
	root["file"] = filename;
	root["md5"] = md5;
	root["original"] = original;

	Json::StyledWriter writer;
	return  writer.write(root);
}
