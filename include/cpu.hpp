#pragma once

#include "registers.hpp"
#include "memory.hpp"
#include "instruction.hpp"

#include <cstdint>
#include <string>
#include <vector>
#include <memory>
#include <set>

namespace arm_emulator {

class CPU {
public:
    // Initialize CPU with memory size (default 1MB)
    explicit CPU(size_t memory_size = 1024 * 1024);
    
    // Reset the CPU state (registers, memory, etc.)
    void reset() noexcept;
    
    // Load a program into memory at the specified address
    bool load_program(const std::vector<uint8_t>& program, uint64_t address = 0);
    
    // Execute a single instruction
    bool step_instruction();
    
    // Run until a halt condition is met (e.g., infinite loop or program end)
    void run();
    
    // Get the current CPU state as a string (for debugging)
    std::string get_state() const;
    
    // Get references to registers and memory (for debugging/testing)
    Registers& get_registers() { return registers; }
    const Registers& get_registers() const { return registers; }
    const Memory& get_memory() const { return *memory; }
    
    // Check if the CPU is in a running state
    bool is_running() const { return running; }
    
    // Set a breakpoint at the specified address
    void set_breakpoint(uint64_t address) { breakpoints.insert(address); }
    
    // Clear a breakpoint
    void clear_breakpoint(uint64_t address) { breakpoints.erase(address); }

private:
    // CPU components
    Registers registers;
    std::unique_ptr<Memory> memory;
    
    // Execution state
    bool running{false};
    std::set<uint64_t> breakpoints;
    
    // Instruction execution helpers
    Instruction decode_instruction(uint32_t instruction_word) const;
    void execute_instruction(const Instruction& instr);
    
    // Instruction implementation methods
    void execute_data_processing(const Instruction& instr);
    void execute_branch(const Instruction& instr);
    void execute_load_store(const Instruction& instr);
    
    // Helper methods
    bool check_condition(Condition cond) const;
    uint64_t get_shifted_operand(uint64_t value, uint8_t shift_type, uint8_t shift_amount) const;
    
    // Memory access helpers with alignment checks
    uint32_t fetch_instruction() const;
};

} // namespace arm_emulator
