#ifndef STORAGE_INFO_H
#define STORAGE_INFO_H
#include <string>
#include <vector>
#include "packet/iHavePacket.h"

struct File {
    std::string name;
    std::string owner_name;
    std::string md5;
	long long expire_date;
};

class Storage_info {
    Storage_info();
public:
	static Storage_info& get();
	void set_name(std::string name);
	bool add_file(const std::string& name, const std::string& owner_name, long long date, const std::string& md5);
	bool remove(const std::string& name);
    IHavePacket list_files_json();

private:
	std::vector<File> files;
    std::string host_name;
};

#endif
