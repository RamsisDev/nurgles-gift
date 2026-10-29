#ifndef FILE_MANAGER_HPP
#define FILE_MANAGER_HPP

#include <string>

namespace Manager {
    // Reads/writes a file on disk (mmap or buffer).
    void process_file(const std::string& filepath, double density_ratio, int mode, const std::string& profile_label);
}

#endif
