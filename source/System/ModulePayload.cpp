#include "ModulePayload.hpp"

using namespace ProjectMaya;

using std::atomic;
using std::shared_ptr;

ModulePayload::ModulePayload() : shutdownState(false) {
}

ModulePayload::~ModulePayload(){
}

void ModulePayload::setupMessageDriver(shared_ptr<MessageDriver> messageDriver, bool firstTime) {
	this->messageDriver = messageDriver;

	// ignore firstTime parameter since we do not add slots in the generic payload
	(void)firstTime;
}

void ModulePayload::operator()() {
}

void ModulePayload::shutdown() {
	this->shutdownState.store(true);
}

bool ModulePayload::shouldShutdown() const {
	return this->shutdownState.load();
}

shared_ptr<MessageDriver> ModulePayload::getMessageDriver() {
	return this->messageDriver;
}

