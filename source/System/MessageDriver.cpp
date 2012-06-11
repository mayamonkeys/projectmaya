#include "MessageDriver.hpp"

using namespace ProjectMaya;

using std::lock_guard;
using std::mutex;
using std::shared_ptr;
using std::string;

shared_ptr<MessageSlot> MessageDriver::createSlot(string id) {
	lock_guard<mutex> mapGuard(this->mapMutex);

	shared_ptr<MessageSlot> slot(new MessageSlot(id));
	this->slots[id] = slot;

	return slot;
}

shared_ptr<MessageSlot> MessageDriver::getSlot(string id) {
	lock_guard<mutex> mapGuard(this->mapMutex);

	return this->slots.at(id);
}

