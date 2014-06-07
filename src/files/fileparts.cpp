#include <fstream>
#include <mutex>
#include <string>
#include <vector>
#include "files/fileparts.h"

using namespace std;

FilePartManager::FilePartManager()
{

}

FilePartManager::~FilePartManager()
{
	for (FilePart* part: parts) {
		delete part;
	}
}

FilePartManager& FilePartManager::get()
{
	static FilePartManager instance;
	return instance;
}

int FilePartManager::add_part(string name, string md5, char* buffer, long size, long offset)
{
	FilePart& part = find(name, md5);
	part.add_part(buffer, size, offset);

	return 0;
}

bool FilePartManager::finalize(string name, string md5, string owner_name, long full_size)
{
	FilePart& part = find(name, md5);
	if (part.isFinished(full_size)) {
		// TODO add file to storage
		part.close();
		for (auto it=parts.begin(); it!=parts.end(); it++) {
			if ((*it)->is(name, md5)) {
				if ((*it)->isFinished(full_size)) {
					delete *it;
					parts.erase(it);
				} else { 
					break;
				}
			}
		}
	}

	return false;
}

auto FilePartManager::find(string name, string md5) -> FilePart&
{
	for (FilePart* part: parts) {
		if (part->is(name, md5)) {
			return *part;
		}
	}
	FilePart* part = new FilePart(name, md5);
	parts.push_back(part);
	return *part;
}

FilePartManager::FilePart::FilePart(string name, string md5): size(0), name(name), md5(md5) 
{
	string file_name = "file_store/" + md5;		
	file.open(file_name, ios::binary);	
}

FilePartManager::FilePart::~FilePart()
{
	remove(("file_store/" + md5).c_str());
}

bool FilePartManager::FilePart::is(string iname, string imd5)
{
	return iname == name and imd5 == md5;	
}

bool FilePartManager::FilePart::isFinished(long isize)
{
	return size == 	isize;
}

void FilePartManager::FilePart::add_part(char* buffer, long part_size, long offset)
{
	mutex.lock();
	file.seekp(offset);	
	file.write(buffer, part_size);
	size += part_size;
	mutex.unlock();
}

void FilePartManager::FilePart::close()
{
	file.close();
}
