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

	void LiarMaxNodeParse::ParseBones(Liar::LiarPluginCfg* ctr, IGameNode* varGameNode)
	{
		bool revertYZ = false;
		IGameObject::ObjectTypes tmpObjectType = varGameNode->GetIGameObject()->GetIGameType();
		std::string boneName;
		if (tmpObjectType == IGameObject::IGAME_BONE)
		{
			/*LiarStructUtil::GetWSTR2Char(varGameNode->GetName(), boneName);
			Liar::LiarBone* liarBone = m_skeleton->GetBone(boneName, true);
			LiarStructUtil::GetWSTR2Char(varGameNode->GetName(), liarBone->GetName());


			IGameNode* parent = varGameNode->GetNodeParent();

			if (!parent)
			{
			liarBone->SetId(-1);
			liarBone->SetParentId(-1);
			}
			else
			{
			LiarStructUtil::GetWSTR2Char(parent->GetName(), boneName);
			Liar::LiarBone* liarParent = m_skeleton->GetBone(boneName);
			liarBone->SetId(static_cast<int>(m_skeleton->GetBoneSize()));
			int parentId = liarParent ? liarParent->GetId() : -1;
			liarBone->SetParentId(parentId);
			}

			LiarMaxBone* liarMaxBone = new LiarMaxBone();
			liarMaxBone->boneId = liarBone->GetId();
			liarMaxBone->node = varGameNode;
			m_allBones->push_back(liarMaxBone);

			GMatrix gm = varGameNode->GetLocalTM(0);
			Point3 pos = gm.Translation();
			Quat rota = gm.Rotation();
			Point3 scale = gm.Scaling();
			Liar::LiarStructUtil::ParsePoint3(liarBone->GetPosition(), pos, revertYZ);
			Liar::LiarStructUtil::ParsePoint4(liarBone->GetRotation(), rota);
			Liar::LiarStructUtil::ParsePoint3(liarBone->GetScale(), scale, revertYZ);*/
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
			/*m_skeleton = new Liar::LiarSkeleton();
			for (int i = 0; i < tmpTopLevelNodeCount; i++)
			{
				IGameNode* tmpGameNode = tmpGameScene->GetTopLevelNode(i);
				ParseBones(ctr, tmpGameNode);
			}*/
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

	void LiarMaxNodeParse::ParseGeometory(Liar::LiarPluginCfg* ctr, IGameMesh* tmpGameMesh, 
		std::vector<IGameMaterial*>& materials, int& meshSize)
	{

		//bool revertYZ = ctr->coordSystemType == IGameConversionManager::CoordSystem::Max3DS ? false : true;
		bool revertYZ = false;

		std::vector<Point3> positions;
		std::vector<Point3> normals;
		std::vector<Point2> texCoords;
		std::vector<Point3> colors;
		std::vector<Point4> keys;
		std::vector<unsigned int> indices;
		std::vector<unsigned int> materialIndics;
		unsigned int curVetexIndex;
		Point4 tmp4;

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

				tmpVertexIndex = AddP3(positions, tmpGameMesh->GetVertex(tmpVertexIndex), revertYZ);

				//Normal
				if (ctr->vertexNormal)
				{
					tmpNormalIndex = tmpFaceEx->norm[tmpFaceVertexIndex];
					tmpNormalIndex = AddP3(normals, tmpGameMesh->GetNormal(tmpNormalIndex), revertYZ);
				}

				// color
				if (ctr->vertexColor)
				{
					tmpColorIndex = tmpFaceEx->color[tmpFaceVertexIndex];
					tmpColorIndex = AddP3(colors, tmpGameMesh->GetColorVertex(tmpColorIndex), revertYZ);
				}

				// uv
				if (ctr->textureCoord)
				{
					tmpUVIndex = tmpFaceEx->texCoord[tmpFaceVertexIndex];
					tmpUVIndex = AddP2(texCoords, tmpGameMesh->GetTexVertex(tmpUVIndex), revertYZ);
				}

				// 顶点索引
				tmp4.Set(tmpVertexIndex, tmpNormalIndex, tmpUVIndex, tmpColorIndex);
				curVetexIndex = AddP4(keys, tmp4);
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

		ParseSkin(ctr, tmpGameMesh);

		// 开始写入
		std::string path = ctr->name;
		std::string folder, last;
		Liar::LiarMaxNodeParse::GetHeadAndLast(path, folder, last, "\\");

		std::string baseName = Liar::LiarMaxNodeParse::GetHead(last);
		std::string meshName = baseName + std::to_string(meshSize++) + Liar::MESH_EXT;
		char fullName[MAX_PATH];
		sprintf_s(fullName, "%s\\%s", folder.c_str(), meshName.c_str());
		FILE* hFile = fopen(fullName, "wb");
		// geometry类型
		Liar::GeometryVertexType type = GetVertexType(ctr);
		fwrite(&type, sizeof(int), 1, hFile);
		// 顶点类型
		Write(Liar::VertexElementAttr::ELEMENT_ATTR_POSITION, positions, hFile);
		Write(Liar::VertexElementAttr::ELEMENT_ATTR_NORMAL, normals, hFile);
		Write(Liar::VertexElementAttr::ELEMENT_ATTR_TEXTURECOORDINATE, texCoords, hFile);
		Write(Liar::VertexElementAttr::ELEMENT_ATTR_COLOR, colors, hFile);
		// 索引
		Write(Liar::VertexElementAttr::ELEMENT_ATTR_RAW_KEY, keys, hFile);
		// 顶点
		Write(Liar::VertexElementAttr::ELEMENT_ATTR_RAW_INDICES, indices, hFile);
		// 材质引用
		//Write(Liar::VertexElementAttr::ELEMENT_ATTR_RAW_MTL_INDICES, materialIndics, hFile);
		size_t mltLen = materialIndics.size();
		size_t mltIndex = -1;
		if (mltLen > 0) mltIndex = materialIndics[0];
		fwrite(&mltIndex, sizeof(int), 1, hFile);
		fclose(hFile);
	}

	void LiarMaxNodeParse::ParseSkin(Liar::LiarPluginCfg* ctr, IGameMesh* tmpGameMesh)
	{
		if (!ctr->exportModifier) return;

		//判断有没有修改器，有修改器的就是骨骼动画
		//std::string boneName;
		//std::string modfierName;
		//int tmpModifiersNum = tmpGameMesh->GetNumModifiers();
		//for (int tmpModifierIndex = 0; tmpModifierIndex < tmpModifiersNum; tmpModifierIndex++)
		//{
		//	IGameModifier* tmpGameModifier = tmpGameMesh->GetIGameModifier(tmpModifierIndex);
		//	LiarStructUtil::GetWSTR2Char(tmpGameModifier->GetUIName(), modfierName);

		//	//只处理骨骼动画修改器
		//	if (tmpGameModifier->IsSkin())
		//	{
		//		IGameSkin* tmpGameSkin = (IGameSkin*)tmpGameModifier;
		//		//int tmpNumOfSkinnedVerts = tmpGameSkin->GetNumOfSkinnedVerts();

		//		//获取顶点受骨骼影响数
		//		int vertSum = tmpGameMesh->GetNumberOfVerts();
		//		for (int i = 0; i < vertSum; ++i)
		//		{
		//			//获取当前顶点的骨骼
		//			size_t type = tmpGameSkin->GetVertexType(i);
		//			if (type == IGameSkin::IGAME_RIGID)
		//			{
		//				INode* bNode = tmpGameSkin->GetBone(i, 0);
		//				LiarStructUtil::GetWSTR2Char(bNode->GetName(), boneName);
		//				Liar::LiarBone* liarBone = m_skeleton->GetBone(boneName);
		//				if (liarBone)
		//				{
		//					Liar::LiarAnimSkinDefine* skinDefine = meshData->GetAnimSkinDefine(i, true);
		//					skinDefine->AddBoneInfo(liarBone->GetId(), 1.0f);
		//				}
		//			}
		//			else //blended
		//			{
		//				int boneNum = tmpGameSkin->GetNumberOfBones(i);
		//				if (boneNum > 0)
		//				{
		//					Liar::LiarAnimSkinDefine* skinDefine = meshData->GetAnimSkinDefine(i, true);
		//					for (int j = 0; j < boneNum; ++j)
		//					{
		//						INode* bNode = tmpGameSkin->GetBone(i, j);
		//						LiarStructUtil::GetWSTR2Char(bNode->GetName(), boneName);
		//						Liar::LiarBone* liarBone = m_skeleton->GetBone(boneName);
		//						if (liarBone)
		//						{
		//							float weight = tmpGameSkin->GetWeight(i, j);
		//							skinDefine->AddBoneInfo(liarBone->GetId(), weight);
		//						}
		//					}
		//				}
		//			}
		//		}

		//	}
		//}

	}


	void LiarMaxNodeParse::ParseAnim(Liar::LiarPluginCfg* ctr)
	{
		/*if (!ctr->exportAnim) return;

		IGameScene* tmpGameScene = GetIGameInterface();
		TimeValue tmpTimeValueBegin = tmpGameScene->GetSceneStartTime();
		TimeValue tmpTimeValueEnd = tmpGameScene->GetSceneEndTime();
		TimeValue tmpTimeValueTicks = tmpGameScene->GetSceneTicks();
		int tmpFrameCount = (tmpTimeValueEnd - tmpTimeValueBegin) / tmpTimeValueTicks;

		m_anim = new Liar::LiarSkeletonAnim;
		m_anim->SetTickPerFrame(tmpTimeValueTicks);

		std::string boneName;

		for (int j = 0; j < m_skeleton->GetBoneSize(); ++j)
		{
			Liar::LiarBone* liarBone = m_skeleton->GetBone(j);
			IGameNode* node = GetMaxNode(liarBone->GetId());

			if (node)
			{
				Liar::LiarTrack* track = m_anim->GetTrack(liarBone->GetId(), true);

				for (int i = 0; i <= tmpFrameCount; ++i)
				{
					int nowMS = i*tmpTimeValueTicks;
					GMatrix gm = node->GetLocalTM(nowMS + tmpTimeValueBegin);
					Point3 pos = gm.Translation();
					Quat rota = gm.Rotation();
					Point3 scale = gm.Scaling();

					if (!Liar::LiarStructUtil::Equal(pos, 0.0f, 0.0f, 0.0f, Liar::EPSILON))
					{
						Liar::LiarKeyFrame* keyFrame = track->GetKeyFrame(Liar::LiarVertexAttr::LiarVertexAttr_TRANSFORM, nowMS, true);
						keyFrame->Set(pos.x, pos.y, pos.z);
					}

					if (!Liar::LiarStructUtil::Equal(rota, 0.0f, 0.0f, 0.0f, 0.0f, Liar::EPSILON))
					{
						Liar::LiarKeyFrame* keyFrame = track->GetKeyFrame(Liar::LiarVertexAttr::LiarVertexAttr_ROTATION, nowMS, true);
						keyFrame->Set(rota.x, rota.y, rota.z, rota.w);
					}

					if (!Liar::LiarStructUtil::Equal(scale, 1.0f, 1.0f, 1.0f, Liar::EPSILON))
					{
						Liar::LiarKeyFrame* keyFrame = track->GetKeyFrame(Liar::LiarVertexAttr::LiarVertexAttr_SCALE, nowMS, true);
						keyFrame->Set(scale.x, scale.y, scale.z);
					}

				}
			}
		}*/
	}

	IGameNode* LiarMaxNodeParse::GetMaxNode(int bondId)
	{
		/*for (std::vector<LiarMaxBone*>::iterator it = m_allBones->begin(); it != m_allBones->end(); ++it)
		{
			if ((*it)->boneId == bondId)
			{
				return (*it)->node;
			}
		}*/
		return nullptr;
	}

	Liar::GeometryVertexType LiarMaxNodeParse::GetVertexType(Liar::LiarPluginCfg* cfg)
	{
		if (!cfg->vertexNormal && !cfg->vertexColor && !cfg->textureCoord)
		{
			return Liar::GeometryVertexType::GEOMETRY_VERTEX_TYPE_POSITION;
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

	int LiarMaxNodeParse::AddP2(std::vector<Point2>& vec, Point2 p, bool revertYZ)
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

	int LiarMaxNodeParse::AddP3(std::vector<Point3>& vec, Point3 p, bool revertYZ)
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

	int LiarMaxNodeParse::AddP4(std::vector<Point4>& vec, Point4 p)
	{
		size_t size = vec.size();
		for (int i = 0; i < size; ++i)
		{
			if (vec[i] == p) return i;
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

	void LiarMaxNodeParse::Write(Liar::VertexElementAttr atype, std::vector<unsigned int>& vec, FILE* hFile)
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
		fwrite(&len, sizeof(unsigned int), 1, hFile);
		for (size_t i = 0; i < len; ++i)
		{
			unsigned int x = vec[i];
			fwrite(&x, sizeof(unsigned int), 1, hFile);
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
