// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "PlayerStruct_Vec3.h"

#define M_PI 3.1415927f

void ClampAngle(vec3& angle) {
	if (angle.x > 89.0f) angle.x = 89.f;
	if (angle.x < -89.0f) angle.x = -89.f;

	if (angle.y > 180.f) angle.y = 180.f; 
	if (angle.y < -180.f) angle.y = -180.f; 

	angle.z = 0.0f;
}

void Normalise(vec3& angle) {
	if (angle.x > 89.0f) angle.x -= 180.0f;
	if (angle.x < -89.0f) angle.x += 180.0f;

	while (angle.y > 180) angle.y -= 360.f;
	while (angle.y < -180) angle.y += 360.f;
}

vec3 CalcAngle(const vec3& vecSource, const vec3& vecDestination){
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
	Normalise(delta);
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

DWORD ClientDLL, EngineDLL;

DWORD dwEntityList = 0x4D09F44;
DWORD dwLocalPlayer = 0xCF7A4C;
DWORD dwClientState = 0x590D8C;

DWORD dwClientState_ViewAngles = 0x4D88;

#define MaxPlayers 20

[[noreturn]] void main() {
	ClientDLL = (DWORD)GetModuleHandle(L"client_panorama.dll");
	EngineDLL = (DWORD)GetModuleHandle(L"engine.dll");
	
	DWORD clientState = *(DWORD*)(EngineDLL + dwClientState);
	vec3* myViewAngle = (vec3*)(clientState + dwClientState_ViewAngles);
	
	while (1) {
		std::this_thread::sleep_for(std::chrono::nanoseconds(1));
		
		PlayerClass localPlayer = *(PlayerClass*)*(DWORD*)(ClientDLL + dwLocalPlayer);

		if (GetAsyncKeyState(VK_LBUTTON)) {

			std::pair<std::optional<float>, vec3> closestTarget{ std::nullopt,{0,0,0} };
			bool firstTime = 1;

			for (int i = 1; i < MaxPlayers; i++) {
				PlayerClass* entity = (PlayerClass*)*(DWORD*)(ClientDLL + dwEntityList + (i) * 0x10);

				if (!entity) continue;
				if (entity->m_iTeamNum == localPlayer.m_iTeamNum) continue;
				if (*(char*)((DWORD)entity + 0xED) == 1) { continue; };// dormant cheak
				if (entity->m_iHealth > 100) { continue; };
				if (entity->m_lifeState != 0) { continue; };
				if (entity->m_iHealth <= 0) { continue; };

				vec3 localHeadPos = localPlayer.m_vecViewOffset + localPlayer.m_vecOrigin;
				vec3 enHeadPos = getBone(entity->m_dwBoneMatrix, 8);

				vec3 AimAngle = CalcAngle(localHeadPos, enHeadPos);

				float distance = getDistance(localPlayer.m_vec3ViewAngle,AimAngle);

				if (distance < closestTarget.first || firstTime) {
					closestTarget.first = distance;
					closestTarget.second = AimAngle;
					firstTime = 0;
				}
			}

			if (closestTarget.first.has_value()) {
				Normalise(closestTarget.second);
				ClampAngle(closestTarget.second);
				*myViewAngle = closestTarget.second;
			}

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
		system("cls");
		printf("Debugging Window:\n");

		std::thread main(main);
		main.detach();
    }
    return TRUE;
}

