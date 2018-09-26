#include "LiarMaterial.h"

#ifndef PLUGINS
#include "LiarTextureHelper.h"
#include "AssetsMgr.hpp"
#endif // !PLUGINS

namespace Liar
{

	// ====================  ���� ================

	LiarTexture::LiarTexture(): 
		m_textureType(0),
		m_path("")
	{
	}


	LiarTexture::~LiarTexture()
	{
	}

#ifndef PLUGINS

	void LiarTexture::Upload(const char* fileName)
	{
		m_path = fileName;
		std::string ext = Liar::StringUtil::GetLast(m_path, ".");
		Liar::StringUtil::StringToUpper(ext);

		if (ext == "DDS")
		{
			m_textureId = Liar::LiarTextureHelper::LoadDDS(fileName);
		}
		else if (ext == "TGA")
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

	void LiarTexture::Upload(const std::string& fileName)
	{
		Upload(fileName.data());
	}

	void LiarTexture::Render(Liar::LiarShaderProgram& shader, size_t texNum)
	{
		if (m_textureId <= 0) return;
        int index = static_cast<int>(texNum);
		std::string name = "texture";
		name = name + std::to_string(index);
		shader.SetInt(name, index);
		glActiveTexture(GL_TEXTURE0 + index);
		glBindTexture(GL_TEXTURE_2D, m_textureId);
	}

#endif // !PLUGINS


	// ====================  ���� ================

	LiarMaterial::LiarMaterial():
		m_type(""),
		m_ambient(new Liar::Vector3D()), m_diffuse(new Liar::Vector3D()), m_specular(new Liar::Vector3D()),
		m_shineness(0.0f)
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
	void LiarMaterial::Render(Liar::LiarShaderProgram& shader)
	{
		for (size_t i = 0; i < m_allTextures->size(); ++i)
		{
			Liar::LiarTexture* texture = m_allTextures->at(i);
			texture->Render(shader, i);
		}
	}
#endif // !PLUGINS


}
