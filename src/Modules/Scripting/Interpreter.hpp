#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <memory>

#include <lua.hpp>

#include "System/Module.hpp"
#include "System/ModulePayload.hpp"


namespace ProjectMaya {

	/**
	 * Interpreter, used for accessing exposed functionality from inside the game.
	 */
	class Interpreter : public ModulePayload {
		public:
			explicit Interpreter(std::shared_ptr<Module> lg);
			~Interpreter();

			void operator()() override;

		private:
			std::shared_ptr<Module> lg;

			lua_State* luaState = NULL;

			/// Exposes specific functionality to the environment.
			void exposeToState(lua_State* luaState);

			// dummy function, to test the interpreter
			void reportSuccess() const ;
	};

}

#endif
