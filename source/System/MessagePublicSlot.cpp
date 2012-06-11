#include "MessagePublicSlot.hpp"
#include "MessageSlot.hpp"

using namespace ProjectMaya;

using std::lock_guard;
using std::mutex;
using std::shared_ptr;
using std::string;

MessagePublicSlot::MessagePublicSlot(MessageSlot* slot, shared_ptr<Alive> alive) {
	this->slot = slot;
	this->alive = alive;
}

bool MessagePublicSlot::isAlive() {
	lock_guard<mutex> aliveGuard(this->alive->stateMutex);
	return this->alive->state;
}

void MessagePublicSlot::addTarget(MessagePublicSlot target) {
	lock_guard<mutex> aliveGuard(this->alive->stateMutex);
	if (this->alive->state) {
		this->slot->addTarget(target);
	}
}

void MessagePublicSlot::drop(shared_ptr<Message> message) {
	lock_guard<mutex> aliveGuard(this->alive->stateMutex);
	if (this->alive->state) {
		this->slot->drop(message);
	}
}

