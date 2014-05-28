/*
 * Declares client's file operations.
 */
#include <string>
#ifndef FILE_H_
#define FILE_H_

namespace client 
{
		int add(std::string path);
		int add(std::string path, std::string filename);
		int get(std::string filename);
		int get(std::string filename, std::string path);
		int remove(std::string filename);
}

#endif
