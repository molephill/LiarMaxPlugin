#pragma once
#include <string>

#include <PluginDefine.h>

#ifdef PLUGINS
#include <impexp.h>
#include <iparamb2.h>
#include <iparamm2.h>
#endif

#ifdef __APPLE__
#include <cctype>
#include <algorithm>
#else
#include <cctype>
#include <algorithm>
#include <windows.h>
#endif // PLUGINS

namespace Liar
{
	class StringUtil
	{
	public:
		static std::string GetLast(const std::string& name, const char* split = "\\")
		{
			std::string::size_type pos = name.find_last_of(split);
			if (pos != std::string::npos)
			{
				return name.substr(pos + 1);
			}
			else
			{
				return name;
			}
		}

		static std::string GetHead(const std::string& name, const char* split = ".")
		{
			std::string::size_type pos = name.find_last_of(split);
			if (pos != std::string::npos)
			{
				return name.substr(0, pos);
			}
			else
			{
				return name;
			}
		}

		static void GetHeadAndLast(const std::string& name, std::string& head, std::string& last, const char* split = ".")
		{
			std::string::size_type pos = name.find_last_of(split);
			if (pos != std::string::npos)
			{
				head = name.substr(0, pos);
				last = name.substr(pos + 1);
			}
		}

#ifdef PLUGINS
		static void GetWSTR2Char(const WStr& name, std::string& out)
		{
			char tmpName[_MAX_PATH];
			size_t len = name.length() + 1;
			size_t converted = 0;
			wcstombs_s(&converted, tmpName, len, name.data(), _TRUNCATE);
			out = tmpName;
		}


		static void GetTChar2Char(const TCHAR* name, std::string& out)
		{
			/*size_t len = wcslen(name) + 1;
			size_t converted = 0;
			char tmpName[_MAX_PATH];
			wcstombs_s(&converted, tmpName, len, name, _TRUNCATE);
			out = tmpName;*/

			char *pszBuf = NULL;
			int needBytes = WideCharToMultiByte(CP_ACP, 0, name, -1, NULL, 0, NULL, NULL);
			if (needBytes > 0) {
				pszBuf = new char[needBytes + 1];
				ZeroMemory(pszBuf, (needBytes + 1) * sizeof(char));
				WideCharToMultiByte(CP_ACP, 0, name, -1, pszBuf, needBytes, NULL, NULL);
			}
			out = pszBuf;

		}
#endif // !PLUGINS


#ifndef __APPLE__
		static void WChar_tToString(const wchar_t* wchar, std::string& out)
		{
			DWORD dwNum = WideCharToMultiByte(CP_OEMCP, NULL, wchar, -1, NULL, 0, NULL, FALSE);
			char *psText = new char[dwNum];
			WideCharToMultiByte(CP_OEMCP, NULL, wchar, -1, psText, dwNum, NULL, FALSE);
			out = psText;
			delete[] psText;
		}
#endif

		static void StringToUpper(std::string& name)
		{
			std::transform(name.begin(), name.end(), name.begin(), ::toupper);
		}

		static void StringToLower(std::string& strExt)
		{
			std::transform(strExt.begin(), strExt.end(), strExt.begin(), ::tolower);
		}
        
        static std::string GetSourcePath(const char* source, const char* base)
        {
            std::string sourceStr = std::string(source);
            std::string baseStr = std::string(base);
            return sourceStr + baseStr;
        }
	};

	// ============================= version eg.. ctr =========================
	class LairVersionCtr
	{
	public:
		static bool CheckVertexOpen(int ver, int pos)
		{
			return ((ver >> pos) & 1) > 0;
		}
	};
}
