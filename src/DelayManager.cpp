#include"DelayManager.hpp"

void GeneralDelayManager::delay(largest_uint const& target_delay) {
	if (target_delay <= max_extra_delay) return;
	largest_uint real_delay = MyClock::sleep_for_how_long(target_delay - max_extra_delay);
	if (real_delay > target_delay) {
		max_extra_delay = real_delay - target_delay + max_extra_delay;
	}
};

void GeneralDelayManager::reset() {
	max_extra_delay = ntris::MIN_DELAY_ERROR;
};

void SmallestDelayManager::delay(largest_uint const& target_delay) {
	if (target_delay <= max_extra_delay) return;
	largest_uint real_delay = MyClock::sleep_for_how_long(target_delay - max_extra_delay);
	if (real_delay > max_extra_delay) {
		max_extra_delay = real_delay;
	}
};

void SmallestDelayManager::reset() {
	max_extra_delay = ntris::MIN_DELAY_ERROR;
};
//
//ArrayDelayManager::delay(largest_uint const& target_delay) {
//	MyClock clock;
//	if (target_delay >= max_delay_size) target_delay = max_delay_size - 1;
//	largest_uint reduced_delay = target_delay - clock.elapsedTime();
//	while (clock.elapsedTime() < target_delay && reduced_delay <= arr_delay[reduced_delay]) {
//		reduced_delay = target_delay - clock.elapsedTime();
//	};
//	if (reduced_delay <= 0) return;
//	largest_uint real_delay = MyClock::sleep_for_how_long(reduced_delay - arr_delay[reduced_delay]);
//	if (real_delay > reduced_delay) {
//		arr_delay[reduced_delay] = real_delay - reduced_delay;
//	}
//}
//
//ArrayDelayManager::reset() {
//	for (const auto& i : arr_delay)
//		i = max_extra_delay;
//}
//
//BucketArrayDelayManager::delay(largest_uint const& target_delay) {
//	if (target_delay <= 0) return;
//	if (target_delay >= partsperframe) target_delay = partsperframe;
//	auto bucket_finder = [&bucket_size](largest_uint index) {
//		return index / bucket_size;
//	};
//	while (target_delay > 0 && target_delay < arr_delay[bucket_finder(target_delay)]) {
//		target_delay -= bucket_size;
//	}
//	if (target_delay < 0) return;
//	largest_uint real_delay = MyClock::sleep_for_how_long(bucket_finder(target_delay) * bucket_size - arr_delay[bucket_finder(target_delay)]);
//	if (real_delay > bucket_finder(target_delay) * bucket_size) arr_delay[bucket_finder(target_delay)] = real_delay - bucket_finder(target_delay) * bucket_size;
//}
//
//BucketArrayDelayManager::reset() {
//	for (const auto& i : arr_delay)
//		i = max_extra_delay;
//}
////
//ArrayLogDelayManager::delay(largest_uint const& target_delay) {
//	if (target_delay <= 0) return;
//	std::size_t index = 0;
//	std::size_t result_index = -1;
//	if (target_delay > max_delay_allowed) target_delay = max_delay_allowed;
//	unsigned long long t_delay = target_delay;
//	while (t_delay > 0) {
//		if (arr_delay[index] < target_delay) result_index = index;
//		t_delay >>= 1;
//		index++;
//	}
//	if (result_index >= array_length || result_index < 0) return;
//	target_delay = largest_uint(1) << result_index;
//	largest_uint real_delay = MyClock::sleep_for_how_long(target_delay - arr_delay[result_index]);
//	if (real_delay > target_delay) arr_delay[result_index] = real_delay - target_delay;
//}
//
//ArrayLogDelayManager::reset() {
//	for (const auto& i : arr_delay)
//		i = max_extra_delay;
//}

void SpamDelayManager::delay(largest_uint const& target_delay) {
	MyClock clock;
	largest_uint _late_time = clock.elapsedTime();
	while (_late_time + smallest_delay_possible < target_delay) {
		largest_uint _prev_time = _late_time;
		MyClock::sleep(1);
		largest_uint _late_time = clock.elapsedTime();
		if (_late_time - _prev_time > smallest_delay_possible)smallest_delay_possible = _late_time - _prev_time;
	}
};

void SpamDelayManager::reset() {
	smallest_delay_possible = ntris::MIN_DELAY_ERROR;
};

void FullThreadDelayManager::delay(largest_uint const& target_delay) {
	MyClock clock;
	largest_uint _last_time = clock.elapsedTime();
	while (clock.elapsedTime() + smallesttimeunit < target_delay) {
		largest_uint __temp = (clock.elapsedTime() - _last_time);
		if (__temp > smallesttimeunit) smallesttimeunit = __temp;
	};
};

void FullThreadDelayManager::reset() {
	smallesttimeunit = ntris::MIN_DELAY_ERROR;
};


void NothingDelayManager::delay(largest_uint const& target_delay) {
};

void NothingDelayManager::reset() {
};

