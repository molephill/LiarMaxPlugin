#pragma once

#include <IConversionManager.h>

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

		// coord System
		IGameConversionManager::CoordSystem coordSystemType;


		// version
		int pluginVersion;

	};
}

