#include <iostream>
#include <string>
#include <unordered_map>

#include <fbxsdk.h>
using namespace fbxsdk;
#pragma comment(lib,"libfbxsdk-md.lib")
#pragma comment(lib,"libxml2-md.lib")
#pragma comment(lib,"zlib-md.lib")

#include "GMath.h"
#include "MeshData.h"
#include <cassert>
#include <fstream>

const char* SRC_FILEPATH = "src\\";
const char* DST_FILEPATH = "dst\\";

/***************************************************************************
プロトタイプ宣言
***************************************************************************/

/// <summary>
/// シーンにあるノードを再帰的に巡回しメッシュ情報を持つノードを、
/// ノード名をキーとして連想配列に保存する<para></para>
/// なお、ノード名の重複はないという前提がある
/// </summary>
void GetMeshList(FbxNode* node, std::unordered_map<std::string, FbxNode*>& meshList);

/// <summary>
/// meshListから頂点,インデックス,テクスチャファイル名を取り出しmeshDataに保存
/// </summary>
bool ExtractMeshData(std::unordered_map<std::string, FbxNode*>& meshList,
	std::vector<MeshData>& meshData);

/// <summary>
/// テクスチャのファイルパスを取得<para></para>
/// 同一メッシュに複数のテクスチャが設定されているものには未対応
/// </summary>
const char* GetTextureFilePath(FbxMesh* mesh, FbxNode* node);

/// <summary>
/// ファイルパスのディレクトリの区切りまでを省いたファイル名を抽出しfileNameに保存
/// </summary>
void ParseFileName(const char* filePath, std::string& fileName);

bool IsEqualVector2D(XMFLOAT2 v1, XMFLOAT2 v2);

bool IsEqualVector3D(XMFLOAT3 v1, XMFLOAT3 v2);

/// <summary>
///ワイド文字列への変換関数
/// </summary>
void ConvertString(std::string& src, std::wstring& dst);

bool SaveMeshData(std::vector<MeshData>& meshData, std::string& saveFileName,
	float scale = 1.0f, float rotX = 0.0f, float rotY = 0.0f, float rotZ = 0.0f);

/***************************************************************************
定義
***************************************************************************/

int main()
{
	std::cout << "FBXMeshConverter" << std::endl;

	//読み込みFBXファイル名を取得
	std::string srcFileName;
	std::cout << "Input 3DModel FileName(with extension) = ";
	std::cin >> srcFileName;
	srcFileName = SRC_FILEPATH + srcFileName;

	//出力FBXファイル名を取得
	std::string dstFileName;
	std::cout << "Output fbfix fileName = ";
	std::cin >> dstFileName;
	dstFileName += ".fbfix";

	//FBXライブラリの準備
	FbxManager*	   manager   = FbxManager::Create();
	FbxIOSettings* ioSetting = FbxIOSettings::Create(manager, IOSROOT);
	FbxImporter*   importer  = FbxImporter::Create(manager, "");
	if (!importer->Initialize(srcFileName.c_str(), -1, manager->GetIOSettings()))
	{
		std::cout << "ファイル読み込み失敗: " << srcFileName.c_str() << std::endl;
		importer->Destroy();
		ioSetting->Destroy();
		manager->Destroy();
		return 1;
	}

	//シーン取得
	FbxScene* scene = FbxScene::Create(manager, "scene");
	importer->Import(scene);

	//トポロジー整形(三角形化)
	FbxGeometryConverter converter(manager);
	converter.Triangulate(scene, true);

	//ノードデータ解析,メッシュリスト取得
	std::unordered_map<std::string, FbxNode*> meshList;
	GetMeshList(scene->GetRootNode(), meshList);

	//fbfix用データの取得(頂点,インデックス情報、テクスチャファイル名)
	std::vector<MeshData> meshData;
	if (!ExtractMeshData(meshList, meshData))
	{
		std::cout << "MeshData取得失敗" << std::endl;
		scene->Destroy();
		importer->Destroy();
		ioSetting->Destroy();
		manager->Destroy();
		return 1;
	}

	//fbfixファイルの保存
	float scale, rotX, rotY, rotZ;
	std::cout << "Scale: ";
	std::cin >> scale;
	std::cout << "RotX: ";
	std::cin >> rotX;
	std::cout << "RotY: ";
	std::cin >> rotY;
	std::cout << "RotZ: ";
	std::cin >> rotZ;

	if (!SaveMeshData(meshData, dstFileName, scale, rotX, rotY, rotZ))
	{
		std::cout << dstFileName << "の保存に失敗" << std::endl;
		scene->Destroy();
		importer->Destroy();
		ioSetting->Destroy();
		manager->Destroy();
		return 1;
	}

	importer->Destroy();
	ioSetting->Destroy();
	manager->Destroy();

	return 0;
}

