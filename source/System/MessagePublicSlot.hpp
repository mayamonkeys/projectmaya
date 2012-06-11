#ifndef MESSAGEPUBLICSLOT_HPP
#define MESSAGEPUBLICSLOT_HPP

#include <memory>
#include <mutex>
#include <string>

#include "Message.hpp"

namespace ProjectMaya {

	class MessageSlot;

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

		private:
			MessageSlot* slot;
			std::shared_ptr<Alive> alive;
	};

}

#endif

