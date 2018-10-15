#include "LiarPluginRead.h"
#include <LiarStringUtil.h>

namespace Liar
{
	Liar::LiarMeshRawData* LiarPluginRead::ReadMeshRawData(const char* path)
	{
		FILE* pFile;
#ifndef __APPLE__
		fopen_s(&pFile, path, "rb+");
#else
		pFile = fopen(path, "rb+");
#endif

		Liar::LiarMeshRawData* rawData = new Liar::LiarMeshRawData();

		// read version
		int ver = 0;
		fread(&ver, sizeof(int), 1, pFile);

		// read mesh`s name
		ReadString(rawData->meshName, pFile);

		// read vertex info
		rawData->SetPos(LiarPluginRead::ReadLiarVecs(pFile));
		rawData->SetNorm(LiarPluginRead::ReadLiarVecs(pFile));
		rawData->SetTexCoord(LiarPluginRead::ReadLiarVecs(pFile));
		rawData->SetColor(LiarPluginRead::ReadLiarVecs(pFile));

		// read face info
		size_t byLen = sizeof(unsigned int);
		size_t len = 0;
		fread(&len, byLen, 1, pFile);
		for (size_t i = 0; i < len; ++i)
		{
			unsigned int pos = 0, norm = 0, tex = 0, color = 0;
			fread(&pos, byLen, 1, pFile);
			fread(&norm, byLen, 1, pFile);
			fread(&tex, byLen, 1, pFile);
			fread(&color, byLen, 1, pFile);
			Liar::LiarFaceDefine* face = new Liar::LiarFaceDefine();
			face->positionIndex = pos;
			face->normalIndex = norm;
			face->texCoordIndex = tex;
			face->colorIndex = color;
			rawData->GetFaces()->push_back(face);
		}

		// read skin info
		size_t skinDefineLen = 0;
		size_t intSize = sizeof(int);
		size_t floatSize = sizeof(float);
		fread(&skinDefineLen, intSize, 1, pFile);
		for (size_t i = 0; i < skinDefineLen; ++i)
		{
			// read write skinDefine
			int vertIndex = 0;
			fread(&vertIndex, intSize, 1, pFile);
			Liar::LiarAnimSkinDefine* skinDefine = rawData->GetAnimSkinDefine(vertIndex);

			// write bone size
			size_t boneInfoSize = 0;
			fread(&boneInfoSize, intSize, 1, pFile);

			// write bone info
			for (size_t j = 0; j < boneInfoSize; ++j)
			{
				int boneId = skinDefine->GetBoneId(j);
				float boneWeight = skinDefine->GetBoneWeight(j);
				fread(&boneId, intSize, 1, pFile);
				fread(&boneWeight, floatSize, 1, pFile);
				skinDefine->AddBoneInfo(boneId, boneWeight);
			}
		}

		// read indices count;
		size_t indiceSize = 0;
		fread(&indiceSize, sizeof(int), 1, pFile);
		// read indices
		for (size_t i = 0; i < indiceSize; ++i)
		{
			int index = 0;
			fread(&index, sizeof(unsigned int), 1, pFile);
			rawData->GetIndices()->push_back(index);
		}


		// read materials info
		size_t matSize = 0;
		// write matSize
		fread(&matSize, sizeof(int), 1, pFile);

		for (size_t i = 0; i < matSize; ++i)
		{
			Liar::LiarMaterialDefine* mat = new Liar::LiarMaterialDefine();
			rawData->AddMaterial(mat);
			// write texSize;
			size_t texSize = 0;
			fread(&texSize, sizeof(int), 1, pFile);
			// write texture
			for (size_t j = 0; j < texSize; ++j)
			{
				// wirte name
				std::string texName("");
				ReadString(texName, pFile);

				// write type
				int type = 0;
				fread(&type, sizeof(int), 1, pFile);

				Liar::LiarTextureDefine* tex = new Liar::LiarTextureDefine(type);
				tex->SetPath(texName);

				mat->GetTextures()->push_back(tex);
			}
		}

		fclose(pFile);

		return rawData;
	}

	std::vector<Liar::Vector3D*>* LiarPluginRead::ReadLiarVecs(FILE* pFile)
	{
		size_t size = 0;
		// read size;
		fread(&size, sizeof(int), 1, pFile);
		// read data
		if (size > 0)
		{
			size_t p3Size = sizeof(Liar::Vector3D);
			std::vector<Liar::Vector3D*>* vec = new std::vector<Liar::Vector3D*>();
			for (size_t i = 0; i < size; ++i)
			{
				Liar::Vector3D* tmp = new Liar::Vector3D();
				fread(tmp, p3Size, 1, pFile);
				vec->push_back(tmp);
			}
			return vec;
		}

		return nullptr;
	}

	// ======================= read skelenton =======================
	Liar::LiarSkeleton* LiarPluginRead::ReadSkeleton(const std::string& path)
	{
		return Liar::LiarPluginRead::ReadSkeleton(path.c_str());
	}

