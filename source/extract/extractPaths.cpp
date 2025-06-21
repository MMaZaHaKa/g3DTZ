#include "Extract.h"
#include "PathFind.h"
//#include "Utils.h"
//#include "FileSystem.h"
#include <iostream>
#include <fstream>
#include "Windows.h" // :D

#define ThePaths (_CPathFind::mspInst)
int gaSpawnRate[] = { 0, 4, 7, 0xF };

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


float ConvertZ(int8_t posZ)
{
	int v = (int)posZ;
	if (v < 0.0f)
		v -= 100.0f;
	return v / 8.0f;
}

bool test() 
{
    CPathFind* P = ThePaths;
    setlocale(LC_NUMERIC, "C");

    for (int g = 0; g < P->m_numCarPathNodes; ++g) {
        CCarPathLink link = P->m_carPathLinks[g];
        int startIdx = GET_BITS(link.field_0, 0x3FFF, 0);
        CPathNode n0 = P->m_pathNodes[startIdx];

        int groupType = (n0.flags9 & 1) ? 2 : 1;
        int base = n0.firstLink;
        int total = n0.numLinks_Flags8 & 0x0F;

        struct SubNode { int nextNode, nodeType, isCross; float X, Y, Z; int median, left, right, speed, flags; float spawn; };
        SubNode subs[12]; int sc = 0;

        for (int i = 0; i < total && sc < 12; ++i) {
            int connGroup = P->m_carPathConnections[base + i];
            if (connGroup < 0) continue;

            int connRaw = P->m_connections[base + i];
            int rawIdx = GET_BITS(connRaw, 0x3FFF, 0);
            int nodeType = (connGroup == g) ? 2 : 1;
            int isCross = GET_BIT(connRaw, 15);

            CPathNode n = P->m_pathNodes[rawIdx];
            float X = n.posX * 2.0f;
            float Y = n.posY * 2.0f;
            float Z = ConvertZ(n.posZ) * 16.0f;

            int median = (int)n.width;
            int left = GET_BITS(link.num_field_2, 0x07, 0);
            int right = GET_BITS(link.num_field_2, 0xE0, 5);
            int speed = GET_BITS(n.flags9, 0x0C, 2);
            int flags = GET_BITS(n.flags9, 0x03, 0);
            int spawnI = GET_BITS(n.flags9, 0x30, 4);
            float spawn = gaSpawnRate[spawnI] / 15.0f;

            subs[sc] = { sc, nodeType, isCross, X, Y, Z, median, left, right, speed, flags, spawn };
            sc++;
        }

        if (sc == 0) continue;

        printf("%d,-1\n", groupType);
        for (int i = 0; i < sc; ++i) {
            auto& s = subs[i];
            printf(
                "%d,%d,%d,%.2f,%.2f,%.2f,%d,%d,%d,%d,%d,%.2f\n",
                s.nodeType, s.nextNode, s.isCross,
                s.X, s.Y, s.Z,
                s.median, s.left, s.right,
                s.speed, s.flags,
                s.spawn
            );
        }
        printf("end\n");
    }

    for (int g = 0; g < P->m_numPedPathNodes; ++g) {
        CCarPathLink link = P->m_carPathLinks[P->m_numCarPathNodes + g];
        int startIdx = GET_BITS(link.field_0, 0x3FFF, 0);
        CPathNode n0 = P->m_pathNodes[startIdx];

        int base = n0.firstLink;
        int total = n0.numLinks_Flags8 & 0x0F;

        struct PedSub { int nextNode, nodeType, isCross; float X, Y, Z; float spawn; };
        PedSub subs[12]; int sc = 0;

        for (int i = 0; i < total && sc < 12; ++i) {
            int connGroup = P->m_carPathConnections[P->m_numCarPathNodes + base + i];
            if (connGroup < 0) continue;

            int connRaw = P->m_connections[base + i];
            int rawIdx = GET_BITS(connRaw, 0x3FFF, 0);
            int nodeType = (connGroup == g) ? 2 : 1;
            int isCross = GET_BIT(connRaw, 15);

            CPathNode n = P->m_pathNodes[rawIdx];
            float X = n.posX * 2.0f;
            float Y = n.posY * 2.0f;
            float Z = ConvertZ(n.posZ) * 16.0f;

            int spawnI = GET_BITS(n.flags9, 0x30, 4);
            float spawn = gaSpawnRate[spawnI] / 15.0f; // ?

            subs[sc] = { sc, nodeType, isCross, X, Y, Z, spawn };
            sc++;
        }

        if (sc == 0) continue;

        printf("0,-1\n");
        for (int i = 0; i < sc; ++i) {
            auto& s = subs[i];
            printf(
                "%d,%d,%d,%.2f,%.2f,%.2f,0,0,0,0,0,%.2f\n",
                s.nodeType, s.nextNode, s.isCross,
                s.X, s.Y, s.Z,
                s.spawn
            );
        }
        printf("end\n");
    }

    return true;
}





bool ExtractPaths()
{
	setlocale(LC_NUMERIC, "C");
	//DebugCode();
    test();

	return true;
}