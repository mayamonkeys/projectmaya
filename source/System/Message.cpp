#include "Message.hpp"
#include "MessagePublicSlot.hpp"

using namespace ProjectMaya;

using std::shared_ptr;
using std::string;

Message::Message(string type) {
	this->type = type;
}

Message::~Message() {
}

string Message::getType() {
	return this->type;
}

bool Message::isType(string type) {
	return this->type == type;
}

void Message::setSource(MessagePublicSlot source) {
	this->source = shared_ptr<MessagePublicSlot>(new MessagePublicSlot(source));
}

MessagePublicSlot Message::getSource() {
	return *(this->source);
}

