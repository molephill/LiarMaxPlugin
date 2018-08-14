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
		int m_bufferSize;

		std::vector<unsigned int>* m_indices;

		int m_vertexOpen;

	public:
		std::vector<Liar::LiarVertexBuffer*>* GetBuffers() { return m_allVertexBuffers; };
		std::vector<unsigned int>* GetIndices() { return m_indices; };


		unsigned int GetBufferSize() { return m_bufferSize; };
		int GetIndicesSize() { return static_cast<int>(m_indices->size()); };

		void SetBufferSize(unsigned int v) { m_bufferSize = v; };
		LiarVertexBuffer* GetBuffer(int index) { return m_allVertexBuffers->at(index); };

		void EraseIndexBuff(int);

		void SetVertexOpen(int v) { m_vertexOpen = v; };

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
		Liar::LiarMaterial* m_material;

	public:
		Liar::LiarGeometry* GetGeo() { return m_geometry; };
		Liar::LiarMaterial* GetMat() { return m_material; };

		std::string meshName;

	public:

		friend std::ostream& operator<<(std::ostream& os, const Liar::LiarMesh& m);

#ifdef PLUGINS
		int vertexNum;
		int faceNum;
		std::string saveName;
		std::string& GetMaterialName() { return m_material->name; };
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

