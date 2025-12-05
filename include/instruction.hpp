#pragma once

#include <cstdint>
#include <string>

namespace arm_emulator {

// ARM instruction opcodes
enum class Opcode {
    // Data processing - register
    ADD,
    SUB,
    AND,
    ORR,
    EOR,
    
    // Data processing - immediate
    ADDI,
    SUBI,
    ANDI,
    ORRI,
    EORI,
    
    // Load/Store
    LDUR,
    STUR,
    
    // Branch
    B,
    BL,
    BR,
    BLR,
    RET,
    
    // Compare and branch
    CBZ,
    CBNZ,
    
    // Invalid/unknown opcode
    INVALID
};

// Addressing mode for memory operations
enum class AddrMode {
    OFFSET,     // Base + offset
    PRE_INDEX,  // Pre-indexed
    POST_INDEX, // Post-indexed
    LITERAL     // PC-relative literal load
};

// Condition codes for conditional execution
enum class Condition {
    EQ, // Equal
    NE, // Not equal
    CS, // Carry set (HS - unsigned higher or same)
    CC, // Carry clear (LO - unsigned lower)
    MI, // Minus/negative
    PL, // Plus/positive or zero
    VS, // Overflow
    VC, // No overflow
    HI, // Unsigned higher
    LS, // Unsigned lower or same
    GE, // Signed greater than or equal
    LT, // Signed less than
    GT, // Signed greater than
    LE, // Signed less than or equal
    AL, // Always (unconditional)
    NV  // Never (reserved)
};

// Structure representing a single ARM instruction
struct Instruction {
    Opcode opcode{Opcode::INVALID};
    Condition cond{Condition::AL};  // Default to always execute
    
    // Register operands (rd, rn, rm, ra)
    uint8_t rd{0};
    uint8_t rn{0};
    uint8_t rm{0};
    uint8_t ra{0};  // For some instructions like MADD
    
    // Immediate values
    int64_t imm{0};
    uint8_t shift{0};  // Shift amount
    
    // Memory addressing
    AddrMode addr_mode{AddrMode::OFFSET};
    bool wback{false};  // Writeback flag for pre/post-indexed addressing
    
    // Original assembly text (for debugging)
    std::string raw_text;
    
    // Helper methods
    bool is_branch() const;
    bool is_memory_op() const;
    bool is_conditional() const { return cond != Condition::AL; }
    
    // Convert instruction to string for debugging
    std::string to_string() const;
};

} // namespace arm_emulator
