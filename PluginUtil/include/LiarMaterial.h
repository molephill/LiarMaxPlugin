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

	// ====================  纹理内容 ================
	class LiarTexContext
	{
	public:
		LiarTexContext();
		~LiarTexContext();

	private:
		std::string m_path;

	public:
		std::string& GetPath() { return m_path; };

		void SetPath(const char* v) { m_path = v; };
		void SetPath(const std::string& v) { m_path = v; };

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
		LiarTexture(bool init = false);
		~LiarTexture();

	private:
		Liar::LiarTexContext* m_texContext;
		int m_textureType;

	public:
		Liar::LiarTexContext* GetTexContext() { return m_texContext; };
		void SetPath(const char*);
		void SetPath(const std::string&);

		void SetType(int v) { m_textureType = v; };
		int GetType() const { return m_textureType; };

#ifndef PLUGINS
		void Render(Liar::Shader&, size_t);
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
		std::string m_type;

	public:
		void EraseIndex(int);

		std::vector<Liar::LiarTexture*>* GetTextures() { return m_allTextures; };
		Liar::LiarTexture* GetTexture(int index) { return m_allTextures->at(index); };

		size_t GetTexSize() { return m_allTextures ? m_allTextures->size() : 0; };

		std::string& GetType() { return m_type; };

#ifdef PLUGINS
	public:
		std::string name;
#else
	public:
		void Render(Liar::Shader&);
#endif // PLUGINS

	};
}

