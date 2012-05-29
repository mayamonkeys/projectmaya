#include <iostream>
#include <stdexcept>
#include <GL/glfw.h>
#include <config.h>
#include "gui.hpp"

Gui::Gui() {
	// init
	std::cout << "Init GLFW...";
	std::cout.flush();
	if (glfwInit() == GL_FALSE) {
		std::cerr << "Could not init GLFW!" << std::endl;
		this->valid = false;
		return;
	}
	std::cout << "ok" << std::endl;
}

Gui::~Gui() {
	if (this->valid) {
		// cleanup
		std::cout << "Shutdown...";
		std::cout.flush();
		glfwTerminate();
		std::cout << "ok" << std::endl;
	}
}

void Gui::run() {
	if (!this->valid) {
		throw std::runtime_error("Gui is not valid after internal error.");
	}
	
	// create window
	std::cout << "Create Window...";
	std::cout.flush();
	glfwOpenWindowHint(GLFW_WINDOW_NO_RESIZE, GL_TRUE);
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 2);
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 0);
	glfwSwapInterval(1);
	if (glfwOpenWindow(0,0,8,8,8,8,8,8,GLFW_WINDOW) == GL_FALSE) {
		std::cerr << "Could not open window!" << std::endl;
		this->valid = false;
		return;
	}
	glfwSetWindowTitle(NAME " - " REVISION);
	std::cout << "ok" << std::endl;

	std::cout << "run mainloop" << std::endl;
	this->running = true;
	while (this->running) {
		glClear(GL_COLOR_BUFFER_BIT);
		glfwSwapBuffers();
		this->running = !glfwGetKey(GLFW_KEY_ESC) && glfwGetWindowParam(GLFW_OPENED);
	}
}

bool Gui::isValid() {
	return this->valid;
}

bool Gui::isRunning() {
	return this->running;
}

