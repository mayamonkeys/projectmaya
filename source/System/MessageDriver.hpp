#ifndef MESSAGEDRIVER_HPP
#define MESSAGEDRIVER_HPP

#include <memory>
#include <mutex>
#include <string>
#include <unordered_map>

#include "MessageSlot.hpp"

namespace ProjectMaya {

	class MessageDriver {
		public:
			std::shared_ptr<MessageSlot> createSlot(std::string id);
			std::shared_ptr<MessageSlot> getSlot(std::string id);

		private:
			std::mutex mapMutex;
			std::unordered_map<std::string, std::shared_ptr<MessageSlot>> slots;
	};

}

#endif

