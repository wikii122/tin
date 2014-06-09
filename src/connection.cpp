#include "connection.h"
#include "server.h"
#include "files/fileparts.h"

#include <sys/types.h> 
#include <sys/socket.h>
#include <cstring>
#include <iostream>


Connection::Connection()
{
	buffer = new char[10240];
	state = ConnectionState::Intro;
	transferred = 0;
}

void Connection::handle()
{
	if (incoming)
		handleIncoming();
	else
		handleOutgoing();
}

void Connection::handleIncoming()
{
	int complete = 0;
	int oldTransferred;
	long long delta;
	char buf[1024];
	switch(state) {
	case ConnectionState::Intro:
		oldTransferred = transferred;
		delta = recv(socket, buf, 1024, 0);
		if (delta == -1 && errno != EAGAIN && errno != EWOULDBLOCK)
			throw std::string("ConnectionHandler::handleOutgoing: Could not recv");

		transferred += delta;

		memcpy(buffer+oldTransferred, buf, transferred-oldTransferred);
		for(int i = 0; i < transferred; ++i)
			if(buffer[i] == '\n')
				complete++;

		if(complete >= 5) {
			int i;
			for(i=0; i < transferred; i++)
			{
				if(buffer[i] == '\n') break;
				name += buffer[i];
			}
			i++;
			for(; i < transferred; i++)
			{
				if(buffer[i] == '\n') break;
				md5 += buffer[i];
			}
			i++;
			size = *((long long*)(buffer+i));
			expiry = *((long long*)(buffer+1+sizeof(long long)+i));
			original = *((bool*)(buffer+i+2+2*sizeof(long long)));

			std::cout << "Receiving: " << name << std::endl << "MD5: " << md5 << std::endl << "Size: " << size << std::endl << "Expiry:" << expiry << std::endl << "isOriginal:" << original << std::endl;

			Storage& storage = Server::get().get_storage();
			std::shared_ptr<LoadedFile> file = storage.get_file(name, md5);
			fileData = file->data + offset;
			transferred = 0;

			state = ConnectionState::PartInfo;
		}
		break;
	case ConnectionState::PartInfo:
		offset = FilePartManager::get().find_gap(name, md5);
		if (size - offset < 10240)
			partSize = size - offset;
		else
			partSize = 10240;
		FilePartManager::get().reserve(name, md5, partSize, offset);

		send(socket, (void*)&offset, sizeof(offset), 0);
		send(socket, "\n", 1, 0);
		send(socket, (void*)&partSize, sizeof(partSize), 0);
		send(socket, "\n", 1, 0);

		transferred = 0;
		state = ConnectionState::Data;

		break;

	case ConnectionState::Data:
		//std::cout << "Buffer: " << std::hex << (void*)buffer << std::endl << "Transferred: " << transferred << std::dec << std::endl;
		delta = recv(socket, buffer+transferred, 1024, 0);
		if (delta == -1 && errno != EAGAIN && errno != EWOULDBLOCK)
			throw std::string("ConnectionHandler::handleIncoming: Could not recv");
		transferred += delta;

		std::cout << "Downloaded " << 100*transferred/partSize << "% of a part, " << 100*transferred/size << "% of the file" << std::endl;

		if(transferred >= partSize)
		{
			FilePartManager::get().add_part(name, md5, buffer, partSize, offset);
			if (FilePartManager::get().find_gap(name, md5) >= size) {
				std::cout << "Finished downloading " << name << std::endl;

				FilePartManager::get().finalize(name, md5, size);

				std::cout << "Setting ownership to " << original << std::endl;
				Storage_info::get().set_ownership(name, md5, original);
				std::cout << "Ownership set to " << original << std::endl;

				state = ConnectionState::Finished;
			} else {
				transferred = 0;
				state = ConnectionState::PartInfo;
			}
		}

		break;

	case ConnectionState::Finished:
		break;
	}
}

void Connection::handleOutgoing()
{
	int complete = 0;
	int delta;
	int oldTransferred;
	char buf[1024];
	int toUpload;
	switch(state) {
	case ConnectionState::Intro:
		send(socket, (void*)name.c_str(), name.length(), 0);
		send(socket, "\n", 1, 0);
		send(socket, (void*)md5.c_str(), md5.length(), 0);
		send(socket, "\n", 1, 0);
		send(socket, (void*)&size, 8, 0);
		send(socket, "\n", 1, 0);
		send(socket, (void*)&expiry, 8, 0);
		send(socket, "\n", 1, 0);
		send(socket, (void*)&original, 1, 0);
		send(socket, "\n", 1, 0);

		transferred = 0;
		state = ConnectionState::PartInfo;

		break;
	case ConnectionState::PartInfo:
		oldTransferred = transferred;
		delta = recv(socket, buf, 1024, 0);
		if (delta == -1 && errno != EAGAIN && errno != EWOULDBLOCK) {
			std::cout << "ERRNO: " << errno << std::endl;
			throw std::string("ConnectionHandler::handleOutgoing: Could not recv");
		}

		transferred += delta;

		memcpy(buffer+oldTransferred, buf, transferred-oldTransferred);

		for(int i = 0; i < transferred; ++i)
			if(buffer[i] == '\n')
				complete++;

		if(complete >= 2) {
			offset = *((long long*)(buffer));
			partSize = *((long long*)(buffer+1+sizeof(long long)));

			std::cout << "Sending: " << name << std::endl << "Offset: " << offset << std::endl << "partSize: " << partSize << std::endl << "isOriginal: " << original << std::endl;

			Storage& storage = Server::get().get_storage();
			std::shared_ptr<LoadedFile> file = storage.get_file(name, md5);
			fileData = file->data + offset;

			complete = 0;
			transferred = 0;

			state = ConnectionState::Data;
		}
		break;
	case ConnectionState::Data:
		toUpload = 10240 < partSize - transferred ? 10240 : partSize - transferred;
		if (send(socket, (void*)(fileData+transferred), toUpload, 0) == -1 && errno != EAGAIN && errno != EWOULDBLOCK)
			throw std::string("ConnectionHandler::handleOutgoing: Could not send");

		transferred += toUpload;

		std::cout << "Uploaded " << 100*transferred/partSize << "% of a part, " << 100*transferred/size << "% of the file" << std::endl;

		if(transferred >= partSize) {
			transferred = 0;
			state = ConnectionState::PartInfo;
		}
		break;

	case ConnectionState::Finished:
		break;
	}
}