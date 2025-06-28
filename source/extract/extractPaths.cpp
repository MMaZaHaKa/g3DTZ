#include "Extract.h"
#include "PathFind.h"
//#include "Utils.h"
//#include "FileSystem.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_set>
#include "Windows.h" // :D

#define ThePaths (_CPathFind::mspInst)
int gaSpawnRate[] = { 0, 4, 7, 15 };
#define PATHZSCALE (1.0f)

#define BIT(num) (1<<(num))
#define GET_BITS(value, mask, shift) (((value) & (mask)) >> (shift))
#define GET_BIT(num, n) (((num) >> (n)) & 1)
#define SET_BIT(num, n, val) ((num) = ((num) & ~BIT(n)) | ((val) << (n)))
#define GET_BYTE(num, n) ((num >> (8 * n)) & 0xFF)
#define SET_BYTE(num, n, val) ((num) = ((num) & ~(0xFF << (8 * (n)))) | ((val) << (8 * (n))))

#define CW_R() SetColor(FOREGROUND_RED)                               // Красный
#define CW_G() SetColor(FOREGROUND_GREEN)                             // Зеленый
#define CW_B() SetColor(FOREGROUND_BLUE)                              // Синий
#define CW_Y() SetColor(FOREGROUND_RED | FOREGROUND_GREEN)            // Желтый
#define CW_C() SetColor(FOREGROUND_GREEN | FOREGROUND_BLUE)           // Голубой (Cyan)
#define CW_M() SetColor(FOREGROUND_RED | FOREGROUND_BLUE)             // Магента (Magenta)
#define CW_W() SetColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE) // Белый
#define CW_K() SetColor(0)                                            // Черный (выключить все цвета)
void SetColor(WORD wAttributes)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, wAttributes);
}
void copyToClipboard(const char* text)
{
	if (OpenClipboard(NULL))
	{
		EmptyClipboard();
		size_t len = strlen(text) + 1;
		HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, len);
		if (hMem) { memcpy(GlobalLock(hMem), text, len); GlobalUnlock(hMem); SetClipboardData(CF_TEXT, hMem); }
		CloseClipboard();
	}
}

void DebugCode()
{
	setlocale(LC_NUMERIC, "C");
	CW_G();

	// https://ehgames.com/gta/map/
	const char* header = "{\"name\":\"VCS\",\"activeLayer\":0,\"layers\":[{\"name\":\"Default Layer\",\"mapId\":\"VCS\",\"mapScale\":0.5,\"mapOffset\":{\"x\":0,\"y\":0},\"blips\":[],\"paths\":[],\"areas\":[";
	char* buffer = (char*)malloc(2 * 1024 * 1024);
	memset(buffer, 0, 2 * 1024 * 1024);
	int offset = 0;
	bool firstElement = true;
	offset += sprintf(buffer + offset, "%s", header);

	printf("num: %d\n\n", ThePaths->m_numPathNodes);
	for (int32_t i = 0; i < ThePaths->m_numPathNodes; ++i)
	{
		CPathNode* n = &ThePaths->m_pathNodes[i];
		float realX = n->posX / 8; //  (x/8) * 16 = gta3 ipl
		float realY = n->posY / 8; //  (x/8) * 16 = *2
		printf("n_%d: X: %f, Y:%f\n", i, realX, realY);

		if (!firstElement) { offset += sprintf(buffer + offset, ","); }
		firstElement = false;
		offset += sprintf(buffer + offset, "{\"active\":false,\"name\":\"n_%d\",\"x\":%.2f,\"y\":%.2f,\"color\":\"#ff0000\",\"radius\":3.0}", i, realX, realY);

	}


	offset += sprintf(buffer + offset, "],\"zones\":[]}],\"editMode\":\"info\",\"showLayerMenu\":false,\"showToolMenu\":true}");
	copyToClipboard(buffer);
	//FILE* js = fopen("C:\\vcstmp\\json.txt", "w");
	//fprintf(js, "%s", buffer);
	//fclose(js);
	free(buffer);
}


