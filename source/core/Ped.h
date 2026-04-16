#pragma once
#include "Common.h"
#include "Utils.h"
#include "Maths.h"

#define NUM_ANIM_OFFSETS (20 + 1) // last nil?
#pragma pack(push, 1)
struct tPedAnimOffset
{
	int32 m_nAnimationGroup;
	int32 m_nAnimationID;
	uint8 m_pad1[8];
	CVuVector vecOffset;
};
static_assert(sizeof(tPedAnimOffset) == 32, "sizeof(tPedAnimOffset)");

struct tPedAnimOffsets
{
	int32 m_nCount;
	uint8 m_pad1[12];
	tPedAnimOffset offsets[NUM_ANIM_OFFSETS];
};

extern tPedAnimOffsets PedAnimInfo;
#pragma pack(pop)

class tFightMove final
{
public:
#ifdef LCS
	int32 animId;
	float startFireTime;
	float endFireTime;
	float comboFollowOnTime;
	float strikeRadius;
	float extendReachMultiplier;
	uint8 hitLevel; // FightMoveHitLevel
	uint8 damage;
	uint8 flags;
	uint8 unk;
#else
	int32 animId;
	float startFireTime;
	float endFireTime;
	float comboFollowOnTime;
	float strikeRadius;
	float extendReachMultiplier;
	uint8 hitLevel; // FightMoveHitLevel
	uint8 damage;
	uint8 flags;
	uint8 unk;
#endif

	int GetStartFireTime() { return static_cast<int>(startFireTime * 30.0f); }
	int GetEndFireTime() { return static_cast<int>(endFireTime * 30.0f); }
	int GetComboFollowOnTime() { return static_cast<int>(comboFollowOnTime * 30.0f); }
	auto GetStrikeRadius() { return Precision(strikeRadius); }
	auto GetExtendReachMultiplier() { return Precision(extendReachMultiplier); }
	int GetDamage() { return damage; }
	int GetFlags() { return flags; }
	char GetHitLevel()
	{
		static const char hitLevels[]{ 'N', 'G', 'L', 'M', 'H' };
		return hitLevels[hitLevel];
	}

	const char* GetAnimationName();
};

class CPed final
{
private:
	inline static tFightMove* m_fightMoves;
	inline static tPedAnimOffsets* m_pedAnimInfo;

public:
	static void LoadFightData(tFightMove* fightMoves) { m_fightMoves = fightMoves; }
	static auto& GetFightMove(int index) { return m_fightMoves[index]; }

	static void LoadAnimOffsets(tPedAnimOffsets* pedAnimInfo) { m_pedAnimInfo = pedAnimInfo; }
	static tPedAnimOffsets* GetAnimOffsets() { return m_pedAnimInfo; }
};