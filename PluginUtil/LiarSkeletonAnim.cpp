#include "LiarSkeletonAnim.h"

namespace Liar
{
	LiarKeyFrame::LiarKeyFrame():
		frameIndex(0)
		, keyVec(new Liar::Vector3D())
	{
	}

	LiarKeyFrame::~LiarKeyFrame()
	{
		
	}

	// ===================== anim ==============
	LiarSkeletonAnim::LiarSkeletonAnim():
		m_allPositionKeys(nullptr)
		, m_allRotationKeys(nullptr)
		, m_allScaleKeys(nullptr)
	{
	}

	Liar::LiarKeyFrame* LiarSkeletonAnim::GetPositionKey(unsigned int frameIndex, bool add)
	{
		if (add && !m_allPositionKeys) m_allPositionKeys = new std::vector<Liar::LiarKeyFrame*>();
		Liar::LiarKeyFrame* findKey = GetKeyFrame(m_allPositionKeys, frameIndex);
		if (!findKey && add)
		{
			findKey = new Liar::LiarKeyFrame();
			m_allPositionKeys->push_back(findKey);
		}
		return findKey;
	}

	Liar::LiarKeyFrame* LiarSkeletonAnim::GetRotationKey(unsigned int frameIndex, bool add)
	{
		if (add && !m_allRotationKeys) m_allRotationKeys = new std::vector<Liar::LiarKeyFrame*>();
		Liar::LiarKeyFrame* findKey = GetKeyFrame(m_allRotationKeys, frameIndex);
		if (!findKey && add)
		{
			findKey = new Liar::LiarKeyFrame();
			m_allRotationKeys->push_back(findKey);
		}
		return findKey;
	}

	Liar::LiarKeyFrame* LiarSkeletonAnim::GetScaleKey(unsigned int frameIndex, bool add)
	{
		if (add && !m_allScaleKeys) m_allScaleKeys = new std::vector<Liar::LiarKeyFrame*>();
		Liar::LiarKeyFrame* findKey = GetKeyFrame(m_allScaleKeys, frameIndex);
		if (!findKey && add)
		{
			findKey = new Liar::LiarKeyFrame();
			m_allScaleKeys->push_back(findKey);
		}
		return findKey;
	}

	Liar::LiarKeyFrame* LiarSkeletonAnim::GetKeyFrame(std::vector<Liar::LiarKeyFrame*>* vec, unsigned int frameIndex)
	{
		if (vec)
		{
			for (std::vector<Liar::LiarKeyFrame*>::iterator it = vec->begin(); it != vec->end(); ++it)
			{
				if ((*it)->frameIndex == frameIndex)
				{
					return *it;
				}
			}
		}

		return nullptr;
	}

	void LiarSkeletonAnim::EraseAll(std::vector<Liar::LiarKeyFrame*>* vec)
	{
		if (vec)
		{
			for (std::vector<Liar::LiarKeyFrame*>::iterator it = vec->begin(); it < vec->end();)
			{
				delete *it;
				it = vec->erase(it);
			}
			std::vector<Liar::LiarKeyFrame*>().swap(*vec);
			delete vec;
		}
	}

	LiarSkeletonAnim::~LiarSkeletonAnim()
	{
		EraseAll(m_allPositionKeys);
		EraseAll(m_allRotationKeys);
		EraseAll(m_allScaleKeys);
	}
}
