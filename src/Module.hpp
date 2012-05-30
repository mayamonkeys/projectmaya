#ifndef MODULE_H
#define MOUDLE_H

#include <atomic>
#include <memory>
#include <mutex>
#include <thread>

using std::atomic;
using std::mutex;
using std::thread;
using std::unique_ptr;

namespace ProjectMaya {
	class Module {
		public:
			Module();
			~Module();

			void start();
			void stop();
			void join();

			bool wasStarted();
			bool wasStopped();

		protected:
			virtual void init() {};
			virtual void run() {};
			virtual void cleanup() {};
			bool shouldShutdown();

		private:
			mutex memberMutex;
			mutex phaseRun;
			mutex phaseCleanup;
			mutex readyInit;
			mutex readyRun;
			unique_ptr<thread> myThread;
			atomic<bool> started;
			atomic<bool> stopped;
			atomic<bool> shutdown;

			void threadFunc();
	};
}

#endif

