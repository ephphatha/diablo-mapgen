#pragma once
#include "../types.h"

extern BYTE gbMaxPlayers;
extern BOOL leveldebug;
extern BOOL light4flag;
extern DWORD glSeedTbl[NUMLEVELS];
extern _gamedata sgGameInitInfo;
extern int gnLevelTypeTbl[NUMLEVELS];
extern int gnDifficulty;
extern PlayerStruct plr[MAX_PLRS];

BOOL delta_quest_inited(int i);
int random_(BYTE idx, int v);
void app_fatal(const char* dummystring);
void SetAutomapView(int nXPos, int nYPos);
void SetRndSeed(int s);
int GetRndSeed();

BYTE* DiabloAllocPtr(DWORD dwBytes);
void mem_free_dbg(void* p);
BYTE* LoadFileInMem(const char* pszName, DWORD* pdwFileLen);
void LoadLvlGFX();
