#include "LiarMaxNodeParse.h"

#include <fstream>

namespace Liar
{

	LiarMaxNodeParse::LiarMaxNodeParse()
	{
	}


	LiarMaxNodeParse::~LiarMaxNodeParse()
	{
	}

	void LiarMaxNodeParse::AddBones(IGameNode* node)
	{
		if (node)
		{
			for (std::vector<IGameNode*>::const_iterator it = m_bones.begin(); it != m_bones.end(); ++it)
			{
				if (*it == node) return;
			}

			m_bones.push_back(node);
		}
	}

	void LiarMaxNodeParse::ParseBones(Liar::LiarPluginCfg* ctr, IGameNode* varGameNode)
	{
		IGameObject::ObjectTypes tmpObjectType = varGameNode->GetIGameObject()->GetIGameType();
		std::string boneName;
		if (tmpObjectType == IGameObject::IGAME_BONE)
		{
			AddBones(varGameNode);
		}

		int tmpChildCount = varGameNode->GetChildCount();
		for (int i = 0; i < tmpChildCount; i++)
		{
			IGameNode* subGameNode = varGameNode->GetNodeChild(i);
			ParseBones(ctr, subGameNode);
		}
	}

	int LiarMaxNodeParse::ParseRootNode(Liar::LiarPluginCfg* ctr, bool tmpSelected)
	{
		IGameScene* tmpGameScene = GetIGameInterface();
		IGameConversionManager* tmpGameConversionManager = GetConversionManager();
		tmpGameConversionManager->SetCoordSystem(ctr->coordSystemType);
		tmpGameScene->InitialiseIGame(tmpSelected);
		tmpGameScene->SetStaticFrame(0);
		int tmpTopLevelNodeCount = tmpGameScene->GetTopLevelNodeCount();
		if (tmpTopLevelNodeCount == 0)
		{
			tmpGameScene->ReleaseIGame();
			return FALSE;
		}

		if (ctr->exportModifier)
		{
			for (int i = 0; i < tmpTopLevelNodeCount; i++)
			{
				IGameNode* tmpGameNode = tmpGameScene->GetTopLevelNode(i);
				ParseBones(ctr, tmpGameNode);
			}
			WriteSkeleon(ctr);
		}

		std::vector<IGameMaterial*> materials;
		int meshSize = 0;
		for (int i = 0; i < tmpTopLevelNodeCount; i++)
		{
			IGameNode* tmpGameNode = tmpGameScene->GetTopLevelNode(i);
			ParseGameNode(ctr, tmpGameNode, materials, meshSize);
		}

		// 写材质
		Write(ctr, materials, meshSize);

		ParseAnim(ctr);

		tmpGameScene->ReleaseIGame();

		return TRUE;
	}

	void LiarMaxNodeParse::ParseGameNode(Liar::LiarPluginCfg* ctr, IGameNode* varGameNode, std::vector<IGameMaterial*>& materials, int& meshSize)
	{
		IGameObject::ObjectTypes tmpObjectType = varGameNode->GetIGameObject()->GetIGameType();
		switch (tmpObjectType)
		{
		case IGameObject::IGAME_UNKNOWN:
			break;
		case IGameObject::IGAME_LIGHT:
			break;
		case IGameObject::IGAME_SPLINE:
			break;
		case IGameObject::IGAME_CAMERA:
			break;
		case IGameObject::IGAME_BONE:
			break;
		case IGameObject::IGAME_IKCHAIN:
			break;
		case IGameObject::IGAME_XREF:
			break;
		case IGameObject::IGAME_MESH:
		case IGameObject::IGAME_HELPER:
			ParseValidMeshNode(ctr, varGameNode, tmpObjectType, materials, meshSize);
			break;
		default:
			break;
		}		
		
	}

	void LiarMaxNodeParse::ParseValidMeshNode(Liar::LiarPluginCfg* ctr, IGameNode* varGameNode, IGameObject::ObjectTypes type, 
		std::vector<IGameMaterial*>& materials, int& meshSize)
	{
		IGameMesh* tmpGameMesh = nullptr;
		IGameObject* tmpGameObject = varGameNode->GetIGameObject();
		if (type == IGameObject::ObjectTypes::IGAME_HELPER)
		{
			IGameSupportObject* tmpGameSupportObject = (IGameSupportObject*)tmpGameObject;
			tmpGameMesh = tmpGameSupportObject->GetMeshObject();
		}
		else
		{
			tmpGameMesh = (IGameMesh*)tmpGameObject;
		}

		if (tmpGameMesh && tmpGameMesh->InitializeData())
		{
			/*Liar::LiarNode* node = parentNode->AddChild();
			LiarStructUtil::GetWSTR2Char(varGameNode->GetName(), node->GetNodeName());*/
			// 解析子结点
			ParseGameMesh(ctr, tmpGameMesh, materials, meshSize);
			// 解析子结点
			ParseChildNode(ctr, varGameNode, materials, meshSize);
		}
	}

	void LiarMaxNodeParse::ParseChildNode(Liar::LiarPluginCfg* ctr, IGameNode* varGameNode, 
		std::vector<IGameMaterial*>& materials, int& meshSize)
	{
		//遍历子节点
		int tmpChildCount = varGameNode->GetChildCount();
		for (size_t i = 0; i < tmpChildCount; i++)
		{
			IGameNode* tmpGameNode = varGameNode->GetNodeChild(static_cast<int>(i));
			ParseGameNode(ctr, tmpGameNode, materials, meshSize);
		}
	}

