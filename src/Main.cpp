#include <iostream>

#include "UserInterface.hpp"

using std::cout;
using std::endl;

using namespace ProjectMaya;


int main() {
	cout << "constructing a new user interface" << endl;

	{
		/* e.g. construct the user interface, do other initialization */
		UserInterface ui;
	}

	cout << "user interface destroyed, shutting down" << endl;
}
