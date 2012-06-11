#include <chrono>
#include <iostream>
#include <thread>
#include <GL/glfw.h>

#include "CodeTables/UserEventsTable.hpp"
#include "MessageTypes/IntMessage.hpp"
#include "MessageTypes/StringMessage.hpp"
#include "InteractionHandler.hpp"

using namespace ProjectMaya;

using std::chrono::milliseconds;
using std::lock_guard;
using std::mutex;
using std::shared_ptr;
using std::this_thread::sleep_for;

InteractionHandler::InteractionHandler() {
}

void InteractionHandler::operator()() {
	milliseconds stime(20);
	shared_ptr<MessageSlot> keySlot = this->getMessageDriver()->getSlot("keys");

	while(!this->shouldShutdown()) {
		while(keySlot->hasMessages()) {
			shared_ptr<Message> m(keySlot->get());

			// check type
			if (m->isType("int")) {
				IntMessage* m2 = dynamic_cast<IntMessage*>(m.get());

				// security check
				if (m2 != nullptr) {
					int keyCode = m2->getData();
					bool pressed = true;

					// not pressed?
					if (keyCode < 0) {
						pressed = false;
						keyCode *= -1;
					}

					this->getMessageDriver()->getSlot("log")->emit(StringMessage("key pressed"));

					if (keyCode == GLFW_KEY_ESC) {
						this->getMessageDriver()->getSlot("userEvents")->emit(IntMessage(UserEventTable::EXIT));
					}
				}
			}
		}

		sleep_for(stime);
	}
}

void InteractionHandler::setupMessageDriver(shared_ptr<MessageDriver> messageDriver, bool firstTime) {
	ModulePayload::setupMessageDriver(messageDriver, firstTime);

	if (firstTime) {
		this->getMessageDriver()->createSlot("keys");
		this->getMessageDriver()->createSlot("log");
		this->getMessageDriver()->createSlot("userEvents");
	}
}

