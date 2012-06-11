#include <chrono>
#include <iostream>
#include <thread>
#include <GL/glfw.h>

#include "MessageTypes/StringMessage.hpp"
#include "InteractionHandler.hpp"

using namespace ProjectMaya;

using std::chrono::milliseconds;
using std::lock_guard;
using std::mutex;
using std::shared_ptr;
using std::this_thread::sleep_for;

InteractionHandler::InteractionHandler(shared_ptr<Module> lg) {
	this->lg = lg;
}

void InteractionHandler::operator()() {
	milliseconds stime(20);

	while(!this->shouldShutdown()) {
		// get queue status
		bool empty;
		{
			lock_guard<mutex> memberGuard(this->memberMutex);
			empty = this->keyQueue.empty();
		}

		while(!empty) {
			// get event
			KeyEvent event;
			{
				lock_guard<mutex> memberGuard(this->memberMutex);
				event = this->keyQueue.front();
				this->keyQueue.pop();
			}

			/// \todo do something with the information
			this->getMessageDriver()->getSlot("log")->emit(StringMessage("key pressed"));

			if (event.keyCode == GLFW_KEY_ESC) {
				this->exit = true;
			}

			// get new queue status
			{
				lock_guard<mutex> memberGuard(this->memberMutex);
				empty = this->keyQueue.empty();
			}
		}

		sleep_for(stime);
	}
}

void InteractionHandler::setupMessageDriver(shared_ptr<MessageDriver> messageDriver, bool firstTime) {
	ModulePayload::setupMessageDriver(messageDriver, firstTime);

	if (firstTime) {
		this->getMessageDriver()->createSlot("log");
	}
}

void InteractionHandler::newKeyEvent(bool pressed, int keyCode) {
	lock_guard<mutex> memberGuard(this->memberMutex);

	// create event
	KeyEvent event;
	event.pressed = pressed;
	event.keyCode = keyCode;

	// add event to queue
	this->keyQueue.push(event);
}

bool InteractionHandler::exitRequested() {
	lock_guard<mutex> memberGuard(this->memberMutex);
	return this->exit;
}
