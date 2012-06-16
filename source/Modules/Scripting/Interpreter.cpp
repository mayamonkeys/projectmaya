#include <chrono>
#include <iostream>
#include <stdexcept>
#include <sstream>
#include <string>
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
using std::string;
using std::stringstream;
using std::this_thread::sleep_for;


Interpreter::Interpreter() {

}

Interpreter::~Interpreter() {
	lua_close(luaState);
}

void Interpreter::operator()() {
	this->getMessageDriver()->getSlot("log")->emit(StringMessage("Initializing the Lua engine"));

	luaState = luaL_newstate();

	if(luaState == NULL)
		throw new runtime_error("could not initialize lua");

	/* load standard libraries */
	luaL_openlibs(luaState);

  /* setup public interfaces */
	exposeToState(luaState);


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
					string script = m2->getData();
					stringstream logmsg;

					// execute
					logmsg << "Exec: \"\"\"" << script << "\"\"\"";
					bool error = luaL_dostring(luaState, script.c_str());

					// get result
					logmsg << " (";
					if (error) {
						logmsg << lua_tostring(luaState, -1);
						lua_pop(luaState, 1);
					} else {
						logmsg << "ok";
					}
					logmsg << ")";

					// log
					this->getMessageDriver()->getSlot("log")->emit(StringMessage(logmsg.str()));
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

void Interpreter::exposeToState(lua_State* luaState) {
	// Register
	getGlobalNamespace(luaState)
		.beginClass<Interpreter>("Interpreter")
			.addFunction("reportSuccess", &Interpreter::reportSuccess)
			.addFunction("print", &Interpreter::print)
		.endClass();

	// Push concrete objects over
	push(luaState, this);
	lua_setglobal(luaState, "Interpreter");

	// Replace print in order to handle it outside of the Lua environment
	luaL_dostring(luaState, "function print(...) Interpreter:print(...) end");
}

void Interpreter::reportSuccess() {
	this->getMessageDriver()->getSlot("log")->emit(StringMessage("Successfully called method from Lua"));
}

int Interpreter::print() {
	int nArgs = lua_gettop(luaState);
	lua_getglobal(luaState, "tostring");
	stringstream stream;

	// dump all parameters
	// no idea, why to start by 2 :D
	for(int i = 2; i <= nArgs; i++) {
		const char *s;
		lua_pushvalue(luaState, -1);
		lua_pushvalue(luaState, i);
		lua_call(luaState, 1, 1);
		s = lua_tostring(luaState, -1);

		if(s == NULL) {
			return luaL_error(luaState, LUA_QL("tostring") " must return a string to ", LUA_QL("print"));
		}

		if(i > 2) {
			stream << "\t";
		}

		stream << s;
		lua_pop(luaState, 1);
	}

	this->getMessageDriver()->getSlot("log")->emit(StringMessage(stream.str()));

	return 0;
}

