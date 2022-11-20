#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include "../common/types.hpp"
#include <string>

namespace standardInstructions
{
enum OpCode : byte
{
    NOP = 0x00,
    LD_BC_nn = 0x01,
    LD_BCm_A = 0x02,
    INC_BC = 0x03,
    INC_B = 0x04,
    DEC_B = 0x05,
    LD_B_n = 0x06,
    RLC_A = 0x07,
    LD_nnm_SP = 0x08,
    ADD_HL_BC = 0x09,
    LD_A_BCm = 0x0A,
    DEC_BC = 0x0B,
    INC_C = 0x0C,
    DEC_C = 0x0D,
    LD_C_n = 0x0E,
    RRC_A = 0x0F,

    STOP = 0x10,
    LD_DE_nn = 0x11,
    LD_DEm_A = 0x12,
    INC_DE = 0x13,
    INC_D = 0x14,
    DEC_D = 0x15,
    LD_D_n = 0x16,
    RL_A = 0x17,
    JR_n = 0x18,
    ADD_HL_DE = 0x19,
    LD_A_DEm = 0x1A,
    DEC_DE = 0x1B,
    INC_E = 0x1C,
    DEC_E = 0x1D,
    LD_E_n = 0x1E,
    RR_A = 0x1F,

    JR_NZ_n = 0x20,
    LD_HL_nn = 0x21,
    LD_HLm_I_A = 0x22,
    INC_HL = 0x23,
    INC_H = 0x24,
    DEC_H = 0x25,
    LD_H_n = 0x26,
    DAA = 0x27,
    JR_Z_n = 0x28,
    ADD_HL_HL = 0x29,
    LD_A_HLm_I = 0x2A,
    DEC_HL = 0x2B,
    INC_L = 0x2C,
    DEC_L = 0x2D,
    LD_L_n = 0x2E,
    CPL = 0x2F,

    JR_NC_n = 0x30,
    LD_SP_nn = 0x31,
    LD_HLm_D_A = 0x32,
    INC_SP = 0x33,
    INC_HLm = 0x34,
    DEC_HLm = 0x35,
    LD_HLm_n = 0x36,
    SCF = 0x37,
    JR_C_n = 0x38,
    ADD_HL_SP = 0x39,
    LD_A_HLm_D = 0x3A,
    DEC_SP = 0x3B,
    INC_A = 0x3C,
    DEC_A = 0x3D,
    LD_A_n = 0x3E,
    CCF = 0x3F,

    LD_B_B = 0x40,
    LD_B_C = 0x41,
    LD_B_D = 0x42,
    LD_B_E = 0x43,
    LD_B_H = 0x44,
    LD_B_L = 0x45,
    LD_B_HLm = 0x46,
    LD_B_A = 0x47,
    LD_C_B = 0x48,
    LD_C_C = 0x49,
    LD_C_D = 0x4A,
    LD_C_E = 0x4B,
    LD_C_H = 0x4C,
    LD_C_L = 0x4D,
    LD_C_HLm = 0x4E,
    LD_C_A = 0x4F,

    LD_D_B = 0x50,
    LD_D_C = 0x51,
    LD_D_D = 0x52,
    LD_D_E = 0x53,
    LD_D_H = 0x54,
    LD_D_L = 0x55,
    LD_D_HLm = 0x56,
    LD_D_A = 0x57,
    LD_E_B = 0x58,
    LD_E_C = 0x59,
    LD_E_D = 0x5A,
    LD_E_E = 0x5B,
    LD_E_H = 0x5C,
    LD_E_L = 0x5D,
    LD_E_HLm = 0x5E,
    LD_E_A = 0x5F,

    LD_H_B = 0x60,
    LD_H_C = 0x61,
    LD_H_D = 0x62,
    LD_H_E = 0x63,
    LD_H_H = 0x64,
    LD_H_L = 0x65,
    LD_H_HLm = 0x66,
    LD_H_A = 0x67,
    LD_L_B = 0x68,
    LD_L_C = 0x69,
    LD_L_D = 0x6A,
    LD_L_E = 0x6B,
    LD_L_H = 0x6C,
    LD_L_L = 0x6D,
    LD_L_HLm = 0x6E,
    LD_L_A = 0x6F,

    LD_HLm_B = 0x70,
    LD_HLm_C = 0x71,
    LD_HLm_D = 0x72,
    LD_HLm_E = 0x73,
    LD_HLm_H = 0x74,
    LD_HLm_L = 0x75,
    HALT = 0x76,
    LD_HLm_A = 0x77,
    LD_A_B = 0x78,
    LD_A_C = 0x79,
    LD_A_D = 0x7A,
    LD_A_E = 0x7B,
    LD_A_H = 0x7C,
    LD_A_L = 0x7D,
    LD_A_HLm = 0x7E,
    LD_A_A = 0x7F,

    ADD_A_B = 0x80,
    ADD_A_C = 0x81,
    ADD_A_D = 0x82,
    ADD_A_E = 0x83,
    ADD_A_H = 0x84,
    ADD_A_L = 0x85,
    ADD_A_HLm = 0x86,
    ADD_A_A = 0x87,
    ADC_A_B = 0x88,
    ADC_A_C = 0x89,
    ADC_A_D = 0x8A,
    ADC_A_E = 0x8B,
    ADC_A_H = 0x8C,
    ADC_A_L = 0x8D,
    ADC_A_HLm = 0x8E,
    ADC_A_A = 0x8F,

    SUB_A_B = 0x90,
    SUB_A_C = 0x91,
    SUB_A_D = 0x92,
    SUB_A_E = 0x93,
    SUB_A_H = 0x94,
    SUB_A_L = 0x95,
    SUB_A_HLm = 0x96,
    SUB_A_A = 0x97,
    SBC_A_B = 0x98,
    SBC_A_C = 0x99,
    SBC_A_D = 0x9A,
    SBC_A_E = 0x9B,
    SBC_A_H = 0x9C,
    SBC_A_L = 0x9D,
    SBC_A_HLm = 0x9E,
    SBC_A_A = 0x9F,

