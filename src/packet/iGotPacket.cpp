#include <jsoncpp/json/json.h>
#include "packet/iGotPacket.h"
#include "server.h"

//! Konstruktor
IGotPacket::IGotPacket(void)
{
	type = PacketType::IGot;
}

//! Destruktor
IGotPacket::~IGotPacket(void)
{
}

/*!
 * Metoda konwertująca pakiet na pakiet w formacie JSON
 * \return Pakiet w formacie JSON, gotowy do wysłania
 * \warning filename powinien zostać ustawiony albo zostanie rzucony wyjątek
 */
std::string IGotPacket::getData()
{
	if(name == "")
		name = Server::get().get_name();
	if(filename == "")
		throw "IGotPacket::getData(): No filename entry";

	Json::Value root;

	root["type"] = "IGot";
	root["name"] = name;
	root["file"] = filename;
	root["md5"] = md5;

	Json::StyledWriter writer;
	return  writer.write(root);
}
