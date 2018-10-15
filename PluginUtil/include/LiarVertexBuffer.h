#pragma once

#include "LiarSkeleton.h"
#include <LiarVertexBuffer.h>

#include <vector>


namespace Liar
{
	class LiarNode
	{
	public:
		LiarNode();
		~LiarNode();

	private:
		std::string m_nodeName;
		std::vector<Liar::LiarNode*>* m_children;

	public:
		unsigned int meshIndex;

	public:
		LiarNode* AddChild();

		void SetNodeName(const char* nodeName) { m_nodeName = nodeName; };
		void SetNodeName(const std::string& nodeName) { m_nodeName = nodeName; };

		std::string& GetNodeName() { return m_nodeName; };

		std::vector<Liar::LiarNode*>* GetChildren() const { return m_children; };

		int GetNumChildren() const { return m_children ? static_cast<int>(m_children->size()) : 0; };
	};

	// =================================== vertex_define ===================================
	class LiarFaceDefine
	{
	public:
		LiarFaceDefine() :positionIndex(0), normalIndex(0), texCoordIndex(0), colorIndex(0) {};
		LiarFaceDefine(unsigned int posIndex, unsigned int normIndex, unsigned int texIndex, unsigned int corIndex) :
			positionIndex(posIndex), normalIndex(normIndex), texCoordIndex(texIndex), colorIndex(corIndex) {}
		~LiarFaceDefine() {};
		
	public:
		unsigned int positionIndex;
		unsigned int normalIndex;
		unsigned int texCoordIndex;
		unsigned int colorIndex;

		void Set(unsigned int posIndex, unsigned int normIndex, unsigned int texIndex, unsigned int corIndex) 
		{
			positionIndex = posIndex;
			normalIndex = normIndex;
			texCoordIndex = texIndex;
			colorIndex = corIndex;
		};

	public:
		bool operator==(const LiarFaceDefine& rhs) const
		{
			return rhs.positionIndex == positionIndex && rhs.normalIndex == normalIndex && rhs.texCoordIndex == texCoordIndex && rhs.colorIndex == colorIndex;
		}

		bool Equal(unsigned int posIndex, unsigned int normIndex, unsigned int texIndex, unsigned int corIndex = 0)
		{
			return positionIndex == posIndex && normalIndex == normIndex && texCoordIndex == texIndex && colorIndex == corIndex;
		}

	};

	// =================================== anim_skin_define ===================================
	class LiarAnimSkinDefine
	{
	public:
		LiarAnimSkinDefine(int posIndex = 0);
		~LiarAnimSkinDefine();

	private:
		int m_positionIndex;
		std::vector<int>* m_boneIds;
		std::vector<float>* m_weights;

	public:
		void SetPositionIndex(int v) { m_positionIndex = v; };
		int GetPositionIndex() const { return m_positionIndex; };

		size_t GetBoneInfoSize() { return m_boneIds->size(); };
		int GetBoneId(size_t i) const { return m_boneIds->at(i); };
		float GetBoneWeight(size_t i) const { return m_weights->at(i); };

		void AddBoneInfo(int, float);
	};

	class LiarTextureDefine
	{
	public:
		LiarTextureDefine(int type):m_type(type){};
		~LiarTextureDefine() {};

	private:
		std::string m_path;
		int m_type;

	public:
		std::string GetPath() { return m_path; };
		int GetType() const { return m_type; };
		void SetPath(const std::string& path) { m_path = path; };
	};

	class LiarMaterialDefine
	{
	public:
		LiarMaterialDefine():m_texs(new std::vector<Liar::LiarTextureDefine*>()){};
		~LiarMaterialDefine() 
		{
			for (std::vector<Liar::LiarTextureDefine*>::iterator it = m_texs->begin(); it != m_texs->end(); ++it)
			{
				delete (*it);
			}
		};

	private:
		std::vector<Liar::LiarTextureDefine*>* m_texs;

	public:
		std::vector<Liar::LiarTextureDefine*>* GetTextures() { return m_texs; };
		size_t GetTexSize() const { return m_texs->size(); };
		Liar::LiarTextureDefine* GetTexture(size_t i) { return m_texs->at(i); };
	};

	// =================================== raw_data ===================================
	class LiarMeshRawData
	{
	public:
		LiarMeshRawData();
		~LiarMeshRawData();

	public:
		std::string meshName;
		std::string saveName;

	private:
		std::vector<Vector3D*>* m_pos;					// pos
		std::vector<Vector3D*>* m_norm;					// normal
		std::vector<Vector3D*>* m_color;				// color
		std::vector<Vector3D*>* m_texCoord;				// tex

		std::vector<Liar::LiarAnimSkinDefine*>* m_skinDefines;
		std::vector<Liar::LiarFaceDefine*>* m_faces;
		std::vector<Liar::LiarMaterialDefine*>* m_materials;

		std::vector<int>* m_indices;

	public:
		std::vector<Vector3D*>* GetPos() { return m_pos; };
		Vector3D* GetPos(size_t index) { return m_pos ? m_pos->at(index) : nullptr; };

