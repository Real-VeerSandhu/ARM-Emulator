#include "memory.hpp"
#include <sstream>
#include <iomanip>
#include <stdexcept>

namespace arm_emulator {

Memory::Memory(size_t size) : memory(size, 0) {
    if (size == 0) {
        throw std::invalid_argument("Memory size must be greater than 0");
    }
}

void Memory::reset() noexcept {
    std::fill(memory.begin(), memory.end(), 0);
}

void Memory::check_address(uint64_t address, size_t size) const {
    if (address + size > memory.size() || address + size < address) {
        throw std::runtime_error("Memory access out of bounds: 0x" + 
                               std::to_string(address) + " + " + 
                               std::to_string(size));
    }
}

uint8_t Memory::read8(uint64_t address) const {
    check_address(address, sizeof(uint8_t));
    return memory[address];
}

uint32_t Memory::read32(uint64_t address) const {
    check_address(address, sizeof(uint32_t));
    uint32_t value = 0;
    for (size_t i = 0; i < sizeof(uint32_t); ++i) {
        value |= static_cast<uint32_t>(memory[address + i]) << (i * 8);
    }
    return value;
}

uint64_t Memory::read64(uint64_t address) const {
    check_address(address, sizeof(uint64_t));
    uint64_t value = 0;
    for (size_t i = 0; i < sizeof(uint64_t); ++i) {
        value |= static_cast<uint64_t>(memory[address + i]) << (i * 8);
    }
    return value;
}

void Memory::write8(uint64_t address, uint8_t value) {
    check_address(address, sizeof(uint8_t));
    memory[address] = value;
}

void Memory::write32(uint64_t address, uint32_t value) {
    check_address(address, sizeof(uint32_t));
    for (size_t i = 0; i < sizeof(uint32_t); ++i) {
        memory[address + i] = static_cast<uint8_t>(value >> (i * 8));
    }
}

void Memory::write64(uint64_t address, uint64_t value) {
    check_address(address, sizeof(uint64_t));
    for (size_t i = 0; i < sizeof(uint64_t); ++i) {
        memory[address + i] = static_cast<uint8_t>(value >> (i * 8));
    }
}

void Memory::load_binary(uint64_t address, const std::vector<uint8_t>& data) {
    check_address(address, data.size());
    std::copy(data.begin(), data.end(), memory.begin() + address);
}

std::string Memory::dump_memory(uint64_t start, uint64_t end) const {
    if (end >= memory.size()) end = memory.size() - 1;
    if (start >= end) return "";
    
    std::ostringstream oss;
    oss << std::hex << std::setfill('0');
    
    for (uint64_t addr = start; addr <= end; addr += 16) {
        oss << "0x" << std::setw(16) << addr << ": ";
        
        // Print hex values
        for (uint64_t i = 0; i < 16 && addr + i <= end; ++i) {
            if (i > 0 && i % 4 == 0) oss << " ";
            oss << std::setw(2) << static_cast<unsigned>(memory[addr + i]) << " ";
        }
        
        // Print ASCII
        oss << " |";
        for (uint64_t i = 0; i < 16 && addr + i <= end; ++i) {
            char c = memory[addr + i];
            oss << (c >= 32 && c < 127 ? c : '.');
        }
        oss << "|\n";
    }
    
    return oss.str();
}

} // namespace arm_emulator