	void LiarMaxNodeParse::ParseGameMesh(Liar::LiarPluginCfg* ctr, IGameMesh* tmpGameMesh, 
		std::vector<IGameMaterial*>& materials, int& meshSize)
	{
		tmpGameMesh->GetMaxMesh()->buildNormals();
		if (!tmpGameMesh->InitializeData())
		{
			return;
		}

		// 解析 Geometory
		ParseGeometory(ctr, tmpGameMesh, materials, meshSize);
	}

	void LiarMaxNodeParse::ParseSkinInfo(int max, Point3 pos, const std::vector<Point3>& vec, std::map<int, std::vector<Point2>>& allSkins,
		std::vector<FloatHeapOperator*>& boneIds, std::vector<FloatHeapOperator*>& boneWeights, int& boneIdIndex, int& boneWeithIndex)
	{
		FloatHeapOperator* u = new FloatHeapOperator(max);
		FloatHeapOperator* f = new FloatHeapOperator(max);

		for (std::map<int, std::vector<Point2>>::const_iterator iter = allSkins.begin(); iter != allSkins.end(); ++iter)
		{
			int posIndex = iter->first;
			const Point3& tmpPos = vec[posIndex];
			if (tmpPos == pos)
			{
				const std::vector<Point2>& info = iter->second;
				size_t len = info.size();
				len = len > max ? max : len;

				for (size_t i = 0; i < len; ++i)
				{
					(*u)[i] = info[i].x;
					(*f)[i] = info[i].y;
				}

				for (size_t i = len; i < max; ++i)
				{
					(*u)[i] = 0.0f;
					(*f)[i] = 0.0f;
				}

				boneIdIndex = AddPoint(boneIds, u);
				boneWeithIndex = AddPoint(boneWeights, f);

				return;
			}
		}

		for (size_t i = 0; i < max; ++i)
		{
			(*u)[i] = 0;
			(*f)[i] = 0.0f;
		}
		boneIdIndex = AddPoint(boneIds, u);
		boneWeithIndex = AddPoint(boneWeights, f);
	}

	void LiarMaxNodeParse::ParseGeometory(Liar::LiarPluginCfg* ctr, IGameMesh* tmpGameMesh, 
		std::vector<IGameMaterial*>& materials, int& meshSize)
	{
		//bool revertYZ = ctr->coordSystemType == IGameConversionManager::CoordSystem::Max3DS ? false : true;
		bool revertYZ = false;

		std::map<int, std::vector<Point2>> allSkins;
		std::vector<Point3> tmpPositions;
		// skin
		int perSkin = ParseSkin(allSkins, tmpPositions, ctr, tmpGameMesh);
		perSkin = perSkin > 4 ? 4 : perSkin;
		size_t skinCount = allSkins.size();

		std::vector<Point3> positions;
		std::vector<Point3> normals;
		std::vector<Point2> texCoords;
		std::vector<Point3> colors;
		std::vector<FloatHeapOperator*> boneIds;
		std::vector<FloatHeapOperator*> boneWeights;
		std::vector<unsigned int> indices;
		std::vector<unsigned int> materialIndics;
		std::vector<IntHeapOperator*> rawkeys;
		unsigned int curVetexIndex;

		int umax = 1;

		if (ctr->vertexNormal) umax++;
		if (ctr->vertexColor) umax++;
		if (ctr->textureCoord) umax++;
		if (ctr->exportModifier && ctr->skin && skinCount > 0) umax += 2;

		int tmpFaceCount = tmpGameMesh->GetNumberOfFaces();
		for (int tmpFaceIndex = 0; tmpFaceIndex < tmpFaceCount; tmpFaceIndex++)
		{
			FaceEx* tmpFaceEx = tmpGameMesh->GetFace(tmpFaceIndex);

			for (int tmpFaceVertexIndex = 0; tmpFaceVertexIndex < 3; tmpFaceVertexIndex++)
			{
				//顶点
				int tmpVertexIndex = tmpFaceEx->vert[tmpFaceVertexIndex];
				int tmpNormalIndex = 0;
				int tmpColorIndex = 0;
				int tmpUVIndex = 0;
				int tmpBoneIndex = 0;
				int tmpBoneWeightIndex = 0;

				Point3 tmpPositoin = tmpGameMesh->GetVertex(tmpVertexIndex);
				tmpVertexIndex = AddPoint(positions, tmpPositoin, revertYZ);

				IntHeapOperator* key = new IntHeapOperator(umax);
				size_t ulen = 0;
				(*key)[ulen++] = tmpVertexIndex;

				//Normal
				if (ctr->vertexNormal)
				{
					tmpNormalIndex = tmpFaceEx->norm[tmpFaceVertexIndex];
					tmpNormalIndex = AddPoint(normals, tmpGameMesh->GetNormal(tmpNormalIndex), revertYZ);
					(*key)[ulen++] = tmpNormalIndex;
				}

				// color
				if (ctr->vertexColor)
				{
					tmpColorIndex = tmpFaceEx->color[tmpFaceVertexIndex];
					tmpColorIndex = AddPoint(colors, tmpGameMesh->GetColorVertex(tmpColorIndex), revertYZ);
					(*key)[ulen++] = tmpColorIndex;
				}

				// uv
				if (ctr->textureCoord)
				{
					tmpUVIndex = tmpFaceEx->texCoord[tmpFaceVertexIndex];
					tmpUVIndex = AddPoint(texCoords, tmpGameMesh->GetTexVertex(tmpUVIndex), ctr->revertUV);
					(*key)[ulen++] = tmpUVIndex;
				}

				// skin
				if (ctr->exportModifier && ctr->skin && skinCount > 0)
				{
					ParseSkinInfo(perSkin, tmpPositoin, tmpPositions, allSkins, boneIds, boneWeights, tmpBoneIndex, tmpBoneWeightIndex);
					(*key)[ulen++] = tmpBoneIndex;
					(*key)[ulen++] = tmpBoneWeightIndex;
				}

				// 顶点索引
				curVetexIndex = AddPoint(rawkeys, key);
				indices.push_back(curVetexIndex);
			}

			//获取当前面的材质
			IGameMaterial* tmpGameMaterial = tmpGameMesh->GetMaterialFromFace(tmpFaceEx);
			if (tmpGameMaterial)
			{
				if (GetTextureSize(tmpGameMaterial) > 0)
				{
					size_t numMtl = materials.size();
					int findIndex = -1;

					for (int mi = 0; mi < numMtl; ++mi)
					{
						if (materials[mi] == tmpGameMaterial)
						{
							findIndex = mi;
							break;
						}
					}

					if (findIndex < 0)
					{
						materials.push_back(tmpGameMaterial);
						findIndex = static_cast<int>(numMtl);
					}


					int miiIndex = -1;
					for (int mii = 0; mii < materialIndics.size(); ++mii)
					{
						if (materialIndics[mii] == findIndex)
						{
							miiIndex = mii;
						}
					}

					if (miiIndex < 0)
					{
						materialIndics.push_back(findIndex);
					}
				}
			}
		}

		// 开始写入
		std::string path = ctr->name;
		std::string folder, last;
		Liar::LiarMaxNodeParse::GetHeadAndLast(path, folder, last, "\\");

		std::string baseName = Liar::LiarMaxNodeParse::GetHead(last);
		baseName = baseName + std::to_string(meshSize++);
		std::string meshName = baseName + Liar::MESH_EXT;
		char fullName[MAX_PATH];
		sprintf_s(fullName, "%s\\%s", folder.c_str(), meshName.c_str());
		FILE* hFile = fopen(fullName, "wb");
		// geometry类型
		Liar::GeometryVertexType GeoType = GetVertexType(ctr);
		fwrite(&GeoType, sizeof(int), 1, hFile);
		// 顶点类型
		Write(Liar::VertexElementAttr::ELEMENT_ATTR_POSITION, positions, hFile);
		Write(GeoType, normals, texCoords, colors, boneIds, boneWeights, perSkin, hFile);
		// 索引
		Write(Liar::VertexElementAttr::ELEMENT_ATTR_RAW_KEY, rawkeys, umax, hFile);
		// 顶点
		Write(Liar::VertexElementAttr::ELEMENT_ATTR_RAW_INDICES, indices, hFile);
		// 材质引用
		//Write(Liar::VertexElementAttr::ELEMENT_ATTR_RAW_MTL_INDICES, materialIndics, hFile);
		size_t mltLen = materialIndics.size();
		int mltIndex = -1;
		if (mltLen > 0) mltIndex = materialIndics[0];
		fwrite(&mltIndex, sizeof(int), 1, hFile);

		fclose(hFile);

		size_t tmpindex = 0;
		for (tmpindex = 0; tmpindex < boneIds.size(); ++tmpindex) delete boneIds[tmpindex];
		for (tmpindex = 0; tmpindex < boneWeights.size(); ++tmpindex) delete boneWeights[tmpindex];
		for (tmpindex = 0; tmpindex < rawkeys.size(); ++tmpindex) delete rawkeys[tmpindex];
	}

