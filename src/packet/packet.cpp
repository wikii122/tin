#include <jsoncpp/json/json.h>
#include "packet/packet.h"

#include "packet/helloPacket.h"
#include "packet/giveFileListPacket.h"
#include "packet/iHavePacket.h"
#include "packet/giveMePacket.h"
#include "packet/iGotPacket.h"
#include "packet/objectionPacket.h"
#include "packet/forgetPacket.h"
#include "packet/iForgotPacket.h"
#include "packet/localPacket.h"

//! Konstruktor
Packet::Packet(void) 
{

}

//! Destruktor
Packet::~Packet(void)
{

}

//! Metoda zwracająca typ pakietu
PacketType Packet::getType() const
{
	return this->type;
}

/*!
 * Metoda konwertująca dane JSON na pakiet. Po konwersji należy zrzutować wynik na pakiet, zgodnie z wynikiem metody getType().
 * \param data Dane w formacie JSON opisujące pakiet
 * \return Pakiet po konwersji
 */
std::shared_ptr<Packet> Packet::getPacket(std::string data)
{
	Json::Value root; //the JSON tree will be stored here
	Json::Reader reader;

	std::shared_ptr<Packet> packet;

	if(!reader.parse(data, root))
		throw std::string("Packet::getPacket: Unparseable data");

	std::string type = root.get("type", "").asString();

	if(type == "Hello")
	{
		auto helloPacket = std::make_shared<HelloPacket>();
		helloPacket->name = root.get("name", "").asString();

		packet = helloPacket;
	}
	else if(type == "GiveFileList")
	{
		auto giveFileListPacket = std::make_shared<GiveFileListPacket>();
		giveFileListPacket->name = root.get("name", "").asString();

		packet = giveFileListPacket;
	}
	else if(type == "IHave")
	{
		auto iHavePacket = std::make_shared<IHavePacket>();
		iHavePacket->name = root.get("name", "").asString();
		
		for(unsigned int i = 0; i < root.get("files", "").size(); ++i)
		{
			IHavePacketFile file;
			file.name = root.get("files", "")[i]["file"].asString();
			file.md5 = root.get("files", "")[i]["md5"].asString();
			file.expires = root.get("files", "")[i]["expires"].asInt64();
			file.isOwner = root.get("files", "")[i]["isOwner"].asBool();

			iHavePacket->files.push_back(file);
		}

		packet = iHavePacket;
	}
	else if(type == "GiveMe")
	{
		auto giveMePacket = std::make_shared<GiveMePacket>();
		giveMePacket->name = root.get("name", "").asString();
		giveMePacket->md5 = root.get("md5", "").asString();
		giveMePacket->filename = root.get("file", "").asString();
		giveMePacket->original = root.get("original", "").asBool();

		packet = giveMePacket;
	}
	else if(type == "IGot")
	{
		auto iGotPacket = std::make_shared<IGotPacket>();
		iGotPacket->name = root.get("name", "").asString();
		iGotPacket->md5 = root.get("md5", "").asString();
		iGotPacket->filename = root.get("file", "").asString();

		packet = iGotPacket;
	}
	else if(type == "Objection")
	{
		auto objectionPacket = std::make_shared<ObjectionPacket>();
		objectionPacket->name = root.get("name", "").asString();
		objectionPacket->md5 = root.get("md5", "").asString();
		objectionPacket->filename = root.get("file", "").asString();

		packet = objectionPacket;
	}
	else if(type == "Forget")
	{
		auto forgetPacket = std::make_shared<ForgetPacket>();
		forgetPacket->name = root.get("name", "").asString();
		forgetPacket->md5 = root.get("md5", "").asString();
		forgetPacket->filename = root.get("file", "").asString();

		packet = forgetPacket;
	}
	else if(type == "IForgot")
	{
		auto iForgotPacket = std::make_shared<IForgotPacket>();
		iForgotPacket->name = root.get("name", "").asString();
		iForgotPacket->md5 = root.get("md5", "").asString();
		iForgotPacket->filename = root.get("file", "").asString();

		packet = iForgotPacket;
	}
	else if(type == "") {
		throw std::string("Packet::getPacket: No type specified");
	} else {
		auto localPacket = std::make_shared<LocalPacket>();
		localPacket->command = type;
		localPacket->file = root.get("file", "").asString();
		localPacket->name = root.get("name", "").asString();
		
		packet = localPacket;
	}

	return packet;
}
