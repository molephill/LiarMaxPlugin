#include "LiarPluginCfg.h"

namespace Liar
{

	LiarPluginCfg::LiarPluginCfg()
		:exportGeom(true), exportMaterial(true),
		vertexNormal(true), textureCoord(true)
		, quaternions(true)
		, skin(true)
		, perVertex(true)
		, coordSystemType(LiarCoordSysType::OpenGL)

		, exportSplines(false), exportController(false), exportModifier(false)
		, objectSpace(false), faceBaseOnSmgrsp(false), vertexColor(false), mappingChannels(false)

		, relative(false), constraints(false), forceSample(false)
		, general(false)
		, perFace(false)
		, staticFrame(0), framePerSample(4)
		, pluginVersion(100)
	{
	}


	LiarPluginCfg::~LiarPluginCfg()
	{
	}
}
