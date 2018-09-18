#pragma once

#include <Vectors.h>
#include <vector>

#include <PluginDefine.h>

namespace Liar
{
	// ================= bone frame ==================== //
	class LiarKeyFrame
	{
	public:
		LiarKeyFrame(int time = 0);
		~LiarKeyFrame();

	private:
		int m_time;
		Liar::Vector3D* m_key;

	public:
		int GetTime() const { return m_time; };
		Liar::Vector3D* GetKey() { return m_key; };
	};

	// ================== track ======================= //
	class LiarTrack
	{
	public:
		LiarTrack(int boneId = 0);
		~LiarTrack();

	private:
		int m_boneId;
		std::vector<Liar::LiarKeyFrame*>* m_positionFrames;
		std::vector<Liar::LiarKeyFrame*>* m_rotationFrames;
		std::vector<Liar::LiarKeyFrame*>* m_scaleFrames;

	public:
		Liar::LiarKeyFrame* GetKeyFrame(Liar::LiarVertexAttr, int, bool add);
		Liar::LiarKeyFrame* GetKeyFrame(Liar::LiarVertexAttr, size_t);
		int GetBoneId() const { return m_boneId; };
		size_t GetNumFrames(Liar::LiarVertexAttr) const;

	private:
		Liar::LiarKeyFrame* GetKeyFrame(std::vector<Liar::LiarKeyFrame*>*, int, bool add);
		std::vector<Liar::LiarKeyFrame*>* GetKeyFrames(Liar::LiarVertexAttr) const;
		void EraseFrames(std::vector<Liar::LiarKeyFrame*>*);
	};


	// ================== anim ======================= //
	class LiarSkeletonAnim
	{
	public:
		LiarSkeletonAnim();
		~LiarSkeletonAnim();

	private:
		std::vector<Liar::LiarTrack*>* m_tracks;

	public:
		Liar::LiarTrack* GetTrack(int, bool add = false);
		Liar::LiarTrack* GetTrackByIndex(size_t index) { return m_tracks->at(index); };
		size_t GetTrackLen() const { return m_tracks->size(); };
	};
}

