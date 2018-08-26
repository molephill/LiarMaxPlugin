#include "LiarMesh.h"

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


	// =============================== Nodes ===============================

	// =============================== Geometory ===============================

	LiarGeometry::LiarGeometry():
		m_rawData(new Liar::LiarVertexRawData())
		, m_vertexFaces(new std::vector<Liar::LiarVertexDefine*>())
	{
#ifndef PLUGINS
		m_allVertexBuffers = new std::vector<Liar::LiarVertexBuffer*>();
#endif // PLUGINS
		m_indices = new std::vector<unsigned int>();
		m_vertexOpen = 0;
	}

	LiarGeometry::~LiarGeometry()
	{
#ifdef PLUGINS
		if (m_allVertexBuffers)
		{
#endif // PLUGINS

			EraseIndexBuff(0);
			std::vector<Liar::LiarVertexBuffer*>().swap(*m_allVertexBuffers);
			delete m_allVertexBuffers;
#ifdef PLUGINS
		}
#endif // PLUGINS

		std::vector<unsigned int>().swap(*m_indices);
		delete m_indices;

		delete m_rawData;
	}

	Liar::LiarVertexBuffer* LiarGeometry::GetBuffer(size_t index)
	{
		if (m_allVertexBuffers && index < m_allVertexBuffers->size())
		{
			return m_allVertexBuffers->at(index);
		}
		else
		{
			return nullptr;
		}
	}

	void LiarGeometry::EraseIndexBuff(int index)
	{
#ifdef PLUGINS
		if (m_allVertexBuffers)
		{
#endif // PLUGINS
			for (std::vector<Liar::LiarVertexBuffer*>::iterator it = m_allVertexBuffers->begin() + index; it != m_allVertexBuffers->end();)
			{
				delete *it;
				it = m_allVertexBuffers->erase(it);
			}
#ifdef PLUGINS
		}
#endif // PLUGINS

	}

	Liar::LiarVertexDefine* LiarGeometry::FindVertexDefine(const Liar::LiarVertexDefine& rhs)
	{
		return FindVertexDefine(rhs.positionIndex, rhs.normalIndex, rhs.texCoordIndex, rhs.colorIndex);
	}

	Liar::LiarVertexDefine* LiarGeometry::FindVertexDefine(unsigned int posIndex, unsigned int normIndex, unsigned int texIndex, unsigned int cor)
	{
		for (std::vector<Liar::LiarVertexDefine*>::iterator it = m_vertexFaces->begin(); it != m_vertexFaces->end(); ++it)
		{
			if ((*it)->Equal(posIndex, normIndex, texIndex, cor))
			{
				return *it;
			}
		}
		return nullptr;
	}

	int LiarGeometry::FindVertexDefineIndex(const Liar::LiarVertexDefine& rhs) const
	{
		return FindVertexDefineIndex(rhs.positionIndex, rhs.normalIndex, rhs.texCoordIndex, rhs.colorIndex);
	}

	int LiarGeometry::FindVertexDefineIndex(unsigned int posIndex, unsigned int normIndex, unsigned int texIndex, unsigned int cor) const
	{
		size_t size = GetVertexFaceSize();
		for (size_t i = 0; i < size; ++i)
		{
			Liar::LiarVertexDefine* tmp = m_vertexFaces->at(i);
			if (tmp->Equal(posIndex, normIndex, texIndex, cor))
			{
				return static_cast<int>(i);
			}
		}
		return -1;
	}

	std::ostream& operator<<(std::ostream& os, const Liar::LiarGeometry& m)
	{
		size_t buffSize = m.m_allVertexBuffers->size();
		os << "buffSize: " << buffSize << "\n";
		for (size_t i = 0; i < buffSize; ++i)
		{
			os << "buffer index: " << i << "\nbuffer info: " << *(m.m_allVertexBuffers->at(i)) << "\n";
		}

		size_t indicesSize = m.m_indices->size();
		os << "indicesSize: " << indicesSize << "\n";
		for (size_t i = 0; i < indicesSize;)
		{
			os << m.m_indices->at(i++) << "," << m.m_indices->at(i++) << "," << m.m_indices->at(i++) << "\n";
		}
		return os;
	}

