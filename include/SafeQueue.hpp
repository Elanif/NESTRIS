#pragma once
#include<queue>
#include<mutex>

template<class T>
class SafeQueue {
private:
	std::queue<T> q;
	std::mutex m;
	std::size_t _size = 0;
public:
	SafeQueue() :
		q()
	{}
	std::size_t size() {
		std::unique_lock<std::mutex> lock(m);
		return _size;
	}
	T pop() {
		std::unique_lock<std::mutex> lock(m);
		--_size;
		T front = q.front();
		q.pop();
		return front;
	}
	void push(T element) {
		std::unique_lock<std::mutex> lock(m);
		q.push(element);
		++_size;
	}
};
