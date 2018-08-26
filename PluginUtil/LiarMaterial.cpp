#include "LiarMaterial.h"
#include <LiarStringUtil.h>

#ifndef PLUGINS
#include "LiarTextureHelper.h"
#include "AssetsMgr.hpp"
#endif // !PLUGINS

namespace Liar
{

	// ====================  texture_content ================
	LiarTexContext::LiarTexContext() :m_path("")
	{
#ifndef PLUGINS
		m_textureId = 0;
#endif // !PLUGINS

	}

	LiarTexContext::~LiarTexContext()
	{
	}

#ifndef PLUGINS
	void LiarTexContext::Upload(const char* fileName)
	{
		m_path = fileName;
		std::string ext = Liar::StringUtil::GetLast(m_path, ".");
		Liar::StringUtil::StringToUpper(ext);

		if (ext == "DDS")
		{
			m_textureId = Liar::LiarTextureHelper::LoadDDS(fileName);
		}
		if (ext == "TGA")
		{
			m_textureId = Liar::LiarTextureHelper::LoadTGA(fileName);
		}
		else
		{
			m_textureId = Liar::LiarTextureHelper::Load(fileName);
		}

		if (m_textureId > 0)
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}
		else
		{
			std::cout << "Texture failed to load at path: " << fileName << std::endl;
		}
	}

	void LiarTexContext::Upload(const std::string& fileName)
	{
		Upload(fileName.data());
	}
#endif // !PLUGINS


	// ====================  纹理内容 ================

	// ====================  纹理 ================

	LiarTexture::LiarTexture(bool init): m_textureType(0)
	{
		if (init) m_texContext = new Liar::LiarTexContext();
	}


	LiarTexture::~LiarTexture()
	{
		if(m_texContext) delete m_texContext;
	}

	void LiarTexture::SetPath(const char* path)
	{
#ifndef PLUGINS
		m_texContext = AssetsMgr::GetInstance().GetTexContext(path);
#endif // !PLUGINS

	}

	void LiarTexture::SetPath(const std::string& path)
	{
		SetPath(path.data());
	}

#ifndef PLUGINS
	void LiarTexture::Render(Liar::Shader& shader, size_t texNum)
	{
		if (!m_texContext) return;
        int index = static_cast<int>(texNum);
		std::string name = "texture";
		name = name + std::to_string(index);
		shader.SetInt(name, index);
		glActiveTexture(GL_TEXTURE0 + index);
		glBindTexture(GL_TEXTURE_2D, m_texContext->GetTextureId());
	}

#endif // !PLUGINS


	// ====================  纹理 ================

	LiarMaterial::LiarMaterial():m_type("")
	{
		m_allTextures = new std::vector<Liar::LiarTexture*>();
	}


	LiarMaterial::~LiarMaterial()
	{
		EraseIndex(0);
		std::vector<Liar::LiarTexture*>().swap(*m_allTextures);
		delete m_allTextures;
	}

	void LiarMaterial::EraseIndex(int index)
	{
		for (std::vector<Liar::LiarTexture*>::iterator it = m_allTextures->begin() + index; it != m_allTextures->end();)
		{
			delete *it;
			it = m_allTextures->erase(it);
		}
	}

#ifndef PLUGINS
	void LiarMaterial::Render(Liar::Shader& shader)
	{
		for (size_t i = 0; i < m_allTextures->size(); ++i)
		{
			Liar::LiarTexture* texture = m_allTextures->at(i);
			texture->Render(shader, i);
		}
	}
#endif // !PLUGINS


}
