#pragma once

#include "LiarMaxNodeParse.h"
#include "LiarPluginCfg.h"
#include <LiarStringUtil.h>

#include <LiarVertexBuffer.h>

namespace Liar
{
	class LiarPluginWrite
	{
	public:
		static void WriteLiarNode(Liar::LiarMaxNodeParse*, Liar::LiarPluginCfg*, const std::string&);
		
	private:
		static void WriteModelHierarchy(Liar::LiarMaxNodeParse*, Liar::LiarPluginCfg*, const std::string& path);
		static void WriteNode(Liar::LiarMaxNodeParse*, Liar::LiarNode*, FILE*);
		static void SetMeshSaveName(Liar::LiarMaxNodeParse*, int, const std::string&, const std::string&);
		
		static void WrtieLiarMeshRawData(Liar::LiarMeshRawData*, const std::string&, Liar::LiarPluginCfg*);

		static void WriteLiarSkelenton(Liar::LiarMaxNodeParse*, Liar::LiarPluginCfg*, const std::string&);
		static void WrtieLiarAnim(Liar::LiarMaxNodeParse*, Liar::LiarPluginCfg*, const std::string&);

		static void WriteLiarVecs(std::vector<Liar::Vector3D*>*, FILE*);

		static void WriteLiarKeyFrames(Liar::LiarTrack*, Liar::LiarVertexAttr, FILE*);

		static void WriteString(std::string&, FILE*);
	};
}

