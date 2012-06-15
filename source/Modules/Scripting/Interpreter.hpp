#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <memory>

#include <lua.hpp>

#include "System/ModulePayload.hpp"


namespace ProjectMaya {

	/**
	 * Interpreter, used for accessing exposed functionality from inside the game.
	 */
	class Interpreter : public ModulePayload {
		public:
			explicit Interpreter();
			~Interpreter();

			void operator()() override;
			void setupMessageDriver(std::shared_ptr<MessageDriver> messageDriver, bool firstTime) override;

		private:
			lua_State* luaState = NULL;

			/// Exposes specific functionality to the environment.
			void exposeToState(lua_State* luaState);

			/// dummy function, to test the interpreter
			void reportSuccess();

			/// replace luas print function
			int print();
	};

}

#endif
