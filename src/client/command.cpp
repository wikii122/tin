#include <iostream>
#include "command.h"
	
using namespace std;

/*
 * This function calls server and ignores input,
 * unless an error happens
 */
//TODO do something meaningful
int communicate::call(string msg)
{
	cout << msg << endl;
	return 0;
}

/*
 * This function calls server and returns response.
 */
string communicate::callForResponse(string msg)
{
	cout << msg << endl;
	return "";
}
