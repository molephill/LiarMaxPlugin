#include "LiarMaxNodeParse.h"
#include <LiarStringUtil.h>
#include <LiarPluginUtil.h>
#include <PluginDefine.h>

#include <fstream>

namespace Liar
{

	LiarMaxNodeParse::LiarMaxNodeParse():m_rootNode(nullptr)
	{
		m_allMeshs = new std::vector<Liar::LiarMesh*>();
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
			return TRUE;
		}

		m_rootNode = new Liar::LiarNode;
		m_rootNode->SetNodeName("root");

		int meshSize = 0;
		for (int i = 0; i < tmpTopLevelNodeCount; i++)
		{
			IGameNode* tmpGameNode = tmpGameScene->GetTopLevelNode(i);
			ParseBones(ctr, tmpGameNode);
			ParseGameNode(ctr, tmpGameNode, m_rootNode, meshSize);
		}

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
		IGameObject::ObjectTypes tmpObjectType = varGameNode->GetIGameObject()->GetIGameType();
		if (tmpObjectType == IGameObject::IGAME_BONE && ctr->skeleton)
		{
			if (!m_allBones) m_allBones = new std::vector<IGameNode*>();
			bool find = false;
			for (std::vector<IGameNode*>::const_iterator cit = m_allBones->begin(); cit != m_allBones->end(); ++cit)
			{
				if (*cit == varGameNode)
				{
					find = true;
					return;
				}
			}

			if(!find) m_allBones->push_back(varGameNode);
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
					Liar::LiarTexture* liarTex = new Liar::LiarTexture(true);
					liarMaterial->GetTextures()->push_back(liarTex);

					//文件路径
					std::string tmpBitmapPath("");
					Liar::StringUtil::GetWSTR2Char(tmpGameTextureMap->GetBitmapFileName(), tmpBitmapPath);

					//文件名
					tmpBitmapPath = Liar::StringUtil::GetLast(tmpBitmapPath);
					liarTex->GetTexContext()->SetPath(tmpBitmapPath);

					// 获取类型
					int mapSlot = tmpGameTextureMap->GetStdMapSlot();
					liarTex->SetType(mapSlot);
					/*Liar::StringUtil::GetWSTR2Char(tmpGameTextureMap->GetTextureClass(), liarTex->GetType());
					Liar::StringUtil::StringToUpper(liarTex->GetType());*/
				}
			}
		}

	}

	void LiarMaxNodeParse::ParseAnim(Liar::LiarPluginCfg* ctr, IGameMesh* tmpGameMesh)
	{
		//判断有没有修改器，有修改器的就是骨骼动画
		//int tmpModifiersNum = tmpGameMesh->GetNumModifiers();
		//for (int tmpModifierIndex = 0; tmpModifierIndex < tmpModifiersNum; tmpModifierIndex++)
		//{
		//	IGameModifier* tmpGameModifier = tmpGameMesh->GetIGameModifier(tmpModifierIndex);

		//	//只处理骨骼动画修改器
		//	if (tmpGameModifier->IsSkin())
		//	{
		//		TimeValue tmpTimeValueBegin = mGameScene->GetSceneStartTime();
		//		TimeValue tmpTimeValueEnd = mGameScene->GetSceneEndTime();
		//		TimeValue tmpTimeValueTicks = mGameScene->GetSceneTicks();

		//		IGameSkin* tmpGameSkin = (IGameSkin*)tmpGameModifier;
		//		int tmpNumOfSkinnedVerts = tmpGameSkin->GetNumOfSkinnedVerts();

		//		//获取顶点受骨骼影响数
		//		for (int tmpVertexIndex = 0; tmpVertexIndex < tmpVectorVertex.size(); tmpVertexIndex++)
		//		{
		//			int tmpNumberOfBoneOnVertex = tmpGameSkin->GetNumberOfBones(tmpVertexIndex);


		//			map<unsigned short, float> tmpMapWeightOneVertex;
		//			for (int tmpBoneIndexOnVertex = 0; tmpBoneIndexOnVertex < tmpNumberOfBoneOnVertex; tmpBoneIndexOnVertex++)
		//			{
		//				//获取当前顶点的骨骼
		//				IGameNode* tmpGameNodeBone = tmpGameSkin->GetIGameBone(tmpVertexIndex, tmpBoneIndexOnVertex);
		//				if (tmpGameNodeBone == nullptr)
		//				{
		//					continue;
		//				}
		//				bool tmpContais = false;
		//				int tmpGameNodeBoneIndex = 0;
		//				for (tmpGameNodeBoneIndex = 0; tmpGameNodeBoneIndex < tmpVectorGameNodeBones.size(); tmpGameNodeBoneIndex++)
		//				{
		//					if (tmpVectorGameNodeBones[tmpGameNodeBoneIndex] == tmpGameNodeBone)
		//					{
		//						tmpContais = true;
		//						break;
		//					}
		//				}
		//				if (tmpContais == false)
		//				{
		//					tmpVectorGameNodeBones.push_back(tmpGameNodeBone);
		//				}


		//				float tmpWeight = tmpGameSkin->GetWeight(tmpVertexIndex, tmpBoneIndexOnVertex);

		//				tmpMapWeightOneVertex.insert(pair<unsigned short, float>((unsigned short)tmpGameNodeBoneIndex, tmpWeight));
		//			}

		//			tmpVectorWeight.push_back(tmpMapWeightOneVertex);
		//		}

		//		//获取第0帧骨骼逆矩阵
		//		for (int tmpGameNodeBoneIndex = 0; tmpGameNodeBoneIndex < tmpVectorGameNodeBones.size(); tmpGameNodeBoneIndex++)
		//		{
		//			INode* tmpNodeBone = tmpVectorGameNodeBones[tmpGameNodeBoneIndex]->GetMaxNode();
		//			Matrix3 tmpMatrix3NodeBone = tmpNodeBone->GetObjTMAfterWSM(0);

		//			tmpVectorBoneGMatrixZeroFrame.push_back(tmpMatrix3NodeBone);

		//			tmpMatrix3NodeBone.Invert();
		//			//GMatrix tmpGMatrixNodeBone(tmpMatrix3NodeBone);
		//			GMatrix tmpGMatrixNodeBoneInvert(tmpMatrix3NodeBone);

		//			tmpVectorBoneGMatrixInvert.push_back(tmpGMatrixNodeBoneInvert);

		//			//test
		//			//Matrix3 tmpTestMatrix3=tmpNodeBone->GetObjTMAfterWSM(0) * tmpMatrix3NodeBone;

		//			GMatrix tmpGMatrixNodeBoneTest(tmpMatrix3NodeBone);
		//			GMatrix tmpTest = tmpGMatrixNodeBoneTest*tmpGMatrixNodeBoneInvert;

		//			int a = 0;
		//		}

		//		//获取骨骼矩阵
		//		for (; tmpTimeValueBegin <= tmpTimeValueEnd; tmpTimeValueBegin += tmpTimeValueTicks)
		//		{
		//			vector<GMatrix> tmpVectorBoneGMatrix;
		//			for (int tmpGameNodeBoneIndex = 0; tmpGameNodeBoneIndex < tmpVectorGameNodeBones.size(); tmpGameNodeBoneIndex++)
		//			{
		//				INode* tmpNodeBone = tmpVectorGameNodeBones[tmpGameNodeBoneIndex]->GetMaxNode();
		//				Matrix3 tmpMatrix3NodeBone = tmpNodeBone->GetObjTMAfterWSM(tmpTimeValueBegin);
		//				GMatrix tmpGMatrixNodeBone(tmpMatrix3NodeBone);

		//				tmpVectorBoneGMatrix.push_back(tmpGMatrixNodeBone);
		//			}

		//			tmpMapBoneGMatrix.insert(pair<TimeValue, vector<GMatrix>>(tmpTimeValueBegin, tmpVectorBoneGMatrix));
		//		}

		//	}
		//}

		//TimeValue tmpTimeValueBegin = mGameScene->GetSceneStartTime();
		//TimeValue tmpTimeValueEnd = mGameScene->GetSceneEndTime();
		//TimeValue tmpTimeValueTicks = mGameScene->GetSceneTicks();
		//int tmpFrameCount = (tmpTimeValueEnd - tmpTimeValueBegin) / tmpTimeValueTicks;
		//tmpFrameCount = tmpFrameCount + 1;

		//int tmpGameNodeBoneSize = tmpVectorGameNodeBones.size();
		//tmpOfStreamAnim.write((char*)(&tmpGameNodeBoneSize), sizeof(tmpGameNodeBoneSize));

		//for (size_t tmpGameNodeBoneIndex = 0; tmpGameNodeBoneIndex < tmpVectorGameNodeBones.size(); tmpGameNodeBoneIndex++)
		//{
		//	const wchar_t* tmpGameNodeBoneName = tmpVectorGameNodeBones[tmpGameNodeBoneIndex]->GetName();
		//	std::wstring tmpGameNodeBoneNameWString(tmpGameNodeBoneName);
		//	std::string tmpGameNodeBoneNameString = ws2s(tmpGameNodeBoneNameWString);


		//	int tmpGameNodeBoneNameStringSize = tmpGameNodeBoneNameString.size() + 1;
		//	tmpOfStreamAnim.write((char*)(&tmpGameNodeBoneNameStringSize), sizeof(tmpGameNodeBoneNameStringSize));
		//	tmpOfStreamAnim.write(tmpGameNodeBoneNameString.c_str(), tmpGameNodeBoneNameStringSize);
		//}


		////写入第0帧逆矩阵
		//int tmpVectorBoneGMatrixInvertSize = tmpVectorBoneGMatrixInvert.size();
		//tmpOfStreamAnim.write((char*)(&tmpVectorBoneGMatrixInvertSize), sizeof(tmpVectorBoneGMatrixInvertSize));


		//for (size_t tmpBoneGMatrixInvertIndex = 0; tmpBoneGMatrixInvertIndex < tmpVectorBoneGMatrixInvert.size(); tmpBoneGMatrixInvertIndex++)
		//{

		//	GMatrix tmpBoneGMatrixInvert = tmpVectorBoneGMatrixInvert[tmpBoneGMatrixInvertIndex];


		//	glm::mat4x4 tmpMat4x4BoneGMatrixInvert;
		//	tmpMat4x4BoneGMatrixInvert[0][0] = tmpBoneGMatrixInvert[0][0]; tmpMat4x4BoneGMatrixInvert[0][1] = tmpBoneGMatrixInvert[0][1]; tmpMat4x4BoneGMatrixInvert[0][2] = tmpBoneGMatrixInvert[0][2]; tmpMat4x4BoneGMatrixInvert[0][3] = tmpBoneGMatrixInvert[0][3];
		//	tmpMat4x4BoneGMatrixInvert[1][0] = tmpBoneGMatrixInvert[1][0]; tmpMat4x4BoneGMatrixInvert[1][1] = tmpBoneGMatrixInvert[1][1]; tmpMat4x4BoneGMatrixInvert[1][2] = tmpBoneGMatrixInvert[1][2]; tmpMat4x4BoneGMatrixInvert[1][3] = tmpBoneGMatrixInvert[1][3];
		//	tmpMat4x4BoneGMatrixInvert[2][0] = tmpBoneGMatrixInvert[2][0]; tmpMat4x4BoneGMatrixInvert[2][1] = tmpBoneGMatrixInvert[2][1]; tmpMat4x4BoneGMatrixInvert[2][2] = tmpBoneGMatrixInvert[2][2]; tmpMat4x4BoneGMatrixInvert[2][3] = tmpBoneGMatrixInvert[2][3];
		//	tmpMat4x4BoneGMatrixInvert[3][0] = tmpBoneGMatrixInvert[3][0]; tmpMat4x4BoneGMatrixInvert[3][1] = tmpBoneGMatrixInvert[3][1]; tmpMat4x4BoneGMatrixInvert[3][2] = tmpBoneGMatrixInvert[3][2]; tmpMat4x4BoneGMatrixInvert[3][3] = tmpBoneGMatrixInvert[3][3];

		//	tmpOfStreamAnim.write((char*)(&tmpMat4x4BoneGMatrixInvert), sizeof(tmpMat4x4BoneGMatrixInvert));
		//}

		////写入骨骼时间轴矩阵
		//int tmpMapBoneGMatrixSize = tmpMapBoneGMatrix.size();
		//tmpOfStreamAnim.write((char*)(&tmpMapBoneGMatrixSize), sizeof(tmpMapBoneGMatrixSize));

		//for (map<TimeValue, vector<GMatrix>>::iterator tmpIterBegin = tmpMapBoneGMatrix.begin(); tmpIterBegin != tmpMapBoneGMatrix.end(); tmpIterBegin++)
		//{
		//	TimeValue tmpTimeValueCurrent = tmpIterBegin->first;

		//	tmpOfStreamAnim.write((char*)(&tmpTimeValueCurrent), sizeof(tmpTimeValueCurrent));

		//	vector<GMatrix> tmpVectorGMatrixCurrent = tmpIterBegin->second;

		//	int tmpVectorGMatrixCurrentSize = tmpVectorGMatrixCurrent.size();
		//	tmpOfStreamAnim.write((char*)(&tmpVectorGMatrixCurrentSize), sizeof(tmpVectorGMatrixCurrentSize));

		//	for (size_t tmpVectorGMatrixCurrentIndex = 0; tmpVectorGMatrixCurrentIndex < tmpVectorGMatrixCurrent.size(); tmpVectorGMatrixCurrentIndex++)
		//	{

		//		GMatrix tmpGMatrixNodeBone = tmpVectorGMatrixCurrent[tmpVectorGMatrixCurrentIndex];

		//		glm::mat4x4 tmpMat4x4BoneGMatrix;
		//		tmpMat4x4BoneGMatrix[0][0] = tmpGMatrixNodeBone[0][0]; tmpMat4x4BoneGMatrix[0][1] = tmpGMatrixNodeBone[0][1]; tmpMat4x4BoneGMatrix[0][2] = tmpGMatrixNodeBone[0][2]; tmpMat4x4BoneGMatrix[0][3] = tmpGMatrixNodeBone[0][3];
		//		tmpMat4x4BoneGMatrix[1][0] = tmpGMatrixNodeBone[1][0]; tmpMat4x4BoneGMatrix[1][1] = tmpGMatrixNodeBone[1][1]; tmpMat4x4BoneGMatrix[1][2] = tmpGMatrixNodeBone[1][2]; tmpMat4x4BoneGMatrix[1][3] = tmpGMatrixNodeBone[1][3];
		//		tmpMat4x4BoneGMatrix[2][0] = tmpGMatrixNodeBone[2][0]; tmpMat4x4BoneGMatrix[2][1] = tmpGMatrixNodeBone[2][1]; tmpMat4x4BoneGMatrix[2][2] = tmpGMatrixNodeBone[2][2]; tmpMat4x4BoneGMatrix[2][3] = tmpGMatrixNodeBone[2][3];
		//		tmpMat4x4BoneGMatrix[3][0] = tmpGMatrixNodeBone[3][0]; tmpMat4x4BoneGMatrix[3][1] = tmpGMatrixNodeBone[3][1]; tmpMat4x4BoneGMatrix[3][2] = tmpGMatrixNodeBone[3][2]; tmpMat4x4BoneGMatrix[3][3] = tmpGMatrixNodeBone[3][3];

		//		tmpOfStreamAnim.write((char*)(&tmpMat4x4BoneGMatrix), sizeof(tmpMat4x4BoneGMatrix));
		//	}
		//}

		////顶点权重信息
		//vector<map<unsigned short, float>> tmpVectorWeightNew;
		//for (size_t tmpVertexIndex = 0; tmpVertexIndex < tmpVectorWeight.size(); tmpVertexIndex++)
		//{
		//	auto tmpIter = tmpMapIndicesOldToNew.find(tmpVertexIndex);
		//	if (tmpIter != tmpMapIndicesOldToNew.end())
		//	{
		//		map<unsigned short, float> tmpMapVertexWeight;
		//		tmpVectorWeightNew.push_back(tmpMapVertexWeight);
		//	}
		//}

		//for (size_t tmpVertexIndex = 0; tmpVertexIndex < tmpVectorWeight.size(); tmpVertexIndex++)
		//{
		//	auto tmpIter = tmpMapIndicesOldToNew.find(tmpVertexIndex);
		//	if (tmpIter != tmpMapIndicesOldToNew.end())
		//	{
		//		int tmpVertexIndexNew = tmpMapIndicesOldToNew[tmpVertexIndex];

		//		map<unsigned short, float> tmpMapWeightOneVertex = tmpVectorWeight[tmpVertexIndex];
		//		tmpVectorWeightNew[tmpVertexIndexNew] = tmpMapWeightOneVertex;
		//	}
		//}

		//int tmpVectorVertexSize = tmpVectorWeightNew.size();
		//tmpOfStreamAnim.write((char*)(&tmpVectorVertexSize), sizeof(tmpVectorVertexSize));

		//for (size_t tmpVertexIndex = 0; tmpVertexIndex < tmpVectorWeightNew.size(); tmpVertexIndex++)
		//{

		//	map<unsigned short, float> tmpMapWeightOneVertex = tmpVectorWeightNew[tmpVertexIndex];

		//	int tmpMapWeightOneVertexSize = tmpMapWeightOneVertex.size();
		//	tmpOfStreamAnim.write((char*)(&tmpMapWeightOneVertexSize), sizeof(tmpMapWeightOneVertexSize));

		//	for (map<unsigned short, float>::iterator tmpIterBegin = tmpMapWeightOneVertex.begin(); tmpIterBegin != tmpMapWeightOneVertex.end(); tmpIterBegin++)
		//	{
		//		tmpOfStreamAnim.write((char*)(&tmpIterBegin->first), sizeof(tmpIterBegin->first));
		//		tmpOfStreamAnim.write((char*)(&tmpIterBegin->second), sizeof(tmpIterBegin->second));
		//	}
		//}


		////计算顶点初始位置 并存储
		//for (size_t tmpVertexIndex = 0; tmpVertexIndex < tmpVectorWeightNew.size(); tmpVertexIndex++)
		//{
		//	map<unsigned short, float> tmpMapWeightOneVertex = tmpVectorWeightNew[tmpVertexIndex];

		//	//写入当前顶点受影响的骨骼数
		//	unsigned short tmpMapWeightOneVertexSize = tmpMapWeightOneVertex.size();
		//	tmpOfStreamAnim.write((char*)(&tmpMapWeightOneVertexSize), sizeof(tmpMapWeightOneVertexSize));

		//	std::vector<glm::vec3> tmpVectorOneVertexPositionNoBone;
		//	for (map<unsigned short, float>::iterator tmpIterBegin = tmpMapWeightOneVertex.begin(); tmpIterBegin != tmpMapWeightOneVertex.end(); tmpIterBegin++)
		//	{
		//		GMatrix tmpBoneGMatrixInvert = tmpVectorBoneGMatrixInvert[tmpIterBegin->first];

		//		glm::mat4x4 tmpMat4x4BoneGMatrixInvert;
		//		tmpMat4x4BoneGMatrixInvert[0][0] = tmpBoneGMatrixInvert[0][0]; tmpMat4x4BoneGMatrixInvert[0][1] = tmpBoneGMatrixInvert[0][1]; tmpMat4x4BoneGMatrixInvert[0][2] = tmpBoneGMatrixInvert[0][2]; tmpMat4x4BoneGMatrixInvert[0][3] = tmpBoneGMatrixInvert[0][3];
		//		tmpMat4x4BoneGMatrixInvert[1][0] = tmpBoneGMatrixInvert[1][0]; tmpMat4x4BoneGMatrixInvert[1][1] = tmpBoneGMatrixInvert[1][1]; tmpMat4x4BoneGMatrixInvert[1][2] = tmpBoneGMatrixInvert[1][2]; tmpMat4x4BoneGMatrixInvert[1][3] = tmpBoneGMatrixInvert[1][3];
		//		tmpMat4x4BoneGMatrixInvert[2][0] = tmpBoneGMatrixInvert[2][0]; tmpMat4x4BoneGMatrixInvert[2][1] = tmpBoneGMatrixInvert[2][1]; tmpMat4x4BoneGMatrixInvert[2][2] = tmpBoneGMatrixInvert[2][2]; tmpMat4x4BoneGMatrixInvert[2][3] = tmpBoneGMatrixInvert[2][3];
		//		tmpMat4x4BoneGMatrixInvert[3][0] = tmpBoneGMatrixInvert[3][0]; tmpMat4x4BoneGMatrixInvert[3][1] = tmpBoneGMatrixInvert[3][1]; tmpMat4x4BoneGMatrixInvert[3][2] = tmpBoneGMatrixInvert[3][2]; tmpMat4x4BoneGMatrixInvert[3][3] = tmpBoneGMatrixInvert[3][3];

		//		glm::vec3& tmpVec3PositionZeroFrame = tmpVectorVertexOnePart[tmpVertexIndex].Position;

		//		glm::vec4 tmpVec4PositionZeroFrame;
		//		tmpVec4PositionZeroFrame.x = tmpVec3PositionZeroFrame.x;
		//		tmpVec4PositionZeroFrame.y = -tmpVec3PositionZeroFrame.z;
		//		tmpVec4PositionZeroFrame.z = tmpVec3PositionZeroFrame.y;
		//		tmpVec4PositionZeroFrame.w = 1;

		//		glm::vec4 tmpPositionNoBone = tmpMat4x4BoneGMatrixInvert * tmpVec4PositionZeroFrame;

		//		tmpOfStreamAnim.write((char*)(&tmpPositionNoBone), sizeof(tmpPositionNoBone));
		//	}
		//}
		//tmpOfStreamAnim.close();

	}


}