	void LiarMaxNodeParse::Write(Liar::GeometryVertexType type,
		std::vector<Point3>& normals, std::vector<Point2>& texCoords, std::vector<Point3>& colors,
		std::vector<FloatHeapOperator*>& boneIds, std::vector<FloatHeapOperator*>& boneWeights, size_t perSkin, FILE* hFile)
	{
		switch (type)
		{
		case GEOMETRY_VERTEX_TYPE_POSITION_NORMAL:
			Write(Liar::VertexElementAttr::ELEMENT_ATTR_NORMAL, normals, hFile);
			break;
		case GEOMETRY_VERTEX_TYPE_POSITION_COLOR:
			Write(Liar::VertexElementAttr::ELEMENT_ATTR_COLOR, colors, hFile);
			break;
		case GEOMETRY_VERTEX_TYPE_POSITION_TEXTURE:
			Write(Liar::VertexElementAttr::ELEMENT_ATTR_TEXTURECOORDINATE, texCoords, hFile);
			break;
		case GEOMETRY_VERTEX_TYPE_POSITION_NORMAL_COLOR:
		{
			Write(Liar::VertexElementAttr::ELEMENT_ATTR_NORMAL, normals, hFile);
			Write(Liar::VertexElementAttr::ELEMENT_ATTR_COLOR, colors, hFile);
			break;
		}
		case GEOMETRY_VERTEX_TYPE_POSITION_NORMAL_TEXTURE:
		{
			Write(Liar::VertexElementAttr::ELEMENT_ATTR_NORMAL, normals, hFile);
			Write(Liar::VertexElementAttr::ELEMENT_ATTR_TEXTURECOORDINATE, texCoords, hFile);
			break;
		}
		case GEOMETRY_VERTEX_TYPE_POSITION_NORMAL_COLOR_TEXTURE:
		{
			Write(Liar::VertexElementAttr::ELEMENT_ATTR_NORMAL, normals, hFile);
			Write(Liar::VertexElementAttr::ELEMENT_ATTR_COLOR, colors, hFile);
			Write(Liar::VertexElementAttr::ELEMENT_ATTR_TEXTURECOORDINATE, texCoords, hFile);
			break;
		}
		case GEOMETRY_VERTEX_TYPE_POSITION_TEXTURE_SKIN:
		{
			Write(Liar::VertexElementAttr::ELEMENT_ATTR_TEXTURECOORDINATE, texCoords, hFile);
			Write(Liar::VertexElementAttr::ELEMENT_ATTR_BONE_INDICES, boneIds, perSkin, hFile);
			Write(Liar::VertexElementAttr::ELEMENT_ATTR_BONE_WEIGHTS, boneWeights, perSkin, hFile);
			break;
		}
		case GEOMETRY_VERTEX_TYPE_POSITION_NORMAL_COLOR_SKIN:
		{
			Write(Liar::VertexElementAttr::ELEMENT_ATTR_NORMAL, normals, hFile);
			Write(Liar::VertexElementAttr::ELEMENT_ATTR_COLOR, colors, hFile);
			Write(Liar::VertexElementAttr::ELEMENT_ATTR_BONE_INDICES, boneIds, perSkin, hFile);
			Write(Liar::VertexElementAttr::ELEMENT_ATTR_BONE_WEIGHTS, boneWeights, perSkin, hFile);
			break;
		}
		case GEOMETRY_VERTEX_TYPE_POSITION_NORMAL_TEXTURE_SKIN:
		{
			Write(Liar::VertexElementAttr::ELEMENT_ATTR_NORMAL, normals, hFile);
			Write(Liar::VertexElementAttr::ELEMENT_ATTR_TEXTURECOORDINATE, texCoords, hFile);
			Write(Liar::VertexElementAttr::ELEMENT_ATTR_BONE_INDICES, boneIds, perSkin, hFile);
			Write(Liar::VertexElementAttr::ELEMENT_ATTR_BONE_WEIGHTS, boneWeights, perSkin, hFile);
			break;
		}
		case GEOMETRY_VERTEX_TYPE_POSITION_NORMAL_COLOR_TEXTURE_SKIN:
		{
			Write(Liar::VertexElementAttr::ELEMENT_ATTR_NORMAL, normals, hFile);
			Write(Liar::VertexElementAttr::ELEMENT_ATTR_COLOR, colors, hFile);
			Write(Liar::VertexElementAttr::ELEMENT_ATTR_TEXTURECOORDINATE, texCoords, hFile);
			Write(Liar::VertexElementAttr::ELEMENT_ATTR_BONE_INDICES, boneIds, perSkin, hFile);
			Write(Liar::VertexElementAttr::ELEMENT_ATTR_BONE_WEIGHTS, boneWeights, perSkin, hFile);
			break;
		}
		}
	}