    AND_B = 0xA0,
    AND_C = 0xA1,
    AND_D = 0xA2,
    AND_E = 0xA3,
    AND_H = 0xA4,
    AND_L = 0xA5,
    AND_HLm = 0xA6,
    AND_A = 0xA7,
    XOR_B = 0xA8,
    XOR_C = 0xA9,
    XOR_D = 0xAA,
    XOR_E = 0xAB,
    XOR_H = 0xAC,
    XOR_L = 0xAD,
    XOR_HLm = 0xAE,
    XOR_A = 0xAF,

    OR_B = 0xB0,
    OR_C = 0xB1,
    OR_D = 0xB2,
    OR_E = 0xB3,
    OR_H = 0xB4,
    OR_L = 0xB5,
    OR_HLm = 0xB6,
    OR_A = 0xB7,
    CP_B = 0xB8,
    CP_C = 0xB9,
    CP_D = 0xBA,
    CP_E = 0xBB,
    CP_H = 0xBC,
    CP_L = 0xBD,
    CP_HLm = 0xBE,
    CP_A = 0xBF,

    RET_NZ = 0xC0,
    POP_BC = 0xC1,
    JP_NZ_nn = 0xC2,
    JP_nn = 0xC3,
    CALL_NZ_nn = 0xC4,
    PUSH_BC = 0xC5,
    ADD_A_n = 0xC6,
    RST_0 = 0xC7,
    RET_Z = 0xC8,
    RET = 0xC9,
    JP_Z_nn = 0xCA,
    EXT_OPS = 0xCB,
    CALL_Z_nn = 0xCC,
    CALL_nn = 0xCD,
    ADC_A_n = 0xCE,
    RST_8 = 0xCF,

    RET_NC = 0xD0,
    POP_DE = 0xD1,
    JP_NC_nn = 0xD2,
    /* NOT IMPLEMENTED		0xD3*/
    CALL_NC_nn = 0xD4,
    PUSH_DE = 0xD5,
    SUB_A_n = 0xD6,
    RST_10 = 0xD7,
    RET_C = 0xD8,
    RETI = 0xD9,
    JP_C_nn = 0xDA,
    /* NOT IMPLEMENTED		0xDB*/
    CALL_C_nn = 0xDC,
    /* NOT IMPLEMENTED		0xDD*/
    SBC_A_n = 0xDE,
    RST_18 = 0xDF,

    LDH_nm_A = 0xE0,
    POP_HL = 0xE1,
    LD_Cm_A = 0xE2,
    /* NOT IMPLEMENTED		0xE4*/
    PUSH_HL = 0xE5,
    AND_n = 0xE6,
    RST_20 = 0xE7,
    ADD_SP_d = 0xE8,
    JP_HLm = 0xE9,
    LD_nnm_A = 0xEA,
    /* NOT IMPLEMENTED		0xEB*/
    /* NOT IMPLEMENTED		0xEC*/
    /* NOT IMPLEMENTED		0xED*/
    XOR_n = 0xEE,
    RST_28 = 0xEF,

