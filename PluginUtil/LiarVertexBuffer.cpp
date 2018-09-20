#include "LiarVertexBuffer.h"

namespace Liar
{
	// =================================== anim_skin_define ===================================
	LiarAnimSkinDefine::LiarAnimSkinDefine() :m_vertIndex(0), m_skins(new std::vector<Liar::LiarSkin*>())
	{
	}

	LiarAnimSkinDefine::~LiarAnimSkinDefine()
	{
		for (std::vector<LiarSkin*>::iterator it = m_skins->begin(); it != m_skins->end();)
		{
			delete (*it);
			it = m_skins->erase(it);
		}
		delete m_skins;
	}

	Liar::LiarSkin* LiarAnimSkinDefine::AddSkin(int boneId, float weight)
	{
		for (std::vector<Liar::LiarSkin*>::iterator it = m_skins->begin(); it != m_skins->end(); ++it)
		{
			if ((*it)->GetBoneId() == boneId)
			{
				return *it;
			}
		}

		Liar::LiarSkin* skin = new Liar::LiarSkin(boneId);
		skin->SetWeight(weight);
		m_skins->push_back(skin);
		return skin;
	}

	// ================ vertex raw data ==============================
	LiarVertexRawData::LiarVertexRawData() :m_pos(new std::vector<Vector3D*>())
		, m_norm(nullptr), m_color(nullptr), m_texCoord(nullptr), m_skinDefines(nullptr)
	{
	}

	void LiarVertexRawData::EraseAll(std::vector<Liar::Vector3D*>* vec)
	{
		if (vec)
		{
			for (std::vector<Vector3D*>::iterator it = vec->begin(); it != vec->end();)
			{
				delete (*it);
				it = vec->erase(it);
			}
			delete vec;
		}
	}

	LiarVertexRawData::~LiarVertexRawData()
	{
		EraseAll(m_pos);
		EraseAll(m_norm);
		EraseAll(m_color);
		EraseAll(m_texCoord);
		m_pos = nullptr;
		m_norm = nullptr;
		m_color = nullptr;
		m_texCoord = nullptr;
	}

	Liar::LiarAnimSkinDefine* LiarVertexRawData::GetAnimSkinDefine(int verIndex, bool add)
	{
		if (!m_skinDefines) m_skinDefines = new std::vector<Liar::LiarAnimSkinDefine*>();
		for (std::vector<Liar::LiarAnimSkinDefine*>::iterator it = m_skinDefines->begin(); it != m_skinDefines->end(); ++it)
		{
			if ((*it)->GetVertIndex() == verIndex)
			{
				return *it;
			}
		}

		if (add)
		{
			Liar::LiarAnimSkinDefine* skinDefine = new Liar::LiarAnimSkinDefine();
			skinDefine->SetVertIndex(verIndex);
			m_skinDefines->push_back(skinDefine);
			return skinDefine;
		}
		else
		{
			return nullptr;
		}
	}

	Liar::Vector3D* LiarVertexRawData::AddData(Liar::LiarVertexType type, size_t index)
	{
		std::vector<Liar::Vector3D*>* vec = GetData(type);
		while (vec->size() <= index)
		{
			vec->push_back(new Liar::Vector3D());
		}
		return vec->at(index);
	}

	int LiarVertexRawData::GetIndex(Liar::LiarVertexType type, float x, float y, float z)
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

	int LiarVertexRawData::GetIndex(Liar::LiarVertexType type, const Liar::Vector3D& v)
	{
		return GetIndex(type, v.x, v.y, v.z);
	}

	std::vector<Liar::Vector3D*>* LiarVertexRawData::GetData(Liar::LiarVertexType type)
	{
		std::vector<Liar::Vector3D*>* vec = nullptr;
		if (type == Liar::LiarVertexType::LiarVertexType_COLOR)
		{
			if (!m_color) m_color = new std::vector<Liar::Vector3D*>();
			vec = m_color;
		}
		else if (type == Liar::LiarVertexType::LiarVertexType_NORMAL)
		{
			if (!m_norm) m_norm = new std::vector<Liar::Vector3D*>();
			vec = m_norm;
		}
		else if (type == Liar::LiarVertexType::LiarVertexType_TEX)
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
