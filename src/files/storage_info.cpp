#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <jsoncpp/json/json.h>
#include "server.h"
#include "files/md5.h"
#include "files/sdbm.h"
#include "files/storage.h"
#include "files/storage_info.h"
#include "packet/iHavePacket.h"

using namespace std;

Storage_info::Storage_info()
{

}

/**
* Funkcja zwracająca jedyną instancję klasy.
*/
Storage_info& Storage_info::get()
{
	static Storage_info instance;
	return instance;
}

/**
* Funkcja określająca właściciela pliku.
*/
void Storage_info::set_ownership(const string& name, const string& md5, bool newstate)
{
	mutex.lock();
	for (File file: files) {
		if (file.name == name and file.md5 == md5) {
			file.isOwner = newstate;
		}
	}
	mutex.unlock();
	Server::get().get_storage().store_data();
}

/**
* Funkcja dodająca plik do listy plików.
*/
bool Storage_info::add_file(const string& name, bool owner_name, long long date, const string& md5, bool local) {
	mutex.lock();
	for (File file: files) {
		if (file.name == name and file.md5 == md5) {
			file.isOwner = owner_name;
			file.expire_date = date;
			file.local = local? local:file.local;
			mutex.unlock();
			return false;
		}
	}
    File f;
    f.name = name;
    f.isOwner = owner_name;
    f.md5 = md5;
	f.expire_date = date;
	f.local = local;
    files.push_back(f);
	mutex.unlock();
    return true;
}

/**
* Funkcja zwracająca pakiet z listą plików.
* Jeżeli all jest true to zwraca listę wszystkich plików,
* a jeśli false to tylko tych które są w tym węźle.
*/
IHavePacket Storage_info::list_files_json(bool all) {
	IHavePacket packet;
	Storage& storage = Server::get().get_storage();
	packet.name = host_name;
	mutex.lock();
	for (File file: files) {
		if (storage.on_drive(file.name) or all) {
			IHavePacketFile meta;
			meta.name = file.name;
			meta.isOwner = file.isOwner;
			meta.expires = file.expire_date;
			meta.md5 = file.md5;
			packet.files.push_back(meta);
		}
	}
	mutex.unlock();
	return packet;
}

/**
* Funkcja usuwająca plik z listy plików.
*/
bool Storage_info::remove(const string& name, const string& md5) {

	mutex.lock();
    std::vector<File>::iterator iter = files.begin();
    while (iter != files.end()) {
        if (iter->name == name and iter->md5 == md5) {
            iter = files.erase(iter);
			mutex.unlock();
            return true;
        } else {
            ++iter;
        }
    }
	mutex.unlock();
    return false;
}

/**
* Funkcja ustawiająca nazwę węzła.
*/
void Storage_info::set_name(string new_name)
{
	host_name = new_name;
}

/**
* Funkcja pobierająca pliki o podanej nazwie.
*/
vector<File> Storage_info::file_info(string name)
{
	vector<File> result;
	mutex.lock();
	for (File file:files) {
		if (file.name == name) {
			result.push_back(file);		
		}
	}
	mutex.unlock();
	return result;
}

/**
* Funkcja usuwająca z listy pliki nieprzechowywane lokalnie.
*/
void Storage_info::clear()
{
	mutex.lock();
	int i = 0;
	for(auto it: files)
	{
		if(it.local) {
			files[i++] = it;
		}
	}
	files.resize(i);
	mutex.unlock();
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