	void LiarMaxNodeParse::WriteSkeleon(Liar::LiarPluginCfg* ctr)
	{
		if (!ctr->skeleton) return;
		std::string path = ctr->name;
		std::string folder, last;
		Liar::LiarMaxNodeParse::GetHeadAndLast(path, folder, last, "\\");
		char fullName[MAX_PATH];
		sprintf_s(fullName, "%s\\%s%s", folder.c_str(), ctr->skeletonName.c_str(), SKELEON_EXT);
		FILE* hFile = fopen(fullName, "wb");
		// 长度
		size_t len = m_bones.size();
		fwrite(&len, sizeof(int), 1, hFile);

		// 集合数据
		std::vector<Point3> positions;
		std::vector<Quat> rotations;
		std::vector<Point3> scales;

		std::string boneName;
		std::string parentName;
		int parentIndex = -1;
		size_t intSize = sizeof(int);
		for (size_t i = 0; i < len; ++i)
		{
			IGameNode* varGameNode = m_bones[i];

			LiarMaxNodeParse::GetWSTR2Char(varGameNode->GetName(), boneName);
			IGameNode* parent = varGameNode->GetNodeParent();

			parentIndex = -1;
			if (parent)
			{
				LiarMaxNodeParse::GetWSTR2Char(parent->GetName(), parentName);
				parentIndex = GetBoneIndex(boneName);
			}

			GMatrix gm = varGameNode->GetLocalTM(0);
			Point3 pos = gm.Translation();
			Quat rota = gm.Rotation();
			Point3 scale = gm.Scaling();

			// 先写名字
			Write(boneName, hFile);
			// 写父骨骼索引
			fwrite(&parentIndex, intSize, 1, hFile);
			// 写矩阵信息
			int posIndex = AddPoint(positions, pos);
			int rotationIndex = AddPoint(rotations, rota);
			int scaleIndex = AddPoint(scales, scale);

			fwrite(&posIndex, intSize, 1, hFile);
			fwrite(&rotationIndex, intSize, 1, hFile);
			fwrite(&scaleIndex, intSize, 1, hFile);
		}

		Write(Liar::VertexElementAttr::ELEMENT_ATTR_POSITION, positions, hFile);
		Write(Liar::VertexElementAttr::ELEMENT_ATTR_ROTATION, rotations, hFile);
		Write(Liar::VertexElementAttr::ELEMENT_ATTR_SCALE, scales, hFile);

		fclose(hFile);
	}

	int LiarMaxNodeParse::GetVertsIndex(const std::vector<Point3>& verts, Point3 p)
	{
		for (int i = 0; i < verts.size(); ++i)
		{
			if (verts[i] == p) return i;
		}
		return -1;
	}

	int LiarMaxNodeParse::GetBoneIndex(const std::string& boneName)
	{
		std::string nodeName;
		for (int i = 0; i < m_bones.size(); ++i)
		{
			LiarMaxNodeParse::GetWSTR2Char(m_bones[i]->GetName(), nodeName);
			if (nodeName == boneName) return i;
		}
		return -1;
	}

