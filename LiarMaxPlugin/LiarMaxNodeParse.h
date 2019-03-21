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
#include <map>

namespace Liar
{
	struct IIHeapOperator
	{
		virtual bool operator==(const IIHeapOperator&) const = 0;
		virtual int operator[](size_t) const = 0;
		virtual int& operator[](size_t) = 0;
	};

	struct IFHeapOperator
	{
		virtual bool operator==(const IFHeapOperator&) const = 0;
		virtual float operator[](size_t) const = 0;
		virtual float& operator[](size_t) = 0;
	};

	class Uint1:public IIHeapOperator
	{
	public:
		Uint1(int val1 = 0) :
			IIHeapOperator(),
			index1(val1) {};

		virtual bool operator==(const IIHeapOperator& rhs) const
		{
			return rhs[0] == index1;
		};

		virtual int operator[](size_t index) const
		{
			return (&index1)[index];
		};

		virtual int& operator[](size_t index)
		{
			return (&index1)[index];
		};

	protected:
		int index1;
	};

	class Uint2:public IIHeapOperator
	{
	public:
		Uint2(int val1 = 0, int val2 = 0) :
			IIHeapOperator(),
			index1(val1),
			index2(val2) {};

		virtual bool operator==(const IIHeapOperator& rhs) const
		{
			return rhs[0] == index1 && rhs[1] == index2;
		};

		virtual int operator[](size_t index) const
		{
			return (&index1)[index];
		};

		virtual int& operator[](size_t index)
		{
			return (&index1)[index];
		};

	protected:
		int index1;
		int index2;
	};

	class Uint3 :public IIHeapOperator
	{
	public:
		Uint3(int val1 = 0, int val2 = 0, int val3 = 0) :
			IIHeapOperator(),
			index1(val1),
			index2(val2),
			index3(val3) {};

		virtual bool operator==(const IIHeapOperator& rhs) const
		{
			return rhs[0] == index1 && rhs[1] == index2 && rhs[2] == index3;
		}

		virtual int operator[](size_t index) const
		{
			return (&index1)[index];
		};

		virtual int& operator[](size_t index)
		{
			return (&index1)[index];
		};

	protected:
		int index1;
		int index2;
		int index3;
	};

	class Uint4 :public IIHeapOperator
	{
	public:
		Uint4(int val1 = 0, int val2 = 0, int val3 = 0, int val4 = 0) :
			IIHeapOperator(),
			index1(val1),
			index2(val2),
			index3(val3),
			index4(val4) {};

		virtual bool operator==(const IIHeapOperator& rhs) const
		{
			return rhs[0] == index1 && rhs[1] == index2 && rhs[2] == index3 && rhs[3] == index4;
		}

		virtual int operator[](size_t index) const
		{
			return (&index1)[index];
		};

		virtual int& operator[](size_t index)
		{
			return (&index1)[index];
		};

	protected:
		int index1;
		int index2;
		int index3;
		int index4;
	};

	class Uint5 :public IIHeapOperator
	{
	public:
		Uint5(int val1 = 0, int val2 = 0, int val3 = 0, int val4 = 0, int val5 = 0) :
			IIHeapOperator(),
			index1(val1),
			index2(val2),
			index3(val3),
			index4(val4),
			index5(val5) {};

		virtual bool operator==(const IIHeapOperator& rhs) const
		{
			return rhs[0] == index1 && rhs[1] == index2 && rhs[2] == index3 && rhs[3] == index4 && rhs[4] == index5;
		}

		virtual int operator[](size_t index) const
		{
			return (&index1)[index];
		};

		virtual int& operator[](size_t index)
		{
			return (&index1)[index];
		};

	protected:
		int index1;
		int index2;
		int index3;
		int index4;
		int index5;
	};

	class Uint6 :public IIHeapOperator
	{
	public:
		Uint6(int val1 = 0, int val2 = 0, int val3 = 0, int val4 = 0, int val5 = 0, int val6 = 0) :
			IIHeapOperator(),
			index1(val1),
			index2(val2),
			index3(val3),
			index4(val4),
			index5(val5),
			index6(val6) {};

