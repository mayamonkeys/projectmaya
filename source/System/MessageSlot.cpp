#include "MessageSlot.hpp"

#include <functional>
#include <sstream>

using namespace ProjectMaya;

using std::hash;
using std::lock_guard;
using std::mutex;
using std::shared_ptr;
using std::string;
using std::stringstream;

MessageSlot::MessageSlot(string id) {
	this->id = id;
	this->alive = shared_ptr<MessagePublicSlot::Alive>(new MessagePublicSlot::Alive);
	this->alive->alive.store(true);
	this->alive->destructorMutex.unlock();
	this->alive->accessCount.store(0);
}

MessageSlot::~MessageSlot() {
	this->alive->alive.store(false);
	this->alive->destructorMutex.lock();
}

MessagePublicSlot MessageSlot::getPublicSlot() {
	return MessagePublicSlot(this, this->alive);
}

void MessageSlot::addTarget(MessagePublicSlot target) {
	lock_guard<mutex> targetsGuard(this->targetsMutex);
	this->targets.push_front(target);
}

void MessageSlot::drop(shared_ptr<Message> message) {
	this->messages.enqueue(message);
}

shared_ptr<Message> MessageSlot::get() {
	return this->messages.dequeue();
}

string MessageSlot::getId() {
	return this->id;
}

string MessageSlot::getGlobalName() {
	hash<void*> hashFunc;
	stringstream stream;
	stream << hashFunc(this) << "|" << this->id;
	return stream.str();
}