#ifndef PLUGINS
	void LiarGeometry::Upload()
	{
		// create buffers/arrays
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		// ============================ fill_data ======================
		size_t size = m_vertexFaces->size();
		for (size_t i = 0; i < size; ++i)
		{
			Liar::LiarVertexDefine* tmp = m_vertexFaces->at(i);
			Liar::LiarVertexBuffer* buffer = new Liar::LiarVertexBuffer();
			buffer->position = m_rawData->GetPos(tmp->positionIndex);
			buffer->normal = m_rawData->GetNorm(tmp->normalIndex);
			buffer->color = m_rawData->GetColor(tmp->colorIndex);
			buffer->uv = m_rawData->GetTexCoord(tmp->texCoordIndex);
			m_allVertexBuffers->push_back(buffer);
		}
		// ============================ fill_data ======================

		int positionSize = Liar::LiarVertexBuffer::GetPositionSize();
		int normalSize = Liar::LiarVertexBuffer::GetNormalSize();
		int colorSize = Liar::LiarVertexBuffer::GetColorSize();
		int uvSize = Liar::LiarVertexBuffer::GetUVSize();

		bool normal = Liar::LairVersionCtr::CheckVertexOpen(m_vertexOpen, LIAR_NORMAL);
		bool color = Liar::LairVersionCtr::CheckVertexOpen(m_vertexOpen, LIAR_COLOR);
		bool uv = Liar::LairVersionCtr::CheckVertexOpen(m_vertexOpen, LIAR_UV);

		int positionOffSize = Liar::LiarVertexBuffer::GetPositionOffSize();
		int normalOffSize = Liar::LiarVertexBuffer::GetNormalOffSize();
		int colorOffSize = Liar::LiarVertexBuffer::GetColorOffSize();
		int uvOffSize = Liar::LiarVertexBuffer::GetUVOffSize();

		int oneSize = positionSize;
		if (normal) 
		{
			oneSize += normalSize;
		}
		else
		{
			normalSize = 0;
		}
		normalOffSize = positionOffSize + normalSize;

		if (uv)
		{
			oneSize += uvSize;
		}
		else
		{
			uvSize = 0;
		}
		uvOffSize = normalOffSize + uvSize;

		if (color)
		{
			oneSize += colorSize;
		}
		else
		{
			colorSize = 0;
		}
		colorOffSize = uvOffSize + normalSize;


		//size_t oneSize = Liar::LiarVertexBuffer::GetBuffSize();
		int bufferSize = static_cast<int>(m_allVertexBuffers->size());
		int totalSize = bufferSize * oneSize;

		glBufferData(GL_ARRAY_BUFFER, totalSize, nullptr, GL_STATIC_DRAW);
		for (size_t i = 0; i < bufferSize; ++i)
		{
			size_t start = i * oneSize;
			Liar::LiarVertexBuffer* buffData = m_allVertexBuffers->at(i);
			glBufferSubData(GL_ARRAY_BUFFER, start + positionOffSize, positionSize, buffData->position);
			if (normal) glBufferSubData(GL_ARRAY_BUFFER, start + normalOffSize, normalSize, buffData->normal);
			if (uv) glBufferSubData(GL_ARRAY_BUFFER, start + uvOffSize, uvSize, buffData->uv);
			if (color) glBufferSubData(GL_ARRAY_BUFFER, start + colorOffSize, colorSize, buffData->color);
		}

		int indiceSize1 = GetIndicesSize() * sizeof(unsigned int);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indiceSize1, m_indices->data(), GL_STATIC_DRAW);

		unsigned int curId = 0;
		// position attribute
		glVertexAttribPointer(curId, 3, GL_FLOAT, GL_FALSE, oneSize, (void*)positionOffSize);
		glEnableVertexAttribArray(curId);
		++curId;

		// normal attribute
		if (normal)
		{
			glVertexAttribPointer(curId, 3, GL_FLOAT, GL_FALSE, oneSize, (void*)normalOffSize);
			glEnableVertexAttribArray(curId);
			++curId;
		}

		// texture coord attribute
		if (uv)
		{
			glVertexAttribPointer(curId, 3, GL_FLOAT, GL_FALSE, oneSize, (void*)uvOffSize);
			glEnableVertexAttribArray(curId);
			++curId;
		}

		// color attribute
		if (color)
		{
			glVertexAttribPointer(curId, 3, GL_FLOAT, GL_FALSE, oneSize, (void*)colorOffSize);
			glEnableVertexAttribArray(curId);
		}

	}

	void LiarGeometry::Render()
	{
		// draw mesh
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, GetIndicesSize(), GL_UNSIGNED_INT, 0);
	}

#endif // !PLUGINS

	// =============================== Geometory ===============================

	LiarMesh::LiarMesh() :
		m_geometry(new Liar::LiarGeometry())
		, m_materials(new std::vector<Liar::LiarMaterial*>())
	{
#ifndef PLUGINS
		m_refCount = 0;
#endif // !PLUGINS

	}


	LiarMesh::~LiarMesh()
	{
		delete m_geometry;
		delete m_materials;
	}

	void LiarMesh::EraseMaterial(int index)
	{
		for (std::vector<Liar::LiarMaterial*>::iterator it = m_materials->begin() + index; it != m_materials->end();)
		{
			delete *it;
			it = m_materials->erase(it);
		}
	}

	std::ostream& operator<<(std::ostream& os, const Liar::LiarMesh& m)
	{
		os << "mesh`s name: " << m.meshName << "\n";
		os << "geometory:\n" << *m.m_geometry << "\n";
		return os;
	}

#ifndef PLUGINS
	void LiarMesh::Upload()
	{
		m_geometry->Upload();
	}


	void LiarMesh::Render(Liar::Shader& shader)
	{
		for (size_t i = 0; i < m_materials->size(); ++i)
		{
			m_materials->at(i)->Render(shader);
		}
		m_geometry->Render();
	}

#endif // PLUGINS


}