//void FindConnectedNodes(
//	int nodeIdx,
//	const CPathFind* paths,
//	std::unordered_set<int>& visited,
//	std::vector<int>& currentGroup
//) {
//	if (visited.count(nodeIdx)) return;
//	visited.insert(nodeIdx);
//	currentGroup.push_back(nodeIdx);
//
//	const CPathNode& node = paths->m_pathNodes[nodeIdx];
//	for (int i = 0; i < node.numLinks; ++i) {
//		int connIdx = node.firstLink + i;
//		int neighborIdx = paths->m_connections[connIdx].idx;
//		FindConnectedNodes(neighborIdx, paths, visited, currentGroup);
//	}
//}
//// Основная функция для группировки нод
//std::vector<std::vector<int>> ExtractPathGroups(const CPathFind* paths) {
//	std::unordered_set<int> visited;
//	std::vector<std::vector<int>> groups;
//
//	for (int i = 0; i < paths->m_numPathNodes; ++i) {
//		if (!visited.count(i)) {
//			std::vector<int> newGroup;
//			FindConnectedNodes(i, paths, visited, newGroup);
//			groups.push_back(newGroup);
//		}
//	}
//
//	return groups;
//}
//void PrintPathGroups(const CPathFind* paths) {
//	auto groups = ExtractPathGroups(paths);
//
//	for (size_t groupId = 0; groupId < groups.size(); ++groupId) {
//		printf("=== Group %d (%zu nodes) ===\n", groupId, groups[groupId].size());
//
//		for (int nodeIdx : groups[groupId]) {
//			CPathNode* node = &paths->m_pathNodes[nodeIdx];
//			float realX = node->posX / 8.0f;
//			float realY = node->posY / 8.0f;
//			float realZ = (node->posZ / PATHZSCALE + 1.0f);
//			//printf("Node %d: X=%f, Y=%f, Z=%f, Links=%d  spwnr %f\n", nodeIdx, realX, realY, realZ, node->numLinks, gaSpawnRate[node->spawnRateIndex] / 15.0f);
//			printf("Node %d: X=%f, Y=%f, Z=%f, Links=%d  spwnr %d\n", nodeIdx, realX, realY, realZ, node->numLinks, node->spawnRateIndex);
//		}
//	}
//}

void test2()
{
	//PrintPathGroups(ThePaths);
	return;
	for (int32_t i = 0; i < ThePaths->m_numPathNodes; ++i)
	{
		CPathNode* n = &ThePaths->m_pathNodes[i];
		///printf("num: %d\n\n", GetNumLinks(n->numLinks_Flags8));
	}
}





