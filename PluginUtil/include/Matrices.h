#pragma once

///////////////////////////////////////////////////////////////////////////////
// Matrice.h
// =========
// NxN Matrix Math classes
//
// The elements of the matrix are stored as column major order.
// | 0 2 |    | 0 3 6 |    |  0  4  8 12 |
// | 1 3 |    | 1 4 7 |    |  1  5  9 13 |
//            | 2 5 8 |    |  2  6 10 14 |
//                         |  3  7 11 15 |
//
// Dependencies: Vector2D, Vector3D, Vector4D
///////////////////////////////////////////////////////////////////////////////

#include <Vectors.h>
#include <PluginDefine.h>

#include <iostream>
#include <iomanip>
#include <vector>

namespace Liar
{
	class Matrix2
	{
	public:
		Matrix2();
		Matrix2(const float src[4]);
		Matrix2(const std::vector<float>&);
		Matrix2(float m0, float m1, float m2, float m3);

	public:
		void Set(const float src[4]);
		void Set(const std::vector<float>&);
		void Set(float, float, float, float);

		void				SetRow(int, const float src[2]);
		void				SetRow(int, const std::vector<float>&);
		void				SetRow(int, const Liar::Vector2D&);

		void				SetCol(int, const float src[2]);
		void				SetCol(int, const std::vector<float>&);
		void				SetCol(int, const Liar::Vector2D&);

		const float*				GetRawData() const { return m.s; };

	public:
		void				Add(const Liar::Matrix2&);
		void				Sub(const Liar::Matrix2&);
		void				Mul(const Liar::Matrix2&);
		void				Mul(Liar::Vector2D&);
		void				Mul(const Liar::Vector2D&, Liar::Vector2D&);
		void				Mul(float v);

	public:
		float			   GetDeterminant() const;
		float			   GetAngle() const;                       // retrieve angle (degree) from matrix

		Liar::Matrix2&	   Identity();
		Liar::Matrix2&	   Transpose();
		Liar::Matrix2&	   Invert();

		// operators
		Liar::Matrix2     operator+(const Liar::Matrix2& rhs) const;    // add rhs
		Liar::Matrix2     operator-(const Liar::Matrix2& rhs) const;    // subtract rhs
		Liar::Matrix2&    operator+=(const Liar::Matrix2& rhs);         // add rhs and update this object
		Liar::Matrix2&    operator-=(const Liar::Matrix2& rhs);         // subtract rhs and update this object
		Liar::Vector2D    operator*(const Liar::Vector2D& rhs) const;    // multiplication: v' = M * v
		Liar::Matrix2     operator*(const Liar::Matrix2& rhs) const;    // multiplication: M3 = M1 * M2
		Liar::Matrix2&    operator*=(const Liar::Matrix2& rhs);         // multiplication: M1' = M1 * M2
		bool        operator==(const Liar::Matrix2& rhs) const;   // exact compare, no epsilon
		bool        operator!=(const Liar::Matrix2& rhs) const;   // exact compare, no epsilon
		float       operator[](int index) const;            // subscript operator v[0], v[1]
		float&      operator[](int index);                  // subscript operator v[0], v[1]

															// friends functions
		friend Liar::Matrix2 operator-(const Liar::Matrix2& m);                     // unary operator (-)
		friend Liar::Matrix2 operator*(float scalar, const Liar::Matrix2& m);       // pre-multiplication
		friend Liar::Vector2D operator*(const Liar::Vector2D& vec, const Matrix2& m); // pre-multiplication
		friend std::ostream& operator<<(std::ostream& os, const Liar::Matrix2& m);



	private:
		union Matrix2Union
		{
			float ss[2][2];
			float s[4];
		}m;
	};

	///////////////////////////////////////////////////////////////////////////
	// 3x3 matrix
	///////////////////////////////////////////////////////////////////////////
	class Matrix3
	{
	public:
		Matrix3();
		Matrix3(const float src[9]);
		Matrix3(const std::vector<float>&);
		Matrix3(float m0, float m1, float m2,           // 1st column
				float m3, float m4, float m5,           // 2nd column
				float m6, float m7, float m8);          // 3rd column

	public:
		void				Set(const float src[9]);
		void				Set(const std::vector<float>&);
		void				Set(float m0, float m1, float m2,   // 1st column
								float m3, float m4, float m5,   // 2nd column
								float m6, float m7, float m8);  // 3rd column

		void				SetRow(int, const float src[3]);
		void				SetRow(int, const std::vector<float>&);
		void				SetRow(int, const Liar::Vector3D&);

		void				SetCol(int, const float src[3]);
		void				SetCol(int, const std::vector<float>&);
		void				SetCol(int, const Liar::Vector3D&);

		const float*		GetRawData() const { return m.s; };

	public:
		void				Add(const Liar::Matrix3&);
		void				Sub(const Liar::Matrix3&);
		void				Mul(const Liar::Matrix3&);
		void				Mul(Liar::Vector3D&);
		void				Mul(const Liar::Vector3D&, Liar::Vector3D&);
		void				Mul(float v);

	public:
		float				GetDeterminant() const;
		Liar::Vector3D		GetAngle() const;
		void				GetAngle(Liar::Vector3D&) const;

		Liar::Matrix3&	   Identity();
		Liar::Matrix3&	   Transpose();
		Liar::Matrix3&	   Invert();

