#include "LiarSkeletonAnim.h"

namespace Liar
{
	LiarKeyFrame::LiarKeyFrame(int time):
		m_time(time)
		,m_x(0.0f), m_y(0.0f), m_z(0.0f)
	{
	}

	LiarKeyFrame::~LiarKeyFrame()
	{
	}

	void LiarKeyFrame::Set(float x, float y, float z, float)
	{
		m_x = x; m_y = y; m_z = z;
	}

	void LiarKeyFrame::Set(const Liar::Vector3D& rhs)
	{
		Set(rhs.x, rhs.y, rhs.z);
	}

	void LiarKeyFrame::Set(const Liar::Vector4D& rhs)
	{
		Set(rhs.x, rhs.y, rhs.z, rhs.w);
	}


	LiarQuatKeyFrame::LiarQuatKeyFrame(int time):
		Liar::LiarKeyFrame(time),
		m_w(0.0f)
	{
	}

	LiarQuatKeyFrame::~LiarQuatKeyFrame()
	{
	}

	// ===================== bone keyFrame ====================
	LiarTrack::LiarTrack(int boneId) :
		m_boneId(boneId)
		,m_positionFrames(new std::vector<Liar::LiarKeyFrame*>())
		, m_rotationFrames(new std::vector<Liar::LiarQuatKeyFrame*>())
		, m_scaleFrames(new std::vector<Liar::LiarKeyFrame*>())
	{

	}

	LiarTrack::~LiarTrack()
	{
		EraseFrames(m_positionFrames);
		EraseFrames(m_scaleFrames);

		for (std::vector<Liar::LiarQuatKeyFrame*>::iterator it = m_rotationFrames->begin(); it < m_rotationFrames->end();)
		{
			delete *it;
			it = m_rotationFrames->erase(it);
		}
		std::vector<Liar::LiarQuatKeyFrame*>().swap(*m_rotationFrames);
		delete m_rotationFrames;
	}

	Liar::LiarKeyFrame* LiarTrack::GetKeyFrame(Liar::LiarVertexAttr type, size_t index)
	{
		if (type == LiarVertexAttr::LiarVertexAttr_ROTATION)
		{
			return m_rotationFrames->at(index);
		}
		else
		{
			std::vector<Liar::LiarKeyFrame*>* vec = GetKeyFrames(type);
			return vec ? vec->at(index) : nullptr;
		}
	}

	Liar::LiarKeyFrame* LiarTrack::GetKeyFrame(Liar::LiarVertexAttr type, int time, bool add)
	{
		if (type == LiarVertexAttr::LiarVertexAttr_ROTATION)
		{
			Liar::LiarQuatKeyFrame* find = nullptr;
			for (std::vector<Liar::LiarQuatKeyFrame*>::iterator it = m_rotationFrames->begin(); it < m_rotationFrames->end(); ++it)
			{
				if ((*it)->GetTime() == time)
				{
					find = *it;
					break;
				}
			}

			if (!find && add)
			{
				find = new Liar::LiarQuatKeyFrame(time);
				m_rotationFrames->push_back(find);
			}
			return find;
		}
		else
		{
			std::vector<Liar::LiarKeyFrame*>* vec = GetKeyFrames(type);
			return GetKeyFrame(vec, time, add);
		}
	}

	Liar::LiarKeyFrame* LiarTrack::GetKeyFrame(std::vector<Liar::LiarKeyFrame*>* vec, int time, bool add)
	{
		Liar::LiarKeyFrame* find = nullptr;
		if (vec)
		{
			for (std::vector<Liar::LiarKeyFrame*>::iterator it = vec->begin(); it < vec->end(); ++it)
			{
				if ((*it)->GetTime() == time)
				{
					find = *it;
					break;
				}
			}

			if (!find && add)
			{
				find = new Liar::LiarKeyFrame(time);
				vec->push_back(find);
			}
		}
		return find;
	}

	size_t LiarTrack::GetNumFrames(Liar::LiarVertexAttr type) const
	{
		std::vector<Liar::LiarKeyFrame*>* vec = GetKeyFrames(type);
		return vec ? vec->size() : 0;
	}

	std::vector<Liar::LiarKeyFrame*>* LiarTrack::GetKeyFrames(Liar::LiarVertexAttr type) const
	{
		std::vector<Liar::LiarKeyFrame*>* vec = nullptr;
		if (type == LiarVertexAttr::LiarVertexAttr_TRANSFORM)
		{
			vec = m_positionFrames;
		}
		else if (type == LiarVertexAttr::LiarVertexAttr_SCALE)
		{
			vec = m_scaleFrames;
		}
		return vec;
	}

	void LiarTrack::EraseFrames(std::vector<Liar::LiarKeyFrame*>* vec)
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


	// ===================== anim ==============
	LiarSkeletonAnim::LiarSkeletonAnim():
		m_tracks(new std::vector<LiarTrack*>()),
		m_tickPerFrame(0)
	{
	}

	LiarSkeletonAnim::~LiarSkeletonAnim()
	{
		for (std::vector<Liar::LiarTrack*>::iterator it = m_tracks->begin(); it < m_tracks->end();)
		{
			delete *it;
			it = m_tracks->erase(it);
		}
		std::vector<Liar::LiarTrack*>().swap(*m_tracks);
		delete m_tracks;
	}

	Liar::LiarTrack* LiarSkeletonAnim::GetTrack(int boneId, bool add)
	{
		Liar::LiarTrack* track = nullptr;
		for (std::vector<Liar::LiarTrack*>::iterator it = m_tracks->begin(); it < m_tracks->end(); ++it)
		{
			if ((*it)->GetBoneId() == boneId)
			{
				track = *it;
				break;
			}
		}

		if (!track && add)
		{
			track = new Liar::LiarTrack(boneId);
			m_tracks->push_back(track);
		}

		return track;
	}
}
