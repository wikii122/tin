#ifndef STORAGE_H
#define STORAGE_H

#include <string>
#include <vector>

using namespace std;

struct File {

    string name;
    string owner_name;
    bool complete;
};

class Storage {

public:
    Storage(const string& path);

    /* dodaje caly plik */
	bool add_file(const char*data, long size, string name, string owner_id);

    /* dodaje czesc pliku
     * jeżeli zadna czesc nie zostala jeszcze dodana, to plik zostanie utworzony */
    bool add_file_part(const char * data, long part_size, long offset, string name, string owner_name);

    /* po dodaniu wszystkich czesci mozna zakonczyc plik */
    bool finish_file(string name);
    bool is_finished(string name);

	bool remove_file(const string& name, string id);

private:
    string path;
    vector<File> files;
};

#endif

