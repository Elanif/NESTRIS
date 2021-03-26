#pragma once
#include<queue>
#include<mutex>
#include<optional>
template<class T>
class SafeQueue {
private:
	std::queue<T> q;
	std::mutex m;
public:
	SafeQueue() :
		q()
	{}
	std::size_t size() {
		std::unique_lock<std::mutex> lock(m);
		return q.size();
	}
	std::optional<T> pop_if_not_empty() {
		std::unique_lock<std::mutex> lock(m);
		if (q.size() > 0) {
			T _result = q.front();
			q.pop();
			return std::optional<T>{_result};
		}
		return {};
	}
	T pop() {
		std::unique_lock<std::mutex> lock(m);
		T front = q.front();
		q.pop();
		return front;
	}
	void push(T element) {
		std::unique_lock<std::mutex> lock(m);
		q.push(element);
	}
};
