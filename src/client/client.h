/*!
 * Stałe do użytku klienta
 */
#ifndef CLIENT_H_
#define CLIENT_H_

//! Funkcje używane do komunikacji pomiędzy klientem a serwerem.
namespace client {
	//! Scieżka do współdzielonego pliku z gniazdem
	static const char SOCKET_PATH[] = "/tmp/tin.socket";
}

#endif
