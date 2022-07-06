#include <list>
#include <mutex>
#include <chrono>
#include <atomic>
#include <thread>
#include <iostream>
#include <functional>

class task_queue {
public:
	task_queue(): do_tasks(true), main_thread([this]() {
		while(do_tasks) {
			while (queue_size() == 0 && do_tasks) {
				std::this_thread::sleep_for(std::chrono::milliseconds(100));
			}
			if (!do_tasks && queue_size() == 0) {
				break;
			}
			for(std::list<std::function<void()>>::iterator task = queue.begin(); task != queue.end(); task = queue.begin()) {
				(*task)();
				std::unique_lock<std::mutex> lock_queue(mtx_queue);
				queue.erase(task);
			}
		}}) {
	}
	void add_task(std::function<void()> task) {
		if (!do_tasks) {
			return;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		std::unique_lock<std::mutex> lock_queue(mtx_queue);
		queue.push_back(task);
	}
	~task_queue() {
		do_tasks = false;
		while (!queue_empty()) {
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}
		main_thread.join();
	}

private:
	std::atomic<bool> do_tasks;
	std::thread main_thread;
	std::mutex mtx_queue;
	std::list<std::function<void()>> queue;
	
	
	bool queue_empty() {
		std::unique_lock<std::mutex> lock(mtx_queue);
		return queue.empty();
	}
	size_t queue_size() {
		std::unique_lock<std::mutex> lock(mtx_queue);
		return queue.size();
	}
};

std::mutex cout_mtx;

void output(const char *string) {
	std::unique_lock<std::mutex> lock(cout_mtx);
	std::cout << string;
}

int main() {
	task_queue taskQueue;
	taskQueue.add_task([](){
		output("Start first task\n");
		std::this_thread::sleep_for(std::chrono::milliseconds(2000));
		output("Finished first task\n");
	});
	taskQueue.add_task([](){
		output("Start second task\n");
		std::this_thread::sleep_for(std::chrono::milliseconds(2000));
		output("Finished second task\n");
	});
	taskQueue.add_task([&](){
		output("Start third task\n");
		std::this_thread::sleep_for(std::chrono::milliseconds(2000));
		output("Finished third task\n");
	});
	taskQueue.add_task([&](){
		output("Start fourth task\n");
		std::this_thread::sleep_for(std::chrono::milliseconds(2000));
		output("Finished fourth task\n");
	});
	taskQueue.add_task([&](){
		output("Start fifth task\n");
		std::this_thread::sleep_for(std::chrono::milliseconds(2000));
		output("Finished fifth task\n");
	});
	return 0;
}
