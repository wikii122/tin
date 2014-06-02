#include<vector>
#include<fstream>
#include<algorithm>
#include <iostream>
#include <sstream>
#include "storage.h"

using namespace std;

Storage::Storage(const string& path) : path(path) {}

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

/* int main() {

    Storage storage("/home/patrycja/studia/semestr6/dotin");
    string data = "dane testowe";
    cout << storage.add_file(data.c_str(), data.size(), "a", "");
    cout << storage.remove_file("a", "");
} */
