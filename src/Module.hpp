#ifndef MODULE_H
#define MODULE_H

#include <atomic>
#include <functional>
#include <memory>
#include <mutex>
#include <thread>


namespace ProjectMaya {
	/**
	 * Basic module, with fine-grained functionality, running independent from each other.
	 */
	template <typename TPayload>
	class Module {
		public:
			typedef std::function<TPayload*()> TPayloadCreator;

			explicit Module(TPayloadCreator pc) {
				this->phaseRun.lock();
				this->phaseCleanup.lock();
				this->readyInit.lock();
				this->readyRun.lock();
				this->started.store(false);
				this->shutdown.store(false);
				this->creator = pc;
				this->myThread = std::unique_ptr<std::thread>(new std::thread(std::bind(&Module::threadFunc, this)));
			}
			Module(const Module& obj) = delete;
			Module(Module&& obj) = delete;
			Module<TPayload>& operator=(const Module& obj) = delete;
			Module<TPayload>& operator=(Module&& obj) = delete;
			~Module(){
				std::lock_guard<std::recursive_mutex> memberGuard(this->memberMutex);

				// get phase relevant lock
				{
					std::lock_guard<std::mutex> readyGuard(this->readyInit);
				}

				// fire up phase
				this->phaseCleanup.unlock();

				// ensure shutdown
				this->stop();

				// end thread
				this->myThread->join();
			}

			TPayload& operator*() {
				return *this->payload;
			}

			bool shouldShutdown() {
				return this->shutdown.load();
			}

			void start() {
				std::lock_guard<std::recursive_mutex> memberGuard(this->memberMutex);

				if (!this->started.load()) {
					// get phase relevant lock
					{
						std::lock_guard<std::mutex> readyGuard(this->readyInit);
					}

					// fire up phase
					this->phaseRun.unlock();

					// save state
					this->started.store(true);
				}
			}

			void stop() {
				std::lock_guard<std::recursive_mutex> memberGuard(this->memberMutex);

				if (!this->shutdown.load()) {
					// get phase relevant locks
					{
						std::lock_guard<std::mutex> readyGuard(this->readyInit);
					}

					// signal shutdown
					this->payload->shutdown();

					// if not started, then do it
					this->start();

					// store state
					this->shutdown.store(true);
				}
			}

			void waitForConstructor() {
				std::lock_guard<std::mutex> phaseGuard(this->readyInit);
			}

			void waitForShutdown() {
				this->stop();
				std::lock_guard<std::mutex> phaseGuard(this->readyRun);
			}

			void join() {
				this->start();
				std::lock_guard<std::mutex> phaseGuard(this->readyRun);
			}

			bool wasStarted() const {
				return this->started.load();
			}

			bool wasStopped() const {
				return this->stopped.load();
			}


		private:
			TPayloadCreator creator;
			TPayload* payload = nullptr;

			std::recursive_mutex memberMutex;
			std::mutex phaseRun;
			std::mutex phaseCleanup;
			std::mutex readyInit;
			std::mutex readyRun;
			std::unique_ptr<std::thread> myThread;
			std::atomic<bool> started;
			std::atomic<bool> shutdown;

			void threadFunc() {
				this->payload = this->creator();
				this->readyInit.unlock();

				std::lock_guard<std::mutex> runGuard(this->phaseRun);
				if (!this->shutdown.load()) {
					(*this->payload)();
				}
				this->readyRun.unlock();

				std::lock_guard<std::mutex> cleanupGuard(this->phaseCleanup);
				delete this->payload;
			}
	};

	template <typename TPayload, typename... TParams>
	Module<TPayload>* createModule(TParams... params) {
		return new Module<TPayload>([&]{
				return new TPayload(params...);
		});
	}

}

#endif
