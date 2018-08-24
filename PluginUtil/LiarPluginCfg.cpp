#include "LiarPluginCfg.h"

namespace Liar
{

	LiarPluginCfg::LiarPluginCfg()
		:exportGeom(true), exportMaterial(true),
		vertexNormal(true), textureCoord(true)
		, quaternions(true), revertUV(true)
		, skin(true)
		, perVertex(true)

		, exportSplines(false), exportController(false), exportModifier(false)
		, objectSpace(false), faceBaseOnSmgrsp(false), vertexColor(false), mappingChannels(false)

		, relative(false), constraints(false), forceSample(false)
		, skeleton(false), general(false)
		, perFace(false)
		, staticFrame(0), framePerSample(4)
		, pluginVersion(100)
	{
#ifdef PLUGINS
		coordSystemType = IGameConversionManager::CoordSystem::IGAME_OGL;
#endif // PLUGINS

	}


	LiarPluginCfg::~LiarPluginCfg()
	{
	}
}