	Liar::LiarSkeleton* LiarPluginRead::ReadSkeleton(const char* path)
	{
		FILE* pFile;
#ifndef __APPLE__
		fopen_s(&pFile, path, "rb+");
#else
		pFile = fopen(path, "rb+");
#endif

		if (!pFile) return nullptr;

		Liar::LiarSkeleton* skeleton = new Liar::LiarSkeleton();

		size_t boneSize = 0;
		fread(&boneSize, sizeof(int), 1, pFile);

		size_t p3Size = sizeof(Liar::Vector3D);
		size_t p4Size = sizeof(Liar::Vector4D);

		for (size_t i = 0; i < boneSize; ++i)
		{
			int id = 0;
			int parentId = 0;
			std::string name;
			fread(&id, sizeof(int), 1, pFile);
			fread(&parentId, sizeof(int), 1, pFile);
			Liar::LiarPluginRead::ReadString(name, pFile);
			Liar::LiarBone* bone = skeleton->GetBone(name, true);
			bone->SetId(id);
			bone->SetParentId(parentId);
			bone->SetName(name);

			Liar::Vector3D* pos = new Liar::Vector3D();
			Liar::Vector4D* rot = new Liar::Vector4D();
			Liar::Vector3D* sca = new Liar::Vector3D();
			fread(pos, p3Size, 1, pFile);
			fread(rot, p4Size, 1, pFile);
			fread(sca, p3Size, 1, pFile);
			bone->SetPositon(pos);
			bone->SetRotation(rot);
			bone->SetScale(sca);
		}

		fclose(pFile);

		return skeleton;
	}

	// ======================= read anim =======================
	Liar::LiarSkeletonAnim* LiarPluginRead::ReadAnim(const std::string& path)
	{
		return Liar::LiarPluginRead::ReadAnim(path.c_str());
	}

	Liar::LiarSkeletonAnim* LiarPluginRead::ReadAnim(const char* path)
	{
		FILE* pFile;
#ifndef __APPLE__
		fopen_s(&pFile, path, "rb+");
#else
		pFile = fopen(path, "rb+");
#endif

		if (!pFile) return nullptr;

		Liar::LiarSkeletonAnim* anim = new Liar::LiarSkeletonAnim();

		int ticksPerFrame = 0;
		fread(&ticksPerFrame, sizeof(int), 1, pFile);
		anim->SetTickPerFrame(ticksPerFrame);

		// read tick size;
		size_t trackLen = 0;
		fread(&trackLen, sizeof(int), 1, pFile);

		for (size_t i = 0; i < trackLen; ++i)
		{
			int boneId = 0;
			fread(&boneId, sizeof(int), 1, pFile);

			Liar::LiarTrack* track = anim->GetTrack(boneId, true);
			ReadLiarKeyFrames(track, Liar::LiarVertexAttr::LiarVertexAttr_TRANSFORM, pFile);
			ReadLiarKeyFrames(track, Liar::LiarVertexAttr::LiarVertexAttr_ROTATION, pFile);
			ReadLiarKeyFrames(track, Liar::LiarVertexAttr::LiarVertexAttr_SCALE, pFile);
		}

		fclose(pFile);

		return anim;
	}

	void LiarPluginRead::ReadLiarKeyFrames(Liar::LiarTrack* track, Liar::LiarVertexAttr type, FILE* pFile)
	{
		size_t num = 0;
		fread(&num, sizeof(int), 1, pFile);

		float x = 0.0f, y = 0.0f, z = 0.0f, w = 0.0f;
		size_t size = sizeof(float);
		for (size_t i = 0; i < num; ++i)
		{
			int time = 0;
			fread(&time, sizeof(int), 1, pFile);
			Liar::LiarKeyFrame* keyFrame = track->GetKeyFrame(type, time, true);
			fread(&x, size, 1, pFile);
			fread(&y, size, 1, pFile);
			fread(&z, size, 1, pFile);
			if (type == Liar::LiarVertexAttr::LiarVertexAttr_ROTATION)
			{
				fread(&w, size, 1, pFile);
			}
			keyFrame->Set(x, y, z, w);
		}
	}

	// ======================= read model ===========================

	void LiarPluginRead::ReadNode(const char* path, Liar::LiarNode& node)
	{
		FILE* pFile;
#ifndef __APPLE__
		fopen_s(&pFile, path, "rb+");
#else
		pFile = fopen(path, "rb+");
#endif
//        if(!pFile) LOG("ERROR::MODEL::FILE_NOT_SUCCESFULLY_READ");
        
		std::string basePath = path;
		basePath = Liar::StringUtil::GetHead(basePath, "/");

		unsigned int ver = 0;
		// read version
		fread(&ver, sizeof(unsigned int), 1, pFile);

		// read vexStatus
		int vexStatus = 0;
		fread(&vexStatus, sizeof(int), 1, pFile);

		// read root
		basePath += "/";
		ReadChildNode(node, basePath, pFile);

		fclose(pFile);
	}

	void LiarPluginRead::ReadChildNode(Liar::LiarNode& node, std::string& basePath, FILE* pFile)
	{
		// read size;
		int size = 0;
		fread(&size, sizeof(int), 1, pFile);

		for (int i = 0; i < size; ++i)
		{
			std::string nodeName;
			Liar::LiarNode* subNode = node.AddChild();
			ReadString(nodeName, pFile);

			nodeName = basePath + nodeName;
			subNode->SetNodeName(nodeName);

			// read children
			ReadChildNode(*subNode, basePath, pFile);
		}
	}

	void LiarPluginRead::ReadString(std::string& s, FILE* pFile)
	{
		int size = 0;
		fread(&size, sizeof(int), 1, pFile);
		for (int i = 0; i < size; ++i)
		{
			char a ;
			fread(&a, sizeof(char), 1, pFile);
			s.push_back(a);
		}
	}

	std::string LiarPluginRead::basePath = "";
}
