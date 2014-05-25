#include <jsoncpp/json/json.h>
#include "net.h"
#include "command.h"

using namespace std;
int server::download(string filename)
{
	download(filename, "");
	return 0;
}

int server::download(string filename, string nodename)
{
	Json::Value root;
	Json::StyledWriter writer;

	root["type"] = "LocalDownload";
	root["file"] = filename;
	root["name"] = nodename;

	string json = writer.write(root);
	communicate::callForResponse(json);	

	return 0;
}

int server::rescan()
{
	Json::Value root;
	Json::StyledWriter writer;

	root["type"] = "LocalRequest";
	root["name"] = "rescan";

	string json = writer.write(root);
	communicate::call(json);	

	return 0;
}

int server::showlist()
{
	Json::Value root;
	Json::StyledWriter writer;

	root["type"] = "LocalRequest";
	root["name"] = "filelist";

	string json = writer.write(root);
	communicate::callForResponse(json);	

	return 0;
}
