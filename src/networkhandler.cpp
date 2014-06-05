#include "networkhandler.h"

NetworkHandler::NetworkHandler()
{
}

NetworkHandler::~NetworkHandler()
{
}

int NetworkHandler::handle()
{
	return 0;
}

auto NetworkHandler::read() -> std::string
{
	return "";
}

int NetworkHandler::write(std::string)
{
	return 0;
}