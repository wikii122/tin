#include <memory>
#include <string>
#include "handler.h"

class FileHandler: Handler
{
public:
	FileHandler();
	~FileHandler();
	
	virtual int handle() override;
	virtual auto read() -> std::string override; // Reads text to first \n
	virtual int write(std::string) override;
	virtual int read(char* buffer, int size); // Reads data chunk to buffer, should be virtual?
};
