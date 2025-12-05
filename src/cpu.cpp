#include "cpu.hpp"
#include "decoder.hpp"
#include "instruction.hpp"
#include <sstream>
#include <iostream>

namespace arm_emulator {

CPU::CPU(size_t memory_size) : memory(std::make_unique<Memory>(memory_size)) {
    reset();
}

void CPU::reset() noexcept {
    registers.reset();
    running = true;
    breakpoints.clear();
}

bool CPU::load_program(const std::vector<uint8_t>& program, uint64_t address) {
    try {
        // Write the program to memory
        for (size_t i = 0; i < program.size(); i++) {
            memory->write8(address + i, program[i]);
        }
        registers.set_pc(address);
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Failed to load program: " << e.what() << std::endl;
        return false;
    }
}

bool CPU::step_instruction() {
    if (!running) return false;
    
    // Check for breakpoint
    uint64_t pc = registers.get_pc();
    if (breakpoints.count(pc)) {
        std::cout << "Breakpoint hit at 0x" << std::hex << pc << std::dec << std::endl;
        return false;
    }
    
    try {
        // Fetch
        uint32_t instruction_word = memory->read32(pc);
        
        // Decode
        Instruction instr = Decoder::decode(instruction_word);
        
        // Execute
        execute_instruction(instr);
        
        // Update PC (if not a branch instruction)
        if (!instr.is_branch()) {
            registers.set_pc(pc + 4);
        }
        
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Error executing instruction at 0x" << std::hex << pc 
                  << ": " << e.what() << std::endl;
        running = false;
        return false;
    }
}

void CPU::run() {
    while (running) {
        if (!step_instruction()) {
            break;
        }
    }
}

std::string CPU::get_state() const {
    std::ostringstream oss;
    oss << registers.to_string() << "\n";
    return oss.str();
}

void CPU::execute_instruction(const Instruction& instr) {
    // Implementation of instruction execution
    // This is a simplified version - you'll need to expand this
    switch (instr.opcode) {
        case Opcode::ADD:
            // Implement ADD logic
            break;
        case Opcode::SUB:
            // Implement SUB logic
            break;
        // Add other instructions...
        default:
            std::ostringstream oss;
            oss << "Unimplemented instruction: " << static_cast<int>(instr.opcode);
            throw std::runtime_error(oss.str());
    }
}

} // namespace arm_emulator