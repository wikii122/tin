#include <algorithm>
#include <fstream>
#include <mutex>
#include <string>
#include <vector>
#include "server.h"
#include "files/fileparts.h"
#include "files/storage.h"

using namespace std;

/*!
 * Tworzy nowy obiekt typu FilePartManager
 */
FilePartManager::FilePartManager()
{

}

FilePartManager::~FilePartManager()
{
	for (FilePart* part: parts) {
		delete part;
	}
}

/*!
 * Zwraca instancje singletona FilePartManager 
 * \return Referencje do instancji
 */
FilePartManager& FilePartManager::get()
{
	static FilePartManager instance;
	return instance;
}

/*!
 * Dodaje część do bufora na podanym offsecie.
 * Jeżeli bufor nie istniał, jest on tworzony.
 *
 * \param name Nazwa pliku
 * \param md5 hash pliku
 * \param buffer Bufor z którego dane należy dodać do pliku
 * \param size rozmiar części
 * \param offest pozycja części w pliku.
 */
int FilePartManager::add_part(string name, string md5, char* buffer, long long size, long long offset)
{
	FilePart& part = find(name, md5);
	part.add_part(buffer, size, offset);

	return 0;
}

/*!
 * Jesli plik ma żądany rozmiar, zakończa go i dodaje do
 * Storage. 
 * \param name Nazwa pliu
 * \param md5  Hash pliku
 * \param full_size docelowy rozmier pliku
 */
bool FilePartManager::finalize(string name, string md5, long long full_size)
{
	FilePart& part = find(name, md5);
	if (part.isFinished(full_size)) {
		part.close();
		for (auto it=parts.begin(); it!=parts.end(); it++) {
			if ((*it)->is(name, md5)) {
				delete *it;
				it = parts.erase(it);
				break;
			}
		}
	}

	return false;
}

/*!
 * Znajduje pierwszą niezarezerwowaną do ściągnięcia część pliku.
 * W przypadku, gdy plik jest zakończony zwraca rozmiar pliku.
 *
 * \param name Nazwa pliku
 * \param md5 Hash pliku
 * \return Offset pierwszej niezarezerwowanej wartości.
 */
long long FilePartManager::find_gap(string name, string md5)
{
	FilePart& part = find(name, md5);
	long long offset = part.first_gap();
	return offset;
}

/*! 
 * Rezerwuje część pliku do pobrania.
 * \param name Nazwa pliku
 * \param md5 Hash pliku
 * \param size Rozmiar rezerwowanej części
 * \param offset Offset rezerwowanej części (w bajtach)
 */
void FilePartManager::reserve(string name, string md5, long long size, long long offset)
{
	
	FilePart& part = find(name, md5);
	part.reserve(size, offset);
}

/*!
 * Znajduje żądany plik
 * \param name Nazwa pliku
 * \param md5 Hash pliku
 * \return Część pliku jako referencje na FilePartManager::FilePart
 */
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

/*!
 * Tworzy nowy obiekt FilePartManager::FilePart
 *
 * \param name Nazwa pliku
 * \param md5 Hash pliku
 */
FilePartManager::FilePart::FilePart(string name, string md5): size(0), name(name), md5(md5) 
{
	string file_name = "file_store/" + md5;		
	file.open(file_name, ios::binary);	
}

/*!
 * Usuwa plik stworzony, by trzymać część pliku.
 */
FilePartManager::FilePart::~FilePart()
{
	remove(("file_store/" + md5).c_str());
}

/*!
 * Sprawdza, czy obiekt jest szukanym.
 * \warning Nie można przeciążyć operatora ==, ponieważ klasę określają dwa atrybuty.
 * \param name Nazwa pliku
 * \param md5 Hash pliku
 * \return Wynik porównania
 */
bool FilePartManager::FilePart::is(string iname, string imd5)
{
	return iname == name and imd5 == md5;	
}

/*!
 * Sprawdza, czy plik jest ściągnięty cały.
 * \param isize Docelowy rozmiar całego pliku
 */
bool FilePartManager::FilePart::isFinished(long long isize)
{
	return size >= isize;
}

/*!
 * Dodaje podaną część do pliku.
 * \param buffer wskaźnik na zawartość do dodania.
 * \param part_size rozmiar części dodawanej
 * \param offset odległość w bajtach od początku pliku.
 */
void FilePartManager::FilePart::add_part(char* buffer, long long part_size, long long offset)
{
	mutex.lock();
	file.seekp(offset);	
	file.write(buffer, part_size);
	size += part_size;
	mutex.unlock();
}

/*!
 * Zamyka plik.
 * Kopiuje zawartość do Storage.
 */
void FilePartManager::FilePart::close()
{
	file.close();
	Server::get().get_storage().add_file("file_store/"+md5, name, false);	
}

/*!
 * Zwraca pierwszy niezarezerwowany fragment.
 * \return Offset brakującego fragmentu.
 */
long long FilePartManager::FilePart::first_gap()
{
	long long size = 0;
	for (pair<long long, long long> csize: part_sizes) {
		if (csize.first <= size) {
			long long diff = csize.second - (size - csize.first);
			size += diff>0? diff:0;
		} else {
			return size;
		}
	}
	return size;
}

/*!
 * Rezerwuje blok.
 * \param size Rozmiar bloku
 * \param offset pozycja bloku w pliku. 
 */
void FilePartManager::FilePart::reserve(long long size, long long offset)
{
	reserving.lock();
	part_sizes.push_back(make_pair(offset, size));
	sort(part_sizes.begin(), part_sizes.end());
	reserving.unlock();
}
