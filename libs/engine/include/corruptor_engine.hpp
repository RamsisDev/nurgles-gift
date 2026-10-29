#ifndef CORRUPTOR_ENGINE_HPP
#define CORRUPTOR_ENGINE_HPP

#include <cstddef>
#include <random>

namespace Engine {
    // Applies noise to a memory buffer.
    void apply_corruption(char* ptr, size_t size, double density_ratio, std::mt19937& gen, int mode);
}

#endif
