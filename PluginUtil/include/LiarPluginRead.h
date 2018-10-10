#pragma once

#include "PluginDefine.h"
#include <AssetsMgr.hpp>

#ifndef PLUGINS
#include "LiarModel.h"
#else
#include <LiarMesh.h>
#endif // !PLUGINS

#include <string>
#include <stdio.h>

namespace Liar
{
	class LiarPluginRead
	{
	public:
#ifndef PLUGINS
		static Liar::Model* ReadModel(const char*, const char* texBasePath = nullptr);
        static Liar::Model* ReadModel(const std::string&, const char* texBasePath = nullptr);
#endif // !PLUGINS
		static void ReadNode(const char*, Liar::LiarNode&);
		static Liar::LiarMesh* ReadMesh(const char* path, const char* basePath=nullptr);
        static Liar::LiarMesh* ReadMesh(const std::string& path, const char* basePath = nullptr);

		static Liar::LiarSkeleton* ReadSkeleton(const char*);
		static Liar::LiarSkeleton* ReadSkeleton(const std::string& path);

		static Liar::LiarSkeletonAnim* ReadAnim(const char*);
		static Liar::LiarSkeletonAnim* ReadAnim(const std::string& path);

	private:

#ifndef PLUGINS
		// ================================ read child model ===============================
		static void ReadChildModel(Liar::Model&, const Liar::LiarNode&, const char* basePath = nullptr);
		// ================================ read child model ===============================
#endif // !PLUGINS

		// ================================ read child node ===============================
		static void ReadChildNode(Liar::LiarNode&, std::string&, FILE*);
		// ================================ read child node ===============================

		static void ReadLiarMesh(Liar::LiarMesh*, FILE*);
		static void ReadLiarGeometery(int, Liar::LiarGeometry*, FILE*);

		static void ReadLiarRawData(Liar::LiarGeometry*, FILE*);
		static std::vector<Liar::Vector3D*>* ReadLiarVecs(FILE*);

		static void ReadLiarFaces(Liar::LiarGeometry*, FILE*);

		static void ReadLiarMaterial(Liar::LiarMesh*, FILE*);

		static Liar::LiarTexture* ReadLiarTexture(FILE*);

		static void ReadLiarKeyFrames(Liar::LiarTrack*, Liar::LiarVertexAttr, FILE*);

		static void ReadString(std::string&, FILE*);

		static std::string basePath;
	};
}

