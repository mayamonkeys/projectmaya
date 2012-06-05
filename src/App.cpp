#include "App.hpp"
#include "InteractionHandler.hpp"
#include "Logger.hpp"
#include "SoundHandler.hpp"
#include "UserInterface.hpp"
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
	this->mSoundHandler = shared_ptr<Module>(Module::create<SoundHandler>(this->mLogger));
	this->mUserInterface = shared_ptr<Module>(Module::create<UserInterface>(this->mInteractionHandler, this->mLogger));

	// wait
	this->mLogger->waitForConstructor();
	this->mInteractionHandler->waitForConstructor();
	this->mSoundHandler->waitForConstructor();
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
	this->mSoundHandler->start();
	this->mUserInterface->start();

	// run and wait for ui to shutdown
	this->mUserInterface->join();

	// async shutdown
	this->mUserInterface->stop();
	this->mSoundHandler->stop();
	this->mInteractionHandler->stop();
	this->mLogger->stop();

	// wait
	this->mUserInterface->waitForShutdown();
	this->mSoundHandler->waitForShutdown();
	this->mInteractionHandler->waitForShutdown();
	this->mLogger->waitForShutdown();

	// async destroy (destructors will wait later)
	this->mUserInterface->destroy();
	this->mSoundHandler->destroy();
	this->mInteractionHandler->destroy();
	this->mLogger->destroy();
}
