#include "level.h"

#include <cstdint>
#include <iostream>
#include <stdio.h>

#include "../types.h"
#include "engine.h"
#include "gendung.h"
#include "monstdat.h"
#include "monster.h"
#include "objdat.h"
#include "objects.h"

inline void WriteLE16(FILE *out, uint16_t val)
{
	fwrite(&val, 1, 2, out);
}

void ExportDun(uint32_t seed)
{
	char fileName[32];
	sprintf(fileName, "%u-%u-%u.dun", seed, currlevel, glSeedTbl[currlevel]);
	FILE *dunFile = fopen(fileName, "wb");

	WriteLE16(dunFile, DMAXX);
	WriteLE16(dunFile, DMAXY);

	/** Tiles. */
	for (int y = 0; y < DMAXY; y++) {
		for (int x = 0; x < DMAXX; x++) {
			WriteLE16(dunFile, dungeon[x][y]);
		}
	}

	/** Padding */
	for (int y = 16; y < MAXDUNY - 16; y++) {
		for (int x = 16; x < MAXDUNX - 16; x++) {
			WriteLE16(dunFile, 0);
		}
	}

	/** Monsters */
	for (int y = 16; y < MAXDUNY - 16; y++) {
		for (int x = 16; x < MAXDUNX - 16; x++) {
			uint16_t monsterId = 0;
			if (dMonster[x][y] > 0) {
				for (int i = 0; i < 128; i++) {
					if (MonstConvTbl[i] == monster[dMonster[x][y] - 1].MType->mtype) {
						monsterId = i + 1;
						break;
					}
				}
			}
			WriteLE16(dunFile, monsterId);
		}
	}

	/** Objects */
	for (int y = 16; y < MAXDUNY - 16; y++) {
		for (int x = 16; x < MAXDUNX - 16; x++) {
			uint16_t objectId = 0;
			if (dObject[x][y] > 0) {
				for (int i = 0; i < 113; i++) {
					if (ObjTypeConv[i] == object[dObject[x][y] - 1]._otype) {
						objectId = i;
						break;
					}
				}
			}
			WriteLE16(dunFile, objectId);
		}
	}

	/** Transparency */
	for (int y = 16; y < MAXDUNY - 16; y++) {
		for (int x = 16; x < MAXDUNX - 16; x++) {
			WriteLE16(dunFile, dTransVal[x][y]);
		}
	}
	fclose(dunFile);
}

void printAsciiLevel(Point spawn, Point stairsDown, char Path[MAX_PATH_LENGTH])
{
	bool steps[MAXDUNX][MAXDUNY];

	for (int i = 0; i < MAXDUNY; ++i) {
		for (int j = 0; j < MAXDUNX; ++j) {
			steps[i][j] = false;
		}
	}

	Point position = spawn;
	steps[position.x][position.y] = true;

	const char pathxdir[9] = { 0, 0, -1, 1, 0, -1, 1, 1, -1 };
	const char pathydir[9] = { 0, -1, 0, 0, 1, -1, -1, 1, 1 };

	for (int i = 0; i < MAX_PATH_LENGTH; ++i) {
		if (Path[i] == 0)
			break;
		position.x += pathxdir[Path[i]];
		position.y += pathydir[Path[i]];
		steps[position.x][position.y] = true;
	}

	for (int boby = 16; boby < MAXDUNY - 17; boby++) {
		for (int bobx = 16; bobx < MAXDUNX - 17; bobx++) {
			if (spawn.x == bobx && spawn.y == boby)
				std::cout << "^";
			else if (stairsDown.x == bobx && stairsDown.y == boby)
				std::cout << "v";
			else if (steps[bobx][boby])
				std::cout << "=";
			else if (nSolidTable[dPiece[bobx][boby]])
				std::cout << "#";
			else
				std::cout << " ";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}
