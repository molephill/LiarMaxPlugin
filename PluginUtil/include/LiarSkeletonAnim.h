#pragma once

#include <Vectors.h>
#include <vector>

namespace Liar
{
	// ================= bone frame ==================== //
	class LiarBoneKeyFrame
	{
	public:
		LiarBoneKeyFrame();
		~LiarBoneKeyFrame();

	private:
		int m_boneId;
		Liar::Vector3D* m_positonKey;
		Liar::Vector3D* m_rotationKey;
		Liar::Vector3D* m_scaleKey;

	public:
		bool operator==(const LiarBoneKeyFrame& rhs) const { return rhs.m_boneId == m_boneId; };
		int GetBoneId() const { return m_boneId; };
		void SetBoneId(int id) { m_boneId = id; };

		Liar::Vector3D* GetPositionKey() { return m_positonKey; };
		Liar::Vector3D* GetRotationKey() { return m_rotationKey; };
		Liar::Vector3D* GetScaleKey() { return m_scaleKey; };
	};

	// ================== key frame ======================= //
	class LiarKeyFrame
	{
	public:
		LiarKeyFrame();
		~LiarKeyFrame();

	private:
		unsigned int m_frameIndex;
		std::vector<Liar::LiarBoneKeyFrame*>* m_boneKeys;

	public:
		void SetFrameIndex(unsigned int index) { m_frameIndex = index; };
		unsigned int GetFrameIndex() const { return m_frameIndex; };

		size_t GetBoneKeyLen() { return m_boneKeys ? m_boneKeys->size() : 0; };
		Liar::LiarBoneKeyFrame* GetBoneKeyFrameById(int, bool add = false);
		Liar::LiarBoneKeyFrame* GetBoneKeyFrame(size_t index) { return m_boneKeys->at(index); };

		bool operator==(const LiarKeyFrame& rhs) const { return rhs.m_frameIndex == m_frameIndex; };
	};


	// ================== anim ======================= //
	class LiarSkeletonAnim
	{
	public:
		LiarSkeletonAnim();
		~LiarSkeletonAnim();

	private:
		std::vector<Liar::LiarKeyFrame*>* m_allKeys;
		int m_tickFrame;

	public:
		Liar::LiarKeyFrame* GetKey(unsigned int, bool add = false);
		size_t GetKeyLen() const { return m_allKeys->size(); };

		void SetTickFrame(int tick) { m_tickFrame = tick; };
		int GetTickFrame() const { return m_tickFrame; };
	};
}

