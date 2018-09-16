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
		bool revertUV;
		bool exportMaterial;
		bool exportModifier;
		bool exportAnim;

		// geom options;
		bool vertexNormal;
		bool vertexColor;
		bool textureCoord;

		// controller options;
		std::string animName;

		// modifiers options;
		bool skeleton;
		bool skin;
		std::string skeletonName;

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

