#pragma once

#include <string>
#include <vector>

#include <PluginDefine.h>
#include <LiarVertexBuffer.h>
#include <LiarStringUtil.h>

#ifndef PLUGINS
#include "Shader.hpp"
#endif // !PLUGINS

namespace Liar
{

	enum TextureType
	{
		None = 0,
		Texture_Diffuse,
		Texture_Specular
	};

	// ====================  纹理内容 ================
	class LiarTexContext
	{
	public:
		LiarTexContext();
		~LiarTexContext();

	private:
		std::string m_path;
		int m_id;
		TextureType m_textureType;

	public:
		std::string& GetPath() { return m_path; };

#ifndef PLUGINS
	private:
		unsigned int m_refCount;
		unsigned int m_textureId;

	public:
		void Upload(const char*);
		void Upload(const std::string&);

		unsigned int GetTextureId() const { return m_textureId; };

		void AddRef() { ++m_refCount; };
		unsigned int SubRef() { return --m_refCount; };
#endif // !PLUGINS
	};
	// ====================  纹理内容 ================

	// ====================  纹理 ================

	class LiarTexture
	{
	public:
		LiarTexture();
		~LiarTexture();

	private:
		std::string m_name;

		Liar::Vector3D* m_ambient;
		Liar::Vector3D* m_diffuse;
		Liar::Vector3D* m_specular;
		float m_shininess;

#ifndef PLUGINS
		Liar::LiarTexContext* m_texContext;
#endif // !PLUGINS

	public:
		std::string& GetName() { return m_name; };
		void SetPath(const char* v);
		void SetPath(const std::string& v);

		Liar::Vector3D* GetAmbient() { return m_ambient; };
		Liar::Vector3D* GetDiffuse() { return m_diffuse; };
		Liar::Vector3D* GetSpecular() { return m_specular; };
		float GetShininess() { return m_shininess; };
		void SetShininess(float v) { m_shininess = v; };

#ifndef PLUGINS
		void Render(Liar::Shader&, int);
#endif // ! PLUGINS

	};

	// ====================  纹理 ================

	class LiarMaterial
	{
	public:
		LiarMaterial();
		~LiarMaterial();

	private:
		std::vector<Liar::LiarTexture*>* m_allTextures;
		int m_textureSize;

	public:
		void EraseIndex(int);

		std::vector<Liar::LiarTexture*>* GetTextures() { return m_allTextures; };
		Liar::LiarTexture* GetTexture(int index) { return m_allTextures->at(index); };

		int GetTexSize() { return m_textureSize; };
		void SetTexSize(int v) { m_textureSize = v; };
       

#ifdef PLUGINS
	public:
		std::string name;
#else
	public:
		void Render(Liar::Shader&);
#endif // PLUGINS

	};
}

