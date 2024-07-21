#include "GMath.h"
#include <cmath>

XMFLOAT2 operator+(const XMFLOAT2& a, const XMFLOAT2& b) 
{
	return XMFLOAT2(a.x + b.x, a.y + b.y); 
}
XMFLOAT2 operator-(const XMFLOAT2& a, const XMFLOAT2& b) 
{
	return XMFLOAT2(a.x - b.x, a.y - b.y); 
}
XMFLOAT2 operator*(const XMFLOAT2& a, const float b) 
{
	return XMFLOAT2(a.x * b, a.y * b); 
}
XMFLOAT2 operator*(const float a, const XMFLOAT2& b) 
{	
	return XMFLOAT2(a * b.x, a * b.y);
}
void operator+=(XMFLOAT2& a, const XMFLOAT2& b) 
{
	a.x += b.x;
	a.y += b.y;
}
void operator-=(XMFLOAT2& a, const XMFLOAT2& b)
{
	a.x -= b.x;
	a.y -= b.y;
}
void operator*=(XMFLOAT2& a, float b)
{
	a.x *= b;
	a.y *= b;
}

XMFLOAT3 operator+(const XMFLOAT3& a, const XMFLOAT3& b)
{
	return XMFLOAT3(a.x + b.x, a.y + b.y, a.z + b.z);
}
XMFLOAT3 operator-(const XMFLOAT3& a, const XMFLOAT3& b)
{
	return XMFLOAT3(a.x - b.x, a.y - b.y, a.z - b.z);
}
XMFLOAT3 operator*(const XMFLOAT3& a, const float b)
{
	return XMFLOAT3(a.x * b, a.y * b, a.z * b);
}
XMFLOAT3 operator*(const float a, const XMFLOAT3& b)
{
	return XMFLOAT3(b.x * a, b.y * a, b.z * a);
}
XMFLOAT3 operator*(const XMFLOAT4& vec, const XMMATRIX& mat)
{
	float v[4] = { vec.x,vec.y,vec.z,vec.w };
	float output[3]{};

	for (int i = 0;i < 3;i++)
	{
		output[i] = 0.0f;
		for (int j = 0;j < 4;j++)
		{
			output[i] += v[j] * mat.r[j].m128_f32[i];
		}
	}

	return XMFLOAT3(output[0], output[1], output[2]);
}
void operator+=(XMFLOAT3& a, const XMFLOAT3& b)
{
	a.x += b.x;
	a.y += b.y;
	a.z += b.z;
}
void operator-=(XMFLOAT3& a, const XMFLOAT3& b)
{
	a.x -= b.x;
	a.y -= b.y;
	a.z -= b.z;
}
void operator*=(XMFLOAT3& a, float b)
{
	a.x *= b;
	a.y *= b;
	a.z *= b;
}

XMFLOAT2 UnitVecAngle(float theta) { return XMFLOAT2(std::cos(theta), std::sin(theta)); }

float Dot(const XMFLOAT2& a, const XMFLOAT2& b)
{
	return a.x * b.x + a.y * b.y;
}
float Cross(const XMFLOAT2& a, const XMFLOAT2& b)
{
	return a.x * b.y - a.y * b.x;
}

float Dot(const XMFLOAT3& a, const XMFLOAT3& b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z;
}
XMFLOAT3 Cross(const XMFLOAT3& a, const XMFLOAT3& b)
{
	return XMFLOAT3
	(
		a.y * b.z - a.z * b.y,
		a.z * b.x - a.x * b.z,
		a.x * b.y - a.y * b.x
	);
}

float Length(const XMFLOAT2& a) 
{
	return std::sqrt(a.x * a.x + a.y * a.y); 
}
float LengthSq(const XMFLOAT2& a) 
{
	return a.x * a.x + a.y * a.y; 
}
XMFLOAT2 Normalize(const XMFLOAT2& a)
{
	float len = Length(a);
	if (len < FLT_EPSILON) return XMFLOAT2(0.0f, 0.0f);

	float inLen = 1.0f / len;
	return XMFLOAT2(a.x * inLen, a.y * inLen);
}

float Length(const XMFLOAT3& a)
{
	return std::sqrt(a.x * a.x + a.y * a.y + a.z * a.z);
}
float LengthSq(const XMFLOAT3& a)
{
	return a.x * a.x + a.y * a.y + a.z * a.z;
}
XMFLOAT3 Normalize(const XMFLOAT3& a)
{
	float len = Length(a);
	if (std::fabs(len) < FLT_EPSILON) return ZeroVec3;

	XMFLOAT3 b = a;
	float c = 1.0f / len;
	return a * c;
}