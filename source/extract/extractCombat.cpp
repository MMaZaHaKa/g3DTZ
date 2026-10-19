//#include "Extract.h"
//#include "Combat.h"
////#include "Utils.h"
////#include "FileSystem.h"
//#include <iostream>
//#include <fstream>
//#include <vector>
//#include <unordered_set>
//#include "Windows.h" // :D
//
//CCombatMoves gFightMoves;
//CCombatMoves* gpFightMoves = &gFightMoves;
//
//CCombatStats gFightStats;
//CCombatStats* gpFightStats = &gFightStats;
//
//void DumpMovesToFile(const char* filename)
//{
//}
//
//void DumpStatsToFile(const char* filename)
//{
//#if 0
//	FILE* f = std::fopen(filename, "w");
//	if (!f) return;
//
//	std::fprintf(f, "hdr\n"); // done --------------------------------------
//	std::fprintf(f,
//		"%d %d %d %d %d %d %d\n",
//		pf->m_numPathNodes,
//		pf->m_numCarPathNodes,
//		pf->m_numPedPathNodes,
//		pf->m_numMapObjects,
//		pf->m_numConnections,
//		pf->m_numCarPathConnections,
//		pf->m_numCarPathLinks
//	);
//	std::fprintf(f, "end\n\n");
//
//
//	// --- nodes
//#ifdef PATH_LOAD_RAW_BYTES
//	static_assert(sizeof(CPathNode) == 0xA, "sizeof(CPathNode) == 0xA");
//	std::fprintf(f, "node\n");
//	for (int i = 0; i < pf->m_numPathNodes; i++) {
//		const uint8_t* b = (const uint8_t*)&pf->m_pathNodes[i];
//		std::fprintf(f, "[%d]", i);
//		for (size_t k = 0; k < sizeof(CPathNode); k++)
//			std::fprintf(f, " 0x%02X", (unsigned)b[k]);
//		std::fprintf(f, "\n");
//	}
//	std::fprintf(f, "end\n\n");
//#else
//	std::fprintf(f, "node\n");
//	for (int i = 0; i < pf->m_numPathNodes; i++) {
//		const CPathNode& n = pf->m_pathNodes[i];
//		// Распакуем флаги из битовых полей:
//		uint8_t dead = n.bDeadEnd;
//		uint8_t dis = n.bDisabled;
//		uint8_t between = n.bBetweenLevels;
//		uint8_t roadblk = n.bUseInRoadBlock;
//		uint8_t water = n.bWaterPath;
//		uint8_t sel = n.bSelected;
//		uint8_t spd = n.speedLimit;
//		uint8_t spawn = n.spawnRateIndex;
//
//		float x = n.posX / 8.0f;
//		float y = n.posY / 8.0f;
//		float z = n.posZ;
//
//		////pos.z = ((n.z / PATHZSCALE) + 1.0f);
//		//if (pos.z < 0)
//		//	pos.z -= 100;
//		//pos.z = ((pos.z / PATHZSCALE) + 1.0f);
//
//		//float PATHZSCALE = 1.0f;
//		//if (fz < 0)
//		//	fz -= 100.0f; // CPathFind::FindNextNodeWandering() vcs
//		////fz = ((fz / PATHZSCALE) + 1.0f); // ped path in mall to hight
//		//fz = ((fz / PATHZSCALE));
//
//		//if (i < m_numCarPathNodes) // car
//		//	fz += 1.0f;
//
//		std::fprintf(f,
//			//"%f %f %f %u %d %d %u %u %u %u %u %u %u %u\n",
//			//"%d %f %f %f %u %d %d %u %u %u %u %u %u %u %u\n", i,
//			"[%d] %f %f %f %d %d %d %u %u %u %u %u %u %u %u\n", i,
//			x, y, z,
//			n.width,
//			n.firstLink,
//			n.numLinks,
//			dead, dis, between, roadblk,
//			water, sel, spd, spawn
//		);
//	}
//	std::fprintf(f, "end\n\n");
//#endif
//
//	// --- clnk (CCarPathLink)
//#ifdef PATH_LOAD_RAW_BYTES
//	static_assert(sizeof(CCarPathLink) == 0x4, "sizeof(CCarPathLink) == 0x4");
//	std::fprintf(f, "clnk\n");
//	for (int i = 0; i < pf->m_numCarPathLinks; i++) {
//		const uint8_t* b = (const uint8_t*)&pf->m_carPathLinks[i];
//		std::fprintf(f, "[%d]", i);
//		for (size_t k = 0; k < sizeof(CCarPathLink); k++)
//			std::fprintf(f, " 0x%02X", (unsigned)b[k]);
//		std::fprintf(f, "\n");
//	}
//	std::fprintf(f, "end\n\n");
//#else
//	std::fprintf(f, "clnk\n");
//	for (int i = 0; i < pf->m_numCarPathLinks; i++) {
//		const CCarPathLink& L = pf->m_carPathLinks[i];
//		uint16_t idx = L.pathNodeIndex;
//		uint8_t  u1 = L.unk1;
//		uint8_t  u2 = L.unk2;
//		int8_t   left = L.numLeftLanes;
//		int8_t   right = L.numRightLanes;
//		uint8_t  u3 = L.unk3;
//		uint8_t  u4 = L.unk4;
//		int8_t   u5 = L.unk5;
//		std::fprintf(f,
//			//"%u %u %u %d %d %u %u %d\n",
//			//"%d %u %u %u %d %d %u %u %d\n", i,
//			"[%d] %u %u %u %d %d %u %u %d\n", i,
//			idx, u1, u2, left, right, u3, u4, u5
//		);
//	}
//	std::fprintf(f, "end\n\n");
//#endif
//
//	// --- ccon (массив uint16_t, авто-соединения)
////#ifdef PATH_LOAD_RAW_BYTES
////	std::fprintf(f, "ccon\n"); // done --------------------------------------
////	for (int i = 0; i < pf->m_numCarPathConnections; i++) {
////		const uint8_t* b = (const uint8_t*)&pf->m_carPathConnections[i];
////		std::fprintf(f, "[%d]", i);
////		for (size_t k = 0; k < sizeof(int16_t); k++)
////			std::fprintf(f, " 0x%02X", (unsigned)b[k]);
////		std::fprintf(f, "\n");
////	}
////	std::fprintf(f, "end\n\n");
////#else
//	std::fprintf(f, "ccon\n"); // done -----------------------------------------
//	for (int i = 0; i < pf->m_numCarPathConnections; i++) {
//		std::fprintf(f, "[%d] %u\n", i, pf->m_carPathConnections[i]);
//	}
//	std::fprintf(f, "end\n\n");
//	//#endif
//
//		// --- pcon (CPathConnection)
//#ifdef PATH_LOAD_RAW_BYTES
//	static_assert(sizeof(CPathConnection) == 0x2, "sizeof(CPathConnection) == 0x2");
//	std::fprintf(f, "pcon\n");
//	for (int i = 0; i < pf->m_numConnections; i++) {
//		const uint8_t* b = (const uint8_t*)&pf->m_connections[i];
//		std::fprintf(f, "[%d]", i);
//		for (size_t k = 0; k < sizeof(CPathConnection); k++)
//			std::fprintf(f, " 0x%02X", (unsigned)b[k]);
//		std::fprintf(f, "\n");
//	}
//	std::fprintf(f, "end\n\n");
//#else
//	std::fprintf(f, "pcon\n");
//	for (int i = 0; i < pf->m_numConnections; i++) {
//		const CPathConnection& c = pf->m_connections[i];
//		// Здесь CPathConnection уже хранит биты в полях:
//		//std::fprintf(f, "%u %u %u\n",
//		//std::fprintf(f, "%d %u %u %u\n", i,
//		std::fprintf(f, "[%d] %u %u %u\n", i,
//			c.idx,
//			(int)c.bTrafficLight,
//			(int)c.bCrossesRoad
//		);
//	}
//	std::fprintf(f, "end\n\n");
//#endif
//
//	// --- dist (uint8_t дальности)
//	std::fprintf(f, "dist\n");
//	for (int i = 0; i < pf->m_numConnections; i++) {
//		//std::fprintf(f, "%u\n", pf->m_distances[i]);
//		//std::fprintf(f, "%d %u\n", i, pf->m_distances[i]);
//		std::fprintf(f, "[%d] %u\n", i, pf->m_distances[i]);
//	}
//	std::fprintf(f, "end\n");
//
//	std::fprintf(f, "secs\n");
//	for (int s = 0; s < 50 * 50; s++) {
//		const CPathSector& sec = pf->m_pathSectors[s];
//		uint8_t nCar = sec.m_pathNodeCount[0];
//		uint8_t nPed = sec.m_pathNodeCount[1];
//
//		std::fprintf(f, "%u %u %u\n", nCar, nPed, s);
//
//		for (int i = 0; i < nCar; i++) {
//			std::fprintf(f, "%u\n", sec.m_pathNodeList[0][i]);
//		}
//		for (int i = 0; i < nPed; i++) {
//			std::fprintf(f, "%u\n", sec.m_pathNodeList[1][i]);
//		}
//	}
//	std::fprintf(f, "end\n\n");
//
//	std::fclose(f);
//#endif
//}
//
//
//
//bool ExtractCombat()
//{
//	setlocale(LC_NUMERIC, "C");
//	//DebugCode(); // json gen
//	//DumpRoadsToFile(ThePaths, "C:\\vcstmp\\PATH.DAT");
//	DumpMovesToFile("COMBATMOVES.DAT");
//	DumpStatsToFile("COMBATSTATS.DAT");
//	//test2();
//	//DumpInternalLinks(ThePaths);
//	//DumpExternalLinks(ThePaths);
//	//dump777();
//
//	return true;
//}