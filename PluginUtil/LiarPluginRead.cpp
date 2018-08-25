#include "LiarPluginRead.h"
#include <LiarStringUtil.h>
#include "Log.hpp"

namespace Liar
{

#ifndef PLUGINS
	// ======================= read model ===========================
    Liar::Model* LiarPluginRead::ReadModel(const std::string& path, const char* texBasePath)
    {
        return Liar::LiarPluginRead::ReadModel(path.c_str(), texBasePath);
    }
	Liar::Model* LiarPluginRead::ReadModel(const char* path, const char* texBasePath)
	{
		Liar::LiarNode* node = new Liar::LiarNode();
		Liar::LiarPluginRead::ReadNode(path, *node);
		Liar::Model* model = new Liar::Model();
		ReadChildModel(*model, *node, texBasePath);
		delete node;
		return model;
	}

	void LiarPluginRead::ReadChildModel(Liar::Model& model, const Liar::LiarNode& node, const char* basePath)
	{
		std::vector<Liar::LiarNode*>* children = node.GetChildren();
		if (children)
		{
			size_t size = children->size();
			for (size_t i = 0; i < size; ++i)
			{
				Liar::LiarNode* subNode = children->at(i);
				std::string& nodeName = subNode->GetNodeName();
				model.AddMesh(nodeName, basePath);

				// add child nodes
				ReadChildModel(model, *subNode, basePath);
			}
		}
	}
#endif // !PLUGINS

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

    Liar::LiarMesh* LiarPluginRead::ReadMesh(const std::string& path, const char* texBasePath)
    {
        return Liar::LiarPluginRead::ReadMesh(path.c_str(), texBasePath);
    }
	Liar::LiarMesh* LiarPluginRead::ReadMesh(const char* path, const char* texBasePath)
	{
		FILE* pFile;
#ifndef __APPLE__
		fopen_s(&pFile, path, "rb+");
#else
        pFile = fopen(path, "rb+");
#endif
		if (!pFile)
		{
			return nullptr;
		}
		else
		{
			if (!texBasePath)
			{
				basePath = path;
				basePath = Liar::StringUtil::GetHead(basePath, "/");
				basePath += "/";
			}
			else
			{
				basePath = texBasePath;
			}

			Liar::LiarMesh* mesh = new Liar::LiarMesh();
			ReadLiarMesh(mesh, pFile);
			fclose(pFile);
			return mesh;
		}
	}

	void LiarPluginRead::ReadLiarMesh(Liar::LiarMesh* mesh, FILE* pFile)
	{
		// read ver
		unsigned int ver = 0;
		fread(&ver, sizeof(unsigned int), 1, pFile);
		// read vertex open status
		int vertexOpen = 0;
		fread(&vertexOpen, sizeof(int), 1, pFile);
		// read mesh`s name
		//fread(&(mesh->meshName), sizeof(std::string), 1, pFile);
		ReadString(mesh->meshName, pFile);
		// read mesh`s Geometery
		ReadLiarGeometery(vertexOpen, mesh->GetGeo(), pFile);
		// read mesh`s material
		ReadLiarMaterial(mesh, pFile);
	}

	void LiarPluginRead::ReadLiarGeometery(int vertexOpen, Liar::LiarGeometry* geo, FILE* pFile)
	{
		// set vertex open
		geo->SetVertexOpen(vertexOpen);

		// read indices count;
		int indiceSize = 0;
		fread(&indiceSize, sizeof(int), 1, pFile);
		// read indices
		for (int i = 0; i < indiceSize; ++i)
		{
			unsigned int index = 0;
			fread(&index, sizeof(unsigned int), 1, pFile);
			geo->GetIndices()->push_back(index);
		}

		// read mesh`s rawData
		ReadLiarRawData(geo, pFile);
		// read mesh`s faces
		ReadLiarFaces(geo, pFile);
	}

	void LiarPluginRead::ReadLiarRawData(Liar::LiarGeometry* geo, FILE* pFile)
	{
		geo->GetRawData()->SetPos(LiarPluginRead::ReadLiarVecs(pFile));
		geo->GetRawData()->SetNorm(LiarPluginRead::ReadLiarVecs(pFile));
		geo->GetRawData()->SetTexCoord(LiarPluginRead::ReadLiarVecs(pFile));
		geo->GetRawData()->SetColor(LiarPluginRead::ReadLiarVecs(pFile));
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

	void LiarPluginRead::ReadLiarFaces(Liar::LiarGeometry* geo, FILE* pFile)
	{
		size_t size = 0;

		// read size;
		fread(&size, sizeof(int), 1, pFile);
		// read data
		size_t pSize = sizeof(Liar::LiarVertexDefine);
		for (size_t i = 0; i < size; ++i)
		{
			Liar::LiarVertexDefine* define = new Liar::LiarVertexDefine();
			fread(define, pSize, 1, pFile);
			geo->GetVertexFaces()->push_back(define);
		}
	}

	void LiarPluginRead::ReadLiarMaterial(Liar::LiarMesh* mesh, FILE* pFile)
	{
		size_t matSize = 0;
		// read matSize
		fread(&matSize, sizeof(int), 1, pFile);
		for (size_t i = 0; i < matSize; ++i)
		{
			Liar::LiarMaterial* mat = new Liar::LiarMaterial();
			mesh->GetMatrials()->push_back(mat);

			// write matType
			ReadString(mat->GetType(), pFile);

			int texSize = 0;
			// read texSize;
			fread(&texSize, sizeof(int), 1, pFile);
			// read texture
			for (int i = 0; i < texSize; ++i)
			{
				Liar::LiarTexture* tex = ReadLiarTexture(pFile);
				mat->GetTextures()->push_back(tex);
			}
		}
	}

	Liar::LiarTexture* LiarPluginRead::ReadLiarTexture(FILE* pFile)
	{
		Liar::LiarTexture* tex = new Liar::LiarTexture();

		// read name
		//fread(&(tex->GetName()), sizeof(std::string), 1, pFile);
		std::string baseName;
		ReadString(baseName, pFile);
		baseName = basePath + baseName;

		// read type
		int type = 0;
		fread(&type, sizeof(int), 1, pFile);
		tex->SetType(type);

		// set name
		tex->SetPath(baseName);

		return tex;
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
