#ifndef MESSAGESLOT_HPP
#define MESSAGESLOT_HPP

#include <forward_list>
#include <memory>
#include <string>
#include <queue>

#include "Message.hpp"
#include "MessagePublicSlot.hpp"

namespace ProjectMaya {

	class MessageSlot {
		public:
			MessageSlot(std::string id);
			~MessageSlot();

			MessagePublicSlot getPublicSlot();
			void addTarget(MessagePublicSlot target);
			void drop(Message message);
			bool hasMessages();
			Message get();
			void emit(Message message);

		private:
			std::mutex targetsMutex;
			std::mutex queueMutex;

			std::string id;
			std::shared_ptr<MessagePublicSlot::Alive> alive;
			std::forward_list<MessagePublicSlot> targets;
			std::queue<Message> messages;
	};

}

#endif

