#include "Quaternion.h"
#include <cmath>

namespace Liar
{
	Quaternion::Quaternion()
	{
		Identity();
	}

	Quaternion::Quaternion(float x, float y, float z, float w)
	{
		m_values[0] = x;
		m_values[1] = y;
		m_values[2] = z;
		m_values[3] = w;
	}

	Quaternion::Quaternion(const Liar::Vector3D& v)
	{
		Quaternion(v.x, v.y, v.z, 1.0f);
	}

	Quaternion::Quaternion(const Liar::Vector4D& v)
	{
		Quaternion(v.x, v.y, v.z, v.w);
	}

	Quaternion::~Quaternion()
	{
	}

	void Quaternion::Identity()
	{
		m_values[0] = 0.0f;
		m_values[0] = 0.0f;
		m_values[0] = 0.0f;
		m_values[1] = 1.0f;
	}

	float Quaternion::operator[](int index) const {
		return m_values[index];
	}

	float& Quaternion::operator[](int index) {
		return m_values[index];
	}

	float Quaternion::DotProduct(const Liar::Quaternion& other)
	{
		return m_values[0] * other[0] + m_values[1] * other[1] + m_values[2] * other[2] + m_values[3] * other[3];
	}

	float Quaternion::DotProduct(const Liar::Vector4D& other)
	{
		return m_values[0] * other[0] + m_values[1] * other[1] + m_values[2] * other[2] + m_values[3] * other[3];
	}

	void Quaternion::Normalize()
	{
		float norm = m_values[0] * m_values[0] + m_values[1] * m_values[1] + m_values[2] * m_values[2] + m_values[3] * m_values[3];
		float dNorm = 1 / norm;
		m_values[0] *= dNorm;
		m_values[1] *= dNorm;
		m_values[2] *= dNorm;
		m_values[3] *= dNorm;
	}

	void Quaternion::Invert()
	{
		m_values[0] *= -1;
		m_values[1] *= -1;
		m_values[2] *= -1;
	}

	void Quaternion::Mul(const Liar::Quaternion& left, const Liar::Quaternion& right, Liar::Quaternion& ret)
	{
		Liar::Quaternion::Mul(left[0], left[1], left[2], left[3], right[0], right[1], right[2], right[3], ret);
	}

	void Quaternion::Mul(const Liar::Quaternion& left, const Liar::Vector4D& right, Liar::Quaternion& ret)
	{
		Liar::Quaternion::Mul(left[0], left[1], left[2], left[3], right[0], right[1], right[2], right[3], ret);
	}

	void Quaternion::Mul(const Liar::Vector4D& left, const Liar::Quaternion& right, Liar::Quaternion& ret)
	{
		Liar::Quaternion::Mul(left[0], left[1], left[2], left[3], right[0], right[1], right[2], right[3], ret);
	}

	void Quaternion::Mul(const Liar::Vector4D& left, const Liar::Vector4D& right, Liar::Quaternion& ret)
	{
		Liar::Quaternion::Mul(left[0], left[1], left[2], left[3], right[0], right[1], right[2], right[3], ret);
	}

	void Quaternion::Mul(const Liar::Vector4D& quat, float scalar, Liar::Quaternion& ret)
	{
		ret[0] = quat[0] * scalar;
		ret[1] = quat[1] * scalar;
		ret[2] = quat[2] * scalar;
		ret[3] = quat[3] * scalar;
	}

	void Quaternion::Mul(const Liar::Quaternion& quat, float scalar, Liar::Quaternion& ret)
	{
		ret[0] = quat[0] * scalar;
		ret[1] = quat[1] * scalar;
		ret[2] = quat[2] * scalar;
		ret[3] = quat[3] * scalar;
	}

	void Quaternion::Mul(float left0, float left1, float left2, float left3
		, float right0, float right1, float right2, float right3
		, Liar::Quaternion& ret)
	{
		ret[0] = left1 * right2 - left2 * right1 + left3 * right0 + left0 * right3;
		ret[1] = left2* right0 - left0 * right2 + left3 * right1 + left1 * right3;
		ret[2] = left0 * right1 - left1 * right0 + left3 * right2 + left2 * right3;
		ret[3] = left3 * right3- left0 * right0 - left1 * right1 - left2 * right2;
	}


