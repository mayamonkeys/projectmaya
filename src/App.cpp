#include "App.hpp"

using std::shared_ptr;

using namespace ProjectMaya;

/**
 * \warning Order of construction.
 */
App::App() {
	this->lg = shared_ptr<Logger>(new Logger);
	this->ih = shared_ptr<InteractionHandler>(new InteractionHandler(this->lg));
	this->ui = shared_ptr<UserInterface>(new UserInterface(this->ih, this->lg));
}

/**
 * \warning Order of destruction.
 */
App::~App() {
	ui->stop();
	ih->stop();
	lg->stop();
}

/**
 * \warning Order of starting.
 */
void App::waitForShutdown() const {
	this->lg->start();
	this->ih->start();
	this->ui->start();
	this->ui->join();
}
