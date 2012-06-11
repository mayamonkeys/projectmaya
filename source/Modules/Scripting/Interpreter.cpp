#include <iostream>
#include <stdexcept>

/// \warning we have to include lua before luabridge
#include <lua.hpp>
#include <LuaBridge.h>

#include "MessageTypes/StringMessage.hpp"
#include "Interpreter.hpp"


using namespace ProjectMaya;
using namespace luabridge;

using std::runtime_error;
using std::shared_ptr;


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

	// do the test call
	luaL_dostring(luaState, "Interpreter:reportSuccess()");
}

void Interpreter::setupMessageDriver(shared_ptr<MessageDriver> messageDriver, bool firstTime) {
	ModulePayload::setupMessageDriver(messageDriver, firstTime);

	if (firstTime) {
		this->getMessageDriver()->createSlot("log");
	}
}

Interpreter::~Interpreter() {
	lua_close(luaState);
}

void Interpreter::exposeToState(lua_State* luaState) {
	// Register
	getGlobalNamespace(luaState)
		.beginClass<Interpreter>("Interpreter")
			.addMethod("reportSuccess", &Interpreter::reportSuccess)
		.endClass();

	//Push concrete objects over
	push(luaState, this);
	lua_setglobal(luaState, "Interpreter");
}

void Interpreter::reportSuccess() {
	this->getMessageDriver()->getSlot("log")->emit(StringMessage("Successfully called method from Lua"));
}
