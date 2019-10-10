// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "utils.h"

#define M_PI 3.1415927f
#define MaxPlayers 20
CONST DWORD dwClientState_ViewAngles = 0x4D88;

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
	angle.y = std::remainderf(angle.y, 360.0f);
	//while (angle.y > 180) angle.y -= 360.f;
	//while (angle.y < -180) angle.y += 360.f;
}

vec3 calcAngle(const vec3& source, const vec3& destination){
	vec3 retAngle;
	vec3 delta = source - destination;
	float hyp = sqrtf(delta.x * delta.x + delta.y * delta.y);
	retAngle.x = (float)(atan(delta.z / hyp) * (180.0f / M_PI));
	retAngle.y = (float)(atan(delta.y / delta.x) * (180.0f / M_PI));
	retAngle.z = 0.f;
	if (delta.x >= 0.f)
		retAngle.y += 180.f;

	return retAngle;
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

[[noreturn]] void main() {

	using namespace std::string_literals; // to stop null termination of strings

	DWORD entityListAddress = *(DWORD*)(findPattern("client_panorama.dll", "\xBB\?\?\?\?\x83\xFF\x01\x0F\x8C\?\?\?\?\x3B\xF8"s) + 1);
	DWORD clientState = **(DWORD**)(findPattern("engine.dll", "\xA1\?\?\?\?\x33\xD2\x6A\x00\x6A\x00\x33\xC9\x89\xB0"s) + 1);
	DWORD localPlayerAddress = ((*(DWORD*)(findPattern("client_panorama.dll", "\x8D\x34\x85\?\?\?\?\x89\x15\?\?\?\?\x8B\x41\x08\x8B\x48\x04\x83\xF9\xFF"s) + 3)) + 4);

	vec3* myViewAngle = (vec3*)(clientState + dwClientState_ViewAngles);
	PlayerClass* localPlayer = *(PlayerClass**)localPlayerAddress;
	
	while (1) {

		std::this_thread::sleep_for(std::chrono::nanoseconds(1));

		if (GetAsyncKeyState(VK_LBUTTON)) {

			std::pair<std::optional<float>, vec3> closestTarget{ std::nullopt,{0,0,0} };
			bool firstTime = 1;

			for (int i = 1; i < MaxPlayers; i++) {
				PlayerClass* entity = *(PlayerClass**)(entityListAddress + (i) * 0x10);

				if (!entity) continue;
				if (entity->m_iTeamNum == localPlayer->m_iTeamNum) continue;
				if (*(char*)((DWORD)entity + 0xED) == 1) { continue; };// dormant cheak
				if (entity->m_iHealth > 100) { continue; };
				if (entity->m_lifeState != 0) { continue; };
				if (entity->m_iHealth <= 0) { continue; };

				vec3 localHeadPos = localPlayer->m_vecViewOffset + localPlayer->m_vecOrigin;
				vec3 enHeadPos = getBone(entity->m_dwBoneMatrix, 8);

				vec3 AimAngle = calcAngle(localHeadPos, enHeadPos);

				float distance = getDistance(localPlayer->m_vec3ViewAngle,AimAngle);

				if (distance < closestTarget.first || firstTime) {
					closestTarget.first = distance;
					closestTarget.second = AimAngle;
					firstTime = 0;
				}
			}

			if (closestTarget.first.has_value()) {
				normalise(closestTarget.second);

				vec3 AimPunch = localPlayer->m_aimPunchAngle;
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