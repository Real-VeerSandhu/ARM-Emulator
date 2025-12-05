#pragma once

#include "instruction.hpp"

namespace arm_emulator {

class Decoder {
public:
    // Decode a 32-bit ARM instruction into our internal representation
    static Instruction decode(uint32_t instruction);
    
private:
    // Helper methods for different instruction types
    static Instruction decode_data_processing_register(uint32_t instruction);
    static Instruction decode_data_processing_immediate(uint32_t instruction);
    static Instruction decode_load_store(uint32_t instruction);
    static Instruction decode_branch(uint32_t instruction);
};

} // namespace arm_emulator
