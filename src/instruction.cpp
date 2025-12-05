#include "instruction.hpp"
#include <sstream>
#include <iomanip>
#include <stdexcept>

namespace arm_emulator {

bool Instruction::is_branch() const {
    return opcode == Opcode::B || opcode == Opcode::BL || 
           opcode == Opcode::BR || opcode == Opcode::BLR ||
           opcode == Opcode::RET || opcode == Opcode::CBZ ||
           opcode == Opcode::CBNZ;
}

bool Instruction::is_memory_op() const {
    return opcode == Opcode::LDUR || opcode == Opcode::STUR;
}

std::string Instruction::to_string() const {
    std::ostringstream oss;
    
    // Opcode
    switch (opcode) {
        case Opcode::ADD:  oss << "ADD"; break;
        case Opcode::SUB:  oss << "SUB"; break;
        case Opcode::AND:  oss << "AND"; break;
        case Opcode::ORR:  oss << "ORR"; break;
        case Opcode::EOR:  oss << "EOR"; break;
        case Opcode::ADDI: oss << "ADDI"; break;
        case Opcode::SUBI: oss << "SUBI"; break;
        case Opcode::ANDI: oss << "ANDI"; break;
        case Opcode::ORRI: oss << "ORRI"; break;
        case Opcode::EORI: oss << "EORI"; break;
        case Opcode::LDUR: oss << "LDUR"; break;
        case Opcode::STUR: oss << "STUR"; break;
        case Opcode::B:    oss << "B"; break;
        case Opcode::BL:   oss << "BL"; break;
        case Opcode::BR:   oss << "BR"; break;
        case Opcode::BLR:  oss << "BLR"; break;
        case Opcode::RET:  oss << "RET"; break;
        case Opcode::CBZ:  oss << "CBZ"; break;
        case Opcode::CBNZ: oss << "CBNZ"; break;
        case Opcode::INVALID: oss << "INVALID"; break;
    }
    
    // Condition code (if conditional)
    if (is_conditional()) {
        switch (cond) {
            case Condition::EQ: oss << ".EQ"; break;
            case Condition::NE: oss << ".NE"; break;
            case Condition::CS: oss << ".CS"; break;
            case Condition::CC: oss << ".CC"; break;
            case Condition::MI: oss << ".MI"; break;
            case Condition::PL: oss << ".PL"; break;
            case Condition::VS: oss << ".VS"; break;
            case Condition::VC: oss << ".VC"; break;
            case Condition::HI: oss << ".HI"; break;
            case Condition::LS: oss << ".LS"; break;
            case Condition::GE: oss << ".GE"; break;
            case Condition::LT: oss << ".LT"; break;
            case Condition::GT: oss << ".GT"; break;
            case Condition::LE: oss << ".LE"; break;
            case Condition::AL: break;  // No suffix for always
            case Condition::NV: oss << ".NV"; break;
        }
    }
    
    // Operands
    switch (opcode) {
        // Format: OP Rd, Rn, Rm
        case Opcode::ADD:
        case Opcode::SUB:
        case Opcode::AND:
        case Opcode::ORR:
        case Opcode::EOR:
            oss << " X" << static_cast<int>(rd) 
                << ", X" << static_cast<int>(rn)
                << ", X" << static_cast<int>(rm);
            if (shift > 0) {
                oss << ", LSL #" << static_cast<int>(shift);
            }
            break;
            
        // Format: OP Rd, Rn, #imm
        case Opcode::ADDI:
        case Opcode::SUBI:
        case Opcode::ANDI:
        case Opcode::ORRI:
        case Opcode::EORI:
            oss << " X" << static_cast<int>(rd)
                << ", X" << static_cast<int>(rn)
                << ", #" << imm;
            break;
            
        // Format: OP Xt, [Xn, #offset]
        case Opcode::LDUR:
        case Opcode::STUR: {
            oss << (opcode == Opcode::LDUR ? " X" : " X") 
                << static_cast<int>(rd)
                << ", [X" << static_cast<int>(rn);
            if (imm != 0) {
                oss << ", #" << imm;
            }
            oss << "]";
            break;
        }
            
        // Format: B #offset
        case Opcode::B:
        case Opcode::BL:
            oss << " #" << imm;
            break;
            
        // Format: BR/BLR Xn
        case Opcode::BR:
        case Opcode::BLR:
            oss << " X" << static_cast<int>(rn);
            break;
            
        // Format: RET [Xn]
        case Opcode::RET:
            if (rn != 30) {  // Default is X30 if not specified
                oss << " X" << static_cast<int>(rn);
            }
            break;
            
        // Format: CBZ/CBNZ Xt, #offset
        case Opcode::CBZ:
        case Opcode::CBNZ:
            oss << " X" << static_cast<int>(rd)
                << ", #" << imm;
            break;
            
        case Opcode::INVALID:
            oss << " <invalid>";
            break;
    }
    
    // Add raw text if available
    if (!raw_text.empty()) {
        oss << " ; " << raw_text;
    }
    
    return oss.str();
}

} // namespace arm_emulator
