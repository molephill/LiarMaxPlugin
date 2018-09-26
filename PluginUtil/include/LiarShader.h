#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <Matrices.h>
#include <Vectors.h>

namespace Liar
{
	enum LiarShaderType
	{
		SHADER_TYPE_VERTEXT = 0,
		SHADER_TYPE_FRAGMENT = 1,
		SHADER_TYPE_PROGROM = 2,
	};

	class LiarBaseShader
	{
	public:
		LiarBaseShader();
		~LiarBaseShader();

	private:
		std::string m_path;
		int m_refCount;
		std::string m_shaderCode;

	public:
		std::string& GetPath() { return m_path; };
		void SetPath(const char*);
		void SetPath(const std::string&);

		const char* GetShaderCode() const { return m_shaderCode.c_str(); };

		int IncRefCount() { return ++m_refCount; };
		int DesRefCount() { return --m_refCount; };
	};

	class LiarShaderProgram
	{
	public:
		LiarShaderProgram();
		~LiarShaderProgram();

	private:
		unsigned int m_ID;

	public:
		void LinkProgrom(const char*, const char*);
		void LinkProgrom(const std::string&, const std::string&);
		void LinkProgrom(const Liar::LiarBaseShader&, const Liar::LiarBaseShader&);

		void Use();
		unsigned int GetID() { return m_ID; };
		// ------------------------------------------------------------------------
		void SetBool(const std::string&, bool value) const;
		// ------------------------------------------------------------------------
		void SetInt(const std::string&, int value) const;
		// ------------------------------------------------------------------------
		void SetFloat(const std::string&, float value) const;
		// ------------------------------------------------------------------------
		void SetVec2(const std::string&, const Liar::Vector2D&) const;
		void SetVec2(const std::string&, float, float) const;
		// ------------------------------------------------------------------------
		void SetVec3(const std::string&, const Liar::Vector3D&) const;
		void SetVec3(const std::string&, float, float, float) const;
		// ------------------------------------------------------------------------
		void SetVec4(const std::string&, const Liar::Vector4D&) const;
		void SetVec4(const std::string&, float, float, float, float) const;
		// ------------------------------------------------------------------------
		void SetMat2(const std::string&, const Liar::Matrix2&) const;
		// ------------------------------------------------------------------------
		void SetMat3(const std::string&, const Liar::Matrix3&) const;
		// ------------------------------------------------------------------------
		void SetMat4(const std::string&, const Liar::Matrix4&) const;

	private:
		void CheckCompileErrors(unsigned int, LiarShaderType);
	};
}

