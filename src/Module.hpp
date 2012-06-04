#ifndef MODULE_H
#define MODULE_H

#include <atomic>
#include <memory>
#include <mutex>
#include <thread>

#include "ModulePayload.hpp"

namespace ProjectMaya {

	/**
	 * Basic module, with fine-grained functionality, running independent from each other.
	 */
	class Module {
		public:
			typedef std::function<ModulePayload*()> TPayloadCreator;

			template <typename TPayload, typename... TParams>
			static Module* create(TParams... params) {
				return new Module([=]{
						return new TPayload(params...);
				});
			}

			explicit Module(const TPayloadCreator& pc);
			explicit Module(TPayloadCreator&& pc);
			~Module();

			Module(const Module& obj) = delete;
			Module(Module&& obj) = delete;
			Module& operator=(const Module& obj) = delete;
			Module& operator=(Module&& obj) = delete;

			template <typename TPayload>
			TPayload& get() {
				return static_cast<TPayload&>(*this->payload);
			}

			bool wasStarted() const;
			bool shouldShutdown();
			bool shouldDestroy();

			void start();
			void stop();
			void destroy();

			void waitForConstructor();
			void join();
			void waitForShutdown();


		private:
			TPayloadCreator creator;
			ModulePayload* payload = nullptr;

			std::recursive_mutex memberMutex;
			std::mutex phaseRun;
			std::mutex phaseCleanup;
			std::mutex readyInit;
			std::mutex readyRun;
			std::unique_ptr<std::thread> myThread;
			std::atomic<bool> started;
			std::atomic<bool> shutdown;
			std::atomic<bool> destroyed;

			void threadFunc();
			void startThread();
	};

}

#endif
