#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <boost/filesystem.hpp>
#include <jsoncpp/json/json.h>
#include "files/md5.h"
#include "files/storage.h"
#include "files/storage_info.h"
#include "server.h"

using namespace std;

/**
* @brief Konstruktor. 
* Jeżeli podana ścieżka istnieje to wczytywane są informacje o plikach w niej istniejących.
*
* @param path ścieżka do folderu, gdzie pliki będą zapisywane
*/
Storage::Storage(const string& path): path(path) 
{
	auto dir_path = boost::filesystem::path(path);
	auto storage_path = boost::filesystem::path("files.json");
	storage_path = boost::filesystem::absolute(dir_path / storage_path);
	
	if (boost::filesystem::exists(storage_path)) {
		ifstream storage_file;
		Json::Reader reader;	
		Json::Value file_list;

		storage_file.open(storage_path.c_str());
		reader.parse(storage_file, file_list);
		storage_file.close();
		
		file_list = file_list["files"];
		for(Json::Value file: file_list) {
			File file_desc;
			file_desc.name = file["name"].asString();
			file_desc.isOwner = file["owner"].asBool();
			file_desc.local = true;
			file_desc.expire_date = file["expire"].asInt64();
			file_desc.md5 = file["md5"].asString();
			Storage_info::get().mutex.lock();
			Storage_info::get().files.push_back(file_desc);
			Storage_info::get().mutex.unlock();
		}
	}
}

Storage::~Storage() {

}

/**
* @brief Funkcja która zapisuje dane o plikach na dysku w pliku files.json.
* Ten plik powstanie w folderze, który został podany w kontruktorze.
*/
void Storage::store_data() {
	ofstream storage_file;
	auto dir_path = boost::filesystem::path(path);
	auto storage_path = boost::filesystem::path("files.json");
	storage_path = dir_path / storage_path;

	Json::StyledWriter writer;
	Json::Value file_list;

	file_list["files"] = Json::arrayValue;
	
	Storage_info::get().mutex.lock();
	for (File file: Storage_info::get().files) {
		if (file.local) {
			Json::Value file_desc;
			file_desc["name"] = file.name;
			file_desc["owner"] = file.isOwner;
			file_desc["expire"] = file.expire_date;
			file_desc["md5"] = file.md5;
			file_list["files"].append(file_desc);
		}
	}
	
	storage_file.open(storage_path.c_str(), ios::out | ios::trunc);
	string list = writer.write(file_list);	
	storage_file << list;
	storage_file.close();
	Storage_info::get().mutex.unlock();
}

/**
* @brief Funkcja dodający plik na dysk. 
* W odróżnieniu od drugiej funkcji o tej samej nazwie,
* ta funkcja przyjmuje wskaźnik na znak jako źródło danych.
* 
* @param data wskaźnik na dane do zapisu
* @param size rozmiar danych do zapisu
* @param name nazwa pliku do zapisu
* @param expire_date data ważności pliku
*
* @return skrót md5 dodawanego pliku
*/
string Storage::add_file(const char* data, long long size, string name, long long expire_date) {
    ofstream file;
	string md5;

	md5 = MD5(string(data, data+size)).hexdigest();

    for(long long i = 0; i < size; i++) {
        stringstream n;
        n << path << "/" << name << "." << md5;

        file.open(n.str().c_str());
        file.clear();

        file.write(data + i, 1);

        if(!file.good())
            return "";
    }
	
    file.close();
	if (expire_date == 0) {
		Storage_info::get().add_file(name, true, expire_date, md5);
	} else {
		Storage_info::get().add_file(name, false, expire_date, md5);
	}

	store_data();
    return md5;
}

