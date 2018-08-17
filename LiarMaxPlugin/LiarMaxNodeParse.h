#pragma once

#include <LiarMesh.h>
#include <LiarPluginCfg.h>

#include <IGame.h>
#include <IGameObject.h>
#include <IGameProperty.h>
#include <IGameControl.h>
#include <IGameModifier.h>
#include <IConversionManager.h>
#include <IGameError.h>

#include <vector>
#include <map>

namespace Liar
{
	class LiarMaxNodeParse
	{
	public:
		LiarMaxNodeParse();
		~LiarMaxNodeParse();

	private:
		Liar::LiarNode* m_rootNode;
		std::vector<Liar::LiarMesh*>* m_allMeshs;
		std::vector<IGameNode*>* m_allBones;

	public:
		int ParseRootNode(Liar::LiarPluginCfg*, bool);

		Liar::LiarNode* GetRootNode() { return m_rootNode; };
		size_t GetMeshSize() { return m_allMeshs->size(); };
		Liar::LiarMesh* GetMesh(size_t index) { return m_allMeshs->at(index); };

	private:
		void ParseGameNode(Liar::LiarPluginCfg*, IGameNode*, Liar::LiarNode*, int&);
		void ParseValidMeshNode(Liar::LiarPluginCfg*, IGameNode*, IGameObject::ObjectTypes, Liar::LiarNode*, int&);
		void ParseChildNode(Liar::LiarPluginCfg*, IGameNode*, Liar::LiarNode*, int&);
		void ParseGameMesh(Liar::LiarPluginCfg*, IGameMesh*, Liar::LiarNode*, int&);

		void ParseBones(Liar::LiarPluginCfg*, IGameNode*);

	private:
		void ParseGeometory(Liar::LiarPluginCfg*, IGameMesh*, int&);
		void ParseAnim(Liar::LiarPluginCfg*, IGameMesh*);
	};

}
