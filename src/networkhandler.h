/*
 * Module responsible for handling networking
 */

#ifndef NETWORK_HANDLER_
#define NETWORK_HANDLER_
#include "handler.h"
#include "packet/packet.h"
#include "packet/giveFileListPacket.h"
#include "packet/iHavePacket.h"
#include "packet/giveMePacket.h"
#include "packet/iGotPacket.h"
#include "packet/objectionPacket.h"
#include "packet/forgetPacket.h"
#include "packet/iForgotPacket.h"

#include <string>
#include <sys/socket.h>
#include <netinet/in.h>  
#include <arpa/inet.h> 
#include <queue>
#include <mutex>

//! Klasa zajmująca się obsługą UDP
class NetworkHandler: public Handler
{
	//! Metoda, która tworzy gniazdo broadcastowe
	void createBroadcastSocket();

	//! Metoda sprawdzająca, czy przyszła obiekcja na dany plik
	bool isObjected(std::string name, std::string md5);
	//! Metoda czyszcząca obiekcje na dany plik
	void clearObjected(std::string name, std::string md5);

	//! Wektor przechowujący informacje o odebranych plikach
	std::vector<std::pair<std::string, std::string>> getFiles();
	//! Metoda czyszcząca informacje o odebranych plikach
	void clearFiles();

	int sock;
	sockaddr_in addr;

	sockaddr_in sender;
	socklen_t sendersize;

	std::vector<sockaddr_in> ownAddr;

	std::string buffer;
public:
	//! Konstruktor
	NetworkHandler();
	//! Destruktor
	~NetworkHandler();

	//! Dodanie do kolejki wysyłanych pakietów
	void addToQueue(std::shared_ptr<Packet> msg);

	//! Obsłuż, zarządzające całą komunikacją TCP
	virtual int handle() override; //Blocking!
	//! Odbierz dane z broadcastu
	virtual auto read() -> std::string override; // Blocking
	//! Wypisz dane broadcastu
	virtual int write(std::string) override;

	//! Odpowiedz ostatniemu nadawcy broadcastu
	int respond(std::string msg);

	//! Obsłuż pakiet typu GiveFileList
	void handlePacket(std::shared_ptr<GiveFileListPacket> packet);
	//! Obsłuż pakiet typu IHave
	void handlePacket(std::shared_ptr<IHavePacket> packet);
	//! Obsłuż pakiet typu GiveMe
	void handlePacket(std::shared_ptr<GiveMePacket> packet);
	//! Obsłuż pakiet typu IGot
	void handlePacket(std::shared_ptr<IGotPacket> packet);
	//! Obsłuż pakiet typu Objection
	void handlePacket(std::shared_ptr<ObjectionPacket> packet);
	//! Obsłuż pakiet typu IForgot
	void handlePacket(std::shared_ptr<IForgotPacket> packet);
	//! Obsłuż pakiet typu Forget
	void handlePacket(std::shared_ptr<ForgetPacket> packet);

	//! Wektor informujący o tym, do jakich plików są obiekcje
	std::vector<std::pair<std::string, std::string>> objected;
	//! Wektor informujący o tym, jakie pliki są w sieci
	std::vector<std::pair<std::string, std::string>> reportedFiles;

	//! Mutex chroniący kolejkę
	std::mutex queueMutex;
	//! Kolejka pakietów
	std::queue<std::shared_ptr<Packet>> queue;
};
#endif