    LDH_A_nm = 0xF0,
    POP_AF = 0xF1,
    LD_A_Cm = 0xF2,
    DI = 0xF3,
    /* NOT IMPLEMENTED		0xF4*/
    PUSH_AF = 0xF5,
    OR_n = 0xF6,
    RST_30 = 0xF7,
    LDHL_SP_d = 0xF8,
    LD_SP_HL = 0xF9,
    LD_A_nnm = 0xFA,
    EI = 0xFB,
    /* NOT IMPLEMENTED		0xFC*/
    /* NOT IMPLEMENTED		0xFD*/
    CP_n = 0xFE,
    RST_38 = 0xFF
};

static std::string opCodeToString(OpCode op)
{
    switch (op)
    {
    case NOP:
        return "NOP";
    case LD_BC_nn:
        return "LD_BC_nn";
    case LD_BCm_A:
        return "LD_BCm_A";
    case INC_BC:
        return "INC_BC";
    case INC_B:
        return "INC_B";
    case DEC_B:
        return "DEC_B";
    case LD_B_n:
        return "LD_B_n";
    case RLC_A:
        return "RLC_A";
    case LD_nnm_SP:
        return "LD_nnm_SP";
    case ADD_HL_BC:
        return "ADD_HL_BC";
    case LD_A_BCm:
        return "LD_A_BCm";
    case DEC_BC:
        return "DEC_BC";
    case INC_C:
        return "INC_C";
    case DEC_C:
        return "DEC_C";
    case LD_C_n:
        return "LD_C_n";
    case RRC_A:
        return "RRC_A";

    case STOP:
        return "STOP";
    case LD_DE_nn:
        return "LD_DE_nn";
    case LD_DEm_A:
        return "LD_DEm_A";
    case INC_DE:
        return "INC_DE";
    case INC_D:
        return "INC_D";
    case DEC_D:
        return "DEC_D";
    case LD_D_n:
        return "LD_D_n";
    case RL_A:
        return "RL_A";
    case JR_n:
        return "JR_n";
    case ADD_HL_DE:
        return "ADD_HL_DE";
    case LD_A_DEm:
        return "LD_A_DEm";
    case DEC_DE:
        return "DEC_DE";
    case INC_E:
        return "INC_E";
    case DEC_E:
        return "DEC_E";
    case LD_E_n:
        return "LD_E_n";
    case RR_A:
        return "RR_A";

    case JR_NZ_n:
        return "JR_NZ_n";
    case LD_HL_nn:
        return "LD_HL_nn";
    case LD_HLm_I_A:
        return "LD_HLm_I_A";
    case INC_HL:
        return "INC_HL";
    case INC_H:
        return "INC_H";
    case DEC_H:
        return "DEC_H";
    case LD_H_n:
        return "LD_H_n";
    case DAA:
        return "DAA";
    case JR_Z_n:
        return "JR_Z_n";
    case ADD_HL_HL:
        return "ADD_HL_HL";
    case LD_A_HLm_I:
        return "LD_A_HLm_I";
    case DEC_HL:
        return "DEC_HL";
    case INC_L:
        return "INC_L";
    case DEC_L:
        return "DEC_L";
    case LD_L_n:
        return "LD_L_n";
    case CPL:
        return "CPL";

    case JR_NC_n:
        return "JR_NC_n";
    case LD_SP_nn:
        return "LD_SP_nn";
    case LD_HLm_D_A:
        return "LD_HLm_D_A";
    case INC_SP:
        return "INC_SP";
    case INC_HLm:
        return "INC_HLm";
    case DEC_HLm:
        return "DEC_HLm";
    case LD_HLm_n:
        return "LD_HLm_n";
    case SCF:
        return "SCF";
    case JR_C_n:
        return "JR_C_n";
    case ADD_HL_SP:
        return "ADD_HL_SP";
    case LD_A_HLm_D:
        return "LD_A_HLm_D";
    case DEC_SP:
        return "DEC_SP";
    case INC_A:
        return "INC_A";
    case DEC_A:
        return "DEC_A";
    case LD_A_n:
        return "LD_A_n";
    case CCF:
        return "CCF";

    case LD_B_B:
        return "LD_B_B";
    case LD_B_C:
        return "LD_B_C";
    case LD_B_D:
        return "LD_B_D";
    case LD_B_E:
        return "LD_B_E";
    case LD_B_H:
        return "LD_B_H";
    case LD_B_L:
        return "LD_B_L";
    case LD_B_HLm:
        return "LD_B_HLm";
    case LD_B_A:
        return "LD_B_A";
    case LD_C_B:
        return "LD_C_B";
    case LD_C_C:
        return "LD_C_C";
    case LD_C_D:
        return "LD_C_D";
    case LD_C_E:
        return "LD_C_E";
    case LD_C_H:
        return "LD_C_H";
    case LD_C_L:
        return "LD_C_L";
    case LD_C_HLm:
        return "LD_C_HLm";
    case LD_C_A:
        return "LD_C_A";

    case LD_D_B:
        return "LD_D_B";
    case LD_D_C:
        return "LD_D_C";
    case LD_D_D:
        return "LD_D_D";
    case LD_D_E:
        return "LD_D_E";
    case LD_D_H:
        return "LD_D_H";
    case LD_D_L:
        return "LD_D_L";
    case LD_D_HLm:
        return "LD_D_HLm";
    case LD_D_A:
        return "LD_D_A";
    case LD_E_B:
        return "LD_E_B";
    case LD_E_C:
        return "LD_E_C";
    case LD_E_D:
        return "LD_E_D";
    case LD_E_E:
        return "LD_E_E";
    case LD_E_H:
        return "LD_E_H";
    case LD_E_L:
        return "LD_E_L";
    case LD_E_HLm:
        return "LD_E_HLm";
    case LD_E_A:
        return "LD_E_A";

    case LD_H_B:
        return "LD_H_B";
    case LD_H_C:
        return "LD_H_C";
    case LD_H_D:
        return "LD_H_D";
    case LD_H_E:
        return "LD_H_E";
    case LD_H_H:
        return "LD_H_H";
    case LD_H_L:
        return "LD_H_L";
    case LD_H_HLm:
        return "LD_H_HLm";
    case LD_H_A:
        return "LD_H_A";
    case LD_L_B:
        return "LD_L_B";
    case LD_L_C:
        return "LD_L_C";
    case LD_L_D:
        return "LD_L_D";
    case LD_L_E:
        return "LD_L_E";
    case LD_L_H:
        return "LD_L_H";
    case LD_L_L:
        return "LD_L_L";
    case LD_L_HLm:
        return "LD_L_HLm";
    case LD_L_A:
        return "LD_L_A";

    case LD_HLm_B:
        return "LD_HLm_B";
    case LD_HLm_C:
        return "LD_HLm_C";
    case LD_HLm_D:
        return "LD_HLm_D";
    case LD_HLm_E:
        return "LD_HLm_E";
    case LD_HLm_H:
        return "LD_HLm_H";
    case LD_HLm_L:
        return "LD_HLm_L";
    case HALT:
        return "HALT";
    case LD_HLm_A:
        return "LD_HLm_A";
    case LD_A_B:
        return "LD_A_B";
    case LD_A_C:
        return "LD_A_C";
    case LD_A_D:
        return "LD_A_D";
    case LD_A_E:
        return "LD_A_E";
    case LD_A_H:
        return "LD_A_H";
    case LD_A_L:
        return "LD_A_L";
    case LD_A_HLm:
        return "LD_A_HLm";
    case LD_A_A:
        return "LD_A_A";

    case ADD_A_B:
        return "ADD_A_B";
    case ADD_A_C:
        return "ADD_A_C";
    case ADD_A_D:
        return "ADD_A_D";
    case ADD_A_E:
        return "ADD_A_E";
    case ADD_A_H:
        return "ADD_A_H";
    case ADD_A_L:
        return "ADD_A_L";
    case ADD_A_HLm:
        return "ADD_A_HLm";
    case ADD_A_A:
        return "ADD_A_A";
    case ADC_A_B:
        return "ADC_A_B";
    case ADC_A_C:
        return "ADC_A_C";
    case ADC_A_D:
        return "ADC_A_D";
    case ADC_A_E:
        return "ADC_A_E";
    case ADC_A_H:
        return "ADC_A_H";
    case ADC_A_L:
        return "ADC_A_L";
    case ADC_A_HLm:
        return "ADC_A_HLm";
    case ADC_A_A:
        return "ADC_A_A";

    case SUB_A_B:
        return "SUB_A_B";
    case SUB_A_C:
        return "SUB_A_C";
    case SUB_A_D:
        return "SUB_A_D";
    case SUB_A_E:
        return "SUB_A_E";
    case SUB_A_H:
        return "SUB_A_H";
    case SUB_A_L:
        return "SUB_A_L";
    case SUB_A_HLm:
        return "SUB_A_HLm";
    case SUB_A_A:
        return "SUB_A_A";
    case SBC_A_B:
        return "SBC_A_B";
    case SBC_A_C:
        return "SBC_A_C";
    case SBC_A_D:
        return "SBC_A_D";
    case SBC_A_E:
        return "SBC_A_E";
    case SBC_A_H:
        return "SBC_A_H";
    case SBC_A_L:
        return "SBC_A_L";
    case SBC_A_HLm:
        return "SBC_A_HLm";
    case SBC_A_A:
        return "SBC_A_A";

    case AND_B:
        return "AND_B";
    case AND_C:
        return "AND_C";
    case AND_D:
        return "AND_D";
    case AND_E:
        return "AND_E";
    case AND_H:
        return "AND_H";
    case AND_L:
        return "AND_L";
    case AND_HLm:
        return "AND_HLm";
    case AND_A:
        return "AND_A";
    case XOR_B:
        return "XOR_B";
    case XOR_C:
        return "XOR_C";
    case XOR_D:
        return "XOR_D";
    case XOR_E:
        return "XOR_E";
    case XOR_H:
        return "XOR_H";
    case XOR_L:
        return "XOR_L";
    case XOR_HLm:
        return "XOR_HLm";
    case XOR_A:
        return "XOR_A";

    case OR_B:
        return "OR_B";
    case OR_C:
        return "OR_C";
    case OR_D:
        return "OR_D";
    case OR_E:
        return "OR_E";
    case OR_H:
        return "OR_H";
    case OR_L:
        return "OR_L";
    case OR_HLm:
        return "OR_HLm";
    case OR_A:
        return "OR_A";
    case CP_B:
        return "CP_B";
    case CP_C:
        return "CP_C";
    case CP_D:
        return "CP_D";
    case CP_E:
        return "CP_E";
    case CP_H:
        return "CP_H";
    case CP_L:
        return "CP_L";
    case CP_HLm:
        return "CP_HLm";
    case CP_A:
        return "CP_A";

    case RET_NZ:
        return "RET_NZ";
    case POP_BC:
        return "POP_BC";
    case JP_NZ_nn:
        return "JP_NZ_nn";
    case JP_nn:
        return "JP_nn";
    case CALL_NZ_nn:
        return "CALL_NZ_nn";
    case PUSH_BC:
        return "PUSH_BC";
    case ADD_A_n:
        return "ADD_A_n";
    case RST_0:
        return "RST_0";
    case RET_Z:
        return "RET_Z";
    case RET:
        return "RET";
    case JP_Z_nn:
        return "JP_Z_nn";
    case EXT_OPS:
        return "EXT_OPS";
    case CALL_Z_nn:
        return "CALL_Z_nn";
    case CALL_nn:
        return "CALL_nn";
    case ADC_A_n:
        return "ADC_A_n";
    case RST_8:
        return "RST_8";

    case RET_NC:
        return "RET_NC";
    case POP_DE:
        return "POP_DE";
    case JP_NC_nn:
        return "JP_NC_nn";
        /* NOT IMPLEMENTED		0xD3*/
    case CALL_NC_nn:
        return "CALL_NC_nn";
    case PUSH_DE:
        return "PUSH_DE";
    case SUB_A_n:
        return "SUB_A_n";
    case RST_10:
        return "RST_10";
    case RET_C:
        return "RET_C";
    case RETI:
        return "RETI";
    case JP_C_nn:
        return "JP_C_nn";
        /* NOT IMPLEMENTED		0xDB*/
    case CALL_C_nn:
        return "CALL_C_nn";
        /* NOT IMPLEMENTED		0xDD*/
    case SBC_A_n:
        return "SBC_A_n";
    case RST_18:
        return "RST_18";

    case LDH_nm_A:
        return "LDH_nm_A";
    case POP_HL:
        return "POP_HL";
    case LD_Cm_A:
        return "LD_Cm_A";
        /* NOT IMPLEMENTED		0xE4*/
    case PUSH_HL:
        return "PUSH_HL";
    case AND_n:
        return "AND_n";
    case RST_20:
        return "RST_20";
    case ADD_SP_d:
        return "ADD_SP_d";
    case JP_HLm:
        return "JP_HLm";
    case LD_nnm_A:
        return "LD_nnm_A";
        /* NOT IMPLEMENTED		0xEB*/
        /* NOT IMPLEMENTED		0xEC*/
        /* NOT IMPLEMENTED		0xED*/
    case XOR_n:
        return "XOR_n";
    case RST_28:
        return "RST_28";

    case LDH_A_nm:
        return "LDH_A_nm";
    case POP_AF:
        return "POP_AF";
    case LD_A_Cm:
        return "LD_A_Cm";
    case DI:
        return "DI";
        /* NOT IMPLEMENTED		0xF4*/
    case PUSH_AF:
        return "PUSH_AF";
    case OR_n:
        return "OR_n";
    case RST_30:
        return "RST_30";
    case LDHL_SP_d:
        return "LDHL_SP_d";
    case LD_SP_HL:
        return "LD_SP_HL";
    case LD_A_nnm:
        return "LD_A_nnm";
    case EI:
        return "EI";
        /* NOT IMPLEMENTED		0xFC*/
        /* NOT IMPLEMENTED		0xFD*/
    case CP_n:
        return "CP_n";
    case RST_38:
        return "RST_38";
    default:
        return "UNHANDLED";
    }
}
} // namespace standardInstructions

