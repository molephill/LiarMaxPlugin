#pragma once

#include <PluginDefine.h>

#ifndef PLUGINS
#include <LiarShader.h>
#endif // !PLUGINS


namespace Liar
{
	class ILiarRender
	{
#ifndef PLUGINS
		virtual void Render(Liar::LiarShaderProgram&) {};
#endif // !PLUGINS

	};
}