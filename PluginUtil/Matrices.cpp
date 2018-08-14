#include "Matrices.h"

namespace Liar
{
	///////////////////////////////////////////////////////////////////////////////
	// transpose 2x2 matrix
	///////////////////////////////////////////////////////////////////////////////
	Matrix2& Matrix2::Transpose()
	{
		std::swap(m.s[1], m.s[2]);
		return *this;
	}

	///////////////////////////////////////////////////////////////////////////////
	// return the determinant of 2x2 matrix
	///////////////////////////////////////////////////////////////////////////////
	float Matrix2::GetDeterminant() const
	{
		return m.s[0] * m.s[3] - m.s[1] * m.s[2];
	}

	///////////////////////////////////////////////////////////////////////////////
	// inverse of 2x2 matrix
	// If cannot find inverse, set identity matrix
	///////////////////////////////////////////////////////////////////////////////
	Matrix2& Matrix2::Invert()
	{
		float determinant = GetDeterminant();
		if (fabs(determinant) <= EPSILON)
		{
			return Identity();
		}

		float tmp = m.s[1];   // copy the first element
		float invDeterminant = 1.0f / determinant;
		m.s[0] = invDeterminant * m.s[3];
		m.s[1] = -invDeterminant * m.s[1];
		m.s[2] = -invDeterminant * m.s[2];
		m.s[3] = invDeterminant * tmp;

		return *this;
	}

	///////////////////////////////////////////////////////////////////////////////
	// retrieve rotation angle in degree from rotation matrix, R
	// R = | c -s |
	//     | s  c |
	// angle = atan(s / c)
	///////////////////////////////////////////////////////////////////////////////
	float Matrix2::GetAngle() const
	{
		// angle between -pi ~ +pi (-180 ~ +180)
		return RAD2DEG * atan2f(m.s[1], m.s[0]);
	}

	void Matrix2::SetRow(int index, const float row[2])
	{
		m.s[index] = row[0];  m.s[index + 2] = row[1];
	}

	void Matrix2::SetRow(int index, const std::vector<float>& row)
	{
		m.s[index] = row[0];  m.s[index + 2] = row[1];
	}

	void Matrix2::SetRow(int index, const Liar::Vector2D& v)
	{
		m.s[index] = v.x;  m.s[index + 2] = v.y;
	}

	void Matrix2::SetCol(int index, const float col[2])
	{
		m.s[index * 2] = col[0];  m.s[index * 2 + 1] = col[1];
	}

	void Matrix2::SetCol(int index, const std::vector<float>& col)
	{
		m.s[index * 2] = col[0];  m.s[index * 2 + 1] = col[1];
	}

	void Matrix2::SetCol(int index, const Liar::Vector2D& v)
	{
		m.s[index * 2] = v.x;  m.s[index * 2 + 1] = v.y;
	}

	// add rhs
	void Matrix2::Add(const Liar::Matrix2& rhs)
	{
		m.s[0] += rhs[0];
		m.s[1] += rhs[1];
		m.s[2] += rhs[2];
		m.s[3] += rhs[3];
	}

	// subtract rhs
	void Matrix2::Sub(const Liar::Matrix2& rhs)
	{
		m.s[0] -= rhs[0];
		m.s[1] -= rhs[1];
		m.s[2] -= rhs[2];
		m.s[3] -= rhs[3];
	}

	// multiplication: M3 = M1 * M2
	void Matrix2::Mul(const Liar::Matrix2& rhs)
	{
		float t0 = m.s[0], t1 = m.s[1], t2 = m.s[2], t3 = m.s[3];
		m.s[0] = t0 * rhs[0] + t2 * rhs[1];
		m.s[1] = t1 * rhs[0] + t3 * rhs[1];
		m.s[2] = t0 * rhs[2] + t2 * rhs[3];
		m.s[3] = t1 * rhs[2] + t3 * rhs[3];
	}

	// multiplication: M1' = M1 * M2
	void Matrix2::Mul(Liar::Vector2D& rhs)
	{
		float tx = rhs.x, ty = rhs.y;
		rhs.x = m.s[0] * tx + m.s[2] * ty;
		rhs.y = m.s[1] * tx + m.s[3] * ty;
	}

	// multiplication: M1' = M1 * M2
	void Matrix2::Mul(const Liar::Vector2D& rhs, Liar::Vector2D& out)
	{
		out.x = m.s[0] * rhs.x + m.s[2] * rhs.y;
		out.y = m.s[1] * rhs.x + m.s[3] * rhs.y;
	}

	void Matrix2::Mul(float v)
	{
		m.s[0] *= v; m.s[1] *= v;
		m.s[2] *= v; m.s[3] *= v;
	}


	///////////////////////////////////////////////////////////////////////////////
	// transpose 3x3 matrix
	///////////////////////////////////////////////////////////////////////////////
	Matrix3& Matrix3::Transpose()
	{
		std::swap(m.s[1], m.s[3]);
		std::swap(m.s[2], m.s[6]);
		std::swap(m.s[5], m.s[7]);
		return *this;
	}

	///////////////////////////////////////////////////////////////////////////////
	// return determinant of 3x3 matrix
	///////////////////////////////////////////////////////////////////////////////
	float Matrix3::GetDeterminant() const
	{
		return m.s[0] * (m.s[4] * m.s[8] - m.s[5] * m.s[7]) -
				m.s[1] * (m.s[3] * m.s[8] - m.s[5] * m.s[6]) +
				m.s[2] * (m.s[3] * m.s[7] - m.s[4] * m.s[6]);
	}