void GetMeshList(FbxNode* node, std::unordered_map<std::string, FbxNode*>& meshList)
{
	if (node == nullptr) return;

	for (int i = 0;i < node->GetNodeAttributeCount();i++)
	{
		FbxNodeAttribute* attr = node->GetNodeAttributeByIndex(i);
		if (attr->GetAttributeType() == FbxNodeAttribute::eMesh)
		{
			meshList[node->GetName()] = node;
		}
	}

	for (int i = 0;i < node->GetChildCount();i++)
	{
		GetMeshList(node->GetChild(i), meshList);
	}
}

bool ExtractMeshData(std::unordered_map<std::string, FbxNode*>& meshList,
	std::vector<MeshData>& meshData)
{
	std::cout << "MeshData Num = " << meshList.size() << std::endl;
	if (meshList.size() == 0) return false;
	meshData.clear();

	//meshListから必要なメッシュ情報を取得
	for (auto i = meshList.begin();i != meshList.end();i++)
	{
		MeshData mData;
		std::cout << "Node Name = " << (*i).first << std::endl;
		FbxNode* node = (*i).second;
		FbxMesh* mesh = node->GetMesh();

		//テクスチャファイル名の取得
		std::string tBuff;
		ParseFileName(GetTextureFilePath(mesh, node), tBuff);
		ConvertString(tBuff, mData.textureFileName);
		std::cout << "\tTexture FileName = " << tBuff << std::endl;

		//頂点情報の取得(頂点座標)
		FbxVector4* vertexBuff = mesh->GetControlPoints();
		for (int i = 0;i < mesh->GetControlPointsCount();i++)
		{
			VertexNormal v{};
			v.pos = XMFLOAT3((float)vertexBuff[i][0], (float)vertexBuff[i][1],
				(float)vertexBuff[i][2]);
			v.uv	 = ZeroVec2; //UV情報は後で取得し統合する
			v.normal = ZeroVec3;

			mData.vertices.push_back(v);
		}

		//レイヤー情報の取得(UV座標)
		FbxLayerElementUV* layerUV = mesh->GetLayer(0)->GetUVs();
		FbxGeometryElement::EMappingMode   uvMapping = layerUV->GetMappingMode();
		FbxGeometryElement::EReferenceMode uvRef	 = layerUV->GetReferenceMode();
		//マッピング,リファレンスは以下のみ対応する
		assert(uvMapping == FbxGeometryElement::eByControlPoint ||
			   uvMapping == FbxGeometryElement::eByPolygonVertex);
		assert(uvRef == FbxGeometryElement::eDirect ||
			   uvRef == FbxGeometryElement::eIndexToDirect);

		//レイヤー情報の取得(法線)
		FbxLayerElementNormal* layerNormal = mesh->GetLayer(0)->GetNormals();
		FbxGeometryElement::EMappingMode   nMapping = layerNormal->GetMappingMode();
		FbxGeometryElement::EReferenceMode nRef = layerNormal->GetReferenceMode();
		assert(nMapping == FbxGeometryElement::eByControlPoint ||
			   nMapping == FbxGeometryElement::eByPolygonVertex);
		assert(nRef == FbxGeometryElement::eDirect ||
			   nRef == FbxGeometryElement::eIndexToDirect);

		// 各ポリゴンの頂点を確認し、位置座標が一致しUV座標のみ異なる頂点があれば
		// MeshDataのverticesに新たな頂点として追加する
		std::vector<bool> checkList(mData.vertices.size());
		for (int i = 0;i < checkList.size();i++) checkList[i] = false;
		std::vector<std::vector<int>> addedVertex;
		for (int polyIndex = 0;polyIndex < mesh->GetPolygonCount();polyIndex++)
		{
			for (int vIndex = 2;vIndex >= 0;vIndex--) //三角形ポリゴンに合わせた条件
			{
				//頂点情報取得
				int vertexIndex, uvIndex;
				vertexIndex = mesh->GetPolygonVertex(polyIndex, vIndex);
				if (uvRef == FbxGeometryElement::eDirect)
				{
					uvIndex = vertexIndex;
				}
				else
				{
					uvIndex = mesh->GetTextureUVIndex(polyIndex, vIndex,
						FbxLayerElement::eTextureDiffuse);
				}

				//UV情報取得
				//FBXのUV座標は上限が逆転しているため対応させる
				FbxVector2 uvData = layerUV->GetDirectArray().GetAt(uvIndex);
				XMFLOAT2   uv	  = { (float)uvData.mData[0],1.0f - (float)uvData.mData[1] };

				//法線情報取得
				int normalIndex;
				if (nRef == FbxGeometryElement::eDirect)
				{
					normalIndex = vertexIndex;
				}
				else
				{
					normalIndex = layerNormal->GetIndexArray().GetAt(vertexIndex);
				}
				FbxVector4 nData = layerNormal->GetDirectArray().GetAt(normalIndex);
				XMFLOAT3 normal = { (float)nData.mData[0],(float)nData.mData[1],
					(float)nData.mData[2] };

				//未確認の頂点なら取得したUV座標を設定しcheckListをtrueにする
				if (!checkList[vertexIndex])
				{
					mData.vertices[vertexIndex].uv = uv;
					mData.vertices[vertexIndex].normal = normal;
					checkList[vertexIndex] = true;
				}
				//確認済み頂点でUV座標が異なるなら別途処理を行う
				else if (!IsEqualVector2D(mData.vertices[vertexIndex].uv, uv) ||
						 !IsEqualVector3D(mData.vertices[vertexIndex].normal, normal))
				{
					//位置,UV座標,法線が共に一致する頂点があれば追加処理を行わない
					bool hit = false;
					for (int i = 0;i < addedVertex.size();i++)
					{
						if (vertexIndex == addedVertex[i][0] &&
							IsEqualVector2D(mData.vertices[addedVertex[i][1]].uv, uv) &&
							IsEqualVector3D(mData.vertices[addedVertex[i][1]].normal, normal))
						{
							hit = true;
							vertexIndex = addedVertex[i][1];
							break;
						}
					}

					//頂点の追加処理
					if (!hit)
					{
						VertexNormal v = { mData.vertices[vertexIndex].pos,uv,normal };
						mData.vertices.push_back(v);
						std::vector<int> add;
						add.push_back(vertexIndex);
						add.push_back((int)mData.vertices.size() - 1);
						addedVertex.push_back(add);
						vertexIndex = add[1];
					}
				}
				//インデックスバッファにインデックス登録
				mData.indices.push_back((unsigned short)vertexIndex);
			}
		}
		std::cout << "\tVertex Num = " << mData.vertices.size() << std::endl;
		std::cout << "\tIndex Num = " << mData.indices.size() << std::endl;
		meshData.push_back(mData);
	}

	return true;
}

