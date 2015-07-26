#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H


#define FLAG_ZERO 0x80 // If the last operation produced 0
#define RESET_ZERO 0x7F

#define FLAG_SUB 0x40 // If the last operation was a substraction
#define RESET_SUB 0xBF

#define FLAG_HALF_CARRY 0x20 // If we the result of operation overflowed past 15
#define RESET_HALF_CARRY 0xDF

#define FLAG_CARRY 0x10 // If the last operation overflowed (> 255 for addition or 0 < for substraction)
#define RESET_CARRY 0xEF

#define NOP				0x00
#define LD_BC_nn		0x01
#define LD_BCm_A		0x02
#define INC_BC			0x03
#define INC_B			0x04
#define DEC_B			0x05
#define LD_B_n			0x06
#define RLC_A			0x07
#define LD_nnm_SP		0x08
#define ADD_HL_BC		0x09
#define LD_A_BCm		0x0A
#define DEC_BC			0x0B
#define INC_C			0x0C
#define DEC_C			0x0D
#define LD_C_n			0x0E
#define RRC_A			0x0F

#define STOP			0x10
#define LD_DE_nn		0x11
#define LD_DEm_A		0x12
#define INC_DE			0x13
#define INC_D			0x14
#define DEC_D			0x15
#define LD_D_n			0x16
#define RL_A			0x17
#define JR_n			0x18
#define ADD_HL_DE		0x19
#define LD_A_DEm		0x1A
#define DEC_DE			0x1B
#define INC_E			0x1C
#define DEC_E			0x1D
#define LD_E_n			0x1E
#define RR_A			0x1F

#define JR_NZ_n			0x20
#define LD_HL_nn		0x21
#define LDI_HLm_A		0x22
#define INC_HL			0x23
#define INC_H			0x24
#define DEC_H			0x25
#define LD_H_n			0x26
#define DAA				0x27
#define JR_Z_n			0x28
#define ADD_HL_HL		0x29
#define LDI_A_HLm		0x2A
#define DEC_HL			0x2B
#define INC_L			0x2C
#define DEC_L			0x2D
#define LD_L_n			0x2E
#define CPL				0x2F

#define JR_NC_n			0x30
#define LD_SP_nn		0x31
#define LDD_HLm_A		0x32
#define INC_SP			0x33
#define INC_HLm			0x34
#define DEC_HLm			0x35
#define LD_HLm_n		0x36
#define SCF				0x37
#define JR_C_n			0x38
#define ADD_HL_SP		0x39
#define LDD_A_HLm		0x3A
#define DEC_SP			0x3B
#define INC_A			0x3C
#define DEC_A			0x3D
#define LD_A_n			0x3E
#define CCF				0x3F

#define LD_B_B			0x40
#define LD_B_C			0x41
#define LD_B_D			0x42
#define LD_B_E			0x43
#define LD_B_H			0x44
#define LD_B_L			0x45
#define LD_B_HLm		0x46
#define LD_B_A			0x47
#define LD_C_B			0x48
#define LD_C_C			0x49
#define LD_C_D			0x4A
#define LD_C_E			0x4B
#define LD_C_H			0x4C
#define LD_C_L			0x4D
#define LD_C_HLm		0x4E
#define LD_C_A			0x4F

#define LD_D_B			0x50
#define LD_D_C			0x51
#define LD_D_D			0x52
#define LD_D_E			0x53
#define LD_D_H			0x54
#define LD_D_L			0x55
#define LD_D_HLm		0x56
#define LD_D_A			0x57
#define LD_E_B			0x58
#define LD_E_C			0x59
#define LD_E_D			0x5A
#define LD_E_E			0x5B
#define LD_E_H			0x5C
#define LD_E_L			0x5D
#define LD_E_HLm		0x5E
#define LD_E_A			0x5F

#define LD_H_B			0x60
#define LD_H_C			0x61
#define LD_H_D			0x62
#define LD_H_E			0x63
#define LD_H_H			0x64
#define LD_H_L			0x65
#define LD_H_HLm		0x66
#define LD_H_A			0x67
#define LD_L_B			0x68
#define LD_L_C			0x69
#define LD_L_D			0x6A
#define LD_L_E			0x6B
#define LD_L_H			0x6C
#define LD_L_L			0x6D
#define LD_L_HLm		0x6E
#define LD_L_A			0x6F

#define LD_HLm_B		0x70
#define LD_HLm_C		0x71
#define LD_HLm_D		0x72
#define LD_HLm_E		0x73
#define LD_HLm_H		0x74
#define LD_HLm_L		0x75
#define HALT			0x76
#define LD_HLm_A		0x77
#define LD_A_B			0x78
#define LD_A_C			0x79
#define LD_A_D			0x7A
#define LD_A_E			0x7B
#define LD_A_H			0x7C
#define LD_A_L			0x7D
#define LD_A_HLm		0x7E
#define LD_A_A			0x7F

