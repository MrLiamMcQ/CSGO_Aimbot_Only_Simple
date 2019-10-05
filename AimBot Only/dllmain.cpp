// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"

union vec3 {
	float data[3];
	struct {
		float x, y, z;
	};
	friend vec3 operator + (vec3 const & lhs, vec3 const & rhs) {
		return vec3{ lhs.x + rhs.x,lhs.y + rhs.y,lhs.z + rhs.z };
	}
	friend vec3 operator - (vec3 const & lhs, vec3 const & rhs) {
		return vec3{ lhs.x - rhs.x,lhs.y - rhs.y,lhs.z - rhs.z };
	}
	friend std::ostream& operator<<(std::ostream& os, vec3 const& pos) {
		os << "xyz: " << pos.x << "," << pos.y << "," << pos.z;
		return os;
	}
};

// from https://github.com/MrLiamMcQ/csgoPlayerStructMakerForHacks 
// from https://botliam.xyz/csgoPlayerStruct.php
// 1 assembly code faster than the most optimised traditional method
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
};

#define M_PI 3.1415927f

bool ClampAngle(vec3& angle) {
	if (angle.x > 89.0f) {
		angle.x = 89.0f;
		return false;
	}if (angle.x < -89.0f) {
		angle.x = -89;
		return false;
	}if (angle.y > 180.f) { angle.y = 180; return false; }
	if (angle.y < -180.f) { angle.y = -180; return false; }
}
void Normalise(vec3& angle) {
	while (angle.x > 89.0f) {
		angle.x -= 180.0f;
	}while (angle.x < -89.0f) {
		angle.x += 180.0f;
	}while (angle.y > 180) { angle.y -= 360.f; }
	while (angle.y < -180) { angle.y += -360.f; }
	angle.z = 0.0f;
};

vec3 calcAngle(const vec3& vecSource, const vec3& vecDestination)
{
	vec3 qAngles;
	vec3 delta = vec3{ (vecSource.data[0] - vecDestination.data[0]), (vecSource.data[1] - vecDestination.data[1]), (vecSource.data[2] - vecDestination.data[2]) };
	float hyp = sqrtf(delta.data[0] * delta.data[0] + delta.data[1] * delta.data[1]);
	qAngles.data[0] = (float)(atan(delta.data[2] / hyp) * (180.0f / M_PI));
	qAngles.data[1] = (float)(atan(delta.data[1] / delta.data[0]) * (180.0f / M_PI));
	qAngles.data[2] = 0.f;
	if (delta.data[0] >= 0.f)
		qAngles.data[1] += 180.f;

	return qAngles;
}

float getDistance(vec3 point1,vec3 point2) {
	vec3 delta = point1 - point2;
	std::cout << delta << std::endl;
	//Normalise(delta);
	float temp = (delta.x*delta.x) + (delta.y*delta.y) + (delta.z*delta.z);
	return sqrtf(temp);
}

vec3 getBone(const DWORD boneaddy, const int id) {
	vec3 bone;
	bone.x = *(float*)((boneaddy + 0x30 * id + 0x0C));
	bone.y = *(float*)((boneaddy + 0x30 * id + 0x1C));
	bone.z = *(float*)((boneaddy + 0x30 * id + 0x2C));
	return bone;
};

DWORD ClientDLL, EngineDLL;

DWORD dwEntityList = 0x4D09F44;
DWORD dwLocalPlayer = 0xCF7A4C;
DWORD dwClientState = 0x590D8C;

DWORD dwClientState_ViewAngles = 0x4D88;

#define numbEnemys 5

[[noreturn]] void main() {
	ClientDLL = (DWORD)GetModuleHandle(L"client_panorama.dll");
	EngineDLL = (DWORD)GetModuleHandle(L"engine.dll");

	PlayerClass* enemies[numbEnemys];
	
	DWORD clientState = *(DWORD*)(EngineDLL + dwClientState);
	vec3* myViewAngle = (vec3*)(clientState + dwClientState_ViewAngles);
	
	while (1) {
		char numbOfEnemies = 0;
		std::this_thread::sleep_for(std::chrono::nanoseconds(1));
		
		// only do when a new round starts
		PlayerClass localPlayer = *(PlayerClass*)*(DWORD*)(ClientDLL + dwLocalPlayer);
		memset(&enemies,0,sizeof(DWORD*)*numbEnemys);
		for (int i = 1; i < numbEnemys*2; i++) {
			PlayerClass* entity = (PlayerClass*)*(DWORD*)(ClientDLL + dwEntityList + (i) * 0x10);
			if (entity->m_iTeamNum != localPlayer.m_iTeamNum) {
				numbOfEnemies++;
				for (int y = 0; y < numbEnemys; y++) {
					if (enemies[y] == NULL) {
						enemies[y] = (PlayerClass*)*(DWORD*)(ClientDLL + dwEntityList + (i) * 0x10);
						break;
					}
				}
			}
		}

		if (GetAsyncKeyState(VK_LBUTTON)) {
			int numberOfTargets = 0;

			std::pair<std::optional<float>, vec3> lowestDistance{ std::nullopt,{0,0,0} };
			bool firstTime = 1;

			for (int i = 0; i < numbEnemys; i++) {
				if (enemies[i] == NULL) { continue; };
				if (*(char*)((DWORD)enemies[i] + 0xED) == 1) { continue; };// dormant cheak
				if (enemies[i]->m_iHealth > 100) { continue; };
				if (enemies[i]->m_lifeState != 0) { continue; };
				if (enemies[i]->m_iHealth <= 0) { continue; };
				numberOfTargets++;

				vec3 localHeadPos = localPlayer.m_vecViewOffset + localPlayer.m_vecOrigin;
				vec3 enHeadPos = getBone(enemies[i]->m_dwBoneMatrix, 8);

				vec3 AimAngle = calcAngle(localHeadPos, enHeadPos);
				std::cout << "test" << std::endl;
				float distance = getDistance(localPlayer.m_vec3ViewAngle,AimAngle);
				std::cout << distance << std::endl;
				if (distance < lowestDistance.first || firstTime) {
					lowestDistance.first = distance;
					lowestDistance.second = AimAngle;
					firstTime = 0;
				}
			}

			if (lowestDistance.first.has_value()) {
				Normalise(lowestDistance.second);
				ClampAngle(lowestDistance.second);
				*myViewAngle = lowestDistance.second;
			}
			//std::cout << "targets: " << numberOfTargets << " enemies: " << (int)numbOfEnemies  << std::endl;
		}
	}
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
		AllocConsole();
		freopen("conin$", "r", stdin);
		freopen("conout$", "w", stdout);
		freopen("conout$", "w", stderr);
		system("cls");
		printf("Debugging Window:\n");

		std::thread main(main);
		main.detach();
    }
    return TRUE;
}

