#include "MyVector.h"
#include <cmath>

#pragma region float3
Vec3::Vec3() :
x(0.f),
y(0.f),
z(0.f)
{
}

Vec3::~Vec3(){
}

Vec3::Vec3(float _x, float _y, float _z) :
x(_x),
y(_y),
z(_z)
{
}

Vec3::Vec3(float point) :
x(point),
y(point),
z(point)
{
}

Vec3 Vec3::operator+(const Vec3& in) const{
	return Vec3(x + in.x, y + in.y, z + in.z);
}
void Vec3::operator+=(const Vec3& in){
	x += in.x;
	y += in.y;
	z += in.z;
}

bool Vec3::operator==(const Vec3& v) const{
	return x == v.x && y == v.y && z == v.z;
}

Vec3 Vec3::operator-(const Vec3& in) const{
	return Vec3(x - in.x, y - in.y, z - in.z);
}
void Vec3::operator-=(const Vec3& in){
	x -= in.x;
	y -= in.y;
	z -= in.z;
}

Vec3 Vec3::operator*(const Vec3& in) const{
	return Vec3(x * in.x, y * in.y, z * in.z);
}
void Vec3::operator*=(const Vec3& in){
	x *= in.x;
	y *= in.y;
	z *= in.z;
}

Vec3 Vec3::operator/(const Vec3& in) const{
	return Vec3(x / in.x, y / in.y, z / in.z);
}
void Vec3::operator/=(const Vec3& in){
	x /= in.x;
	y /= in.y;
	z /= in.z;
}

Vec3 Vec3::operator+(float in) const{
	return Vec3(x + in, y + in, z + in);
}
void Vec3::operator+=(float in){
	x += in;
	y += in;
	z += in;
}

Vec3 Vec3::operator-(float in) const{
	return Vec3(x - in, y - in, z - in);
}
void Vec3::operator-=(float in){
	x -= in;
	y -= in;
	z -= in;
}

Vec3 Vec3::operator*(float in) const{
	return Vec3(x * in, y * in, z * in);
}
void Vec3::operator*=(float in){
	x *= in;
	y *= in;
	z *= in;
}

Vec3 Vec3::operator/(float in) const{
	return Vec3(x / in, y / in, z / in);
}
void Vec3::operator/=(float in){
	x /= in;
	y /= in;
	z /= in;
}

void Vec3::operator=(const Vec3& in){
	x = in.x;
	y = in.y;
	z = in.z;
}
void Vec3::operator=(float in){
	x = in;
	y = in;
	z = in;
}

float Vec3::getLength() const{
	return std::sqrt(x*x + y*y + z*z);
}

Vec3 Vec3::lerp(const Vec3& beg, const Vec3& end, float ratio){
	return beg + (end - beg) * ratio;
}

D3DXVECTOR3* Vec3::getD3DVector(){
	return reinterpret_cast<D3DXVECTOR3*>(this);
}
const D3DXVECTOR3* Vec3::getD3DVector_c() const{
	return reinterpret_cast<const D3DXVECTOR3*>(this);
}

#pragma endregion

#pragma region float2

Vec2::Vec2() :
x(0.f),
y(0.f)
{
}

Vec2::~Vec2(){
}

Vec2::Vec2(float _x, float _y) :
x(_x),
y(_y)
{
}

Vec2::Vec2(float point) :
x(point),
y(point)
{
}

Vec2 Vec2::operator+(const Vec2& in) const{
	return Vec2(x + in.x, y + in.y);
}
void Vec2::operator+=(const Vec2& in){
	x += in.x;
	y += in.y;
}

Vec2 Vec2::operator-(const Vec2& in) const{
	return Vec2(x - in.x, y - in.y);
}
void Vec2::operator-=(const Vec2& in){
	x -= in.x;
	y -= in.y;
}

Vec2 Vec2::operator*(const Vec2& in) const{
	return Vec2(x * in.x, y * in.y);
}
void Vec2::operator*=(const Vec2& in){
	x *= in.x;
	y *= in.y;
}

