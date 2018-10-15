#include "LiarMaxNodeParse.h"

#include <LiarStringUtil.h>
#include <LiarPluginUtil.h>
#include <PluginDefine.h>

#include <fstream>

namespace Liar
{

	LiarMaxNodeParse::LiarMaxNodeParse():m_rootNode(nullptr), m_anim(nullptr)
		, m_skeleton(nullptr)
	{
		m_allMeshData = new std::vector<Liar::LiarMeshRawData*>();
		m_allBones = new std::vector<Liar::LiarMaxBone*>();
	}


	LiarMaxNodeParse::~LiarMaxNodeParse()
	{
		for (std::vector<Liar::LiarMeshRawData*>::iterator it = m_allMeshData->begin(); it < m_allMeshData->end();)
		{
			delete *it;
			it = m_allMeshData->erase(it);
		}
		delete m_allMeshData;
		m_allMeshData = nullptr;

		for (std::vector<Liar::LiarMaxBone*>::iterator it = m_allBones->begin(); it < m_allBones->end();)
		{
			delete *it;
			it = m_allBones->erase(it);
		}
		delete m_allBones;
		m_allBones = nullptr;

		if (m_anim) delete m_anim;
		if (m_skeleton) delete m_skeleton;
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

		m_rootNode = new Liar::LiarNode();
		m_rootNode->SetNodeName("root");

		if (ctr->exportModifier)
		{
			m_skeleton = new Liar::LiarSkeleton();
			for (int i = 0; i < tmpTopLevelNodeCount; i++)
			{
				IGameNode* tmpGameNode = tmpGameScene->GetTopLevelNode(i);
				ParseBones(ctr, tmpGameNode);
			}
		}


		int meshSize = 0;
		for (int i = 0; i < tmpTopLevelNodeCount; i++)
		{
			IGameNode* tmpGameNode = tmpGameScene->GetTopLevelNode(i);
			ParseGameNode(ctr, tmpGameNode, m_rootNode, meshSize);
		}

		ParseAnim(ctr);

		tmpGameScene->ReleaseIGame();

		return TRUE;
	}