/**
* @brief Funkcja dodojąca plik z dysku do kolekcji. 
* W odróżnieniu od drugiej funkcji o tej nazwie,
* ta funkcja przyjmuje ścieżkę do pliku znajdującego się już na dysku, kopiuje go i dodaje
* do kolekcji.
* 
* @param src_path ścieżka do pliku, gdzie on obecnie się znajduje
* @param name nazwa pliku, pod którą będzie on dostępny
* @param local wskazduje, czy (wbrew swojej nazwie) węzeł jest właścicielem pliku
*
* @return skrót dodawanego pliku
*/
string Storage::add_file(string src_path, string name, bool local)
{
	auto dir = boost::filesystem::path(path);
	dir = boost::filesystem::canonical(dir);
	if (!boost::filesystem::exists(boost::filesystem::path(src_path))) {
		return "";
	}

	ifstream src(src_path, ios::binary);      
	stringstream file_content;
	file_content << src.rdbuf();
	string md5 = MD5(file_content.str()).hexdigest();
	auto file = boost::filesystem::path(name+"."+md5);
	auto list = Storage_info::get().file_info(name);
	for (auto file: list) {
		if (md5 == file.md5) {
			return md5;
		}
	}

	string dst_path = (dir/file).string<string>();
	ofstream dst(dst_path, ios::binary);       
	src.seekg(ios::beg);
	dst << src.rdbuf();

    File f;
    f.name = name;
	// TODO change owner node.
    f.isOwner = local;
	f.expire_date = 0;
	f.md5 = md5;
	f.local = true;

	Storage_info::get().mutex.lock();
	Storage_info::get().files.push_back(f);
	Storage_info::get().mutex.unlock();
	store_data();
	
	return md5;
}

/**
* @brief Funkcja kopiująca plik z kolekcji na dysk.
*
* @param name nazwa pliku do skopiowania
* @param dst_path ścieżka docelowa dla tego pliku
*
* @return false jeżeli taki plik nie jest dostępny i nie udało się go skopiować,
* true jeżeli operacja się udała
*/
bool Storage::copy_file(string name, string dst_path)
{
	auto file_list = Storage_info::get().file_info(name);
	auto dir = boost::filesystem::path(path);
	auto file = boost::filesystem::path(name+"."+file_list[file_list.size()-1].md5);

	dir = boost::filesystem::canonical(dir);
	
	if (!on_drive(name)) {
		return false;
	}

	string src_path = (dir/file).string<string>();
	ifstream src(src_path, ios::binary);      
	ofstream dst(dst_path, ios::binary);       

	dst << src.rdbuf();
	
	return true;
}

/**
* @brief Funkcja sprawdzająca czy taki plik istnieje.
*
* @param name nazwa pliku do sprawdzenia
* @param md5 skrót sprawdzanego pliku; jeżeli jest pustym stringiem, to jest ignorowany
*
* @return true jeżeli plik o podanej nazwie istnieje na dysku, false w przeciwnym wypadku
*/
bool Storage::on_drive(string name, string md5) 
{
	bool ignore = (md5 == "");
	for (File file: Storage_info::get().files) {
		if (file.name == name and (file.md5 == md5 or ignore)) {
			return true;
		}
	}

	return false;
}

/**
* @brief Funkcja dodająca część pliku. 
* Przy dodawaniu każdej części rozmiar pliku jest
* zwiększany (o ile to potrzebne), a obszar pomiędzy obecnym końcem pliku a początkiem
* dodawanych danych jest wypełniany zerami.
*
* @param data dane części pliku do zapisu
* @param part_size rozmiar dodawanej części pliku
* @param offset miejsce, od którego ma być dodawana obecna część
* @param name nazwa pliku, do którego dodawana jest część
*
* @return true jeżeli udało się dodać część pliku, false w przeciwnym wypadku
*/
bool Storage::add_file_part(const char * data, long long part_size, long long offset, string name) {
    for (File file : Storage_info::get().files) {
        if (file.name == name and file.local == true) {
            return false;
        } else if (file.name == name and file.local == false) {
            ofstream f((path + "/" + name).c_str(), ios_base::in | ios_base::binary);
            f.seekp(offset);
            f.write(data, part_size);
            f.close();

            return f.good();
        }  
    }

    cout << "bez in" << endl;
    ofstream f((path + "/" + name).c_str(), ios_base::binary);
    f.seekp(offset);
    f.write(data, part_size);
    f.close();

    File meta;
    meta.name = name;
    meta.isOwner = false;
    meta.local = false;

	Storage_info::get().files.push_back(meta);

    return f.good();
}

