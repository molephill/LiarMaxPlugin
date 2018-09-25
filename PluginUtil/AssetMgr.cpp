//
//  AssetsMgr.cpp
//  OpenGL
//
//  Created by 毛伟 on 2018/6/10.
//  Copyright © 2018年 lier. All rights reserved.
//

#include <AssetsMgr.hpp>

namespace Liar
{
	AssetsMgr::AssetsMgr() :m_allTextures(nullptr), m_allMeshes(nullptr)
	{
	}

	AssetsMgr::~AssetsMgr()
	{
	}

	Liar::LiarMesh* AssetsMgr::GetMesh(const char* fileName, const char* base)
	{
		if (!m_allMeshes) m_allMeshes = new std::vector<Liar::LiarMesh*>();
		size_t len = m_allMeshes->size();
		Liar::LiarMesh* ret = nullptr;
		for (size_t i = 0; i < len; i++)
		{
			Liar::LiarMesh* mesh = m_allMeshes->at(i);
			if (std::strcmp(fileName, mesh->meshName.data()) == 0)
			{
				ret = mesh;
				break;
			}
		}

		if (ret == nullptr)
		{
			ret = Liar::LiarPluginRead::ReadMesh(fileName, base);
			ret->meshName = fileName;
#ifndef PLUGINS
			ret->Upload();
#endif // PLUGINS
			m_allMeshes->push_back(ret);
		}

#ifndef PLUGINS
		ret->AddRef();
#endif // !PLUGINS		
		return ret;
	}

	Liar::LiarMesh* AssetsMgr::GetMesh(const std::string& fileName, const char* base)
	{
		return GetMesh(fileName.data(), base);
	}

	Liar::LiarTexture* AssetsMgr::GetTexture(const char* fileName)
	{
		if (!m_allTextures) m_allTextures = new std::vector<Liar::LiarTexture*>();
		size_t len = m_allTextures->size();
		Liar::LiarTexture* ret = nullptr;
		for (size_t i = 0; i < len; ++i)
		{
			Liar::LiarTexture* textContext = m_allTextures->at(i);
			if (std::strcmp(fileName, textContext->GetPath().data()) == 0)
			{
				ret = textContext;
				break;
			}
		}

		if (ret == nullptr)
		{
			ret = new Liar::LiarTexture();
#ifndef PLUGINS
			ret->Upload(fileName);
#endif // PLUGINS
			m_allTextures->push_back(ret);
		}

#ifndef PLUGINS
		ret->AddRef();
#endif // PLUGINS

		return ret;
	}

	Liar::LiarTexture* AssetsMgr::GetTexture(const std::string& fileName)
	{
		return GetTexture(fileName.data());
	}

	std::string AssetsMgr::GetPath(const char* base)
	{
		return "" + BASE_PATH + base;
	}

#ifndef PLUGINS
	void AssetsMgr::PrintMat4(const glm::mat4 & view)
	{
		std::cout << std::fixed << std::setprecision(5);
		std::cout << "[" << std::setw(10) << view[0][0] << " " << std::setw(10) << view[0][1] << " " << std::setw(10) << view[0][2] << " " << std::setw(10) << view[0][3] << "]\n"
			<< "[" << std::setw(10) << view[1][0] << " " << std::setw(10) << view[1][1] << " " << std::setw(10) << view[1][2] << " " << std::setw(10) << view[1][3] << "]\n"
			<< "[" << std::setw(10) << view[2][0] << " " << std::setw(10) << view[2][1] << " " << std::setw(10) << view[2][2] << " " << std::setw(10) << view[2][3] << "]\n"
			<< "[" << std::setw(10) << view[3][0] << " " << std::setw(10) << view[3][1] << " " << std::setw(10) << view[3][2] << " " << std::setw(10) << view[3][3] << "]\n";
		std::cout << std::resetiosflags(std::ios_base::fixed | std::ios_base::floatfield);
	}
#endif // PLUGINS

	AssetsMgr* AssetsMgr::m_instance = nullptr;
}