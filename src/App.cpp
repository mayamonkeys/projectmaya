#include "App.hpp"

using std::shared_ptr;

using namespace ProjectMaya;

/**
 * \warning Order of construction.
 */
App::App() {
	// async constructors
	this->mLogger = shared_ptr<Module<Logger>>(createModule<Logger>());
	this->mInteractionHandler = shared_ptr<Module<InteractionHandler>>(createModule<InteractionHandler>(this->mLogger));
	this->mUserInterface = shared_ptr<Module<UserInterface>>(createModule<UserInterface>(this->mInteractionHandler, this->mLogger));

	// wait
	this->mLogger->waitForConstructor();
	this->mLogger->waitForConstructor();
	this->mUserInterface->waitForConstructor();
}

/**
 * \warning Order of destruction.
 */
App::~App() {
	this->mUserInterface->stop();
	this->mInteractionHandler->stop();
	this->mLogger->stop();
}

/**
 * \warning Order of starting.
 */
void App::waitForShutdown() const {
	this->mLogger->start();
	this->mInteractionHandler->start();
	this->mUserInterface->start();
	this->mUserInterface->join();
}
