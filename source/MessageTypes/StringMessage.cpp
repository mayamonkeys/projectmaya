#include "StringMessage.hpp"

using namespace ProjectMaya;

using std::string;

StringMessage::StringMessage(string data) : Message("string") {
	this->data = data;
}

string StringMessage::getData() {
	return this->data;
}

