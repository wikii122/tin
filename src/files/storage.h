#ifndef STORAGE_H
#define STORAGE_H
#include <string>
#include <vector>
#include <memory>
#include <files/storage_info.h>

struct LoadedFile {
	LoadedFile();
	~LoadedFile();
	long long size;
	char* data;
	File meta;	
};

/**
* Klasa odpowiedzialna za przechowywanie i obsługę rzeczywistych danych.
*/
class Storage {

public:
    Storage(const std::string& path);
    Storage(const Storage&) = delete;
	~Storage();

    /* 
	 * Adds entire file to storage.
	 * Returns stringified md5.
	 */
	auto add_file(const char*data, long long size, std::string name, long long expire_date=0) -> std::string;
	auto add_file(std::string path, std::string name, bool local=true) -> std::string;
	// Kopiuje wybrany plik do podanego katalogu
	bool copy_file(std::string name, std::string path);
	bool on_drive(std::string name, std::string md5="");
	
    /* dodaje czesc pliku
     * jeżeli zadna czesc nie zostala jeszcze dodana, to plik zostanie utworzony */
    bool add_file_part(const char * data, long long part_size, long long offset, std::string name);

    /* po dodaniu wszystkich czesci mozna zakonczyc plik */
    bool finish_file(std::string name);
    bool is_finished(std::string name);

	bool remove_file(const std::string& name, const std::string& md5);
	/* 
	 * zwraca plikostrukturę LoadedFile
     * jezeli pliku nie udalo sie odczytac, to jest zwracany pusty wskaznik 
	 */
	auto get_file(std::string name, std::string md5) -> std::shared_ptr<LoadedFile>;
	void store_data();
private:
    std::string path;

};

#endif

