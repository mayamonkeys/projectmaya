#include "MessagePublicSlot.hpp"
#include "MessageSlot.hpp"

using namespace ProjectMaya;

using std::lock_guard;
using std::mutex;
using std::shared_ptr;
using std::string;

bool MessagePublicSlot::Alive::startAccess() {
	bool result = false;

	// alive?
	if (this->alive.load()) {
		// start access
		if (this->accessCount.fetch_add(1) == 0) {
			this->destructorMutex.lock();
		}

		// still alive?
		if (this->alive.load()) {
			result = true;
		} else {
			this->stopAccess();
		}
	}

	return result;
}

void MessagePublicSlot::Alive::stopAccess() {
	if (this->accessCount.fetch_sub(1) == 1) {
		this->destructorMutex.unlock();
	}
}

MessagePublicSlot::MessagePublicSlot(MessageSlot* slot, shared_ptr<Alive> alive) {
	this->slot = slot;
	this->alive = alive;
}

bool MessagePublicSlot::isAlive() {
	return this->alive->alive.load();
}

void MessagePublicSlot::addTarget(MessagePublicSlot target) {
	if (this->alive->startAccess()) {
		this->slot->addTarget(target);
		this->alive->stopAccess();
	}
}

void MessagePublicSlot::drop(shared_ptr<Message> message) {
	if (this->alive->startAccess()) {
		this->slot->drop(message);
		this->alive->stopAccess();
	}
}

string MessagePublicSlot::getId() {
	string result("");
	if (this->alive->startAccess()) {
		result = this->slot->getId();
		this->alive->stopAccess();
	}
	return result;
}

string MessagePublicSlot::getGlobalName() {
	string result("");
	if (this->alive->startAccess()) {
		result = this->slot->getGlobalName();
		this->alive->stopAccess();
	}
	return result;
}