		// operators
		Liar::Matrix3     operator+(const Liar::Matrix3& rhs) const;    // add rhs
		Liar::Matrix3     operator-(const Liar::Matrix3& rhs) const;    // subtract rhs
		Liar::Matrix3&    operator+=(const Liar::Matrix3& rhs);         // add rhs and update this object
		Liar::Matrix3&    operator-=(const Liar::Matrix3& rhs);         // subtract rhs and update this object
		Liar::Vector3D     operator*(const Liar::Vector3D& rhs) const;    // multiplication: v' = M * v
		Liar::Matrix3     operator*(const Liar::Matrix3& rhs) const;    // multiplication: M3 = M1 * M2
		Liar::Matrix3&    operator*=(const Liar::Matrix3& rhs);         // multiplication: M1' = M1 * M2
		bool        operator==(const Liar::Matrix3& rhs) const;   // exact compare, no epsilon
		bool        operator!=(const Liar::Matrix3& rhs) const;   // exact compare, no epsilon
		float       operator[](int index) const;            // subscript operator v[0], v[1]
		float&      operator[](int index);                  // subscript operator v[0], v[1]

															// friends functions
		friend Liar::Matrix3 operator-(const Liar::Matrix3& m);                     // unary operator (-)
		friend Liar::Matrix3 operator*(float scalar, const Liar::Matrix3& m);       // pre-multiplication
		friend Liar::Vector3D operator*(const Liar::Vector3D& vec, const Liar::Matrix3& m); // pre-multiplication
		friend std::ostream& operator<<(std::ostream& os, const Liar::Matrix3& m);

	private:
		union Matrix3Union
		{
			float ss[3][3];
			float s[6];
		}m;
	};


	///////////////////////////////////////////////////////////////////////////
	// 4x4 matrix
	///////////////////////////////////////////////////////////////////////////
	class Matrix4
	{
	public:
		Matrix4();  // init with identity
		Matrix4(const float src[16]);
		Matrix4(const std::vector<float>&);
		Matrix4(float m00, float m01, float m02, float m03, // 1st column
				float m04, float m05, float m06, float m07, // 2nd column
				float m08, float m09, float m10, float m11, // 3rd column
				float m12, float m13, float m14, float m15);// 4th column

	public:
		void					Set(const float src[16]);
		void					Set(const std::vector<float>&);
		void					Set(float m00, float m01, float m02, float m03, // 1st column
									float m04, float m05, float m06, float m07, // 2nd column
									float m08, float m09, float m10, float m11, // 3rd column
									float m12, float m13, float m14, float m15);// 4th column

		void					SetRow(int, const float row[4]);
		void					SetRow(int, const std::vector<float>&);
		void					SetRow(int, const Liar::Vector4D&);
		void					SetRow(int, const Liar::Vector3D&);

		void					SetCol(int, const float row[4]);
		void					SetCol(int, const std::vector<float>&);
		void					SetCol(int, const Liar::Vector4D&);
		void					SetCol(int, const Liar::Vector3D&);

		const float*			GetRawData() const { return m.s; };

	public:
		void					GetTranspose(std::vector<float>&);
		float					GetDeterminant() const;
		Liar::Matrix3			GetRotationMatrix() const;
		void					GetRotationMatrix(Liar::Matrix3&) const;
		Liar::Vector3D			GetAngle() const;
		void					GetAngle(Liar::Vector3D&) const;
		void					GetAngle(float&, float&, float&) const;

	public:
		Liar::Matrix4&    Identity();
		Liar::Matrix4&    Transpose();                            // transpose itself and return reference
		Liar::Matrix4&    Invert();                               // check best inverse method before inverse
		Liar::Matrix4&    InvertEuclidean();                      // inverse of Euclidean transform matrix
		Liar::Matrix4&    InvertAffine();                         // inverse of affine transform matrix
		Liar::Matrix4&    InvertProjective();                     // inverse of projective matrix using partitioning
		Liar::Matrix4&    InvertGeneral();                        // inverse of generic matrix

															// transform matrix
		Liar::Matrix4&    Translate(float x, float y, float z);   // translation by (x,y,z)
		Liar::Matrix4&    Translate(const Liar::Vector3D& v);            //
		Liar::Matrix4&    Rotate(float angle, const Liar::Vector3D& axis); // rotate angle(degree) along the given axix
		Liar::Matrix4&    Rotate(float angle, float x, float y, float z);
		Liar::Matrix4&	  Rotate(float angleX, float angleY, float angleZ);
		Liar::Matrix4&    RotateX(float angle);                   // rotate on X-axis with degree
		Liar::Matrix4&    RotateY(float angle);                   // rotate on Y-axis with degree
		Liar::Matrix4&    RotateZ(float angle);                   // rotate on Z-axis with degree
		Liar::Matrix4&    Scale(float scale);                     // uniform scale
		Liar::Matrix4&    Scale(float sx, float sy, float sz);    // scale by (sx, sy, sz) on each axis
		Liar::Matrix4&    LookAt(float tx, float ty, float tz);   // face object to the target direction
		Liar::Matrix4&    LookAt(float tx, float ty, float tz, float ux, float uy, float uz);
		Liar::Matrix4&    LookAt(const Liar::Vector3D& target);
		Liar::Matrix4&    LookAt(const Liar::Vector3D& target, const Liar::Vector3D& up);
		//@@Matrix4&    skew(float angle, const Vector3& axis); //

