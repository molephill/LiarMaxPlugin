﻿#include "LiarMaxNodeParse.h"
#include <LiarStringUtil.h>
#include <LiarPluginUtil.h>
#include <PluginDefine.h>

#include <fstream>

namespace Liar
{

	LiarMaxNodeParse::LiarMaxNodeParse():m_rootNode(nullptr), m_anim(nullptr)
	{
		m_allMeshs = new std::vector<Liar::LiarMesh*>();
		m_allMats = new std::vector<Liar::LiarMaterialNode*>();
	}


	LiarMaxNodeParse::~LiarMaxNodeParse()
	{
		for (std::vector<Liar::LiarMesh*>::iterator it = m_allMeshs->begin(); it < m_allMeshs->end();)
		{
			delete *it;
			it = m_allMeshs->erase(it);
		}
		std::vector<Liar::LiarMesh*>().swap(*m_allMeshs);
		delete m_allMeshs;
		m_allMeshs = nullptr;

		for (std::vector<Liar::LiarMaterialNode*>::iterator it = m_allMats->begin(); it < m_allMats->end();)
		{
			delete *it;
			it = m_allMats->erase(it);
		}
		std::vector<Liar::LiarMaterialNode*>().swap(*m_allMats);
		delete m_allMats;
		m_allMats = nullptr;

		
	}

