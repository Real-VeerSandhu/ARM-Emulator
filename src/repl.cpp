#include "repl.hpp"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <algorithm>

namespace arm_emulator {

REPL::REPL(CPU& cpu_ref) : cpu(cpu_ref) {}

void REPL::run() {
    std::cout << "ARM Emulator - Type 'help' for available commands\n";
    std::string line;
    
    while (true) {
        std::cout << "arm> ";
        if (!std::getline(std::cin, line)) {
            break;  // Exit on EOF (Ctrl+D)
        }
        
        // Skip empty lines
        if (line.empty()) {
            continue;
        }
        
        // Process the command
        if (!process_command(line)) {
            break;
        }
    }
}

bool REPL::process_command(const std::string& line) {
    auto args = split_line(line);
    if (args.empty()) {
        return true;
    }
    
    const std::string& cmd = args[0];
    
    try {
        if (cmd == "step" || cmd == "s") {
            handle_step();
        } else if (cmd == "run" || cmd == "r") {
            handle_run();
        } else if (cmd == "break" || cmd == "b") {
            handle_break(args);
        } else if (cmd == "reg" || cmd == "r") {
            handle_register(args);
        } else if (cmd == "mem" || cmd == "m") {
            handle_memory(args);
        } else if (cmd == "help" || cmd == "h" || cmd == "?") {
            handle_help();
        } else if (cmd == "quit" || cmd == "q" || cmd == "exit") {
            return false;
        } else {
            std::cout << "Unknown command: " << cmd << "\n";
            std::cout << "Type 'help' for available commands\n";
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
    }
    
    return true;
}

void REPL::handle_step() {
    if (!cpu.step_instruction()) {
        std::cout << "Execution stopped\n";
    }
    print_state();
}

void REPL::handle_run() {
    cpu.run();
    print_state();
}

void REPL::handle_break(const std::vector<std::string>& args) {
    if (args.size() < 2) {
        std::cout << "Usage: break <address>\n";
        return;
    }
    
    try {
        uint64_t address = std::stoull(args[1], nullptr, 0);
        cpu.set_breakpoint(address);
        std::cout << "Breakpoint set at 0x" << std::hex << address << "\n";
    } catch (const std::exception&) {
        std::cerr << "Invalid address: " << args[1] << "\n";
    }
}

void REPL::handle_register(const std::vector<std::string>& args) {
    if (args.size() < 2) {
        // Show all registers
        std::cout << cpu.get_state() << "\n";
        return;
    }
    
    // Handle register modification: reg X0 = 0x1234
    if (args.size() >= 3 && args[2] == "=") {
        try {
            std::string reg = args[1];
            uint64_t value = std::stoull(args[3], nullptr, 0);
            
            if (reg[0] == 'X' || reg[0] == 'x') {
                int reg_num = std::stoi(reg.substr(1));
                if (reg_num >= 0 && reg_num < 31) {
                    cpu.get_registers().set_register(reg_num, value);
                    std::cout << "Set " << reg << " = 0x" << std::hex << value << "\n";
                    return;
                }
            } else if (reg == "SP" || reg == "sp") {
                cpu.get_registers().set_sp(value);
                std::cout << "Set SP = 0x" << std::hex << value << "\n";
                return;
            } else if (reg == "PC" || reg == "pc") {
                cpu.get_registers().set_pc(value);
                std::cout << "Set PC = 0x" << std::hex << value << "\n";
                return;
            }
        } catch (const std::exception&) {
            // Fall through to error message
        }
        
        std::cerr << "Invalid register or value\n";
        return;
    }
    
    // Show specific register
    std::string reg = args[1];
    try {
        uint64_t value = 0;
        
        if (reg[0] == 'X' || reg[0] == 'x') {
            int reg_num = std::stoi(reg.substr(1));
            if (reg_num >= 0 && reg_num < 31) {
                value = cpu.get_registers().get_register(reg_num);
                std::cout << reg << " = 0x" << std::hex << value << "\n";
                return;
            }
        } else if (reg == "SP" || reg == "sp") {
            value = cpu.get_registers().get_sp();
            std::cout << "SP = 0x" << std::hex << value << "\n";
            return;
        } else if (reg == "PC" || reg == "pc") {
            value = cpu.get_registers().get_pc();
            std::cout << "PC = 0x" << std::hex << value << "\n";
            return;
        }
    } catch (const std::exception&) {
        // Fall through to error message
    }
    
    std::cerr << "Invalid register: " << reg << "\n";
}

void REPL::handle_memory(const std::vector<std::string>& args) {
    if (args.size() < 2) {
        std::cout << "Usage: mem <address> [length]\n";
        return;
    }
    
    try {
        uint64_t address = std::stoull(args[1], nullptr, 0);
        size_t length = 64;  // Default length
        
        if (args.size() >= 3) {
            length = std::stoull(args[2], nullptr, 0);
        }
        
        std::cout << cpu.get_memory().dump_memory(address, address + length - 1) << "\n";
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
    }
}

void REPL::handle_help() const {
    std::cout << "Available commands:\n"
              << "  step, s        - Execute one instruction\n"
              << "  run, r         - Run until breakpoint or end of program\n"
              << "  break, b <addr>- Set breakpoint at address\n"
              << "  reg, r         - Show all registers\n"
              << "  reg <reg>      - Show value of specific register\n"
              << "  reg <reg> = <val> - Set register value\n"
              << "  mem, m <addr> [len] - Show memory contents\n"
              << "  help, h, ?     - Show this help\n"
              << "  quit, q, exit  - Exit the emulator\n";
}

void REPL::print_state() const {
    std::cout << cpu.get_state() << "\n";
}

std::vector<std::string> REPL::split_line(const std::string& line) const {
    std::vector<std::string> result;
    std::istringstream iss(line);
    std::string token;
    
    while (iss >> token) {
        // Handle quoted strings
        if (token[0] == '\"') {
            std::string quoted = token.substr(1);
            if (quoted.back() == '\"') {
                quoted.pop_back();
                result.push_back(quoted);
            } else {
                std::string rest;
                while (std::getline(iss, rest, '\"') && !iss.eof()) {
                    quoted += " " + rest;
                }
                if (!rest.empty()) {
                    quoted += " " + rest;
                }
                result.push_back(quoted);
            }
        } else {
            result.push_back(token);
        }
    }
    
    return result;
}

} // namespace arm_emulator