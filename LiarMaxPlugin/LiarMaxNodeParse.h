#pragma once

#include <PluginDefine.h>

#include "LiarPluginCfg.h"

#include <LiarStringUtil.h>
#include <LiarVertexBuffer.h>
#include <LiarSkeletonAnim.h>

#include <IGame.h>
#include <IGameObject.h>
#include <IGameProperty.h>
#include <IGameControl.h>
#include <IGameModifier.h>
#include <IConversionManager.h>
#include <IGameError.h>

#include <vector>

namespace Liar
{
	struct LiarMaxBone
	{
		LiarMaxBone() :boneId(0), node(nullptr) {};
		~LiarMaxBone() { node = nullptr; };

		int boneId;
		IGameNode* node;
	};

	class LiarMaxNodeParse
	{
	public:
		LiarMaxNodeParse();
		~LiarMaxNodeParse();

	private:
		Liar::LiarNode* m_rootNode;
		std::vector<Liar::LiarMeshRawData*>* m_allMeshData;
		Liar::LiarSkeletonAnim* m_anim;
		Liar::LiarSkeleton* m_skeleton;
		std::vector<LiarMaxBone*>* m_allBones;

	public:
		int ParseRootNode(Liar::LiarPluginCfg*, bool);

		Liar::LiarNode* GetRootNode() { return m_rootNode; };

		size_t GetMeshRawDataSize() const { return m_allMeshData->size(); };
		Liar::LiarMeshRawData* GetMeshRawData(size_t index) { return m_allMeshData->at(index); };

		Liar::LiarSkeletonAnim* GetAnim() { return m_anim; };
		Liar::LiarSkeleton* GetSkeleton() { return m_skeleton; };

	private:
		void ParseGameNode(Liar::LiarPluginCfg*, IGameNode*, Liar::LiarNode*, int&);
		void ParseValidMeshNode(Liar::LiarPluginCfg*, IGameNode*, IGameObject::ObjectTypes, Liar::LiarNode*, int&);
		void ParseChildNode(Liar::LiarPluginCfg*, IGameNode*, Liar::LiarNode*, int&);
		void ParseGameMesh(Liar::LiarPluginCfg*, IGameMesh*, Liar::LiarNode*, int&);
		void ParseGeometory(Liar::LiarPluginCfg*, IGameMesh*, int&);

		void ParseBones(Liar::LiarPluginCfg*, IGameNode*);
		void ParseSkin(Liar::LiarPluginCfg*, IGameMesh*, Liar::LiarMeshRawData*);
		void ParseAnim(Liar::LiarPluginCfg*);

		IGameNode* GetMaxNode(int);
	};

}