//#define PATHZSCALE (1.0f)
//void
//renderPathNodes(void)
//{
//	int i, j;
//	static rw::Atomic* atomic;
//	if (atomic == nil) {
//		atomic = rw::Atomic::create();
//		atomic->setGeometry(cubeGeo, 0);
//		rw::Frame* f = rw::Frame::create();
//		atomic->setFrame(f);
//	}
//
//	static int numIndices;
//	static int numVertices;
//	static rw::RWDEVICE::Im3DVertex* vertices;
//	static uint16* indices;
//	if (vertices == nil) {
//		numVertices = gpThePaths->m_numPathNodes;
//		vertices = rwNewT(rw::RWDEVICE::Im3DVertex, numVertices, 0);
//		numIndices = gpThePaths->m_numConnections * 2;
//		indices = rwNewT(uint16, numIndices, 0);
//		int idx = 0;
//		for (i = 0; i < gpThePaths->m_numPathNodes; i++) {
//			CPathNode* pn = &gpThePaths->m_pathNodes[i];
//
//			vertices[i].setX(pn->x / 8.0f);
//			vertices[i].setY(pn->y / 8.0f);
//			vertices[i].setZ(pn->z / PATHZSCALE + 1.0f);
//			if (i < gpThePaths->m_numCarPathNodes)
//				vertices[i].setColor(255, 0, 0, 255);
//			else
//				vertices[i].setColor(0, 255, 0, 255);
//
//			for (j = 0; j < pn->numLinks; j++) {
//				assert(i < numVertices);
//				indices[idx++] = i;
//				assert(gpThePaths->m_connections[pn->firstLink + j].idx < numVertices);
//				indices[idx++] = gpThePaths->m_connections[pn->firstLink + j].idx;
//				assert(idx <= numIndices);
//			}
//		}
//		assert(idx == numIndices);
//	}
//
//	rw::im3d::Transform(vertices, numVertices, nil, rw::im3d::EVERYTHING);
//	for (i = 0; i < numIndices; i += 10000)
//		rw::im3d::RenderIndexedPrimitive(rw::PRIMTYPELINELIST, indices + i, min(10000, numIndices - i));
//	rw::im3d::End();
//
//	//	return;
//
//		// Render connection flags
//	rw::Frame* f = atomic->getFrame();
//	for (i = 0; i < gpThePaths->m_numPathNodes; i++) {
//		CPathNode* pn = &gpThePaths->m_pathNodes[i];
//
//		for (j = 0; j < pn->numLinks; j++) {
//			bool draw = false;
//			cubeMat->color.red = 0;
//			cubeMat->color.green = 0;
//			cubeMat->color.blue = 0;
//			if (gpThePaths->m_connections[pn->firstLink + j].bTrafficLight) {
//				cubeMat->color.red = 255;
//				draw = true;
//			}
//			if (gpThePaths->m_connections[pn->firstLink + j].bCrossesRoad) {
//				cubeMat->color.green = 255;
//				draw = true;
//			}
//			if (!draw)
//				continue;
//			int k = gpThePaths->m_connections[pn->firstLink + j].idx;
//
//			rw::V3d pos;
//			pos.x = (gpThePaths->m_pathNodes[i].x + gpThePaths->m_pathNodes[k].x) / 8.0f / 2;
//			pos.y = (gpThePaths->m_pathNodes[i].y + gpThePaths->m_pathNodes[k].y) / 8.0f / 2;
//			pos.z = (gpThePaths->m_pathNodes[i].z + gpThePaths->m_pathNodes[k].z) / PATHZSCALE / 2 + 1.0f;
//
//			f->translate(&pos, rw::COMBINEREPLACE);
//			atomic->render();
//		}
//	}
//
//	/*
//		// nodes as boxes
//		rw::Frame *f = atomic->getFrame();
//		for(i = 0; i < gpThePaths->m_numPathNodes; i++){
//			CPathNode *pn = &gpThePaths->m_pathNodes[i];
//
//			if(i >= 0 && i < gpThePaths->m_numCarPathNodes){
//				cubeMat->color.red = 255;
//				cubeMat->color.green = 0;
//				cubeMat->color.blue = 0;
//			}else if(i < gpThePaths->m_numPathNodes){
//				cubeMat->color.red = 0;
//				cubeMat->color.green = 255;
//				cubeMat->color.blue = 0;
//			}
//
//			rw::V3d pos;
//			pos.x = pn->x/8.0f;
//			pos.y = pn->y/8.0f;
//			pos.z = pn->z/PATHZSCALE;
//
//			pos.z += 1.0f;
//
//			f->translate(&pos, rw::COMBINEREPLACE);
//			atomic->render();
//		}
//	*/
//}



