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

void ArrayDelayManager::delay(largest_uint const& _target_delay) {
	largest_uint target_delay = _target_delay;
	MyClock clock;
	if (target_delay >= array_length) target_delay = array_length - 1;
	largest_uint reduced_delay = target_delay - clock.elapsedTime();
	while (clock.elapsedTime() < target_delay && reduced_delay <= arr_delay[reduced_delay]) {
		reduced_delay = target_delay - clock.elapsedTime();
	};
	if (reduced_delay <= 0) return;
	largest_uint real_delay = MyClock::sleep_for_how_long(reduced_delay - arr_delay[reduced_delay]);
	if (real_delay > reduced_delay) {
		arr_delay[reduced_delay] = real_delay - reduced_delay;
	}
}

void ArrayDelayManager::reset() {
	for (auto& i : arr_delay)
		i = ntris::MIN_DELAY_ERROR;
}

void BucketArrayDelayManager::delay(largest_uint const& _target_delay) {
	if (_target_delay <= 0) return;
	largest_uint target_delay = _target_delay;
	const largest_uint bucket_size = ntris::bucket_size;
	auto bucket_finder = [&bucket_size](const largest_uint& index) {
		return index / bucket_size;
	};
	std::size_t bucket_index = bucket_finder(target_delay);
	if (bucket_index > array_length) bucket_index = array_length - 1;
	while (target_delay > 0 && target_delay < arr_delay[bucket_finder(target_delay)]) {
		target_delay -= bucket_size;
	}
	if (target_delay < 0) return;
	largest_uint real_delay = MyClock::sleep_for_how_long(bucket_finder(target_delay) * bucket_size - arr_delay[bucket_finder(target_delay)]);
	if (real_delay > bucket_finder(target_delay) * bucket_size) arr_delay[bucket_finder(target_delay)] = real_delay - bucket_finder(target_delay) * bucket_size;
}

void BucketArrayDelayManager::reset() {
	for (auto& i : arr_delay)
		i = ntris::MIN_DELAY_ERROR;
}

ArrayLogDelayManager::ArrayLogDelayManager()
	:array_length(log2(MyClock::getPartsPerSecond()) + 1)
{
	arr_delay = new largest_uint[array_length]{ ntris::MIN_DELAY_ERROR };
}
ArrayLogDelayManager::~ArrayLogDelayManager() {
	delete[] arr_delay;
}

void ArrayLogDelayManager::delay(largest_uint const& _target_delay) {
	largest_uint target_delay = _target_delay;
	if (target_delay <= ntris::MIN_DELAY_ERROR) return;
	std::size_t index = 0;
	if (target_delay > array_length-1) target_delay = array_length -1;
	unsigned long long t_delay = target_delay;
	while (t_delay > 0) {
		if (arr_delay[index] < target_delay) break;
		t_delay >>= 1;
		index++;
	}
	if (t_delay > 0 && index<array_length) {
		largest_uint real_delay = MyClock::sleep_for_how_long(target_delay - arr_delay[index]);
		if (real_delay > target_delay) arr_delay[index] = real_delay - target_delay;
	}
}

void ArrayLogDelayManager::reset() {
	for (std::size_t i = 0; i < array_length; ++i) {
		arr_delay[i] = ntris::MIN_DELAY_ERROR;
	}
}

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

