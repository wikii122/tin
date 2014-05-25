#include <jsoncpp/json/json.h>
#include <boost/filesystem.hpp>
#include "file.h"
#include "command.h"

using namespace std;

string absolute(string path);
string filename(string path);

int server::add(string path) 
{
	add(path, filename(path));
	return 0;
}
int server::add(string path, string filename)
{

	Json::Value root;
	Json::StyledWriter writer;

	root["type"] = "LocalFileAdd";
	root["file"] = absolute(path); 
	root["name"] = filename;

	string json = writer.write(root);
	communicate::call(json);

	return 0;
}
int server::get(string filename)
{
	get(filename, absolute(filename));
	return 0;
}
int server::get(string filename, string path)
{
	Json::Value root;
	Json::StyledWriter writer;

	root["type"] = "LocalFileGet";
	root["file"] = absolute(path);
	root["name"] = filename;

	string json = writer.write(root);
	communicate::call(json);
	return 0;
}
int server::remove(string filename)
{
	Json::Value root;
	Json::StyledWriter writer;

	root["type"] = "LocalRemove";
	root["name"] = filename;

	string json = writer.write(root);
	communicate::call(json);
	return 0;
}

string absolute(string path)
{
	auto rel_path = boost::filesystem::path(path);
	auto abs_path = boost::filesystem::complete(rel_path);
	return abs_path.string<string>();
}
string filename(string path)
{
	auto rel_path = boost::filesystem::path(path);
	auto abs_path = boost::filesystem::complete(rel_path);
	return abs_path.filename().string<string>();
}
