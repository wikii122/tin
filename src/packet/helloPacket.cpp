#include <jsoncpp/json/json.h>
#include "packet/helloPacket.h"
#include "server.h"

//! Konstruktor
HelloPacket::HelloPacket(void)
{
	type = PacketType::Hello;
}

//! Destruktor
HelloPacket::~HelloPacket(void)
{
}

/*!
 * Metoda konwertująca pakiet na pakiet w formacie JSON
 * \return Pakiet w formacie JSON, gotowy do wysłania
 */
std::string HelloPacket::getData()
{
	if(name == "")
		name = Server::get().get_name();

	Json::Value root;

	root["type"] = "Hello";
	root["name"] = name;

	Json::StyledWriter writer;
	return  writer.write(root);
}
