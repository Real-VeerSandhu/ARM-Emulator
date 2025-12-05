// repl.hpp
#pragma once

#include "cpu.hpp"
#include <string>
#include <vector>

namespace arm_emulator {

class REPL {
public:
    REPL(CPU& cpu);
    
    // Start the REPL
    void run();
    
private:
    CPU& cpu;
    
    // Process a single command
    bool process_command(const std::string& line);
    
    // Command handlers
    void handle_step();
    void handle_run();
    void handle_break(const std::vector<std::string>& args);
    void handle_register(const std::vector<std::string>& args);
    void handle_memory(const std::vector<std::string>& args);
    void handle_help() const;
    
    // Helper methods
    void print_state() const;
    std::vector<std::string> split_line(const std::string& line) const;
};

} // namespace arm_emulator