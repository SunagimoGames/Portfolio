#pragma once

#include <vector>
#include <string>

#include<DirectXMath.h>
using namespace DirectX;

struct Vertex
{
	XMFLOAT3 pos;
	XMFLOAT2 uv;
};

//–@ü‘Î‰‚Ì’¸“_\‘¢‘Ì
struct VertexNormal
{
	XMFLOAT3 pos;
	XMFLOAT2 uv;
	XMFLOAT3 normal;
};

struct MeshData
{
	std::vector<VertexNormal> vertices;
	std::vector<unsigned short> indices;
	std::wstring textureFileName;
};