	void LiarMaxNodeParse::ParseMatNodes(IGameScene* tmpGameScene)
	{
		int matCount = tmpGameScene->GetRootMaterialCount();
		for (int i = 0; i < matCount; ++i)
		{
			IGameMaterial* tmpGameMaterial = tmpGameScene->GetRootMaterial(i);
			if (tmpGameMaterial)
			{
				Liar::LiarMaterialNode* liarNode = new Liar::LiarMaterialNode();
				liarNode->node = tmpGameMaterial;
				liarNode->index = i;
				m_allMats->push_back(liarNode);

				Liar::StringUtil::GetWSTR2Char(tmpGameMaterial->GetMaterialName(), liarNode->name);
				Liar::StringUtil::GetWSTR2Char(tmpGameMaterial->GetMaterialClass(), liarNode->type);

				size_t tmpNumberOfTextureMaps = tmpGameMaterial->GetNumberOfTextureMaps();		//how many texture of the material
				for (int tmpTextureMapIndex = 0; tmpTextureMapIndex < tmpNumberOfTextureMaps; tmpTextureMapIndex++)
				{
					IGameTextureMap* tmpGameTextureMap = tmpGameMaterial->GetIGameTextureMap(tmpTextureMapIndex);
					if (tmpGameTextureMap != NULL)
					{
						std::string texCls;
						Liar::StringUtil::GetTChar2Char(tmpGameTextureMap->GetTextureClass(), texCls);
						Liar::StringUtil::StringToUpper(texCls);
						if (strcmp(texCls.c_str(), "BITMAP") == 0)
						{
							//文件路径
							std::string tmpBitmapPath("");
							Liar::StringUtil::GetTChar2Char(tmpGameTextureMap->GetBitmapFileName(), tmpBitmapPath);

							if (tmpBitmapPath != "")
							{
								Liar::LiarTexture* liarTex = new Liar::LiarTexture(true);
								liarNode->texs->push_back(liarTex);

								//文件名
								tmpBitmapPath = Liar::StringUtil::GetLast(tmpBitmapPath);
								liarTex->GetTexContext()->SetPath(tmpBitmapPath);

								// 获取类型
								int mapSlot = tmpGameTextureMap->GetStdMapSlot();
								liarTex->SetType(mapSlot);
							}
						}
					}
				}
			}
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

		ParseMatNodes(tmpGameScene);

		m_rootNode = new Liar::LiarNode();
		m_rootNode->SetNodeName("root");

		m_allBones = new std::vector<Liar::LiarBone*>();
		for (int i = 0; i < tmpTopLevelNodeCount; i++)
		{
			IGameNode* tmpGameNode = tmpGameScene->GetTopLevelNode(i);
			ParseBones(ctr, tmpGameNode);
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

	Liar::LiarBone* LiarMaxNodeParse::GetLiarBone(IGameNode* node)
	{
		for (std::vector<Liar::LiarBone*>::const_iterator cit = m_allBones->begin(); cit != m_allBones->end(); ++cit)
		{
			if ((*cit)->Equal(node))
			{
				return *cit;
			}
		}
		return nullptr;
	}

	void LiarMaxNodeParse::ParseBones(Liar::LiarPluginCfg* ctr, IGameNode* varGameNode)
	{
		if (ctr->skeleton)
		{
			IGameObject::ObjectTypes tmpObjectType = varGameNode->GetIGameObject()->GetIGameType();
			if (tmpObjectType == IGameObject::IGAME_BONE)
			{
				Liar::LiarBone* findBone = GetLiarBone(varGameNode);
				if (!findBone)
				{
					Liar::LiarBone* liarBone = new Liar::LiarBone();
					Liar::StringUtil::GetWSTR2Char(varGameNode->GetName(), liarBone->name);
					liarBone->node = varGameNode;

					IGameNode* parent = varGameNode->GetNodeParent();

					if (!parent)
					{
						liarBone->id = -1;
						liarBone->parentId = -1;
					}
					else
					{
						Liar::LiarBone* liarParent = GetLiarBone(parent);
						liarBone->id = static_cast<int>(m_allBones->size());
						liarBone->parentId = liarParent ? liarParent->id : -1;
					}

					m_allBones->push_back(liarBone);
				}
			}

			int tmpChildCount = varGameNode->GetChildCount();
			for (int i = 0; i < tmpChildCount; i++)
			{
				IGameNode* subGameNode = varGameNode->GetNodeChild(i);
				ParseBones(ctr, subGameNode);
			}
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
			Liar::StringUtil::GetWSTR2Char(varGameNode->GetName(), node->GetNodeName());
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
		Liar::LiarMesh* liarMesh = new Liar::LiarMesh();
		m_allMeshs->push_back(liarMesh);
		++index;

		//bool revertYZ = ctr->coordSystemType == IGameConversionManager::CoordSystem::Max3DS ? false : true;
		bool revertYZ = false;

		Liar::Vector3D* tmp = new Liar::Vector3D();
		std::vector<IGameMaterial*> materials;

		int tmpFaceCount = tmpGameMesh->GetNumberOfFaces();
		for (int tmpFaceIndex = 0; tmpFaceIndex < tmpFaceCount; tmpFaceIndex++)
		{
			FaceEx* tmpFaceEx = tmpGameMesh->GetFace(tmpFaceIndex);
			for (int tmpFaceVertexIndex = 0; tmpFaceVertexIndex < 3; tmpFaceVertexIndex++)
			{
				//顶点
				unsigned long tmpVertexIndex = tmpFaceEx->vert[tmpFaceVertexIndex];
				unsigned long tmpNormalIndex = 0;
				unsigned long tmpColorIndex = 0;
				unsigned long tmpUVIndex = 0;

				Liar::Vector3D* pos = liarMesh->GetGeo()->GetRawData()->AddPos(tmpVertexIndex);
				Liar::LiarStructUtil::ParsePoint3(pos, tmpGameMesh->GetVertex(tmpVertexIndex), revertYZ);

				//Normal
				if (ctr->vertexNormal)
				{
					tmpNormalIndex = tmpFaceEx->norm[tmpFaceVertexIndex];
					Liar::LiarStructUtil::ParsePoint3(tmp, tmpGameMesh->GetNormal(tmpNormalIndex), revertYZ);
					tmpNormalIndex = liarMesh->GetGeo()->GetRawData()->GetIndex(LIAR_NORMAL, *tmp);
					Liar::Vector3D* norm = liarMesh->GetGeo()->GetRawData()->AddNorm(tmpNormalIndex);
					norm->Copy(*tmp);
				}

				// color
				if (ctr->vertexColor)
				{
					tmpColorIndex = tmpFaceEx->color[tmpFaceVertexIndex];
					Liar::LiarStructUtil::ParsePoint3(tmp, tmpGameMesh->GetColorVertex(tmpColorIndex), revertYZ);
					tmpColorIndex = liarMesh->GetGeo()->GetRawData()->GetIndex(LIAR_COLOR, *tmp);
					Liar::Vector3D* color = liarMesh->GetGeo()->GetRawData()->AddColor(tmpColorIndex);
					color->Copy(*tmp);
				}

				// uv
				if (ctr->textureCoord)
				{
					tmpUVIndex = tmpFaceEx->texCoord[tmpFaceVertexIndex];
					Liar::LiarStructUtil::ParsePoint3(tmp, tmpGameMesh->GetTexVertex(tmpUVIndex), ctr->revertUV);
					tmpUVIndex = liarMesh->GetGeo()->GetRawData()->GetIndex(LIAR_UV, *tmp);
					Liar::Vector3D* texCoord = liarMesh->GetGeo()->GetRawData()->AddTex(tmpUVIndex);
					texCoord->Copy(*tmp);
				}

				// 顶点索引
				size_t curVetexIndex = liarMesh->GetGeo()->FindVertexDefineIndex(tmpVertexIndex, tmpNormalIndex, tmpUVIndex, tmpColorIndex);
				if (curVetexIndex == -1)
				{
					// 顶点索引
					curVetexIndex = liarMesh->GetGeo()->GetVertexFaceSize();
					Liar::LiarVertexDefine* find = new Liar::LiarVertexDefine(tmpVertexIndex, tmpNormalIndex, tmpUVIndex, tmpColorIndex);
					liarMesh->GetGeo()->GetVertexFaces()->push_back(find);
				}

				liarMesh->GetGeo()->GetIndices()->push_back(static_cast<unsigned int>(curVetexIndex));
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
				}
			}
		}

		delete tmp;

		for (std::vector<IGameMaterial*>::iterator it = materials.begin(); it != materials.end(); ++it)
		{
			IGameMaterial* tmpGameMaterial = *it;
			Liar::LiarMaterial* liarMaterial = new Liar::LiarMaterial();
			Liar::StringUtil::GetWSTR2Char(tmpGameMaterial->GetMaterialClass(), liarMaterial->GetType());
			liarMesh->GetMatrials()->push_back(liarMaterial);
			Liar::StringUtil::GetWSTR2Char(tmpGameMaterial->GetMaterialName(), liarMaterial->name);
			size_t tmpNumberOfTextureMaps = tmpGameMaterial->GetNumberOfTextureMaps();		//how many texture of the material
			for (int tmpTextureMapIndex = 0; tmpTextureMapIndex < tmpNumberOfTextureMaps; tmpTextureMapIndex++)
			{
				IGameTextureMap* tmpGameTextureMap = tmpGameMaterial->GetIGameTextureMap(tmpTextureMapIndex);
				if (tmpGameTextureMap != NULL)
				{
					std::string texCls;
					Liar::StringUtil::GetTChar2Char(tmpGameTextureMap->GetTextureClass(), texCls);
					Liar::StringUtil::StringToUpper(texCls);
					if (strcmp(texCls.c_str(), "BITMAP") == 0)
					{
						//文件路径
						std::string tmpBitmapPath("");
						Liar::StringUtil::GetTChar2Char(tmpGameTextureMap->GetBitmapFileName(), tmpBitmapPath);

						if (tmpBitmapPath != "")
						{
							Liar::LiarTexture* liarTex = new Liar::LiarTexture(true);
							liarMaterial->GetTextures()->push_back(liarTex);

							//文件名
							tmpBitmapPath = Liar::StringUtil::GetLast(tmpBitmapPath);
							liarTex->GetTexContext()->SetPath(tmpBitmapPath);

							// 获取类型
							int mapSlot = tmpGameTextureMap->GetStdMapSlot();
							liarTex->SetType(mapSlot);
						}
					}
				}
			}
		}

		ParseSkin(ctr, tmpGameMesh, liarMesh);

	}

	void LiarMaxNodeParse::ParseSkin(Liar::LiarPluginCfg* ctr, IGameMesh* tmpGameMesh, Liar::LiarMesh* liarMesh)
	{
		//判断有没有修改器，有修改器的就是骨骼动画
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
					//获取当前顶点的骨骼
					size_t type = tmpGameSkin->GetVertexType(i);
					if (type == IGameSkin::IGAME_RIGID)
					{
						INode* bNode = tmpGameSkin->GetBone(i, 0);
						Liar::LiarBone* liarBone = GetLiarBone((IGameNode*)bNode);
						if (liarBone)
						{
							Liar::LiarAnimSkinDefine* skinDefine = liarMesh->GetGeo()->GetRawData()->GetAnimSkinDefine(i);
							Liar::LiarSkin* liarSkin = skinDefine->AddSkin(liarBone->id, 1.0f);
							liarSkin->node = bNode;
							//liarSkin->bonId = tmpGameSkin->GetBoneID(i, 0);
						}
					}
					else //blended
					{
						int boneNum = tmpGameSkin->GetNumberOfBones(i);
						if (boneNum > 0)
						{
							Liar::LiarAnimSkinDefine* skinDefine = liarMesh->GetGeo()->GetRawData()->GetAnimSkinDefine(i);
							for (int j = 0; j < boneNum; ++j)
							{
								INode* bNode = tmpGameSkin->GetBone(i, j);
								Liar::LiarBone* liarBone = GetLiarBone((IGameNode*)bNode);
								if (liarBone)
								{
									float weight = tmpGameSkin->GetWeight(i, j);
									Liar::LiarSkin* liarSkin = skinDefine->AddSkin(liarBone->id, weight);
									liarSkin->node = bNode;
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
		IGameScene* tmpGameScene = GetIGameInterface();
		TimeValue tmpTimeValueBegin = tmpGameScene->GetSceneStartTime();
		TimeValue tmpTimeValueEnd = tmpGameScene->GetSceneEndTime();
		TimeValue tmpTimeValueTicks = tmpGameScene->GetSceneTicks();
		int tmpFrameCount = (tmpTimeValueEnd - tmpTimeValueBegin) / tmpTimeValueTicks;

		m_anim = new Liar::LiarSkeletonAnim;

		bool revertYZ = false;
		for (int i = 0; i<tmpFrameCount; i++)
		{
			for (int j = 0; j<m_allBones->size(); ++j)
			{
				IGameNode* node = m_allBones->at(j)->node;
				LiarBone* liarBone = GetLiarBone(node);

				if (liarBone)
				{
					GMatrix gm = node->GetLocalTM(i*tmpTimeValueTicks);
					Point3 pos = gm.Translation();
					Point3 rota = gm.Rotation().Vector();
					Point3 scale = gm.Scaling();
					
					
					if (pos.x != 0.0f || pos.y != 0.0f || pos.z != 0.0f)
					{
						Liar::LiarKeyFrame* keyFrame = m_anim->GetPositionKey(i, true);
						Liar::LiarStructUtil::ParsePoint3(keyFrame->keyVec, pos, revertYZ);
					}

					if (rota.x != 0.0f || rota.y != 0.0f || rota.z != 0.0f)
					{
						Liar::LiarKeyFrame* keyFrame = m_anim->GetRotationKey(i, true);
						Liar::LiarStructUtil::ParsePoint3(keyFrame->keyVec, rota, revertYZ);
					}

					if (scale.x != 1.0f || scale.y != 1.0f || scale.z != 1.0f)
					{
						Liar::LiarKeyFrame* keyFrame = m_anim->GetScaleKey(i, true);
						Liar::LiarStructUtil::ParsePoint3(keyFrame->keyVec, scale, revertYZ);
					}
				}
			}
		}
	}

}