namespace extendedInstructions
{
enum OpCode : uint8_t
{
    RLC_B = 0x00,
    RLC_C = 0x01,
    RLC_D = 0x02,
    RLC_E = 0x03,
    RLC_H = 0x04,
    RLC_L = 0x05,
    RLC_HLm = 0x06,
    RLC_A = 0x07,
    RRC_B = 0x08,
    RRC_C = 0x09,
    RRC_D = 0x0A,
    RRC_E = 0x0B,
    RRC_H = 0x0C,
    RRC_L = 0x0D,
    RRC_HLm = 0x0E,
    RRC_A = 0x0F,

    RL_B = 0x10,
    RL_C = 0x11,
    RL_D = 0x12,
    RL_E = 0x13,
    RL_H = 0x14,
    RL_L = 0x15,
    RL_HLm = 0x16,
    RL_A = 0x17,
    RR_B = 0x18,
    RR_C = 0x19,
    RR_D = 0x1A,
    RR_E = 0x1B,
    RR_H = 0x1C,
    RR_L = 0x1D,
    RR_HLm = 0x1E,
    RR_A = 0x1F,

    SLA_B = 0x20,
    SLA_C = 0x21,
    SLA_D = 0x22,
    SLA_E = 0x23,
    SLA_H = 0x24,
    SLA_L = 0x25,
    SLA_HLm = 0x26,
    SLA_A = 0x27,
    SRA_B = 0x28,
    SRA_C = 0x29,
    SRA_D = 0x2A,
    SRA_E = 0x2B,
    SRA_H = 0x2C,
    SRA_L = 0x2D,
    SRA_HLm = 0x2E,
    SRA_A = 0x2F,

