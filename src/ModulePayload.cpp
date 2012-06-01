#include "ModulePayload.hpp"

using namespace ProjectMaya;

using std::atomic;

ModulePayload::ModulePayload() : shutdownState(false) {
}

ModulePayload::~ModulePayload(){
}

void ModulePayload::shutdown() {
	this->shutdownState.store(true);
}

bool ModulePayload::shouldShutdown() const {
	return this->shutdownState.load();
}
