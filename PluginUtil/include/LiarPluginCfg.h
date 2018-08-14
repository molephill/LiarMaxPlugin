#pragma once

namespace Liar
{
	enum LiarCoordSysType
	{
		Max3DS = 0,
		DirectX,
		OpenGL
	};

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
		bool skin;
		bool general;

		// normals;
		bool perVertex;
		bool perFace;

		// frame
		int staticFrame;
		int framePerSample;

		// coord System
		LiarCoordSysType coordSystemType;

		// version
		int pluginVersion;

	};
}

