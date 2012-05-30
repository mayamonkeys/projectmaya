#include "App.hpp"

using std::shared_ptr;

using namespace ProjectMaya;

App::App() {
	this->ih = shared_ptr<InteractionHandler>(new InteractionHandler);
	this->ui = shared_ptr<UserInterface>(new UserInterface(this->ih));
}

void App::waitForShutdown() const {
	this->ih->start();
	this->ui->start();
	this->ui->join();
}
