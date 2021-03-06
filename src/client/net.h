/*
 * Client's network interface.
 */
#include <string>
#ifndef NET_H_
#define NET_H_

namespace client {
	int download(std::string filename, std::string nodename);
	//! Pobiera plik z sieci
	int download(std::string filename);
	//! Odświerza listę plików
	int rescan();
	//! Wyświetla dostępne pliki.
	int showlist();
}

#endif
