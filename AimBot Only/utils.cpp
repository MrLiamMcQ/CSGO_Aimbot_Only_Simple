#include "utils.h"
#include "pch.h"

DWORD findPattern(const char* moduleName, std::string pattern) {
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

vec3 operator + (vec3 const & lhs, vec3 const & rhs) {
	return vec3{ lhs.x + rhs.x,lhs.y + rhs.y,lhs.z + rhs.z };
}
vec3 operator - (vec3 const & lhs, vec3 const & rhs) {
	return vec3{ lhs.x - rhs.x,lhs.y - rhs.y,lhs.z - rhs.z };
}
std::ostream& operator<<(std::ostream& os, vec3 const& pos) {
	os << "xyz: " << pos.x << "," << pos.y << "," << pos.z;
	return os;
}
vec3& vec3::operator *= (float number) {
	this->x *= number;
	this->y *= number;
	this->z *= number;
	return *this;
}

vec3 & vec3::operator-=(vec3 const & rhs)
{
	this->x -= rhs.x;
	this->y -= rhs.y;
	this->z -= rhs.z;
	return *this;
}
