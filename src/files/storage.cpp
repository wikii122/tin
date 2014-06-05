#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <boost/filesystem.hpp>
#include <jsoncpp/json/json.h>
#include "files/storage.h"
#include "server.h"

using namespace std;

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
			cout << file_desc.name;
			file_desc.owner_name = file["owner"].asString();
			file_desc.complete = true;
			files.push_back(file_desc);

			Storage_info::get().add_file(file_desc.name, file_desc.owner_name);
		}
	}
}

Storage::~Storage() {
	auto dir_path = boost::filesystem::path(path);
	auto storage_path = boost::filesystem::path("files.json");
	storage_path = dir_path / storage_path;

	Json::StyledWriter writer;
	Json::Value file_list;

	file_list["files"] = Json::arrayValue;
	
	for (File file: files) {
		if (file.complete) {
			Json::Value file_desc;
			file_desc["name"] = file.name;
			file_desc["owner"] = file.owner_name;
			file_list["files"].append(file_desc);
		}
	}
	
	ofstream storage_file;
	storage_file.open(storage_path.c_str());
	string list = writer.write(file_list);	
	storage_file << list;
	storage_file.close();
}

bool Storage::add_file(const char* data, long size, string name, string owner_name) {

    ofstream file;

    for(long i = 0; i < size; i++) {
        stringstream n;
        n << path << "/" << name;

        file.open(n.str().c_str());
        file.clear();

        file.write(data + i, 1);

        if(!file.good())
            return false;
    }

    file.close();
    File f;
    f.name = name;
    f.owner_name = owner_name;

    files.push_back(f);
    return true;
}

bool Storage::add_file(string src_path, string name)
{
	// TODO prepare name
	auto dir = boost::filesystem::path(path);
	auto file = boost::filesystem::path(name);

	dir = boost::filesystem::canonical(dir);

	cout << (dir/file).c_str() << endl;
	if (!boost::filesystem::exists(boost::filesystem::path(src_path))) {
		return false;
	}
	string dst_path = (dir/file).string<string>();
	ifstream src(src_path, ios::binary);      
	ofstream dst(dst_path, ios::binary);       

	dst << src.rdbuf();

    File f;
    f.name = name;
	// TODO change owner node.
    f.owner_name = Server::get().get_name();
	f.complete = true;
    files.push_back(f);
	
	return true;
}

bool Storage::copy_file(string name, string dst_path)
{
	auto dir = boost::filesystem::path(path);
	auto file = boost::filesystem::path(name);

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

bool Storage::on_drive(string name) 
{
	for (File file: files) {
		if (file.name == name) {
			return true;
		}
	}

	return false;
}

bool Storage::add_file_part(const char * data, long part_size, long offset, string name, string owner_name) {
    for (File file : files) {
        if (file.name == name and file.complete == true) {
            return false;
        } else if (file.name == name and file.complete == false) {
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
    meta.owner_name = owner_name;
    meta.complete = false;

    files.push_back(meta);

    return f.good();
}

bool Storage::finish_file(string name) {
    for (File & file : files) {
        if (file.name == name and file.complete == false) {
            file.complete = true;
            return true;
        }
    }

    return false;
}

bool Storage::is_finished(string name) {
    for (File file : files) {
        if (file.name == name) {
            return file.complete;
        }
    }

    return false;
}

bool Storage::remove_file(const string& name) {

    std::vector<File>::iterator iter = files.begin();
    while (iter != files.end()) {
        if (iter->name == name) {
            stringstream n;
            n << path << "/" << name;
            remove(n.str().c_str());
            files.erase(iter);
            return true;
        } else {
            ++iter;
        }
    }

    return false;
}

shared_ptr<vector<char>> Storage::get_file(string name) {
    for (File file : files) {
        if (file.name == name) {
            ifstream f((path + "/" + name).c_str());

            f.seekg(0, ios::end);
            streamsize size = f.tellg();
            f.seekg(0, ios::beg);

            shared_ptr<vector<char>> buf(new vector<char>);
            buf->reserve(size);
            f.read(buf->data(), size);

            if (f.good()) {
                return buf;
            } else {
                shared_ptr<vector<char>>();
            }
        }
    }

    return shared_ptr<vector<char>>();
}

/* int main() {

    Storage storage("/home/patrycja/studia/semestr6/dotin");
    string data = "dane testowe";
    cout << storage.add_file(data.c_str(), data.size(), "a", "");
    cout << storage.remove_file("a", "");
} */