//bool test() 
//{
//    CPathFind* P = ThePaths;
//    setlocale(LC_NUMERIC, "C");
//
//    for (int g = 0; g < P->m_numCarPathNodes; ++g) {
//        CCarPathLink link = P->m_carPathLinks[g];
//        int startIdx = GET_BITS(link.field_0, 0x3FFF, 0);
//        CPathNode n0 = P->m_pathNodes[startIdx];
//
//        int groupType = (n0.flags9 & 1) ? 2 : 1;
//        int base = n0.firstLink;
//        int total = n0.numLinks_Flags8 & 0x0F;
//
//        struct SubNode { int nextNode, nodeType, isCross; float X, Y, Z; int median, left, right, speed, flags; float spawn; };
//        SubNode subs[12]; int sc = 0;
//
//        for (int i = 0; i < total && sc < 12; ++i) {
//            int connGroup = P->m_carPathConnections[base + i];
//            if (connGroup < 0) continue;
//
//            int connRaw = P->m_connections[base + i];
//            int rawIdx = GET_BITS(connRaw, 0x3FFF, 0);
//            int nodeType = (connGroup == g) ? 2 : 1;
//            int isCross = GET_BIT(connRaw, 15);
//
//            CPathNode n = P->m_pathNodes[rawIdx];
//            float X = n.posX * 2.0f;
//            float Y = n.posY * 2.0f;
//            float Z = ConvertZ(n.posZ) * 16.0f;
//
//            int median = (int)n.width;
//            int left = GET_BITS(link.num_field_2, 0x07, 0);
//            int right = GET_BITS(link.num_field_2, 0xE0, 5);
//            int speed = GET_BITS(n.flags9, 0x0C, 2);
//            int flags = GET_BITS(n.flags9, 0x03, 0);
//            int spawnI = GET_BITS(n.flags9, 0x30, 4);
//            float spawn = gaSpawnRate[spawnI] / 15.0f;
//
//            subs[sc] = { sc, nodeType, isCross, X, Y, Z, median, left, right, speed, flags, spawn };
//            sc++;
//        }
//
//        if (sc == 0) continue;
//
//        printf("%d,-1\n", groupType);
//        for (int i = 0; i < sc; ++i) {
//            auto& s = subs[i];
//            printf(
//                "%d,%d,%d,%.2f,%.2f,%.2f,%d,%d,%d,%d,%d,%.2f\n",
//                s.nodeType, s.nextNode, s.isCross,
//                s.X, s.Y, s.Z,
//                s.median, s.left, s.right,
//                s.speed, s.flags,
//                s.spawn
//            );
//        }
//        printf("end\n");
//    }
//
//    for (int g = 0; g < P->m_numPedPathNodes; ++g) {
//        CCarPathLink link = P->m_carPathLinks[P->m_numCarPathNodes + g];
//        int startIdx = GET_BITS(link.field_0, 0x3FFF, 0);
//        CPathNode n0 = P->m_pathNodes[startIdx];
//
//        int base = n0.firstLink;
//        int total = n0.numLinks_Flags8 & 0x0F;
//
//        struct PedSub { int nextNode, nodeType, isCross; float X, Y, Z; float spawn; };
//        PedSub subs[12]; int sc = 0;
//
//        for (int i = 0; i < total && sc < 12; ++i) {
//            int connGroup = P->m_carPathConnections[P->m_numCarPathNodes + base + i];
//            if (connGroup < 0) continue;
//
//            int connRaw = P->m_connections[base + i];
//            int rawIdx = GET_BITS(connRaw, 0x3FFF, 0);
//            int nodeType = (connGroup == g) ? 2 : 1;
//            int isCross = GET_BIT(connRaw, 15);
//
//            CPathNode n = P->m_pathNodes[rawIdx];
//            float X = n.posX * 2.0f;
//            float Y = n.posY * 2.0f;
//            float Z = ConvertZ(n.posZ) * 16.0f;
//
//            int spawnI = GET_BITS(n.flags9, 0x30, 4);
//            float spawn = gaSpawnRate[spawnI] / 15.0f; // ?
//
//            subs[sc] = { sc, nodeType, isCross, X, Y, Z, spawn };
//            sc++;
//        }
//
//        if (sc == 0) continue;
//
//        printf("0,-1\n");
//        for (int i = 0; i < sc; ++i) {
//            auto& s = subs[i];
//            printf(
//                "%d,%d,%d,%.2f,%.2f,%.2f,0,0,0,0,0,%.2f\n",
//                s.nodeType, s.nextNode, s.isCross,
//                s.X, s.Y, s.Z,
//                s.spawn
//            );
//        }
//        printf("end\n");
//    }
//
//    return true;
//}


