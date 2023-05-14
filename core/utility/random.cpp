#include "utility.hpp"

#include <random>

namespace core::utility {

int rng_uniform_distribution(int min, int max) {
    static std::random_device random_device;
    static std::mt19937 generator(random_device());
    std::uniform_int_distribution<> distribution(min, max);

    return distribution(generator);
}

}  // namespace core::utility
