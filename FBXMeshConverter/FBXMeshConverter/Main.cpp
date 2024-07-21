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
�v���g�^�C�v�錾
***************************************************************************/

/// <summary>
/// �V�[���ɂ���m�[�h���ċA�I�ɏ��񂵃��b�V���������m�[�h���A
/// �m�[�h�����L�[�Ƃ��ĘA�z�z��ɕۑ�����<para></para>
/// �Ȃ��A�m�[�h���̏d���͂Ȃ��Ƃ����O�񂪂���
/// </summary>
void GetMeshList(FbxNode* node, std::unordered_map<std::string, FbxNode*>& meshList);

/// <summary>
/// meshList���璸�_,�C���f�b�N�X,�e�N�X�`���t�@�C���������o��meshData�ɕۑ�
/// </summary>
bool ExtractMeshData(std::unordered_map<std::string, FbxNode*>& meshList,
	std::vector<MeshData>& meshData);

/// <summary>
/// �e�N�X�`���̃t�@�C���p�X���擾<para></para>
/// ���ꃁ�b�V���ɕ����̃e�N�X�`�����ݒ肳��Ă�����̂ɂ͖��Ή�
/// </summary>
const char* GetTextureFilePath(FbxMesh* mesh, FbxNode* node);

/// <summary>
/// �t�@�C���p�X�̃f�B���N�g���̋�؂�܂ł��Ȃ����t�@�C�����𒊏o��fileName�ɕۑ�
/// </summary>
void ParseFileName(const char* filePath, std::string& fileName);

bool IsEqualVector2D(XMFLOAT2 v1, XMFLOAT2 v2);

bool IsEqualVector3D(XMFLOAT3 v1, XMFLOAT3 v2);

/// <summary>
///���C�h������ւ̕ϊ��֐�
/// </summary>
void ConvertString(std::string& src, std::wstring& dst);

bool SaveMeshData(std::vector<MeshData>& meshData, std::string& saveFileName,
	float scale = 1.0f, float rotX = 0.0f, float rotY = 0.0f, float rotZ = 0.0f);

/***************************************************************************
��`
***************************************************************************/

