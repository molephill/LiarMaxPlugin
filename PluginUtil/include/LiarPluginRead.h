#pragma once

#include "PluginDefine.h"

#ifndef PLUGINS
#include "Model.h"
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
#endif // !PLUGINS
		static void ReadNode(const char*, Liar::LiarNode&);
		static Liar::LiarMesh* ReadMesh(const char* path, const char* basePath=nullptr);

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
		static void ReadLiarMaterial(Liar::LiarMesh*, FILE*);

		static void ReadLiarVertexBuffer(Liar::LiarVertexBuffer*, FILE*, bool, bool, bool);
		static Liar::LiarTexture* ReadLiarTexture(FILE*);

		static void ReadString(std::string&, FILE*);

		static std::string basePath;
	};
}

