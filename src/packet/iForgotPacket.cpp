#include <jsoncpp/json/json.h>
#include "packet/iForgotPacket.h"
#include "server.h"

//! Konstruktor
IForgotPacket::IForgotPacket(void)
{
	type = PacketType::IHave;
}

//! Destruktor
IForgotPacket::~IForgotPacket(void)
{
}

/*!
 * Metoda konwertująca pakiet na pakiet w formacie JSON
 * \return Pakiet w formacie JSON, gotowy do wysłania
 * \warning filename powinien zostać ustawiony albo zostanie rzucony wyjątek
 */
std::string IForgotPacket::getData()
{
	if(name == "")
		name = Server::get().get_name();
	if(filename == "")
		throw "IForgotPacket::getData(): No filename entry";

	Json::Value root;

	root["type"] = "IForgot";
	root["name"] = name;
	root["file"] = filename;
	root["md5"] = md5;

	Json::StyledWriter writer;
	return  writer.write(root);
}
