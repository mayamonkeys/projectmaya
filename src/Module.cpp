#include <functional>

#include "Module.hpp"

using std::bind;
using std::lock_guard;
using std::mutex;
using std::atomic;
using std::thread;
using std::unique_ptr;

using namespace ProjectMaya;

Module::Module() {
	this->phaseRun.lock();
	this->phaseCleanup.lock();
	this->readyInit.lock();
	this->readyRun.lock();
	this->started.store(false);
	this->stopped.store(false);
	this->shutdown.store(false);
	this->myThread = unique_ptr<thread>(new thread(bind(&Module::threadFunc, this)));
}

Module::~Module() {
	this->stop();
}

void Module::start() {
	lock_guard<mutex> memberGuard(this->memberMutex);

	if (!this->started.load()) {
		// get phase relevant lock
		lock_guard<mutex> readyGuard(this->readyInit);

		// fire up phase
		this->phaseRun.unlock();

		this->started.store(true);
	}
}

void Module::stop() {
	lock_guard<mutex> memberGuard(this->memberMutex);

	if (!this->stopped.load()) {
		// get phase relevant locks
		lock_guard<mutex> readyGuard(this->readyInit);

		// signal shutdown
		this->shutdown.store(true);

		// fire up phase
		this->phaseCleanup.unlock();

		// if not started, then do it
		if (!this->started.load()) {
			this->phaseRun.unlock();
			this->started.store(true);
		}

		// wait for result
		this->myThread->join();

		this->stopped.store(true);
	}
}

void Module::join() {
	this->start();
	lock_guard<mutex> phaseGuard(this->readyRun);
}

void Module::threadFunc() {
	this->init();
	this->readyInit.unlock();

	lock_guard<mutex> runGuard(this->phaseRun);
	if (!this->phaseCleanup.try_lock()) {
		this->run();
		this->readyRun.unlock();

		lock_guard<mutex> cleanupGuard(this->phaseCleanup);
		this->cleanup();
	} else {
		this->cleanup();
		this->phaseCleanup.unlock();
	}
}

bool Module::wasStarted() const {
	return this->started.load();
}

bool Module::wasStopped() const {
	return this->stopped.load();
}

bool Module::shouldShutdown() const {
	return this->shutdown.load();
}
