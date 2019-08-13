#pragma once
#define SLEEP_MICROSECONDS
#define SLEEP_SFML

#ifdef SLEEP_CX11
#include<chrono>
#endif // SLEEP_CX11
#ifdef SLEEP_SFML
#include<SFML/System/Clock.hpp>
#include<SFML/System.hpp>
#endif

class MyClock {
	#ifdef SLEEP_CX11
	std::chrono::high_resolution_clock::time_point starting_time = std::chrono::high_resolution_clock::now();
	#endif // SLEEP_CX11
	#ifdef SLEEP_SFML
	sf::Clock clock;
	#endif // SLEEP_SFML
public:
	largest_uint elapsedTime() {
	#ifdef SLEEP_CX11
	#ifdef SLEEP_MILLISECONDS
		return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - starting_time).count();
	 // SLEEP_MILLISECONDS

	EP_MICROSECONDS
		return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - starting_time).count();
	#endif // SLEEP_MICROSECONDS

	#ifdef SLEEP_NANOSECONDS
		return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - starting_time).count();
	#endif // SLEEP_NANOSECONDS
	#endif // SLEEP_CX11

	#ifdef SLEEP_SFML
	#ifdef SLEEP_MILLISECONDS
		return clock.getElapsedTime().asMilliseconds();
	#endif // SLEEP_MILLISECONDS

	#ifdef SLEEP_MICROSECONDS
		return clock.getElapsedTime().asMicroseconds();
	#endif // SLEEP_MICROSECONDS

	#ifdef SLEEP_NANOSECONDS
		return clock.getElapsedTime().asMicroseconds() / 1000;
	#endif // SLEEP_NANOSECONDS
	#endif // SLEEP_SFML
	}

	void restart() {
#ifdef SLEEP_CX11
		starting_time = std::chrono::high_resolution_clock::now();
#endif // SLEEP_CX11
#ifdef SLEEP_SFML
		clock.restart();
#endif // SLEEP_SFML
	}

	static constexpr unsigned long long getPartsPerSecond() {
#ifdef SLEEP_MILLISECONDS
		return 1000ull;
#endif // SLEEP_MILLISECONDS
#ifdef SLEEP_MICROSECONDS
		return 1000000ull;
#endif // SLEEP_MICROSECONDS
#ifdef SLEEP_NANOSECONDS
		return 1000000000ull;
#endif // SLEEP_NANOSECONDS
	}

	static void sleep(largest_uint const& _delay) {
		if (_delay <= 0) return;
#ifdef SLEEP_CX11
#ifdef SLEEP_MILLISECONDS
		std::this_thread::sleep_for(std::chrono::milliseconds(_delay));
		return;
#endif
#ifdef SLEEP_MICROSECONDS
		std::this_thread::sleep_for(std::chrono::microseconds(_delay));
		return;
#endif
#ifdef SLEEP_NANOSECONDS
		std::this_thread::sleep_for(std::chrono::nanoseconds(_delay));
		return;
#endif
#else
		// SLEEP_CX11
#ifdef SLEEP_SFML
#ifdef SLEEP_MILLISECONDS
		sf::sleep(sf::milliseconds(_delay));
		return;
#endif
#ifdef SLEEP_MICROSECONDS
		sf::sleep(sf::microseconds(_delay));
		return;
#endif
#ifdef SLEEP_NANOSECONDS
		sf::sleep(sf::microseconds(_delay / 1000 + 1));
		return;
#endif // SLEEP_NANOSECONDS
#endif // SLEEP_SFML
#endif
	}

	static largest_uint sleep_for_how_long(largest_uint const& _delay) {
		static MyClock clock;
		clock.restart();
		sleep(_delay);
		return clock.elapsedTime();
	}

};