	void Quaternion::Add(const Liar::Quaternion& left, const Liar::Quaternion& right, Liar::Quaternion& ret)
	{
		Liar::Quaternion::Add(left[0], left[1], left[2], left[3], right[0], right[1], right[2], right[3], ret);
	}

	void Quaternion::Add(const Liar::Vector4D& left, const Liar::Quaternion& right, Liar::Quaternion& ret)
	{
		Liar::Quaternion::Add(left[0], left[1], left[2], left[3], right[0], right[1], right[2], right[3], ret);
	}

	void Quaternion::Add(const Liar::Quaternion& left, const Liar::Vector4D& right, Liar::Quaternion& ret)
	{
		Liar::Quaternion::Add(left[0], left[1], left[2], left[3], right[0], right[1], right[2], right[3], ret);
	}

	void Quaternion::Add(const Liar::Vector4D& left, const Liar::Vector4D& right, Liar::Quaternion& ret)
	{
		Liar::Quaternion::Add(left[0], left[1], left[2], left[3], right[0], right[1], right[2], right[3], ret);
	}

	void Quaternion::Add(float left0, float left1, float left2, float left3
		, float right0, float right1, float right2, float right3
		, Liar::Quaternion& ret)
	{
		ret[0] = left0 + right0;
		ret[1] = left1 + right1;
		ret[2] = left2 + right2;
		ret[3] = left3 + right3;
	}

	/**
	* Return the conjugate quaternion
	* @remark
	*		The math: q* = -x, -y, -z, w
	*/

	void Quaternion::GetConjugate(const Liar::Quaternion& source, Liar::Quaternion& ret)
	{
		ret[0] = -source[0];
		ret[1] = -source[1];
		ret[2] = -source[2];
		ret[3] = source[3];
	}

	/**
	* Linear Interpolation
	*/

	void Quaternion::Lerp(const Liar::Quaternion& from, const Liar::Quaternion& to, float time, Liar::Quaternion& ret)
	{
		Liar::Quaternion::Lerp(from[0], from[1], from[2], from[3], to[0], to[1], to[2], to[3], time, ret);
	}

	void Quaternion::Lerp(const Liar::Vector4D& from, const Liar::Quaternion& to, float time, Liar::Quaternion& ret)
	{
		Liar::Quaternion::Lerp(from[0], from[1], from[2], from[3], to[0], to[1], to[2], to[3], time, ret);
	}

	void Quaternion::Lerp(const Liar::Quaternion& from, const Liar::Vector4D& to, float time, Liar::Quaternion& ret)
	{
		Liar::Quaternion::Lerp(from[0], from[1], from[2], from[3], to[0], to[1], to[2], to[3], time, ret);
	}

	void Quaternion::Lerp(const Liar::Vector4D& from, const Liar::Vector4D& to, float time, Liar::Quaternion& ret)
	{
		Liar::Quaternion::Lerp(from[0], from[1], from[2], from[3], to[0], to[1], to[2], to[3], time, ret);
	}

	void Quaternion::Lerp(float fx, float fy, float fz, float fw,
							float tx, float ty, float tz, float tw,
							float time, Liar::Quaternion& ret)
	{
		Liar::Quaternion::Scale(fx, fy, fz, fw, 1 - time);
		Liar::Quaternion::Scale(tx, ty, tz, tw, time);

		ret[0] = fx + tx;
		ret[1] = fy + ty;
		ret[2] = fz + tz;
		ret[3] = fw + tw;

		ret.Normalize();
	}

	/**
	* Spherical Interpolation with no inversion
	*/
	void Quaternion::SlerpNoInvert(const Liar::Quaternion& from,  const Liar::Quaternion& to, float time, Liar::Quaternion& ret)
	{
		Liar::Quaternion::SlerpNoInvert(from[0], from[1], from[2], from[3], to[0], to[1], to[2], to[3], time, ret);
	}

