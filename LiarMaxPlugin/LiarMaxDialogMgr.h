#pragma once

#include <windef.h>

namespace Liar
{
	class LiarPluginCfg;

	class LiarMaxDialogMgr
	{
	public:
		LiarMaxDialogMgr();
		~LiarMaxDialogMgr();

	private:
		void SetGeoEnable(HWND);
		void SetControllerEnable(HWND);
		void SetModifierEnable(HWND);
		void SetVertexNormalEnable(HWND);
		void ConfirmHandle(HWND);

	public:
		void Init(HWND, LPARAM);
		void ChangeHandle(HWND, WPARAM, bool, const std::string&);

	private:
		LiarPluginCfg* m_pluginCfg;
	};
}

