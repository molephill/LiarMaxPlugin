#pragma once

#include <Vectors.h>
#include <vector>

#include <PluginDefine.h>

#ifdef PLUGINS
#include <triobj.h>
#endif // PLUGINS


namespace Liar
{
	// ================= bone frame ==================== //
	class LiarKeyFrame
	{
	public:
		LiarKeyFrame(int time = 0);
		virtual ~LiarKeyFrame();

	protected:
		int m_time;
		float m_x;
		float m_y;
		float m_z;

	public:
		int GetTime() const { return m_time; };
		
		float GetX() const { return m_x; };
		float GetY() const { return m_y; };
		float GetZ() const { return m_z; };

		void Set(const Liar::Vector3D& rhs);
		virtual void Set(float x, float y, float z, float w = 0);
		virtual void Set(const Liar::Vector4D& rhs);

#ifdef PLUGINS
		void Set(const Quat& quat) { Set(quat.x, quat.y, quat.z, quat.w); };
		void Set(const Point3& quat) { Set(quat.x, quat.y, quat.z); };
#endif // PLUGINS
	};

	class LiarQuatKeyFrame :public LiarKeyFrame
	{
	public:
		LiarQuatKeyFrame(int time = 0);
		~LiarQuatKeyFrame();

	protected:
		float m_w;

	public:
		virtual void Set(const Liar::Vector4D& rhs) { Set(rhs.x, rhs.y, rhs.z, rhs.w); };
		virtual void Set(float x, float y, float z, float w = 0) { m_x = x; m_y = y; m_z = z; m_w = w; };

		float GetW() const { return m_w; };
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
		std::vector<Liar::LiarQuatKeyFrame*>* m_rotationFrames;
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
		int m_tickPerFrame;

	public:
		Liar::LiarTrack* GetTrack(int, bool add = false);
		Liar::LiarTrack* GetTrackByIndex(size_t index) { return m_tracks->at(index); };
		size_t GetTrackLen() const { return m_tracks->size(); };

		void SetTickPerFrame(int v) { m_tickPerFrame = v; };
		int GetTickPerFrame() const { return m_tickPerFrame; };
	};
}

