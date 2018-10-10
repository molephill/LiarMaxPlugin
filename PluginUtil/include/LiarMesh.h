#pragma once
#include <PluginDefine.h>
#include <LiarStringUtil.h>
#include <LiarMaterial.h>
#include <LiarSkeleton.h>
#include <LiarSkeletonAnim.h>
#include <LiarGeometries.h>
#include <LiarRefrence.h>

#ifndef PLUGINS
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

	class LiarMesh:public LiarReference
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
		void Render(Liar::LiarShaderProgram&);
#endif // !PLUGINS

	};
}

