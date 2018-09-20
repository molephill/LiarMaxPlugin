#pragma once
namespace Liar
{
	// maxs version
	#define PLUGINS 2019

	const float DEG2RAD = 3.141593f / 180.0f;
	const float RAD2DEG = 180.0f / 3.141593f;
	const float EPSILON = 0.00001f;

	enum LiarVertexType
	{
		LiarVertexType_POSITION = 0x0,
		LiarVertexType_NORMAL = 0x1,
		LiarVertexType_COLOR = 0x2,
		LiarVertexType_TEX = 0x3
	};

	enum LiarVertexAttr
	{
		LiarVertexAttr_TRANSFORM = 0x0,
		LiarVertexAttr_ROTATION = 0x1,
		LiarVertexAttr_SCALE	= 0x2
	};

	enum LiarTextureType
	{
		/** Dummy value.
		*
		*  No texture, but the value to be used as 'texture semantic'
		*  (#aiMaterialProperty::mSemantic) for all material properties
		*  *not* related to textures.
		*/
		LiarTextureType_NONE = 0x0,

		/** The texture is combined with the result of the diffuse
		*  lighting equation.
		*/
		LiarTextureType_DIFFUSE = 0x1,

		/** The texture is combined with the result of the specular
		*  lighting equation.
		*/
		LiarTextureType_SPECULAR = 0x2,

		/** The texture is combined with the result of the ambient
		*  lighting equation.
		*/
		LiarTextureType_AMBIENT = 0x3,

	};
}