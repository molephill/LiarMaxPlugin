#pragma once

#include <PluginDefine.h>

#ifdef PLUGINS
#include <IConversionManager.h>
#endif // PLUGINS


namespace Liar
{
	class LiarPluginCfg
	{
	public:
		LiarPluginCfg();
		~LiarPluginCfg();

	public:
		// export options
		bool exportGeom;
		bool exportSplines;
		bool exportController;
		bool exportMaterial;
		bool exportModifier;

		// geom options;
		bool objectSpace;
		bool faceBaseOnSmgrsp;
		bool mappingChannels;
		bool vertexNormal;
		bool vertexColor;
		bool textureCoord;

		// controller options;
		bool relative;
		bool constraints;
		bool forceSample;
		bool quaternions;

		// modifiers options;
		bool skeleton;
		bool skin;
		bool general;

		// normals;
		bool perVertex;
		bool perFace;

		// frame
		int staticFrame;
		int framePerSample;

#ifdef PLUGINS
		// coord System
		IGameConversionManager::CoordSystem coordSystemType;
#endif // PLUGINS

		// version
		int pluginVersion;

	};
}