//static int GetNumLinks(char numLinks_Flags8) { return numLinks_Flags8 & 0x0F; }
//static bool IsDeadEnd(char numLinks_Flags8) { return (numLinks_Flags8 & 0x10) != 0; }
//static bool IsDisabled(char numLinks_Flags8) { return (numLinks_Flags8 & 0x20) != 0; }
//static bool BetweenLevels(char numLinks_Flags8) { return (numLinks_Flags8 & 0x40) != 0; }
//static bool UseInRoad(char numLinks_Flags8) { return (numLinks_Flags8 & 0x80) != 0; }
//
//static bool IsWaterPath(char flags9) { return (flags9 & 0x01) != 0; }
//static bool IsSelected(char flags9) { return (flags9 & 0x02) != 0; }
//static int GetSpeedLimit(char flags9) { return (flags9 >> 2) & 0x03; }
//static int GetSpawnRateIdx(char flags9) { return (flags9 >> 4) & 0x03; }
//
//inline uint16_t ConnectedNode(const CPathFind* pf, int linkIdx) {
//    // lo 14 бит — индекс узла
//    return pf->m_connections[linkIdx] & 0x3FFF;
//}
//inline bool ConnectionCrossesRoad(const CPathFind* pf, int linkIdx) {
//    // бит 15 (0x8000) — пересечение дороги
//    return (pf->m_connections[linkIdx] & 0x8000) != 0;
//}
//inline bool ConnectionHasTrafficLight(const CPathFind* pf, int linkIdx) {
//    // бит 14 (0x4000) — светофор
//    return (pf->m_connections[linkIdx] & 0x4000) != 0;
//}
//
//void PrintLinkInfo(int i, int j, float dist, bool crossesRoad, bool trafficLight)
//{
//    printf("    Node %4d - Node %4d | dist=%5.1f | crossesRoad=%d | trafficLight=%d\n",
//        i, j, dist, (int)crossesRoad, (int)trafficLight);
//
//}
//void PrintNodeHeader(int idx, const CPathNode& n) {
//    float realX = n.posX / 8.0f;
//    float realY = n.posY / 8.0f;
//    float realZ = n.posZ; // ConvertZ()?
//    printf("Node #%4d: (X=%.2f, Y=%.2f, Z=%.2f)  width=%.1f  links=%d%s%s%s%s\n",
//        idx,
//        realX, realY, realZ,
//        n.width / 1.0f,
//        GetNumLinks(n.numLinks_Flags8),
//        IsDeadEnd(n.numLinks_Flags8) ? " DEADEND" : "",
//        IsDisabled(n.numLinks_Flags8) ? " DISABLED" : "",
//        BetweenLevels(n.numLinks_Flags8) ? " ZLEVEL" : "",
//        UseInRoad(n.numLinks_Flags8) ? " ROADUSE" : ""
//    );
//    printf("    water=%d sel=%d speed=%d spawnIdx=%d\n",
//        (int)IsWaterPath(n.flags9),
//        (int)IsSelected(n.flags9),
//        GetSpeedLimit(n.flags9),
//        GetSpawnRateIdx(n.flags9)
//    );
//}
//void DumpInternalLinks(const CPathFind* pf) {
//    printf("=== INTERNAL LINKS ===\n");
//    for (int i = 0; i < pf->m_numPathNodes; i++) {
//        const CPathNode& n = pf->m_pathNodes[i];
//        int baseI = i / 12;
//        int numLinks = GetNumLinks(n.numLinks_Flags8);
//        if (numLinks == 0) continue;
//
//        PrintNodeHeader(i, n);
//
//        for (int k = 0; k < numLinks; k++) {
//            int connIdx = n.firstLink + k;
//            int j = ConnectedNode(pf, connIdx);
//            float dist = pf->m_distances[connIdx];
//            bool cr = ConnectionCrossesRoad(pf, connIdx);
//            bool tl = ConnectionHasTrafficLight(pf, connIdx);
//            int baseJ = j / 12;
//            if (baseI == baseJ) {
//                PrintLinkInfo(i, j, dist, cr, tl);
//            }
//        }
//    }
//}
//void DumpExternalLinks(const CPathFind* pf)
//{
//    printf("=== EXTERNAL LINKS ===\n");
//    for (int i = 0; i < pf->m_numPathNodes; i++) {
//        const CPathNode& n = pf->m_pathNodes[i];
//        int baseI = i / 12;
//        int numLinks = GetNumLinks(n.numLinks_Flags8);
//        if (numLinks == 0) continue;
//
//        PrintNodeHeader(i, n);
//
//        for (int k = 0; k < numLinks; k++) {
//            int connIdx = n.firstLink + k;
//            int j = ConnectedNode(pf, connIdx);
//            float dist = pf->m_distances[connIdx];
//            bool cr = ConnectionCrossesRoad(pf, connIdx);
//            bool tl = ConnectionHasTrafficLight(pf, connIdx);
//            int baseJ = j / 12;
//            if (baseI != baseJ) {
//                PrintLinkInfo(i, j, dist, cr, tl);
//            }
//        }
//    }
//}



