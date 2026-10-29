#include "corruptor_engine.hpp"

namespace Engine {
    void apply_corruption(char* ptr, size_t size, double density_ratio, std::mt19937& gen, int mode) {
        if (size == 0 || density_ratio <= 0) return;

        if (mode == 1 && size > 64) {
            std::uniform_int_distribution<int> dist_byte(0, 255);
            for (size_t i = 0; i < 64; ++i) {
                ptr[i] = static_cast<char>(dist_byte(gen));
            }
        }

        size_t total_corruptions = static_cast<size_t>(size * density_ratio);
        std::uniform_int_distribution<size_t> dist_pos(0, size - 1);
        std::uniform_int_distribution<int> dist_byte(0, 255);

        for (size_t i = 0; i < total_corruptions; ++i) {
            size_t targetOffset = dist_pos(gen);
            ptr[targetOffset] = static_cast<char>(dist_byte(gen));
        }
    }
}
