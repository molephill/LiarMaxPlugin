#include "LiarSkeletonAnim.h"

namespace Liar
{
	LiarKeyFrame::LiarKeyFrame():
		m_frameIndex(0)
		, m_boneKeys(new std::vector<LiarBoneKeyFrame*>())
	{
	}

	LiarKeyFrame::~LiarKeyFrame()
	{
		
	}

	Liar::LiarBoneKeyFrame* LiarKeyFrame::GetBoneKeyFrameById(int boneId, bool add)
	{
		Liar::LiarBoneKeyFrame* find = nullptr;
		for (std::vector<Liar::LiarBoneKeyFrame*>::iterator it = m_boneKeys->begin(); it < m_boneKeys->end(); ++it)
		{
			if ((*it)->GetBoneId() == boneId)
			{
				find = *it;
				break;
			}
		}

		if (!find && add)
		{
			find = new Liar::LiarBoneKeyFrame();
			find->SetBoneId(boneId);
			m_boneKeys->push_back(find);
		}

		return find;
	}

	// ===================== bone keyFrame ====================
	LiarBoneKeyFrame::LiarBoneKeyFrame():
		m_positonKey(new Liar::Vector3D()), m_rotationKey(new Liar::Vector3D()), m_scaleKey(new Liar::Vector3D())
	{

	}

	LiarBoneKeyFrame::~LiarBoneKeyFrame()
	{
		delete m_positonKey;
		delete m_rotationKey;
		delete m_scaleKey;
	}

	// ===================== anim ==============
	LiarSkeletonAnim::LiarSkeletonAnim():
		m_allKeys(new std::vector<LiarKeyFrame*>())
		, m_tickFrame(0)
	{
	}

	LiarSkeletonAnim::~LiarSkeletonAnim()
	{
		for (std::vector<Liar::LiarKeyFrame*>::iterator it = m_allKeys->begin(); it < m_allKeys->end();)
		{
			delete *it;
			it = m_allKeys->erase(it);
		}
		std::vector<Liar::LiarKeyFrame*>().swap(*m_allKeys);
		delete m_allKeys;
	}

	Liar::LiarKeyFrame* LiarSkeletonAnim::GetKey(unsigned int frameIndex, bool add)
	{
		Liar::LiarKeyFrame* find = nullptr;
		for (std::vector<Liar::LiarKeyFrame*>::iterator it = m_allKeys->begin(); it < m_allKeys->end(); ++it)
		{
			if ((*it)->GetFrameIndex() == frameIndex)
			{
				find = *it;
				break;
			}
		}

		if (!find && add)
		{
			find = new Liar::LiarKeyFrame();
			find->SetFrameIndex(frameIndex);
			m_allKeys->push_back(find);
		}

		return find;
	}
}
