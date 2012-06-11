#include <ctime>
#include <iomanip>
#include <chrono>
#include <iostream>
#include <sstream>
#include <thread>

#include "Logger.hpp"
#include "MessageTypes/IntMessage.hpp"
#include "MessageTypes/StringMessage.hpp"

using namespace ProjectMaya;

using std::chrono::milliseconds;
using std::chrono::system_clock;
using std::clog;
using std::endl;
using std::string;
using std::stringstream;
using std::localtime;
using std::lock_guard;
using std::mutex;
using std::noskipws;
using std::put_time;
using std::shared_ptr;
using std::this_thread::sleep_for;

void Logger::operator()() {
	milliseconds stime(20);
	shared_ptr<MessageSlot> defaultSlot = this->getMessageDriver()->getSlot("defaultLog");

	while(!this->shouldShutdown()) {
		while(defaultSlot->hasMessages()) {
			shared_ptr<Message> m = defaultSlot->get();
			string msg("unkown message");

			// check type
			if (m->isType("string")) {
				StringMessage* m2 = dynamic_cast<StringMessage*>(m.get());

				// security check (fails if cast was not successful)
				if (m2 != nullptr) {
					msg = m2->getData();
				}
			} else if (m->isType("int")) {
				IntMessage* m2 = dynamic_cast<IntMessage*>(m.get());

				// security check
				if (m2 != nullptr) {
					stringstream stream;
					stream << "Event (id="<< m2->getData() << ")";
					msg = stream.str();
				}
			}

			// formatting: timestamp and message
			auto now = system_clock::to_time_t(system_clock::now());
			clog << "[" << put_time(localtime(&now), "%H:%M:%S") << "] " << m->getSource().getGlobalName() << ": " << msg << endl;
		}

		sleep_for(stime);
	}
}

void Logger::setupMessageDriver(shared_ptr<MessageDriver> messageDriver, bool firstTime) {
	ModulePayload::setupMessageDriver(messageDriver, firstTime);

	if (firstTime) {
		this->getMessageDriver()->createSlot("defaultLog");
	}
}

