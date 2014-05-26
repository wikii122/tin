/*
 * All user interactions are made via this program.
 */
#include <iostream>
#include <boost/program_options.hpp>
#include "file.h"
#include "net.h"
#include "run.h"

namespace opt = boost::program_options;

using namespace std;

int main(int argc, char** argv)
{
	opt::options_description general("Options for TIN client");
	general.add_options()
		("command", opt::value<string>(), "command to execute")
		("subargs", opt::value<vector<string>>(), "additional arguments");

	opt::positional_options_description pos;
	pos.add("command", 1)
		.add("subargs", -1);
	
	opt::variables_map args;
	opt::parsed_options parsed = opt::command_line_parser(argc, argv)
		.options(general).positional(pos).allow_unregistered().run();

	opt::store(parsed, args);

	string command;
	try {
		command = args["command"].as<string>();
	} catch (const boost::bad_any_cast& e) {
		command = "help";
	}

	vector<string> arguments;
	try {	
		arguments = args["subargs"].as<vector<string>>();
	} catch (const boost::bad_any_cast& e) {
		// give empty string vector, already there.
	}
	
	// TODO make this more readable.
	if (command == "help") {
		cout << "This is a help message" << endl;
	} else if (command == "start") {
		if (!arguments.empty()) {
			server::start(arguments[0]);
		} else {
			// TODO unleash hell.
		}
	} else if (command == "stop") {
		if (arguments.empty()) {
			server::stop();	
		} // else 
	} else if (command == "restart") {
		if (!arguments.empty()) {
			server::restart(arguments[0]);
		} else {
			// TODO unleash hell
		}
	} else if (command == "add") {
		if (arguments.size() == 2) {
			server::add(arguments[0], arguments[1]);
		} else if (arguments.size() == 1) {
			server::add(arguments[0]);
		} else {
			// TODO unleash hell
		}
	} else if (command == "get") {
		if (arguments.size() == 2) {
			server::get(arguments[0], arguments[1]);
		} else if (arguments.size() == 1) {
			server::get(arguments[0]);
		} else {
			// TODO unleash hell
		}
	} else if (command == "download") {
		if (arguments.size() == 2) {
			server::download(arguments[0], arguments[1]);
		} else if (arguments.size() == 1) {
			server::download(arguments[0]);
		} else {
			// TODO unleash hell
		}
	} else if (command == "rescan") {
		if (arguments.empty()) {
			server::rescan();
		}
	} else if (command == "show-list") {
		if (arguments.empty()) {
			server::showlist();
		}
	} else if (command == "remove") {
		if (arguments.size() == 1) {
			server::remove(arguments[0]);
		}
	} else {
		cout << "This is a unknown command message" << endl;
	}
}