		// operators
		Liar::Matrix4     operator+(const Liar::Matrix4& rhs) const;    // add rhs
		Liar::Matrix4     operator-(const Liar::Matrix4& rhs) const;    // subtract rhs
		Liar::Matrix4&    operator+=(const Liar::Matrix4& rhs);         // add rhs and update this object
		Liar::Matrix4&    operator-=(const Liar::Matrix4& rhs);         // subtract rhs and update this object
		Liar::Vector4D     operator*(const Liar::Vector4D& rhs) const;    // multiplication: v' = M * v
		Liar::Vector3D     operator*(const Liar::Vector3D& rhs) const;    // multiplication: v' = M * v
		Liar::Matrix4     operator*(const Liar::Matrix4& rhs) const;    // multiplication: M3 = M1 * M2
		Liar::Matrix4&    operator*=(const Liar::Matrix4& rhs);         // multiplication: M1' = M1 * M2
		bool        operator==(const Liar::Matrix4& rhs) const;   // exact compare, no epsilon
		bool        operator!=(const Liar::Matrix4& rhs) const;   // exact compare, no epsilon
		float       operator[](int index) const;            // subscript operator v[0], v[1]
		float&      operator[](int index);                  // subscript operator v[0], v[1]

															// friends functions
		friend Liar::Matrix4 operator-(const Liar::Matrix4& m);                     // unary operator (-)
		friend Liar::Matrix4 operator*(float scalar, const Matrix4& m);       // pre-multiplication
		friend Liar::Vector3D operator*(const Liar::Vector3D& vec, const Liar::Matrix4& m); // pre-multiplication
		friend Liar::Vector4D operator*(const Liar::Vector4D& vec, const Liar::Matrix4& m); // pre-multiplication
		friend std::ostream& operator<<(std::ostream& os, const Liar::Matrix4& m);

	private:
		float       GetCofactor(float m0, float m1, float m2,
					float m3, float m4, float m5,
					float m6, float m7, float m8) const;

	private:
		union Matrix4Union
		{
			float ss[4][4];
			float s[16];
		}m;
	};


	///////////////////////////////////////////////////////////////////////////
	// inline functions for Matrix2
	///////////////////////////////////////////////////////////////////////////
	inline Matrix2::Matrix2()
	{
		// initially identity matrix
		Identity();
	}

	inline Matrix2::Matrix2(const float src[4])
	{
		Set(src);
	}

	inline Matrix2::Matrix2(const std::vector<float>& src)
	{
		Set(src);
	}

	inline Matrix2::Matrix2(float m0, float m1, float m2, float m3)
	{
		Set(m0, m1, m2, m3);
	}

	inline void Matrix2::Set(const float src[4])
	{
		m.s[0] = src[0];  m.s[1] = src[1];  m.s[2] = src[2];  m.s[3] = src[3];
	}

	inline void Matrix2::Set(const std::vector<float>& src)
	{
		m.s[0] = src[0];  m.s[1] = src[1];  m.s[2] = src[2];  m.s[3] = src[3];
	}

	inline void Matrix2::Set(float vm0, float vm1, float vm2, float vm3)
	{
		m.s[0] = vm0;  m.s[1] = vm1;  m.s[2] = vm2;  m.s[3] = vm3;
	}

	inline Matrix2& Matrix2::Identity()
	{
		m.s[0] = m.s[3] = 1.0f;
		m.s[1] = m.s[2] = 0.0f;
		return *this;
	}

	inline Matrix2 Matrix2::operator+(const Matrix2& rhs) const
	{
		return Matrix2(m.s[0] + rhs[0], m.s[1] + rhs[1], m.s[2] + rhs[2], m.s[3] + rhs[3]);
	}

	inline Matrix2 Matrix2::operator-(const Matrix2& rhs) const
	{
		return Matrix2(m.s[0] - rhs[0], m.s[1] - rhs[1], m.s[2] - rhs[2], m.s[3] - rhs[3]);
	}

	inline Matrix2& Matrix2::operator+=(const Matrix2& rhs)
	{
		m.s[0] = m.s[0] + rhs[0]; m.s[1] = m.s[1] + rhs[1]; m.s[2] = m.s[2] + rhs[2]; m.s[3] = m.s[3] + rhs[3];
		return *this;
	}

	inline Matrix2& Matrix2::operator-=(const Matrix2& rhs)
	{
		m.s[0] = m.s[0] - rhs[0]; m.s[1] = m.s[1] - rhs[1]; m.s[2] -= rhs[2]; m.s[3] -= rhs[3];
		return *this;
	}

	inline Vector2D Matrix2::operator*(const Liar::Vector2D& rhs) const
	{
		return Liar::Vector2D(m.s[0] * rhs.x + m.s[2] * rhs.y, m.s[1] * rhs.x + m.s[3] * rhs.y);
	}

	inline Matrix2 Matrix2::operator*(const Liar::Matrix2& rhs) const
	{
		return Liar::Matrix2(m.s[0] * rhs[0] + m.s[2] * rhs[1], m.s[1] * rhs[0] + m.s[3] * rhs[1],
							 m.s[0] * rhs[2] + m.s[2] * rhs[3], m.s[1] * rhs[2] + m.s[3] * rhs[3]);
	}

	inline Matrix2& Matrix2::operator*=(const Matrix2& rhs)
	{
		float t0 = m.s[0], t1 = m.s[1], t2 = m.s[2], t3 = m.s[3];
		m.s[0] = t0 * rhs[0] + t2 * rhs[1];
		m.s[1] = t1 * rhs[0] + t3 * rhs[1];
		m.s[2] = t0 * rhs[2] + t2 * rhs[3];
		m.s[3] = t1 * rhs[2] + t3 * rhs[3];
		return *this;
	}

