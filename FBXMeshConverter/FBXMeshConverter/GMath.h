#pragma once

#include<DirectXMath.h>
using namespace DirectX;

XMFLOAT2 UnitVecAngle(float theta);

float Length(const XMFLOAT2& a);
float LengthSq(const XMFLOAT2& a);
XMFLOAT2 Normalize(const XMFLOAT2& a);

float Length(const XMFLOAT3& a);
float LengthSq(const XMFLOAT3& a);
XMFLOAT3 Normalize(const XMFLOAT3& a);

float Dot(const XMFLOAT2& a, const XMFLOAT2& b);
float Cross(const XMFLOAT2& a, const XMFLOAT2& b);

float Dot(const XMFLOAT3& a, const XMFLOAT3& b);
XMFLOAT3 Cross(const XMFLOAT3& a, const XMFLOAT3& b);

XMFLOAT2 operator+(const XMFLOAT2& a, const XMFLOAT2& b);
XMFLOAT2 operator-(const XMFLOAT2& a, const XMFLOAT2& b);
XMFLOAT2 operator*(const XMFLOAT2& a, const float b);
XMFLOAT2 operator*(const float a, const XMFLOAT2& b);
void operator+=(XMFLOAT2& a, const XMFLOAT2& b);
void operator-=(XMFLOAT2& a, const XMFLOAT2& b);
void operator*=(XMFLOAT2& a, float b);

XMFLOAT3 operator+(const XMFLOAT3& a, const XMFLOAT3& b);
XMFLOAT3 operator-(const XMFLOAT3& a, const XMFLOAT3& b);
XMFLOAT3 operator*(const XMFLOAT3& a, const float b);
XMFLOAT3 operator*(const float a, const XMFLOAT3& b);
XMFLOAT3 operator*(const XMFLOAT4& vec, const XMMATRIX& mat);
void operator+=(XMFLOAT3& a, const XMFLOAT3& b);
void operator-=(XMFLOAT3& a, const XMFLOAT3& b);
void operator*=(XMFLOAT3& a, float b);

const XMFLOAT2 UnitVec2X(1.0f, 0.0f);
const XMFLOAT3 UnitVec3X(1.0f, 0.0f, 0.0f);
const XMFLOAT2 UnitVec2Y(0.0f, 1.0f);
const XMFLOAT3 UnitVec3Y(0.0f, 1.0f, 0.0f);
const XMFLOAT3 UnitVec3Z(0.0f, 0.0f, 1.0f);
const XMFLOAT2 ZeroVec2(0.0f, 0.0f);
const XMFLOAT3 ZeroVec3(0.0f, 0.0f, 0.0f);

struct Segment
{
	XMFLOAT2 start;
	XMFLOAT2 end;

	Segment() :start(ZeroVec2), end(UnitVec2X) {}
	Segment(const XMFLOAT2& a, const XMFLOAT2& b) :start(a), end(b) {}
};

struct Circle
{
	XMFLOAT2 pos;
	float r;

	Circle() :pos(ZeroVec2), r(1.0f) {}
	Circle(const XMFLOAT2& p, float rad) : pos(p), r(rad) {}
};

struct Rect
{
	float left, top, right, bottom;

	Rect() :left(0.0f), top(0.0f), right(0.0f), bottom(0.0f) {}
	Rect(float lt, float tp, float rt, float bm) :left(lt), top(tp), right(rt), bottom(bm) {}
};