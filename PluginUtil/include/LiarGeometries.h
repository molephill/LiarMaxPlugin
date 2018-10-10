#pragma once

#include <string>
#include <vector>

#include <PluginDefine.h>
#include <LiarVertexBuffer.h>
#include <LiarRefrence.h>

namespace Liar
{
	class LiarBaseGeometry :public Liar::LiarReference
	{
	public:
		LiarBaseGeometry();
		virtual ~LiarBaseGeometry();

	protected:

#ifndef PLUGINS
		unsigned int m_vertexArrayID;
		unsigned int m_vertexbuffer;
		unsigned int m_elementbuffer;
#endif // PLUGINS

		std::vector<unsigned int>* m_indices;
		size_t m_indiceSize;

	public:
		std::vector<unsigned int>* GetIndices() { return m_indices; };
		int GetIndicesSize() { return static_cast<int>(m_indices->size()); };

#ifndef PLUGINS
	public:
		virtual void Upload();
		virtual void Render();

	protected:
		virtual void ReleaseSourceData();
		virtual void UploadSub() = 0;
#endif // PLUGINS
	};

	class LiarGeometry :public LiarBaseGeometry
	{
	public:
		LiarGeometry();
		~LiarGeometry();

	private:
		int m_vertexOpen;

		Liar::LiarVertexRawData* m_rawData;
		std::vector<Liar::LiarVertexDefine*>* m_vertexFaces;

	public:

		void SetVertexOpen(int v) { m_vertexOpen = v; };

		Liar::LiarVertexRawData* GetRawData() { return m_rawData; };

		Liar::LiarVertexDefine* FindVertexDefine(const Liar::LiarVertexDefine&);
		Liar::LiarVertexDefine* FindVertexDefine(unsigned int, unsigned int, unsigned int, unsigned int cor = 0);

		int FindVertexDefineIndex(const Liar::LiarVertexDefine&) const;
		int FindVertexDefineIndex(unsigned int, unsigned int, unsigned int, unsigned int cor = 0) const;

		std::vector<Liar::LiarVertexDefine*>* GetVertexFaces() { return m_vertexFaces; };
		size_t GetVertexFaceSize() const { return m_vertexFaces ? m_vertexFaces->size() : 0; };
		LiarVertexDefine* GetFace(size_t index) { return m_vertexFaces->at(index); };

		friend std::ostream& operator<<(std::ostream& os, const Liar::LiarGeometry& m);

#ifndef PLUGINS
	protected:
		virtual void ReleaseSourceData();
		virtual void UploadSub();
#endif
	};
}