	int LiarMaxNodeParse::ParseSkin(std::map<int, std::vector<Point2>>& allSkins, std::vector<Point3>& vec, Liar::LiarPluginCfg* ctr, IGameMesh* tmpGameMesh)
	{
		int max = 0;

		if (ctr->exportModifier && ctr->skin)
		{
			//判断有没有修改器，有修改器的就是骨骼动画
			std::string boneName;
			std::string modfierName;
			int tmpModifiersNum = tmpGameMesh->GetNumModifiers();
			for (int tmpModifierIndex = 0; tmpModifierIndex < tmpModifiersNum; tmpModifierIndex++)
			{
				IGameModifier* tmpGameModifier = tmpGameMesh->GetIGameModifier(tmpModifierIndex);

				//只处理骨骼动画修改器
				if (tmpGameModifier->IsSkin())
				{
					IGameSkin* tmpGameSkin = (IGameSkin*)tmpGameModifier;
					//int tmpNumOfSkinnedVerts = tmpGameSkin->GetNumOfSkinnedVerts();

					//获取顶点受骨骼影响数
					int vertSum = tmpGameMesh->GetNumberOfVerts();
					for (int i = 0; i < vertSum; ++i)
					{
						Point3 pos = tmpGameMesh->GetVertex(i);
						int posIndex = AddPoint(vec, pos);
						//获取当前顶点的骨骼
						std::vector<Point2> info;
						size_t type = tmpGameSkin->GetVertexType(i);
						int curnum = 0;
						if (type == IGameSkin::IGAME_RIGID)
						{
							INode* bNode = tmpGameSkin->GetBone(i, 0);
							LiarMaxNodeParse::GetWSTR2Char(bNode->GetName(), boneName);
							int boneIndex = GetBoneIndex(boneName);
							if (boneIndex >= 0)
							{
								Point2 sf;
								sf.x = boneIndex;
								sf.y = 1.0f;
								info.push_back(sf);
								++curnum;
							}
						}
						else //blended
						{
							int boneNum = tmpGameSkin->GetNumberOfBones(i);
							if (boneNum > 0)
							{
								for (int j = 0; j < boneNum; ++j)
								{
									INode* bNode = tmpGameSkin->GetBone(i, j);
									LiarMaxNodeParse::GetWSTR2Char(bNode->GetName(), boneName);
									int boneIndex = GetBoneIndex(boneName);
									if (boneIndex >= 0)
									{
										float weight = tmpGameSkin->GetWeight(i, j);
										Point2 sf;
										sf.x = boneIndex;
										sf.y = weight;
										info.push_back(sf);
										++curnum;
									}
								}

								max = curnum > max ? curnum : max;
							}
						}

						allSkins[posIndex] = info;
					}

				}
			}
		}

		return max;
	}


	void LiarMaxNodeParse::ParseAnim(Liar::LiarPluginCfg* ctr)
	{
		if (!ctr->exportAnim) return;

		IGameScene* tmpGameScene = GetIGameInterface();
		TimeValue tmpTimeValueBegin = tmpGameScene->GetSceneStartTime();
		TimeValue tmpTimeValueEnd = tmpGameScene->GetSceneEndTime();
		TimeValue tmpTimeValueTicks = tmpGameScene->GetSceneTicks();
		int tmpFrameCount = (tmpTimeValueEnd - tmpTimeValueBegin) / tmpTimeValueTicks;

		// 记录
		std::map<std::string, std::vector<GMatrix>> anis;
		int trackLen = 0;
		std::string boneName;
		for (size_t i = 0; i < m_bones.size(); ++i)
		{
			IGameNode* node = m_bones[i];
			LiarMaxNodeParse::GetWSTR2Char(node->GetName(), boneName);
			std::map<std::string, std::vector<GMatrix>>::iterator it;
			it = anis.find(boneName);
			if (it == anis.end())
			{
				anis[boneName];
				it = anis.find(boneName);
			}
			std::vector<GMatrix>& vec = it->second;
			GMatrix tmp;
			for (int j = 0; j <= tmpFrameCount; ++j)
			{
				int nowMS = j*tmpTimeValueTicks;
				GMatrix gm = node->GetLocalTM(nowMS + tmpTimeValueBegin);
				if (tmp == gm) continue;
				tmp = gm;
				if( j > 0) vec.push_back(gm);
			}

			if (vec.size() > 0) ++trackLen;
			else anis.erase(boneName);
		}

		std::string path = ctr->name;
		std::string folder, last;
		Liar::LiarMaxNodeParse::GetHeadAndLast(path, folder, last, "\\");
		char fullName[MAX_PATH];
		sprintf_s(fullName, "%s\\%s%s", folder.c_str(), ctr->animName.c_str(), ANI_EXT);
		FILE* hFile = fopen(fullName, "wb");

		// 写频率
		fwrite(&tmpTimeValueTicks, sizeof(int), 1, hFile);
		// 写长度
		fwrite(&tmpFrameCount, sizeof(int), 1, hFile);
		// track 长度
		fwrite(&trackLen, sizeof(int), 1, hFile);


		// 总信息
		std::vector<Point3> positions;
		std::vector<Quat> rotations;
		std::vector<Point3> scales;

		size_t intSize = sizeof(int);
		std::map<std::string, std::vector<GMatrix>>::iterator iter;
		for (iter = anis.begin(); iter != anis.end(); ++iter)
		{
			const std::string& boneName = iter->first;
			std::vector<GMatrix>& tracks = iter->second;
			size_t keyFrameLen = tracks.size();
			// 骨骼名字
			Write(boneName, hFile);
			// keyFrame长度
			fwrite(&keyFrameLen, intSize, 1, hFile);
			for (size_t i = 0; i < keyFrameLen; ++i)
			{
				GMatrix gm = tracks[i];
				Point3 pos = gm.Translation();
				Quat rota = gm.Rotation();
				Point3 scale = gm.Scaling();

				int posIndex = AddPoint(positions, pos);
				int rotationIndex = AddPoint(rotations, rota);
				int scaleIndex = AddPoint(scales, scale);

				fwrite(&posIndex, intSize, 1, hFile);
				fwrite(&rotationIndex, intSize, 1, hFile);
				fwrite(&scaleIndex, intSize, 1, hFile);
			}
		}

		Write(Liar::VertexElementAttr::ELEMENT_ATTR_POSITION, positions, hFile);
		Write(Liar::VertexElementAttr::ELEMENT_ATTR_ROTATION, rotations, hFile);
		Write(Liar::VertexElementAttr::ELEMENT_ATTR_SCALE, scales, hFile);

		fclose(hFile);
	}

