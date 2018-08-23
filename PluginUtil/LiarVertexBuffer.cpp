#include "LiarVertexBuffer.h"
#include <PluginDefine.h>

namespace Liar
{
	// ================ vertex raw data ==============================
	LiarVertexRawData::LiarVertexRawData() :m_pos(new std::vector<Vector3D*>()), m_norm(nullptr), m_color(nullptr), m_texCoord(nullptr)
	{
	}

	LiarVertexRawData::~LiarVertexRawData()
	{
		for (std::vector<Vector3D*>::iterator it = m_pos->begin(); it != m_pos->end();)
		{
			delete (*it);
			it = m_pos->erase(it);
		}
		delete m_pos;

		if (m_norm)
		{
			for (std::vector<Vector3D*>::iterator it = m_norm->begin(); it != m_norm->end();)
			{
				delete (*it);
				it = m_norm->erase(it);
			}
			delete m_norm;
		}

		if (m_color)
		{
			for (std::vector<Vector3D*>::iterator it = m_color->begin(); it != m_color->end();)
			{
				delete (*it);
				it = m_color->erase(it);
			}
			delete m_color;
		}

		if (m_color)
		{
			for (std::vector<Vector3D*>::iterator it = m_texCoord->begin(); it != m_texCoord->end();)
			{
				delete (*it);
				it = m_texCoord->erase(it);
			}
			delete m_texCoord;
		}
	}

	Liar::Vector3D* LiarVertexRawData::AddData(int type, size_t index)
	{
		std::vector<Liar::Vector3D*>* vec = GetData(type);
		while (vec->size() <= index)
		{
			vec->push_back(new Liar::Vector3D());
		}
		return vec->at(index);
	}

	int LiarVertexRawData::GetIndex(int type, float x, float y, float z)
	{
		std::vector<Liar::Vector3D*>* vec = GetData(type);
		size_t size = vec->size();
		for (size_t i = 0; i < size; ++i)
		{
			Liar::Vector3D* tmp = vec->at(i);
			if (tmp->Equal(x, y, z)) return static_cast<int>(i);
		}

		return static_cast<int>(size);
	}

	int LiarVertexRawData::GetIndex(int type, const Liar::Vector3D& v)
	{
		return GetIndex(type, v.x, v.y, v.z);
	}

	std::vector<Liar::Vector3D*>* LiarVertexRawData::GetData(int type)
	{
		std::vector<Liar::Vector3D*>* vec = nullptr;
		if (type == LIAR_COLOR)
		{
			if (!m_color) m_color = new std::vector<Liar::Vector3D*>();
			vec = m_color;
		}
		else if (type == LIAR_NORMAL)
		{
			if (!m_norm) m_norm = new std::vector<Liar::Vector3D*>();
			vec = m_norm;
		}
		else if (type == LIAR_UV)
		{
			if (!m_texCoord) m_texCoord = new std::vector<Liar::Vector3D*>();
			vec = m_texCoord;
		}
		else
		{
			vec = m_pos;
		}
		return vec;
	}

	Liar::Vector3D* LiarVertexRawData::AddPos(size_t index)
	{
		while (m_pos->size() <= index)
		{
			m_pos->push_back(new Liar::Vector3D());
		}
		return m_pos->at(index);
	}

	Liar::Vector3D* LiarVertexRawData::AddNorm(size_t index)
	{
		if (!m_norm) m_norm = new std::vector<Liar::Vector3D*>();
		while (m_norm->size() <= index)
		{
			m_norm->push_back(new Liar::Vector3D());
		}
		return m_norm->at(index);
	}

	Liar::Vector3D* LiarVertexRawData::AddColor(size_t index)
	{
		if (!m_color) m_color = new std::vector<Liar::Vector3D*>();
		while (m_color->size() <= index)
		{
			m_color->push_back(new Liar::Vector3D());
		}
		return m_color->at(index);
	}

	Liar::Vector3D* LiarVertexRawData::AddTex(size_t index)
	{
		if (!m_texCoord) m_texCoord = new std::vector<Liar::Vector3D*>();
		while (m_texCoord->size() <= index)
		{
			m_texCoord->push_back(new Liar::Vector3D());
		}
		return m_texCoord->at(index);
	}

	// ================ vertex raw data ==============================

	LiarVertexBuffer::LiarVertexBuffer():position(nullptr), normal(nullptr), color(nullptr), uv(nullptr)
	{
	}

	LiarVertexBuffer::~LiarVertexBuffer()
	{
	}

#ifndef PLUGINS
	void LiarVertexBuffer::Upload(size_t start)
	{
		size_t positionSize = Liar::LiarVertexBuffer::GetPositionSize();
		size_t normalSize = LiarVertexBuffer::GetNormalSize();
		size_t colorSize = Liar::LiarVertexBuffer::GetColorSize();
		size_t uvSize = Liar::LiarVertexBuffer::GetUVSize();

		size_t normalOffSize = positionSize;
		size_t colorOffSize = positionSize + normalOffSize;
		size_t uvOffSize = colorOffSize + colorSize;

		glBufferSubData(GL_ARRAY_BUFFER, start, positionSize, position);
		glBufferSubData(GL_ARRAY_BUFFER, start + normalOffSize, normalSize, normal);
		glBufferSubData(GL_ARRAY_BUFFER, start + colorOffSize, colorSize, color);
		glBufferSubData(GL_ARRAY_BUFFER, start + uvOffSize, uvSize, uv);
	}
#endif // !PLUGINS


}
