#include <jsoncpp/json/json.h>
#include "client/net.h"
#include "client/command.h"

using namespace std;

/*!
 * Przekazuje do serwera polecenie pobrania żądanego pliku.
 * \param filename Nazwa pliku do pobrania.
 */
int client::download(string filename)
{
	download(filename, "");
	return 0;
}

/*
 *
 */
int client::download(string filename, string nodename)
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

/*!
 * Wysyła polecenie ponownego ściągnięcia listy
 * plików z sieci do serwera.
 */
int client::rescan()
{
	Json::Value root;
	Json::StyledWriter writer;

	root["type"] = "LocalRequest";
	root["name"] = "rescan";

	string json = writer.write(root);
	communicate::call(json);	

	return 0;
}

/*!
 * Pokazuje listę plików na serwerze.
 */
int client::showlist()
{
	Json::Value root;
	Json::Value files;
	Json::StyledWriter writer;
	Json::Reader reader;

	root["type"] = "LocalRequest";
	root["name"] = "filelist";

	string json = writer.write(root);
	string response = communicate::callForResponse(json);	
	
	reader.parse(response, root);
	files = root["files"];
	for (Json::Value file: files) {
		cout << file["file"].asString() << endl;
	}


	return 0;
}
