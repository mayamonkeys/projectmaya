#include "ReusableThread.hpp"

using namespace ProjectMaya;

using std::bind;
using std::lock_guard;
using std::mutex;
using std::shared_ptr;
using std::terminate;
using std::thread;
using std::unique_ptr;

ReusableThread::ReusableThread() {
	this->helper = shared_ptr<THelper>(new THelper);
	this->helper->shutdown.store(false);
	this->helper->payload = []{};
	this->helper->stepBeginMutex.lock();
	this->helper->stepEndMutex.lock();
	this->t = unique_ptr<thread>(new thread(bind(&ReusableThread::threadFunc, this->helper)));

	// wait until dummy payload is loaded
	this->helper->stepBeginMutex.lock();
}

ReusableThread::ReusableThread(ReusableThread&& obj) {
	lock_guard<mutex> objMemberGuard(obj.memberMutex);

	// create dummy objects to swap
	this->helper = shared_ptr<THelper>(new THelper);
	this->helper->stepEndMutex.lock();
	this->t = unique_ptr<thread>(new thread([]{}));

	// swap
	std::swap(this->helper, obj.helper);
	std::swap(this->t, obj.t);
}

ReusableThread::~ReusableThread() {
	lock_guard<mutex> memberGuard(this->memberMutex);
	if (this->t->joinable()) {
		this->helper->shutdown.store(true);
		this->helper->stepEndMutex.unlock();
		this->t->join();
	}
}

ReusableThread& ReusableThread::operator=(ReusableThread&& obj) {
	lock_guard<mutex> memberGuard(this->memberMutex);
	lock_guard<mutex> objMemberGuard(obj.memberMutex);
	if (this->t->joinable()) {
		terminate();
	}

	std::swap(this->helper, obj.helper);
	std::swap(this->t, obj.t);

	return *this;
}

void ReusableThread::join() {
	lock_guard<mutex> memberGuard(this->memberMutex);
	if (this->t->joinable()) {
		lock_guard<mutex> runGuard(this->helper->runMutex);
	}
}

void ReusableThread::detach() {
	lock_guard<mutex> memberGuard(this->memberMutex);
	if (this->t->joinable()) {
		this->t->detach();
	}
}

bool ReusableThread::joinable() {
	return this->t->joinable();
}

thread::id ReusableThread::get_id() const {
	return this->t->get_id();
}

void ReusableThread::threadFunc(std::shared_ptr<THelper> helper) {
	while(!helper->shutdown.load()) {
		{
			lock_guard<mutex> runGuard(helper->runMutex);
			helper->stepBeginMutex.unlock();
			try {
				helper->payload();
			} catch(...) {
				/* nop */
			}
		}
		helper->stepEndMutex.lock();
	}
}

void ReusableThread::swap(ReusableThread& obj) {
	lock_guard<mutex> memberGuard(this->memberMutex);
	lock_guard<mutex> objMemberGuard(obj.memberMutex);

	std::swap(this->helper, obj.helper);
	std::swap(this->t, obj.t);
}
