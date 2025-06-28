#pragma once
//#include "Maths.h"

#pragma pack(push, 1) // VCS PS2 !!!!

struct CPathConnection	// 2 not original name
{
	uint16_t idx : 14;
	uint16_t bTrafficLight : 1;
	uint16_t bCrossesRoad : 1;
};
struct CCarPathLink // 4    ?
{
	////uint16_t idx : 14;
	//uint16_t bTrafficLight : 1; // ?
	//uint16_t bCrossesRoad : 1; // ?

	uint16_t pathNodeIndex : 14;
	uint16_t unk1 : 1; // ?
	uint16_t unk2 : 1; // ?

	int8_t numLeftLanes : 3;
	int8_t numRightLanes : 3;
	int8_t unk3 : 1; // ?
	int8_t unk4 : 1; // ?

	int8_t unk5;
};
struct CPathNode
{
	int16_t posX;
	int16_t posY;
	int8_t posZ; // #define PATHZSCALE (1.0f)
	int8_t width; // flood fill group?
	int16_t firstLink;

	uint8_t numLinks : 4;
	uint8_t bDeadEnd : 1; //?
	uint8_t bDisabled : 1;
	uint8_t bBetweenLevels : 1;
	uint8_t bUseInRoadBlock : 1;

	uint8_t bWaterPath : 1;
	uint8_t bSelected : 1; // bOnlySmallBoats?  bSelected?
	uint8_t speedLimit : 2;
	uint8_t spawnRateIndex : 2; // int gaSpawnRate[] = { 0, 4, 7, 15 };
	uint8_t unk1 : 1; // 0 always
	uint8_t unk2 : 1; // 1 always
};
struct __declspec(align(2)) CPathFind // todo move into class
{
	CPathNode* m_pathNodes;
	CCarPathLink* m_carPathLinks;
	uint16_t* m_carPathConnections;

	int32_t m_numPathNodes;      // 8387
	int32_t m_numCarPathNodes;   // 3079
	int32_t m_numPedPathNodes;   // 5308
	int16_t m_numMapObjects; // 0
	int16_t m_numConnections;// 17625
	int16_t m_numCarPathConnections; // ?   6354
	int16_t pad_AAAA_field_1E;
	int32_t m_numCarPathLinks; // ?  3177
	int16_t pad_AAAA_field_24;
	int16_t field_26;
	char AAAAAA_field_28[1608];
	__int16 field_670;
	__int16 field_672;
	int field_674;
	char field_678[29988];
	__int16 field_7B9C;
	__int16 field_7B9E;

	CPathConnection* m_connections; //16*
	uint8_t* m_distances; //8*

	char field_7BA8[508];
	int field_7DA4;
	int field_7DA8;
	int field_7DAC;
	char __field_7DB0[5520];
	CPathNode m_aPathNodes[8387];
	int field_1DADE;
};
#pragma pack(pop)



class _CPathFind final // todo fields
{
public:
	inline static CPathFind* mspInst;
	static void Init(CPathFind* inst) { mspInst = inst; }

};