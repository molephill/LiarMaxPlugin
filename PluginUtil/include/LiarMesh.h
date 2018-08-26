#pragma once
#include <string>
#include <vector>

#include <PluginDefine.h>
#include <LiarStringUtil.h>
#include <LiarMaterial.h>
#include "LiarVertexBuffer.h"

#ifndef PLUGINS
#include "Shader.hpp"
#include "Camera3D.h"
#endif // !PLUGINS


namespace Liar
{
	class LiarNode
	{
	public:
		LiarNode();
		~LiarNode();

	private:
		std::string m_nodeName;
		std::vector<Liar::LiarNode*>* m_children;

	public:
		unsigned int meshIndex;

	public:
		LiarNode* AddChild();

		void SetNodeName(const char* nodeName) { m_nodeName = nodeName; };
		void SetNodeName(const std::string& nodeName) { m_nodeName = nodeName; };

		std::string& GetNodeName() { return m_nodeName; };

		std::vector<Liar::LiarNode*>* GetChildren() const { return m_children; };

		int GetNumChildren() const { return m_children ? static_cast<int>(m_children->size()) : 0; };
	};

	class LiarGeometry
	{
	public:
		LiarGeometry();
		~LiarGeometry();

	private:
		std::vector<Liar::LiarVertexBuffer*>* m_allVertexBuffers;
		std::vector<unsigned int>* m_indices;
		int m_vertexOpen;

		Liar::LiarVertexRawData* m_rawData;
		std::vector<Liar::LiarVertexDefine*>* m_vertexFaces;

	public:
		std::vector<Liar::LiarVertexBuffer*>* GetBuffers() { return m_allVertexBuffers; };
		std::vector<unsigned int>* GetIndices() { return m_indices; };

		size_t GetBufferSize() { return m_allVertexBuffers ? m_allVertexBuffers->size() : 0; };
		size_t GetIndicesSize() { return m_indices->size(); };

		void SetVertexOpen(int v) { m_vertexOpen = v; };

		Liar::LiarVertexRawData* GetRawData() { return m_rawData; };

		Liar::LiarVertexDefine* FindVertexDefine(const Liar::LiarVertexDefine&);
		Liar::LiarVertexDefine* FindVertexDefine(unsigned int, unsigned int, unsigned int, unsigned int cor = 0);

		int FindVertexDefineIndex(const Liar::LiarVertexDefine&) const;
		int FindVertexDefineIndex(unsigned int, unsigned int, unsigned int, unsigned int cor = 0) const;

		std::vector<Liar::LiarVertexDefine*>* GetVertexFaces() { return m_vertexFaces; };
		size_t GetVertexFaceSize() const { return m_vertexFaces ? m_vertexFaces->size() : 0; };
		LiarVertexDefine* GetFace(size_t index) { return m_vertexFaces->at(index); };

		LiarVertexBuffer* GetBuffer(size_t);

		void EraseIndexBuff(int);
		friend std::ostream& operator<<(std::ostream& os, const Liar::LiarGeometry& m);
        
#ifndef PLUGINS
    public:
        void Upload();
        void Render();
        
    private:
        unsigned int VAO;
        unsigned int VBO;
        unsigned int EBO;
#endif
	};

	class LiarMesh
	{
	public:
		LiarMesh();
		~LiarMesh();

	private:
		Liar::LiarGeometry* m_geometry;
		std::vector<Liar::LiarMaterial*>* m_materials;

	public:
		Liar::LiarGeometry* GetGeo() { return m_geometry; };

		std::vector<Liar::LiarMaterial*>* GetMatrials() { return m_materials; };
		Liar::LiarMaterial* GetMat(size_t index) { return m_materials->at(index); };

		void EraseMaterial(int);

		std::string meshName;

	public:

		friend std::ostream& operator<<(std::ostream& os, const Liar::LiarMesh& m);

#ifdef PLUGINS
		std::string saveName;
#else
	public:
		void Upload();
		void Render(Liar::Shader&);

	private:
		unsigned int m_refCount;

	public:
		void AddRef() { ++m_refCount; };
		unsigned int SubRef() { return --m_refCount; };
#endif // !PLUGINS

	};
}