	inline float Matrix2::operator[](int index) const
	{
		return m.s[index];
	}

	inline float& Matrix2::operator[](int index)
	{
		return m.s[index];
	}

	inline bool Matrix2::operator==(const Matrix2& rhs) const
	{
		return (m.s[0] == rhs[0]) && (m.s[1] == rhs[1]) && (m.s[2] == rhs[2]) && (m.s[3] == rhs[3]);
	}

	inline bool Matrix2::operator!=(const Matrix2& rhs) const
	{
		return (m.s[0] != rhs[0]) || (m.s[1] != rhs[1]) || (m.s[2] != rhs[2]) || (m.s[3] != rhs[3]);
	}

	inline Matrix2 operator-(const Matrix2& rhs)
	{
		return Liar::Matrix2(-rhs[0], -rhs[1], -rhs[2], -rhs[3]);
	}

	inline Matrix2 operator*(float s, const Matrix2& rhs)
	{
		return Matrix2(s*rhs[0], s*rhs[1], s*rhs[2], s*rhs[3]);
	}

	inline Liar::Vector2D operator*(const Liar::Vector2D& v, const Matrix2& rhs)
	{
		return Liar::Vector2D(v.x*rhs[0] + v.y*rhs[1], v.x*rhs[2] + v.y*rhs[3]);
	}

	inline std::ostream& operator<<(std::ostream& os, const Matrix2& m)
	{
		os << std::fixed << std::setprecision(5);
		os << "[" << std::setw(10) << m[0] << " " << std::setw(10) << m[2] << "]\n"
			<< "[" << std::setw(10) << m[1] << " " << std::setw(10) << m[3] << "]\n";
		os << std::resetiosflags(std::ios_base::fixed | std::ios_base::floatfield);
		return os;
	}
	// END OF MATRIX2 INLINE //////////////////////////////////////////////////////




	///////////////////////////////////////////////////////////////////////////
	// inline functions for Matrix3
	///////////////////////////////////////////////////////////////////////////
	inline Matrix3::Matrix3()
	{
		// initially identity matrix
		Identity();
	}

	inline Matrix3::Matrix3(const float src[9])
	{
		Set(src);
	}

	inline Matrix3::Matrix3(const std::vector<float>& src)
	{
		Set(src);
	}

	inline Matrix3::Matrix3(float m0, float m1, float m2,
							float m3, float m4, float m5,
							float m6, float m7, float m8)
	{
		Set(m0, m1, m2, m3, m4, m5, m6, m7, m8);
	}



	inline void Matrix3::Set(const float src[9])
	{
		m.s[0] = src[0];  m.s[1] = src[1];  m.s[1] = src[2];
		m.s[3] = src[3];  m.s[4] = src[4];  m.s[5] = src[5];
		m.s[6] = src[6];  m.s[7] = src[7];  m.s[8] = src[8];
	}

	inline void Matrix3::Set(const std::vector<float>& src)
	{
		m.s[0] = src[0];  m.s[1] = src[1];  m.s[1] = src[2];
		m.s[3] = src[3];  m.s[4] = src[4];  m.s[5] = src[5];
		m.s[6] = src[6];  m.s[7] = src[7];  m.s[8] = src[8];
	}

	inline void Matrix3::Set(float mv0, float mv1, float mv2,
								float mv3, float mv4, float mv5,
								float mv6, float mv7, float mv8)
	{
		m.s[0] = mv0;  m.s[1] = mv1;  m.s[2] = mv2;
		m.s[3] = mv3;  m.s[4] = mv4;  m.s[5] = mv5;
		m.s[6] = mv6;  m.s[7] = mv7;  m.s[8] = mv8;
	}

	inline Matrix3& Matrix3::Identity()
	{
		m.s[0] = m.s[4] = m.s[8] = 1.0f;
		m.s[1] = m.s[2] = m.s[3] = m.s[5] = m.s[6] = m.s[7] = 0.0f;
		return *this;
	}

	inline Matrix3 Matrix3::operator+(const Matrix3& rhs) const
	{
		return Matrix3( m.s[0] + rhs[0], m.s[1] + rhs[1], m.s[2] + rhs[2],
						m.s[3] + rhs[1], m.s[4] + rhs[4], m.s[5] + rhs[5],
						m.s[6] + rhs[6], m.s[7] + rhs[7], m.s[8] + rhs[8]);
	}

	inline Matrix3 Matrix3::operator-(const Matrix3& rhs) const
	{
		return Matrix3( m.s[0] - rhs[0], m.s[1] - rhs[1], m.s[2] - rhs[2],
						m.s[3] - rhs[1], m.s[4] - rhs[4], m.s[5] - rhs[5],
						m.s[6] - rhs[6], m.s[7] - rhs[7], m.s[8] - rhs[8]);
	}

	inline Matrix3& Matrix3::operator+=(const Matrix3& rhs)
	{
		m.s[0] += rhs[0]; m.s[1] += rhs[1]; m.s[2] += rhs[2];
		m.s[3] += rhs[1]; m.s[4] += rhs[4]; m.s[5] += rhs[5];
		m.s[6] += rhs[6]; m.s[7] += rhs[7]; m.s[8] += rhs[8];
		return *this;
	}

