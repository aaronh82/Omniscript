//
// Created by Aaron Houghton on 2/24/16.
//

#ifndef PROJECT_THREADPOOL_H
#define PROJECT_THREADPOOL_H


#include <vector>
#include <thread>
#include <queue>
#include <future>

class ThreadPool {
	size_t thread_count;
	std::vector<std::thread> workers;
	std::queue<std::function<void()>> tasks;
	std::mutex queue_mutex;
	std::condition_variable condition;
	bool stop;

public:
	ThreadPool(size_t);
	template <typename F, typename... Args>
	auto enqueue(F&& f, Args&&... args)
			-> std::future<typename std::result_of<F(Args...)>::type>;
	~ThreadPool();
};

inline ThreadPool::ThreadPool(size_t threads) : thread_count(threads), stop(false) {
	for (size_t i = 0; i < threads; ++i) {
		workers.emplace_back(
				[this] {
					for (;;) {
						std::function<void()> task;

						{
							std::unique_lock<std::mutex> lock(this->queue_mutex);
							this->condition.wait(lock,
								[this]{ return this->stop || !this->tasks.empty(); });
							if (this->stop && this->tasks.empty())
								return;
							task = std::move(this->tasks.front());
							this->tasks.pop();
						}

						task();
					}
				}
		);
	}
}

inline ThreadPool::~ThreadPool() {
	{
		std::unique_lock<std::mutex> lock(queue_mutex);
		stop = true;
	}
	condition.notify_all();
	for (std::thread &worker: workers)
		worker.join();
}

#endif //PROJECT_THREADPOOL_H
