#include "App.hpp"
#include "ReusableThread.hpp"
#include <iostream>
#include <memory>

using std::shared_ptr;

using namespace ProjectMaya;

/**
 * \warning Order of construction.
 */
App::App() {
	// async constructors
	this->mLogger = shared_ptr<Module>(Module::create<Logger>());
	this->mInteractionHandler = shared_ptr<Module>(Module::create<InteractionHandler>(this->mLogger));
	this->mUserInterface = shared_ptr<Module>(Module::create<UserInterface>(this->mInteractionHandler, this->mLogger));

	// wait
	this->mLogger->waitForConstructor();
	this->mLogger->waitForConstructor();
	this->mUserInterface->waitForConstructor();
}

/**
 * \warning Order of destruction.
 */
App::~App() {
	// destroy
}

/**
 * \warning Order of starting.
 */
void App::waitForShutdown() const {
	// start
	this->mLogger->start();
	this->mInteractionHandler->start();
	this->mUserInterface->start();

	// run and wait for ui to shutdown
	this->mUserInterface->join();

	// async shutdown
	this->mUserInterface->stop();
	this->mInteractionHandler->stop();
	this->mLogger->stop();

	// wait
	this->mUserInterface->waitForShutdown();
	this->mInteractionHandler->waitForShutdown();
	this->mLogger->waitForShutdown();

	// async destroy (destructors will wait later)
	this->mUserInterface->destroy();
	this->mInteractionHandler->destroy();
	this->mLogger->destroy();
}
