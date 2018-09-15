#include "LiarSkeleton.h"

namespace Liar
{
	// ================================ skin =======================
	LiarSkin::LiarSkin() :
		bonId(-1), weight(0.0f)
	{

	}

	LiarSkin::~LiarSkin()
	{

	}

	// =============================== bone =========================
	LiarBone::LiarBone() :
		id(-1), parentId(-1),name("")
	{

	}

	LiarBone::~LiarBone()
	{

	}


	LiarSkeleton::LiarSkeleton()
	{
	}


	LiarSkeleton::~LiarSkeleton()
	{
	}

}