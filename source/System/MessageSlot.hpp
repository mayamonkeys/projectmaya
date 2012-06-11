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
			void drop(std::shared_ptr<Message> message);
			bool hasMessages();
			std::shared_ptr<Message> get();
			std::string getId();
			std::string getGlobalName();

			template <class TMessage>
			void emit(TMessage message) {
				std::lock_guard<std::mutex> targetsGuard(this->targetsMutex);
				for (auto target : this->targets) {
					std::shared_ptr<TMessage> m(new TMessage(message));
					m->setSource(this->getPublicSlot());
					target.drop(m);
				}
			};

		private:
			std::mutex targetsMutex;
			std::mutex queueMutex;

			std::string id;
			std::shared_ptr<MessagePublicSlot::Alive> alive;
			std::forward_list<MessagePublicSlot> targets;
			std::queue<std::shared_ptr<Message>> messages;
	};

}

#endif