	inline Matrix3& Matrix3::operator-=(const Matrix3& rhs)
	{
		m.s[0] -= rhs[0]; m.s[1] -= rhs[1]; m.s[2] -= rhs[2];
		m.s[3] -= rhs[1]; m.s[4] -= rhs[4]; m.s[5] -= rhs[5];
		m.s[6] -= rhs[6]; m.s[7] -= rhs[7]; m.s[8] -= rhs[8];
		return *this;
	}

	inline Vector3D Matrix3::operator*(const Vector3D& rhs) const
	{
		return Vector3D(m.s[0] * rhs.x + m.s[3] * rhs.y + m.s[6] * rhs.z,
						m.s[1] * rhs.x + m.s[4] * rhs.y + m.s[7] * rhs.z,
						m.s[2] * rhs.x + m.s[5] * rhs.y + m.s[8] * rhs.z);
	}

	inline Matrix3 Matrix3::operator*(const Matrix3& rhs) const
	{
		return Matrix3( m.s[0] * rhs[0] + m.s[3] * rhs[1] + m.s[6] * rhs[2],	m.s[1] * rhs[0] + m.s[4] * rhs[1] + m.s[7] * rhs[2],	m.s[2] * rhs[0] + m.s[5] * rhs[1] + m.s[8] * rhs[2],
						m.s[0] * rhs[3] + m.s[3] * rhs[4] + m.s[6] * rhs[5],	m.s[1] * rhs[3] + m.s[4] * rhs[4] + m.s[7] * rhs[5],	m.s[2] * rhs[3] + m.s[5] * rhs[4] + m.s[8] * rhs[5],
						m.s[0] * rhs[6] + m.s[3] * rhs[7] + m.s[6] * rhs[8],	m.s[1] * rhs[6] + m.s[4] * rhs[7] + m.s[7] * rhs[8],	m.s[2] * rhs[6] + m.s[5] * rhs[7] + m.s[8] * rhs[8]);
	}

	inline Matrix3& Matrix3::operator*=(const Matrix3& rhs)
	{
		float t0 = m.s[0], t1 = m.s[1], t2 = m.s[2], t3 = m.s[3], t4 = m.s[4], t5 = m.s[5], t6 = m.s[6], t7 = m.s[7], t8 = m.s[8];
		m.s[0] = t0 * rhs[0] + t3 * rhs[1] + t6 * rhs[2];
		m.s[1] = t1 * rhs[0] + t4 * rhs[1] + t7 * rhs[2];
		m.s[2] = t2 * rhs[0] + t5 * rhs[1] + t8 * rhs[2];
		m.s[3] = t0 * rhs[3] + t3 * rhs[4] + t6 * rhs[5];
		m.s[4] = t1 * rhs[3] + t4 * rhs[4] + t7 * rhs[5];
		m.s[5] = t2 * rhs[3] + t5 * rhs[4] + t8 * rhs[5];
		m.s[6] = t0 * rhs[6] + t3 * rhs[7] + t6 * rhs[8];
		m.s[7] = t1 * rhs[6] + t4 * rhs[7] + t7 * rhs[8];
		m.s[8] = t2 * rhs[6] + t5 * rhs[7] + t8 * rhs[8];
		return *this;
	}

	inline bool Matrix3::operator==(const Matrix3& rhs) const
	{
		return (m.s[0] == rhs[0]) && (m.s[1] == rhs[1]) && (m.s[2] == rhs[2]) &&
				(m.s[3] == rhs[3]) && (m.s[4] == rhs[4]) && (m.s[5] == rhs[5]) &&
				(m.s[6] == rhs[6]) && (m.s[7] == rhs[7]) && (m.s[8] == rhs[8]);
	}

	inline bool Matrix3::operator!=(const Matrix3& rhs) const
	{
		return (m.s[0] != rhs[0]) || (m.s[1] != rhs[1]) || (m.s[2] != rhs[2]) ||
				(m.s[3] != rhs[3]) || (m.s[4] != rhs[4]) || (m.s[5] != rhs[5]) ||
				(m.s[6] != rhs[6]) || (m.s[7] != rhs[7]) || (m.s[8] != rhs[8]);
	}

	inline Matrix3 operator-(const Matrix3& rhs)
	{
		return Matrix3(-rhs[0], -rhs[1], -rhs[2], -rhs[3], -rhs[4], -rhs[5], -rhs[6], -rhs[7], -rhs[8]);
	}

	inline Matrix3 operator*(float s, const Matrix3& rhs)
	{
		return Matrix3(s*rhs[0], s*rhs[1], s*rhs[2], s*rhs[3], s*rhs[4], s*rhs[5], s*rhs[6], s*rhs[7], s*rhs[8]);
	}

	inline Vector3D operator*(const Vector3D& v, const Matrix3& m)
	{
		return Vector3D(v.x*m[0] + v.y*m[1] + v.z*m[2], v.x*m[3] + v.y*m[4] + v.z*m[5], v.x*m[6] + v.y*m[7] + v.z*m[8]);
	}

	inline float Matrix3::operator[](int index) const
	{
		return m.s[index];
	}

	inline float& Matrix3::operator[](int index)
	{
		return m.s[index];
	}

	inline std::ostream& operator<<(std::ostream& os, const Matrix3& m)
	{
		os << std::fixed << std::setprecision(5);
		os << "[" << std::setw(10) << m[0] << " " << std::setw(10) << m[3] << " " << std::setw(10) << m[6] << "]\n"
			<< "[" << std::setw(10) << m[1] << " " << std::setw(10) << m[4] << " " << std::setw(10) << m[7] << "]\n"
			<< "[" << std::setw(10) << m[2] << " " << std::setw(10) << m[5] << " " << std::setw(10) << m[8] << "]\n";
		os << std::resetiosflags(std::ios_base::fixed | std::ios_base::floatfield);
		return os;
	}
	// END OF MATRIX3 INLINE //////////////////////////////////////////////////////




