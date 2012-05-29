#include <cstdlib>
#include "gui.hpp"

int main() {
	Gui gui;
	
	if (gui.isValid()) {
		gui.run();
	} else {
		exit(EXIT_FAILURE);
	}
	
	return EXIT_SUCCESS;
}
