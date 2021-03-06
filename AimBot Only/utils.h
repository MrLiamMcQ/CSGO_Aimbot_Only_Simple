#pragma once

#include <ostream>
typedef union vec3 {
	struct {
		float x, y, z;
	};
	float data[3];
	friend vec3 operator + (vec3 const & lhs, vec3 const & rhs);
	friend vec3 operator - (vec3 const & lhs, vec3 const & rhs);
	vec3& operator *= (float number);
	vec3& operator -= (vec3 const & rhs);
	friend std::ostream& operator<<(std::ostream& os, vec3 const& pos);
};

#include <windows.h>
#include <Psapi.h>
#include <algorithm>
#include <string>
DWORD findPattern(const char* moduleName, std::string pattern);

// from https://github.com/MrLiamMcQ/csgoPlayerStructMakerForHacks		https://botliam.xyz/csgoPlayerStruct.php
struct PlayerClass {
	unsigned long cs_gamerules_data;
	char _0x70[28];
	bool m_bFreezePeriod;
	char _0x74[79];
	unsigned long m_clrRender;
	bool m_bIsQueuedMatchmaking;
	bool m_bIsValveDS;
	char _0x100[126];
	int m_iTeamNum;
	char _0x104[8];
	int m_iHealth;
	int m_fFlags;
	vec3 m_vecViewOffset;
	vec3 m_vecVelocity;
	char _0x14C[12];
	vec3 m_vec3ViewAngle;
	vec3 m_vecOrigin;
	char _0x25C[8];
	int m_hOwnerEntity;
	char _0x25F[268];
	unsigned char m_MoveType;
	char _0x268[2];
	unsigned char m_lifeState;
	char _0x31C[8];
	float m_flSimulationTime;
	char _0x444[176];
	unsigned long m_Collision;
	char _0x474[292];
	unsigned long m_rgflCoordinateFrame;
	char _0x93D[44];
	unsigned long m_CollisionGroup;
	char _0x980[1221];
	bool m_bSpotted;
	char _0x99D[66];
	bool m_bSpottedByMask;
	char _0xCF8[28];
	bool m_bBombPlanted;
	char _0x1320[858];
	unsigned long m_SurvivalRules;
	char _0x1A84[1572];
	unsigned long m_SurvivalGameRuleDecisionTypes;
	char _0x1B88[1888];
	int m_iCompetitiveRanking;
	char _0x268C[256];
	int m_iCompetitiveWins;
	char _0x2690[2816];
	unsigned long m_nForceBone;
	int m_iMostRecentModelBoneCounter;
	char _0x2924[20];
	unsigned long m_dwBoneMatrix;
	char _0x2990[632];
	float m_flLastBoneSetupTime;
	char _0x2994[104];
	float m_flC4Blow;
	float m_flTimerLength;
	char _0x29AC[16];
	float m_flDefuseLength;
	float m_flDefuseCountDown;
	char _0x2D70[28];
	int m_hOwner;
	char _0x2DF8[928];
	float m_flNextAttack;
	char _0x2EF8[132];
	int m_hMyWeapons;
	char _0x2FAA[252];
	int m_hActiveWeapon;
	char _0x2FAC[174];
	unsigned char m_iItemDefinitionIndex;
	char _0x2FBC[1];
	int m_iEntityQuality;
	char _0x2FC0[12];
	unsigned long m_Local;
	unsigned char m_iItemIDHigh;
	char _0x302C[7];
	int m_iAccountID;
	char _0x3038[96];
	vec3 m_aimPunchAngle;
	int m_aimPunchAngleVel;
	unsigned long m_szCustomName;
	char _0x31B4[368];
	unsigned long m_OriginalOwnerXuidLow;
	unsigned long m_OriginalOwnerXuidHigh;
	unsigned long m_nFallbackPaintKit;
	unsigned long m_nFallbackSeed;
	float m_flFallbackWear;
	unsigned long m_nFallbackStatTrak;
	char _0x31E4[16];
	unsigned long m_thirdPersonViewAngles;
	char _0x31E8[8];
	int m_iFOV;
	int m_iFOVStart;
	char _0x3238[44];
	float m_flNextPrimaryAttack;
	char _0x3244[28];
	int m_iState;
	char _0x3285[8];
	int m_iClip1;
	char _0x3310[61];
	bool m_bInReload;
	char _0x3374[138];
	float m_fAccuracyPenalty;
	char _0x3388[96];
	int m_iObserverMode;
	char _0x33D0[16];
	int m_hObserverTarget;
	char _0x342C[68];
	bool m_bStartedArming;
	char _0x35B0[91];
	unsigned long m_nTickBase;
	char _0x3910[384];
	unsigned long m_szLastPlaceName;
	char _0x3918[860];
	bool m_bIsScoped;
	char _0x392C[7];
	bool m_bIsDefusing;
	char _0x3A78[19];
	bool m_bGunGameImmunity;
	char _0xA380[331];
	float m_flLowerBodyYawTarget;
	char _0xA3F0[26884];
	int m_iShotsFired;
	char _0xA3F4[108];
	float m_flFlashMaxAlpha;
	float m_flFlashDuration;
	char _0xB334[20];
	int m_iGlowIndex;
	char _0xB340[3876];
	bool m_bHasHelmet;
	char _0xB344[11];
	unsigned long m_ArmorValue;
	unsigned long m_angEyeAnglesX;
	unsigned long m_angEyeAnglesY;
	char _0xB3AC[4];
	bool m_bHasDefuser;
};