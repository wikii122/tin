#ifndef STORAGE_H
#define STORAGE_H
#include <string>
#include <vector>
#include <memory>


class Storage {

public:
    Storage(const std::string& path);
	~Storage();

    /* 
	 * Adds entire file to storage.
	 * Returns stringified md5.
	 */
	auto add_file(const char*data, long size, std::string name, std::string owner_id, long long expire_date) -> std::string;
	auto add_file(std::string path, std::string name) -> std::string;
	// Kopiuje wybrany plik do podanego katalogu
	bool copy_file(std::string name, std::string path);
	bool on_drive(std::string name);
	
    /* dodaje czesc pliku
     * jeżeli zadna czesc nie zostala jeszcze dodana, to plik zostanie utworzony */
    bool add_file_part(const char * data, long part_size, long offset, std::string name, std::string owner_name);

    /* po dodaniu wszystkich czesci mozna zakonczyc plik */
    bool finish_file(std::string name);
    bool is_finished(std::string name);

	bool remove_file(const std::string& name);
/* zwraca wskaznik na wektor
    * jezeli pliku nie udalo sie odczytac, to jest zwracany pusty wskaznik */
	std::shared_ptr<std::vector<char>> get_file(std::string name);
	
private:
    std::string path;
};

#endif

