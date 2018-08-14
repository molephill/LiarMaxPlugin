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

	LiarGeometry::LiarGeometry():m_vertexOpen(0)
	{
		m_allVertexBuffers = new std::vector<Liar::LiarVertexBuffer*>();
		m_bufferSize = 0;

		m_indices = new std::vector<unsigned int>();
	}

	LiarGeometry::~LiarGeometry()
	{
		EraseIndexBuff(0);
		std::vector<Liar::LiarVertexBuffer*>().swap(*m_allVertexBuffers);
		delete m_allVertexBuffers;

		std::vector<unsigned int>().swap(*m_indices);
		delete m_indices;
	}

	void LiarGeometry::EraseIndexBuff(int index)
	{
		for (std::vector<Liar::LiarVertexBuffer*>::iterator it = m_allVertexBuffers->begin() + index; it != m_allVertexBuffers->end();)
		{
			delete *it;
			it = m_allVertexBuffers->erase(it);
			--m_bufferSize;
		}
	}

	std::ostream& operator<<(std::ostream& os, const Liar::LiarGeometry& m)
	{
		os << "buffSize: " << m.m_bufferSize << "\n";
		for (size_t i = 0; i < m.m_bufferSize; ++i)
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

		size_t positionSize = Liar::LiarVertexBuffer::GetPositionSize();
		size_t normalSize = Liar::LiarVertexBuffer::GetNormalSize();
		size_t colorSize = Liar::LiarVertexBuffer::GetColorSize();
		size_t uvSize = Liar::LiarVertexBuffer::GetUVSize();

		bool pos = Liar::LairVersionCtr::CheckVertexOpen(m_vertexOpen, LIAR_POSITION);
		bool normal = Liar::LairVersionCtr::CheckVertexOpen(m_vertexOpen, LIAR_NORMAL);
		bool color = Liar::LairVersionCtr::CheckVertexOpen(m_vertexOpen, LIAR_COLOR);
		bool uv = Liar::LairVersionCtr::CheckVertexOpen(m_vertexOpen, LIAR_UV);

		size_t positionOffSize = Liar::LiarVertexBuffer::GetPositionOffSize();
		size_t normalOffSize = Liar::LiarVertexBuffer::GetNormalOffSize();
		size_t colorOffSize = Liar::LiarVertexBuffer::GetColorOffSize();
		size_t uvOffSize = Liar::LiarVertexBuffer::GetUVOffSize();

		size_t oneSize = positionSize;
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
		size_t totalSize = m_bufferSize * oneSize;

		glBufferData(GL_ARRAY_BUFFER, totalSize, nullptr, GL_STATIC_DRAW);
		for (int i = 0; i < m_bufferSize; ++i)
		{
			size_t start = i * oneSize;
			Liar::LiarVertexBuffer* buffData = m_allVertexBuffers->at(i);
			glBufferSubData(GL_ARRAY_BUFFER, start + positionOffSize, positionSize, buffData->position);
			if (normal) glBufferSubData(GL_ARRAY_BUFFER, start + normalOffSize, normalSize, buffData->normal);
			if (uv) glBufferSubData(GL_ARRAY_BUFFER, start + uvOffSize, uvSize, buffData->uv);
			if (color) glBufferSubData(GL_ARRAY_BUFFER, start + colorOffSize, colorSize, buffData->color);
		}

		size_t indiceSize1 = GetIndicesSize() * sizeof(unsigned int);
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
			glVertexAttribPointer(curId, 2, GL_FLOAT, GL_FALSE, oneSize, (void*)uvOffSize);
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
		, m_material(new Liar::LiarMaterial())
	{
#ifndef PLUGINS
		m_refCount = 0;
#endif // !PLUGINS

	}


	LiarMesh::~LiarMesh()
	{
		delete m_geometry;
		delete m_material;
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
		m_geometry->Render();
		m_material->Render(shader);
	}

#endif // PLUGINS


}
