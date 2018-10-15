#pragma once


#include <string>
#include <stdio.h>

#include <LiarVertexBuffer.h>
#include <LiarSkeletonAnim.h>

namespace Liar
{
	class LiarPluginRead
	{
	public:
		static void ReadNode(const char*, Liar::LiarNode&);

		static Liar::LiarMeshRawData* ReadMeshRawData(const char*);

		static Liar::LiarSkeleton* ReadSkeleton(const char*);
		static Liar::LiarSkeleton* ReadSkeleton(const std::string& path);

		static Liar::LiarSkeletonAnim* ReadAnim(const char*);
		static Liar::LiarSkeletonAnim* ReadAnim(const std::string& path);

	private:

		// ================================ read child node ===============================
		static void ReadChildNode(Liar::LiarNode&, std::string&, FILE*);
		// ================================ read child node ===============================

		static std::vector<Liar::Vector3D*>* ReadLiarVecs(FILE*);

		static void ReadLiarKeyFrames(Liar::LiarTrack*, Liar::LiarVertexAttr, FILE*);

		static void ReadString(std::string&, FILE*);
	};
}

