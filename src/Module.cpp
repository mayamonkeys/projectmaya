#include <functional>

#include "Module.hpp"

using namespace ProjectMaya;

using std::bind;
using std::lock_guard;
using std::mutex;
using std::recursive_mutex;
using std::thread;
using std::unique_ptr;

Module::Module(const TPayloadCreator& pc) : creator(pc) {
	this->startThread();
}

Module::Module(TPayloadCreator&& pc) : creator(std::move(pc)) {
	this->startThread();
}

Module::~Module(){
	lock_guard<recursive_mutex> memberGuard(this->memberMutex);

	// get phase relevant lock
	{
		lock_guard<mutex> readyGuard(this->readyInit);
	}

	this->destroy();

	// end thread
	this->myThread->join();
}

bool Module::shouldShutdown() {
	return this->shutdown.load();
}

bool Module::shouldDestroy() {
	return this->destroyed.load();
}

void Module::start() {
	lock_guard<recursive_mutex> memberGuard(this->memberMutex);

	if (!this->started.load()) {
		// get phase relevant lock
		{
			lock_guard<mutex> readyGuard(this->readyInit);
		}

		// fire up phase
		this->phaseRun.unlock();

		// save state
		this->started.store(true);
	}
}

void Module::stop() {
	lock_guard<recursive_mutex> memberGuard(this->memberMutex);

	if (!this->shutdown.load()) {
		// get phase relevant locks
		{
			lock_guard<mutex> readyGuard(this->readyInit);
		}

		// signal shutdown
		this->payload->shutdown();

		// if not started, then do it
		this->start();

		// store state
		this->shutdown.store(true);
	}
}

void Module::destroy() {
	lock_guard<recursive_mutex> memberGuard(this->memberMutex);

	if (!this->destroyed.load()) {
		// get phase relevant locks
		{
			lock_guard<mutex> readyGuard(this->readyInit);
		}

		// fire up phase
		this->phaseCleanup.unlock();

		// ensure shutdown
		this->stop();

		// store state
		this->destroyed.store(true);
	}
}

void Module::waitForConstructor() {
	lock_guard<mutex> phaseGuard(this->readyInit);
}

void Module::waitForShutdown() {
	this->stop();
	lock_guard<mutex> phaseGuard(this->readyRun);
}

void Module::join() {
	this->start();
	lock_guard<mutex> phaseGuard(this->readyRun);
}

bool Module::wasStarted() const {
	return this->started.load();
}

void Module::threadFunc() {
	this->payload = this->creator();
	this->readyInit.unlock();

	lock_guard<mutex> runGuard(this->phaseRun);
	if (!this->shutdown.load()) {
		(*this->payload)();
	}
	this->readyRun.unlock();

	lock_guard<mutex> cleanupGuard(this->phaseCleanup);
	delete this->payload;
}

void Module::startThread() {
	this->phaseRun.lock();
	this->phaseCleanup.lock();
	this->readyInit.lock();
	this->readyRun.lock();
	this->started.store(false);
	this->shutdown.store(false);
	this->destroyed.store(false);
	this->myThread = unique_ptr<thread>(new thread(bind(&Module::threadFunc, this)));
}
