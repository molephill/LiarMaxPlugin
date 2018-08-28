#pragma once

#include "Matrices.h"

namespace Liar
{
	class Quaternion
	{
	public:
		Quaternion();
		Quaternion(float, float, float, float);
		Quaternion(const Liar::Vector3D&);
		Quaternion(const Liar::Vector4D&);
		~Quaternion();

	private:
		float m_values[4];

	public:
		void Identity();
		void Normalize();
		void Invert();

		float DotProduct(const Quaternion&);
		float DotProduct(const Vector4D&);

		float       operator[](int index) const;            // subscript operator v[0], v[1]
		float&      operator[](int index);                  // subscript operator v[0], v[1]

	public:
		static void Mul(const Liar::Quaternion&, const Liar::Quaternion&, Liar::Quaternion&);
		static void Mul(const Liar::Vector4D&, const Liar::Quaternion&, Liar::Quaternion&);
		static void Mul(const Liar::Quaternion&, const Liar::Vector4D&, Liar::Quaternion&);
		static void Mul(const Liar::Vector4D&, const Liar::Vector4D&, Liar::Quaternion&);

		static void Mul(const Liar::Quaternion&, float, Liar::Quaternion&);
		static void Mul(const Liar::Vector4D&, float, Liar::Quaternion&);

		static void Add(const Liar::Quaternion&, const Liar::Quaternion&, Liar::Quaternion&);
		static void Add(const Liar::Vector4D&, const Liar::Quaternion&, Liar::Quaternion&);
		static void Add(const Liar::Quaternion&, const Liar::Vector4D&, Liar::Quaternion&);
		static void Add(const Liar::Vector4D&, const Liar::Vector4D&, Liar::Quaternion&);

		static void GetConjugate(const Liar::Quaternion&, Liar::Quaternion&);

		static void Lerp(const Liar::Quaternion&, const Liar::Quaternion&, float, Liar::Quaternion&);
		static void Lerp(const Liar::Vector4D&, const Liar::Quaternion&, float, Liar::Quaternion&);
		static void Lerp(const Liar::Quaternion&, const Liar::Vector4D&, float, Liar::Quaternion&);
		static void Lerp(const Liar::Vector4D&, const Liar::Vector4D&, float, Liar::Quaternion&);

		static void SlerpNoInvert(const Liar::Quaternion&, const Liar::Quaternion&, float, Liar::Quaternion&);
		static void SlerpNoInvert(const Liar::Vector4D&, const Liar::Quaternion&, float, Liar::Quaternion&);
		static void SlerpNoInvert(const Liar::Quaternion&, const Liar::Vector4D&, float, Liar::Quaternion&);
		static void SlerpNoInvert(const Liar::Vector4D&, const Liar::Vector4D&, float, Liar::Quaternion&);

		static void ToMatrix(const Liar::Quaternion&, Liar::Matrix4&);
		static void ToMatrix(const Liar::Vector4D&, Liar::Matrix4&);

	private:
		static void Scale(float&, float&, float&, float&, float);

		static void Mul(float, float, float, float,
			float, float, float, float,
			Liar::Quaternion&);

		static void Add(float, float, float, float,
			float, float, float, float,
			Liar::Quaternion&);

		static void Lerp(float, float, float, float,
			float, float, float, float,
			float, Liar::Quaternion&);

		static void SlerpNoInvert(float, float, float, float,
			float, float, float, float,
			float, Liar::Quaternion&);

		static void ToMatrix(float, float, float, float, Liar::Matrix4&);
	};
}

