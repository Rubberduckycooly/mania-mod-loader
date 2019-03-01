#pragma once
#include <cstdio>
#include "MemAccess.h"

static const int ModLoaderVer = 1;
static const int GameVer = 4;

struct PatchInfo
{
	void *address;
	const void *data;
	int datasize;
};

struct PatchList
{
	const PatchInfo *Patches;
	int Count;
};

struct PointerInfo
{
	void *address;
	void *data;
};

struct PointerList
{
	const PointerInfo *Pointers;
	int Count;
};

typedef void(__cdecl *ModInitFunc)(const char *path);

typedef void(__cdecl *ModEvent)();

struct ModInfo
{
	int LoaderVersion;
	int GameVersion;
};

typedef uint32_t _DWORD;
typedef uint16_t _WORD;
typedef uint8_t _BYTE;

enum ShieldTypes
{
	ShieldTypes_None,
	ShieldTypes_Regular,
	ShieldTypes_Bubble,
	ShieldTypes_Flame,
	ShieldTypes_Lightning
};

enum Characters
{
	Characters_None		= 0b00000,
	Characters_Sonic	= 0b00001,
	Characters_Tails	= 0b00010,
	Characters_Knuckles = 0b00100,
	Characters_Mighty	= 0b01000,
	Characters_Ray		= 0b10000
};

struct fileinfo
{
	int FileSize;
	_DWORD Unknown04;
	FILE* File;
	int Unknown0C;
	int Unknown10;
	BYTE* FileAddress;
	bool Unknown18;
	bool IsEncrypted;
	_BYTE decryptionkey[32];
	_BYTE gap3A[0x2000];
	/*_DWORD fileoff;
	_DWORD size;
	_DWORD dword4010;
	_DWORD dword4014;
	_DWORD dword4018;
	_DWORD dword401C;
	_DWORD encrypted;
	_DWORD dword4024;
	_BYTE decryptionkey[32];
	_WORD word4048;
	_BYTE byte404A;*/
};

struct fileheader
{
	int filenamehash[4];
	int offset;
	int size;
	int encrypted;
	char index;
	char anonymous_8[3];
};

// TODO: Player structure has changed a little, This needs updating
struct PlayerData
{
	char field_0[2];
	__int16 XPos;
	char field_4[2];
	__int16 YPos;
	_DWORD dword8;
	_DWORD dwordC;
	int XSpeed;
	int YSpeed;
	char field_18[40];
	_DWORD dword40;
	_BYTE gap44[8];
	_BYTE byte4C;
	char field_4D;
	_BYTE byte4E;
	_BYTE byte4F;
	_BYTE gap50[2];
	_BYTE byte52;
	char field_53;
	_BYTE byte54;
	char field_55[3];
	_DWORD dword58;
	_BYTE gap5C[60];
	_DWORD dword98;
	_DWORD dword9C;
	_DWORD dwordA0;
	char field_A4[12];
	_WORD wordB0;
	_WORD wordB2;
	_BYTE gapB4[2];
	_WORD wordB6;
	char field_B8[8];
	_DWORD Character;
	_DWORD RingCount;
	_DWORD dwordC8;
	_DWORD ShieldAbility;
	_DWORD LifeCount;
	_DWORD dwordD4;
	_DWORD dwordD8;
	_DWORD CombineRing;
	char field_E0[68];
	int SuperMode;
	char field_128[12];
	_DWORD dword134;
	_BYTE gap138[4];
	_DWORD dword13C;
	_BYTE gap140[60];
	_DWORD dword17C;
	_DWORD dword180;
	_DWORD dword184;
	_DWORD dword188;
	_DWORD dword18C;
	_DWORD dword190;
	_BYTE gap194[8];
	_DWORD dword19C;
	_DWORD dword1A0;
	char field_1A4[32];
	int JumpAbilityFlag;
	void(__cdecl *JumpAbilities)();
	void(__cdecl *PeelOutThing)();
};

// define function and variable pointers here
DataPointer(BYTE, ReadFromPack, 0x6F1804);
DataPointer(BYTE, ConsoleEnabled, 0x6F1806);

DataPointer(float, MusicVolume, 0xE4805C);
DataPointer(PlayerData, Player1Data, 0x85E9A0);
DataPointer(PlayerData, Player2Data, 0x85EDF8);
DataPointer(int, TimerCentiframes, 0xE4874D);
DataPointer(int, DebugEnabled, 0xE48768);
DataPointer(int, UpdateTimer, 0xE48770);
DataPointer(char, GameMode, 0xE487787);
DataPointer(char, TimerCentiseconds, 0xE48778);
DataPointer(char, TimerSeconds, 0xE48779);
DataPointer(char, TimerMinutes, 0xE4877A);

// If a function has a jump function where the function is just a jump to the proper
// function it should be used whenever possible. These usually stay in the same spots in 
// the same general area between versions and improves the chances of future version
// compatibility. These functions are almost always compiler generated, Hence the close
// proximity.
FunctionPointer(int, playWav, (_WORD wavIndex, int a2, _BYTE playLength), 0x5BBBF0);
FunctionPointer(_WORD, loadWav, (const char *name), 0x5BBB50);

// These functions are a special case. Their jump functions do not align between versions so we'll use their stored 
// jump functions in a stored "offset" which does match between versions. Most functions have these as well 
// but usually isn't needed to get a working function. To make matters worse. Some functions don't have a jump function
// but do have an "offset". Meaning we'll need 2 different ways to get these and those for multi-version support,
// and to make things worse. The "offsets" are dynamically set, We'll have to use them only in runtime.
OffsetFunctionPointer(void *, spawnObject, (_WORD objectID, _DWORD type, _DWORD x, _DWORD y), 0xE9C700); // It's return is a object unique pointer to the object's memory.
#define PrintDebug printf

VoidFunc(InitPlayer, 0x4C33F0);
FastcallFunctionPointer(int, Player_CheckGoSuper, (PlayerData *a1, int emeraldflags), 0x4C8280);
VoidFunc(Sonic_JumpAbilities, 0x4C8630);
VoidFunc(Tails_JumpAbilities, 0x4C8990);
VoidFunc(Knuckles_JumpAbilities, 0x4C8A70);
VoidFunc(Mighty_JumpAbilities, 0x4C8B70);
VoidFunc(Ray_JumpAbilities, 0x4C8DF0);
VoidFunc(Sonic_CheckDoPeelOut, 0x4C8FF0);
VoidFunc(ERZSuperSonic_JumpAbilities, 0x4C2340);
FastcallFunctionPointer(void, HashFilename, (char *filename, int *hash), 0x5CAE80); // Incorrect address on version 1.05.0713 (Before Patch).
VoidFunc(MainGameLoop, 0x5C7540); 
//VoidFunc(IncrementTimer, 0x5C74E0);
