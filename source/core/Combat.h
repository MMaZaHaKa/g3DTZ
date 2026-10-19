//#pragma once
//#include "Maths.h"
//#include "Common.h"
//
//#pragma pack(push, 1)
//class CCombatMoveData_A_A
//{
//public:
//    uint8 field_0[0x18]; // min field p2-p1, or min 0x8 unk_973DD44. 0x18 точно, мб меньше
//};
//
//class CCombatMoveData_A
//{
//public:
//    CCombatMoveData_A_A** field_0; // 4 ptr to pointers (16b) [p1,p2,p3,p4]
//    int32 m_nCount; // 4
//};
//
//class CCombatMove
//{
//public:
//    uint8 field_0[16];
//    CVuVector vec_field_10;
//    void* ptr_field_20; // reloc todo
//    void* ptr_field_24; // reloc todo
//    int16 m_nDelta;
//    int8 m_nFlags;
//    int8 field_2B;
//    int8 flags_field_2C;
//    uint8 field_2D[6];
//    char m_name[11];
//    uint8 field_3E;
//    uint8 field_3F;
//};
//static_assert(sizeof(CCombatMove) == 0x40-(4*2)+(sizeof(void*)*2), "sizeof(CCombatMove) == 0x40");
//
//class CCombatMoveData
//{
//public:
//    CCombatMove* m_aMoves;
//    int32 m_nMoveCount; // 104 // 0x68
//};
//
//class CCombatMoveData_B_A
//{
//public:
//    uint8 field_0[32]; // 32 =  0x6A0 / 0x35
//};
//
//class CCombatMoveData_B
//{
//public:
//    CCombatMoveData_B_A* field_0;
//    int32 m_nCount; // 53 // 0x35
//};
//
//class CCombatMoves
//{
//public:
//    CCombatMoveData_A m_DataA;   // 0x04
//    CCombatMoveData m_MovesData; // 0x68
//    CCombatMoveData_B m_DataB;   // 0x35
//    void* m_pEntity;
//    int8 field_1C;
//    int8 field_1D;
//    // todo max 0xF8
//
//
//    static void Initialise(const char* pFileName);
//};
//
//
//class CCombatStat_A
//{
//public:
//    float field_0[2];
//    uint8 field_8[8];
//};
//static_assert(sizeof(CCombatStat_A) == 0x10, "sizeof(CCombatStat_A) == 0x10");
//
//class CCombatStat_B_A // recheck, guessed by unk_973E36C size 4 to adhesiveLimitTable, probably int32
//{
//public:
//    uint8 field_8[4];
//};
//static_assert(sizeof(CCombatStat_B_A) == 0x4, "sizeof(CCombatStat_B_A) == 0x4");
//
//class CCombatStat_B
//{
//public:
//    int32 field_0;
//    CCombatStat_B_A* m_pData;
//};
//static_assert(sizeof(CCombatStat_B) == 0x8-(4)+(sizeof(void*)), "sizeof(CCombatStat_B) == 0x8");
//
//class CCombatStat
//{
//public:
//    int32 m_ePedStat; // NUM_PEDSTATS = 42
//    int8 field_4;
//    uint8 m_nCount_A;
//    uint8 field_6[2]; // pad?
//    CCombatStat_A* m_aArr_A;
//    CCombatStat_B* m_aArr_B;
//    uint8 m_nCount_B;
//    uint8 field_11[3]; // pad?
//    int32 field_14;
//    int32 field_18;
//    int32 field_1C;
//    int32 field_20;
//    int32 field_24;
//    int32 field_28;
//
//
//};
//static_assert(sizeof(CCombatStat) == 0x2C-(4*2)+(sizeof(void*)*2), "sizeof(CCombatStat) == 0x2C");
//
//class CCombatStats
//{
//public:
//    CCombatStat* m_aStats;
//    int16 m_nNumStats; // 3
//    uint8 field_6[2]; // pad
//
//    static void Initialise(const char* pFileName);
//};
//static_assert(sizeof(CCombatStats) == 0x8-(4)+(sizeof(void*)), "sizeof(CCombatStats) == 0x8");
//#pragma pack(pop)