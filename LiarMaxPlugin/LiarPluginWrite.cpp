#include "LiarPluginWrite.h"

namespace Liar
{
	void LiarPluginWrite::WriteModelHierarchy(Liar::LiarMaxNodeParse* parse, Liar::LiarPluginCfg* liarPluginCfg, const std::string& path)
	{
		Liar::LiarNode* rootNode = parse->GetRootNode();
		// ignore root

		size_t size = parse->GetMeshRawDataSize();
		if (size <= 0) return;

		std::string folder, last;
		Liar::StringUtil::GetHeadAndLast(path, folder, last, "\\");

		std::string baseName, ext;
		Liar::StringUtil::GetHeadAndLast(last, baseName, ext, ".");
		Liar::StringUtil::StringToLower(ext);

		//if (size == 1)
		//{
		//	if (!parse->liarPluginCfg->singleExportModel)
		//	{
		//		char fullName[MAX_PATH];
		//		sprintf_s(fullName, "%s.%s", baseName.c_str(), ext.c_str());
		//		// set mesh`s saveName
		//		SetMeshSaveName(parse, 0, baseName, fullName);
		//		return;
		//	}
		//}

		char fullName[MAX_PATH];
		sprintf_s(fullName, "%s\\%s.%s", folder.c_str(), baseName.c_str(), ext.c_str());
		FILE* hFile = fopen(fullName, "wb");

		// write ver
		fwrite(&(liarPluginCfg->pluginVersion), sizeof(unsigned int), 1, hFile);

		// get vertex_open_status
		int vetOpen = 0;
		// write vertex open status
		fwrite(&vetOpen, sizeof(int), 1, hFile);

		// set rootNode`name = baseName;
		rootNode->SetNodeName(baseName);

		WriteNode(parse, rootNode, hFile);

		fclose(hFile);
	}

	void LiarPluginWrite::WriteNode(Liar::LiarMaxNodeParse* parse, Liar::LiarNode* curNode, FILE* hFile)
	{
		int size = curNode->GetNumChildren();
		// write nodeSize;
		fwrite(&size, sizeof(int), 1, hFile);
		for (int i = 0; i < size; ++i)
		{
			char saveName[MAX_PATH];
			Liar::LiarNode* subNode = curNode->GetChildren()->at(i);

			std::string& curNodeName = curNode->GetNodeName();
			std::string& subNodeName = subNode->GetNodeName();

			Liar::StringUtil::StringToLower(curNodeName);
			Liar::StringUtil::StringToLower(subNodeName);

			sprintf_s(saveName, "%s_%s.mesh", curNodeName.c_str(), subNodeName.c_str());

			// set mesh`s saveName
			SetMeshSaveName(parse, subNode->meshIndex, subNodeName, saveName);

			// write self`s name;
			std::string szStr = saveName;
			WriteString(szStr, hFile);

			// change self`s name
			sprintf_s(saveName, "%s_%s", curNodeName.c_str(), subNodeName.c_str());
			subNode->SetNodeName(saveName);

			// write children;
			WriteNode(parse, subNode, hFile);
		}
	}

	void LiarPluginWrite::SetMeshSaveName(Liar::LiarMaxNodeParse* parse, int index, const std::string& meshName, const std::string& saveName)
	{
		size_t meshSize = parse->GetMeshRawDataSize();
		for (size_t i = 0; i < meshSize; ++i)
		{
			if (i == index)
			{
				Liar::LiarMeshRawData* meshData = parse->GetMeshRawData(i);
				meshData->saveName = saveName;
				meshData->meshName = meshName;
				return;
			}
		}
	}

	void LiarPluginWrite::WriteLiarNode(Liar::LiarMaxNodeParse* parse, Liar::LiarPluginCfg* liarPlugin, const std::string& path)
	{
		// write ModelHierarchy
		WriteModelHierarchy(parse, liarPlugin, path);

		std::string folder = Liar::StringUtil::GetHead(path, "\\");
		size_t meshSize = parse->GetMeshRawDataSize();
		for (size_t i = 0; i < meshSize; ++i)
		{
			Liar::LiarMeshRawData* meshData = parse->GetMeshRawData(i);

			WrtieLiarMeshRawData(meshData, folder, liarPlugin);
		}

		// write skeleton
		WriteLiarSkelenton(parse, liarPlugin, folder);

		// write anim
		WrtieLiarAnim(parse, liarPlugin, folder);
	}

