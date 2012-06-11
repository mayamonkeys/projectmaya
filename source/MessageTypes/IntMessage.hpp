#ifndef INTMESSAGE_HPP
#define INTMESSAGE_HPP

#include "System/Message.hpp"

namespace ProjectMaya {

	class IntMessage : public Message {
		public:
			IntMessage(int data);
			int getData();

		private:
			int data;
	};

}

#endif

