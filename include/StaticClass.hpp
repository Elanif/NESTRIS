#pragma once
class StaticClass {
public:
	StaticClass() = delete;
	StaticClass(StaticClass const&) = delete;
	StaticClass& operator=(StaticClass const&) = delete;
	//~StaticClass() = delete;
};