		virtual bool operator==(const IIHeapOperator& rhs) const
		{
			return rhs[0] == index1 && rhs[1] == index2 && rhs[2] == index3 && rhs[3] == index4 && rhs[4] && rhs[5] == index6;
		};

		virtual int operator[](size_t index) const
		{
			return (&index1)[index];
		};

		virtual int& operator[](size_t index)
		{
			return (&index1)[index];
		};

	protected:
		int index1;
		int index2;
		int index3;
		int index4;
		int index5;
		int index6;
	};


	class Float1:public IFHeapOperator
	{
	public:
		Float1(float val1 = 0) :
			IFHeapOperator(),
			x(val1) {};

		virtual bool operator==(const IFHeapOperator& rhs) const
		{
			return rhs[0] == x;
		};

		virtual float operator[](size_t index) const
		{
			return (&x)[index];
		}

		virtual float& operator[](size_t index)
		{
			return (&x)[index];
		}

	protected:
		float x;
	};

	class Float2 :public IFHeapOperator
	{
	public:
		Float2(float val1 = 0, float val2 = 0) :
			IFHeapOperator(),
			x(val1),
			y(val2) {};

		virtual bool operator==(const IFHeapOperator& rhs) const
		{
			return rhs[0] == x && rhs[1] == y;
		}

		virtual float operator[](size_t index) const
		{
			return (&x)[index];
		}

		virtual float& operator[](size_t index)
		{
			return (&x)[index];
		}

	protected:
		float x;
		float y;
	};

	class Float3 :public IFHeapOperator
	{
	public:
		Float3(float val1 = 0, float val2 = 0, float val3 = 0) :
			IFHeapOperator(),
			x(val1),
			y(val2),
			z(val3) {};

		virtual bool operator==(const IFHeapOperator& rhs) const
		{
			return rhs[0] == x && rhs[1] == y && rhs[2] == z;
		}

		virtual float operator[](size_t index) const
		{
			return (&x)[index];
		}

		virtual float& operator[](size_t index)
		{
			return (&x)[index];
		}

	protected:
		float x;
		float y;
		float z;
	};

	class Float4 :public IFHeapOperator
	{
	public:
		Float4(float val1 = 0, float val2 = 0, float val3 = 0, float val4 = 0) :
			IFHeapOperator(),
			x(val1),
			y(val2),
			z(val3),
			w(val4) {};

		virtual bool operator==(const IFHeapOperator& rhs) const
		{
			return rhs[0] == x && rhs[1] == y && rhs[2] == z && rhs[3] == w;
		}

		virtual float operator[](size_t index) const
		{
			return (&x)[index];
		}

		virtual float& operator[](size_t index)
		{
			return (&x)[index];
		}

