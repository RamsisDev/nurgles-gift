#include "file_manager.hpp"
#include "corruptor_engine.hpp" // Layer 1
#include <iostream>
#include <vector>
#include <fstream>
#include <chrono>
#include <filesystem>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/stat.h>

namespace fs = std::filesystem;

namespace Manager {
    void process_file(const std::string& filepath, double density_ratio, int mode, const std::string& profile_label) {
        std::cout << "[" << profile_label << "] PROCESSING: " << filepath << "... ";

        int fd = open(filepath.c_str(), O_RDWR);
        if (fd == -1) {
            perror("[ERROR]");
            return;
        }

        struct stat sb;
        if (fstat(fd, &sb) == -1) {
            perror("[ERROR] stat");
            close(fd);
            return;
        }

        size_t fileSize = sb.st_size;
        if (fileSize == 0) {
            std::cout << "[EMPTY/OK]" << std::endl;
            close(fd);
            return;
        }

        auto now = std::chrono::high_resolution_clock::now().time_since_epoch().count();
        std::mt19937 gen(static_cast<unsigned int>(now));

        if (fileSize < 4096) {
            std::vector<char> buffer(fileSize);
            std::ifstream is(filepath, std::ios::binary);
            if (!is.read(buffer.data(), fileSize)) {
                std::cerr << "[ERROR] Read failed" << std::endl;
                close(fd);
                return;
            }
            is.close();

            Engine::apply_corruption(buffer.data(), fileSize, density_ratio, gen, mode);

            std::ofstream os(filepath, std::ios::binary | std::ios::trunc);
            os.write(buffer.data(), fileSize);
            os.close();
        } else {
            char* mmap_ptr = static_cast<char*>(mmap(NULL, fileSize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0));
            
            if (mmap_ptr == MAP_FAILED) {
                perror("[ERROR] mmap");
                close(fd);
                return;
            }

            Engine::apply_corruption(mmap_ptr, fileSize, density_ratio, gen, mode);
            msync(mmap_ptr, fileSize, MS_ASYNC);
            munmap(mmap_ptr, fileSize);
        }

        close(fd);
        std::cout << "[OK]" << std::endl;
    }
}