	///////////////////////////////////////////////////////////////////////////
	// inline functions for Matrix4
	///////////////////////////////////////////////////////////////////////////
	inline Matrix4::Matrix4()
	{
		// initially identity matrix
		Identity();
	}

	inline Matrix4::Matrix4(const float src[16])
	{
		Set(src);
	}

	inline Matrix4::Matrix4(const std::vector<float>& src)
	{
		Set(src);
	}

	inline Matrix4::Matrix4(float m00, float m01, float m02, float m03,
							float m04, float m05, float m06, float m07,
							float m08, float m09, float m10, float m11,
							float m12, float m13, float m14, float m15)
	{
		Set(m00, m01, m02, m03, m04, m05, m06, m07, m08, m09, m10, m11, m12, m13, m14, m15);
	}

	inline void Matrix4::Set(const float src[16])
	{
		m.s[0] = src[0];  m.s[1] = src[1];  m.s[2] = src[2];  m.s[3] = src[3];
		m.s[4] = src[4];  m.s[5] = src[5];  m.s[6] = src[6];  m.s[7] = src[7];
		m.s[8] = src[8];  m.s[9] = src[9];  m.s[10] = src[10]; m.s[11] = src[11];
		m.s[12] = src[12]; m.s[13] = src[13]; m.s[14] = src[14]; m.s[15] = src[15];
	}

	inline void Matrix4::Set(const std::vector<float>& src)
	{
		m.s[0] = src[0];  m.s[1] = src[1];  m.s[2] = src[2];  m.s[3] = src[3];
		m.s[4] = src[4];  m.s[5] = src[5];  m.s[6] = src[6];  m.s[7] = src[7];
		m.s[8] = src[8];  m.s[9] = src[9];  m.s[10] = src[10]; m.s[11] = src[11];
		m.s[12] = src[12]; m.s[13] = src[13]; m.s[14] = src[14]; m.s[15] = src[15];
	}

	inline void Matrix4::Set(float m00, float m01, float m02, float m03,
		float m04, float m05, float m06, float m07,
		float m08, float m09, float mv10, float mv11,
		float mv12, float mv13, float mv14, float mv15)
	{
		m.s[0] = m00;  m.s[1] = m01;  m.s[2] = m02;  m.s[3] = m03;
		m.s[4] = m04;  m.s[5] = m05;  m.s[6] = m06;  m.s[7] = m07;
		m.s[8] = m08;  m.s[9] = m09;  m.s[10] = mv10;  m.s[11] = mv11;
		m.s[12] = mv12;  m.s[13] = mv13;  m.s[14] = mv14;  m.s[15] = mv15;
	}

	inline void Matrix4::GetTranspose(std::vector<float>& out)
	{
		out = { m.s[0], m.s[1], m.s[2], m.s[3], m.s[4], m.s[5], m.s[6], m.s[7], m.s[8], m.s[9], m.s[10], m.s[11], m.s[12], m.s[13], m.s[14], m.s[15] };
	}

	inline Matrix4& Matrix4::Identity()
	{
		m.s[0] = m.s[5] = m.s[10] = m.s[15] = 1.0f;
		m.s[1] = m.s[2] = m.s[3] = m.s[4] = m.s[6] = m.s[7] = m.s[8] = m.s[9] = m.s[11] = m.s[12] = m.s[13] = m.s[14] = 0.0f;
		return *this;
	}

	inline Matrix4 Matrix4::operator+(const Matrix4& rhs) const
	{
		return Matrix4(	m.s[0] + rhs[0], m.s[1] + rhs[1], m.s[2] + rhs[2], m.s[3] + rhs[3],
						m.s[4] + rhs[4], m.s[5] + rhs[5], m.s[6] + rhs[6], m.s[7] + rhs[7],
						m.s[8] + rhs[8], m.s[9] + rhs[9], m.s[10] + rhs[10], m.s[11] + rhs[11],
						m.s[12] + rhs[12], m.s[13] + rhs[13], m.s[14] + rhs[14], m.s[15] + rhs[15]);
	}



	inline Matrix4 Matrix4::operator-(const Matrix4& rhs) const
	{
		return Matrix4(m.s[0] - rhs[0], m.s[1] - rhs[1], m.s[2] - rhs[2], m.s[3] - rhs[3],
						m.s[4] - rhs[4], m.s[5] - rhs[5], m.s[6] - rhs[6], m.s[7] - rhs[7],
						m.s[8] - rhs[8], m.s[9] - rhs[9], m.s[10] - rhs[10], m.s[11] - rhs[11],
						m.s[12] - rhs[12], m.s[13] - rhs[13], m.s[14] - rhs[14], m.s[15] - rhs[15]);
	}



	inline Matrix4& Matrix4::operator+=(const Matrix4& rhs)
	{
		m.s[0] += rhs[0]; m.s[1] += rhs[1]; m.s[2] -= rhs[2]; m.s[3] -= rhs[3];
		m.s[4] += rhs[4]; m.s[5] += rhs[5]; m.s[6] -= rhs[6]; m.s[7] -= rhs[7];
		m.s[8] += rhs[8]; m.s[9] += rhs[9]; m.s[10] -= rhs[10]; m.s[11] -= rhs[11];
		m.s[12] += rhs[12]; m.s[13] -= rhs[13]; m.s[14] -= rhs[14]; m.s[15] -= rhs[15];
		return *this;
	}

