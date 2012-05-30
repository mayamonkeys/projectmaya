#ifndef MODULE_H
#define MOUDLE_H

#include <atomic>
#include <memory>
#include <mutex>
#include <thread>


namespace ProjectMaya {

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
			virtual void init(){};
			virtual void run(){};
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
