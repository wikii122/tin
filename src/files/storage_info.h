#ifndef STORAGE_INFO_H
#define STORAGE_INFO_H
#include <mutex>
#include <string>
#include <vector>
#include "files/storage.h"
#include "packet/iHavePacket.h"

struct File {
	std::string name;
	bool isOwner;
	std::string md5;
	long long expire_date;
	bool local;
};

/**
* @brief  Moduł odpowiedzialny za przechowywanie listy plików i informacji o nich.
*/
class Storage_info {
	friend class Storage;
    Storage_info();
    Storage_info(const Storage_info&) = delete;
public:
	static Storage_info& get();
	void set_name(std::string name);
	bool add_file(const std::string& name, bool owner_name, long long date, const std::string& md5, bool local=true);
	bool remove(const std::string& name, const std::string& md5);
	auto file_info(std::string name) -> std::vector<File>;
    IHavePacket list_files_json(bool all);
	void clear();
	void set_ownership(const std::string& name, const std::string& md5, bool state);

private:
	std::vector<File> files;
    std::string host_name;
	std::mutex mutex;
};

#endif
