#include "decoder.hpp"
#include <stdexcept>
#include <cassert>

namespace arm_emulator {

Instruction Decoder::decode(uint32_t instruction) {
    Instruction instr;
    
    // Extract the opcode (bits 24-28)
    uint8_t op0 = (instruction >> 25) & 0x7;
    (void)op0; // Suppress unused variable warning
    
    // Check for Data Processing - Register instructions (op0 == 0x2)
    if (op0 == 0x2) {
        return decode_data_processing_register(instruction);
    }
    
    // Check for Data Processing - Immediate instructions (op0 == 0x1 or 0x3)
    if (op0 == 0x1 || op0 == 0x3) {
        return decode_data_processing_immediate(instruction);
    }
    
    // Check for Load/Store instructions (op0 == 0x0 or 0x4)
    if (op0 == 0x0 || op0 == 0x4) {
        return decode_load_store(instruction);
    }
    
    // Check for Branch instructions (op0 == 0x5)
    if (op0 == 0x5) {
        return decode_branch(instruction);
    }
    
    // If we get here, the instruction is not supported
    instr.opcode = Opcode::INVALID;
    return instr;
}

Instruction Decoder::decode_data_processing_register(uint32_t instruction) {
    Instruction instr;
    
    // Extract fields
    uint8_t opcode = (instruction >> 21) & 0xF;
    // is_64bit is not currently used in the implementation
    // bool is_64bit = (instruction >> 31) & 0x1;
    
    // Set the appropriate opcode
    switch (opcode) {
        case 0x0: instr.opcode = Opcode::AND; break;
        case 0x1: instr.opcode = Opcode::EOR; break;
        case 0x2: instr.opcode = Opcode::SUB; break;
        case 0x4: instr.opcode = Opcode::ADD; break;
        case 0xA: instr.opcode = Opcode::ORR; break;
        default:
            instr.opcode = Opcode::INVALID;
            return instr;
    }
    
    // Set the registers
    instr.rd = instruction & 0x1F;
    instr.rn = (instruction >> 5) & 0x1F;
    instr.rm = (instruction >> 16) & 0x1F;
    
    // Set the shift amount (if any)
    uint8_t shift = (instruction >> 22) & 0x3;
    if (shift != 0) {
        instr.shift = shift;
    }
    
    return instr;
}

Instruction Decoder::decode_data_processing_immediate(uint32_t instruction) {
    Instruction instr;
    
    // Extract fields
    uint8_t opcode = (instruction >> 23) & 0x3;
    // is_64bit is not currently used in the implementation
    // bool is_64bit = (instruction >> 31) & 0x1;
    
    // Set the appropriate opcode
    switch (opcode) {
        case 0x0: instr.opcode = Opcode::ADDI; break;
        case 0x1: instr.opcode = Opcode::SUBI; break;
        case 0x2: instr.opcode = Opcode::ANDI; break;
        case 0x3: instr.opcode = Opcode::ORRI; break;
        default:
            instr.opcode = Opcode::INVALID;
            return instr;
    }
    
    // Set the registers and immediate
    instr.rd = instruction & 0x1F;
    instr.rn = (instruction >> 5) & 0x1F;
    
    // Extract and sign-extend the immediate
    int32_t imm12 = (instruction >> 10) & 0xFFF;
    if (imm12 & 0x800) {  // Sign extend 12-bit immediate
        imm12 |= 0xFFFFF000;
    }
    instr.imm = imm12;
    
    return instr;
}

Instruction Decoder::decode_load_store(uint32_t instruction) {
    Instruction instr;
    
    // Check if it's a load or store
    bool is_load = (instruction >> 22) & 0x1;
    // is_64bit is not currently used in the implementation
    // bool is_64bit = (instruction >> 30) & 0x1;
    
    // Set the opcode
    instr.opcode = is_load ? Opcode::LDUR : Opcode::STUR;
    
    // Set the registers
    instr.rd = instruction & 0x1F;  // Target register
    instr.rn = (instruction >> 5) & 0x1F;  // Base register
    
    // Extract and sign-extend the offset
    int32_t offset = (instruction >> 10) & 0x1FFFFF;
    if (offset & 0x100000) {  // Sign extend 21-bit offset
        offset |= 0xFFE00000;
    }
    instr.imm = offset;
    
    return instr;
}

Instruction Decoder::decode_branch(uint32_t instruction) {
    Instruction instr;
    
    // Check the opcode
    uint8_t opcode = (instruction >> 26) & 0x3F;
    
    switch (opcode) {
        case 0x0:  // B (unconditional)
            instr.opcode = Opcode::B;
            break;
            
        case 0x1:  // BL (branch with link)
            instr.opcode = Opcode::BL;
            break;
            
        case 0x2:  // BR (branch to register)
            instr.opcode = Opcode::BR;
            instr.rn = instruction & 0x1F;  // Register to branch to
            return instr;
            
        case 0x3:  // BLR (branch with link to register)
            instr.opcode = Opcode::BLR;
            instr.rn = instruction & 0x1F;  // Register to branch to
            return instr;
            
        case 0x4:  // RET (return from subroutine)
            instr.opcode = Opcode::RET;
            instr.rn = (instruction & 0x1F) ? (instruction & 0x1F) : 30;  // Default to X30 if not specified
            return instr;
            
        default:
            // Check for CBZ/CBNZ
            if ((opcode & 0x3C) == 0x24) {  // CBZ/CBNZ
                bool is_cbnz = (instruction >> 24) & 0x1;
                instr.opcode = is_cbnz ? Opcode::CBNZ : Opcode::CBZ;
                instr.rd = instruction & 0x1F;
                
                // Extract and sign-extend the offset
                int32_t offset = (instruction >> 5) & 0x7FFFF;
                if (offset & 0x40000) {  // Sign extend 19-bit offset
                    offset |= 0xFFF80000;
                }
                instr.imm = offset * 4;  // Scale by 4 for byte offset
                
                return instr;
            }
            
            // Unsupported instruction
            instr.opcode = Opcode::INVALID;
            return instr;
    }
    
    // For B and BL instructions, extract the offset
    int32_t offset = instruction & 0x3FFFFFF;
    if (offset & 0x2000000) {  // Sign extend 26-bit offset
        offset |= 0xFC000000;
    }
    instr.imm = offset * 4;  // Scale by 4 for byte offset
    
    return instr;
}

} // namespace arm_emulator
