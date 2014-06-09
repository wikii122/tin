#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <jsoncpp/json/json.h>
#include <sys/socket.h>
#include <sys/un.h>
#include "client/command.h"
#include "client/client.h"
	
using namespace std;

/*!
 * Funkcja ta wysyła wiadomość do serwera i ignoruje wynik, chyba
 * że zostanie napotkany błąd.
 * 
 * \param msg Wiadomość do wysłania.
 */
int communicate::call(string msg)
{
	callForResponse(msg);
	return 0;
}

/*!
 * Funkcja ta wysyła wiadomość do serwera i ignoruje wynik, chyba
 * że zostanie napotkany błąd.
 * 
 * \param msg Wiadomość do wysłania.
 * \return Odpowiedź od serwera.
 */
string communicate::callForResponse(string msg)
{	
	int socket_fd, state, len;
	struct sockaddr_un address;
	char buffer[1025];
	
	socket_fd = socket(PF_UNIX, SOCK_STREAM, 0);
	if (socket_fd == -1) {
		cout << "Could not create socket" << endl;;
		throw "Socket";
	}
	
	// Zero address structure
	memset(&address, 0, sizeof(struct sockaddr_un));
	address.sun_family = AF_UNIX;
	strcpy(address.sun_path, client::SOCKET_PATH);
	state = connect(socket_fd, (struct sockaddr *) &address, sizeof(struct sockaddr_un));
	
	if(state == -1) {
		cout << "Socket file does not exist...\nIs server running?" << endl;
		exit(1);	
	}
	
	len = msg.length();

	write(socket_fd, msg.c_str(), len);

	// TODO add timeout
	int guard = 0, i = 0;
	char sign;
	do {
		read(socket_fd, &sign, 1);
		if (sign == '{') guard++;
		else if (sign == '}') guard--;
		buffer[i++] = sign;
	} while (guard and i < 1025);

	buffer[i] = '\0';

	string response(buffer, buffer+i);
	Json::Reader reader;
	Json::Value value;
	reader.parse(response, value);
	if (value.get("display", false).asBool()) {
		cout << value.get("msg", "").asString() << endl;
	}	
	return response;
}
