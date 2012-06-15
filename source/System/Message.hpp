#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <memory>
#include <string>

namespace ProjectMaya{

	class MessagePublicSlot;

	/**
	 * Copyable object that will be send to/from message slot
	 */
	class Message {
		public:
			Message(std::string type);
			virtual ~Message();

			std::string getType();
			bool isType(std::string);

			void setSource(MessagePublicSlot source);
			MessagePublicSlot getSource();

		private:
			std::string type;
			std::shared_ptr<MessagePublicSlot> source;
	};

}

#endif