    SWAP_B = 0x30,
    SWAP_C = 0x31,
    SWAP_D = 0x32,
    SWAP_E = 0x33,
    SWAP_H = 0x34,
    SWAP_L = 0x35,
    SWAP_HLm = 0x36,
    SWAP_A = 0x37,
    SRL_B = 0x38,
    SRL_C = 0x39,
    SRL_D = 0x3A,
    SRL_E = 0x3B,
    SRL_H = 0x3C,
    SRL_L = 0x3D,
    SRL_HLm = 0x3E,
    SRL_A = 0x3F,

    BIT_0_B = 0x40,
    BIT_0_C = 0x41,
    BIT_0_D = 0x42,
    BIT_0_E = 0x43,
    BIT_0_H = 0x44,
    BIT_0_L = 0x45,
    BIT_0_HLm = 0x46,
    BIT_0_A = 0x47,
    BIT_1_B = 0x48,
    BIT_1_C = 0x49,
    BIT_1_D = 0x4A,
    BIT_1_E = 0x4B,
    BIT_1_H = 0x4C,
    BIT_1_L = 0x4D,
    BIT_1_HLm = 0x4E,
    BIT_1_A = 0x4F,

    BIT_2_B = 0x50,
    BIT_2_C = 0x51,
    BIT_2_D = 0x52,
    BIT_2_E = 0x53,
    BIT_2_H = 0x54,
    BIT_2_L = 0x55,
    BIT_2_HLm = 0x56,
    BIT_2_A = 0x57,
    BIT_3_B = 0x58,
    BIT_3_C = 0x59,
    BIT_3_D = 0x5A,
    BIT_3_E = 0x5B,
    BIT_3_H = 0x5C,
    BIT_3_L = 0x5D,
    BIT_3_HLm = 0x5E,
    BIT_3_A = 0x5F,

    BIT_4_B = 0x60,
    BIT_4_C = 0x61,
    BIT_4_D = 0x62,
    BIT_4_E = 0x63,
    BIT_4_H = 0x64,
    BIT_4_L = 0x65,
    BIT_4_HLm = 0x66,
    BIT_4_A = 0x67,
    BIT_5_B = 0x68,
    BIT_5_C = 0x69,
    BIT_5_D = 0x6A,
    BIT_5_E = 0x6B,
    BIT_5_H = 0x6C,
    BIT_5_L = 0x6D,
    BIT_5_HLm = 0x6E,
    BIT_5_A = 0x6F,

    BIT_6_B = 0x70,
    BIT_6_C = 0x71,
    BIT_6_D = 0x72,
    BIT_6_E = 0x73,
    BIT_6_H = 0x74,
    BIT_6_L = 0x75,
    BIT_6_HLm = 0x76,
    BIT_6_A = 0x77,
    BIT_7_B = 0x78,
    BIT_7_C = 0x79,
    BIT_7_D = 0x7A,
    BIT_7_E = 0x7B,
    BIT_7_H = 0x7C,
    BIT_7_L = 0x7D,
    BIT_7_HLm = 0x7E,
    BIT_7_A = 0x7F,

    RES_0_B = 0x80,
    RES_0_C = 0x81,
    RES_0_D = 0x82,
    RES_0_E = 0x83,
    RES_0_H = 0x84,
    RES_0_L = 0x85,
    RES_0_HLm = 0x86,
    RES_0_A = 0x87,
    RES_1_B = 0x88,
    RES_1_C = 0x89,
    RES_1_D = 0x8A,
    RES_1_E = 0x8B,
    RES_1_H = 0x8C,
    RES_1_L = 0x8D,
    RES_1_HLm = 0x8E,
    RES_1_A = 0x8F,

    RES_2_B = 0x90,
    RES_2_C = 0x91,
    RES_2_D = 0x92,
    RES_2_E = 0x93,
    RES_2_H = 0x94,
    RES_2_L = 0x95,
    RES_2_HLm = 0x96,
    RES_2_A = 0x97,
    RES_3_B = 0x98,
    RES_3_C = 0x99,
    RES_3_D = 0x9A,
    RES_3_E = 0x9B,
    RES_3_H = 0x9C,
    RES_3_L = 0x9D,
    RES_3_HLm = 0x9E,
    RES_3_A = 0x9F,

