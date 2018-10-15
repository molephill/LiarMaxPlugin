#include "LiarVertexBuffer.h"

namespace Liar
{
	// =============================== Nodes ===============================

	LiarNode::LiarNode()
		:m_nodeName(""), m_children(nullptr), meshIndex(0)
	{

	}

	LiarNode::~LiarNode()
	{
		if (m_children)
		{
			for (std::vector<Liar::LiarNode*>::iterator it = m_children->begin(); it != m_children->end();)
			{
				delete (*it);
				it = m_children->erase(it);
			}
			std::vector<Liar::LiarNode*>().swap(*m_children);
			delete m_children;
		}
	}

	Liar::LiarNode* LiarNode::AddChild()
	{
		if (!m_children) m_children = new std::vector<Liar::LiarNode*>();
		Liar::LiarNode* node = new Liar::LiarNode();
		m_children->push_back(node);
		return node;
	}

	// =================================== anim_skin_define ===================================
	LiarAnimSkinDefine::LiarAnimSkinDefine(int posIndex) :
		m_positionIndex(posIndex),
		m_boneIds(new std::vector<int>()),
		m_weights(new std::vector<float>())
	{
	}

	LiarAnimSkinDefine::~LiarAnimSkinDefine()
	{
		std::vector<int>().swap(*m_boneIds);
		delete m_boneIds;
		m_boneIds = nullptr;

		std::vector<float>().swap(*m_weights);
		delete m_weights;
		m_weights = nullptr;
	}

	void LiarAnimSkinDefine::AddBoneInfo(int boneId, float weight)
	{
		if (weight > 0)
		{
			bool find = false;
			for (size_t i = 0; i < m_boneIds->size(); ++i)
			{
				if (m_boneIds->at(i) == boneId)
				{
					find = true;
					break;
				}
			}

			if (!find)
			{
				m_boneIds->push_back(boneId);
				m_weights->push_back(weight);
			}
		}
	}

	// ================ vertex raw data ==============================
	LiarMeshRawData::LiarMeshRawData() :
		m_pos(new std::vector<Vector3D*>()), 
		m_faces(new std::vector<Liar::LiarFaceDefine*>()),
		m_indices(new std::vector<int>()),
		meshName(""),saveName(""),
		m_norm(nullptr), m_color(nullptr), m_texCoord(nullptr), m_skinDefines(nullptr),
		m_materials(nullptr)
	{
	}

	void LiarMeshRawData::EraseAll(std::vector<Liar::Vector3D*>* vec)
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

	LiarMeshRawData::~LiarMeshRawData()
	{
		EraseAll(m_pos);
		EraseAll(m_norm);
		EraseAll(m_color);
		EraseAll(m_texCoord);
		m_pos = nullptr;
		m_norm = nullptr;
		m_color = nullptr;
		m_texCoord = nullptr;

		for (std::vector<Liar::LiarFaceDefine*>::iterator it = m_faces->begin(); it != m_faces->end(); ++it)
		{
			delete (*it);
		}
		delete m_faces;

		if (m_materials)
		{
			for (std::vector<Liar::LiarMaterialDefine*>::iterator it = m_materials->begin(); it != m_materials->end(); ++it)
			{
				delete (*it);
			}
			delete m_materials;
		}
	}

	Liar::LiarAnimSkinDefine* LiarMeshRawData::GetAnimSkinDefine(int verIndex, bool add)
	{
		if (!m_skinDefines) m_skinDefines = new std::vector<Liar::LiarAnimSkinDefine*>();
		for (std::vector<Liar::LiarAnimSkinDefine*>::iterator it = m_skinDefines->begin(); it != m_skinDefines->end(); ++it)
		{
			if ((*it)->GetPositionIndex() == verIndex)
			{
				return *it;
			}
		}

		if (add)
		{
			Liar::LiarAnimSkinDefine* skinDefine = new Liar::LiarAnimSkinDefine();
			skinDefine->SetPositionIndex(verIndex);
			m_skinDefines->push_back(skinDefine);
			return skinDefine;
		}
		else
		{
			return nullptr;
		}
	}

