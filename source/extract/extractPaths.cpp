#include "Extract.h"
#include "PathFind.h"
//#include "Utils.h"
//#include "FileSystem.h"
#include <fstream>
#include "Windows.h" // :D

bool ExtractPaths()
{
	_CPathFind* gpThePath = CPathFind::mspInst;

	printf("num: %d\n\n", gpThePath->m_numPathNodes);
	for (int32_t i = 0; i < gpThePath->m_numPathNodes; ++i)
	{
		CPathNode* n = &gpThePath->m_pathNodes[i];
		float realX = n->posX / 8; //  (x/8) * 16 = gta3 ipl
		float realY = n->posY / 8; //  (x/8) * 16 = *2
		printf("n_%d: X: %f, Y:%f\n", i, realX, realY);
	}

	return true;
}