#pragma once

#include <cmath>
#include <array>
#include <iomanip>
#include <iostream>
#include <stdexcept>

//右手系の列優先

const float Pi = 3.1415926535f;

inline float DegToRad(float deg) { return deg * Pi / 180.0f; }
inline float RadToDeg(float rad) { return rad * 180.0f / Pi; }

class Vec2
{
public:
	Vec2(const Vec2& copyRef):x(copyRef.GetX()), y(copyRef.GetY()) {}

	Vec2():x(0.0f), y(0.0f) {}

	Vec2(float numX, float numY):x(numX), y(numY) {}

	Vec2(float num):x(num), y(num) {}

/***********************************************************************************
ゲッターセッターアジャスター
***********************************************************************************/

	inline float GetX() const { return x; }
	inline void  SetX(float numX) { x = numX; }
	inline void  AdjustX(float numX) { x += numX; }
	inline float GetY() const { return y; }
	inline void  SetY(float numY) { y = numY; }
	inline void  AdjustY(float numY) { y += numY; }

	static Vec2 Zero() { return Vec2(0.0f, 0.0f); }

/***********************************************************************************
関数
***********************************************************************************/

	static float Length(float x, float y) { return sqrtf(x * x + y * y); }
	float Length() { return sqrtf(x * x + y * y); }
	static float Dot(const Vec2& a, const Vec2& b) { return a.GetX() * b.GetX() + a.GetY() * b.GetY(); }

	static Vec2 Normalize(float x, float y)
	{
		float len = Length(x, y);
		if (len == 0) return Vec2();

		x /= len;
		y /= len;
		return Vec2(x, y);
	}

	Vec2 Normalize()
	{
		float len = Length();
		if (len == 0) return Vec2();

		x /= len;
		y /= len;
		return Vec2(x, y);
	}

	Vec2 Opposite()
	{
		x = -x;
		y = -y;
		return *this;
	}

/***********************************************************************************
演算子(代入演算子はメンバ内で実装し,メンバ外の算術演算子(二項)で使用しコードの重複を避ける)
***********************************************************************************/

	inline Vec2& operator= (const Vec2& rhs)
	{
		x = rhs.GetX();
		y = rhs.GetY();
		return *this;
	}

	inline Vec2& operator+= (const Vec2& rhs)
	{
		x += rhs.GetX();
		y += rhs.GetY();
		return *this;
	}
	inline Vec2& operator+=(const float rhs)
	{
		x += rhs;
		y += rhs;
		return *this;
	}

	inline Vec2& operator-= (const Vec2& rhs)
	{
		x -= rhs.GetX();
		y -= rhs.GetY();
		return *this;
	}
	inline Vec2& operator-=(const float rhs)
	{
		x -= rhs;
		y -= rhs;
		return *this;
	}

	inline Vec2& operator*=(const Vec2& rhs)
	{
		x *= rhs.GetX();
		y *= rhs.GetY();
		return *this;
	}
	inline Vec2& operator*=(const float rhs)
	{
		x *= rhs;
		y *= rhs;
		return *this;
	}

private:
	float x, y;
};
//演算子
//inlineは各コンパイル単位で独立して扱われるためリンカが重複とみなすことはない
inline const Vec2 operator+(const Vec2& lhs, const Vec2& rhs) { return Vec2(lhs.GetX(), lhs.GetY()) += rhs; }
inline const Vec2 operator+(const Vec2& lhs, const float rhs)   { return Vec2(lhs.GetX(), lhs.GetY()) += rhs; }
inline const Vec2 operator+(const float lhs, const Vec2& rhs)   { return Vec2(lhs, lhs) += rhs; }

inline const Vec2 operator-(const Vec2& lhs, const Vec2& rhs) { return Vec2(lhs.GetX(), lhs.GetY()) -= rhs; }
inline const Vec2 operator-(const Vec2& lhs, const float rhs)   { return Vec2(lhs.GetX(), lhs.GetY()) -= rhs; }
inline const Vec2 operator-(const float lhs, const Vec2& rhs)   { return Vec2(lhs, lhs) -= rhs; }

