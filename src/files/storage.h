#ifndef STORAGE_H
#define STORAGE_H
#include <string>
#include <vector>
#include <memory>


class Storage {

public:
    Storage(const std::string& path);

    /* dodaje caly plik */
	bool add_file(const char*data, long size, std::string name, std::string owner_id);
	// Dodaje cały plik z dysku
	bool add_file(std::string path, std::string name);
	// Kopiuje wybrany plik do podanego katalogu
	bool copy_file(std::string name, std::string path);
	
    /* dodaje czesc pliku
     * jeżeli zadna czesc nie zostala jeszcze dodana, to plik zostanie utworzony */
    bool add_file_part(const char * data, long part_size, long offset, std::string name, std::string owner_name);

    /* po dodaniu wszystkich czesci mozna zakonczyc plik */
    bool finish_file(std::string name);
    bool is_finished(std::string name);

	bool remove_file(const std::string& name);
/* zwraca wskaznik na wektor
    * jezeli pliku nie udalo sie odczytac, to jest zwracany pusty wskaznik */
   	shared_ptr<vector<char>> get_file(string name);
	
	struct File {
		std::string name;
		std::string owner_name;
    	bool complete;
	};

private:
    std::string path;
	std::vector<File> files;
};

#endif