/**
* @brief Funkcja zamykająca plik, który był dodawany w częściach. 
* Taka funkcja jest potrzebna,
* gdyż trzeba zaznaczyć, że wszystkie części pliku zostały już dodane.
*
* @param name nazwa pliku
* @return true jeżeli taki plik istniał i nie był zamknięty, false w przeciwnym wypadku
*/
bool Storage::finish_file(string name) {
    for (File& file : Storage_info::get().files) {
        if (file.name == name and file.local == false) {
            file.local = true;
            return true;
        }
    }

    return false;
}

/**
* @brief Funkcja sprawdzająca czy plik dodawany w częściach jest już zakończony. 
* Taka funkcja jest potrzebna, gdyż może nastąpić odwołanie do pliku, który nie jest jeszcze kompletny.
*
* @param name nazwa pliku
* @return true jeżeli taki plik istniał i był zamknięty, false w przeciwnym wypadku
*/
bool Storage::is_finished(string name) {
    for (File file : Storage_info::get().files) {
        if (file.name == name) {
            return file.local;
        }
    }

    return false;
}


/**
* @brief Funkcja usuwająca plik z dysku.
*
* @param name nazwa pliku do usunięcia
* @param md5 skrót jednoznacznie identyfikujący plik
* @return true jeżeli taki plik istaniał i udało się go usunąć,
* false w przeciwnym wypadku
*/
bool Storage::remove_file(const string& name, const string& md5) {
	auto list = Storage_info::get().files;
    std::vector<File>::iterator iter = list.begin();

    while (iter != Storage_info::get().files.end()) {
        if (iter->name == name and iter->md5 == md5) {
            stringstream n;
            n << path << "/" << name << "." << list[list.size()-1].md5;
            remove(n.str().c_str());

			if (iter->isOwner) {
				iter = list.erase(iter);
			} else {
            	iter->local = false;
			}

			store_data();
            return true;
        } else {
            ++iter;
        }
    }

	store_data();
    return false;
}

/**
* @brief Funkcja odczytywująca dane z pliku. 
* 
* Po odczytaniu dane są dostępne w buforze
* dostępnym poprzez zwracaną strukturę.
*
* @param name nazwa pliku do odczytania
* @return wskaźnik na strukturę opisującą pobrany plik
*/
auto Storage::get_file(string name, string md5) -> shared_ptr<LoadedFile> {
	auto result = make_shared<LoadedFile>();
    for (File file : Storage_info::get().files) {
        if (file.name == name and file.md5 == md5) {
            ifstream f;
			f.open((path+"/"+name+"."+md5));

            f.seekg(0, ios::end);
            streamsize size_e = f.tellg();
            f.seekg(0, ios::beg);
			streamsize size_b = f.tellg();
			result->size = size_e - size_b;
			result->meta = file;
			result->data = new char[size_e - size_b];

            f.read(result->data, size_e - size_b);
			f.close();
			break;
		}
	}
	return result;
}

LoadedFile::LoadedFile(): size(0), data(nullptr)
{
	meta = File();
}

LoadedFile::~LoadedFile()
{
	if (data != nullptr) {
		delete[] data;
	}
}

/* int main() {

    Storage storage("/home/patrycja/studia/semestr6/dotin");
    string data = "dane testowe";
  cout << storage.add_file(data.c_str(), data.size(), "a", "");
    cout << storage.remove_file("a", "");
} */