inline const Vec2 operator*(const Vec2& lhs, const Vec2& rhs) { return Vec2(lhs.GetX(), lhs.GetY()) *= rhs; }
inline const Vec2 operator*(const Vec2& lhs, const float rhs) { return Vec2(lhs.GetX(), lhs.GetY()) *= rhs; }
inline const Vec2 operator*(const float lhs, const Vec2& rhs) { return Vec2(lhs, lhs) *= rhs; }

class Vec3
{
public:
	Vec3(const Vec3& copyRef):x(copyRef.GetX()), y(copyRef.GetY()),z(copyRef.GetZ()) {}

	Vec3() :x(0.0f), y(0.0f), z(0.0f) {}

	Vec3(float numX, float numY, float numZ):x(numX), y(numY), z(numZ) {}

	Vec3(float num):x(num), y(num), z(num) {}

/***********************************************************************************
ゲッターセッターアジャスター
***********************************************************************************/

	float GetX() const { return x; }
	void  SetX(float numX) { x = numX; }
	void  AdjustX(float numX) { x += numX; }
	float GetY() const { return y; }
	void  SetY(float numY) { y = numY; }
	void  AdjustY(float numY) { y += numY; }
	float GetZ() const { return z; }
	void  SetZ(float numZ) { z = numZ; }
	void  AdjustZ(float numZ) { z += numZ; }
	
	static Vec3 Zero() { return Vec3(0.0f, 0.0f, 0.0f); }
	static Vec3 Up() { return Vec3(0.0f, 1.0f, 0.0f); }
	static Vec3 Right() { return Vec3(1.0f, 0.0f, 0.0f); }
	static Vec3 Forward() { return Vec3(0.0f, 0.0f, 1.0f); }

/***********************************************************************************
関数
***********************************************************************************/

	float Length() { return sqrtf(x * x + y * y + z * z); }
	static float Length(float x, float y, float z) { return sqrtf(x * x + y * y + z * z); }
	static float Dot(const Vec3& a, const Vec3& b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
	static Vec3 Cross(const Vec3& a, const Vec3& b)
	{
		Vec3 tmp;
		tmp.x = a.y * b.z - a.z * b.y;
		tmp.y = a.z * b.x - a.x * b.z;
		tmp.z = a.x * b.y - a.y * b.x;
		return tmp;
	}

	static Vec3 Normalize(float x, float y, float z)
	{
		float len = Length(x, y, z);
		if (len == 0) return Vec3();

		x /= len;
		y /= len;
		z /= len;
		return Vec3(x, y, z);
	}

	Vec3 Normalize()
	{
		float len = Length();
		if (len == 0) return Vec3();

		x /= len;
		y /= len;
		z /= len;
		return *this;
	}

	Vec3 Opposite()
	{
		x = -x;
		y = -y;
		z = -z;
		return *this;
	}
	

/***********************************************************************************
演算子(代入演算子はメンバ内で実装し,メンバ外の算術演算子(二項)で使用しコードの重複を避ける)
***********************************************************************************/

	inline Vec3& operator= (const Vec3& rhs)
	{
		x = rhs.GetX();
		y = rhs.GetY();
		z = rhs.GetZ();
		return *this;
	}

	inline Vec3& operator+= (const Vec3& rhs)
	{
		x += rhs.GetX();
		y += rhs.GetY();
		z += rhs.GetZ();
		return *this;
	}
	inline Vec3& operator+= (const float& rhs)
	{
		x += rhs;
		y += rhs;
		z += rhs;
		return *this;
	}

	inline Vec3& operator-= (const Vec3& rhs)
	{
		x -= rhs.GetX();
		y -= rhs.GetY();
		z -= rhs.GetZ();
		return *this;
	}
	inline Vec3& operator-= (const float& rhs)
	{
		x -= rhs;
		y -= rhs;
		z -= rhs;
		return *this;
	}

	inline Vec3& operator*= (const Vec3& rhs)
	{
		x *= rhs.GetX();
		y *= rhs.GetY();
		z *= rhs.GetZ();
		return *this;
	}
	inline Vec3& operator*= (const float& rhs)
	{
		x *= rhs;
		y *= rhs;
		z *= rhs;
		return *this;
	}

private:
	float x, y, z;
};
//演算子
inline const Vec3 operator+(const Vec3& lhs, const Vec3& rhs) { return Vec3(lhs.GetX(), lhs.GetY(), lhs.GetZ()) += rhs; }
inline const Vec3 operator+(const Vec3& lhs, const float rhs) { return Vec3(lhs.GetX(), lhs.GetY(), lhs.GetZ()) += rhs; }
inline const Vec3 operator+(const float lhs, const Vec3& rhs) { return Vec3(lhs, lhs, lhs) += rhs; }

inline const Vec3 operator-(const Vec3& lhs, const Vec3& rhs) { return Vec3(lhs.GetX(), lhs.GetY(), lhs.GetZ()) -= rhs; }
inline const Vec3 operator-(const Vec3& lhs, const float rhs) { return Vec3(lhs.GetX(), lhs.GetY(), lhs.GetZ()) -= rhs; }
inline const Vec3 operator-(const float lhs, const Vec3& rhs) { return Vec3(lhs, lhs, lhs) -= rhs; }

inline const Vec3 operator*(const Vec3& lhs, const Vec3& rhs) { return Vec3(lhs.GetX(), lhs.GetY(), lhs.GetZ()) *= rhs; }
inline const Vec3 operator*(const Vec3& lhs, const float rhs) { return Vec3(lhs.GetX(), lhs.GetY(), lhs.GetZ()) *= rhs; }
inline const Vec3 operator*(const float lhs, const Vec3& rhs) { return Vec3(lhs, lhs, lhs) *= rhs; }

class Quaternion
{
public:
	Quaternion():x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}

