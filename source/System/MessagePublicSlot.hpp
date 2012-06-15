#ifndef MESSAGEPUBLICSLOT_HPP
#define MESSAGEPUBLICSLOT_HPP

#include <memory>
#include <mutex>
#include <string>

#include "Message.hpp"

namespace ProjectMaya {

	class MessageSlot;

	/**
	 * Public, thread safe message slot
	 *
	 * Also accessible (without effect) if origin was deleted
	 */
	class MessagePublicSlot {
		public:
			struct Alive {
				std::mutex stateMutex;
				bool state;
			};

			MessagePublicSlot(MessageSlot* slot, std::shared_ptr<Alive> alive);

			bool isAlive();
			void addTarget(MessagePublicSlot target);
			void drop(std::shared_ptr<Message> message);
			std::string getId();
			std::string getGlobalName();

		private:
			MessageSlot* slot;
			std::shared_ptr<Alive> alive;
	};

}

#endif

