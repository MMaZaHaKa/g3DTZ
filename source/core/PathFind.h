#pragma once
#include "Maths.h"

class CPathFind final
{
public:
	inline static CPathFind* mspInst;
	static void Init(CPathFind* inst) { mspInst = inst; }

};