	protected:
		float x;
		float y;
		float z;
		float w;
	};

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
		GEOMETRY_VERTEX_TYPE_POSITION_NORMAL_COLOR_TEXTURE,
		GEOMETRY_VERTEX_TYPE_POSITION_TEXTURE_SKIN,
		GEOMETRY_VERTEX_TYPE_POSITION_NORMAL_COLOR_SKIN,
		GEOMETRY_VERTEX_TYPE_POSITION_NORMAL_TEXTURE_SKIN,
		GEOMETRY_VERTEX_TYPE_POSITION_NORMAL_COLOR_TEXTURE_SKIN
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
		ELEMENT_ATTR_BONE_INDICES,
		ELEMENT_ATTR_BONE_WEIGHTS,
		ELEMENT_ATTR_SCALE
	};

	static const char* MESH_EXT = ".MESH";
	static const char* MATERIAL_EXT = ".MTL";
	static const char* SKELEON_EXT = ".SKE";
	static const char* ANI_EXT = ".ANI";

	class LiarMaxNodeParse
	{
	public:
		LiarMaxNodeParse();
		~LiarMaxNodeParse();

	public:
		int ParseRootNode(Liar::LiarPluginCfg*, bool);

	private:
		std::vector<IGameNode*> m_bones;

		void ParseGameNode(Liar::LiarPluginCfg*, IGameNode*, std::vector<IGameMaterial*>&, int&);
		void ParseValidMeshNode(Liar::LiarPluginCfg*, IGameNode*, IGameObject::ObjectTypes, std::vector<IGameMaterial*>&, int&);
		void ParseChildNode(Liar::LiarPluginCfg*, IGameNode*, std::vector<IGameMaterial*>&, int&);
		void ParseGameMesh(Liar::LiarPluginCfg*, IGameMesh*, std::vector<IGameMaterial*>&, int&);
		void ParseGeometory(Liar::LiarPluginCfg*, IGameMesh*, std::vector<IGameMaterial*>&, int&);

		void ParseBones(Liar::LiarPluginCfg*, IGameNode*);
		size_t ParseSkin(std::map<int, std::vector<Point2>>&, std::vector<Point3>&, Liar::LiarPluginCfg*, IGameMesh*);
		void ParseAnim(Liar::LiarPluginCfg*);

		// 解析skin信息
		IIHeapOperator* NewUintBase(size_t);
		IFHeapOperator* NewFloatBase(size_t);
		void ParseSkinInfo(size_t, Point3, const std::vector<Point3>&, std::map<int, std::vector<Point2>>&, std::vector<IIHeapOperator*>&, std::vector<IFHeapOperator*>&, int&, int&);

		Liar::GeometryVertexType GetVertexType(Liar::LiarPluginCfg*);

		// 获得顶点索引
		int GetVertsIndex(const std::vector<Point3>&, Point3);
		int GetBoneIndex(const std::string&);

		void RevertYZ(Point3&);
		void RevertYZ(Point2&);

		int AddPoint(std::vector<Point2>& vec, Point2, bool revert = false);
		int AddPoint(std::vector<Point3>& vec, Point3, bool revert = false);
		int AddPoint(std::vector<Point4>& vec, Point4);
		int AddPoint(std::vector<Quat>&, Quat);
		int AddPoint(std::vector<IIHeapOperator*>&, IIHeapOperator*);
		int AddPoint(std::vector<IFHeapOperator*>&, IFHeapOperator*);
		void AddBones(IGameNode*);

		// 写数据
		void Write(std::vector<Point2>&, FILE*);
		void Write(std::vector<Point3>&, FILE*);
		void Write(std::vector<Point4>&, FILE*, bool = false);
		void Write(std::vector<Quat>&, FILE*);
		void Write(std::vector<IIHeapOperator*>&, size_t, FILE*);
		void Write(std::vector<IFHeapOperator*>&, size_t, FILE*);
		void Write(std::vector<int>&, FILE*);
		void Write(std::vector<unsigned int>&, FILE*);
		void Write(std::vector<float>&, FILE*);
		void Write(const std::string&, FILE*);

		// 写骨骼
		void WriteSkeleon(Liar::LiarPluginCfg*);

		// 写类型数据
		void Write(Liar::VertexElementAttr, std::vector<Point2>&, FILE*);
		void Write(Liar::VertexElementAttr, std::vector<Point3>&, FILE*);
		void Write(Liar::VertexElementAttr, std::vector<Point4>&, FILE*);
		void Write(Liar::VertexElementAttr, std::vector<Quat>&, FILE*);
		void Write(Liar::VertexElementAttr, std::vector<IIHeapOperator*>&, size_t, FILE*);
		void Write(Liar::VertexElementAttr, std::vector<IFHeapOperator*>&, size_t, FILE*);
		void Write(Liar::VertexElementAttr, std::vector<unsigned int>&, FILE*);
		void Write(Liar::VertexElementAttr, std::vector<float>&, FILE*);

		// 写入可变动的数据
		void Write(Liar::GeometryVertexType, std::vector<Point3>&, std::vector<Point2>&, std::vector<Point3>&, std::vector<IIHeapOperator*>&, std::vector<IFHeapOperator*>&, size_t, FILE*);

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
