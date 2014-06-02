#ifndef STORAGE_INFO_H
#define STORAGE_INFO_H
#include <string>
#include <vector>

struct File {
    std::string name;
    std::string owner_name;
    std::string md5;
};

class Storage_info {
public:
    Storage_info();
	void set_name(std::string name);
	bool add_file(const std::string& name, const std::string& owner_name);
	bool remove(const std::string& name);
	bool copy_file(const std::string&name, std::string owner_name);
    std::string list_files_json();

private:
	std::vector<File> files;
    std::string host_name;
};

#endif
