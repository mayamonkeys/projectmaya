#include <chrono>
#include <iostream>
#include <thread>
#include <GL/glfw.h>

#include "InteractionHandler.hpp"

using namespace ProjectMaya;

using std::chrono::milliseconds;
using std::lock_guard;
using std::mutex;
using std::this_thread::sleep_for;

void InteractionHandler::run() {
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

			// do something
			std::cout << "Key " << event.keyCode << " " << event.pressed << std::endl;
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