//
//struct RawPathNode {
//    CPathNode  node;           // posX,posY,posZ,width,flags9…
//    CCarPathLink link;         // field_0=NextNode, field_2=lanes, field_3=IsCrossRoad, field_4=NodeType
//};
//
//// одна функция-генератор для секции
//void DumpSection(int groupType, RawPathNode* arr, int count) {
//    int numGroups = count / 12;
//    printf("path\n%d, -1\n", groupType);
//    for (int g = 0; g < numGroups; ++g) {
//        for (int k = 0; k < 12; ++k) {
//            auto& R = arr[g * 12 + k];
//            // извлечь поля
//            int NodeType = R.link.field_4;
//            int NextNode = R.link.field_0;
//            int IsCrossRoad = (int)R.link.field_3;
//            // координаты в GTA: raw posX,posY целые = мировые/16 => умножаем обратно
//            float X = (R.node.posX) * 16.0f;
//            float Y = (R.node.posY) * 16.0f;
//            float Z = (R.node.posZ) * 16.0f;
//            float Median = R.node.width;
//            int   lanes = R.link.num_field_2;
//            int   LeftLanes = lanes >> 4;       // старшие 4 бита
//            int   RightLanes = lanes & 0x0F;     // младшие 4 бита
//            int   SpeedLimit = GetSpeedLimit(R.node.flags9);
//            int   Flags = R.node.flags9;    // здесь же: WaterPath, селект и т.п.
//            // финальный spawnRate
//            int   spIdx = GetSpawnRateIdx(R.node.flags9);
//            float SpawnRate = gaSpawnRate[spIdx] / 15.0f;
//
//            printf("  %d,%d,%d,%.2f,%.2f,%.2f,%.2f,%d,%d,%d,%d,%.2f\n",
//                NodeType, NextNode, IsCrossRoad,
//                X, Y, Z, Median,
//                LeftLanes, RightLanes,
//                SpeedLimit, Flags,
//                SpawnRate
//            );
//        }
//    }
//    printf("end\n\n");
//}
//
//// главный дампер
//void DumpAllPaths(RawPathNode* allNodes, CPathFind* pf) {
//    // CAR + WATER в первых m_numCarPathNodes
//    // PED — в оставшихся
//    int carCount = pf->m_numCarPathNodes;
//    int pedCount = pf->m_numPedPathNodes;
//    // динамические списки
//    RawPathNode* carArr = (RawPathNode*)malloc(sizeof(RawPathNode) * carCount);
//    RawPathNode* waterArr = (RawPathNode*)malloc(sizeof(RawPathNode) * carCount);
//    RawPathNode* pedArr = (RawPathNode*)malloc(sizeof(RawPathNode) * pedCount);
//    int c = 0, w = 0, p = 0;
//
//    // разделяем кар/вотер
//    for (int i = 0; i < carCount; ++i) {
//        RawPathNode R = allNodes[i];
//        if (IsWaterPath(R.node.flags9)) waterArr[w++] = R;
//        else                               carArr[c++] = R;
//    }
//    // теперь пешеходы
//    for (int i = carCount; i < carCount + pedCount; ++i)
//        pedArr[p++] = allNodes[i];
//
//    // дампим
//    DumpSection(1, carArr, c);
//    DumpSection(2, waterArr, w);
//    DumpSection(0, pedArr, p);
//
//    free(carArr);
//    free(waterArr);
//    free(pedArr);
//}
//
//RawPathNode* BuildRawArray(CPathFind* pf) {
//    int total = pf->m_numPathNodes;
//    RawPathNode* arr = (RawPathNode*)malloc(sizeof(RawPathNode) * total);
//    // сначала копируем все CPathNode и соответствующий CCarPathLink
//    for (int i = 0; i < total; ++i) {
//        arr[i].node = pf->m_pathNodes[i];
//        // берём ссылку на link по индексу: для пешеходов просто обнуляем
//        if (i < pf->m_numCarPathNodes) {
//            arr[i].link = pf->m_carPathLinks[i];
//        }
//        else {
//            // для Ped-звеньев ничего нет: обнуляем
//            memset(&arr[i].link, 0, sizeof(CCarPathLink));
//            arr[i].link.field_4 = 0; // NodeType = Null
//            arr[i].link.field_0 = -1;
//        }
//    }
//    return arr;
//}
//
//void dump777()
//{
//    RawPathNode* all = BuildRawArray(ThePaths);
//    DumpSection(1, all, ThePaths->m_numCarPathNodes);
//    DumpSection(2, all, ThePaths->m_numCarPathNodes);
//    DumpSection(0, all + ThePaths->m_numCarPathNodes, ThePaths->m_numPedPathNodes);
//    free(all);
//}



