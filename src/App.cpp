#include "App.hpp"

using std::shared_ptr;

using namespace ProjectMaya;

App::App() {
	this->ui = shared_ptr<UserInterface>(new UserInterface());
}

void App::waitForShutdown() const {
	this->ui->join();
}
