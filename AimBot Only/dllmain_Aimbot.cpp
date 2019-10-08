// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "PlayerStruct_Vec3.h"

#define M_PI 3.1415927f

void clampAngle(vec3& angle) {
	if (angle.x > 89.0f) angle.x = 89.f;
	if (angle.x < -89.0f) angle.x = -89.f;

	if (angle.y > 180.f) angle.y = 180.f; 
	if (angle.y < -180.f) angle.y = -180.f; 

	angle.z = 0.0f;
}

void normalise(vec3& angle) {
	if (angle.x > 89.0f) angle.x -= 180.0f;
	if (angle.x < -89.0f) angle.x += 180.0f;

	while (angle.y > 180) angle.y -= 360.f;
	while (angle.y < -180) angle.y += 360.f;
}

vec3 calcAngle(const vec3& vecSource, const vec3& vecDestination){
	vec3 qAngles;
	vec3 delta = vecSource - vecDestination;
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
	normalise(delta);
	float temp = (delta.x*delta.x) + (delta.y*delta.y) + (delta.z*delta.z);
	return sqrtf(temp);
}

vec3 getBone(const DWORD boneaddy, const int id) {
	vec3 bone;
	bone.x = *(float*)((boneaddy + 0x30 * id + 0x0C));
	bone.y = *(float*)((boneaddy + 0x30 * id + 0x1C));
	bone.z = *(float*)((boneaddy + 0x30 * id + 0x2C));
	return bone;
}

#include <windows.h>
#include <Psapi.h>
#include <algorithm>
#include <string>
using namespace std::string_literals;
DWORD findPatternAA(const char* moduleName, std::string pattern) {
	DWORD startAddress = (DWORD)GetModuleHandleA(moduleName);
	MODULEINFO info;
	GetModuleInformation(GetCurrentProcess(), (HMODULE)startAddress, &info, sizeof(MODULEINFO));
	DWORD endAddress = startAddress + info.SizeOfImage;

	int foundAmount = 0;
	int amountQuestionMarks = std::count(pattern.begin(), pattern.end(), '?');

	for (DWORD i = startAddress; i < endAddress - pattern.length(); i++) {
		for (DWORD j = 0; j < pattern.length(); j++) {
			if (pattern[j] == '?')
				continue;
			if ((BYTE)pattern[j] == *(BYTE*)(i + j))
				foundAmount++;
			if (foundAmount >= pattern.length() - amountQuestionMarks) {
				return i;
			}
		}
		foundAmount = 0;
	}
	return 0;
}

CONST DWORD dwClientState_ViewAngles = 0x4D88;

#define MaxPlayers 20

[[noreturn]] void main() {
	using namespace std::string_literals;

	DWORD dwEntityList = 0x4D0C004;

	DWORD clientDLL = (DWORD)GetModuleHandle(L"client_panorama.dll");

	DWORD clientState = **(DWORD**)(findPatternAA("engine.dll", "\xA1\?\?\?\?\x33\xD2\x6A\x00\x6A\x00\x33\xC9\x89\xB0"s) + 1);
	DWORD localPlayerAddress = ((*(DWORD*)(findPatternAA("client_panorama.dll", "\x8D\x34\x85\?\?\?\?\x89\x15\?\?\?\?\x8B\x41\x08\x8B\x48\x04\x83\xF9\xFF"s) + 3)) + 4);

	vec3* myViewAngle = (vec3*)(clientState + dwClientState_ViewAngles);
	
	while (1) {

		PlayerClass localPlayer = **(PlayerClass**)localPlayerAddress;

		std::this_thread::sleep_for(std::chrono::nanoseconds(1));

		if (GetAsyncKeyState(VK_LBUTTON)) {

			std::pair<std::optional<float>, vec3> closestTarget{ std::nullopt,{0,0,0} };
			bool firstTime = 1;

			for (int i = 1; i < MaxPlayers; i++) {
				PlayerClass* entity = *(PlayerClass**)(clientDLL + dwEntityList + (i) * 0x10);

				if (!entity) continue;
				if (entity->m_iTeamNum == localPlayer.m_iTeamNum) continue;
				if (*(char*)((DWORD)entity + 0xED) == 1) { continue; };// dormant cheak
				if (entity->m_iHealth > 100) { continue; };
				if (entity->m_lifeState != 0) { continue; };
				if (entity->m_iHealth <= 0) { continue; };

				vec3 localHeadPos = localPlayer.m_vecViewOffset + localPlayer.m_vecOrigin;
				vec3 enHeadPos = getBone(entity->m_dwBoneMatrix, 8);

				//std::cout << "myHead: " << localHeadPos << "  en: " << enHeadPos << std::endl;

				vec3 AimAngle = calcAngle(localHeadPos, enHeadPos);

				float distance = getDistance(localPlayer.m_vec3ViewAngle,AimAngle);

				if (distance < closestTarget.first || firstTime) {
					closestTarget.first = distance;
					closestTarget.second = AimAngle;
					firstTime = 0;
				}
			}

			if (closestTarget.first.has_value()) {
				normalise(closestTarget.second);

				vec3 AimPunch = localPlayer.m_aimPunchAngle;
				AimPunch *= 2.f;

				closestTarget.second -= AimPunch;

				normalise(closestTarget.second);
				clampAngle(closestTarget.second);

				*myViewAngle = closestTarget.second;
			}

		}
	}
}

BOOL APIENTRY DllMain(HMODULE hModule,DWORD  ul_reason_for_call,LPVOID lpReserved){
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
		AllocConsole();
		freopen("conin$", "r", stdin);
		freopen("conout$", "w", stdout);
		system("cls");
		printf("Debugging Window:\n");

		std::thread main(main);
		main.detach();
    }
    return TRUE;
}