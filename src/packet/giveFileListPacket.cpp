#include <jsoncpp/json/json.h>
#include "packet/giveFileListPacket.h"
#include "server.h"

//! Konstruktor
GiveFileListPacket::GiveFileListPacket(void)
{
	type = PacketType::GiveFileList;
}

//! Destruktor
GiveFileListPacket::~GiveFileListPacket(void)
{
}

/*!
 * Metoda konwertująca pakiet na pakiet w formacie JSON
 * \return Pakiet w formacie JSON, gotowy do wysłania
 */
std::string GiveFileListPacket::getData()
{
	if(name == "")
		name = Server::get().get_name();

	Json::Value root;

	root["type"] = "GiveFileList";
	root["name"] = name;

	Json::StyledWriter writer;
	return  writer.write(root);
}