const char* GetTextureFilePath(FbxMesh* mesh, FbxNode* node)
{
	for (int i = 0;i < node->GetMaterialCount();i++)
	{
		FbxSurfaceMaterial* material = node->GetMaterial(i);
		FbxProperty prop = material->FindProperty(FbxSurfaceMaterial::sDiffuse);
		for (int j = 0;j < prop.GetSrcObjectCount<FbxFileTexture>();j++)
		{
			FbxFileTexture* texture = prop.GetSrcObject<FbxFileTexture>(j);
			if (!texture) continue;
			return texture->GetRelativeFileName();
		}
	}

	return "White";
}

void ParseFileName(const char* filePath, std::string& fileName)
{
	fileName.clear();
	fileName = filePath;

	int last = (int)fileName.find_last_of('\\');
	if (last == std::string::npos) return;
	fileName = fileName.substr(last + 1, fileName.size() - last - 1);
}

bool IsEqualVector2D(XMFLOAT2 v1, XMFLOAT2 v2)
{
	return std::fabs(v1.x - v2.x) < FLT_EPSILON &&
		   std::fabs(v1.y - v2.y) < FLT_EPSILON;
}

bool IsEqualVector3D(XMFLOAT3 v1, XMFLOAT3 v2)
{
	return std::fabs(v1.x - v2.x) < FLT_EPSILON &&
		   std::fabs(v1.y - v2.y) < FLT_EPSILON &&
		   std::fabs(v1.z - v2.z) < FLT_EPSILON;
}

