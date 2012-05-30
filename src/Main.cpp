#include <iostream>

#include "App.hpp"

using std::cout;
using std::endl;

using namespace ProjectMaya;


int main() {
	cout << "constructing a new user interface" << endl;

	App app;
	app.waitForShutdown();

	cout << "user interface destroyed, shutting down" << endl;
}
