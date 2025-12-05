#include "registers.hpp"
#include <sstream>
#include <iomanip>
#include <stdexcept>

namespace arm_emulator {

Registers::Registers() {
    reset();
}

void Registers::reset() noexcept {
    registers.fill(0);
    // Initialize SP to a reasonable value (top of memory - 8)
    registers[static_cast<size_t>(SpecialRegister::SP)] = 0xFFFF0000;
}

uint64_t Registers::get_register(size_t index) const {
    if (index >= TOTAL_REGISTERS) {
        throw std::out_of_range("Register index out of range");
    }
    
    // XZR always returns 0
    if (index == static_cast<size_t>(SpecialRegister::XZR)) {
        return 0;
    }
    
    return registers[index];
}

void Registers::set_register(size_t index, uint64_t value) noexcept {
    if (index >= TOTAL_REGISTERS) {
        return; // Silently ignore out-of-range writes
    }
    
    // Writes to XZR are ignored
    if (index == static_cast<size_t>(SpecialRegister::XZR)) {
        return;
    }
    
    registers[index] = value;
}

std::string Registers::to_string() const {
    std::ostringstream oss;
    oss << std::hex << std::setfill('0');
    
    // General purpose registers
    for (size_t i = 0; i < NUM_REGISTERS; ++i) {
        if (i % 4 == 0) {
            if (i > 0) oss << "\n";
            oss << "X" << std::setw(2) << i << ": ";
        } else {
            oss << "  ";
        }
        oss << "0x" << std::setw(16) << get_register(i);
    }
    
    // Special registers
    oss << "\nXZR: 0x" << std::setw(16) << get_register(static_cast<size_t>(SpecialRegister::XZR));
    oss << "  SP: 0x" << std::setw(16) << get_register(static_cast<size_t>(SpecialRegister::SP));
    oss << "\n PC: 0x" << std::setw(16) << get_register(static_cast<size_t>(SpecialRegister::PC));
    
    return oss.str();
}

} // namespace arm_emulator