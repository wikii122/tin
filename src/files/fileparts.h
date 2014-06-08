#include <fstream>
#include <mutex>
#include <string>
#include <vector>

class FilePartManager {
	class FilePart;
	FilePartManager();
	std::vector<FilePart*> parts;
	FilePart& find(std::string, std::string);
public:
	FilePartManager(const FilePartManager&)=delete;
	~FilePartManager();
	static FilePartManager& get();
	int add_part(std::string name, std::string md5, char* buffer,  long size, long offset);
	bool finalize(std::string name, std::string md5, long full_size);
	long find_gap(std::string name, std::string md5);
	void reserve(std::string name, std::string md5, long size, long offset);
private:
	class FilePart {
		friend class FilePartManager;
		long size;
		std::vector<std::pair<long, long>> part_sizes;
		std::mutex mutex;
		std::mutex reserving;
		std::ofstream file;
		std::string name;
		std::string md5;
		
		void close();
	public:
		FilePart(std::string, std::string);
		FilePart(const FilePart&)=delete;
		~FilePart();
		bool is(std::string name, std::string md5);
		void add_part(char* buffer, long size, long offset);
		bool isFinished(long size);
		long first_gap();
		void reserve(long size, long offset);
	};
};

