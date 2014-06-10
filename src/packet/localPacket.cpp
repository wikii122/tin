#include <jsoncpp/json/json.h>
#include <iostream>
#include "packet/localPacket.h"
#include "server.h"

//! Konstruktor
LocalPacket::LocalPacket()
{
	type = PacketType::Local;
}

//! Destruktor
LocalPacket::~LocalPacket()
{

}

/*!
 * Metoda konwertująca pakiet na pakiet w formacie JSON
 * \return Pakiet w formacie JSON, gotowy do wysłania
 * \warning filename powinien zostać ustawiony albo zostanie rzucony wyjątek
 */
auto LocalPacket::getData() -> std::string
{
	if (name == "")
		name = Server::get().get_name();
	if (file == "")
		throw "LocalPacket::getData(): No filename entry";

	Json::Value root;

	root["type"] = command;
	root["name"] = name;
	root["file"] = file;

	Json::StyledWriter writer;
	return  writer.write(root);
}
