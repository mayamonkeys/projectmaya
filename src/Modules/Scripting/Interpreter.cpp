#include <iostream>
#include <stdexcept>

/// \warning we have to include lua before luabridge
#include <lua.hpp>
#include <LuaBridge.h>

#include "Interpreter.hpp"
#include "Modules/Log/Logger.hpp"


using namespace ProjectMaya;
using namespace luabridge;

using std::runtime_error;
using std::shared_ptr;


Interpreter::Interpreter(shared_ptr<Module> lg) {
	this->lg = lg;
}

void Interpreter::operator()() {
	lg->get<Logger>().log("Interpreter", "Initializing the Lua engine");

	luaState = luaL_newstate();

	if(luaState == NULL)
		throw new runtime_error("could not initialize lua");

	/* load standard libraries */
	luaL_openlibs(luaState);

	exposeToState(luaState);

	lg->get<Logger>().log("Interpreter", "Lua engine successfully initialized");

	// do the test call
	luaL_dostring(luaState, "Interpreter:reportSuccess()");
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

void Interpreter::reportSuccess() const {
	lg->get<Logger>().log("Interpreter", "Successfully called method from Lua");
}