    RES_4_B = 0xA0,
    RES_4_C = 0xA1,
    RES_4_D = 0xA2,
    RES_4_E = 0xA3,
    RES_4_H = 0xA4,
    RES_4_L = 0xA5,
    RES_4_HLm = 0xA6,
    RES_4_A = 0xA7,
    RES_5_B = 0xA8,
    RES_5_C = 0xA9,
    RES_5_D = 0xAA,
    RES_5_E = 0xAB,
    RES_5_H = 0xAC,
    RES_5_L = 0xAD,
    RES_5_HLm = 0xAE,
    RES_5_A = 0xAF,

    RES_6_B = 0xB0,
    RES_6_C = 0xB1,
    RES_6_D = 0xB2,
    RES_6_E = 0xB3,
    RES_6_H = 0xB4,
    RES_6_L = 0xB5,
    RES_6_HLm = 0xB6,
    RES_6_A = 0xB7,
    RES_7_B = 0xB8,
    RES_7_C = 0xB9,
    RES_7_D = 0xBA,
    RES_7_E = 0xBB,
    RES_7_H = 0xBC,
    RES_7_L = 0xBD,
    RES_7_HLm = 0xBE,
    RES_7_A = 0xBF,

    SET_0_B = 0xC0,
    SET_0_C = 0xC1,
    SET_0_D = 0xC2,
    SET_0_E = 0xC3,
    SET_0_H = 0xC4,
    SET_0_L = 0xC5,
    SET_0_HLm = 0xC6,
    SET_0_A = 0xC7,
    SET_1_B = 0xC8,
    SET_1_C = 0xC9,
    SET_1_D = 0xCA,
    SET_1_E = 0xCB,
    SET_1_H = 0xCC,
    SET_1_L = 0xCD,
    SET_1_HLm = 0xCE,
    SET_1_A = 0xCF,

    SET_2_B = 0xD0,
    SET_2_C = 0xD1,
    SET_2_D = 0xD2,
    SET_2_E = 0xD3,
    SET_2_H = 0xD4,
    SET_2_L = 0xD5,
    SET_2_HLm = 0xD6,
    SET_2_A = 0xD7,
    SET_3_B = 0xD8,
    SET_3_C = 0xD9,
    SET_3_D = 0xDA,
    SET_3_E = 0xDB,
    SET_3_H = 0xDC,
    SET_3_L = 0xDD,
    SET_3_HLm = 0xDE,
    SET_3_A = 0xDF,

    SET_4_B = 0xE0,
    SET_4_C = 0xE1,
    SET_4_D = 0xE2,
    SET_4_E = 0xE3,
    SET_4_H = 0xE4,
    SET_4_L = 0xE5,
    SET_4_HLm = 0xE6,
    SET_4_A = 0xE7,
    SET_5_B = 0xE8,
    SET_5_C = 0xE9,
    SET_5_D = 0xEA,
    SET_5_E = 0xEB,
    SET_5_H = 0xEC,
    SET_5_L = 0xED,
    SET_5_HLm = 0xEE,
    SET_5_A = 0xEF,

