#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <Matrices.h>
#include <Vectors.h>

#include <ILiarRef.h>

namespace Liar
{
	enum LiarShaderType
	{
		SHADER_TYPE_VERTEXT = 0,
		SHADER_TYPE_FRAGMENT = 1,
		SHADER_TYPE_PROGROM = 2,
	};

	class LiarBaseShader:public ILiarRef
	{
	public:
		LiarBaseShader();
		~LiarBaseShader();

	private:
		std::string m_path;
		std::string m_shaderCode;

	public:
		std::string& GetPath() { return m_path; };
		void SetPath(const char*);
		void SetPath(const std::string&);

		const char* GetShaderCode() const { return m_shaderCode.c_str(); };
	};

	class LiarShaderProgram:public ILiarRef
	{
	public:
		LiarShaderProgram();
		~LiarShaderProgram();

	private:
		unsigned int m_ID;
		std::string m_name;

	public:
		void LinkProgram(const char*, const char*);
		void LinkProgram(const std::string&, const std::string&);
#ifndef PLUGINS
		void LinkProgram(const Liar::LiarBaseShader&, const Liar::LiarBaseShader&);
#endif // PLUGINS

		void SetName(std::string& name) { m_name = name; };
		void SetName(const char* name) { m_name = name; };
		std::string GetName() { return m_name; };
		unsigned int GetID() { return m_ID; };

#ifndef PLUGINS
		void Use();
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
#endif // !PLUGINS
	};
}

