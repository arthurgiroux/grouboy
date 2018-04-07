#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H


#define FLAG_ZERO 0x80 // If the last operation produced 0

#define FLAG_SUB 0x40 // If the last operation was a substraction

#define FLAG_HALF_CARRY 0x20 // If we the result of operation overflowed past 15

#define FLAG_CARRY 0x10 // If the last operation overflowed (> 255 for addition or 0 < for substraction)

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
#define LD_HLm_I_A		0x22
#define INC_HL			0x23
#define INC_H			0x24
#define DEC_H			0x25
#define LD_H_n			0x26
#define DAA				0x27
#define JR_Z_n			0x28
#define ADD_HL_HL		0x29
#define LD_A_HLm_I		0x2A
#define DEC_HL			0x2B
#define INC_L			0x2C
#define DEC_L			0x2D
#define LD_L_n			0x2E
#define CPL				0x2F

#define JR_NC_n			0x30
#define LD_SP_nn		0x31
#define LD_HLm_D_A		0x32
#define INC_SP			0x33
#define INC_HLm			0x34
#define DEC_HLm			0x35
#define LD_HLm_n		0x36
#define SCF				0x37
#define JR_C_n			0x38
#define ADD_HL_SP		0x39
#define LD_A_HLm_D		0x3A
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
#define LD_Cm_A			0xE2
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
#define LD_A_Cm			0xF2
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



/*********** OPCODES EXTENSION ***********/

#define RLC_B			0x00
#define RLC_C			0x01
#define RLC_D			0x02
#define RLC_E			0x03
#define RLC_H			0x04
#define RLC_L			0x05
#define RLC_HLm			0x06
#define RLC_A			0x07
#define RRC_B			0x08
#define RRC_C			0x09
#define RRC_D			0x0A
#define RRC_E			0x0B
#define RRC_H			0x0C
#define RRC_L			0x0D
#define RRC_HLm			0x0E
#define RRC_A			0x0F


#define RL_B			0x10
#define RL_C			0x11
#define RL_D			0x12
#define RL_E			0x13
#define RL_H			0x14
#define RL_L			0x15
#define RL_HLm			0x16
#define RL_A			0x17
#define RR_B			0x18
#define RR_C			0x19
#define RR_D			0x1A
#define RR_E			0x1B
#define RR_H			0x1C
#define RR_L			0x1D
#define RR_HLm			0x1E
#define RR_A			0x1F


#define SLA_B			0x20
#define SLA_C			0x21
#define SLA_D			0x22
#define SLA_E			0x23
#define SLA_H			0x24
#define SLA_L			0x25
#define SLA_HLm			0x26
#define SLA_A			0x27
#define SRA_B			0x28
#define SRA_C			0x29
#define SRA_D			0x2A
#define SRA_E			0x2B
#define SRA_H			0x2C
#define SRA_L			0x2D
#define SRA_HLm			0x2E
#define SRA_A			0x2F


#define SWAP_B			0x30
#define SWAP_C			0x31
#define SWAP_D			0x32
#define SWAP_E			0x33
#define SWAP_H			0x34
#define SWAP_L			0x35
#define SWAP_HLm		0x36
#define SWAP_A			0x37
#define SRL_B			0x38
#define SRL_C			0x39
#define SRL_D			0x3A
#define SRL_E			0x3B
#define SRL_H			0x3C
#define SRL_L			0x3D
#define SRL_HLm			0x3E
#define SRL_A			0x3F


#define BIT_0_B			0x40
#define BIT_0_C			0x41
#define BIT_0_D			0x42
#define BIT_0_E			0x43
#define BIT_0_H			0x44
#define BIT_0_L			0x45
#define BIT_0_HLm		0x46
#define BIT_0_A			0x47
#define BIT_1_B			0x48
#define BIT_1_C			0x49
#define BIT_1_D			0x4A
#define BIT_1_E			0x4B
#define BIT_1_H			0x4C
#define BIT_1_L			0x4D
#define BIT_1_HLm		0x4E
#define BIT_1_A			0x4F


#define BIT_2_B			0x50
#define BIT_2_C			0x51
#define BIT_2_D			0x52
#define BIT_2_E			0x53
#define BIT_2_H			0x54
#define BIT_2_L			0x55
#define BIT_2_HLm		0x56
#define BIT_2_A			0x57
#define BIT_3_B			0x58
#define BIT_3_C			0x59
#define BIT_3_D			0x5A
#define BIT_3_E			0x5B
#define BIT_3_H			0x5C
#define BIT_3_L			0x5D
#define BIT_3_HLm		0x5E
#define BIT_3_A			0x5F

#define BIT_4_B			0x60
#define BIT_4_C			0x61
#define BIT_4_D			0x62
#define BIT_4_E			0x63
#define BIT_4_H			0x64
#define BIT_4_L			0x65
#define BIT_4_HLm		0x66
#define BIT_4_A			0x67
#define BIT_5_B			0x68
#define BIT_5_C			0x69
#define BIT_5_D			0x6A
#define BIT_5_E			0x6B
#define BIT_5_H			0x6C
#define BIT_5_L			0x6D
#define BIT_5_HLm		0x6E
#define BIT_5_A			0x6F



