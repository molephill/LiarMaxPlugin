#include "LiarGeometries.h"

namespace Liar
{
	// =============================== Geometory ===============================

#ifndef PLUGINS
	LiarBaseGeometry::LiarBaseGeometry() :
		Liar::LiarReference(),
		m_vertexArrayID(0), m_elementbuffer(0), m_vertexbuffer(0),
		m_indices(new std::vector<unsigned int>()), m_indiceSize(0)
	{

	}
#else
	LiarBaseGeometry::LiarBaseGeometry() :
		Liar::LiarReference(),
		m_indices(new std::vector<unsigned int>()), m_indiceSize(0)
	{

	}
#endif // !PLUGINS


	LiarBaseGeometry::~LiarBaseGeometry()
	{
#ifndef PLUGINS
		if (m_vertexArrayID > 0) glDeleteBuffers(1, &m_vertexArrayID);
		if (m_vertexbuffer > 0) glDeleteBuffers(1, &m_vertexbuffer);
		if (m_elementbuffer > 0) glDeleteBuffers(1, &m_elementbuffer);
#endif // !PLUGINS
	}

#ifndef PLUGINS
	void LiarBaseGeometry::Upload()
	{
		glGenVertexArrays(1, &m_vertexArrayID);
		glBindVertexArray(m_vertexArrayID);


		glGenBuffers(1, &m_vertexbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, m_vertexbuffer);

		UploadSub();

		int indiceSize1 = GetIndicesSize() * sizeof(unsigned int);
		glGenBuffers(1, &m_elementbuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_elementbuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indiceSize1, m_indices->data(), GL_STATIC_DRAW);

		ReleaseSourceData();
	}

	void LiarBaseGeometry::Render()
	{
		// draw mesh
		glBindVertexArray(m_vertexArrayID);
		glDrawElements(GL_TRIANGLES, m_indiceSize, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}

	void LiarBaseGeometry::ReleaseSourceData()
	{
		m_indiceSize = m_indices->size();
		std::vector<unsigned int>().swap(*m_indices);
		delete m_indices;
	}
#endif // !PLUGINS

	LiarGeometry::LiarGeometry() :
		Liar::LiarBaseGeometry(),
		m_rawData(new Liar::LiarVertexRawData()),
		m_vertexFaces(new std::vector<Liar::LiarVertexDefine*>()),
		m_vertexOpen(0)
	{
	}

	LiarGeometry::~LiarGeometry()
	{

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
		size_t indicesSize = m.m_indices->size();
		os << "indicesSize: " << indicesSize << "\n";
		for (size_t i = 0; i < indicesSize;)
		{
			os << m.m_indices->at(i++) << "," << m.m_indices->at(i++) << "," << m.m_indices->at(i++) << "\n";
		}
		return os;
	}

#ifndef PLUGINS
	void LiarGeometry::UploadSub()
	{
		int positionSize = Liar::LiarVertexBuffer::GetPositionSize();
		int normalSize = Liar::LiarVertexBuffer::GetNormalSize();
		int colorSize = Liar::LiarVertexBuffer::GetColorSize();
		int uvSize = Liar::LiarVertexBuffer::GetUVSize();

		int positionOffSize = Liar::LiarVertexBuffer::GetPositionOffSize();
		int normalOffSize = Liar::LiarVertexBuffer::GetNormalOffSize();
		int colorOffSize = Liar::LiarVertexBuffer::GetColorOffSize();
		int uvOffSize = Liar::LiarVertexBuffer::GetUVOffSize();

		bool normal = m_rawData->HasNorm();
		bool color = m_rawData->HasColor();
		bool uv = m_rawData->HasTexCoord();

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

		size_t bufferSize = m_vertexFaces->size();
		size_t totalSize = bufferSize * oneSize;

		glBufferData(GL_ARRAY_BUFFER, totalSize, nullptr, GL_STATIC_DRAW);
		for (size_t i = 0; i < bufferSize; ++i)
		{
			Liar::LiarVertexDefine* tmp = m_vertexFaces->at(i);
			size_t start = i * oneSize;
			glBufferSubData(GL_ARRAY_BUFFER, start + positionOffSize, positionSize, m_rawData->GetPos(tmp->positionIndex));
			if (normal) glBufferSubData(GL_ARRAY_BUFFER, start + normalOffSize, normalSize, m_rawData->GetNorm(tmp->normalIndex));
			if (uv) glBufferSubData(GL_ARRAY_BUFFER, start + uvOffSize, uvSize, m_rawData->GetTexCoord(tmp->texCoordIndex));
			if (color) glBufferSubData(GL_ARRAY_BUFFER, start + colorOffSize, colorSize, m_rawData->GetColor(tmp->colorIndex));
		}

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

		// skin anim info
		bool skin = m_rawData->HasSkin();
	}

	void LiarGeometry::ReleaseSourceData()
	{
		Liar::LiarBaseGeometry::ReleaseSourceData();
		delete m_rawData;
	}

#endif // !PLUGINS

	// =============================== Geometory ===============================
}
