#ifndef MESSAGESLOT_HPP
#define MESSAGESLOT_HPP

#include <forward_list>
#include <memory>
#include <string>
#include <queue>

#include "Message.hpp"
#include "MessagePublicSlot.hpp"

namespace ProjectMaya {

	/**
	 * Postbox which holds a message queue and manages his own message targets
	 */
	class MessageSlot {
		public:
			MessageSlot(std::string id);
			~MessageSlot();

			/// Returns public, always available slot interface
			MessagePublicSlot getPublicSlot();

			/// Add new target (used by emit)
			void addTarget(MessagePublicSlot target);

			/// Enqueue a new message
			void drop(std::shared_ptr<Message> message);

			/// Ask if queue holds messages, that should be processed
			bool hasMessages();

			/// Get first message from queue
			std::shared_ptr<Message> get();

			/// Get slot id
			std::string getId();

			/// Get global (unique) slot name
			std::string getGlobalName();

			/// Send/copy message to all registered targets
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

