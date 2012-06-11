#include "MessageSlot.hpp"

using namespace ProjectMaya;

using std::lock_guard;
using std::mutex;
using std::shared_ptr;
using std::string;

MessageSlot::MessageSlot(string id) {
	this->id = id;
	this->alive = shared_ptr<MessagePublicSlot::Alive>(new MessagePublicSlot::Alive);
	this->alive->state = true;
}

MessageSlot::~MessageSlot() {
	lock_guard<mutex> aliveGuard(this->alive->stateMutex);
	this->alive->state = false;
}

MessagePublicSlot MessageSlot::getPublicSlot() {
	return MessagePublicSlot(this, this->alive);
}

void MessageSlot::addTarget(MessagePublicSlot target) {
	lock_guard<mutex> targetsGuard(this->targetsMutex);
	this->targets.push_front(target);
}

void MessageSlot::drop(Message message) {
	lock_guard<mutex> queueGuard(this->queueMutex);
	this->messages.push(message);
}

bool MessageSlot::hasMessages() {
	lock_guard<mutex> queueGuard(this->queueMutex);
	return !this->messages.empty();
}

Message MessageSlot::get() {
	lock_guard<mutex> queueGuard(this->queueMutex);
	Message m = this->messages.front();
	this->messages.pop();
	return m;
}

void MessageSlot::emit(Message message) {
	lock_guard<mutex> targetsGuard(this->targetsMutex);
	for (auto target : this->targets) {
		Message m(message);
		m.setSource(this->getPublicSlot());
		target.drop(m);
	}
}


