#pragma once

#include <Vectors.h>
#include <PluginDefine.h>

#ifdef PLUGINS
#include <IGame.h>
#include <IGameObject.h>
#endif // PLUGINS


namespace Liar
{
	class LiarSkin
	{
	public:
		LiarSkin();
		~LiarSkin();

	public:
		int bonId;
		float weight;

#ifdef PLUGINS
		INode* node;
		bool operator==(const Liar::LiarSkin& rhs) const
		{
			return rhs.node == node;
		}

		bool Equal(const Liar::LiarSkin& rhs)
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

	public:
		int id;
		int parentId;
		std::string name;

	public:

#ifdef PLUGINS
		IGameNode* node;
#endif // PLUGINS


		bool operator==(const Liar::LiarBone& rhs) const
		{
			return rhs.name == name;
		}

		bool Equal(const char* cname)
		{
			return strcmp(name.c_str(), cname) == 0;
		}

		bool Equal(const std::string& cname)
		{
			return cname == name;
		}
	};

	class LiarSkeleton
	{
	public:
		LiarSkeleton();
		~LiarSkeleton();
	};
}

