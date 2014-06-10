/*
 * Set of utilities used to communicate with server.
 */
#include <string>
#ifndef COMMAND_H_
#define COMMAND_H_

//! Narzędzia do bezpośredniej komunikacji z serwerem.
namespace communicate
{
	//! Wysłanie wiadomość bez konieczności czytania odpowiedzi.
	int call(std::string msg);
	//! Wysłanie wiadomości w celu przeczytania odpowiedzi.
	auto callForResponse(std::string msg) -> std::string;
}

#endif
