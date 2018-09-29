#include "LiarShader.h"
#include <AssetsMgr.hpp>

namespace Liar
{
	LiarBaseShader::LiarBaseShader() :
		Liar::ILiarRef(),
		m_path(""), m_shaderCode("")
	{

	}

	LiarBaseShader::~LiarBaseShader()
	{

	}

	void LiarBaseShader::SetPath(const char* path)
	{
		if (strcmp(path, m_path.c_str()) != 0)
		{
			m_path = path;
			std::ifstream shaderFile;
			shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
			try
			{
				// open files
				shaderFile.open(path);
				std::stringstream shaderStream;
				// read file's buffer contents into streams
				shaderStream << shaderFile.rdbuf();
				// close file handlers
				shaderFile.close();
				// convert stream into string
				m_shaderCode = shaderStream.str();
			}
			catch (std::ifstream::failure e)
			{
				std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
			}
		}
	}

	void LiarBaseShader::SetPath(const std::string& path)
	{
		SetPath(path.c_str());
	}

	LiarShaderProgram::LiarShaderProgram():
		Liar::ILiarRef(),
		m_ID(0),m_name("")
	{

	}

	LiarShaderProgram::~LiarShaderProgram()
	{

	}

	void LiarShaderProgram::LinkProgram(const std::string& vertexFile, const std::string& fragmentFile)
	{
		LinkProgram(vertexFile.c_str(), fragmentFile.c_str());
	}

	void LiarShaderProgram::LinkProgram(const char* vertexFile, const char* fragmentFile)
	{
#ifndef PLUGINS
		Liar::LiarBaseShader* vertex = AssetsMgr::GetInstance().GetBaseShader(vertexFile);
		Liar::LiarBaseShader* fragment = AssetsMgr::GetInstance().GetBaseShader(fragmentFile);
		LinkProgram(*vertex, *fragment);
#endif // !PLUGINS
	}

#ifndef PLUGINS
	void LiarShaderProgram::LinkProgram(const Liar::LiarBaseShader& vertexShader, const Liar::LiarBaseShader& fragmentShader)
	{
		const char* vShaderCode = vertexShader.GetShaderCode();
		const char * fShaderCode = fragmentShader.GetShaderCode();
		// 2. compile shaders
		unsigned int vertex, fragment;

		// vertex shader
		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vShaderCode, NULL);
		glCompileShader(vertex);
		CheckCompileErrors(vertex, LiarShaderType::SHADER_TYPE_VERTEXT);

		// fragment Shader
		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fShaderCode, NULL);
		glCompileShader(fragment);
		CheckCompileErrors(fragment, LiarShaderType::SHADER_TYPE_FRAGMENT);

		// shader Program
		m_ID = glCreateProgram();
		glAttachShader(m_ID, vertex);
		glAttachShader(m_ID, fragment);
		glLinkProgram(m_ID);
		CheckCompileErrors(m_ID, LiarShaderType::SHADER_TYPE_PROGROM);

		// delete the shaders as they're linked into our program now and no longer necessary
		glDeleteShader(vertex);
		glDeleteShader(fragment);
	}

	// ¼ì²â±¨´í
	void LiarShaderProgram::CheckCompileErrors(unsigned int shader, LiarShaderType type)
	{
		int success;
		char infoLog[DEFAULT_BUFF_SIZE];
		if (type != LiarShaderType::SHADER_TYPE_PROGROM)
		{
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(shader, DEFAULT_BUFF_SIZE, NULL, infoLog);
				std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
			}
		}
		else
		{
			glGetProgramiv(shader, GL_LINK_STATUS, &success);
			if (!success)
			{
				glGetProgramInfoLog(shader, DEFAULT_BUFF_SIZE, NULL, infoLog);
				std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
			}
		}
	}

	void LiarShaderProgram::Use()
	{
		glUseProgram(m_ID);
	}

	// utility uniform functions
	// ------------------------------------------------------------------------
	void LiarShaderProgram::SetInt(const std::string& name, int value) const
	{
		glUniform1i(glGetUniformLocation(m_ID, name.c_str()), value);
	}

	// ------------------------------------------------------------------------
	void LiarShaderProgram::SetBool(const std::string& name, bool value) const
	{
		glUniform1i(glGetUniformLocation(m_ID, name.c_str()), static_cast<int>(value));
	}
	// ------------------------------------------------------------------------

	void LiarShaderProgram::SetFloat(const std::string& name, float value) const
	{
		glUniform1f(glGetUniformLocation(m_ID, name.c_str()), value);
	}

	// ------------------------------------------------------------------------
	void LiarShaderProgram::SetVec2(const std::string& name, const Liar::Vector2D& value) const
	{
		SetVec2(name, value.x, value.y);
	}
	void LiarShaderProgram::SetVec2(const std::string& name, float x, float y) const
	{
		glUniform2f(glGetUniformLocation(m_ID, name.c_str()), x, y);
	}

	// ------------------------------------------------------------------------
	void LiarShaderProgram::SetVec3(const std::string& name, const Liar::Vector3D& value) const
	{
		SetVec3(name, value.x, value.y, value.z);
	}
	void LiarShaderProgram::SetVec3(const std::string& name, float x, float y, float z) const
	{
		glUniform3f(glGetUniformLocation(m_ID, name.c_str()), x, y, z);
	}

	// ------------------------------------------------------------------------
	void LiarShaderProgram::SetVec4(const std::string& name, const Liar::Vector4D& value) const
	{
		SetVec4(name, value.x, value.y, value.z, value.w);
	}
	void LiarShaderProgram::SetVec4(const std::string& name, float x, float y, float z, float w) const
	{
		glUniform4f(glGetUniformLocation(m_ID, name.c_str()), x, y, z, w);
	}

	// ------------------------------------------------------------------------
	void LiarShaderProgram::SetMat2(const std::string& name, const Liar::Matrix2& mat) const
	{
		glUniformMatrix2fv(glGetUniformLocation(m_ID, name.c_str()), 1, GL_FALSE, mat.Get());
	}

	// ------------------------------------------------------------------------
	void LiarShaderProgram::SetMat3(const std::string& name, const Liar::Matrix3& mat) const
	{
		glUniformMatrix3fv(glGetUniformLocation(m_ID, name.c_str()), 1, GL_FALSE, mat.Get());
	}

	// ------------------------------------------------------------------------
	void LiarShaderProgram::SetMat4(const std::string&name, const Liar::Matrix4& m) const
	{
		glUniformMatrix4fv(glGetUniformLocation(m_ID, name.c_str()), 1, GL_FALSE, m.Get());
	}
#endif // !PLUGINS

}
