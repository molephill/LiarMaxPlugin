#include "LiarPluginCfg.h"

namespace Liar
{

	LiarPluginCfg::LiarPluginCfg()
		:exportGeom(true), exportMaterial(true)
		, revertUV(true)
		, exportAnim(true), exportModifier(true)

		, vertexNormal(true), textureCoord(true), vertexColor(false)

		, skeleton(true)
		, skin(true)

		, staticFrame(0), framePerSample(4)

		, pluginVersion(100)

		, skeletonName("")
		, animName("")
		, coordSystemType(IGameConversionManager::CoordSystem::IGAME_OGL)
		, name("")
	{

	}


	LiarPluginCfg::~LiarPluginCfg()
	{
	}
}
