#include <jsoncpp/json/json.h>
#include "packet/forgetPacket.h"
#include "server.h"

//! Konstruktor
ForgetPacket::ForgetPacket(void)
{
	type = PacketType::IHave;
}

//! Destruktor
ForgetPacket::~ForgetPacket(void)
{
}

/*!
 * Metoda konwertująca pakiet na pakiet w formacie JSON
 * \return Pakiet w formacie JSON, gotowy do wysłania
 * \warning filename powinien zostać ustawiony albo zostanie rzucony wyjątek
 */
std::string ForgetPacket::getData()
{
	if(name == "")
		name = Server::get().get_name();
	if(filename == "")
		throw "ForgetPacket::getData(): No filename entry";

	Json::Value root;

	root["type"] = "Forget";
	root["name"] = name;
	root["file"] = filename;
	root["md5"] = md5;

	Json::StyledWriter writer;
	return  writer.write(root);
}
