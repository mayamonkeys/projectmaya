#include "App.hpp"

using namespace ProjectMaya;

App::App() {
	this->ui = shared_ptr<UserInterface>(new UserInterface());
}

void App::waitForShutdown() {
	this->ui->join();
}

