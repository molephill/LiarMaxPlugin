#pragma once
namespace Liar
{
	class LiarReference
	{
	public:
		LiarReference() :m_refCount(1) {};
		virtual ~LiarReference() {};

	protected:
		int m_refCount;

	public:
		int IncRefCount() { return ++m_refCount; };
		int DesRefCount() { return --m_refCount; };	
	};
}

