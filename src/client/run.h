/*
 * Set of functions used to control the state 
 * of server.
 */
#include <string>
#ifndef RUN_H_
#define RUN_H_

//! Funkcje wpływające bezpośrednio na działanie serwera.
namespace server {
	//! Start serwera.
	int start(std::string name);
	//! Stop serwera.
	int stop();
	//! Restart serwera
	int restart(std::string name);
	//! Scieżka do pliku wykonywalnego serwera.
	static const char server_path[] = "./server"; 

}

#endif
