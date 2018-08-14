#include "LiarMaterial.h"

#ifndef PLUGINS
#include <stb_image.h>
#include <AssetsMgr.hpp>
#endif // !PLUGINS

namespace Liar
{

	// ====================  纹理内容 ================
	LiarTexContext::LiarTexContext() :m_path(""),m_id(0),m_textureType(TextureType::None)
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

		int width, height, nrComponents;
		unsigned char *data = stbi_load(fileName, &width, &height, &nrComponents, 0);
		if (data)
		{
			GLenum format;
			if (nrComponents == 1)
				format = GL_RED;
			else if (nrComponents == 3)
				format = GL_RGB;
			else if (nrComponents == 4)
				format = GL_RGBA;

			glGenTextures(1, &m_textureId);
			glBindTexture(GL_TEXTURE_2D, m_textureId);
			glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			stbi_image_free(data);
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

	LiarTexture::LiarTexture():m_name(""), m_shininess(0.0f)
	{
		m_ambient = new Liar::Vector3D();
		m_diffuse = new Liar::Vector3D();
		m_specular = new Liar::Vector3D();
	}


	LiarTexture::~LiarTexture()
	{
		delete m_ambient;
		delete m_diffuse;
		delete m_specular;
	}

	void LiarTexture::SetPath(const char* path)
	{
		m_name = path;

#ifndef PLUGINS
		m_texContext = AssetsMgr::GetInstance().GetTexContext(m_name);
#endif // !PLUGINS

	}

	void LiarTexture::SetPath(const std::string& path)
	{
		SetPath(path.data());
	}

#ifndef PLUGINS
	void LiarTexture::Render(Liar::Shader& shader, int i)
	{
		if (!m_texContext) return;
		std::string name = "texture";
		name = name + std::to_string(i);
		shader.SetInt(name, i);
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, m_texContext->GetTextureId());
	}

#endif // !PLUGINS


	// ====================  纹理 ================

	LiarMaterial::LiarMaterial()
	{
		m_allTextures = new std::vector<Liar::LiarTexture*>();
		m_textureSize = 0;
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
			--m_textureSize;
		}
	}

#ifndef PLUGINS
	void LiarMaterial::Render(Liar::Shader& shader)
	{
		for (int i = 0; i < m_textureSize; ++i)
		{
			Liar::LiarTexture* texture = m_allTextures->at(i);
			texture->Render(shader, i);
		}
	}
#endif // !PLUGINS


}
