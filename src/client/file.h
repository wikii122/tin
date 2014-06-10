/*
 * Declares client's file operations.
 */
#include <string>
#ifndef FILE_H_
#define FILE_H_

namespace client 
{
	//! Dodaj plik do serwera
	int add(std::string path);
	//! Dodaj plik do serwera
	int add(std::string path, std::string filename);
	//! Skopiuj plik z serwera
	int get(std::string filename);
	//! Skopiuj plik z serwera
	int get(std::string filename, std::string path);
	//! Usuń plik z serwera.
	int remove(std::string filename);
}

#endif