	///////////////////////////////////////////////////////////////////////////////
	// inverse 3x3 matrix
	// If cannot find inverse (det=0), set identity matrix
	// M^-1 = adj(M) / det(M)
	//        | m4m8-m5m7  m5m6-m3m8  m3m7-m4m6 |
	//      = | m7m2-m8m1  m0m8-m2m6  m6m1-m7m0 | / det(M)
	//        | m1m5-m2m4  m2m3-m0m5  m0m4-m1m3 |
	///////////////////////////////////////////////////////////////////////////////
	Matrix3& Matrix3::Invert()
	{
		float determinant, invDeterminant;
		float tmp0, tmp1, tmp2, tmp3, tmp4, tmp5, tmp6, tmp7, tmp8;

		tmp0 = m.s[4] * m.s[8] - m.s[5] * m.s[7];
		tmp1 = m.s[7] * m.s[2] - m.s[8] * m.s[1];
		tmp2 = m.s[1] * m.s[5] - m.s[2] * m.s[4];
		tmp3 = m.s[5] * m.s[6] - m.s[3] * m.s[8];
		tmp4 = m.s[0] * m.s[8] - m.s[2] * m.s[6];
		tmp5 = m.s[2] * m.s[3] - m.s[0] * m.s[5];
		tmp6 = m.s[3] * m.s[7] - m.s[4] * m.s[6];
		tmp7 = m.s[6] * m.s[1] - m.s[7] * m.s[0];
		tmp8 = m.s[0] * m.s[4] - m.s[1] * m.s[3];

		// check determinant if it is 0
		determinant = m.s[0] * tmp0 + m.s[1] * tmp3 + m.s[2] * tmp6;
		if (fabs(determinant) <= EPSILON)
		{
			return Identity(); // cannot inverse, make it idenety matrix
		}

		// divide by the determinant
		invDeterminant = 1.0f / determinant;
		m.s[0] = invDeterminant * tmp0;
		m.s[1] = invDeterminant * tmp1;
		m.s[2] = invDeterminant * tmp2;
		m.s[3] = invDeterminant * tmp3;
		m.s[4] = invDeterminant * tmp4;
		m.s[5] = invDeterminant * tmp5;
		m.s[6] = invDeterminant * tmp6;
		m.s[7] = invDeterminant * tmp7;
		m.s[8] = invDeterminant * tmp8;

		return *this;
	}



	///////////////////////////////////////////////////////////////////////////////
	// retrieve angles in degree from rotation matrix, M = Rx*Ry*Rz
	// Rx: rotation about X-axis, pitch
	// Ry: rotation about Y-axis, yaw(heading)
	// Rz: rotation about Z-axis, roll
	//    Rx           Ry          Rz
	// |1  0   0| | Cy  0 Sy| |Cz -Sz 0|   | CyCz        -CySz         Sy  |
	// |0 Cx -Sx|*|  0  1  0|*|Sz  Cz 0| = | SxSyCz+CxSz -SxSySz+CxCz -SxCy|
	// |0 Sx  Cx| |-Sy  0 Cy| | 0   0 1|   |-CxSyCz+SxSz  CxSySz+SxCz  CxCy|
	//
	// Pitch: atan(-m[7] / m[8]) = atan(SxCy/CxCy)
	// Yaw  : asin(m[6]) = asin(Sy)
	// Roll : atan(-m[3] / m[0]) = atan(SzCy/CzCy)
	///////////////////////////////////////////////////////////////////////////////
	Vector3D Matrix3::GetAngle() const
	{
		Liar::Vector3D vec;
		GetAngle(vec);
		return vec;
	}

	void Matrix3::GetAngle(Liar::Vector3D& out) const
	{
		float pitch, yaw, roll;         // 3 angles

										// find yaw (around y-axis) first
										// NOTE: asin() returns -90~+90, so correct the angle range -180~+180
										// using z value of forward vector
		yaw = RAD2DEG * asinf(m.s[6]);
		if (m.s[8] < 0)
		{
			if (yaw >= 0) yaw = 180.0f - yaw;
			else         yaw = -180.0f - yaw;
		}

		// find roll (around z-axis) and pitch (around x-axis)
		// if forward vector is (1,0,0) or (-1,0,0), then m[0]=m[4]=m[9]=m[10]=0
		if (m.s[0] > -EPSILON && m.s[0] < EPSILON)
		{
			roll = 0;  //@@ assume roll=0
			pitch = RAD2DEG * atan2f(m.s[1], m.s[4]);
		}
		else
		{
			roll = RAD2DEG * atan2f(-m.s[3], m.s[0]);
			pitch = RAD2DEG * atan2f(-m.s[7], m.s[8]);
		}
		out.x = pitch;
		out.y = yaw;
		out.z = roll;
	}

	void Matrix3::Add(const Liar::Matrix3& rhs)
	{
		m.s[0] += rhs[0];
		m.s[1] += rhs[1];
		m.s[2] += rhs[2];
		m.s[3] += rhs[1];
		m.s[4] += rhs[4];
		m.s[5] += rhs[5];
		m.s[6] += rhs[6];
		m.s[7] += rhs[7];
		m.s[8] += rhs[8];
	}

	void Matrix3::Sub(const Liar::Matrix3& rhs)
	{
		m.s[0] -= rhs[0];
		m.s[1] -= rhs[1];
		m.s[2] -= rhs[2];
		m.s[3] -= rhs[1];
		m.s[4] -= rhs[4];
		m.s[5] -= rhs[5];
		m.s[6] -= rhs[6];
		m.s[7] -= rhs[7];
		m.s[8] -= rhs[8];
	}