	void LiarPluginWrite::WrtieLiarMeshRawData(Liar::LiarMeshRawData* rawData, const std::string& path, Liar::LiarPluginCfg* liarPlugin)
	{
		std::string& meshName = rawData->saveName;
		char fullName[MAX_PATH];

		sprintf_s(fullName, "%s\\%s", path.c_str(), meshName.c_str());

		FILE* hFile = fopen(fullName, "wb");

		// write ver
		fwrite(&(liarPlugin->pluginVersion), sizeof(unsigned int), 1, hFile);
		
		// write mesh`s name
		WriteString(rawData->meshName, hFile);

		// write vertex info
		LiarPluginWrite::WriteLiarVecs(rawData->GetPos(), hFile);
		LiarPluginWrite::WriteLiarVecs(rawData->GetNorm(), hFile);
		LiarPluginWrite::WriteLiarVecs(rawData->GetTexCoord(), hFile);
		LiarPluginWrite::WriteLiarVecs(rawData->GetColor(), hFile);

		// write face info
		size_t len = rawData->GetFaces()->size();
		fwrite(&len, sizeof(unsigned int), 1, hFile);
		for (size_t i = 0; i < len; ++i)
		{
			Liar::LiarFaceDefine* face = rawData->GetFaces()->at(i);
			fwrite(&(face->positionIndex), sizeof(unsigned int), 1, hFile);
			fwrite(&(face->normalIndex), sizeof(unsigned int), 1, hFile);
			fwrite(&(face->texCoordIndex), sizeof(unsigned int), 1, hFile);
			fwrite(&(face->colorIndex), sizeof(unsigned int), 1, hFile);
		}

		// write skin info
		size_t skinDefineLen = rawData->GetSkinDefineLen();
		size_t intSize = sizeof(int);
		size_t floatSize = sizeof(float);
		fwrite(&skinDefineLen, intSize, 1, hFile);
		for (int i = 0; i < skinDefineLen; ++i)
		{
			Liar::LiarAnimSkinDefine* skinDefine = rawData->GetAnimSkinDefine(i);
			// write skinDefine
			int vertIndex = skinDefine->GetPositionIndex();
			fwrite(&vertIndex, intSize, 1, hFile);

			// write bone size
			size_t boneInfoSize = skinDefine->GetBoneInfoSize();
			fwrite(&boneInfoSize, intSize, 1, hFile);

			// write bone info
			for (size_t j = 0; j < boneInfoSize; ++j)
			{
				int boneId = skinDefine->GetBoneId(j);
				float boneWeight = skinDefine->GetBoneWeight(j);
				fwrite(&boneId, intSize, 1, hFile);
				fwrite(&boneWeight, floatSize, 1, hFile);
			}
		}

		// write indices count;
		size_t indiceSize = rawData->GetIndices()->size();
		fwrite(&indiceSize, sizeof(int), 1, hFile);
		// write indices
		fwrite(&(rawData->GetIndices()->front()), sizeof(unsigned int), indiceSize, hFile);

		// write materials info
		size_t matSize = rawData->GetMatrialSize();
		// write matSize
		fwrite(&matSize, sizeof(int), 1, hFile);

		for (size_t i = 0; i < matSize; ++i)
		{
			Liar::LiarMaterialDefine* mat = rawData->GetMatrials()->at(i);
			// write texSize;
			size_t texSize = mat->GetTexSize();
			fwrite(&texSize, sizeof(int), 1, hFile);
			// write texture
			for (size_t j = 0; j < texSize; ++j)
			{
				Liar::LiarTextureDefine* tex = mat->GetTexture(j);
				// wirte name
				std::string texName = tex->GetPath();
				WriteString(texName, hFile);

				// write type
				int type = tex->GetType();
				fwrite(&type, sizeof(int), 1, hFile);
			}
		}

		fclose(hFile);
	}

