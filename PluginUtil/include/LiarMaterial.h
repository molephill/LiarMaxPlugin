#pragma once

#include <string>
#include <vector>

#include <LiarVertexBuffer.h>
#include <LiarStringUtil.h>

#ifndef PLUGINS
#include "Shader.hpp"
#else
#include <IGame.h>
#endif // !PLUGINS

namespace Liar
{
	// ====================  Œ∆¿Ì ================

	class LiarTexture
	{
	public:
		LiarTexture();
		~LiarTexture();

	private:
		int m_textureType;
		std::string m_path;

	public:
		void SetPath(const char* v) { m_path = v; };
		void SetPath(const std::string& v) { m_path = v; };

		std::string& GetPath() { return m_path; };

		void SetType(int v) { m_textureType = v; };
		int GetType() const { return m_textureType; };

#ifndef PLUGINS
		void Render(Liar::Shader&, size_t);

	private:
		unsigned int m_refCount;
		unsigned int m_textureId;

	public:
		void Upload(const char*);
		void Upload(const std::string&);

		unsigned int GetTextureId() const { return m_textureId; };

		void AddRef() { ++m_refCount; };
		unsigned int SubRef() { return --m_refCount; };
#endif // ! PLUGINS

	};

	// ====================  Œ∆¿Ì ================

	class LiarMaterial
	{
	public:
		LiarMaterial();
		~LiarMaterial();

	private:
		std::vector<Liar::LiarTexture*>* m_allTextures;
		std::string m_type;

		Liar::Vector3D* m_ambient;
		Liar::Vector3D* m_diffuse;
		Liar::Vector3D* m_specular;
		float m_shineness;

	public:
		void EraseIndex(int);

		std::vector<Liar::LiarTexture*>* GetTextures() { return m_allTextures; };
		Liar::LiarTexture* GetTexture(int index) { return m_allTextures->at(index); };

		size_t GetTexSize() { return m_allTextures ? m_allTextures->size() : 0; };

		std::string& GetType() { return m_type; };

		Liar::Vector3D* GetAmbient() { return m_ambient; };
		Liar::Vector3D* GetDiffuse() { return m_diffuse; };
		Liar::Vector3D* GetSpecular() { return m_specular; };

		float GetShineness() const { return m_shineness; };

#ifdef PLUGINS
	public:
		std::string name;
#else
	public:
		void Render(Liar::Shader&);
#endif // PLUGINS

	};
}