		std::vector<Vector3D*>* GetNorm() { return m_norm; };
		Vector3D* GetNorm(size_t index) { return m_norm ? m_norm->at(index) : nullptr; };

		std::vector<Vector3D*>* GetColor() { return m_color; };
		Vector3D* GetColor(size_t index) { return m_color ? m_color->at(index) : nullptr; };

		std::vector<Vector3D*>* GetTexCoord() { return m_texCoord; };
		Vector3D* GetTexCoord(size_t index) { return m_texCoord ? m_texCoord->at(index) : nullptr; };

		void SetPos(std::vector<Vector3D*>* v) { m_pos = v; };
		void SetNorm(std::vector<Vector3D*>* v) { m_norm = v; };
		void SetColor(std::vector<Vector3D*>* v) { m_color = v; };
		void SetTexCoord(std::vector<Vector3D*>* v) { m_texCoord = v; };

		Liar::Vector3D* AddPos(size_t);
		Liar::Vector3D* AddNorm(size_t);
		Liar::Vector3D* AddColor(size_t);
		Liar::Vector3D* AddTex(size_t);
		void AddMaterial(Liar::LiarMaterialDefine* v)
		{
			if (!v) return;
			if (!m_materials) m_materials = new std::vector<Liar::LiarMaterialDefine*>();
			m_materials->push_back(v);
		}
        
        bool HasNorm() { return m_norm ? true : false; };
        bool HasColor() { return m_color ? true : false; };
        bool HasTexCoord() { return m_texCoord ? true : false; };
		bool HasSkin() { return m_skinDefines ? true : false; };

		Liar::Vector3D* AddData(Liar::LiarVertexType, size_t);
		int GetIndex(Liar::LiarVertexType, float, float, float);
		int GetIndex(Liar::LiarVertexType, const Liar::Vector3D&);

		std::vector<Liar::LiarFaceDefine*>* GetFaces() { return m_faces; };

		std::vector<Liar::LiarMaterialDefine*>* GetMatrials() { return m_materials; };
		size_t GetMatrialSize() const { return m_materials ? m_materials->size() : 0; };

		std::vector<int>* GetIndices() { return m_indices; };

		size_t GetFaceSize() const { return m_faces->size(); };

		// ======================
		Liar::LiarAnimSkinDefine* GetAnimSkinDefine(int, bool add = false);
		std::vector<Liar::LiarAnimSkinDefine*>* GetSkinDefines() { return m_skinDefines; };
		size_t GetSkinDefineLen() const { return m_skinDefines ? m_skinDefines->size() : 0; };

		Liar::LiarFaceDefine* FindFaceDefine(const Liar::LiarFaceDefine&);
		Liar::LiarFaceDefine* FindFaceDefine(unsigned int, unsigned int, unsigned int, unsigned int cor = 0);

		int FindFaceDefineIndex(const Liar::LiarFaceDefine&) const;
		int FindFaceDefineIndex(unsigned int, unsigned int, unsigned int, unsigned int cor = 0) const;

	private:
		std::vector<Liar::Vector3D*>* GetData(Liar::LiarVertexType);
		void EraseAll(std::vector<Liar::Vector3D*>*);
	};

	class LiarVertexBuffer
	{
	public:
		LiarVertexBuffer();
		~LiarVertexBuffer();

	public:
		Liar::Vector3D* position;
		Liar::Vector3D* normal;
		Liar::Vector3D* color;
		Liar::Vector3D* uv;

		friend std::ostream& operator<<(std::ostream& os, const Liar::LiarVertexBuffer& m)
		{
			if (m.position) os << "position: " << *(m.position) << "\n";
			if (m.normal) os << "normal: " << *(m.normal) << "\n";
			if (m.color) os << "color: " << *(m.color) << "\n";
			if (m.uv) os << "uv: " << *(m.uv) << "\n";
			return os;
		}

	public:
		static int GetPositionSize() { return sizeof(Liar::Vector3D); };
		static int GetNormalSize() { return sizeof(Liar::Vector3D); };
		static int GetColorSize() { return sizeof(Liar::Vector3D); };
		static int GetUVSize() { return sizeof(Liar::Vector3D); };

		static int GetPositionOffSize() { return 0; };
		static int GetNormalOffSize() { return Liar::LiarVertexBuffer::GetPositionSize(); };
		static int GetColorOffSize()
		{
			return Liar::LiarVertexBuffer::GetNormalOffSize() + Liar::LiarVertexBuffer::GetNormalSize();
		}

		static int GetUVOffSize()
		{
			return Liar::LiarVertexBuffer::GetColorOffSize() + Liar::LiarVertexBuffer::GetColorSize();
		}

		static int GetBuffSize() 
		{
			return Liar::LiarVertexBuffer::GetPositionSize() + Liar::LiarVertexBuffer::GetNormalSize()
				+ Liar::LiarVertexBuffer::GetColorSize() + Liar::LiarVertexBuffer::GetUVSize();
		}

	};
}

