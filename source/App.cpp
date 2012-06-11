#include "App.hpp"
#include "Modules/Interaction/InteractionHandler.hpp"
#include "Modules/Scripting/Interpreter.hpp"
#include "Modules/Log/Logger.hpp"
#include "Modules/Sound/SoundHandler.hpp"
#include "Modules/GUI/UserInterface.hpp"

using std::shared_ptr;

using namespace ProjectMaya;

/**
 * \warning Order of construction.
 */
App::App() {
	// async constructors
	this->mLogger = shared_ptr<Module>(Module::create<Logger>());
	this->mInterpreter = shared_ptr<Module>(Module::create<Interpreter>());
	this->mInteractionHandler = shared_ptr<Module>(Module::create<InteractionHandler>());
	this->mSoundHandler = shared_ptr<Module>(Module::create<SoundHandler>());
	this->mUserInterface = shared_ptr<Module>(Module::create<UserInterface>());

	// wait
	this->mLogger->waitForConstructor();
	this->mInteractionHandler->waitForConstructor();
	this->mSoundHandler->waitForConstructor();
	this->mUserInterface->waitForConstructor();

	// link modules
	MessagePublicSlot logTarget(this->mLogger->getMessageSlot("defaultLog"));
	this->mInterpreter->getMessageSlot("log").addTarget(logTarget);
	this->mInteractionHandler->getMessageSlot("log").addTarget(logTarget);
	this->mSoundHandler->getMessageSlot("log").addTarget(logTarget);
	this->mUserInterface->getMessageSlot("log").addTarget(logTarget);

	this->mUserInterface->getMessageSlot("keyEvents").addTarget(this->mInteractionHandler->getMessageSlot("keys"));

	this->mInteractionHandler->getMessageSlot("userEvents").addTarget(this->mUserInterface->getMessageSlot("user"));
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
	this->mInterpreter->start();
	this->mInteractionHandler->start();
	this->mSoundHandler->start();
	this->mUserInterface->start();

	// run and wait for ui to shutdown
	this->mUserInterface->join();

	// async shutdown
	this->mUserInterface->stop();
	this->mSoundHandler->stop();
	this->mInteractionHandler->stop();
	this->mInterpreter->stop();
	this->mLogger->stop();

	// wait
	this->mUserInterface->waitForShutdown();
	this->mSoundHandler->waitForShutdown();
	this->mInteractionHandler->waitForShutdown();
	this->mInterpreter->waitForShutdown();
	this->mLogger->waitForShutdown();

	// async destroy (destructors will wait later)
	this->mUserInterface->destroy();
	this->mSoundHandler->destroy();
	this->mInteractionHandler->destroy();
	this->mInterpreter->destroy();
	this->mLogger->destroy();
}