	void LiarPluginWrite::WriteLiarSkelenton(Liar::LiarMaxNodeParse* parse, Liar::LiarPluginCfg* liarPlugin, const std::string& path)
	{
		if (!liarPlugin->exportModifier || !parse->GetSkeleton() || parse->GetSkeleton()->GetBoneSize() <= 0) return;
		
		char fullName[MAX_PATH];
		sprintf_s(fullName, "%s\\%s.skeleton", path.c_str(), liarPlugin->skeletonName.c_str());
		FILE* hFile = fopen(fullName, "wb");
		size_t boneLen = parse->GetSkeleton()->GetBoneSize();
		// write bone size;
		fwrite(&boneLen, sizeof(int), 1, hFile);

		size_t p3Size = sizeof(Liar::Vector3D);
		size_t p4Size = sizeof(Liar::Vector4D);
		for (size_t i = 0; i < boneLen; ++i)
		{
			// write all bone;
			Liar::LiarBone* bone = parse->GetSkeleton()->GetBone(i);
			int id = bone->GetId();
			int parentId = bone->GetParentId();
			fwrite(&id, sizeof(int), 1, hFile);
			fwrite(&parentId, sizeof(int), 1, hFile);
			Liar::LiarPluginWrite::WriteString(bone->GetName(), hFile);
			fwrite(bone->GetPosition(), p3Size, 1, hFile);
			fwrite(bone->GetRotation(), p4Size, 1, hFile);
			fwrite(bone->GetScale(), p3Size, 1, hFile);
		}
		fclose(hFile);
	}

	void LiarPluginWrite::WrtieLiarAnim(Liar::LiarMaxNodeParse* parse, Liar::LiarPluginCfg* liarPlugin, const std::string& path)
	{
		if (!liarPlugin->exportAnim) return;
		Liar::LiarSkeletonAnim* anim = parse->GetAnim();
		if (!anim || anim->GetTrackLen() <= 0) return;

		char fullName[MAX_PATH];
		sprintf_s(fullName, "%s\\%s.anim", path.c_str(), liarPlugin->animName.c_str());
		FILE* hFile = fopen(fullName, "wb");

		// write ticks per frame
		int ticksPerFrame = anim->GetTickPerFrame();
		fwrite(&ticksPerFrame, sizeof(int), 1, hFile);

		// write track len
		size_t trackLen = anim->GetTrackLen();
		fwrite(&trackLen, sizeof(int), 1, hFile);

		for (size_t i = 0; i < trackLen; ++i)
		{
			Liar::LiarTrack* track = anim->GetTrackByIndex(i);

			// write boneId
			int boneId = track->GetBoneId();
			fwrite(&boneId, sizeof(int), 1, hFile);

			// write keys
			WriteLiarKeyFrames(track, Liar::LiarVertexAttr::LiarVertexAttr_TRANSFORM, hFile);
			WriteLiarKeyFrames(track, Liar::LiarVertexAttr::LiarVertexAttr_ROTATION, hFile);
			WriteLiarKeyFrames(track, Liar::LiarVertexAttr::LiarVertexAttr_SCALE, hFile);
		}

		fclose(hFile);
	}

	void LiarPluginWrite::WriteLiarKeyFrames(Liar::LiarTrack* track, Liar::LiarVertexAttr type, FILE* hFile)
	{
		// write key frame num
		size_t num = track->GetNumFrames(type);
		fwrite(&num, sizeof(int), 1, hFile);

		float tmp = 0.0f;
		size_t size = sizeof(float);
		for (size_t i = 0; i < num; ++i)
		{
			Liar::LiarKeyFrame* keyFrame = track->GetKeyFrame(type, i);
			
			// write keyFrame
			int time = keyFrame->GetTime();
			fwrite(&time, sizeof(int), 1, hFile);
			tmp = keyFrame->GetX();
			fwrite(&tmp, size, 1, hFile);
			tmp = keyFrame->GetY();
			fwrite(&tmp, size, 1, hFile);
			tmp = keyFrame->GetZ();
			fwrite(&tmp, size, 1, hFile);

			if (type == Liar::LiarVertexAttr::LiarVertexAttr_ROTATION)
			{
				Liar::LiarQuatKeyFrame* tmpFrame = static_cast<Liar::LiarQuatKeyFrame*>(keyFrame);
				tmp = tmpFrame->GetW();
				fwrite(&tmp, size, 1, hFile);
			}
		}
	}

	void LiarPluginWrite::WriteLiarVecs(std::vector<Liar::Vector3D*>* vec, FILE* hFile)
	{
		size_t p3Size = sizeof(Liar::Vector3D);
		size_t size = vec ? vec->size() : 0;

		// write size;
		fwrite(&size, sizeof(int), 1, hFile);
		// write data
		for (size_t i = 0; i < size; ++i)
		{
			fwrite(vec->at(i), p3Size, 1, hFile);
		}
	}

	void LiarPluginWrite::WriteString(std::string& s, FILE* hFile)
	{
		int size = static_cast<int>(s.size());
		// wirte char num;
		fwrite(&size, sizeof(int), 1, hFile);
		fwrite(&s.front(), sizeof(char), size, hFile);
	}
}
