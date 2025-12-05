#pragma once

#include <array>
#include <cstdint>
#include <stdexcept>
#include <string>

namespace arm_emulator {

// Number of general-purpose registers (X0-X30)
constexpr size_t NUM_REGISTERS = 31;

// Special register indices
enum class SpecialRegister {
    XZR = 31,  // Zero register (reads as 0, writes ignored)
    SP = 32,    // Stack pointer
    PC = 33,    // Program counter
    NUM_SPECIAL_REGISTERS = 34
};

// Total number of registers including special ones
constexpr size_t TOTAL_REGISTERS = static_cast<size_t>(SpecialRegister::NUM_SPECIAL_REGISTERS);

class Registers {
public:
    Registers();
    
    // Reset all registers to zero
    void reset() noexcept;
    
    // Register access by index (0-30 for X0-X30, 31 for XZR, 32 for SP, 33 for PC)
    uint64_t get_register(size_t index) const;
    void set_register(size_t index, uint64_t value) noexcept;
    
    // Convenience methods for special registers
    uint64_t get_pc() const { return get_register(static_cast<size_t>(SpecialRegister::PC)); }
    void set_pc(uint64_t value) { set_register(static_cast<size_t>(SpecialRegister::PC), value); }
    
    uint64_t get_sp() const { return get_register(static_cast<size_t>(SpecialRegister::SP)); }
    void set_sp(uint64_t value) { set_register(static_cast<size_t>(SpecialRegister::SP), value); }
    
    // Dump all registers to string for debugging
    std::string to_string() const;

private:
    std::array<uint64_t, TOTAL_REGISTERS> registers;
};

} // namespace arm_emulator
