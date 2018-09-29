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
	AssetsMgr::AssetsMgr() :
		m_allTextures(new std::vector<Liar::LiarTexture*>()), 
		m_allMeshes(new std::vector<Liar::LiarMesh*>()), 
		m_allShaders(new std::vector<Liar::LiarBaseShader*>()),
		m_allPrograms(new std::vector<Liar::LiarShaderProgram*>()),
		m_allSkeletons(new std::vector<Liar::LiarSkeleton*>())
	{
	}

	AssetsMgr::~AssetsMgr()
	{
	}

	Liar::LiarMesh* AssetsMgr::GetMesh(const char* fileName, const char* base)
	{
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
		ret->IncRefCount();
#endif // !PLUGINS		
		return ret;
	}

	Liar::LiarMesh* AssetsMgr::GetMesh(const std::string& fileName, const char* base)
	{
		return GetMesh(fileName.data(), base);
	}

	Liar::LiarTexture* AssetsMgr::GetTexture(const char* fileName)
	{
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
		ret->DesRefCount();
#endif // PLUGINS

		return ret;
	}

	Liar::LiarTexture* AssetsMgr::GetTexture(const std::string& fileName)
	{
		return GetTexture(fileName.data());
	}

	Liar::LiarBaseShader* AssetsMgr::GetBaseShader(const char* fileName)
	{
		Liar::LiarBaseShader* ret = nullptr;
		for (std::vector<Liar::LiarBaseShader*>::iterator it = m_allShaders->begin(); it < m_allShaders->end(); ++it)
		{
			if (std::strcmp(fileName, (*it)->GetPath().data()) == 0)
			{
				ret = *it;
				break;
			}
		}

		if (!ret)
		{
			ret = new Liar::LiarBaseShader();
			ret->SetPath(fileName);
			m_allShaders->push_back(ret);
		}

		ret->IncRefCount();

		return ret;
	}

	Liar::LiarBaseShader* AssetsMgr::GetBaseShader(const std::string& fileName)
	{
		return GetBaseShader(fileName.c_str());
	}

	Liar::LiarShaderProgram* AssetsMgr::GetShaderProgrom(const char* name, const char* vertexFile, const char* fragmentFile)
	{
		Liar::LiarShaderProgram* ret = nullptr;
		for (std::vector<Liar::LiarShaderProgram*>::iterator it = m_allPrograms->begin(); it < m_allPrograms->end(); ++it)
		{
			if (std::strcmp(name, (*it)->GetName().data()) == 0)
			{
				ret = *it;
				break;
			}
		}

		if (!ret)
		{
			ret = new Liar::LiarShaderProgram();
			ret->LinkProgram(vertexFile, fragmentFile);
			ret->SetName(name);
			m_allPrograms->push_back(ret);
		}

		ret->IncRefCount();

		return ret;
	}

	Liar::LiarShaderProgram* AssetsMgr::GetShaderProgrom(const std::string& name, const std::string& vertexFile, const std::string& fragmentFile)
	{
		return GetShaderProgrom(name.c_str(), vertexFile.c_str(), fragmentFile.c_str());
	}

	void AssetsMgr::ReleaseShaderProgram(Liar::LiarShaderProgram* program)
	{
		for (std::vector<Liar::LiarShaderProgram*>::iterator it = m_allPrograms->begin(); it < m_allPrograms->end();)
		{
			if (program == *it)
			{
				if ((*it)->DesRefCount() <= 0)
				{
					delete *it;
					it = m_allPrograms->erase(it);
				}
				else
				{
					++it;
				}
			}
			else
			{
				++it;
			}
		}
	}

	void AssetsMgr::ReleaseShaderProgram(const char* name)
	{
		for (std::vector<Liar::LiarShaderProgram*>::iterator it = m_allPrograms->begin(); it < m_allPrograms->end();)
		{
			if (std::strcmp(name, (*it)->GetName().data()) == 0)
			{
				if ((*it)->DesRefCount() <= 0)
				{
					delete *it;
					it = m_allPrograms->erase(it);
				}
				else
				{
					++it;
				}
			}
			else
			{
				++it;
			}
		}
	}

	void AssetsMgr::ReleaseShaderProgram(const std::string& name)
	{
		ReleaseShaderProgram(name.c_str());
	}

	Liar::LiarSkeleton* AssetsMgr::GetSkeleton(const char* path)
	{
		if (!path) return nullptr;
		Liar::LiarSkeleton* ret = nullptr;
#ifndef PLUGINS
		for (std::vector<Liar::LiarSkeleton*>::iterator it = m_allSkeletons->begin(); it < m_allSkeletons->end();)
		{
			if (std::strcmp(path, (*it)->GetPath().data()) == 0)
			{
				ret = *it;
				break;
			}
		}

		if (!ret)
		{
			ret = Liar::LiarPluginRead::ReadSkeleton(path);
			ret->SetPath(path);
			m_allSkeletons->push_back(ret);
		}
		ret->IncRefCount();
#endif // !PLUGINS
		return ret;
	}

	Liar::LiarSkeleton* AssetsMgr::GetSkeleton(const std::string& path)
	{
		return GetSkeleton(path.c_str());
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