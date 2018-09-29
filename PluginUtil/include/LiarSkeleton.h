#pragma once

#include <Vectors.h>
#include <vector>

#ifdef PLUGINS
#include <IGame.h>
#include <IGameObject.h>
#endif // PLUGINS


namespace Liar
{
	class LiarSkin
	{
	public:
		LiarSkin(int boneId = -1);
		~LiarSkin();

	private:
		int m_boneId;
		float m_weight;

	public:
		int GetBoneId() const { return m_boneId; };
		void SetBoneId(int v) { m_boneId = v; };

		float GetWeight() const { return m_weight; };
		void SetWeight(float v) { m_weight = v; };

#ifdef PLUGINS
		INode* node;
		bool operator==(const Liar::LiarSkin& rhs) const
		{
			return rhs.node == node;
		}
#endif // PLUGINS

	};

	class LiarBone
	{
	public:
		LiarBone();
		~LiarBone();

	private:
		Liar::Vector3D* m_position;
		Liar::Vector4D* m_rotation;
		Liar::Vector3D* m_scale;

		int m_id;
		int m_parentId;
		std::string m_name;

	public:
		
		int GetId() const { return m_id; };
		void SetId(int v) { m_id = v; };

		int GetParentId() const { return m_parentId; };
		void SetParentId(int v) { m_parentId = v; };

		std::string& GetName() { return m_name; };
		void SetName(const std::string& name) { m_name = name; };

		Liar::Vector3D* GetPosition() { return m_position; };
		Liar::Vector4D* GetRotation() { return m_rotation; };
		Liar::Vector3D* GetScale() { return m_scale; };

		void SetPositon(Liar::Vector3D* v) { m_position = v; };
		void SetRotation(Liar::Vector4D* v) { m_rotation = v; };
		void SetScale(Liar::Vector3D* v) { m_scale = v; };

	public:

#ifdef PLUGINS
		IGameNode* node;
#endif // PLUGINS
		bool operator==(const Liar::LiarBone& rhs) const
		{
			return rhs.m_name == m_name;
		}

		bool Equal(const char* cname)
		{
			return strcmp(m_name.c_str(), cname) == 0;
		}

		bool Equal(const std::string& cname)
		{
			return cname == m_name;
		}
	};

	class LiarSkeleton
	{
	public:
		LiarSkeleton();
		~LiarSkeleton();

	private:
		std::vector<Liar::LiarBone*>* m_bones;

	public:
		Liar::LiarBone* GetBone(std::string&, bool add = false);
		Liar::LiarBone* GetBone(size_t index) { return m_bones->at(index); };

		size_t GetBoneSize() const { return m_bones->size(); };
	};
}

