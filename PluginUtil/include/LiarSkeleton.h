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

#ifdef PLUGINS
		IGameNode* node;
		bool Equal(IGameNode* rhs)
		{
			return rhs == node;
		}
#endif // PLUGINS

	};

	class LiarSkeleton
	{
	public:
		LiarSkeleton();
		~LiarSkeleton();
	};
}

