#pragma once

#include <thread>
#include <vector>
#include <queue>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <atomic>

class thread_pool {
public:
	thread_pool(size_t thread_num) {
		m_running = true;
		for (size_t i = 0; i < thread_num; i++) {
			m_threads.emplace_back([this, i] {
				while (m_running) {
					std::function<void()> task;
					{
						std::unique_lock<std::mutex> lock(m_mutex);
						m_cond.wait(lock, [this]() {return !m_tasks.empty() || !m_running;});

						if (!m_running && m_tasks.empty()) return;

						task = std::move(m_tasks.front());
						m_tasks.pop();
					}
					task();
				}
			});
		}
	}

	~thread_pool() {
		{
			std::unique_lock<std::mutex> lock(m_mutex);
			m_running = false;
		}
		m_cond.notify_all();
		for (auto& t : m_threads) {
			if (t.joinable()) t.join();
		}
	}

	void queue(std::function<void()> job) {
		{
			std::unique_lock<std::mutex> lock(m_mutex);
			m_tasks.push(job);
		}
		m_cond.notify_one();
	}

private:
	std::vector<std::thread> m_threads;
	std::queue<std::function<void() >> m_tasks;

	std::mutex m_mutex;
	std::condition_variable m_cond;
	std::atomic<bool> m_running;
};
