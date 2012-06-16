#include <chrono>
#include <iostream>
#include <sstream>
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
using std::stringstream;
using std::this_thread::sleep_for;


InteractionHandler::InteractionHandler() {

}

InteractionHandler::~InteractionHandler() {

}

void InteractionHandler::operator()() {
	milliseconds stime(20);
	shared_ptr<MessageSlot> keySlot = this->getMessageDriver()->getSlot("keys");
	bool scriptInput = false;
	stringstream script;

	while(!this->shouldShutdown()) {
		shared_ptr<Message> m;
		while((m = keySlot->get()).get() != nullptr) {
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

					if (keyCode == GLFW_KEY_ESC) {
						this->getMessageDriver()->getSlot("userEvents")->emit(IntMessage(UserEventTable::EXIT));
					}

					if ((keyCode == GLFW_KEY_F12) && pressed) {
						if (scriptInput) {
							scriptInput = false;
							this->getMessageDriver()->getSlot("userEvents")->emit(IntMessage(UserEventTable::CONSOLE_OPEN));
						} else {
							scriptInput = true;
							script.str("");
							this->getMessageDriver()->getSlot("userEvents")->emit(IntMessage(UserEventTable::CONSOLE_CLOSE));
						}
					}

					if ((keyCode == GLFW_KEY_ENTER) && pressed && scriptInput) {
						this->getMessageDriver()->getSlot("exec")->emit(StringMessage(script.str()));
						script.str("");
					}
				}
			} else if (m->isType("string")) {
				StringMessage* m2 = dynamic_cast<StringMessage*>(m.get());

				// security check
				if (m2 != nullptr) {
					if (scriptInput) {
						script << m2->getData();
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
		this->getMessageDriver()->createSlot("chars");
		this->getMessageDriver()->createSlot("exec");
	}
}