void DumpRoadsToFile(CPathFind* pf, const char* filename)
{
	FILE* f = std::fopen(filename, "w");
	if (!f) return;

	std::fprintf(f, "hdr\n");
	std::fprintf(f,
		"%d %d %d %d %d %d %d\n",
		pf->m_numPathNodes,
		pf->m_numCarPathNodes,
		pf->m_numPedPathNodes,
		pf->m_numMapObjects,
		pf->m_numConnections,
		pf->m_numCarPathConnections,
		pf->m_numCarPathLinks
	);
	std::fprintf(f, "end\n\n");


	// --- nodes
	std::fprintf(f, "node\n");
	for (int i = 0; i < pf->m_numPathNodes; i++) {
		const CPathNode& n = pf->m_pathNodes[i];
		// Распакуем флаги из битовых полей:
		uint8_t dead = n.bDeadEnd;
		uint8_t dis = n.bDisabled;
		uint8_t between = n.bBetweenLevels;
		uint8_t roadblk = n.bUseInRoadBlock;
		uint8_t water = n.bWaterPath;
		uint8_t sel = n.bSelected;
		uint8_t spd = n.speedLimit;
		uint8_t spawn = n.spawnRateIndex;

		float x = n.posX / 8.0f;
		float y = n.posY / 8.0f;
		float z = n.posZ;

		////pos.z = ((n.z / PATHZSCALE) + 1.0f);
		//if (pos.z < 0)
		//	pos.z -= 100;
		//pos.z = ((pos.z / PATHZSCALE) + 1.0f);

		//float PATHZSCALE = 1.0f;
		//if (fz < 0)
		//	fz -= 100.0f; // CPathFind::FindNextNodeWandering() vcs
		////fz = ((fz / PATHZSCALE) + 1.0f); // ped path in mall to hight
		//fz = ((fz / PATHZSCALE));

		//if (i < m_numCarPathNodes) // car
		//	fz += 1.0f;

		std::fprintf(f,
			//"%f %f %f %u %d %d %u %u %u %u %u %u %u %u\n",
			//"%d %f %f %f %u %d %d %u %u %u %u %u %u %u %u\n", i,
			"[%d] %f %f %f %u %d %d %u %u %u %u %u %u %u %u\n", i,
			x, y, z,
			n.width,
			n.firstLink,
			(int)n.numLinks,
			dead, dis, between, roadblk,
			water, sel, spd, spawn
		);
	}
	std::fprintf(f, "end\n\n");

	// --- clnk (CCarPathLink)
	std::fprintf(f, "clnk\n");
	for (int i = 0; i < pf->m_numCarPathLinks; i++) {
		const CCarPathLink& L = pf->m_carPathLinks[i];
		uint16_t idx = L.pathNodeIndex;
		uint8_t  u1 = L.unk1;
		uint8_t  u2 = L.unk2;
		int8_t   left = L.numLeftLanes;
		int8_t   right = L.numRightLanes;
		uint8_t  u3 = L.unk3;
		uint8_t  u4 = L.unk4;
		int8_t   u5 = L.unk5;
		std::fprintf(f,
			//"%u %u %u %d %d %u %u %d\n",
			//"%d %u %u %u %d %d %u %u %d\n", i,
			"[%d] %u %u %u %d %d %u %u %d\n", i,
			idx, u1, u2, left, right, u3, u4, u5
		);
	}
	std::fprintf(f, "end\n\n");

	// --- ccon (массив uint16_t, авто-соединения)
	std::fprintf(f, "ccon\n");
	for (int i = 0; i < pf->m_numCarPathConnections; i++) {
		//uint16_t v = pf->m_carPathConnections[i];
		//uint16_t idx = v & 0x3FFF;
		//uint8_t  lightFlag = (v >> 14) & 1;
		//uint8_t  crossFlag = (v >> 15) & 1;
		CPathConnection v = *(CPathConnection*)&(pf->m_carPathConnections[i]);
		uint16_t idx = v.idx;
		uint8_t  lightFlag = v.bTrafficLight;
		uint8_t  crossFlag = v.bCrossesRoad;
		//std::fprintf(f, "%u %u %u\n", idx, lightFlag, crossFlag);
		//std::fprintf(f, "%d %u %u %u\n", i, idx, lightFlag, crossFlag);
		std::fprintf(f, "[%d] %u %u %u\n", i, idx, lightFlag, crossFlag);
	}
	std::fprintf(f, "end\n\n");

	// --- pcon (CPathConnection)
	std::fprintf(f, "pcon\n");
	for (int i = 0; i < pf->m_numConnections; i++) {
		const CPathConnection& c = pf->m_connections[i];
		// Здесь CPathConnection уже хранит биты в полях:
		//std::fprintf(f, "%u %u %u\n",
		//std::fprintf(f, "%d %u %u %u\n", i,
		std::fprintf(f, "[%d] %u %u %u\n", i,
			c.idx,
			(int)c.bTrafficLight,
			(int)c.bCrossesRoad
		);
	}
	std::fprintf(f, "end\n\n");

	// --- dist (uint8_t дальности)
	std::fprintf(f, "dist\n");
	for (int i = 0; i < pf->m_numConnections; i++) {
		//std::fprintf(f, "%u\n", pf->m_distances[i]);
		//std::fprintf(f, "%d %u\n", i, pf->m_distances[i]);
		std::fprintf(f, "[%d] %u\n", i, pf->m_distances[i]);
	}
	std::fprintf(f, "end\n");

	std::fclose(f);
}



bool ExtractPaths()
{
	setlocale(LC_NUMERIC, "C");
	//DebugCode(); // json gen
	//DumpRoadsToFile(ThePaths, "C:\\vcstmp\\PATH.DAT");
	DumpRoadsToFile(ThePaths, "PATH.DAT");
    //test2();
    //DumpInternalLinks(ThePaths);
    //DumpExternalLinks(ThePaths);
    //dump777();

	return true;
}