	Liar::GeometryVertexType LiarMaxNodeParse::GetVertexType(Liar::LiarPluginCfg* cfg)
	{
		if (!cfg->vertexNormal && !cfg->vertexColor && !cfg->textureCoord)
		{
			return Liar::GeometryVertexType::GEOMETRY_VERTEX_TYPE_POSITION;
		}
		else if (cfg->exportModifier && cfg->skin && cfg->textureCoord && !cfg->vertexColor && !cfg->vertexNormal)
		{
			return Liar::GeometryVertexType::GEOMETRY_VERTEX_TYPE_POSITION_TEXTURE_SKIN;
		}
		else if (cfg->exportModifier && cfg->skin && cfg->vertexNormal && cfg->vertexColor && !cfg->textureCoord)
		{
			return Liar::GeometryVertexType::GEOMETRY_VERTEX_TYPE_POSITION_NORMAL_COLOR_SKIN;
		}
		else if (cfg->exportModifier && cfg->skin && cfg->vertexNormal && !cfg->vertexColor && cfg->textureCoord)
		{
			return Liar::GeometryVertexType::GEOMETRY_VERTEX_TYPE_POSITION_NORMAL_TEXTURE_SKIN;
		}
		else if (cfg->vertexNormal && !cfg->vertexColor && !cfg->textureCoord)
		{
			return Liar::GeometryVertexType::GEOMETRY_VERTEX_TYPE_POSITION_NORMAL;
		}
		else if (!cfg->vertexNormal && cfg->vertexColor && !cfg->textureCoord)
		{
			return Liar::GeometryVertexType::GEOMETRY_VERTEX_TYPE_POSITION_COLOR;
		}
		else if (!cfg->vertexNormal && !cfg->vertexColor && cfg->textureCoord)
		{
			return Liar::GeometryVertexType::GEOMETRY_VERTEX_TYPE_POSITION_TEXTURE;
		}
		else if (cfg->vertexNormal && cfg->vertexColor && !cfg->textureCoord)
		{
			return Liar::GeometryVertexType::GEOMETRY_VERTEX_TYPE_POSITION_NORMAL_COLOR;
		}
		else if (cfg->vertexNormal && !cfg->vertexColor && cfg->textureCoord)
		{
			return Liar::GeometryVertexType::GEOMETRY_VERTEX_TYPE_POSITION_NORMAL_TEXTURE;
		}
		else
		{
			return Liar::GeometryVertexType::GEOMETRY_VERTEX_TYPE_POSITION_NORMAL_COLOR_TEXTURE;
		}
	}

	int LiarMaxNodeParse::AddPoint(std::vector<Point2>& vec, Point2 p, bool revertYZ)
	{
		if (revertYZ) RevertYZ(p);
		size_t size = vec.size();
		for (int i = 0; i < size; ++i)
		{
			if (vec[i] == p) return i;
		}
		vec.push_back(p);
		return static_cast<int>(size);
	}

	int LiarMaxNodeParse::AddPoint(std::vector<Point3>& vec, Point3 p, bool revertYZ)
	{
		if (revertYZ) RevertYZ(p);
		size_t size = vec.size();
		for (int i = 0; i < size; ++i)
		{
			if (vec[i] == p) return i;
		}
		vec.push_back(p);
		return static_cast<int>(size);
	}

	int LiarMaxNodeParse::AddPoint(std::vector<Point4>& vec, Point4 p)
	{
		size_t size = vec.size();
		for (int i = 0; i < size; ++i)
		{
			if (vec[i] == p) return i;
		}
		vec.push_back(p);
		return static_cast<int>(size);
	}

	int LiarMaxNodeParse::AddPoint(std::vector<Quat>& vec, Quat p)
	{
		size_t size = vec.size();
		for (int i = 0; i < size; ++i)
		{
			if (vec[i] == p) return i;
		}
		vec.push_back(p);
		return static_cast<int>(size);
	}

	int LiarMaxNodeParse::AddPoint(std::vector<IntHeapOperator*>& vec, IntHeapOperator* p)
	{
		size_t size = vec.size();
		for (int i = 0; i < size; ++i)
		{
			if ((*(vec[i])) == (*p)) return i;
		}
		vec.push_back(p);
		return static_cast<int>(size);
	}

	int LiarMaxNodeParse::AddPoint(std::vector<FloatHeapOperator*>& vec, FloatHeapOperator* p)
	{
		size_t size = vec.size();
		for (int i = 0; i < size; ++i)
		{
			if ((*(vec[i])) == (*p)) return i;
		}
		vec.push_back(p);
		return static_cast<int>(size);
	}

	// 写类型数据
	void LiarMaxNodeParse::Write(Liar::VertexElementAttr atype, std::vector<Point2>& vec, FILE* hFile)
	{
		fwrite(&atype, sizeof(int), 1, hFile);
		Write(vec, hFile);
	}

