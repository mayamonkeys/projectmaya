#include "ShutdownHelper.hpp"

using namespace ProjectMaya;

using std::atomic;

ShutdownHelper::ShutdownHelper() : shutdownState(false) {
}

void ShutdownHelper::shutdown() {
	this->shutdownState.store(true);
}

bool ShutdownHelper::shouldShutdown() const {
	return this->shutdownState.load();
}

