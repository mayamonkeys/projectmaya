#include <chrono>
#include <iostream>
#include <stdexcept>
#include <thread>

/// \warning we have to include lua before luabridge
#include <lua.hpp>
#include <LuaBridge.h>

#include "MessageTypes/StringMessage.hpp"
#include "Interpreter.hpp"


using namespace ProjectMaya;
using namespace luabridge;

using std::chrono::milliseconds;
using std::runtime_error;
using std::shared_ptr;
using std::this_thread::sleep_for;

Interpreter::Interpreter() {
}

void Interpreter::operator()() {
	this->getMessageDriver()->getSlot("log")->emit(StringMessage("Initializing the Lua engine"));

	luaState = luaL_newstate();

	if(luaState == NULL)
		throw new runtime_error("could not initialize lua");

	/* load standard libraries */
	luaL_openlibs(luaState);

	exposeToState(luaState);

	this->getMessageDriver()->getSlot("log")->emit(StringMessage("Lua engine successfully initialized"));

	// listen to events
	shared_ptr<MessageSlot> execSlot = this->getMessageDriver()->getSlot("exec");
	milliseconds stime(200);
	while (!this->shouldShutdown()) {
		shared_ptr<Message> m;
		while ((m = execSlot->get()).get() != nullptr) {

			// check type
			if (m->isType("string")) {
				StringMessage* m2 = dynamic_cast<StringMessage*>(m.get());

				// security check
				if (m2 != nullptr) {
					this->getMessageDriver()->getSlot("log")->emit(StringMessage(m2->getData()));
					luaL_dostring(luaState, m2->getData().c_str());
				}
			}
		}

		sleep_for(stime);
	}
}

void Interpreter::setupMessageDriver(shared_ptr<MessageDriver> messageDriver, bool firstTime) {
	ModulePayload::setupMessageDriver(messageDriver, firstTime);

	if (firstTime) {
		this->getMessageDriver()->createSlot("log");
		this->getMessageDriver()->createSlot("exec");
	}
}

Interpreter::~Interpreter() {
	lua_close(luaState);
}

void Interpreter::exposeToState(lua_State* luaState) {
	// Register
	getGlobalNamespace(luaState)
		.beginClass<Interpreter>("Interpreter")
			.addFunction("reportSuccess", &Interpreter::reportSuccess)
		.endClass();

	//Push concrete objects over
	push(luaState, this);
	lua_setglobal(luaState, "Interpreter");
}

void Interpreter::reportSuccess() {
	this->getMessageDriver()->getSlot("log")->emit(StringMessage("Successfully called method from Lua"));
}
