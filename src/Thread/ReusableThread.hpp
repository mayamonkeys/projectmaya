#ifndef REUSABLETHREAD_H
#define REUSABLETHREAD_H

#include <atomic>
#include <functional>
#include <memory>
#include <thread>

namespace ProjectMaya {

	class ReusableThread {
		public:
			ReusableThread();
			~ReusableThread();
			ReusableThread(const ReusableThread& obj) = delete;
			ReusableThread(ReusableThread&& obj);
			ReusableThread& operator=(const ReusableThread& obj) = delete;
			ReusableThread& operator=(ReusableThread&& obj);

			template <class Function, class... Args>
			void start(Function&& f, Args&&... args) {
				std::lock_guard<std::mutex> memberGuard(this->memberMutex);
				if (this->t->joinable()) {
					std::lock_guard<std::mutex> runGuard(this->helper->runMutex);
					this->helper->payload = std::bind(f, args...);
					this->helper->stepEndMutex.unlock();
				}

				// wait until payload is loaded
				this->helper->stepBeginMutex.lock();
			}
			void join();
			void detach();
			bool joinable();
			std::thread::id get_id() const;
			void swap(ReusableThread& obj);

		private:
			struct THelper {
					std::atomic<bool> shutdown;
					std::mutex stepBeginMutex;
					std::mutex stepEndMutex;
					std::mutex runMutex;
					std::function<void()> payload;
			};

			std::mutex memberMutex;
			std::unique_ptr<std::thread> t;
			std::shared_ptr<THelper> helper;

			static void threadFunc(std::shared_ptr<THelper> helper);
	};

}

#endif
