#pragma once

#include "LiarPluginCfg.h"
#include <IGame.h>
#include <IGameObject.h>
#include <IGameProperty.h>
#include <IGameControl.h>
#include <IGameModifier.h>
#include <IConversionManager.h>
#include <IGameError.h>

#include <cctype>
#include <algorithm>
#include <windows.h>
#include <string>

#include <vector>

namespace Liar
{
	// 不想引用了，直接复制一份过来，尽量分开
	enum GeometryVertexType
	{
		GEOMETRY_VERTEX_TYPE_NONE,
		GEOMETRY_VERTEX_TYPE_POSITION,
		GEOMETRY_VERTEX_TYPE_POSITION_NORMAL,
		GEOMETRY_VERTEX_TYPE_POSITION_COLOR,
		GEOMETRY_VERTEX_TYPE_POSITION_TEXTURE,
		GEOMETRY_VERTEX_TYPE_POSITION_NORMAL_COLOR,
		GEOMETRY_VERTEX_TYPE_POSITION_NORMAL_TEXTURE,
		GEOMETRY_VERTEX_TYPE_POSITION_NORMAL_COLOR_TEXTURE
	};

	enum VertexElementAttr
	{
		ELEMENT_ATTR_POSITION,
		ELEMENT_ATTR_NORMAL,
		ELEMENT_ATTR_TEXTURECOORDINATE,
		ELEMENT_ATTR_COLOR,
		ELEMENT_ATTR_ROTATION,
		ELEMENT_ATTR_RAW_KEY,
		ELEMENT_ATTR_RAW_INDICES,
		ELEMENT_ATTR_RAW_MTL_INDICES
	};

	static const char* MESH_EXT = ".MESH";
	static const char* MATERIAL_EXT = ".MTL";

	class LiarMaxNodeParse
	{
	public:
		LiarMaxNodeParse();
		~LiarMaxNodeParse();

	public:
		int ParseRootNode(Liar::LiarPluginCfg*, bool);

	private:
		void ParseGameNode(Liar::LiarPluginCfg*, IGameNode*, std::vector<IGameMaterial*>&, int&);
		void ParseValidMeshNode(Liar::LiarPluginCfg*, IGameNode*, IGameObject::ObjectTypes, std::vector<IGameMaterial*>&, int&);
		void ParseChildNode(Liar::LiarPluginCfg*, IGameNode*, std::vector<IGameMaterial*>&, int&);
		void ParseGameMesh(Liar::LiarPluginCfg*, IGameMesh*, std::vector<IGameMaterial*>&, int&);
		void ParseGeometory(Liar::LiarPluginCfg*, IGameMesh*, std::vector<IGameMaterial*>&, int&);

		void ParseBones(Liar::LiarPluginCfg*, IGameNode*);
		void ParseSkin(Liar::LiarPluginCfg*, IGameMesh*);
		void ParseAnim(Liar::LiarPluginCfg*);

		IGameNode* GetMaxNode(int);
		Liar::GeometryVertexType GetVertexType(Liar::LiarPluginCfg*);

		void RevertYZ(Point3&);
		void RevertYZ(Point2&);

		int AddP2(std::vector<Point2>& vec, Point2, bool revert = true);
		int AddP3(std::vector<Point3>& vec, Point3, bool revert = true);
		int AddP4(std::vector<Point4>& vec, Point4);

		// 写数据
		void Write(std::vector<Point2>&, FILE*);
		void Write(std::vector<Point3>&, FILE*);
		void Write(std::vector<Point4>&, FILE*, bool = false);
		void Write(std::vector<int>&, FILE*);
		void Write(std::vector<unsigned int>&, FILE*);
		void Write(const std::string&, FILE*);

		// 写类型数据
		void Write(Liar::VertexElementAttr, std::vector<Point2>&, FILE*);
		void Write(Liar::VertexElementAttr, std::vector<Point3>&, FILE*);
		void Write(Liar::VertexElementAttr, std::vector<Point4>&, FILE*);
		void Write(Liar::VertexElementAttr, std::vector<unsigned int>&, FILE*);

		// 写材质
		void Write(Liar::LiarPluginCfg*, std::vector<IGameMaterial*>&, int meshSize);
		void Write(IGameMaterial*, FILE*);

		// 获得材质数量
		size_t GetTextureSize(IGameMaterial*);

	public:
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

			int nLen = WideCharToMultiByte(CP_ACP, 0, name, -1, NULL, 0, NULL, NULL);

			if (nLen <= 0) return;

			char* pszDst = new char[nLen];
			if (NULL == pszDst) return;

			WideCharToMultiByte(CP_ACP, 0, name, -1, pszDst, nLen, NULL, NULL);
			pszDst[nLen - 1] = 0;

			out = pszDst;
			delete[] pszDst;

		}

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


		static void WChar_tToString(const wchar_t* wchar, std::string& out)
		{
			DWORD dwNum = WideCharToMultiByte(CP_OEMCP, NULL, wchar, -1, NULL, 0, NULL, FALSE);
			char *psText = new char[dwNum];
			WideCharToMultiByte(CP_OEMCP, NULL, wchar, -1, psText, dwNum, NULL, FALSE);
			out = psText;
			delete[] psText;
		}

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

}
