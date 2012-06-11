#include <functional>
#include <utility>

#include "Module.hpp"

using namespace ProjectMaya;

using std::bind;
using std::lock_guard;
using std::mutex;
using std::recursive_mutex;
using std::shared_ptr;
using std::string;
using std::thread;
using std::unique_ptr;

Module::Module(const TPayloadCreator& pc) {
	this->helper = shared_ptr<THelper>(new THelper);
	this->helper->creator = pc;
	this->startThread();
}

Module::Module(TPayloadCreator&& pc) {
	this->helper = shared_ptr<THelper>(new THelper);
	this->helper->creator = std::move(pc);
	this->startThread();
}

Module::Module(Module&& obj) {
	lock_guard<recursive_mutex> memberGuard(obj.memberMutex);

	// create dummy modul
	this->helper = shared_ptr<THelper>(new THelper);
	this->helper->dummy = true;
	this->startThread();

	// synchronize payload pointer to avoid crash of get
	this->helper->payload = obj.helper->payload;

	// swap
	std::swap(this->helper, obj.helper);
	std::swap(this->myThread, obj.myThread);
}

Module::~Module(){
	lock_guard<recursive_mutex> memberGuard(this->memberMutex);

	// get phase relevant lock
	{
		lock_guard<mutex> readyGuard(this->helper->readyInit);
	}

	this->destroy();

	// end thread
	this->myThread->join();
}

MessagePublicSlot Module::getMessageSlot(string id) {
	lock_guard<recursive_mutex> memberGuard(this->memberMutex); // optional?
	return this->helper->messageDriver->getSlot(id)->getPublicSlot();
}

bool Module::shouldShutdown() {
	return this->helper->shutdown.load();
}

bool Module::shouldDestroy() {
	return this->helper->destroyed.load();
}

void Module::start() {
	lock_guard<recursive_mutex> memberGuard(this->memberMutex);

	if (!this->helper->started.load()) {
		// get phase relevant lock
		{
			lock_guard<mutex> readyGuard(this->helper->readyInit);
		}

		// fire up phase
		this->helper->phaseRun.unlock();

		// save state
		this->helper->started.store(true);
	}
}

void Module::stop() {
	lock_guard<recursive_mutex> memberGuard(this->memberMutex);

	if (!this->helper->shutdown.load()) {
		// get phase relevant locks
		{
			lock_guard<mutex> readyGuard(this->helper->readyInit);
		}

		// signal shutdown
		this->helper->payload->shutdown();

		// if not started, then do it
		this->start();

		// store state
		this->helper->shutdown.store(true);
	}
}

void Module::destroy() {
	lock_guard<recursive_mutex> memberGuard(this->memberMutex);

	if (!this->helper->destroyed.load()) {
		// get phase relevant locks
		{
			lock_guard<mutex> readyGuard(this->helper->readyInit);
		}

		// fire up phase
		this->helper->phaseCleanup.unlock();

		// ensure shutdown
		this->stop();

		// store state
		this->helper->destroyed.store(true);
	}
}

void Module::waitForConstructor() {
	lock_guard<mutex> phaseGuard(this->helper->readyInit);
}

void Module::waitForShutdown() {
	this->stop();
	lock_guard<mutex> phaseGuard(this->helper->readyRun);
}

void Module::join() {
	this->start();
	lock_guard<mutex> phaseGuard(this->helper->readyRun);
}

bool Module::wasStarted() const {
	return this->helper->started.load();
}

void Module::threadFunc(shared_ptr<THelper> helper) {
	if (!helper->dummy) {
		helper->payload = helper->creator();
		helper->payload->setupMessageDriver(helper->messageDriver, true);
	}
	helper->readyInit.unlock();

	lock_guard<mutex> runGuard(helper->phaseRun);
	if (!helper->shutdown.load() && !helper->dummy) {
		try {
			(*helper->payload)();
		} catch(...) {
			/* nop */
		}
	}
	helper->readyRun.unlock();

	lock_guard<mutex> cleanupGuard(helper->phaseCleanup);
	if (!helper->dummy) {
		delete helper->payload;
	}
}

void Module::startThread() {
	// create message system
	this->helper->messageDriver = shared_ptr<MessageDriver>(new MessageDriver());

	// prepare helper
	this->helper->phaseRun.lock();
	this->helper->phaseCleanup.lock();
	this->helper->readyInit.lock();
	this->helper->readyRun.lock();
	this->helper->started.store(false);
	this->helper->shutdown.store(false);
	this->helper->destroyed.store(false);
	this->myThread = unique_ptr<thread>(new thread(bind(&Module::threadFunc, this->helper)));
}