	void LiarMaxNodeParse::Write(Liar::VertexElementAttr atype, std::vector<Point3>& vec, FILE* hFile)
	{
		fwrite(&atype, sizeof(int), 1, hFile);
		Write(vec, hFile);
	}

	void LiarMaxNodeParse::Write(Liar::VertexElementAttr atype, std::vector<Point4>& vec, FILE* hFile)
	{
		fwrite(&atype, sizeof(int), 1, hFile);
		bool transInt = atype == Liar::VertexElementAttr::ELEMENT_ATTR_RAW_KEY;
		Write(vec, hFile, transInt);
	}

	void LiarMaxNodeParse::Write(Liar::VertexElementAttr atype, std::vector<Quat>& vec, FILE* hFile)
	{
		fwrite(&atype, sizeof(int), 1, hFile);
		Write(vec, hFile);
	}

	void LiarMaxNodeParse::Write(Liar::VertexElementAttr atype, std::vector<IntHeapOperator*>& vec, size_t max, FILE* hFile)
	{
		size_t blockSize = sizeof(int);
		fwrite(&atype, blockSize, 1, hFile);

		Write(vec, max, hFile);
	}

	void LiarMaxNodeParse::Write(Liar::VertexElementAttr atype, std::vector<FloatHeapOperator*>& vec, size_t max, FILE* hFile)
	{
		size_t blockSize = sizeof(int);
		fwrite(&atype, blockSize, 1, hFile);

		Write(vec, max, hFile);
	}

	void LiarMaxNodeParse::Write(Liar::VertexElementAttr atype, std::vector<unsigned int>& vec, FILE* hFile)
	{
		fwrite(&atype, sizeof(int), 1, hFile);
		Write(vec, hFile);
	}

	void LiarMaxNodeParse::Write(Liar::VertexElementAttr atype, std::vector<float>& vec, FILE* hFile)
	{
		fwrite(&atype, sizeof(int), 1, hFile);
		Write(vec, hFile);
	}

	void LiarMaxNodeParse::Write(std::vector<Point2>& vec, FILE* hFile)
	{
		size_t len = vec.size();
		fwrite(&len, sizeof(int), 1, hFile);
		for (size_t i = 0; i < len; ++i)
		{
			float x = vec[i].x;
			float y = vec[i].y;
			fwrite(&x, sizeof(float), 1, hFile);
			fwrite(&y, sizeof(float), 1, hFile);
		}
	}

	void LiarMaxNodeParse::Write(std::vector<Point3>& vec, FILE* hFile)
	{
		size_t len = vec.size();
		fwrite(&len, sizeof(int), 1, hFile);
		for (size_t i = 0; i < len; ++i)
		{
			float x = vec[i].x;
			float y = vec[i].y;
			float z = vec[i].z;
			fwrite(&x, sizeof(float), 1, hFile);
			fwrite(&y, sizeof(float), 1, hFile);
			fwrite(&z, sizeof(float), 1, hFile);
		}
	}

	void LiarMaxNodeParse::Write(std::vector<Point4>& vec, FILE* hFile, bool transInt)
	{
		size_t len = vec.size();
		fwrite(&len, sizeof(int), 1, hFile);
		if (transInt)
		{
			size_t strip = sizeof(int);
			for (size_t i = 0; i < len; ++i)
			{
				int x = static_cast<int>(vec[i].x);
				int y = static_cast<int>(vec[i].y);
				int z = static_cast<int>(vec[i].z);
				int w = static_cast<int>(vec[i].w);
				fwrite(&x, strip, 1, hFile);
				fwrite(&y, strip, 1, hFile);
				fwrite(&z, strip, 1, hFile);
				fwrite(&w, strip, 1, hFile);
			}
		}
		else
		{
			size_t strip = sizeof(float);
			for (size_t i = 0; i < len; ++i)
			{
				float x = vec[i].x;
				float y = vec[i].y;
				float z = vec[i].z;
				float w = vec[i].w;
				fwrite(&x, strip, 1, hFile);
				fwrite(&y, strip, 1, hFile);
				fwrite(&z, strip, 1, hFile);
				fwrite(&w, strip, 1, hFile);
			}
		}
	}

	void LiarMaxNodeParse::Write(std::vector<Quat>& vec, FILE* hFile)
	{
		size_t len = vec.size();
		fwrite(&len, sizeof(int), 1, hFile);
		size_t strip = sizeof(float);
		for (size_t i = 0; i < len; ++i)
		{
			float x = vec[i].x;
			float y = vec[i].y;
			float z = vec[i].z;
			float w = vec[i].w;
			fwrite(&x, strip, 1, hFile);
			fwrite(&y, strip, 1, hFile);
			fwrite(&z, strip, 1, hFile);
			fwrite(&w, strip, 1, hFile);
		}
	}

	void LiarMaxNodeParse::Write(std::vector<IntHeapOperator*>& vec, size_t max,  FILE* hFile)
	{
		size_t blockSize = sizeof(int);

		// 数组长度
		size_t len = vec.size();
		fwrite(&len, blockSize, 1, hFile);

		// 单个长度
		fwrite(&max, blockSize, 1, hFile);
		for (size_t i = 0; i < len; ++i)
		{
			IntHeapOperator* tmp = vec[i];
			for (size_t j = 0; j < max; ++j)
			{
				int tmpVal = (*tmp)[j];
				fwrite(&tmpVal, blockSize, 1, hFile);
			}
		}
	}

