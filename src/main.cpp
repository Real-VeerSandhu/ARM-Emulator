// main.cpp
#include "cpu.hpp"
#include "repl.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

namespace arm_emulator {

std::vector<uint8_t> read_binary_file(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    if (!file) {
        throw std::runtime_error("Failed to open file: " + filename);
    }
    
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);
    
    std::vector<uint8_t> buffer(size);
    if (!file.read(reinterpret_cast<char*>(buffer.data()), size)) {
        throw std::runtime_error("Failed to read file: " + filename);
    }
    
    return buffer;
}

} // namespace arm_emulator

int main(int argc, char* argv[]) {
    try {
        arm_emulator::CPU cpu;
        
        // If a filename was provided, load it into memory
        if (argc > 1) {
            try {
                auto program = arm_emulator::read_binary_file(argv[1]);
                uint64_t load_address = 0x400000;  // Default load address
                
                if (argc > 2) {
                    load_address = std::stoull(argv[2], nullptr, 0);
                }
                
                if (!cpu.load_program(program, load_address)) {
                    std::cerr << "Failed to load program\n";
                    return 1;
                }
                
                std::cout << "Loaded program at 0x" << std::hex << load_address 
                          << " (" << program.size() << " bytes)\n";
            } catch (const std::exception& e) {
                std::cerr << "Error: " << e.what() << "\n";
                return 1;
            }
        } else {
            std::cout << "No program loaded. Use the REPL to enter instructions.\n";
        }
        
        // Start the REPL
        arm_emulator::REPL repl(cpu);
        repl.run();
        
    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << "\n";
        return 1;
    }
    
    return 0;
}