	Quaternion(float numX, float numY, float numZ, float numW):x(numX), y(numY), z(numZ), w(numW) {}

private:
	float x, y, z, w;
};


enum class MatKind
{
	Zero,
	Identity,
	Augment,
	Translation,
	Rot_X,
	Rot_Y,
	Rot_Z
};

class Mat4
{
public:
	Mat4() :elements{} {}

	Mat4(const Mat4& copyRef) : elements(copyRef.elements) {}

	Mat4(const std::array<std::array<float, 4>, 4>& elementsVar)
	{
		for (int i = 0;i < 4;i++)
		{
			for (int j = 0;j < 4;j++)
			{
				elements[i][j] = elementsVar[i][j];
			}
		}
	}

	Mat4(MatKind kind)
	{
		if (kind == MatKind::Zero || kind == MatKind::Identity)
		{
			for (int i = 0;i < 4;i++)
			{
				for (int j = 0;j < 4;j++)
				{
					elements[i][j] = 0;
				}
			}

			if (kind == MatKind::Identity)
			{
				for (int i = 0;i < 4;i++)
				{
					elements[i][i] = 1;
				}
			}
		}
		else throw std::runtime_error("Failed to Create Matrix4*4: Invalid Construct Parameter");
	}

	Mat4(MatKind kind, float x, float y, float z)
	{
		if (kind == MatKind::Translation)
		{
			for (int i = 0;i < 3;i++)
			{
				for (int j = 0;j < 4;j++)
				{
					float num = (i == j) ? 1.0f : 0.0f;
					elements[i][j] = num;
				}
			}
			elements[0][3] = x;
			elements[1][3] = y;
			elements[2][3] = z;
			elements[3][3] = 1.0f;
		}
		else throw std::runtime_error("Failed to Create Matrix4*4: Invalid Construct Parameter");
	}

	Mat4(MatKind kind, float num)
	{
		if (kind == MatKind::Augment)
		{
			for (int i = 0;i < 4;i++)
			{
				for (int j = 0;j < 4;j++)
				{
					elements[i][j] = (i == j) ? num : 0.0f;
				}
			}
			elements[3][3] = 1.0f;
		}
		else if (kind == MatKind::Rot_X || kind == MatKind::Rot_Y || kind == MatKind::Rot_Z)
		{
			for (int i = 0;i < 4;i++)
			{
				for (int j = 0;j < 4;j++)
				{
					elements[i][j] = (i == j) ? 1.0f : 0.0f;
				}
			}

			//引数numをラジアンとして用いる
			if (kind == MatKind::Rot_X)
			{
				elements[1][1] = cos(num);
				elements[2][1] = sin(num);
				elements[1][2] = -sin(num);
				elements[2][2] = cos(num);
			}
			else if (kind == MatKind::Rot_Y)
			{
				elements[0][0] = cos(num);
				elements[2][0] = -sin(num);
				elements[0][2] = sin(num);
				elements[2][2] = cos(num);
			}
			else if (kind == MatKind::Rot_Z)
			{
				elements[0][0] = cos(num);
				elements[1][0] = sin(num);
				elements[0][1] = -sin(num);
				elements[1][1] = cos(num);
			}
		}
		else throw std::runtime_error("Failed to Create Matrix4*4: Invalid Construct Parameter");
	}

/***********************************************************************************
ゲッターセッターアジャスター
***********************************************************************************/