	void LiarMaxNodeParse::Write(std::vector<FloatHeapOperator*>& vec, size_t max, FILE* hFile)
	{
		size_t blockSize = sizeof(int);

		// 数组长度
		size_t len = vec.size();
		fwrite(&len, blockSize, 1, hFile);

		// 单个长度
		fwrite(&max, blockSize, 1, hFile);
		blockSize = sizeof(float);
		for (size_t i = 0; i < len; ++i)
		{
			FloatHeapOperator* tmp = vec[i];
			for (size_t j = 0; j < max; ++j)
			{
				float tmpVal = (*tmp)[j];
				fwrite(&tmpVal, blockSize, 1, hFile);
			}
		}
	}

	void LiarMaxNodeParse::Write(std::vector<int>& vec, FILE* hFile)
	{
		size_t len = vec.size();
		fwrite(&len, sizeof(int), 1, hFile);
		for (size_t i = 0; i < len; ++i)
		{
			int x = vec[i];
			fwrite(&x, sizeof(int), 1, hFile);
		}
	}

	void LiarMaxNodeParse::Write(std::vector<unsigned int>& vec, FILE* hFile)
	{
		size_t len = vec.size();
		fwrite(&len, sizeof(int), 1, hFile);
		for (size_t i = 0; i < len; ++i)
		{
			unsigned int x = vec[i];
			fwrite(&x, sizeof(unsigned int), 1, hFile);
		}
	}

	void LiarMaxNodeParse::Write(std::vector<float>& vec, FILE* hFile)
	{
		size_t len = vec.size();
		fwrite(&len, sizeof(int), 1, hFile);
		for (size_t i = 0; i < len; ++i)
		{
			float x = vec[i];
			fwrite(&x, sizeof(float), 1, hFile);
		}
	}

	void LiarMaxNodeParse::Write(Liar::LiarPluginCfg* ctr, std::vector<IGameMaterial*>& vec, int meshSize)
	{
		std::string path = ctr->name;
		FILE* hFile = fopen(path.c_str(), "wb");

		// 包含mesh数量
		fwrite(&meshSize, sizeof(int), 1, hFile);

		// 材质
		size_t len = vec.size();
		fwrite(&len, sizeof(int), 1, hFile);
		for (size_t i = 0; i < len; ++i)
		{
			Write(vec[i], hFile);
		}

		fclose(hFile);
	}

	void LiarMaxNodeParse::Write(IGameMaterial* tmpGameMaterial, FILE* hFile)
	{
		size_t texLen = GetTextureSize(tmpGameMaterial);
		fwrite(&texLen, sizeof(int), 1, hFile);

		size_t tmpNumberOfTextureMaps = tmpGameMaterial->GetNumberOfTextureMaps();		//how many texture of the material
		for (int tmpTextureMapIndex = 0; tmpTextureMapIndex < tmpNumberOfTextureMaps; tmpTextureMapIndex++)
		{
			IGameTextureMap* tmpGameTextureMap = tmpGameMaterial->GetIGameTextureMap(tmpTextureMapIndex);
			if (tmpGameTextureMap != NULL)
			{
				std::string texCls;
				Liar::LiarMaxNodeParse::GetTChar2Char(tmpGameTextureMap->GetTextureClass(), texCls);
				Liar::LiarMaxNodeParse::StringToUpper(texCls);
				if (strcmp(texCls.c_str(), "BITMAP") == 0)
				{
					//文件路径
					std::string tmpBitmapPath("");
					Liar::LiarMaxNodeParse::GetTChar2Char(tmpGameTextureMap->GetBitmapFileName(), tmpBitmapPath);

					if (tmpBitmapPath != "")
					{
						// 获取类型
						int mapSlot = tmpGameTextureMap->GetStdMapSlot();
						//文件名
						tmpBitmapPath = Liar::LiarMaxNodeParse::GetLast(tmpBitmapPath);

						fwrite(&mapSlot, sizeof(int), 1, hFile);
						Write(tmpBitmapPath, hFile);
					}
				}
			}
		}
	}

	void LiarMaxNodeParse::Write(const std::string& s, FILE* hFile)
	{
		size_t szStr = s.size();
		fwrite(&szStr, sizeof(int), 1, hFile);
		fwrite(&s.front(), sizeof(char), szStr, hFile);
	}

	size_t LiarMaxNodeParse::GetTextureSize(IGameMaterial* tmpGameMaterial)
	{
		size_t len = 0;
		size_t tmpNumberOfTextureMaps = tmpGameMaterial->GetNumberOfTextureMaps();		//how many texture of the material
		for (int tmpTextureMapIndex = 0; tmpTextureMapIndex < tmpNumberOfTextureMaps; tmpTextureMapIndex++)
		{
			IGameTextureMap* tmpGameTextureMap = tmpGameMaterial->GetIGameTextureMap(tmpTextureMapIndex);
			if (tmpGameTextureMap != NULL)
			{
				std::string texCls;
				Liar::LiarMaxNodeParse::GetTChar2Char(tmpGameTextureMap->GetTextureClass(), texCls);
				Liar::LiarMaxNodeParse::StringToUpper(texCls);
				if (strcmp(texCls.c_str(), "BITMAP") == 0)
				{
					//文件路径
					std::string tmpBitmapPath("");
					Liar::LiarMaxNodeParse::GetTChar2Char(tmpGameTextureMap->GetBitmapFileName(), tmpBitmapPath);

					if (tmpBitmapPath != "")
					{
						len++;
					}
				}
			}
		}
		return len;
	}

	void LiarMaxNodeParse::RevertYZ(Point3& point)
	{
		point.x = point.x;
		point.y = point.z;
		point.z = point.y;
	}

	void LiarMaxNodeParse::RevertYZ(Point2& point)
	{
		point.x = point.x;
		point.y = 1 - point.y;
	}

}
