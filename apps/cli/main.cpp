#include <iostream>
#include <string>
#include <filesystem>
#include "file_manager.hpp" // Layer 2

namespace fs = std::filesystem;

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Usage: ./nurgle <path> [low|extreme|manual_density] [manual_mode]\n";
        return 1;
    }

    std::string target = argv[1];
    double density_ratio;
    int mode;
    std::string profile_label;
    std::string mode_arg = (argc > 2) ? argv[2] : "";

    if (mode_arg == "low") {
        density_ratio = 0.1;
        mode = 0;
        profile_label = "LOW_MODE";
    } else if (mode_arg == "extreme") {
        density_ratio = 0.6;
        mode = 1;
        profile_label = "EXTREME_MODE";
    } else {
        try {
            // numeric arg = manual density
            density_ratio = std::stod(mode_arg);
            mode = (argc > 3) ? std::stoi(argv[3]) : 0;
            profile_label = "MANUAL";
        } catch (...) {
            density_ratio = 0;
            mode = 0;
            profile_label = "UNKNOWN";
        }
    }

    try {
        if (fs::is_directory(target)) {
            std::cout << "[INFO] Scanning directory: " << target << std::endl;
            for (const auto& entry : fs::recursive_directory_iterator(target)) {
                if (fs::is_regular_file(entry.path())) {
                    Manager::process_file(entry.path().string(), density_ratio, mode, profile_label);
                }
            }
        } else if (fs::exists(target)) {
            // single file
            Manager::process_file(target, density_ratio, mode, profile_label);
        } else if (profile_label == "UNKNOWN") {
             std::cerr << "[ERROR] Path does not exist or invalid arguments." << std::endl;
             return 1;
        }
    } catch (const std::exception& e) {
        std::cerr << "[FATAL] Exception: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
