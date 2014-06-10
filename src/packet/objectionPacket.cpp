#include <jsoncpp/json/json.h>
#include "packet/objectionPacket.h"
#include "server.h"

//! Konstruktor
ObjectionPacket::ObjectionPacket(void)
{
	type = PacketType::Objection;
}

//! Destruktor
ObjectionPacket::~ObjectionPacket(void)
{
}

/*!
 * Metoda konwertująca pakiet na pakiet w formacie JSON
 * \return Pakiet w formacie JSON, gotowy do wysłania
 * \warning filename powinien zostać ustawiony albo zostanie rzucony wyjątek
 */
std::string ObjectionPacket::getData()
{
	if(name == "")
		name = Server::get().get_name();
	if(filename == "")
		throw "ObjectionPacket::getData(): No filename entry";

	Json::Value root;

	root["type"] = "Objection";
	root["name"] = name;
	root["file"] = filename;
	root["md5"] = md5;

	Json::StyledWriter writer;
	return  writer.write(root);
}