	const float* GetPtr() { return static_cast<const float*>(&elements[0][0]); }
	float GetElement(int i, int j) const { return elements[i][j]; }
	void  SetElement(int i, int j, float num) { elements[i][j] = num; }
	void  AdjustElement(int i, int j, float num) { elements[i][j] += num; }

/***********************************************************************************
関数
***********************************************************************************/

	static void PrintMat(const Mat4& mat)
	{
		std::cout << "(列優先)mat4 = \n";
		std::cout << std::fixed; //浮動小数点を固定小数点表記で入出力指示
		for (int i = 0; i < 4;i++)
		{
			for (int j = 0;j < 4;j++)
			{
				char space = (j == 3) ? '\n' : ' ';
				std::cout << std::setprecision(3) << mat.elements[i][j] << space;
			}
		}
		std::cout << std::defaultfloat;
	}

	/// <summary>引数ベクトルを元にビュー行列を返す<para></para>
	/// 引数は単位ベクトルでなくてもよい</summary>
	/// <param name="eye">カメラ座標</param>
	/// <param name="target">注視点座標</param>
	/// <param name="worldUp">ワールド座標系上方向単位ベクトル</param>
	static Mat4 View(const Vec3& eye, const Vec3& target, const Vec3& worldUp)
	{
	//注視点方向,カメラ右手方向,カメラ上方向の単位ベクトルを作成
		Vec3 tempZ = target - eye;
		const Vec3 f = tempZ.Normalize(); //forward
		const Vec3 r = Vec3::Cross(worldUp, f).Normalize(); //right
		const Vec3 u = Vec3::Cross(f, r); //up
		
	//ビュー行列の計算
		Mat4 viewMat = Mat4(MatKind::Zero);
		viewMat[0][0] = r.GetX();  viewMat[0][1] = r.GetY();  viewMat[0][2] = r.GetZ();  viewMat[0][3] = Vec3::Dot(r, eye);
		viewMat[1][0] = u.GetX();  viewMat[1][1] = u.GetY();  viewMat[1][2] = u.GetZ();  viewMat[1][3] = Vec3::Dot(u, eye);
		viewMat[2][0] = f.GetX();  viewMat[2][1] = f.GetY();  viewMat[2][2] = f.GetZ();  viewMat[2][3] = Vec3::Dot(f, eye);
		viewMat[3][0] = 0.0f;	   viewMat[3][1] = 0.0f;	  viewMat[3][2] = 0.0f;		 viewMat[3][3] = 1.0f;

		return viewMat;
	}

	static Mat4 Proj(const float fovY, const float aspect, const float nearZ, const float farZ)
	{
		const float cot = 1.0f / tan(fovY / 2.0f);

		Mat4 projInfo = Mat4(MatKind::Zero);
		projInfo[0][0] = cot / aspect;
		projInfo[1][1] = -cot;
		projInfo[2][2] = (nearZ + farZ) / (nearZ - farZ);
		projInfo[2][3] = -1.0f;
		projInfo[3][2] = (2.0f * farZ * nearZ) / (nearZ - farZ);
		
		return projInfo;
	}

/***********************************************************************************
演算子
***********************************************************************************/

	//constオブジェクト用(読み取り専用)
	const std::array<float, 4>& operator[](int index) const { return elements[index]; }

	//非constオブジェクト向け(読み書き可能)
	std::array<float, 4>& operator[](int index) { return elements[index]; }	

	inline Mat4 operator*(const Mat4& rhs) const
	{
		Mat4 result;
		for (int i = 0;i < 4;i++)
		{
			for (int j = 0;j < 4;j++)
			{
				float value = 0.0f;
				for (int k = 0;k < 4;k++)
				{
					value += this->GetElement(i, k) * rhs.GetElement(k, j);
				}
				result.SetElement(i, j, value);
			}
		}
		return result;
	}

private:
	std::array<std::array<float, 4>, 4> elements;
};