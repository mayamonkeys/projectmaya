#include "IntMessage.hpp"

using namespace ProjectMaya;

IntMessage::IntMessage(int data) : Message("int") {
	this->data = data;
}

int IntMessage::getData() {
	return this->data;
}