    SET_6_B = 0xF0,
    SET_6_C = 0xF1,
    SET_6_D = 0xF2,
    SET_6_E = 0xF3,
    SET_6_H = 0xF4,
    SET_6_L = 0xF5,
    SET_6_HLm = 0xF6,
    SET_6_A = 0xF7,
    SET_7_B = 0xF8,
    SET_7_C = 0xF9,
    SET_7_D = 0xFA,
    SET_7_E = 0xFB,
    SET_7_H = 0xFC,
    SET_7_L = 0xFD,
    SET_7_HLm = 0xFE,
    SET_7_A = 0xFF
};

static std::string opCodeToString(OpCode op)
{
    switch (op)
    {
    case RLC_B:
        return "RLC_B";
    case RLC_C:
        return "RLC_C";
    case RLC_D:
        return "RLC_D";
    case RLC_E:
        return "RLC_E";
    case RLC_H:
        return "RLC_H";
    case RLC_L:
        return "RLC_L";
    case RLC_HLm:
        return "RLC_HLm";
    case RLC_A:
        return "RLC_A";
    case RRC_B:
        return "RRC_B";
    case RRC_C:
        return "RRC_C";
    case RRC_D:
        return "RRC_D";
    case RRC_E:
        return "RRC_E";
    case RRC_H:
        return "RRC_H";
    case RRC_L:
        return "RRC_L";
    case RRC_HLm:
        return "RRC_HLm";
    case RRC_A:
        return "RRC_A";

    case RL_B:
        return "RL_B";
    case RL_C:
        return "RL_C";
    case RL_D:
        return "RL_D";
    case RL_E:
        return "RL_E";
    case RL_H:
        return "RL_H";
    case RL_L:
        return "RL_L";
    case RL_HLm:
        return "RL_HLm";
    case RL_A:
        return "RL_A";
    case RR_B:
        return "RR_B";
    case RR_C:
        return "RR_C";
    case RR_D:
        return "RR_D";
    case RR_E:
        return "RR_E";
    case RR_H:
        return "RR_H";
    case RR_L:
        return "RR_L";
    case RR_HLm:
        return "RR_HLm";
    case RR_A:
        return "RR_A";

    case SLA_B:
        return "SLA_B";
    case SLA_C:
        return "SLA_C";
    case SLA_D:
        return "SLA_D";
    case SLA_E:
        return "SLA_E";
    case SLA_H:
        return "SLA_H";
    case SLA_L:
        return "SLA_L";
    case SLA_HLm:
        return "SLA_HLm";
    case SLA_A:
        return "SLA_A";
    case SRA_B:
        return "SRA_B";
    case SRA_C:
        return "SRA_C";
    case SRA_D:
        return "SRA_D";
    case SRA_E:
        return "SRA_E";
    case SRA_H:
        return "SRA_H";
    case SRA_L:
        return "SRA_L";
    case SRA_HLm:
        return "SRA_HLm";
    case SRA_A:
        return "SRA_A";

    case SWAP_B:
        return "SWAP_B";
    case SWAP_C:
        return "SWAP_C";
    case SWAP_D:
        return "SWAP_D";
    case SWAP_E:
        return "SWAP_E";
    case SWAP_H:
        return "SWAP_H";
    case SWAP_L:
        return "SWAP_L";
    case SWAP_HLm:
        return "SWAP_HLm";
    case SWAP_A:
        return "SWAP_A";
    case SRL_B:
        return "SRL_B";
    case SRL_C:
        return "SRL_C";
    case SRL_D:
        return "SRL_D";
    case SRL_E:
        return "SRL_E";
    case SRL_H:
        return "SRL_H";
    case SRL_L:
        return "SRL_L";
    case SRL_HLm:
        return "SRL_HLm";
    case SRL_A:
        return "SRL_A";

    case BIT_0_B:
        return "BIT_0_B";
    case BIT_0_C:
        return "BIT_0_C";
    case BIT_0_D:
        return "BIT_0_D";
    case BIT_0_E:
        return "BIT_0_E";
    case BIT_0_H:
        return "BIT_0_H";
    case BIT_0_L:
        return "BIT_0_L";
    case BIT_0_HLm:
        return "BIT_0_HLm";
    case BIT_0_A:
        return "BIT_0_A";
    case BIT_1_B:
        return "BIT_1_B";
    case BIT_1_C:
        return "BIT_1_C";
    case BIT_1_D:
        return "BIT_1_D";
    case BIT_1_E:
        return "BIT_1_E";
    case BIT_1_H:
        return "BIT_1_H";
    case BIT_1_L:
        return "BIT_1_L";
    case BIT_1_HLm:
        return "BIT_1_HLm";
    case BIT_1_A:
        return "BIT_1_A";

    case BIT_2_B:
        return "BIT_2_B";
    case BIT_2_C:
        return "BIT_2_C";
    case BIT_2_D:
        return "BIT_2_D";
    case BIT_2_E:
        return "BIT_2_E";
    case BIT_2_H:
        return "BIT_2_H";
    case BIT_2_L:
        return "BIT_2_L";
    case BIT_2_HLm:
        return "BIT_2_HLm";
    case BIT_2_A:
        return "BIT_2_A";
    case BIT_3_B:
        return "BIT_3_B";
    case BIT_3_C:
        return "BIT_3_C";
    case BIT_3_D:
        return "BIT_3_D";
    case BIT_3_E:
        return "BIT_3_E";
    case BIT_3_H:
        return "BIT_3_H";
    case BIT_3_L:
        return "BIT_3_L";
    case BIT_3_HLm:
        return "BIT_3_HLm";
    case BIT_3_A:
        return "BIT_3_A";

    case BIT_4_B:
        return "BIT_4_B";
    case BIT_4_C:
        return "BIT_4_C";
    case BIT_4_D:
        return "BIT_4_D";
    case BIT_4_E:
        return "BIT_4_E";
    case BIT_4_H:
        return "BIT_4_H";
    case BIT_4_L:
        return "BIT_4_L";
    case BIT_4_HLm:
        return "BIT_4_HLm";
    case BIT_4_A:
        return "BIT_4_A";
    case BIT_5_B:
        return "BIT_5_B";
    case BIT_5_C:
        return "BIT_5_C";
    case BIT_5_D:
        return "BIT_5_D";
    case BIT_5_E:
        return "BIT_5_E";
    case BIT_5_H:
        return "BIT_5_H";
    case BIT_5_L:
        return "BIT_5_L";
    case BIT_5_HLm:
        return "BIT_5_HLm";
    case BIT_5_A:
        return "BIT_5_A";

    case BIT_6_B:
        return "BIT_6_B";
    case BIT_6_C:
        return "BIT_6_C";
    case BIT_6_D:
        return "BIT_6_D";
    case BIT_6_E:
        return "BIT_6_E";
    case BIT_6_H:
        return "BIT_6_H";
    case BIT_6_L:
        return "BIT_6_L";
    case BIT_6_HLm:
        return "BIT_6_HLm";
    case BIT_6_A:
        return "BIT_6_A";
    case BIT_7_B:
        return "BIT_7_B";
    case BIT_7_C:
        return "BIT_7_C";
    case BIT_7_D:
        return "BIT_7_D";
    case BIT_7_E:
        return "BIT_7_E";
    case BIT_7_H:
        return "BIT_7_H";
    case BIT_7_L:
        return "BIT_7_L";
    case BIT_7_HLm:
        return "BIT_7_HLm";
    case BIT_7_A:
        return "BIT_7_A";

    case RES_0_B:
        return "RES_0_B";
    case RES_0_C:
        return "RES_0_C";
    case RES_0_D:
        return "RES_0_D";
    case RES_0_E:
        return "RES_0_E";
    case RES_0_H:
        return "RES_0_H";
    case RES_0_L:
        return "RES_0_L";
    case RES_0_HLm:
        return "RES_0_HLm";
    case RES_0_A:
        return "RES_0_A";
    case RES_1_B:
        return "RES_1_B";
    case RES_1_C:
        return "RES_1_C";
    case RES_1_D:
        return "RES_1_D";
    case RES_1_E:
        return "RES_1_E";
    case RES_1_H:
        return "RES_1_H";
    case RES_1_L:
        return "RES_1_L";
    case RES_1_HLm:
        return "RES_1_HLm";
    case RES_1_A:
        return "RES_1_A";

    case RES_2_B:
        return "RES_2_B";
    case RES_2_C:
        return "RES_2_C";
    case RES_2_D:
        return "RES_2_D";
    case RES_2_E:
        return "RES_2_E";
    case RES_2_H:
        return "RES_2_H";
    case RES_2_L:
        return "RES_2_L";
    case RES_2_HLm:
        return "RES_2_HLm";
    case RES_2_A:
        return "RES_2_A";
    case RES_3_B:
        return "RES_3_B";
    case RES_3_C:
        return "RES_3_C";
    case RES_3_D:
        return "RES_3_D";
    case RES_3_E:
        return "RES_3_E";
    case RES_3_H:
        return "RES_3_H";
    case RES_3_L:
        return "RES_3_L";
    case RES_3_HLm:
        return "RES_3_HLm";
    case RES_3_A:
        return "RES_3_A";

    case RES_4_B:
        return "RES_4_B";
    case RES_4_C:
        return "RES_4_C";
    case RES_4_D:
        return "RES_4_D";
    case RES_4_E:
        return "RES_4_E";
    case RES_4_H:
        return "RES_4_H";
    case RES_4_L:
        return "RES_4_L";
    case RES_4_HLm:
        return "RES_4_HLm";
    case RES_4_A:
        return "RES_4_A";
    case RES_5_B:
        return "RES_5_B";
    case RES_5_C:
        return "RES_5_C";
    case RES_5_D:
        return "RES_5_D";
    case RES_5_E:
        return "RES_5_E";
    case RES_5_H:
        return "RES_5_H";
    case RES_5_L:
        return "RES_5_L";
    case RES_5_HLm:
        return "RES_5_HLm";
    case RES_5_A:
        return "RES_5_A";

    case RES_6_B:
        return "RES_6_B";
    case RES_6_C:
        return "RES_6_C";
    case RES_6_D:
        return "RES_6_D";
    case RES_6_E:
        return "RES_6_E";
    case RES_6_H:
        return "RES_6_H";
    case RES_6_L:
        return "RES_6_L";
    case RES_6_HLm:
        return "RES_6_HLm";
    case RES_6_A:
        return "RES_6_A";
    case RES_7_B:
        return "RES_7_B";
    case RES_7_C:
        return "RES_7_C";
    case RES_7_D:
        return "RES_7_D";
    case RES_7_E:
        return "RES_7_E";
    case RES_7_H:
        return "RES_7_H";
    case RES_7_L:
        return "RES_7_L";
    case RES_7_HLm:
        return "RES_7_HLm";
    case RES_7_A:
        return "RES_7_A";

    case SET_0_B:
        return "SET_0_B";
    case SET_0_C:
        return "SET_0_C";
    case SET_0_D:
        return "SET_0_D";
    case SET_0_E:
        return "SET_0_E";
    case SET_0_H:
        return "SET_0_H";
    case SET_0_L:
        return "SET_0_L";
    case SET_0_HLm:
        return "SET_0_HLm";
    case SET_0_A:
        return "SET_0_A";
    case SET_1_B:
        return "SET_1_B";
    case SET_1_C:
        return "SET_1_C";
    case SET_1_D:
        return "SET_1_D";
    case SET_1_E:
        return "SET_1_E";
    case SET_1_H:
        return "SET_1_H";
    case SET_1_L:
        return "SET_1_L";
    case SET_1_HLm:
        return "SET_1_HLm";
    case SET_1_A:
        return "SET_1_A";

    case SET_2_B:
        return "SET_2_B";
    case SET_2_C:
        return "SET_2_C";
    case SET_2_D:
        return "SET_2_D";
    case SET_2_E:
        return "SET_2_E";
    case SET_2_H:
        return "SET_2_H";
    case SET_2_L:
        return "SET_2_L";
    case SET_2_HLm:
        return "SET_2_HLm";
    case SET_2_A:
        return "SET_2_A";
    case SET_3_B:
        return "SET_3_B";
    case SET_3_C:
        return "SET_3_C";
    case SET_3_D:
        return "SET_3_D";
    case SET_3_E:
        return "SET_3_E";
    case SET_3_H:
        return "SET_3_H";
    case SET_3_L:
        return "SET_3_L";
    case SET_3_HLm:
        return "SET_3_HLm";
    case SET_3_A:
        return "SET_3_A";

    case SET_4_B:
        return "SET_4_B";
    case SET_4_C:
        return "SET_4_C";
    case SET_4_D:
        return "SET_4_D";
    case SET_4_E:
        return "SET_4_E";
    case SET_4_H:
        return "SET_4_H";
    case SET_4_L:
        return "SET_4_L";
    case SET_4_HLm:
        return "SET_4_HLm";
    case SET_4_A:
        return "SET_4_A";
    case SET_5_B:
        return "SET_5_B";
    case SET_5_C:
        return "SET_5_C";
    case SET_5_D:
        return "SET_5_D";
    case SET_5_E:
        return "SET_5_E";
    case SET_5_H:
        return "SET_5_H";
    case SET_5_L:
        return "SET_5_L";
    case SET_5_HLm:
        return "SET_5_HLm";
    case SET_5_A:
        return "SET_5_A";

    case SET_6_B:
        return "SET_6_B";
    case SET_6_C:
        return "SET_6_C";
    case SET_6_D:
        return "SET_6_D";
    case SET_6_E:
        return "SET_6_E";
    case SET_6_H:
        return "SET_6_H";
    case SET_6_L:
        return "SET_6_L";
    case SET_6_HLm:
        return "SET_6_HLm";
    case SET_6_A:
        return "SET_6_A";
    case SET_7_B:
        return "SET_7_B";
    case SET_7_C:
        return "SET_7_C";
    case SET_7_D:
        return "SET_7_D";
    case SET_7_E:
        return "SET_7_E";
    case SET_7_H:
        return "SET_7_H";
    case SET_7_L:
        return "SET_7_L";
    case SET_7_HLm:
        return "SET_7_HLm";
    case SET_7_A:
        return "SET_7_A";
    default:
        return "UNHANDLED";
    }
}
} // namespace extendedInstructions

#endif