void ConvertString(std::string& src, std::wstring& dst)
{
	wchar_t* buff = new wchar_t[src.size() + 1];
	size_t cvt;
	mbstowcs_s(&cvt, buff, src.size() + 1, src.c_str(), _TRUNCATE);
	dst = buff;
	delete[] buff;
}

bool SaveMeshData(std::vector<MeshData>& meshData, std::string& saveFileName,
	float scale, float rotX, float rotY, float rotZ)
{
	saveFileName = DST_FILEPATH + saveFileName;
	std::ofstream fout;
	fout.open(saveFileName.c_str(), std::ios::out | std::ios::binary | std::ios::trunc);
	if (!fout) return false;

	//メッシュの個数を保存(4バイト)
	unsigned int meshNum = (unsigned int)meshData.size();
	fout.write((char*)&meshNum, sizeof(unsigned int));

	//モデルデータの調整
	//スケール変換
	XMMATRIX transform = XMMatrixScaling(scale, -scale, scale);
	transform *= XMMatrixRotationX(XMConvertToRadians(rotX));
	transform *= XMMatrixRotationY(XMConvertToRadians(rotY));
	transform *= XMMatrixRotationZ(XMConvertToRadians(rotZ));
	//回転変換
	for (size_t i = 0;i < meshData.size();i++)
	{
		for (size_t j = 0;j < meshData[i].vertices.size();j++)
		{
			XMFLOAT3 p = meshData[i].vertices[j].pos;
			meshData[i].vertices[j].pos =
				XMFLOAT4(p.x, p.y, p.z, 1.0f) * transform;

			//スケール変換の影響を考慮し
			//法線ベクトルを正規化して保存
			XMFLOAT3 n = meshData[i].vertices[j].normal;
			meshData[i].vertices[j].normal =
				Normalize(XMFLOAT4(n.x, n.y, n.z, 0.0f) * transform);
		}
	}


	//MeshDataの保存
	for (size_t i = 0;i < meshData.size();i++)
	{
		//頂点の個数を保存(4バイト)
		unsigned int vertexNum = (unsigned int)meshData[i].vertices.size();
		fout.write((char*)&vertexNum, sizeof(unsigned int));
		
		//インデックスの個数を保存(4バイト)
		unsigned int indexNum = (unsigned int)meshData[i].indices.size();
		fout.write((char*)&indexNum, sizeof(unsigned int));

		//頂点データの保存(sizeof(VertexNormal) * vertexNumバイト)
		fout.write((char*)meshData[i].vertices.data(), sizeof(VertexNormal) * vertexNum);

		//インデックスデータの保存(sizeof(unsigned short) * indexNumバイト)
		fout.write((char*)meshData[i].indices.data(), sizeof(unsigned short) * indexNum);

		//テクスチャファイル名を保存(ワイド文字256文字分)
		wchar_t textureNameBuff[256];
		size_t nameSize = meshData[i].textureFileName.size();
		nameSize = (nameSize >= 255) ? 255 : nameSize;
		meshData[i].textureFileName.copy(textureNameBuff, nameSize);
		textureNameBuff[nameSize] = '\0';
		fout.write((char*)textureNameBuff, sizeof(wchar_t) * 256);
	}
	fout.close();

	return true;
}