	void LiarMaxNodeParse::ParseGameNode(Liar::LiarPluginCfg* ctr, IGameNode* varGameNode, Liar::LiarNode* parentNode, int& index)
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
			ParseValidMeshNode(ctr, varGameNode, tmpObjectType, parentNode, index);
			break;
		default:
			break;
		}		
		
	}

	void LiarMaxNodeParse::ParseBones(Liar::LiarPluginCfg* ctr, IGameNode* varGameNode)
	{
		bool revertYZ = false;
		IGameObject::ObjectTypes tmpObjectType = varGameNode->GetIGameObject()->GetIGameType();
		std::string boneName;
		if (tmpObjectType == IGameObject::IGAME_BONE)
		{
			LiarStructUtil::GetWSTR2Char(varGameNode->GetName(), boneName);
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
			Liar::LiarStructUtil::ParsePoint3(liarBone->GetScale(), scale, revertYZ);
		}

		int tmpChildCount = varGameNode->GetChildCount();
		for (int i = 0; i < tmpChildCount; i++)
		{
			IGameNode* subGameNode = varGameNode->GetNodeChild(i);
			ParseBones(ctr, subGameNode);
		}
	}

	void LiarMaxNodeParse::ParseValidMeshNode(Liar::LiarPluginCfg* ctr, IGameNode* varGameNode, IGameObject::ObjectTypes type, Liar::LiarNode* parentNode, int& index)
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
			Liar::LiarNode* node = parentNode->AddChild();
			LiarStructUtil::GetWSTR2Char(varGameNode->GetName(), node->GetNodeName());
			// 解析子结点
			ParseGameMesh(ctr, tmpGameMesh, node, index);
			// 解析子结点
			ParseChildNode(ctr, varGameNode, node, index);
		}
	}

	void LiarMaxNodeParse::ParseChildNode(Liar::LiarPluginCfg* ctr, IGameNode* varGameNode, Liar::LiarNode* parent, int& index)
	{
		//遍历子节点
		int tmpChildCount = varGameNode->GetChildCount();
		for (size_t i = 0; i < tmpChildCount; i++)
		{
			IGameNode* tmpGameNode = varGameNode->GetNodeChild(static_cast<int>(i));
			ParseGameNode(ctr, tmpGameNode, parent, index);
		}
	}

	void LiarMaxNodeParse::ParseGameMesh(Liar::LiarPluginCfg* ctr, IGameMesh* tmpGameMesh, Liar::LiarNode* node, int& index)
	{
		tmpGameMesh->GetMaxMesh()->buildNormals();
		if (!tmpGameMesh->InitializeData())
		{
			return;
		}

		// 解析 Geometory
		node->meshIndex = index;
		ParseGeometory(ctr, tmpGameMesh, index);
	}

	void LiarMaxNodeParse::ParseGeometory(Liar::LiarPluginCfg* ctr, IGameMesh* tmpGameMesh, int& index)
	{
		Liar::LiarMeshRawData* meshData = new Liar::LiarMeshRawData();
		m_allMeshData->push_back(meshData);

		++index;

		//bool revertYZ = ctr->coordSystemType == IGameConversionManager::CoordSystem::Max3DS ? false : true;
		bool revertYZ = false;

		Liar::Vector3D* tmp = new Liar::Vector3D();
		std::vector<IGameMaterial*> materials;

		int tmpFaceCount = tmpGameMesh->GetNumberOfFaces();
		for (int tmpFaceIndex = 0; tmpFaceIndex < tmpFaceCount; tmpFaceIndex++)
		{
			FaceEx* tmpFaceEx = tmpGameMesh->GetFace(tmpFaceIndex);

			Liar::LiarFaceDefine* liarFace = new Liar::LiarFaceDefine();
			meshData->GetFaces()->push_back(liarFace);

			for (int tmpFaceVertexIndex = 0; tmpFaceVertexIndex < 3; tmpFaceVertexIndex++)
			{
				//顶点
				unsigned long tmpVertexIndex = tmpFaceEx->vert[tmpFaceVertexIndex];
				unsigned long tmpNormalIndex = 0;
				unsigned long tmpColorIndex = 0;
				unsigned long tmpUVIndex = 0;

				Liar::Vector3D* pos = meshData->AddPos(tmpVertexIndex);
				Liar::LiarStructUtil::ParsePoint3(pos, tmpGameMesh->GetVertex(tmpVertexIndex), revertYZ);

				//Normal
				if (ctr->vertexNormal)
				{
					tmpNormalIndex = tmpFaceEx->norm[tmpFaceVertexIndex];
					Liar::LiarStructUtil::ParsePoint3(tmp, tmpGameMesh->GetNormal(tmpNormalIndex), revertYZ);
					tmpNormalIndex = meshData->GetIndex(Liar::LiarVertexType::LiarVertexType_NORMAL, *tmp);
					Liar::Vector3D* norm = meshData->AddNorm(tmpNormalIndex);
					norm->Set(*tmp);
				}

				// color
				if (ctr->vertexColor)
				{
					tmpColorIndex = tmpFaceEx->color[tmpFaceVertexIndex];
					Liar::LiarStructUtil::ParsePoint3(tmp, tmpGameMesh->GetColorVertex(tmpColorIndex), revertYZ);
					tmpColorIndex = meshData->GetIndex(Liar::LiarVertexType::LiarVertexType_COLOR, *tmp);
					Liar::Vector3D* color = meshData->AddColor(tmpColorIndex);
					color->Set(*tmp);
				}

				// uv
				if (ctr->textureCoord)
				{
					tmpUVIndex = tmpFaceEx->texCoord[tmpFaceVertexIndex];
					Liar::LiarStructUtil::ParsePoint3(tmp, tmpGameMesh->GetTexVertex(tmpUVIndex), ctr->revertUV);
					tmpUVIndex = meshData->GetIndex(Liar::LiarVertexType::LiarVertexType_TEX, *tmp);
					Liar::Vector3D* texCoord = meshData->AddTex(tmpUVIndex);
					texCoord->Set(*tmp);
				}

				// 顶点索引
				size_t curVetexIndex = meshData->FindFaceDefineIndex(tmpVertexIndex, tmpNormalIndex, tmpUVIndex, tmpColorIndex);
				if (curVetexIndex == -1)
				{
					// 顶点索引
					curVetexIndex = meshData->GetFaces()->size();
					Liar::LiarFaceDefine* find = new Liar::LiarFaceDefine(tmpVertexIndex, tmpNormalIndex, tmpUVIndex, tmpColorIndex);
					meshData->GetFaces()->push_back(find);
				}

				meshData->GetIndices()->push_back(static_cast<unsigned int>(curVetexIndex));
			}

			//获取当前面的材质
			IGameMaterial* tmpGameMaterial = tmpGameMesh->GetMaterialFromFace(tmpFaceEx);
			if (tmpGameMaterial)
			{
				IGameMaterial* find = nullptr;
				for (std::vector<IGameMaterial*>::iterator it = materials.begin(); it != materials.end(); ++it)
				{
					if ((*it) == tmpGameMaterial)
					{
						find = tmpGameMaterial;
						break;
					}
				}
				if (!find)
				{
					materials.push_back(tmpGameMaterial);

					Liar::LiarMaterialDefine* liarMaterial = new Liar::LiarMaterialDefine();
					size_t tmpNumberOfTextureMaps = tmpGameMaterial->GetNumberOfTextureMaps();		//how many texture of the material
					for (int tmpTextureMapIndex = 0; tmpTextureMapIndex < tmpNumberOfTextureMaps; tmpTextureMapIndex++)
					{
						IGameTextureMap* tmpGameTextureMap = tmpGameMaterial->GetIGameTextureMap(tmpTextureMapIndex);
						if (tmpGameTextureMap != NULL)
						{
							std::string texCls;
							LiarStructUtil::GetTChar2Char(tmpGameTextureMap->GetTextureClass(), texCls);
							Liar::StringUtil::StringToUpper(texCls);
							if (strcmp(texCls.c_str(), "BITMAP") == 0)
							{
								//文件路径
								std::string tmpBitmapPath("");
								LiarStructUtil::GetTChar2Char(tmpGameTextureMap->GetBitmapFileName(), tmpBitmapPath);

								if (tmpBitmapPath != "")
								{
									// 获取类型
									int mapSlot = tmpGameTextureMap->GetStdMapSlot();

									if (mapSlot <= Liar::LiarTextureType::LiarTextureType_AMBIENT)
									{
										Liar::LiarTextureDefine* liarTex = new Liar::LiarTextureDefine(mapSlot);
										liarMaterial->GetTextures()->push_back(liarTex);

										//文件名
										tmpBitmapPath = Liar::StringUtil::GetLast(tmpBitmapPath);
										liarTex->SetPath(tmpBitmapPath);
									}
								}
							}
						}
					}

					if (liarMaterial->GetTexSize() > 0)
					{
						meshData->AddMaterial(liarMaterial);
					}
					else
					{
						delete liarMaterial;
					}

				}
			}
		}

		delete tmp;

		ParseSkin(ctr, tmpGameMesh, meshData);

	}

	void LiarMaxNodeParse::ParseSkin(Liar::LiarPluginCfg* ctr, IGameMesh* tmpGameMesh, Liar::LiarMeshRawData* meshData)
	{
		if (!ctr->exportModifier) return;

		//判断有没有修改器，有修改器的就是骨骼动画
		std::string boneName;
		std::string modfierName;
		int tmpModifiersNum = tmpGameMesh->GetNumModifiers();
		for (int tmpModifierIndex = 0; tmpModifierIndex < tmpModifiersNum; tmpModifierIndex++)
		{
			IGameModifier* tmpGameModifier = tmpGameMesh->GetIGameModifier(tmpModifierIndex);
			LiarStructUtil::GetWSTR2Char(tmpGameModifier->GetUIName(), modfierName);

			//只处理骨骼动画修改器
			if (tmpGameModifier->IsSkin())
			{
				IGameSkin* tmpGameSkin = (IGameSkin*)tmpGameModifier;
				//int tmpNumOfSkinnedVerts = tmpGameSkin->GetNumOfSkinnedVerts();

				//获取顶点受骨骼影响数
				int vertSum = tmpGameMesh->GetNumberOfVerts();
				for (int i = 0; i < vertSum; ++i)
				{
					//获取当前顶点的骨骼
					size_t type = tmpGameSkin->GetVertexType(i);
					if (type == IGameSkin::IGAME_RIGID)
					{
						INode* bNode = tmpGameSkin->GetBone(i, 0);
						LiarStructUtil::GetWSTR2Char(bNode->GetName(), boneName);
						Liar::LiarBone* liarBone = m_skeleton->GetBone(boneName);
						if (liarBone)
						{
							Liar::LiarAnimSkinDefine* skinDefine = meshData->GetAnimSkinDefine(i, true);
							skinDefine->AddBoneInfo(liarBone->GetId(), 1.0f);
						}
					}
					else //blended
					{
						int boneNum = tmpGameSkin->GetNumberOfBones(i);
						if (boneNum > 0)
						{
							Liar::LiarAnimSkinDefine* skinDefine = meshData->GetAnimSkinDefine(i, true);
							for (int j = 0; j < boneNum; ++j)
							{
								INode* bNode = tmpGameSkin->GetBone(i, j);
								LiarStructUtil::GetWSTR2Char(bNode->GetName(), boneName);
								Liar::LiarBone* liarBone = m_skeleton->GetBone(boneName);
								if (liarBone)
								{
									float weight = tmpGameSkin->GetWeight(i, j);
									skinDefine->AddBoneInfo(liarBone->GetId(), weight);
								}
							}
						}
					}
				}

			}
		}

	}


	void LiarMaxNodeParse::ParseAnim(Liar::LiarPluginCfg* ctr)
	{
		if (!ctr->exportAnim) return;

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
		}
	}

	IGameNode* LiarMaxNodeParse::GetMaxNode(int bondId)
	{
		for (std::vector<LiarMaxBone*>::iterator it = m_allBones->begin(); it != m_allBones->end(); ++it)
		{
			if ((*it)->boneId == bondId)
			{
				return (*it)->node;
			}
		}
		return nullptr;
	}

}
