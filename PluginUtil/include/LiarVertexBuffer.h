#pragma once

#include "Vectors.h"
#include <vector>

#ifndef PLUGINS
#include <glad/glad.h>
#endif // !PLUGINS


namespace Liar
{
	class LiarVertexDefine
	{
	public:
		LiarVertexDefine() :positionIndex(0), normalIndex(0), texCoordIndex(0), colorIndex(0) {};
		LiarVertexDefine(unsigned int posIndex, unsigned int normIndex, unsigned int texIndex, unsigned int corIndex) :
			positionIndex(posIndex), normalIndex(normIndex), texCoordIndex(texIndex), colorIndex(corIndex) {}
		~LiarVertexDefine() {};
		
	public:
		unsigned int positionIndex;
		unsigned int normalIndex;
		unsigned int texCoordIndex;
		unsigned int colorIndex;

	public:
		bool operator==(const LiarVertexDefine& rhs) const
		{
			return rhs.positionIndex == positionIndex && rhs.normalIndex == normalIndex && rhs.texCoordIndex == texCoordIndex && rhs.colorIndex == colorIndex;
		}

		bool Equal(unsigned int posIndex, unsigned int normIndex, unsigned int texIndex, unsigned int corIndex = 0)
		{
			return positionIndex == posIndex && normalIndex == normIndex && texCoordIndex == texIndex && colorIndex == corIndex;
		}

	};

	class LiarVertexRawData
	{
	public:
		LiarVertexRawData();
		~LiarVertexRawData();

	private:
		std::vector<Vector3D*>* m_pos;					// pos
		std::vector<Vector3D*>* m_norm;					// normal
		std::vector<Vector3D*>* m_color;				// color
		std::vector<Vector3D*>* m_texCoord;				// tex

	public:
		std::vector<Vector3D*>* GetPos() { return m_pos; };
		Vector3D* GetPos(size_t index) { return m_pos ? m_pos->at(index) : nullptr; };

		std::vector<Vector3D*>* GetNorm() { return m_norm; };
		Vector3D* GetNorm(size_t index) { return m_norm ? m_norm->at(index) : nullptr; };

		std::vector<Vector3D*>* GetColor() { return m_color; };
		Vector3D* GetColor(size_t index) { return m_color ? m_color->at(index) : nullptr; };

		std::vector<Vector3D*>* GetTexCoord() { return m_texCoord; };
		Vector3D* GetTexCoord(size_t index) { return m_texCoord ? m_texCoord->at(index) : nullptr; };

		void SetPos(std::vector<Vector3D*>* v) { m_pos = v; };
		void SetNorm(std::vector<Vector3D*>* v) { m_norm = v; };
		void SetColor(std::vector<Vector3D*>* v) { m_color = v; };
		void SetTexCoord(std::vector<Vector3D*>* v) { m_texCoord = v; };

		Liar::Vector3D* AddPos(size_t);
		Liar::Vector3D* AddNorm(size_t);
		Liar::Vector3D* AddColor(size_t);
		Liar::Vector3D* AddTex(size_t);

		Liar::Vector3D* AddData(int, size_t);
		int GetIndex(int, float, float, float);
		int GetIndex(int, const Liar::Vector3D&);

	private:
		std::vector<Liar::Vector3D*>* GetData(int);
	};

	class LiarVertexBuffer
	{
	public:
		LiarVertexBuffer();
		~LiarVertexBuffer();

	public:
		Liar::Vector3D* position;
		Liar::Vector3D* normal;
		Liar::Vector3D* color;
		Liar::Vector3D* uv;

		friend std::ostream& operator<<(std::ostream& os, const Liar::LiarVertexBuffer& m)
		{
			if (m.position) os << "position: " << *(m.position) << "\n";
			if (m.normal) os << "normal: " << *(m.normal) << "\n";
			if (m.color) os << "color: " << *(m.color) << "\n";
			if (m.uv) os << "uv: " << *(m.uv) << "\n";
			return os;
		}

#ifndef PLUGINS
	public:
		static int GetPositionSize() { return sizeof(Liar::Vector3D); };
		static int GetNormalSize() { return sizeof(Liar::Vector3D); };
		static int GetColorSize() { return sizeof(Liar::Vector3D); };
		static int GetUVSize() { return sizeof(Liar::Vector3D); };

		static int GetPositionOffSize() { return 0; };
		static int GetNormalOffSize() { return Liar::LiarVertexBuffer::GetPositionSize(); };
		static int GetColorOffSize()
		{
			return Liar::LiarVertexBuffer::GetNormalOffSize() + Liar::LiarVertexBuffer::GetNormalSize();
		}

		static int GetUVOffSize()
		{
			return Liar::LiarVertexBuffer::GetColorOffSize() + Liar::LiarVertexBuffer::GetColorSize();
		}

		static int GetBuffSize() 
		{
			return Liar::LiarVertexBuffer::GetPositionSize() + Liar::LiarVertexBuffer::GetNormalSize()
				+ Liar::LiarVertexBuffer::GetColorSize() + Liar::LiarVertexBuffer::GetUVSize();
		}

	public:
		void Upload(size_t);
#endif // !PLUGINS

	};
}

