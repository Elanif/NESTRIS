#pragma once
#include"ntris.hpp"
#include"MyClock.hpp"
//constexpr largest_uint log2(largest_uint _logarg) {
//	constexpr std::size_t max_bits = std::numeric_limits<unsigned long long>::digits;
//	for (std::size_t i = 0; i < max_bits && _logarg>0; ++i) {
//		_logarg <<= std::size_t{ 1 };
//	}
//	return _logarg;
//}

class DelayManager {
public:
	virtual void delay(largest_uint const& target_delay) = 0;
	virtual void reset() = 0;
};

class GeneralDelayManager : public DelayManager {
private:
	largest_uint max_extra_delay = ntris::MIN_DELAY_ERROR;
public:
	void delay(largest_uint const& target_delay);
	void reset();
};

class SmallestDelayManager : public DelayManager {
private:
	largest_uint max_extra_delay = ntris::MIN_DELAY_ERROR;
public:
	void delay(largest_uint const& target_delay);
	void reset();
};

//class ArrayDelayManager : public DelayManager {
//private:
//	const largest_uint max_extra_delay = ntris::MIN_DELAY_ERROR;
//	const std::size_t max_delay_size{ MyClock::getPartsPerSecond() };
//	largest_uint arr_delay[max_delay_size] = { max_extra_delay };
//public:
//	void delay(largest_uint const& target_delay);
//	void reset();
//};
//
//class BucketArrayDelayManager : public DelayManager {
//private:
//	largest_uint max_extra_delay = ntris::MIN_DELAY_ERROR;
//	const std::size_t partsperframe{ MyClock::getPartsPerSecond() };
//	const std::size_t bucket_size{ ntris::bucket_size };
//	largest_uint arr_delay[partsperframe / bucket_size + 1] = { ntris::MIN_DELAY_ERROR };
//public:
//	void delay(largest_uint const& target_delay);
//	void reset();
//};

//class ArrayLogDelayManager : public DelayManager {
//private:
//	largest_uint max_extra_delay = ntris::MIN_DELAY_ERROR;
//	constexpr std::size_t partsperframe{ MyClock::getPartsPerSecond() };
//	constexpr std::size_t array_length{ log2(partsperframe) + 1 };
//	largest_uint arr_delay[array_length] = { ntris::MIN_DELAY_ERROR };
//public:
//	void delay(largest_uint const& target_delay);
//	void reset();
//};

class SpamDelayManager : public DelayManager {
private:
	largest_uint smallest_delay_possible = ntris::MIN_DELAY_ERROR;
public:
	void delay(largest_uint const& target_delay);
	void reset();
};

class FullThreadDelayManager : public DelayManager {
private:
	largest_uint smallesttimeunit = ntris::MIN_DELAY_ERROR;
public:
	void delay(largest_uint const& target_delay);
	void reset();
};

class NothingDelayManager : public DelayManager {
private:
public:
	void delay(largest_uint const& target_delay);
	void reset();
};