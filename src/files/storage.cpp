#include<vector>
#include<fstream>
#include<algorithm>
#include <iostream>
#include <sstream>

using namespace std;

struct File {
 
	string name;
	string id;

};

class Storage {
public:
	Storage(const string& path) : path(path) {}
	

	bool add_file(const char* data, long size, string name, string id) {
	
		ofstream file;

		for( long i = 0; i < size; i++) {
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
		f.id = id;

		files.push_back(f);
		return true;
	}


	bool remove_file(const string& name, string id){

		std::vector<File>::iterator iter = files.begin();
			while (iter != files.end()) {
				
				if (iter->name == name) {
					stringstream n;
					n << path << "/" << name;

					remove(n.str().c_str());
					files.erase(iter);
					return true;
				}
				else {
					++iter;

				}

			}
		return false;
	}

private:
	string path;
	vector<File> files;
};

int main() {

	Storage storage("/home/patrycja/studia/semestr6/dotin");
	string data = "dane testowe";
	cout << storage.add_file(data.c_str(), data.size(), "a", "");
	cout << storage.remove_file("a", "");
}
