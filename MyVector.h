#pragma once
#include <D3D11.h>
#include <D3DX10.h>
#include <D3DX11.h>

class Vec2{
public:
	float x;
	float y;

public:
	static const Vec2& SCREEN_CENTER;
	static const Vec2& SCREEN_SIZE;

	Vec2();
	~Vec2();

	Vec2(float x, float y);
	Vec2(float point);

	// 汎用的に使われるであろう関数群、演算子。
	/////////////////////////////////////////////////////
	Vec2 operator+(const Vec2& in) const;
	void operator+=(const Vec2& in);

	Vec2 operator-(const Vec2& in) const;
	void operator-=(const Vec2& in);

	Vec2 operator*(const Vec2& in) const;
	void operator*=(const Vec2& in);

	Vec2 operator/(const Vec2& in) const;
	void operator/=(const Vec2& in);

	Vec2 operator+(float in) const;
	void operator+=(float in);

	Vec2 operator-(float in) const;
	void operator-=(float in);

	Vec2 operator*(float in) const;
	void operator*=(float in);

	Vec2 operator/(float in) const;
	void operator/=(float in);

	void operator=(const Vec2& in);
	void operator=(float in);
	/////////////////////////////////////////////////////

	/////////////////////////////////////////////////////
	static Vec2 lerp(const Vec2& beg, const Vec2& end, float ratio);
	/////////////////////////////////////////////////////
};

class Vec3{
public:
	float x;
	float y;
	float z;

public:
	Vec3();
	~Vec3();

	Vec3(float x, float y, float z);
	Vec3(float point);

	// 汎用的に使われるであろう関数群、演算子。
	/////////////////////////////////////////////////////
	Vec3 operator+(const Vec3& in) const;
	void operator+=(const Vec3& in);

	Vec3 operator-(const Vec3& in) const;
	void operator-=(const Vec3& in);

	Vec3 operator*(const Vec3& in) const;
	void operator*=(const Vec3& in);

	Vec3 operator/(const Vec3& in) const;
	void operator/=(const Vec3& in);

	Vec3 operator+(float in) const;
	void operator+=(float in);

	Vec3 operator-(float in) const;
	void operator-=(float in);

	Vec3 operator*(float in) const;
	void operator*=(float in);

	Vec3 operator/(float in) const;
	void operator/=(float in);

	void operator=(const Vec3& in);
	void operator=(float in);

	bool operator==(const Vec3& v) const;

	float getLength() const;
	/////////////////////////////////////////////////////

	/////////////////////////////////////////////////////
	static Vec3 lerp(const Vec3& beg, const Vec3& end, float ratio);
	/////////////////////////////////////////////////////

	D3DXVECTOR3* getD3DVector();
	const D3DXVECTOR3* getD3DVector_c() const;
};

class Vec3I{
public:
	Vec3I();
	Vec3I(int x, int y, int z);
	Vec3I(int v);
	int x;
	int y;
	int z;
	void operator+=(const Vec3I& v);
	void operator-=(const Vec3I& v);
	void operator*=(const Vec3I& v);
	void operator/=(const Vec3I& v);
	Vec3I operator*(const Vec3I& v) const;
	Vec3I operator-(const Vec3I& v) const;
	Vec3I operator+(const Vec3I& v) const;
	Vec3I operator/(const Vec3I& v) const;
	Vec3I operator*(int v) const;
	Vec3I operator-(int v) const;
	Vec3I operator+(int v) const;
	Vec3I operator/(int v) const;
	bool operator==(const Vec3I& v) const;
	bool operator!=(const Vec3I& v) const;
	Vec3I operator %(int v) const;

	bool isOutArea(const Vec3I& beg, const Vec3I& end) const;
	int distance() const;
};