#pragma once
// maxs version
//#define PLUGINS 2019

// export params
#define LIAR_NORMAL   1
#define LIAR_COLOR	  2
#define LIAR_UV		  3

const float DEG2RAD = 3.141593f / 180.0f;
const float RAD2DEG = 180.0f / 3.141593f;
const float EPSILON = 0.00001f;

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