#define ADD_A_B			0x80
#define ADD_A_C			0x81
#define ADD_A_D			0x82
#define ADD_A_E			0x83
#define ADD_A_H			0x84
#define ADD_A_L			0x85
#define ADD_A_HLm		0x86
#define ADD_A_A			0x87
#define ADC_A_B			0x88
#define ADC_A_C			0x89
#define ADC_A_D			0x8A
#define ADC_A_E			0x8B
#define ADC_A_H			0x8C
#define ADC_A_L			0x8D
#define ADC_A_HLm		0x8E
#define ADC_A_A			0x8F

#define SUB_A_B			0x90
#define SUB_A_C			0x91
#define SUB_A_D			0x92
#define SUB_A_E			0x93
#define SUB_A_H			0x94
#define SUB_A_L			0x95
#define SUB_A_HLm		0x96
#define SUB_A_A			0x97
#define SBC_A_B			0x98
#define SBC_A_C			0x99
#define SBC_A_D			0x9A
#define SBC_A_E			0x9B
#define SBC_A_H			0x9C
#define SBC_A_L			0x9D
#define SBC_A_HLm		0x9E
#define SBC_A_A			0x9F

#define AND_B			0xA0
#define AND_C			0xA1
#define AND_D			0xA2
#define AND_E			0xA3
#define AND_H			0xA4
#define AND_L			0xA5
#define AND_HLm			0xA6
#define AND_A			0xA7
#define XOR_B			0xA8
#define XOR_C			0xA9
#define XOR_D			0xAA
#define XOR_E			0xAB
#define XOR_H			0xAC
#define XOR_L			0xAD
#define XOR_HLm			0xAE
#define XOR_A			0xAF

#define OR_B			0xB0
#define OR_C			0xB1
#define OR_D			0xB2
#define OR_E			0xB3
#define OR_H			0xB4
#define OR_L			0xB5
#define OR_HLm			0xB6
#define OR_A			0xB7
#define CP_B			0xB8
#define CP_C			0xB9
#define CP_D			0xBA
#define CP_E			0xBB
#define CP_H			0xBC
#define CP_L			0xBD
#define CP_HLm			0xBE
#define CP_A			0xBF

#define RET_NZ			0xC0
#define POP_BC			0xC1
#define JP_NZ_nn		0xC2
#define JP_nn			0xC3
#define CALL_NZ_nn		0xC4
#define PUSH_BC			0xC5
#define ADD_A_n			0xC6
#define RST_0			0xC7
#define RET_Z			0xC8
#define RET				0xC9
#define JP_Z_nn			0xCA
#define EXT_OPS			0xCB
#define CALL_Z_nn		0xCC
#define CALL_nn			0xCD
#define ADC_A_n			0xCE
#define RST_8			0xCF

#define RET_NC			0xD0
#define POP_DE			0xD1
#define JP_NC_nn		0xD2
/* NOT IMPLEMENTED		0xD3*/
#define CALL_NC_nn		0xD4
#define PUSH_DE			0xD5
#define SUB_A_n			0xD6
#define RST_10			0xD7
#define RET_C			0xD8
#define RETI			0xD9
#define JP_C_nn			0xDA
/* NOT IMPLEMENTED		0xDB*/
#define CALL_C_nn		0xDC
/* NOT IMPLEMENTED		0xDD*/
#define SBC_A_n			0xDE
#define RST_18			0xDF

#define LDH_nm_A		0xE0
#define POP_HL			0xE1
#define LDH_Cm_A		0xE2
/* NOT IMPLEMENTED		0xE3*/
/* NOT IMPLEMENTED		0xE4*/
#define PUSH_HL			0xE5
#define AND_n			0xE6
#define RST_20			0xE7
#define ADD_SP_d		0xE8
#define JP_HLm			0xE9
#define LD_nnm_A		0xEA
/* NOT IMPLEMENTED		0xEB*/
/* NOT IMPLEMENTED		0xEC*/
/* NOT IMPLEMENTED		0xED*/
#define XOR_n			0xEE
#define RST_28			0xEF

#define LDH_A_nm		0xF0
#define POP_AF			0xF1
/* NOT IMPLEMENTED		0xF2*/
#define DI				0xF3
/* NOT IMPLEMENTED		0xF4*/
#define PUSH_AF			0xF5
#define OR_n			0xF6
#define RST_30			0xF7
#define LDHL_SP_d		0xF8
#define LD_SP_HL		0xF9
#define LD_A_nnm		0xFA
#define EI				0xFB
/* NOT IMPLEMENTED		0xFC*/
/* NOT IMPLEMENTED		0xFD*/
#define CP_n			0xFE
#define RST_38			0xFF


#endif
