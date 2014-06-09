#include <fstream>
#include <mutex>
#include <string>
#include <vector>

/*!
 * \brief Klasa zarządzająca częściami plików
 *
 * Z racji że pliki przychodzą w częściach, trzeba je gdzieś przechowywać
 * Takimi fragmentami zarządza ta klasa.
 */
class FilePartManager {
	class FilePart;
	//! Towrzy obiekt klasy
	FilePartManager();
	std::vector<FilePart*> parts;
	//! Wyszukuje plik wśród fragmentów.
	FilePart& find(std::string, std::string);
public:
	//! Kopiowanie zablokowane
	FilePartManager(const FilePartManager&)=delete;
	//! Zwalnia pamięć po usunięciu instancji.
	~FilePartManager();
	//! Zwraca instancje klasy
	static FilePartManager& get();
	//! Dodaje część do pliku.
	int add_part(std::string name, std::string md5, char* buffer,  long long size, long long offset);
	//! Zakończa plik.
	bool finalize(std::string name, std::string md5, long long full_size);
	//! Zwraca pierwszą niezarezerwowaną część pliku.
	long long find_gap(std::string name, std::string md5);
	//! Rezerwuje część pliku.
	void reserve(std::string name, std::string md5, long long size, long long offset);
private:
	/*!
	 * \brief Reprezentacja częściowo ściągniętego pliku.
	 */
	class FilePart {
		friend class FilePartManager;
		long long size;
		std::vector<std::pair<long long, long long>> part_sizes;
		std::mutex mutex;
		std::mutex reserving;
		std::ofstream file;
		std::string name;
		std::string md5;
		//! Zamyka plik i zwraca go do Storage
		void close();
	public:
		//! Tworzy nową część pliku.
		FilePart(std::string, std::string);
		FilePart(const FilePart&)=delete;
		~FilePart();
		//! Sprawdzanie tożsamości
		bool is(std::string name, std::string md5);
		//! Dodaje zawartość bufora do pliku.
		void add_part(char* buffer, long long size, long long offset);
		//! Sprawdza, czy cały rozmiar pliku został dodany.
		bool isFinished(long long size);
		//! Zwraca pierwszy niezarezerowne fragment.
		long long first_gap();
		//! Rezerwuje fragment
		void reserve(long long size, long long offset);
	};
};

