#pragma once


#include <chrono>

namespace Helpers {
	float getFrameRate() {
        static std::chrono::steady_clock::time_point last;

        auto current = std::chrono::high_resolution_clock::now();

        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(current - last);

        last = current;

        return 1000000.0f / static_cast<float>(duration.count());
	}
}