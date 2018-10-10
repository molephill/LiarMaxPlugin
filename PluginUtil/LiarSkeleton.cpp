#include "LiarSkeleton.h"

namespace Liar
{
	// =============================== bone =========================
	LiarBone::LiarBone() :
		m_id(-1), m_parentId(-1),m_name("")
		, m_position(new Liar::Vector3D()), m_rotation(new Liar::Vector4D()), m_scale(new Liar::Vector3D())
	{

	}

	LiarBone::~LiarBone()
	{
		delete m_position;
		delete m_rotation;
		delete m_scale;
	}


#if PLUGINS
	LiarSkeleton::LiarSkeleton() :
		Liar::LiarReference(),
		m_bones(new std::vector<Liar::LiarBone*>())
#else
	LiarSkeleton::LiarSkeleton() :
		Liar::LiarReference(),
		m_bones(new std::vector<Liar::LiarBone*>()),
		m_path("")
#endif // PLUGINS
	{
	}


	LiarSkeleton::~LiarSkeleton()
	{
		for (std::vector<Liar::LiarBone*>::iterator it = m_bones->begin(); it != m_bones->end();)
		{
			delete *it;
			it = m_bones->erase(it);
		}
		std::vector<Liar::LiarBone*>().swap(*m_bones);
		delete m_bones;
	}

	Liar::LiarBone* LiarSkeleton::GetBone(std::string& name, bool add)
	{
		Liar::LiarBone* find = nullptr;

		for (std::vector<Liar::LiarBone*>::const_iterator cit = m_bones->begin(); cit != m_bones->end(); ++cit)
		{
			if ((*cit)->Equal(name))
			{
				find = *cit;
				break;
			}
		}

		if (!find && add)
		{
			find = new Liar::LiarBone();
			m_bones->push_back(find);
		}

		return find;
	}

}