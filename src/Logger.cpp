#include <ctime>
#include <iomanip>

#include <chrono>
#include <iostream>
#include <sstream>

#include "Logger.hpp"

using namespace ProjectMaya;

using std::chrono::milliseconds;
using std::chrono::system_clock;
using std::clog;
using std::endl;
using std::string;
using std::stringstream;
using std::localtime;
using std::lock_guard;
using std::mutex;
using std::noskipws;
using std::put_time;
using std::this_thread::sleep_for;

void Logger::run() {
	milliseconds stime(20);

	while(!this->shouldShutdown()) {
		// get queue status
		bool empty;
		{
			lock_guard<mutex> memberGuard(this->memberMutex);
			empty = this->msgQueue.empty();
		}

		while(!empty) {
			string msg;
			// get msg
			{
				lock_guard<mutex> memberGuard(this->memberMutex);
				msg = this->msgQueue.front();
				this->msgQueue.pop();
			}

			// formatting: timestamp and message
			auto now = system_clock::to_time_t(system_clock::now());
			clog << "[" << put_time(localtime(&now), "%H:%M:%S") << "]" << msg << endl;

			// get new queue status
			{
				lock_guard<mutex> memberGuard(this->memberMutex);
				empty = this->msgQueue.empty();
			}
		}

		sleep_for(stime);
	}
}

void Logger::log(const string& msg) {
	lock_guard<mutex> memberGuard(this->memberMutex);

	stringstream stream;
	stream << noskipws << "[*]: " << msg;

	this->msgQueue.push(stream.str());
}

void Logger::log(const int& id, const string& msg) {
	lock_guard<mutex> memberGuard(this->memberMutex);

	stringstream stream;
	stream << noskipws << "[" << id << "]: " << msg;

	this->msgQueue.push(stream.str());
}

void Logger::log(const string& name, const string& msg) {
	lock_guard<mutex> memberGuard(this->memberMutex);

	stringstream stream;
	stream << noskipws << "[" << name << "]" << ": " << msg;

	this->msgQueue.push(stream.str());
}