int main()
{
	std::cout << "FBXMeshConverter" << std::endl;

	//�ǂݍ���FBX�t�@�C�������擾
	std::string srcFileName;
	std::cout << "Input 3DModel FileName(with extension) = ";
	std::cin >> srcFileName;
	srcFileName = SRC_FILEPATH + srcFileName;

	//�o��FBX�t�@�C�������擾
	std::string dstFileName;
	std::cout << "Output fbfix fileName = ";
	std::cin >> dstFileName;
	dstFileName += ".fbfix";

	//FBX���C�u�����̏���
	FbxManager*	   manager   = FbxManager::Create();
	FbxIOSettings* ioSetting = FbxIOSettings::Create(manager, IOSROOT);
	FbxImporter*   importer  = FbxImporter::Create(manager, "");
	if (!importer->Initialize(srcFileName.c_str(), -1, manager->GetIOSettings()))
	{
		std::cout << "�t�@�C���ǂݍ��ݎ��s: " << srcFileName.c_str() << std::endl;
		importer->Destroy();
		ioSetting->Destroy();
		manager->Destroy();
		return 1;
	}

	//�V�[���擾
	FbxScene* scene = FbxScene::Create(manager, "scene");
	importer->Import(scene);

	//�g�|���W�[���`(�O�p�`��)
	FbxGeometryConverter converter(manager);
	converter.Triangulate(scene, true);

	//�m�[�h�f�[�^���,���b�V�����X�g�擾
	std::unordered_map<std::string, FbxNode*> meshList;
	GetMeshList(scene->GetRootNode(), meshList);

	//fbfix�p�f�[�^�̎擾(���_,�C���f�b�N�X���A�e�N�X�`���t�@�C����)
	std::vector<MeshData> meshData;
	if (!ExtractMeshData(meshList, meshData))
	{
		std::cout << "MeshData�擾���s" << std::endl;
		scene->Destroy();
		importer->Destroy();
		ioSetting->Destroy();
		manager->Destroy();
		return 1;
	}

	//fbfix�t�@�C���̕ۑ�
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
		std::cout << dstFileName << "�̕ۑ��Ɏ��s" << std::endl;
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

	//meshList����K�v�ȃ��b�V�������擾
	for (auto i = meshList.begin();i != meshList.end();i++)
	{
		MeshData mData;
		std::cout << "Node Name = " << (*i).first << std::endl;
		FbxNode* node = (*i).second;
		FbxMesh* mesh = node->GetMesh();

		//�e�N�X�`���t�@�C�����̎擾
		std::string tBuff;
		ParseFileName(GetTextureFilePath(mesh, node), tBuff);
		ConvertString(tBuff, mData.textureFileName);
		std::cout << "\tTexture FileName = " << tBuff << std::endl;

		//���_���̎擾(���_���W)
		FbxVector4* vertexBuff = mesh->GetControlPoints();
		for (int i = 0;i < mesh->GetControlPointsCount();i++)
		{
			VertexNormal v{};
			v.pos = XMFLOAT3((float)vertexBuff[i][0], (float)vertexBuff[i][1],
				(float)vertexBuff[i][2]);
			v.uv	 = ZeroVec2; //UV���͌�Ŏ擾����������
			v.normal = ZeroVec3;

			mData.vertices.push_back(v);
		}

		//���C���[���̎擾(UV���W)
		FbxLayerElementUV* layerUV = mesh->GetLayer(0)->GetUVs();
		FbxGeometryElement::EMappingMode   uvMapping = layerUV->GetMappingMode();
		FbxGeometryElement::EReferenceMode uvRef	 = layerUV->GetReferenceMode();
		//�}�b�s���O,���t�@�����X�͈ȉ��̂ݑΉ�����
		assert(uvMapping == FbxGeometryElement::eByControlPoint ||
			   uvMapping == FbxGeometryElement::eByPolygonVertex);
		assert(uvRef == FbxGeometryElement::eDirect ||
			   uvRef == FbxGeometryElement::eIndexToDirect);

		//���C���[���̎擾(�@��)
		FbxLayerElementNormal* layerNormal = mesh->GetLayer(0)->GetNormals();
		FbxGeometryElement::EMappingMode   nMapping = layerNormal->GetMappingMode();
		FbxGeometryElement::EReferenceMode nRef = layerNormal->GetReferenceMode();
		assert(nMapping == FbxGeometryElement::eByControlPoint ||
			   nMapping == FbxGeometryElement::eByPolygonVertex);
		assert(nRef == FbxGeometryElement::eDirect ||
			   nRef == FbxGeometryElement::eIndexToDirect);

		// �e�|���S���̒��_���m�F���A�ʒu���W����v��UV���W�݈̂قȂ钸�_�������
		// MeshData��vertices�ɐV���Ȓ��_�Ƃ��Ēǉ�����
		std::vector<bool> checkList(mData.vertices.size());
		for (int i = 0;i < checkList.size();i++) checkList[i] = false;
		std::vector<std::vector<int>> addedVertex;
		for (int polyIndex = 0;polyIndex < mesh->GetPolygonCount();polyIndex++)
		{
			for (int vIndex = 2;vIndex >= 0;vIndex--) //�O�p�`�|���S���ɍ��킹������
			{
				//���_���擾
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

				//UV���擾
				//FBX��UV���W�͏�����t�]���Ă��邽�ߑΉ�������
				FbxVector2 uvData = layerUV->GetDirectArray().GetAt(uvIndex);
				XMFLOAT2   uv	  = { (float)uvData.mData[0],1.0f - (float)uvData.mData[1] };

				//�@�����擾
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

				//���m�F�̒��_�Ȃ�擾����UV���W��ݒ肵checkList��true�ɂ���
				if (!checkList[vertexIndex])
				{
					mData.vertices[vertexIndex].uv = uv;
					mData.vertices[vertexIndex].normal = normal;
					checkList[vertexIndex] = true;
				}
				//�m�F�ςݒ��_��UV���W���قȂ�Ȃ�ʓr�������s��
				else if (!IsEqualVector2D(mData.vertices[vertexIndex].uv, uv) ||
						 !IsEqualVector3D(mData.vertices[vertexIndex].normal, normal))
				{
					//�ʒu,UV���W,�@�������Ɉ�v���钸�_������Βǉ��������s��Ȃ�
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

					//���_�̒ǉ�����
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
				//�C���f�b�N�X�o�b�t�@�ɃC���f�b�N�X�o�^
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

	//���b�V���̌���ۑ�(4�o�C�g)
	unsigned int meshNum = (unsigned int)meshData.size();
	fout.write((char*)&meshNum, sizeof(unsigned int));

	//���f���f�[�^�̒���
	//�X�P�[���ϊ�
	XMMATRIX transform = XMMatrixScaling(scale, -scale, scale);
	transform *= XMMatrixRotationX(XMConvertToRadians(rotX));
	transform *= XMMatrixRotationY(XMConvertToRadians(rotY));
	transform *= XMMatrixRotationZ(XMConvertToRadians(rotZ));
	//��]�ϊ�
	for (size_t i = 0;i < meshData.size();i++)
	{
		for (size_t j = 0;j < meshData[i].vertices.size();j++)
		{
			XMFLOAT3 p = meshData[i].vertices[j].pos;
			meshData[i].vertices[j].pos =
				XMFLOAT4(p.x, p.y, p.z, 1.0f) * transform;

			//�X�P�[���ϊ��̉e�����l����
			//�@���x�N�g���𐳋K�����ĕۑ�
			XMFLOAT3 n = meshData[i].vertices[j].normal;
			meshData[i].vertices[j].normal =
				Normalize(XMFLOAT4(n.x, n.y, n.z, 0.0f) * transform);
		}
	}


	//MeshData�̕ۑ�
	for (size_t i = 0;i < meshData.size();i++)
	{
		//���_�̌���ۑ�(4�o�C�g)
		unsigned int vertexNum = (unsigned int)meshData[i].vertices.size();
		fout.write((char*)&vertexNum, sizeof(unsigned int));
		
		//�C���f�b�N�X�̌���ۑ�(4�o�C�g)
		unsigned int indexNum = (unsigned int)meshData[i].indices.size();
		fout.write((char*)&indexNum, sizeof(unsigned int));

		//���_�f�[�^�̕ۑ�(sizeof(VertexNormal) * vertexNum�o�C�g)
		fout.write((char*)meshData[i].vertices.data(), sizeof(VertexNormal) * vertexNum);

		//�C���f�b�N�X�f�[�^�̕ۑ�(sizeof(unsigned short) * indexNum�o�C�g)
		fout.write((char*)meshData[i].indices.data(), sizeof(unsigned short) * indexNum);

		//�e�N�X�`���t�@�C������ۑ�(���C�h����256������)
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