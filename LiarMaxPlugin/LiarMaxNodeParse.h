#pragma once

#include <LiarMesh.h>
#include <LiarPluginCfg.h>
#include <LiarSkeletonAnim.h>

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
		std::vector<Liar::LiarBone*>* m_allBones;
		std::vector<Liar::LiarMaterialNode*>* m_allMats;
		Liar::LiarSkeletonAnim* m_anim;

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
		void ParseGeometory(Liar::LiarPluginCfg*, IGameMesh*, int&);

		void ParseMatNodes(IGameScene*);

		void ParseBones(Liar::LiarPluginCfg*, IGameNode*);
		void ParseSkin(Liar::LiarPluginCfg*, IGameMesh*, Liar::LiarMesh*);
		void ParseAnim(Liar::LiarPluginCfg*);

		Liar::LiarBone* GetLiarBone(std::string&);
	};

}