	inline Matrix4& Matrix4::operator-=(const Matrix4& rhs)
	{
		m.s[0] -= rhs[0]; m.s[1] -= rhs[1]; m.s[2] -= rhs[2]; m.s[3] -= rhs[3];
		m.s[4] -= rhs[4]; m.s[5] -= rhs[5]; m.s[6] -= rhs[6]; m.s[7] -= rhs[7];
		m.s[8] -= rhs[8]; m.s[9] -= rhs[9]; m.s[10] -= rhs[10]; m.s[11] -= rhs[11];
		m.s[12] -= rhs[12]; m.s[13] -= rhs[13]; m.s[14] -= rhs[14]; m.s[15] -= rhs[15];
		return *this;
	}

	inline Vector4D Matrix4::operator*(const Vector4D& rhs) const
	{
		return Vector4D(m.s[0] * rhs.x + m.s[4] * rhs.y + m.s[8] * rhs.z + m.s[12] * rhs.w,
						m.s[1] * rhs.x + m.s[5] * rhs.y + m.s[9] * rhs.z + m.s[13] * rhs.w,
						m.s[2] * rhs.x + m.s[6] * rhs.y + m.s[10] * rhs.z + m.s[14] * rhs.w,
						m.s[3] * rhs.x + m.s[7] * rhs.y + m.s[11] * rhs.z + m.s[15] * rhs.w);
	}

	inline Vector3D Matrix4::operator*(const Vector3D& rhs) const
	{
		return Vector3D(m.s[0] * rhs.x + m.s[4] * rhs.y + m.s[8] * rhs.z + m.s[12],
						m.s[1] * rhs.x + m.s[5] * rhs.y + m.s[9] * rhs.z + m.s[13],
						m.s[2] * rhs.x + m.s[6] * rhs.y + m.s[10] * rhs.z + m.s[14]);
	}

	inline Matrix4 Matrix4::operator*(const Matrix4& n) const
	{
		return Matrix4(m.s[0] * n[0] + m.s[4] * n[1] + m.s[8] * n[2] + m.s[12] * n[3],		m.s[1] * n[0] + m.s[5] * n[1] + m.s[9] * n[2] + m.s[13] * n[3],		m.s[2] * n[0] + m.s[6] * n[1] + m.s[10] * n[2] + m.s[14] * n[3],	m.s[3] * n[0] + m.s[7] * n[1] + m.s[11] * n[2] + m.s[15] * n[3],
						m.s[0] * n[4] + m.s[4] * n[5] + m.s[8] * n[6] + m.s[12] * n[7],		m.s[1] * n[4] + m.s[5] * n[5] + m.s[9] * n[6] + m.s[13] * n[7],		m.s[2] * n[4] + m.s[6] * n[5] + m.s[10] * n[6] + m.s[14] * n[7],	m.s[3] * n[4] + m.s[7] * n[5] + m.s[11] * n[6] + m.s[15] * n[7],
						m.s[0] * n[8] + m.s[4] * n[9] + m.s[8] * n[10] + m.s[12] * n[11],	m.s[1] * n[8] + m.s[5] * n[9] + m.s[9] * n[10] + m.s[13] * n[11],	m.s[2] * n[8] + m.s[6] * n[9] + m.s[10] * n[10] + m.s[14] * n[11],	m.s[3] * n[8] + m.s[7] * n[9] + m.s[11] * n[10] + m.s[15] * n[11],
						m.s[0] * n[12] + m.s[4] * n[13] + m.s[8] * n[14] + m.s[12] * n[15], m.s[1] * n[12] + m.s[5] * n[13] + m.s[9] * n[14] + m.s[13] * n[15], m.s[2] * n[12] + m.s[6] * n[13] + m.s[10] * n[14] + m.s[14] * n[15], m.s[3] * n[12] + m.s[7] * n[13] + m.s[11] * n[14] + m.s[15] * n[15]);
	}