	Liar::Vector3D* LiarMeshRawData::AddData(Liar::LiarVertexType type, size_t index)
	{
		std::vector<Liar::Vector3D*>* vec = GetData(type);
		while (vec->size() <= index)
		{
			vec->push_back(new Liar::Vector3D());
		}
		return vec->at(index);
	}

	int LiarMeshRawData::GetIndex(Liar::LiarVertexType type, float x, float y, float z)
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

	int LiarMeshRawData::GetIndex(Liar::LiarVertexType type, const Liar::Vector3D& v)
	{
		return GetIndex(type, v.x, v.y, v.z);
	}

	std::vector<Liar::Vector3D*>* LiarMeshRawData::GetData(Liar::LiarVertexType type)
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

	Liar::Vector3D* LiarMeshRawData::AddPos(size_t index)
	{
		while (m_pos->size() <= index)
		{
			m_pos->push_back(new Liar::Vector3D());
		}
		return m_pos->at(index);
	}

	Liar::Vector3D* LiarMeshRawData::AddNorm(size_t index)
	{
		if (!m_norm) m_norm = new std::vector<Liar::Vector3D*>();
		while (m_norm->size() <= index)
		{
			m_norm->push_back(new Liar::Vector3D());
		}
		return m_norm->at(index);
	}

	Liar::Vector3D* LiarMeshRawData::AddColor(size_t index)
	{
		if (!m_color) m_color = new std::vector<Liar::Vector3D*>();
		while (m_color->size() <= index)
		{
			m_color->push_back(new Liar::Vector3D());
		}
		return m_color->at(index);
	}

	Liar::Vector3D* LiarMeshRawData::AddTex(size_t index)
	{
		if (!m_texCoord) m_texCoord = new std::vector<Liar::Vector3D*>();
		while (m_texCoord->size() <= index)
		{
			m_texCoord->push_back(new Liar::Vector3D());
		}
		return m_texCoord->at(index);
	}

	Liar::LiarFaceDefine* LiarMeshRawData::FindFaceDefine(const Liar::LiarFaceDefine& rhs)
	{
		return FindFaceDefine(rhs.positionIndex, rhs.normalIndex, rhs.texCoordIndex, rhs.colorIndex);
	}

	Liar::LiarFaceDefine* LiarMeshRawData::FindFaceDefine(unsigned int posIndex, unsigned int normIndex, unsigned int texIndex, unsigned int cor)
	{
		for (std::vector<Liar::LiarFaceDefine*>::iterator it = m_faces->begin(); it != m_faces->end(); ++it)
		{
			if ((*it)->Equal(posIndex, normIndex, texIndex, cor))
			{
				return *it;
			}
		}
		return nullptr;
	}

	int LiarMeshRawData::FindFaceDefineIndex(const Liar::LiarFaceDefine& rhs) const
	{
		return FindFaceDefineIndex(rhs.positionIndex, rhs.normalIndex, rhs.texCoordIndex, rhs.colorIndex);
	}

	int LiarMeshRawData::FindFaceDefineIndex(unsigned int posIndex, unsigned int normIndex, unsigned int texIndex, unsigned int cor) const
	{
		size_t size = GetFaceSize();
		for (size_t i = 0; i < size; ++i)
		{
			Liar::LiarFaceDefine* tmp = m_faces->at(i);
			if (tmp->Equal(posIndex, normIndex, texIndex, cor))
			{
				return static_cast<int>(i);
			}
		}
		return -1;
	}

	// ================ vertex raw data ==============================

	LiarVertexBuffer::LiarVertexBuffer():position(nullptr), normal(nullptr), color(nullptr), uv(nullptr)
	{
	}

	LiarVertexBuffer::~LiarVertexBuffer()
	{
	}

}