#define BIT_6_B			0x70
#define BIT_6_C			0x71
#define BIT_6_D			0x72
#define BIT_6_E			0x73
#define BIT_6_H			0x74
#define BIT_6_L			0x75
#define BIT_6_HLm		0x76
#define BIT_6_A			0x77
#define BIT_7_B			0x78
#define BIT_7_C			0x79
#define BIT_7_D			0x7A
#define BIT_7_E			0x7B
#define BIT_7_H			0x7C
#define BIT_7_L			0x7D
#define BIT_7_HLm		0x7E
#define BIT_7_A			0x7F


#define RES_0_B			0x80
#define RES_0_C			0x81
#define RES_0_D			0x82
#define RES_0_E			0x83
#define RES_0_H			0x84
#define RES_0_L			0x85
#define RES_0_HLm		0x86
#define RES_0_A			0x87
#define RES_1_B			0x88
#define RES_1_C			0x89
#define RES_1_D			0x8A
#define RES_1_E			0x8B
#define RES_1_H			0x8C
#define RES_1_L			0x8D
#define RES_1_HLm		0x8E
#define RES_1_A			0x8F


#define RES_2_B			0x90
#define RES_2_C			0x91
#define RES_2_D			0x92
#define RES_2_E			0x93
#define RES_2_H			0x94
#define RES_2_L			0x95
#define RES_2_HLm		0x96
#define RES_2_A			0x97
#define RES_3_B			0x98
#define RES_3_C			0x99
#define RES_3_D			0x9A
#define RES_3_E			0x9B
#define RES_3_H			0x9C
#define RES_3_L			0x9D
#define RES_3_HLm		0x9E
#define RES_3_A			0x9F

#define RES_4_B			0xA0
#define RES_4_C			0xA1
#define RES_4_D			0xA2
#define RES_4_E			0xA3
#define RES_4_H			0xA4
#define RES_4_L			0xA5
#define RES_4_HLm		0xA6
#define RES_4_A			0xA7
#define RES_5_B			0xA8
#define RES_5_C			0xA9
#define RES_5_D			0xAA
#define RES_5_E			0xAB
#define RES_5_H			0xAC
#define RES_5_L			0xAD
#define RES_5_HLm		0xAE
#define RES_5_A			0xAF



#define RES_6_B			0xB0
#define RES_6_C			0xB1
#define RES_6_D			0xB2
#define RES_6_E			0xB3
#define RES_6_H			0xB4
#define RES_6_L			0xB5
#define RES_6_HLm		0xB6
#define RES_6_A			0xB7
#define RES_7_B			0xB8
#define RES_7_C			0xB9
#define RES_7_D			0xBA
#define RES_7_E			0xBB
#define RES_7_H			0xBC
#define RES_7_L			0xBD
#define RES_7_HLm		0xBE
#define RES_7_A			0xBF


#define SET_0_B			0xC0
#define SET_0_C			0xC1
#define SET_0_D			0xC2
#define SET_0_E			0xC3
#define SET_0_H			0xC4
#define SET_0_L			0xC5
#define SET_0_HLm		0xC6
#define SET_0_A			0xC7
#define SET_1_B			0xC8
#define SET_1_C			0xC9
#define SET_1_D			0xCA
#define SET_1_E			0xCB
#define SET_1_H			0xCC
#define SET_1_L			0xCD
#define SET_1_HLm		0xCE
#define SET_1_A			0xCF


#define SET_2_B			0xD0
#define SET_2_C			0xD1
#define SET_2_D			0xD2
#define SET_2_E			0xD3
#define SET_2_H			0xD4
#define SET_2_L			0xD5
#define SET_2_HLm		0xD6
#define SET_2_A			0xD7
#define SET_3_B			0xD8
#define SET_3_C			0xD9
#define SET_3_D			0xDA
#define SET_3_E			0xDB
#define SET_3_H			0xDC
#define SET_3_L			0xDD
#define SET_3_HLm		0xDE
#define SET_3_A			0xDF

#define SET_4_B			0xE0
#define SET_4_C			0xE1
#define SET_4_D			0xE2
#define SET_4_E			0xE3
#define SET_4_H			0xE4
#define SET_4_L			0xE5
#define SET_4_HLm		0xE6
#define SET_4_A			0xE7
#define SET_5_B			0xE8
#define SET_5_C			0xE9
#define SET_5_D			0xEA
#define SET_5_E			0xEB
#define SET_5_H			0xEC
#define SET_5_L			0xED
#define SET_5_HLm		0xEE
#define SET_5_A			0xEF



#define SET_6_B			0xF0
#define SET_6_C			0xF1
#define SET_6_D			0xF2
#define SET_6_E			0xF3
#define SET_6_H			0xF4
#define SET_6_L			0xF5
#define SET_6_HLm		0xF6
#define SET_6_A			0xF7
#define SET_7_B			0xF8
#define SET_7_C			0xF9
#define SET_7_D			0xFA
#define SET_7_E			0xFB
#define SET_7_H			0xFC
#define SET_7_L			0xFD
#define SET_7_HLm		0xFE
#define SET_7_A			0xFF

#endif