	void Quaternion::SlerpNoInvert(const Liar::Vector4D& from, const Liar::Quaternion& to, float time, Liar::Quaternion& ret)
	{
		Liar::Quaternion::SlerpNoInvert(from[0], from[1], from[2], from[3], to[0], to[1], to[2], to[3], time, ret);
	}

	void Quaternion::SlerpNoInvert(const Liar::Quaternion& from, const Liar::Vector4D& to, float time, Liar::Quaternion& ret)
	{
		Liar::Quaternion::SlerpNoInvert(from[0], from[1], from[2], from[3], to[0], to[1], to[2], to[3], time, ret);
	}

	void Quaternion::SlerpNoInvert(const Liar::Vector4D& from, const Liar::Vector4D& to, float time, Liar::Quaternion& ret)
	{
		Liar::Quaternion::SlerpNoInvert(from[0], from[1], from[2], from[3], to[0], to[1], to[2], to[3], time, ret);
	}


	void Quaternion::SlerpNoInvert(float fx, float fy, float fz, float fw,
		float tx, float ty, float tz, float tw,
		float time, Liar::Quaternion& ret)
	{
		float cosAngle = fx*tx + fy*ty + fz*tz + fw*tw;
		if (cosAngle < 0.95 && cosAngle > -0.95) 
		{

			float angle = acos(cosAngle);
			float sinInvert = 1 / sin(angle);
			float fromScale = sin(angle * (1 - time)) * sinInvert;
			float toScale = sin(angle * time) * sinInvert;
			
			Liar::Quaternion::Scale(fx, fy, fz, fw, fromScale);
			Liar::Quaternion::Scale(tx, ty, tz, tw, toScale);

			ret[0] = fx + tx;
			ret[1] = fy + ty;
			ret[2] = fz + tz;
			ret[3] = fw + tw;
		}
		else {
			Liar::Quaternion::Lerp(fx, fy, fz, fw, tx, ty, tz, tw, time, ret);
		}
	}

	void Quaternion::Scale(float& x, float& y, float& z, float& w, float scalar)
	{
		x *= scalar;
		y *= scalar;
		z *= scalar;
		w *= scalar;
	}

	/**
	* Return a matrix equivalent to the quaternion.
	* @remark
	*	Assumes the quaternion is normalized (x^2 + y^2 + z^2 + w^2 = 1). No
	*	test is performed to check.
	*/

	void Quaternion::ToMatrix(const Liar::Quaternion& v, Liar::Matrix4& ret)
	{
		Liar::Quaternion::ToMatrix(v[0], v[1], v[2], v[3], ret);
	}

	void Quaternion::ToMatrix(const Liar::Vector4D& v, Liar::Matrix4& ret)
	{
		Liar::Quaternion::ToMatrix(v[0], v[1], v[2], v[3], ret);
	}

	void Quaternion::ToMatrix(float values0, float values1, float values2, float values3, Liar::Matrix4& ret)
	{
		// Row 1
		ret[0] = 1 - 2 * (values1 * values1 + values2 * values2);
		ret[4] = 2 * (values0 * values1 - values3 * values2);
		ret[8] = 2 * (values3 * values1 + values0 * values2);
		ret[12] = 0;

		// Row 2
		ret[1] = 2 * (values0 * values1 + values3 * values2);
		ret[5] = 1 - 2 * (values0 * values0 + values2 * values2);
		ret[9] = 2 * (values1 * values2 - values3 * values0);
		ret[13] = 0;

		// Row 3
		ret[2] = 2 * (values0 * values2 - values3 * values1);
		ret[6] = 2 * (values1 * values2 + values3 * values0);
		ret[10] = 1 - 2 * (values0 * values0 + values1 * values1);
		ret[14] = 0;

		// Row 4
		ret[3] = 0;
		ret[7] = 0;
		ret[11] = 0;
		ret[15] = 1;
	}
}