Vec2 Vec2::operator/(const Vec2& in) const{
	return Vec2(x / in.x, y / in.y);
}
void Vec2::operator/=(const Vec2& in){
	x /= in.x;
	y /= in.y;
}

Vec2 Vec2::operator+(float in) const{
	return Vec2(x + in, y + in);
}
void Vec2::operator+=(float in){
	x += in;
	y += in;
}

Vec2 Vec2::operator-(float in) const{
	return Vec2(x - in, y - in);
}
void Vec2::operator-=(float in){
	x -= in;
	y -= in;
}

Vec2 Vec2::operator*(float in) const{
	return Vec2(x * in, y * in);
}
void Vec2::operator*=(float in){
	x *= in;
	y *= in;
}

Vec2 Vec2::operator/(float in) const{
	return Vec2(x / in, y / in);
}
void Vec2::operator/=(float in){
	x /= in;
	y /= in;
}

void Vec2::operator=(const Vec2& in){
	x = in.x;
	y = in.y;
}
void Vec2::operator=(float in){
	x = in;
	y = in;
}

Vec2 Vec2::lerp(const Vec2& beg, const Vec2& end, float ratio){
	return beg + (end - beg) * ratio;
}

#pragma endregion

#pragma region int3

Vec3I::Vec3I() :x(0), y(0), z(0){
}

Vec3I::Vec3I(int _x, int _y, int _z) : x(_x), y(_y), z(_z){
}

Vec3I Vec3I::operator*(const Vec3I& v) const{
	return Vec3I(x * v.x, y * v.y, z * v.z);
}

Vec3I Vec3I::operator/(const Vec3I& v) const{
	return Vec3I(x / v.x, y / v.y, z / v.z);
}

Vec3I Vec3I::operator-(const Vec3I& v) const{
	return Vec3I(x - v.x, y - v.y, z - v.z);
}

Vec3I Vec3I::operator+(const Vec3I& v) const{
	return Vec3I(x + v.x, y + v.y, z + v.z);
}

void Vec3I::operator*=(const Vec3I& v){
	x *= v.x;
	y *= v.y;
	z *= v.z;
}

void Vec3I::operator/=(const Vec3I& v){
	x /= v.x;
	y /= v.y;
	z /= v.z;
}

void Vec3I::operator-=(const Vec3I& v){
	x -= v.x;
	y -= v.y;
	z -= v.z;
}

void Vec3I::operator+=(const Vec3I& v){
	x += v.x;
	y += v.y;
	z += v.z;
}

Vec3I::Vec3I(int v) :
x(v), y(v), z(v)
{
}

Vec3I Vec3I::operator*(int v) const{
	return Vec3I(x*v, y*v, z*v);
}
Vec3I Vec3I::operator-(int v) const{
	return Vec3I(x - v, y - v, z - v);
}
Vec3I Vec3I::operator+(int v) const{
	return Vec3I(x + v, y + v, z + v);
}
Vec3I Vec3I::operator/(int v) const{
	return Vec3I(x / v, y / v, z / v);
}

bool Vec3I::operator==(const Vec3I& v) const{
	if (x == v.x && y == v.y && z == v.z){
		return true;
	}
	else{
		return false;
	}
}

bool Vec3I::operator!=(const Vec3I& v) const{
	if (x != v.x || y != v.y || z != v.z){
		return true;
	}
	else{
		return false;
	}
}

bool Vec3I::isOutArea(const Vec3I& beg, const Vec3I& end) const{
	// エリアの中に入っているべき。そうでなければ・・・という計算をする形で行う。
	return (x >= beg.x && x <= end.x && y >= beg.y && y <= end.y && z >= beg.z && z <= end.z) ? false : true;
}

Vec3I Vec3I::operator%(int v) const{
	return Vec3I(x%v, y%v, z%v);
}

int Vec3I::distance() const{
	return max(max(abs(x), abs(y)), abs(z));
}

#pragma endregion