#pragma once

#include <Vectors.h>
#include <vector>

namespace Liar
{
	// ================== key frame ======================= //
	class LiarKeyFrame
	{
	public:
		LiarKeyFrame();
		~LiarKeyFrame();

	public:
		unsigned int frameIndex;
		Liar::Vector3D* keyVec;
	};

	// ================== anim ======================= //
	class LiarSkeletonAnim
	{
	public:
		LiarSkeletonAnim();
		~LiarSkeletonAnim();

	private:
		std::vector<Liar::LiarKeyFrame*>* m_allPositionKeys;
		std::vector<Liar::LiarKeyFrame*>* m_allRotationKeys;
		std::vector<Liar::LiarKeyFrame*>* m_allScaleKeys;

	public:
		LiarKeyFrame* GetPositionKey(unsigned int, bool add = false);
		LiarKeyFrame* GetRotationKey(unsigned int, bool add = false);
		LiarKeyFrame* GetScaleKey(unsigned int, bool add = false);

		std::vector<Liar::LiarKeyFrame*>* GetPositionKeys() { return m_allPositionKeys; };
		std::vector<Liar::LiarKeyFrame*>* GetRotationKeys() { return m_allRotationKeys; };
		std::vector<Liar::LiarKeyFrame*>* GetScaleKeys() { return m_allScaleKeys; };

	private:
		void EraseAll(std::vector<Liar::LiarKeyFrame*>*);
		LiarKeyFrame* GetKeyFrame(std::vector<Liar::LiarKeyFrame*>*, unsigned int);
	};
}