	inline Matrix4& Matrix4::operator*=(const Matrix4& n)
	{
		float tmp0 = m.s[0], tmp1 = m.s[1], tmp2 = m.s[2], tmp3 = m.s[3];
		float tmp4 = m.s[4], tmp5 = m.s[5], tmp6 = m.s[6], tmp7 = m.s[7];
		float tmp8 = m.s[8], tmp9 = m.s[9], tmp10 = m.s[10], tmp11 = m.s[11];
		float tmp12 = m.s[12], tmp13 = m.s[13], tmp14 = m.s[14], tmp15 = m.s[15];

		m.s[0] = tmp0 * n[0] + tmp4 * n[1] + tmp8 * n[2] + tmp12 * n[3];
		m.s[1] = tmp1 * n[0] + tmp5 * n[1] + tmp9 * n[2] + tmp13 * n[3];
		m.s[2] = tmp2 * n[0] + tmp6 * n[1] + tmp10 * n[2] + tmp14 * n[3];
		m.s[3] = tmp3 * n[0] + tmp7 * n[1] + tmp11 * n[2] + tmp15 * n[3];
		m.s[4] = tmp0 * n[4] + tmp4 * n[5] + tmp8 * n[6] + tmp12 * n[7];
		m.s[5] = tmp1 * n[4] + tmp5 * n[5] + tmp9 * n[6] + tmp13 * n[7];
		m.s[6] = tmp2 * n[4] + tmp6 * n[5] + tmp10 * n[6] + tmp14 * n[7];
		m.s[7] = tmp3 * n[4] + tmp7 * n[5] + tmp11 * n[6] + tmp15 * n[7];
		m.s[8] = tmp0 * n[8] + tmp4 * n[9] + tmp8 * n[10] + tmp12 * n[11];
		m.s[9] = tmp1 * n[8] + tmp5 * n[9] + tmp9 * n[10] + tmp13 * n[11];
		m.s[10] = tmp2 * n[8] + tmp6 * n[9] + tmp10 * n[10] + tmp14 * n[11];
		m.s[11] = tmp3 * n[8] + tmp7 * n[9] + tmp11 * n[10] + tmp15 * n[11];
		m.s[12] = tmp0 * n[12] + tmp4 * n[13] + tmp8 * n[14] + tmp12 * n[15];
		m.s[13] = tmp1 * n[12] + tmp5 * n[13] + tmp9 * n[14] + tmp13 * n[15];
		m.s[14] = tmp2 * n[12] + tmp6 * n[13] + tmp10 * n[14] + tmp14 * n[15];
		m.s[15] = tmp3 * n[12] + tmp7 * n[13] + tmp11 * n[14] + tmp15 * n[15];
		return *this;
	}

	inline bool Matrix4::operator==(const Matrix4& n) const
	{
		return (m.s[0] == n[0]) && (m.s[1] == n[1]) && (m.s[2] == n[2]) && (m.s[3] == n[3]) &&
				(m.s[4] == n[4]) && (m.s[5] == n[5]) && (m.s[6] == n[6]) && (m.s[7] == n[7]) &&
				(m.s[8] == n[8]) && (m.s[9] == n[9]) && (m.s[10] == n[10]) && (m.s[11] == n[11]) &&
				(m.s[12] == n[12]) && (m.s[13] == n[13]) && (m.s[14] == n[14]) && (m.s[15] == n[15]);
	}

	inline bool Matrix4::operator!=(const Matrix4& n) const
	{
		return (m.s[0] != n[0]) || (m.s[1] != n[1]) || (m.s[2] != n[2]) || (m.s[3] != n[3]) ||
				(m.s[4] != n[4]) || (m.s[5] != n[5]) || (m.s[6] != n[6]) || (m.s[7] != n[7]) ||
				(m.s[8] != n[8]) || (m.s[9] != n[9]) || (m.s[10] != n[10]) || (m.s[11] != n[11]) ||
				(m.s[12] != n[12]) || (m.s[13] != n[13]) || (m.s[14] != n[14]) || (m.s[15] != n[15]);
	}

	inline Matrix4 operator-(const Matrix4& rhs)
	{
		return Matrix4(-rhs[0], -rhs[1], -rhs[2], -rhs[3], -rhs[4], -rhs[5], -rhs[6], -rhs[7], -rhs[8], -rhs[9], -rhs[10], -rhs[11], -rhs[12], -rhs[13], -rhs[14], -rhs[15]);
	}

	inline Matrix4 operator*(float s, const Matrix4& rhs)
	{
		return Matrix4(s*rhs[0], s*rhs[1], s*rhs[2], s*rhs[3], s*rhs[4], s*rhs[5], s*rhs[6], s*rhs[7], s*rhs[8], s*rhs[9], s*rhs[10], s*rhs[11], s*rhs[12], s*rhs[13], s*rhs[14], s*rhs[15]);
	}

	inline Vector4D operator*(const Vector4D& v, const Matrix4& m)
	{
		return Vector4D(v.x*m[0] + v.y*m[1] + v.z*m[2] + v.w*m[3], v.x*m[4] + v.y*m[5] + v.z*m[6] + v.w*m[7], v.x*m[8] + v.y*m[9] + v.z*m[10] + v.w*m[11], v.x*m[12] + v.y*m[13] + v.z*m[14] + v.w*m[15]);
	}

	inline Vector3D operator*(const Vector3D& v, const Matrix4& m)
	{
		return Vector3D(v.x*m[0] + v.y*m[1] + v.z*m[2], v.x*m[4] + v.y*m[5] + v.z*m[6], v.x*m[8] + v.y*m[9] + v.z*m[10]);
	}

	inline float Matrix4::operator[](int index) const
	{
		return m.s[index];
	}

	inline float& Matrix4::operator[](int index)
	{
		return m.s[index];
	}

	inline std::ostream& operator<<(std::ostream& os, const Matrix4& m)
	{
		os << std::fixed << std::setprecision(5);
		os << "[" << std::setw(10) << m[0] << " " << std::setw(10) << m[4] << " " << std::setw(10) << m[8] << " " << std::setw(10) << m[12] << "]\n"
			<< "[" << std::setw(10) << m[1] << " " << std::setw(10) << m[5] << " " << std::setw(10) << m[9] << " " << std::setw(10) << m[13] << "]\n"
			<< "[" << std::setw(10) << m[2] << " " << std::setw(10) << m[6] << " " << std::setw(10) << m[10] << " " << std::setw(10) << m[14] << "]\n"
			<< "[" << std::setw(10) << m[3] << " " << std::setw(10) << m[7] << " " << std::setw(10) << m[11] << " " << std::setw(10) << m[15] << "]\n";
		os << std::resetiosflags(std::ios_base::fixed | std::ios_base::floatfield);
		return os;
	}

}

