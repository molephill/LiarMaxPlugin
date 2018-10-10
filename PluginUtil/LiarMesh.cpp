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

	LiarMesh::LiarMesh() :
		Liar::LiarReference(),
		m_geometry(new Liar::LiarGeometry())
		, m_materials(new std::vector<Liar::LiarMaterial*>())
	{
	}

	LiarMesh::~LiarMesh()
	{
		delete m_geometry;

		EraseMaterial(0);
		std::vector<LiarMaterial*>().swap(*m_materials);
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


	void LiarMesh::Render(Liar::LiarShaderProgram& shader)
	{
		for (std::vector<Liar::LiarMaterial*>::iterator it = m_materials->begin(); it != m_materials->end(); ++it)
		{
			(*it)->Render(shader);
		}
		m_geometry->Render();
	}

#endif // PLUGINS


}
