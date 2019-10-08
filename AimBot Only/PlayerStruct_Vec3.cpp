#include "PlayerStruct_Vec3.h"
#include "pch.h"

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
