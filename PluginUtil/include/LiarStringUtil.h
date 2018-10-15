#pragma once
#include <string>

#ifdef __APPLE__
#include <cctype>
#include <algorithm>
#else
#include <cctype>
#include <algorithm>
#include <windows.h>
#endif //

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
