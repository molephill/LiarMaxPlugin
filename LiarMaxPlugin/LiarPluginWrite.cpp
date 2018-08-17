#include "LiarPluginWrite.h"

namespace Liar
{
	void LiarPluginWrite::WriteModelHierarchy(Liar::LiarMaxNodeParse* parse, Liar::LiarPluginCfg* liarPluginCfg, const std::string& path)
	{
		Liar::LiarNode* rootNode = parse->GetRootNode();
		// ignore root

		size_t size = parse->GetMeshSize();
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
		int vetOpen = Liar::LiarPluginWrite::GetVertexVer(liarPluginCfg);
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
		size_t meshSize = parse->GetMeshSize();
		for (size_t i = 0; i < meshSize; ++i)
		{
			if (i == index)
			{
				Liar::LiarMesh* mesh = parse->GetMesh(i);
				mesh->saveName = saveName;
				mesh->meshName = meshName;
				return;
			}
		}
	}

	void LiarPluginWrite::WriteMesh(Liar::LiarMaxNodeParse* parse, Liar::LiarPluginCfg* liarPlugin, const std::string& path)
	{
		// write ModelHierarchy
		WriteModelHierarchy(parse, liarPlugin, path);

		// get vertex_open_status
		int vetOpen = Liar::LiarPluginWrite::GetVertexVer(liarPlugin);

		std::string folder = Liar::StringUtil::GetHead(path, "\\");
		size_t meshSize = parse->GetMeshSize();
		for (size_t i = 0; i < meshSize; ++i)
		{
			Liar::LiarMesh* mesh = parse->GetMesh(i);

			WriteLiarMesh(mesh, folder, liarPlugin, vetOpen);
		}
	}

	void LiarPluginWrite::WriteLiarMesh(Liar::LiarMesh* mesh, const std::string& path, Liar::LiarPluginCfg* liarPlugin, int vetOpen)
	{
		std::string& meshName = mesh->saveName;
		char fullName[MAX_PATH];

		sprintf_s(fullName, "%s\\%s", path.c_str(), meshName.c_str());

		FILE* hFile = fopen(fullName, "wb");

		// write ver
		fwrite(&(liarPlugin->pluginVersion), sizeof(unsigned int), 1, hFile);
		// write vertex open status
		fwrite(&vetOpen, sizeof(int), 1, hFile);
		// write mesh`s name
		//Liar::StringUtil::StringToLower(mesh->meshName);
		WriteString(mesh->meshName, hFile);
		// write mesh`s geometery
		WriteLiarGeometery(liarPlugin, mesh->GetGeo(), hFile);
		// write mesh`s material
		WriteLiarMaterial(mesh, hFile);

		fclose(hFile);
	}

	void LiarPluginWrite::WriteLiarGeometery(Liar::LiarPluginCfg* liarPlugin, Liar::LiarGeometry* geo, FILE* hFile)
	{
		// write indices count;
		int indiceSize = static_cast<int>(geo->GetIndicesSize());
		fwrite(&indiceSize, sizeof(int), 1, hFile);
		// write indices
		fwrite(&(geo->GetIndices()->front()), sizeof(unsigned int), indiceSize, hFile);

		size_t bufferSize = geo->GetBufferSize();
		// write bufferSize;
		fwrite(&bufferSize, sizeof(int), 1, hFile);
		// write buffer
		for (int i = 0; i < bufferSize; ++i)
		{
			WriteLiarVertexBuffer(liarPlugin, geo->GetBuffer(i), hFile);
		}
	}

	void LiarPluginWrite::WriteLiarMaterial(Liar::LiarMesh* mesh, FILE* hFile)
	{
		size_t matSize = mesh->GetMatrials()->size();
		// write matSize
		fwrite(&matSize, sizeof(int), 1, hFile);

		for (size_t i = 0; i < matSize; ++i)
		{
			Liar::LiarMaterial* mat = mesh->GetMat(i);
			size_t texSize = mat->GetTexSize();
			// write texSize;
			fwrite(&texSize, sizeof(int), 1, hFile);
			// write texture
			for (int i = 0; i < texSize; ++i)
			{
				WriteLiarTexture(mat->GetTexture(i), hFile);
			}
		}
	}

	void LiarPluginWrite::WriteLiarVertexBuffer(Liar::LiarPluginCfg* liarPluginCfg, Liar::LiarVertexBuffer* buff, FILE* hFile)
	{
		// write pos
		size_t p3Size = sizeof(Liar::Vector3D);
		fwrite(buff->position, p3Size, 1, hFile);
		// write normal
		if (liarPluginCfg->vertexNormal)
		{
			fwrite(buff->normal, p3Size, 1, hFile);
		}
		// write color
		if (liarPluginCfg->vertexColor)
		{
			fwrite(buff->color, p3Size, 1, hFile);
		}
		// write uv
		if (liarPluginCfg->textureCoord)
		{
			size_t p2Size = sizeof(Liar::Vector2D);
			fwrite(buff->uv, p2Size, 1, hFile);
		}
	}

	void LiarPluginWrite::WriteLiarTexture(Liar::LiarTexture* tex, FILE* hFile)
	{
		// wirte name
		std::string& texName = tex->GetTexContext()->GetPath();
		WriteString(texName, hFile);

		// write type
		std::string& texType = tex->GetType();
		WriteString(texType, hFile);
	}

	void LiarPluginWrite::WriteString(std::string& s, FILE* hFile)
	{
		int size = static_cast<int>(s.size());
		// wirte char num;
		fwrite(&size, sizeof(int), 1, hFile);
		fwrite(&s.front(), sizeof(char), size, hFile);
	}

	int LiarPluginWrite::GetVertexVer(Liar::LiarPluginCfg* liarPluginCfg)
	{
		int ver = 0;
		if (liarPluginCfg->vertexNormal)
		{
			ver |= (1 << LIAR_NORMAL);
		}

		if (liarPluginCfg->vertexColor)
		{
			ver |= (1 << LIAR_COLOR);
		}

		if (liarPluginCfg->textureCoord)
		{
			ver |= (1 << LIAR_UV);
		}

		return ver;
	}
}