	void Matrix3::Mul(const Liar::Matrix3& rhs)
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
	}

	void Matrix3::Mul(Liar::Vector3D& rhs)
	{
		float tx = rhs.x, ty = rhs.y, tz = rhs.z;
		rhs.x = tx*m.s[0] + ty*m.s[1] + tz*m.s[2];
		rhs.y = tx*m.s[3] + ty*m.s[4] + tz*m.s[5];
		rhs.z = tx*m.s[6] + ty*m.s[7] + tz*m.s[8];
	}

	void Matrix3::Mul(const Liar::Vector3D& rhs, Liar::Vector3D& out)
	{
		out.x = rhs.x*m.s[0] + rhs.y*m.s[1] + rhs.z*m.s[2];
		out.y = rhs.x*m.s[3] + rhs.y*m.s[4] + rhs.z*m.s[5];
		out.z = rhs.x*m.s[6] + rhs.y*m.s[7] + rhs.z*m.s[8];
	}

	void Matrix3::Mul(float v)
	{
		m.s[0] *= v; m.s[1] *= v; m.s[2] *= v;
		m.s[3] *= v; m.s[4] *= v; m.s[5] *= v;
		m.s[6] *= v; m.s[7] *= v; m.s[8] *= v;
	}

	//=============================================================================

	void Matrix3::SetRow(int index, const float row[3])
	{
		m.s[index] = row[0];  m.s[index + 3] = row[1];  m.s[index + 6] = row[2];
	}

	void Matrix3::SetRow(int index, const std::vector<float>& row)
	{
		m.s[index] = row[0];  m.s[index + 3] = row[1];  m.s[index + 6] = row[2];
	}

	void Matrix3::SetRow(int index, const Liar::Vector3D& v)
	{
		m.s[index] = v.x;  m.s[index + 3] = v.y;  m.s[index + 6] = v.z;
	}

	void Matrix3::SetCol(int index, const float col[3])
	{
		m.s[index * 3] = col[0];  m.s[index * 3 + 1] = col[1];  m.s[index * 3 + 2] = col[2];
	}

	void Matrix3::SetCol(int index, const std::vector<float>& col)
	{
		m.s[index * 3] = col[0];  m.s[index * 3 + 1] = col[1];  m.s[index * 3 + 2] = col[2];
	}

	void Matrix3::SetCol(int index, const Liar::Vector3D& v)
	{
		m.s[index * 3] = v.x;  m.s[index * 3 + 1] = v.y;  m.s[index * 3 + 2] = v.z;
	}


	///////////////////////////////////////////////////////////////////////////////
	// transpose 4x4 matrix
	///////////////////////////////////////////////////////////////////////////////
	Matrix4& Matrix4::Transpose()
	{
		std::swap(m.s[1], m.s[4]);
		std::swap(m.s[2], m.s[8]);
		std::swap(m.s[3], m.s[12]);
		std::swap(m.s[6], m.s[9]);
		std::swap(m.s[7], m.s[13]);
		std::swap(m.s[11], m.s[14]);

		return *this;
	}

	void Matrix4::SetRow(int index, const float row[4])
	{
		m.s[index] = row[0];  m.s[index + 4] = row[1];  m.s[index + 8] = row[2];  m.s[index + 12] = row[3];
	}

	void Matrix4::SetRow(int index, const std::vector<float>& row)
	{
		m.s[index] = row[0];  m.s[index + 4] = row[1];  m.s[index + 8] = row[2];  m.s[index + 12] = row[3];
	}

	void Matrix4::SetRow(int index, const Liar::Vector4D& v)
	{
		m.s[index] = v.x;  m.s[index + 4] = v.y;  m.s[index + 8] = v.z;  m.s[index + 12] = v.w;
	}

	void Matrix4::SetRow(int index, const Liar::Vector3D& v)
	{
		m.s[index] = v.x;  m.s[index + 4] = v.y;  m.s[index + 8] = v.z;
	}

	void Matrix4::SetCol(int index, const float col[4])
	{
		m.s[index * 4] = col[0];  m.s[index * 4 + 1] = col[1];  m.s[index * 4 + 2] = col[2];  m.s[index * 4 + 3] = col[3];
	}

	void Matrix4::SetCol(int index, const std::vector<float>& col)
	{
		m.s[index * 4] = col[0];  m.s[index * 4 + 1] = col[1];  m.s[index * 4 + 2] = col[2];  m.s[index * 4 + 3] = col[3];
	}

	void Matrix4::SetCol(int index, const Liar::Vector4D& v)
	{
		m.s[index * 4] = v.x;  m.s[index * 4 + 1] = v.y;  m.s[index * 4 + 2] = v.z;  m.s[index * 4 + 3] = v.w;
	}

	void Matrix4::SetCol(int index, const Liar::Vector3D& v)
	{
		m.s[index * 4] = v.x;  m.s[index * 4 + 1] = v.y;  m.s[index * 4 + 2] = v.z;
	}


	///////////////////////////////////////////////////////////////////////////////
	// inverse 4x4 matrix
	///////////////////////////////////////////////////////////////////////////////
	Matrix4& Matrix4::Invert()
	{
		// If the 4th row is [0,0,0,1] then it is affine matrix and
		// it has no projective transformation.
		if (m.s[3] == 0 && m.s[7] == 0 && m.s[11] == 0 && m.s[15] == 1)
			this->InvertAffine();
		else
		{
			this->InvertGeneral();
			/*@@ invertProjective() is not optimized (slower than generic one)
			if(fabs(m[0]*m[5] - m[1]*m[4]) > EPSILON)
			this->invertProjective();   // inverse using matrix partition
			else
			this->invertGeneral();      // generalized inverse
			*/
		}

		return *this;
	}



	///////////////////////////////////////////////////////////////////////////////
	// compute the inverse of 4x4 Euclidean transformation matrix
	//
	// Euclidean transformation is translation, rotation, and reflection.
	// With Euclidean transform, only the position and orientation of the object
	// will be changed. Euclidean transform does not change the shape of an object
	// (no scaling). Length and angle are reserved.
	//
	// Use inverseAffine() if the matrix has scale and shear transformation.
	//
	// M = [ R | T ]
	//     [ --+-- ]    (R denotes 3x3 rotation/reflection matrix)
	//     [ 0 | 1 ]    (T denotes 1x3 translation matrix)
	//
	// y = M*x  ->  y = R*x + T  ->  x = R^-1*(y - T)  ->  x = R^T*y - R^T*T
	// (R is orthogonal,  R^-1 = R^T)
	//
	//  [ R | T ]-1    [ R^T | -R^T * T ]    (R denotes 3x3 rotation matrix)
	//  [ --+-- ]   =  [ ----+--------- ]    (T denotes 1x3 translation)
	//  [ 0 | 1 ]      [  0  |     1    ]    (R^T denotes R-transpose)
	///////////////////////////////////////////////////////////////////////////////
	Matrix4& Matrix4::InvertEuclidean()
	{
		// transpose 3x3 rotation matrix part
		// | R^T | 0 |
		// | ----+-- |
		// |  0  | 1 |
		float tmp;
		tmp = m.s[1];  m.s[1] = m.s[4];  m.s[4] = tmp;
		tmp = m.s[2];  m.s[2] = m.s[8];  m.s[8] = tmp;
		tmp = m.s[6];  m.s[6] = m.s[9];  m.s[9] = tmp;

		// compute translation part -R^T * T
		// | 0 | -R^T x |
		// | --+------- |
		// | 0 |   0    |
		float x = m.s[12];
		float y = m.s[13];
		float z = m.s[14];
		m.s[12] = -(m.s[0] * x + m.s[4] * y + m.s[8] * z);
		m.s[13] = -(m.s[1] * x + m.s[5] * y + m.s[9] * z);
		m.s[14] = -(m.s[2] * x + m.s[6] * y + m.s[10] * z);

		// last row should be unchanged (0,0,0,1)

		return *this;
	}



	///////////////////////////////////////////////////////////////////////////////
	// compute the inverse of a 4x4 affine transformation matrix
	//
	// Affine transformations are generalizations of Euclidean transformations.
	// Affine transformation includes translation, rotation, reflection, scaling,
	// and shearing. Length and angle are NOT preserved.
	// M = [ R | T ]
	//     [ --+-- ]    (R denotes 3x3 rotation/scale/shear matrix)
	//     [ 0 | 1 ]    (T denotes 1x3 translation matrix)
	//
	// y = M*x  ->  y = R*x + T  ->  x = R^-1*(y - T)  ->  x = R^-1*y - R^-1*T
	//
	//  [ R | T ]-1   [ R^-1 | -R^-1 * T ]
	//  [ --+-- ]   = [ -----+---------- ]
	//  [ 0 | 1 ]     [  0   +     1     ]
	///////////////////////////////////////////////////////////////////////////////
	Matrix4& Matrix4::InvertAffine()
	{
		// R^-1
		Matrix3 r(m.s[0], m.s[1], m.s[2], m.s[4], m.s[5], m.s[6], m.s[8], m.s[9], m.s[10]);
		r.Invert();
		m.s[0] = r[0];  m.s[1] = r[1];  m.s[2] = r[2];
		m.s[4] = r[3];  m.s[5] = r[4];  m.s[6] = r[5];
		m.s[8] = r[6];  m.s[9] = r[7];  m.s[10] = r[8];

		// -R^-1 * T
		float x = m.s[12];
		float y = m.s[13];
		float z = m.s[14];
		m.s[12] = -(r[0] * x + r[3] * y + r[6] * z);
		m.s[13] = -(r[1] * x + r[4] * y + r[7] * z);
		m.s[14] = -(r[2] * x + r[5] * y + r[8] * z);

		// last row should be unchanged (0,0,0,1)
		//m[3] = m[7] = m[11] = 0.0f;
		//m[15] = 1.0f;

		return *this;
	}



	///////////////////////////////////////////////////////////////////////////////
	// inverse matrix using matrix partitioning (blockwise inverse)
	// It devides a 4x4 matrix into 4 of 2x2 matrices. It works in case of where
	// det(A) != 0. If not, use the generic inverse method
	// inverse formula.
	// M = [ A | B ]    A, B, C, D are 2x2 matrix blocks
	//     [ --+-- ]    det(M) = |A| * |D - ((C * A^-1) * B)|
	//     [ C | D ]
	//
	// M^-1 = [ A' | B' ]   A' = A^-1 - (A^-1 * B) * C'
	//        [ ---+--- ]   B' = (A^-1 * B) * -D'
	//        [ C' | D' ]   C' = -D' * (C * A^-1)
	//                      D' = (D - ((C * A^-1) * B))^-1
	//
	// NOTE: I wrap with () if it it used more than once.
	//       The matrix is invertable even if det(A)=0, so must check det(A) before
	//       calling this function, and use invertGeneric() instead.
	///////////////////////////////////////////////////////////////////////////////
	Matrix4& Matrix4::InvertProjective()
	{
		// partition
		Matrix2 a(m.s[0], m.s[1], m.s[4], m.s[5]);
		Matrix2 b(m.s[8], m.s[9], m.s[12], m.s[13]);
		Matrix2 c(m.s[2], m.s[3], m.s[6], m.s[7]);
		Matrix2 d(m.s[10], m.s[11], m.s[14], m.s[15]);

		// pre-compute repeated parts
		a.Invert();             // A^-1
		Matrix2 ab = a * b;     // A^-1 * B
		Matrix2 ca = c * a;     // C * A^-1
		Matrix2 cab = ca * b;   // C * A^-1 * B
		Matrix2 dcab = d - cab; // D - C * A^-1 * B

		// check determinant if |D - C * A^-1 * B| = 0
		//NOTE: this function assumes det(A) is already checked. if |A|=0 then,
		//      cannot use this function.
		float determinant = dcab[0] * dcab[3] - dcab[1] * dcab[2];
		if (fabs(determinant) <= EPSILON)
		{
			return Identity();
		}

		// compute D' and -D'
		Matrix2 d1 = dcab;      //  (D - C * A^-1 * B)
		d1.Invert();            //  (D - C * A^-1 * B)^-1
		Matrix2 d2 = -d1;       // -(D - C * A^-1 * B)^-1

		// compute C'
		Matrix2 c1 = d2 * ca;   // -D' * (C * A^-1)

		// compute B'
		Matrix2 b1 = ab * d2;   // (A^-1 * B) * -D'

		// compute A'
		Matrix2 a1 = a - (ab * c1); // A^-1 - (A^-1 * B) * C'

									// assemble inverse matrix
		m.s[0] = a1[0];  m.s[4] = a1[2]; /*|*/ m.s[8] = b1[0];  m.s[12] = b1[2];
		m.s[1] = a1[1];  m.s[5] = a1[3]; /*|*/ m.s[9] = b1[1];  m.s[13] = b1[3];
		/*-----------------------------+-----------------------------*/
		m.s[2] = c1[0];  m.s[6] = c1[2]; /*|*/ m.s[10] = d1[0];  m.s[14] = d1[2];
		m.s[3] = c1[1];  m.s[7] = c1[3]; /*|*/ m.s[11] = d1[1];  m.s[15] = d1[3];

		return *this;
	}



	///////////////////////////////////////////////////////////////////////////////
	// compute the inverse of a general 4x4 matrix using Cramer's Rule
	// If cannot find inverse, return indentity matrix
	// M^-1 = adj(M) / det(M)
	///////////////////////////////////////////////////////////////////////////////
	Matrix4& Matrix4::InvertGeneral()
	{
		// get cofactors of minor matrices
		float cofactor0 = GetCofactor(m.s[5], m.s[6], m.s[7], m.s[9], m.s[10], m.s[11], m.s[13], m.s[14], m.s[15]);
		float cofactor1 = GetCofactor(m.s[4], m.s[6], m.s[7], m.s[8], m.s[10], m.s[11], m.s[12], m.s[14], m.s[15]);
		float cofactor2 = GetCofactor(m.s[4], m.s[5], m.s[7], m.s[8], m.s[9], m.s[11], m.s[12], m.s[13], m.s[15]);
		float cofactor3 = GetCofactor(m.s[4], m.s[5], m.s[6], m.s[8], m.s[9], m.s[10], m.s[12], m.s[13], m.s[14]);

		// get determinant
		float determinant = m.s[0] * cofactor0 - m.s[1] * cofactor1 + m.s[2] * cofactor2 - m.s[3] * cofactor3;
		if (fabs(determinant) <= EPSILON)
		{
			return Identity();
		}

		// get rest of cofactors for adj(M)
		float cofactor4 = GetCofactor(m.s[1], m.s[2], m.s[3], m.s[9], m.s[10], m.s[11], m.s[13], m.s[14], m.s[15]);
		float cofactor5 = GetCofactor(m.s[0], m.s[2], m.s[3], m.s[8], m.s[10], m.s[11], m.s[12], m.s[14], m.s[15]);
		float cofactor6 = GetCofactor(m.s[0], m.s[1], m.s[3], m.s[8], m.s[9], m.s[11], m.s[12], m.s[13], m.s[15]);
		float cofactor7 = GetCofactor(m.s[0], m.s[1], m.s[2], m.s[8], m.s[9], m.s[10], m.s[12], m.s[13], m.s[14]);

		float cofactor8 = GetCofactor(m.s[1], m.s[2], m.s[3], m.s[5], m.s[6], m.s[7], m.s[13], m.s[14], m.s[15]);
		float cofactor9 = GetCofactor(m.s[0], m.s[2], m.s[3], m.s[4], m.s[6], m.s[7], m.s[12], m.s[14], m.s[15]);
		float cofactor10 = GetCofactor(m.s[0], m.s[1], m.s[3], m.s[4], m.s[5], m.s[7], m.s[12], m.s[13], m.s[15]);
		float cofactor11 = GetCofactor(m.s[0], m.s[1], m.s[2], m.s[4], m.s[5], m.s[6], m.s[12], m.s[13], m.s[14]);

		float cofactor12 = GetCofactor(m.s[1], m.s[2], m.s[3], m.s[5], m.s[6], m.s[7], m.s[9], m.s[10], m.s[11]);
		float cofactor13 = GetCofactor(m.s[0], m.s[2], m.s[3], m.s[4], m.s[6], m.s[7], m.s[8], m.s[10], m.s[11]);
		float cofactor14 = GetCofactor(m.s[0], m.s[1], m.s[3], m.s[4], m.s[5], m.s[7], m.s[8], m.s[9], m.s[11]);
		float cofactor15 = GetCofactor(m.s[0], m.s[1], m.s[2], m.s[4], m.s[5], m.s[6], m.s[8], m.s[9], m.s[10]);

		// build inverse matrix = adj(M) / det(M)
		// adjugate of M is the transpose of the cofactor matrix of M
		float invDeterminant = 1.0f / determinant;
		m.s[0] = invDeterminant * cofactor0;
		m.s[1] = -invDeterminant * cofactor4;
		m.s[2] = invDeterminant * cofactor8;
		m.s[3] = -invDeterminant * cofactor12;

		m.s[4] = -invDeterminant * cofactor1;
		m.s[5] = invDeterminant * cofactor5;
		m.s[6] = -invDeterminant * cofactor9;
		m.s[7] = invDeterminant * cofactor13;

		m.s[8] = invDeterminant * cofactor2;
		m.s[9] = -invDeterminant * cofactor6;
		m.s[10] = invDeterminant * cofactor10;
		m.s[11] = -invDeterminant * cofactor14;

		m.s[12] = -invDeterminant * cofactor3;
		m.s[13] = invDeterminant * cofactor7;
		m.s[14] = -invDeterminant * cofactor11;
		m.s[15] = invDeterminant * cofactor15;

		return *this;
	}



	///////////////////////////////////////////////////////////////////////////////
	// return determinant of 4x4 matrix
	///////////////////////////////////////////////////////////////////////////////
	float Matrix4::GetDeterminant() const
	{
		return  m.s[0] * GetCofactor(m.s[5], m.s[6], m.s[7], m.s[9], m.s[10], m.s[11], m.s[13], m.s[14], m.s[15]) -
				m.s[1] * GetCofactor(m.s[4], m.s[6], m.s[7], m.s[8], m.s[10], m.s[11], m.s[12], m.s[14], m.s[15]) +
				m.s[2] * GetCofactor(m.s[4], m.s[5], m.s[7], m.s[8], m.s[9], m.s[11], m.s[12], m.s[13], m.s[15]) -
				m.s[3] * GetCofactor(m.s[4], m.s[5], m.s[6], m.s[8], m.s[9], m.s[10], m.s[12], m.s[13], m.s[14]);
	}



	///////////////////////////////////////////////////////////////////////////////
	// compute cofactor of 3x3 minor matrix without sign
	// input params are 9 elements of the minor matrix
	// NOTE: The caller must know its sign.
	///////////////////////////////////////////////////////////////////////////////
	float Matrix4::GetCofactor(float m0, float m1, float m2,
					float m3, float m4, float m5,
					float m6, float m7, float m8) const
	{
		return m0 * (m4 * m8 - m5 * m7) -
			m1 * (m3 * m8 - m5 * m6) +
			m2 * (m3 * m7 - m4 * m6);
	}



	///////////////////////////////////////////////////////////////////////////////
	// translate this matrix by (x, y, z)
	///////////////////////////////////////////////////////////////////////////////
	Matrix4& Matrix4::Translate(const Vector3D& v)
	{
		return Translate(v.x, v.y, v.z);
	}

	Matrix4& Matrix4::Translate(float x, float y, float z)
	{
		m.s[0] += m.s[3] * x;   m.s[4] += m.s[7] * x;   m.s[8] += m.s[11] * x;   m.s[12] += m.s[15] * x;
		m.s[1] += m.s[3] * y;   m.s[5] += m.s[7] * y;   m.s[9] += m.s[11] * y;   m.s[13] += m.s[15] * y;
		m.s[2] += m.s[3] * z;   m.s[6] += m.s[7] * z;   m.s[10] += m.s[11] * z;   m.s[14] += m.s[15] * z;

		return *this;
	}



	///////////////////////////////////////////////////////////////////////////////
	// uniform scale
	///////////////////////////////////////////////////////////////////////////////
	Matrix4& Matrix4::Scale(float s)
	{
		return Scale(s, s, s);
	}

	Matrix4& Matrix4::Scale(float x, float y, float z)
	{
		m.s[0] *= x;   m.s[4] *= x;   m.s[8] *= x;   m.s[12] *= x;
		m.s[1] *= y;   m.s[5] *= y;   m.s[9] *= y;   m.s[13] *= y;
		m.s[2] *= z;   m.s[6] *= z;   m.s[10] *= z;   m.s[14] *= z;
		return *this;
	}



	///////////////////////////////////////////////////////////////////////////////
	// build a rotation matrix with given angle(degree) and rotation axis, then
	// multiply it with this object
	///////////////////////////////////////////////////////////////////////////////
	Matrix4& Matrix4::Rotate(float angle, const Vector3D& axis)
	{
		return Rotate(angle, axis.x, axis.y, axis.z);
	}

	Matrix4& Matrix4::Rotate(float angle, float x, float y, float z)
	{
		float c = cosf(angle * DEG2RAD);    // cosine
		float s = sinf(angle * DEG2RAD);    // sine
		float c1 = 1.0f - c;                // 1 - c
		float   tm0 = m.s[0], tm4 = m.s[4], tm8 = m.s[8], tm12 = m.s[12],
				tm1 = m.s[1], tm5 = m.s[5], tm9 = m.s[9], tm13 = m.s[13],
				tm2 = m.s[2], tm6 = m.s[6], tm10 = m.s[10], tm14 = m.s[14];

		// build rotation matrix
		float r0 = x * x * c1 + c;
		float r1 = x * y * c1 + z * s;
		float r2 = x * z * c1 - y * s;
		float r4 = x * y * c1 - z * s;
		float r5 = y * y * c1 + c;
		float r6 = y * z * c1 + x * s;
		float r8 = x * z * c1 + y * s;
		float r9 = y * z * c1 - x * s;
		float r10 = z * z * c1 + c;

		// multiply rotation matrix
		m.s[0] = r0 * tm0 + r4 * tm1 + r8 * tm2;
		m.s[1] = r1 * tm0 + r5 * tm1 + r9 * tm2;
		m.s[2] = r2 * tm0 + r6 * tm1 + r10* tm2;
		m.s[4] = r0 * tm4 + r4 * tm5 + r8 * tm6;
		m.s[5] = r1 * tm4 + r5 * tm5 + r9 * tm6;
		m.s[6] = r2 * tm4 + r6 * tm5 + r10* tm6;
		m.s[8] = r0 * tm8 + r4 * tm9 + r8 * tm10;
		m.s[9] = r1 * tm8 + r5 * tm9 + r9 * tm10;
		m.s[10] = r2 * tm8 + r6 * tm9 + r10* tm10;
		m.s[12] = r0 * tm12 + r4 * tm13 + r8 * tm14;
		m.s[13] = r1 * tm12 + r5 * tm13 + r9 * tm14;
		m.s[14] = r2 * tm12 + r6 * tm13 + r10* tm14;

		return *this;
	}

	Matrix4& Matrix4::Rotate(float angleX, float angleY, float angleZ)
	{
		RotateZ(angleZ);
		RotateY(angleY);
		RotateX(angleX);

		return *this;
	}

	Matrix4& Matrix4::RotateX(float angle)
	{
		float c = cosf(angle * DEG2RAD);
		float s = sinf(angle * DEG2RAD);
		float	tm1 = m.s[1], tm2 = m.s[2],
				tm5 = m.s[5], tm6 = m.s[6],
				tm9 = m.s[9], tm10 = m.s[10],
				tm13 = m.s[13], tm14 = m.s[14];

		m.s[1] = tm1 * c + tm2 *-s;
		m.s[2] = tm1 * s + tm2 * c;
		m.s[5] = tm5 * c + tm6 *-s;
		m.s[6] = tm5 * s + tm6 * c;
		m.s[9] = tm9 * c + tm10*-s;
		m.s[10] = tm9 * s + tm10* c;
		m.s[13] = tm13* c + tm14*-s;
		m.s[14] = tm13* s + tm14* c;

		return *this;
	}

	Matrix4& Matrix4::RotateY(float angle)
	{
		float c = cosf(angle * DEG2RAD);
		float s = sinf(angle * DEG2RAD);
		float	tm0 = m.s[0], tm2 = m.s[2],
				tm4 = m.s[4], tm6 = m.s[6],
				tm8 = m.s[8], tm10 = m.s[10],
				tm12 = m.s[12], tm14 = m.s[14];

		m.s[0] = tm0 * c + tm2 * s;
		m.s[2] = tm0 *-s + tm2 * c;
		m.s[4] = tm4 * c + tm6 * s;
		m.s[6] = tm4 *-s + tm6 * c;
		m.s[8] = tm8 * c + tm10* s;
		m.s[10] = tm8 *-s + tm10* c;
		m.s[12] = tm12* c + tm14* s;
		m.s[14] = tm12*-s + tm14* c;

		return *this;
	}

	Matrix4& Matrix4::RotateZ(float angle)
	{
		float c = cosf(angle * DEG2RAD);
		float s = sinf(angle * DEG2RAD);
		float	tm0 = m.s[0], tm1 = m.s[1],
				tm4 = m.s[4], tm5 = m.s[5],
				tm8 = m.s[8], tm9 = m.s[9],
				tm12 = m.s[12], tm13 = m.s[13];

		m.s[0] = tm0 * c + tm1 *-s;
		m.s[1] = tm0 * s + tm1 * c;
		m.s[4] = tm4 * c + tm5 *-s;
		m.s[5] = tm4 * s + tm5 * c;
		m.s[8] = tm8 * c + tm9 *-s;
		m.s[9] = tm8 * s + tm9 * c;
		m.s[12] = tm12* c + tm13*-s;
		m.s[13] = tm12* s + tm13* c;

		return *this;
	}



	///////////////////////////////////////////////////////////////////////////////
	// rotate matrix to face along the target direction
	// NOTE: This function will clear the previous rotation and scale info and
	// rebuild the matrix with the target vector. But it will keep the previous
	// translation values.
	// NOTE: It is for rotating object to look at the target, NOT for camera
	///////////////////////////////////////////////////////////////////////////////
	Matrix4& Matrix4::LookAt(const Vector3D& target)
	{
		return LookAt(target.x, target.y, target.z);
	}

	Matrix4& Matrix4::LookAt(const Vector3D& target, const Vector3D& upVec)
	{
		return LookAt(target.x, target.y, target.z, upVec.x, upVec.y, upVec.z);
	}

	Matrix4& Matrix4::LookAt(float tx, float ty, float tz)
	{
		//// compute forward vector and normalize
		//Vector3D position = Vector3D(m12, m13, m14);
		//Vector3D forward = target - position;
		//forward.Normalize();
		//Vector3D up;             // up vector of object
		//Vector3D left;           // left vector of object

		//						// compute temporal up vector
		//						// if forward vector is near Y-axis, use up vector (0,0,-1) or (0,0,1)
		//if (fabs(forward.x) < EPSILON && fabs(forward.z) < EPSILON)
		//{
		//	// forward vector is pointing +Y axis
		//	if (forward.y > 0)
		//		up.Set(0, 0, -1);
		//	// forward vector is pointing -Y axis
		//	else
		//		up.Set(0, 0, 1);
		//}
		//else
		//{
		//	// assume up vector is +Y axis
		//	up.Set(0, 1, 0);
		//}

		//// compute left vector
		//left = up.Cross(forward);
		//left.Normalize();

		//// re-compute up vector
		//up = forward.Cross(left);
		////up.normalize();

		//// NOTE: overwrite rotation and scale info of the current matrix
		//this->SetCol(0, left);
		//this->SetCol(1, up);
		//this->SetCol(2, forward);

		// compute forward vector and normalize
		Liar::Vector3D* forward = new Liar::Vector3D(m.s[12], m.s[13], m.s[14]);
		forward->Sub(tx, ty, tz);
		forward->Normalize();

		Liar::Vector3D* tmp = new Liar::Vector3D();

		// compute temporal up vector
		// if forward vector is near Y-axis, use up vector (0,0,-1) or (0,0,1)
		if (fabs(forward->x) < EPSILON && fabs(forward->z) < EPSILON)
		{
			// forward vector is pointing +Y axis
			if (forward->y > 0)
				tmp->Set(0.0, 0.0, -1.0f);
			// forward vector is pointing -Y axis
			else
				tmp->Set(0.0f, 0.0f, 1.0f);
		}
		else
		{
			// assume up vector is +Y axis
			tmp->Set(0.0f, 1.0f, 0.0f);
		}

		this->SetCol(2, *forward);

		// compute left vector
		tmp->CrossC(*forward);
		tmp->Normalize();

		// re-compute up vector
		forward->CrossC(*tmp);
		forward->Normalize();
		//up.normalize();

		// NOTE: overwrite rotation and scale info of the current matrix
		this->SetCol(0, *tmp);
		this->SetCol(1, *forward);

		delete tmp;
		delete forward;

		return *this;
	}

	Matrix4& Matrix4::LookAt(float tx, float ty, float tz, float ux, float uy, float uz)
	{
		//// compute forward vector and normalize
		//Vector3D position = Vector3D(m12, m13, m14);
		//Vector3D forward = target - position;
		//forward.Normalize();

		//// compute left vector
		//Vector3D left = upVec.Cross(forward);
		//left.Normalize();

		//// compute orthonormal up vector
		//Vector3D up = forward.Cross(left);
		//up.Normalize();

		//// NOTE: overwrite rotation and scale info of the current matrix
		//this->SetCol(0, left);
		//this->SetCol(1, up);
		//this->SetCol(2, forward);

		// compute forward vector and normalize
		Liar::Vector3D* forward = new Liar::Vector3D(m.s[12], m.s[13], m.s[14]);
		forward->Sub(tx, ty, tz);
		forward->Normalize();

		// compute left vector
		Liar::Vector3D* left = new Liar::Vector3D(ux, uy, uz);
		left->CrossC(*forward);
		left->Normalize();

		this->SetCol(0, *left);
		this->SetCol(2, *forward);

		// compute orthonormal up vector
		forward->CrossC(*left);
		forward->Normalize();

		// NOTE: overwrite rotation and scale info of the current matrix
		this->SetCol(1, *forward);

		delete forward;
		delete left;

		return *this;
	}



	///////////////////////////////////////////////////////////////////////////////
	// return 3x3 matrix containing rotation only
	///////////////////////////////////////////////////////////////////////////////
	Matrix3 Matrix4::GetRotationMatrix() const
	{
		Matrix3 mat(m.s[0], m.s[1], m.s[2],
			m.s[4], m.s[5], m.s[6],
			m.s[8], m.s[9], m.s[10]);
		return mat;
	}

	void Matrix4::GetRotationMatrix(Liar::Matrix3& v) const
	{
		v.Set(m.s[0], m.s[1], m.s[2], m.s[4], m.s[5], m.s[6], m.s[8], m.s[9], m.s[10]);
	}


	/*@@
	///////////////////////////////////////////////////////////////////////////////
	// skew with a given angle on the axis
	///////////////////////////////////////////////////////////////////////////////
	Matrix4& Matrix4::skew(float angle, const Vector3& axis)
	{
	float t = tanf(angle * DEG2RAD);    // tangent
	m[0] += m[1] * t;
	m[4] += m[5] * t;
	m[8] += m[9] * t;
	m[12]+= m[13]* t;
	return *this;
	}
	*/



	///////////////////////////////////////////////////////////////////////////////
	// retrieve angles in degree from rotation matrix, M = Rx*Ry*Rz
	// Rx: rotation about X-axis, pitch
	// Ry: rotation about Y-axis, yaw(heading)
	// Rz: rotation about Z-axis, roll
	//    Rx           Ry          Rz
	// |1  0   0| | Cy  0 Sy| |Cz -Sz 0|   | CyCz        -CySz         Sy  |
	// |0 Cx -Sx|*|  0  1  0|*|Sz  Cz 0| = | SxSyCz+CxSz -SxSySz+CxCz -SxCy|
	// |0 Sx  Cx| |-Sy  0 Cy| | 0   0 1|   |-CxSyCz+SxSz  CxSySz+SxCz  CxCy|
	//
	// Pitch: atan(-m[9] / m[10]) = atan(SxCy/CxCy)
	// Yaw  : asin(m[8]) = asin(Sy)
	// Roll : atan(-m[4] / m[0]) = atan(SzCy/CzCy)
	///////////////////////////////////////////////////////////////////////////////
	Vector3D Matrix4::GetAngle() const
	{
		float pitch = 0.0f, yaw = 0.0f, roll = 0.0f;         // 3 angles
		GetAngle(pitch, yaw, roll);

		return Vector3D(pitch, yaw, roll);
	}

	void Matrix4::GetAngle(Liar::Vector3D& out) const
	{
		float pitch = 0.0f, yaw = 0.0f, roll = 0.0f;         // 3 angles
		GetAngle(pitch, yaw, roll);
		out.Set(pitch, yaw, roll);
	}

	void Matrix4::GetAngle(float& pitch, float& yaw, float& roll) const
	{
		// find yaw (around y-axis) first
		// NOTE: asin() returns -90~+90, so correct the angle range -180~+180
		// using z value of forward vector

		yaw = RAD2DEG * asinf(m.s[8]);
		if (m.s[10] < 0)
		{
			if (yaw >= 0) yaw = 180.0f - yaw;
			else         yaw = -180.0f - yaw;
		}

		// find roll (around z-axis) and pitch (around x-axis)
		// if forward vector is (1,0,0) or (-1,0,0), then m[0]=m[4]=m[9]=m[10]=0
		if (m.s[0] > -EPSILON && m.s[0] < EPSILON)
		{
			roll = 0;  //@@ assume roll=0
			pitch = RAD2DEG * atan2f(m.s[1], m.s[5]);
		}
		else
		{
			roll = RAD2DEG * atan2f(-m.s[4], m.s[0]);
			pitch = RAD2DEG * atan2f(-m.s[9], m.s[0]);
		}
	}

}