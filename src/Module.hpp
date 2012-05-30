#ifndef MODULE_H
#define MODULE_H

#include <atomic>
#include <memory>
#include <mutex>
#include <thread>


namespace ProjectMaya {

	/**
	 * Basic module, with fine-grained functionality, running independent from each other.
	 */
	class Module {
		public:
			explicit Module();
			virtual ~Module();

			void start();
			void stop();
			void join();

			bool wasStarted() const;
			bool wasStopped() const;

		protected:
			/// Initializes the module.
			virtual void init(){};
			/// Runs the module specific service.
			virtual void run(){};
			/// Cleans the module specific garbage.
			virtual void cleanup(){};
			bool shouldShutdown() const;

		private:
			std::mutex memberMutex;
			std::mutex phaseRun;
			std::mutex phaseCleanup;
			std::mutex readyInit;
			std::mutex readyRun;
			std::unique_ptr<std::thread> myThread;
			std::atomic<bool> started;
			std::atomic<bool> stopped;
			std::atomic<bool> shutdown;

			void threadFunc();
	};

}

#endif
