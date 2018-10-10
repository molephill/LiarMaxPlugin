#pragma once

#include <PluginDefine.h>

namespace Liar
{
	class LiarReference
	{
	public:
#ifndef PLUGINS
		LiarReference() :m_refCount(0) {};
#else
		LiarReference() {};
#endif // !PLUGINS
		virtual ~LiarReference() {};

#ifndef PLUGINS
	protected:
		int m_refCount;

	public:
		int IncRefCount() { return ++m_refCount; };
		int DesRefCount() { return --m_refCount; };
#endif // !PLUGINS
	
	};
}

