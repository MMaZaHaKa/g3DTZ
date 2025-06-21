#pragma once
//#include "Maths.h"

#pragma pack(push, 1) // VCS PS2 !!!!
struct CCarPathLink
{
	__int16 field_0; // 0_pathNodeIndex, 1_pathNodeIndex, 2_pathNodeIndex, 3_pathNodeIndex, 4_pathNodeIndex, 5_pathNodeIndex, 6_pathNodeIndex, 7_pathNodeIndex  0_pathNodeIndex, 1_pathNodeIndex, 2_pathNodeIndex, 3_pathNodeIndex, 4_pathNodeIndex, 5_pathNodeIndex, 6_, 7_
	char num_field_2; // 0_numLeftLanes, 1_numLeftLanes, 2_numLeftLanes, 3_numRightLanes, 4_numRightLanes, 5_numRightLanes, 6_, 7_
	char field_3; // todo
	char field_4; // todo
};
struct CPathNode
{
	__int16 posX;
	__int16 posY;
	char posZ;
	char width;
	__int16 firstLink;
	char numLinks_Flags8; // 0__numlinks[b0], 1__numlinks[b1], 2__numlinks[b2], 3__numlinks[b3], 4_bDeadEnd ? , 5_bDisabled, 6_bBetweenLevels, 7_bUseInRoad
	char flags9; // 0_bWaterPath, 1_(1?bSelected?), 2_speedLimit[b0], 3_speedLimit[b1], 4_spawnRateINDEX[b0], 5_spawnRateINDEX[b1], 6_(0? always), 7_(1? always)
};
struct __declspec(align(2)) CPathFind // todo class
{
	CPathNode* m_pathNodes;
	CCarPathLink* m_carPathLinks;
	__int16* m_carPathConnections;
	int m_numPathNodes;      // 8387
	int m_numCarPathNodes;   // 3079
	int m_numPedPathNodes;   // 5308
	__int16 m_numMapObjects; // 0
	__int16 m_numConnections;// 17625
	__int16 possible_m_numCarPathLinks_field_1C; // ?   6354
	__int16 pad_AAAA_field_1E;
	__int16 possible_m_numCarPathLinks_field_20; // ?  3177
	__int16 field_22;
	__int16 pad_AAAA_field_24;
	__int16 field_26;
	char AAAAAA_field_28[1608];
	__int16 field_670;
	__int16 field_672;
	int field_674;
	char field_678[29988];
	__int16 field_7B9C;
	__int16 field_7B9E;
	__int16* m_connections;
	char* m_distances;
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