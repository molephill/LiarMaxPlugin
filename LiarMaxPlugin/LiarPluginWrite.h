#pragma once

#include "LiarMaxNodeParse.h"
#include <LiarPluginCfg.h>

namespace Liar
{
	class LiarPluginWrite
	{
	public:
		static void WriteMesh(Liar::LiarMaxNodeParse*, Liar::LiarPluginCfg*, const std::string&);
		
	private:
		static void WriteModelHierarchy(Liar::LiarMaxNodeParse*, Liar::LiarPluginCfg*, const std::string& path);
		static void WriteNode(Liar::LiarMaxNodeParse*, Liar::LiarNode*, FILE*);
		static void SetMeshSaveName(Liar::LiarMaxNodeParse*, int, const std::string&, const std::string&);

		static void WriteLiarMesh(Liar::LiarMesh*, const std::string&, Liar::LiarPluginCfg* liarPlugin, int);
		static void WriteLiarGeometery(Liar::LiarPluginCfg*, Liar::LiarGeometry*, FILE*);
		static void WriteLiarMaterial(Liar::LiarMesh*, FILE*);

		static void WriteLiarRawData(Liar::LiarVertexRawData*, FILE*);
		static void WriteLiarVecs(std::vector<Liar::Vector3D*>*, FILE*);

		static void WriteLiarFaces(Liar::LiarGeometry*, FILE*);

		static void WriteLiarTexture(Liar::LiarTexture*, FILE*);

		static void WriteString(std::string&, FILE*);

		static int GetVertexVer(Liar::LiarPluginCfg*);
	};
}

