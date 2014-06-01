#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include "md5.h"
#include "sdbm.h"
#include "json/json.h"

using namespace std;

struct File {

    string name;
    string owner_name;
    string md5;

};

class Storage_info {
public:
    Storage_info(string host_name) : host_name(host_name) {}

    bool add_file(const string& name, const string& owner_name) {

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
        return true;
    }

    string list_files_json() {
        Json::Value root;
        root["type"] = "IHave";
        root["name"] = host_name;
        root["files"] = Json::arrayValue;

        for (File f : files) {
            Json::Value v;

            v["file"] = f.name;
            v["md5"] = f.md5;
            v["isOwner"] = (host_name == f.owner_name);

            root["files"].append(v);

            cout << host_name << ' ' << f.owner_name << endl;
            cout << root << endl;
        }

        return "";
    }

    bool remove(const string& name, string id) {
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

    bool copy_file(const string& name, string owner_name) {
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

private:
    vector<File> files;
    string host_name;
};

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