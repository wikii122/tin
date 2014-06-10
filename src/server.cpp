#include <iostream>
#include <string>
#include <thread>
#include <boost/filesystem.hpp>
#include "clienthandler.h"
#include "networkhandler.h"
#include "server.h"

namespace filesystem=boost::filesystem;

using namespace std;

/*!
 * Tworzy nową instancję serwera. Dodatkowo tworzy plik file_store na przechowywanie
 * plików oraz inicjuje obiekty rozszerzające Handler oraz Storage.
 */
Server::Server(): 
	running(false) {
	auto rel_path = filesystem::path("file_store");
	auto abs_path = filesystem::complete(rel_path);

	client_handler = new ClientHandler();
	network_handler = new NetworkHandler();
	connection_handler = new ConnectionHandler();

	handlers.push_back(client_handler);
	handlers.push_back(network_handler);
	handlers.push_back(connection_handler);

	if (!filesystem::exists(abs_path)) {
		filesystem::create_directories(abs_path);
	} 

	storage = new Storage(abs_path.string<string>());
}

/*!
 * Zwraca instancje serwera, jeśli takowa nie istnieje,
 * tworzy nową.
 * \return Jedyna instancja klasy Server
 */
Server& Server::get()
{
	static Server instance;
	return instance;
}

/*!
 * Czyści pamięć.
 */
Server::~Server()
{
	running = false;
	delete client_handler;
	delete network_handler;
	delete connection_handler;
	delete storage;
}

/*!
 * \param new_name Nazwa serwera
 */
int Server::set_name(string new_name)
{
	name = new_name;	
	Storage_info::get().set_name(new_name);
	return 0;
}

/*!
 * \return Nazwa serwera
 */
string Server::get_name()
{
	return name;
}

/*!
 * Funkcja tworzy po jednym wątku dla każdej instancji klasy Handler,
 * i zaczyna cyklicznie wywoływać ich funkcje Handler::handle. 
 */
void Server::serve()
{
	int i = 0;
	running = true;
	for (Handler* handler: handlers) {
		auto fun = [this](Handler* handler) {
			while (this->running) try {
				handler->handle();
			} catch (exception err) {
				cout << err.what() << endl;
				terminate();
			} catch (char* err) {
				cout << err << endl;
				terminate();
			} catch (const char* err) {
				cout << err << endl;
				terminate();
			} catch (string err) {
				cout << err << endl;
				terminate();
			} catch (...) {
				cout << "Unhandled error" << endl;
				terminate();
			}
		};
		threads[i] = thread(fun, handler);
		if (i == no_threads) {
			throw string("Too many jobs passed to server");
		} else if (i > 0) {
			threads[i].detach();
		}
		i++;
	}
	threads[0].join();
}

/*!
 * \return Referencja do instancji Storage
 */
Storage& Server::get_storage()
{
	return *storage;
}

/*!
 * \return Referencja do instancji Storage_info
 */
Storage_info& Server::get_storage_info()
{
	return Storage_info::get();
}

/*!
 * \return Referencja do instancji ClientHandler
 */
ClientHandler& Server::client()
{
	return *client_handler;
}

/*!
 * \return Referencja do instancji NetworkHandler
 */
NetworkHandler& Server::network()
{
	return *network_handler;
}

/*!
 * \return Referencja do instancji ConnctionHandler.
 */
ConnectionHandler& Server::connection()
{
	return *connection_handler;
}
