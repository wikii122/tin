#include <jsoncpp/json/json.h>
#include <boost/filesystem.hpp>
#include "client/file.h"
#include "client/command.h"

using namespace std;

string absolute(string path);
string filename(string path);

/*!
 * Dodaje plik bez podanej nazwy
 *
 * \param path Ścieżka do pliku.
 */
int client::add(string path) 
{
	add(path, filename(path));
	return 0;
}

/*!
 * Wysyła do serwera polecenia dodania pliku z podanej
 * ścieżki pod podana nazwą.
 *
 * \param path Ścieżka do pliku.
 * \param filename Nazwa pliku na serwerze.
 */
int client::add(string path, string filename)
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

/*!
 * Wysyła polecenie skopiowania pliku ze Storage na dysk do obecnego
 * katalogu.
 *
 * \param filename Nazwa pliku na dysku.
 */
int client::get(string filename)
{
	get(filename, absolute(filename));
	return 0;
}

/*!
 * Wysyła polecenie skopiowania pliku ze Storage na dysk
 * do podanej ścieżki.
 *
 * \param filename Nazwa pliku.
 * \param path Ścieżka dla pliku.
 */
int client::get(string filename, string path)
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
/*!
 * Wysyła polecenie usunięcia pliku z serwera.
 *
 * \param filename Nazwa pliku do usunięcia.
 */
int client::remove(string filename)
{
	Json::Value root;
	Json::StyledWriter writer;

	root["type"] = "LocalRemove";
	root["name"] = filename;

	string json = writer.write(root);
	communicate::call(json);
	return 0;
}

/*!
 * Funkcja zwraca absolutną ścieżkę do pliku.
 * \param path Względna ścieżka dla pliku.
 * \return Absolutna ścieżka do pliku.
 */
string absolute(string path)
{
	auto rel_path = boost::filesystem::path(path);
	auto abs_path = boost::filesystem::complete(rel_path);
	return abs_path.string<string>();
}
/*!
 * Pozyskuje nazwę pliku ze ścieżki.
 * \param path Scieżka do pliku
 * \return Nazwa pliku.
 */
string filename(string path)
{
	auto rel_path = boost::filesystem::path(path);
	auto abs_path = boost::filesystem::complete(rel_path);
	return abs_path.filename().string<string>();
}
