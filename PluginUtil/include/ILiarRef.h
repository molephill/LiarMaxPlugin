#pragma once

namespace Liar
{
	class ILiarRef
	{
	public:
		ILiarRef() :m_refCount(0) {};
		~ILiarRef() {};

	private:
		int m_refCount;

	public:
		int IncRefCount() { return ++m_refCount; };
		int DesRefCount() { return --m_refCount; };
	};
}

