#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <jsoncpp/json/json.h>
#include "files/md5.h"
#include "files/sdbm.h"
#include "files/storage_info.h"

using namespace std;


Storage_info::Storage_info()
{

}

Storage_info& Storage_info::get()
{
	static Storage_info instance;
	return instance;
}

bool Storage_info::add_file(const string& name, const string& owner_name) {

    unsigned long salt = sdbm(owner_name.c_str());

    /* hash jest liczony z nazwy pliku polaczonej sola */
    stringstream sstr;
    sstr << name << salt;
    string md5 = MD5(sstr.str()).hexdigest();

    File f;
    f.name = name;
    f.owner_name = owner_name;
    f.md5 = md5;

    files.push_back(f);

    /* nie jest potrzebne sprawdzanie, czy taki plik istnieje? */
    return true;
}

string Storage_info::list_files_json() {
	string json;
    Json::Value root;
	Json::StyledWriter writer;

    root["type"] = "IHave";
    root["name"] = host_name;
    root["files"] = Json::arrayValue;

    for (File f : files) {
        Json::Value v;

        v["file"] = f.name;
        v["md5"] = f.md5;
        v["isOwner"] = (host_name == f.owner_name);

        root["files"].append(v);
    }

	json = writer.write(root);

    return json;
}

bool Storage_info::remove(const string& name) {

    std::vector<File>::iterator iter = files.begin();
    while (iter != files.end()) {
        if (iter->name == name) {
            iter = files.erase(iter);
            return true;
        } else {
            ++iter;
        }
    }

    return false;
}

bool Storage_info::copy_file(const string& name, string owner_name) {

    std::vector<File>::iterator iter = files.begin();

    while (iter != files.end()) {
        if (iter->name == name) {
            File temp;
            temp.name = name;
            temp.owner_name = owner_name;

            temp = *iter;
            files.push_back(temp);
            return true;
        } else {
            ++iter;
        }
    }

    return false;
}

void Storage_info::set_name(string new_name)
{
	host_name = new_name;
}

/*
int main() {

    //Storage_info storage;
    //string name = "plik1";
    //string id = "1";

    //string name2 = "plik2";
    //string id2 = "2";

    //cout << storage.add_file("plik1", "1") << endl;
    //cout << storage.add_file("plik2", "2") << endl;

    //cout <<storage.remove("plik2", "2") << endl;

    //cout <<storage.copy_file("plik1","1") << endl;

    Storage_info storage("host_name");
    storage.add_file("filename", "hostID");
    storage.add_file("filename2", "host_name");
    storage.list_files_json();
}
*/
