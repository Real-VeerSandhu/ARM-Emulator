#pragma once

#include <cstdint>
#include <vector>
#include <stdexcept>
#include <string>

namespace arm_emulator {

class Memory {
public:
    // Initialize memory with the specified size in bytes
    explicit Memory(size_t size = 1024 * 1024);  // Default to 1MB
    
    // Reset all memory to zero
    void reset() noexcept;
    
    // Memory access methods with bounds checking
    uint8_t read8(uint64_t address) const;
    uint32_t read32(uint64_t address) const;
    uint64_t read64(uint64_t address) const;
    
    void write8(uint64_t address, uint8_t value);
    void write32(uint64_t address, uint32_t value);
    void write64(uint64_t address, uint64_t value);
    
    // Load binary data into memory at the specified address
    void load_binary(uint64_t address, const std::vector<uint8_t>& data);
    
    // Get the size of the memory in bytes
    size_t size() const noexcept { return memory.size(); }
    
    // Dump memory region to string (for debugging)
    std::string dump_memory(uint64_t start, uint64_t end) const;

private:
    std::vector<uint8_t> memory;
    
    // Helper method to check if an address is valid
    void check_address(uint64_t address, size_t size) const;
};

} // namespace arm_emulator
