#ifndef STORAGE_INFO_H
#define STORAGE_INFO_H

#include <string>

using namespace std;

struct File {
    string name;
    string owner_name;
    string md5;
};

class Storage_info {
public:

    Storage_info(string host_name);
	bool add_file(const string& name, const string& owner_name);
	bool remove(const string& name, string id);
	bool copy_file(const string&name, string owner_name);
    string list_files_json();

private:
    vector<File> files;
    string host_name;
};

#endif
