#ifndef STRINGMESSAGE_HPP
#define STRINGMESSAGE_HPP

#include <string>

#include "System/Message.hpp"

namespace ProjectMaya {

	class StringMessage : public Message {
		public:
			StringMessage(std::string data);
			std::string getData();

		private:
			std::string data;
	};

}

#endif

