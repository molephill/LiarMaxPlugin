#pragma once

#include <cmath>
#include <iostream>

#include <PluginDefine.h>

namespace Liar
{
	///////////////////////////////////////////////////////////////////////////////
	// 2D vector
	///////////////////////////////////////////////////////////////////////////////
	struct Vector2D
	{
		float x;
		float y;

		// ctors
		Vector2D() : x(0.0f), y(0.0f) {};
		Vector2D(float x, float y) : x(x), y(y) {};

		// utils functions
		void        Set(float x, float y);
		float       Length() const;                         //
		float       Distance(const Vector2D& vec) const;     // distance between two vectors
		Vector2D&   Normalize();                            //
		float       Dot(const Vector2D& vec) const;          // dot product
		bool        Equal(const Vector2D& vec, float e) const; // compare with epsilon
		Vector2D	Clone() const;

		void		Negative();

		void		Add(const Vector2D&);
		void		Add(float);
		void		Add(float, float);

		void		Sub(const Vector2D&);
		void		Sub(float);
		void		Sub(float, float);

		void		Mul(const Vector2D&);
		void		Mul(float);
		void		Mul(float, float);

		void		Div(const Vector2D&);
		void		Div(float);
		void		Div(float, float);
															  // operators
		Vector2D     operator-() const;                      // unary operator (negate)
		Vector2D     operator+(const Vector2D& rhs) const;    // add rhs
		Vector2D     operator-(const Vector2D& rhs) const;    // subtract rhs
		Vector2D&    operator+=(const Vector2D& rhs);         // add rhs and update this object
		Vector2D&    operator-=(const Vector2D& rhs);         // subtract rhs and update this object
		Vector2D     operator*(const float scale) const;     // scale
		Vector2D     operator*(const Vector2D& rhs) const;    // multiply each element
		Vector2D&    operator*=(const float scale);          // scale and update this object
		Vector2D&    operator*=(const Vector2D& rhs);         // multiply each element and update this object
		Vector2D     operator/(const float scale) const;     // inverse scale
		Vector2D     operator/(const Vector2D&) const;       // inverse scale
		Vector2D&    operator/=(const float scale);          // scale and update this object
		void		 operator=(const Vector2D&);
		
		bool        operator==(const Vector2D& rhs) const;   // exact compare, no epsilon
		bool        operator!=(const Vector2D& rhs) const;   // exact compare, no epsilon
		bool        operator<(const Vector2D& rhs) const;    // comparison for sort
		float       operator[](int index) const;            // subscript operator v[0], v[1]
		float&      operator[](int index);                  // subscript operator v[0], v[1]

		friend Vector2D operator*(const float a, const Vector2D vec);
		friend std::ostream& operator<<(std::ostream& os, const Vector2D& vec);
	};



	///////////////////////////////////////////////////////////////////////////////
	// 3D vector
	///////////////////////////////////////////////////////////////////////////////
	struct Vector3D
	{
		float x;
		float y;
		float z;

		// ctors
		Vector3D() : x(0.0f), y(0.0f), z(0.0f) {};
		Vector3D(float x, float y, float z) : x(x), y(y), z(z) {};

		// utils functions
		void        Set(float x, float y, float z);
		float       Length() const;                         //
		float       Distance(const Vector3D& vec) const;     // distance between two vectors
		float       Angle(const Vector3D& vec) const;        // angle between two vectors
		Vector3D&   Normalize();                            //
		float       Dot(const Vector3D& vec) const;          // dot product
		Vector3D    Cross(const Vector3D& vec) const;        // cross product
		void		Cross(float, float, float);		// cross product
		void		CrossC(const Liar::Vector3D& vec);		// cross product
		bool        Equal(const Vector3D& vec, float e) const; // compare with epsilon
		Vector3D	Clone() const;

		void		Negative();

		void		Add(const Vector3D&);
		void		Add(float);
		void		Add(float, const Vector2D&);
		void		Add(const Vector2D&, float);
		void		Add(float, float, float);

		void		Sub(const Vector3D&);
		void		Sub(float);
		void		Sub(float, const Vector2D&);
		void		Sub(const Vector2D&, float);
		void		Sub(float, float, float);

		void		Mul(const Vector3D&);
		void		Mul(float);
		void		Mul(float, float, float);

		void		Div(const Vector3D&);
		void		Div(float);
		void		Div(float, float, float);

															  // operators
		Vector3D     operator-() const;                      // unary operator (negate)
		Vector3D     operator+(const Vector3D& rhs) const;    // add rhs
		Vector3D     operator-(const Vector3D& rhs) const;    // subtract rhs
		Vector3D&    operator+=(const Vector3D& rhs);         // add rhs and update this object
		Vector3D&    operator-=(const Vector3D& rhs);         // subtract rhs and update this object
		Vector3D     operator*(const float scale) const;     // scale
		Vector3D     operator*(const Vector3D& rhs) const;    // multiplay each element
		Vector3D&    operator*=(const float scale);          // scale and update this object
		Vector3D&    operator*=(const Vector3D& rhs);         // product each element and update this object
		Vector3D     operator/(const float scale) const;     // inverse scale
		Vector3D     operator/(const Vector3D&) const;     // inverse scale
		Vector3D&    operator/=(const float scale);          // scale and update this object
		void		 operator=(const Vector3D&);

		bool        operator==(const Vector3D& rhs) const;   // exact compare, no epsilon
		bool        operator!=(const Vector3D& rhs) const;   // exact compare, no epsilon
		bool        operator<(const Vector3D& rhs) const;    // comparison for sort
		float       operator[](int index) const;            // subscript operator v[0], v[1]
		float&      operator[](int index);                  // subscript operator v[0], v[1]

		friend Vector3D operator*(const float a, const Vector3D vec);
		friend std::ostream& operator<<(std::ostream& os, const Vector3D& vec);
	};



	///////////////////////////////////////////////////////////////////////////////
	// 4D vector
	///////////////////////////////////////////////////////////////////////////////
	struct Vector4D
	{
		float x;
		float y;
		float z;
		float w;

		// ctors
		Vector4D() : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {};
		Vector4D(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {};

		// utils functions
		void        Set(float x, float y, float z, float w);
		float       Length() const;                         //
		float       Distance(const Vector4D& vec) const;     // distance between two vectors
		Vector4D&   Normalize();                            //
		float       Dot(const Vector4D& vec) const;          // dot product
		bool        Equal(const Vector4D& vec, float e) const; // compare with epsilon
		Vector4D	Clone() const;

		void		Negative();

		void		Add(const Vector4D&);
		void		Add(float);
		void		Add(float, const Vector3D&);
		void		Add(const Vector3D&, float);
		void		Add(float, float, const Vector2D&);
		void		Add(float, const Vector2D&, float);
		void		Add(const Vector2D&, float, float);
		void		Add(float, float, float, float);

		void		Sub(const Vector4D&);
		void		Sub(float);
		void		Sub(float, const Vector3D&);
		void		Sub(const Vector3D&, float);
		void		Sub(float, float, const Vector2D&);
		void		Sub(float, const Vector2D&, float);
		void		Sub(const Vector2D&, float, float);
		void		Sub(float, float, float, float);

		void		Mul(const Vector4D&);
		void		Mul(float);
		void		Mul(float, float, float, float);

		void		Div(const Vector4D&);
		void		Div(float);
		void		Div(float, float, float, float);

															  // operators
		Vector4D     operator-() const;                      // unary operator (negate)
		Vector4D     operator+(const Vector4D& rhs) const;    // add rhs
		Vector4D     operator-(const Vector4D& rhs) const;    // subtract rhs
		Vector4D&    operator+=(const Vector4D& rhs);         // add rhs and update this object
		Vector4D&    operator-=(const Vector4D& rhs);         // subtract rhs and update this object
		Vector4D     operator*(const float scale) const;     // scale
		Vector4D     operator*(const Vector4D& rhs) const;    // multiply each element
		Vector4D&    operator*=(const float scale);          // scale and update this object
		Vector4D&    operator*=(const Vector4D& rhs);         // multiply each element and update this object
		Vector4D     operator/(const float scale) const;     // inverse scale
		Vector4D     operator/(const Vector4D&) const;     // inverse scale
		Vector4D&    operator/=(const float scale);          // scale and update this object
		void		 operator=(const Vector4D&);

		bool        operator==(const Vector4D& rhs) const;   // exact compare, no epsilon
		bool        operator!=(const Vector4D& rhs) const;   // exact compare, no epsilon
		bool        operator<(const Vector4D& rhs) const;    // comparison for sort
		float       operator[](int index) const;            // subscript operator v[0], v[1]
		float&      operator[](int index);                  // subscript operator v[0], v[1]

		friend Vector4D operator*(const float a, const Vector4D vec);
		friend std::ostream& operator<<(std::ostream& os, const Vector4D& vec);
	};



	// fast math routines from Doom3 SDK
	inline float InvSqrt(float x)
	{
		float xhalf = 0.5f * x;
		int i = *(int*)&x;          // get bits for floating value
		i = 0x5f3759df - (i >> 1);    // gives initial guess
		x = *(float*)&i;            // convert bits back to float
		x = x * (1.5f - xhalf*x*x); // Newton step
		return x;
	}



	///////////////////////////////////////////////////////////////////////////////
	// inline functions for Vector2
	///////////////////////////////////////////////////////////////////////////////
	inline Vector2D Vector2D::operator-() const {
		return Vector2D(-x, -y);
	}

	inline Vector2D Vector2D::operator+(const Vector2D& rhs) const {
		return Vector2D(x + rhs.x, y + rhs.y);
	}

	inline Vector2D Vector2D::operator-(const Vector2D& rhs) const {
		return Vector2D(x - rhs.x, y - rhs.y);
	}

	inline Vector2D& Vector2D::operator+=(const Vector2D& rhs) {
		x += rhs.x; y += rhs.y; return *this;
	}

	inline Vector2D& Vector2D::operator-=(const Vector2D& rhs) {
		x -= rhs.x; y -= rhs.y; return *this;
	}

	inline Vector2D Vector2D::operator*(const float a) const {
		return Vector2D(x*a, y*a);
	}

	inline Vector2D Vector2D::operator*(const Vector2D& rhs) const {
		return Vector2D(x*rhs.x, y*rhs.y);
	}

	inline Vector2D& Vector2D::operator*=(const float a) {
		x *= a; y *= a; return *this;
	}

	inline Vector2D& Vector2D::operator*=(const Vector2D& rhs) {
		x *= rhs.x; y *= rhs.y; return *this;
	}

	inline Vector2D Vector2D::operator/(const float a) const {
		return Vector2D(x / a, y / a);
	}

	inline Vector2D Vector2D::operator/(const Vector2D& rhs) const {
		return Vector2D(x / rhs.x, y / rhs.y);
	}

	inline Vector2D& Vector2D::operator/=(const float a) {
		x /= a; y /= a; return *this;
	}

	inline bool Vector2D::operator==(const Vector2D& rhs) const {
		return (x == rhs.x) && (y == rhs.y);
	}

	inline bool Vector2D::operator!=(const Vector2D& rhs) const {
		return (x != rhs.x) || (y != rhs.y);
	}

	inline void Vector2D::operator=(const Vector2D& rhs) {
		x = rhs.x; y = rhs.y;
	}

	inline bool Vector2D::operator<(const Vector2D& rhs) const {
		if (x < rhs.x) return true;
		if (x > rhs.x) return false;
		if (y < rhs.y) return true;
		if (y > rhs.y) return false;
		return false;
	}

	inline float Vector2D::operator[](int index) const {
		return (&x)[index];
	}

	inline float& Vector2D::operator[](int index) {
		return (&x)[index];
	}

	inline void Vector2D::Set(float x, float y) {
		this->x = x; this->y = y;
	}

	inline float Vector2D::Length() const {
		return sqrtf(x*x + y*y);
	}

	inline float Vector2D::Distance(const Vector2D& vec) const {
		return sqrtf((vec.x - x)*(vec.x - x) + (vec.y - y)*(vec.y - y));
	}

	inline Vector2D& Vector2D::Normalize() {
		//@@const float EPSILON = 0.000001f;
		float xxyy = x*x + y*y;
		//@@if(xxyy < EPSILON)
		//@@    return *this;

		//float invLength = invSqrt(xxyy);
		float invLength = 1.0f / sqrtf(xxyy);
		x *= invLength;
		y *= invLength;
		return *this;
	}

	inline void Vector2D::Negative() {
		x = -x; y = -y;
	}

	inline void Vector2D::Add(const Vector2D& rhs) {
		x += rhs.x; y += rhs.y;
	}

	inline void Vector2D::Add(float v) {
		x += v; y += v;
	}

	inline void Vector2D::Add(float vx, float vy) {
		x += vx; y += vy;
	}

	inline void Vector2D::Sub(const Vector2D& rhs) {
		x -= rhs.x; y -= rhs.y;
	}

	inline void Vector2D::Sub(float v) {
		x -= v; y -= v;
	}

	inline void Vector2D::Sub(float vx, float vy) {
		x -= vx; y -= vy;
	}

	inline void Vector2D::Mul(const Vector2D& rhs) {
		x *= rhs.x; y *= rhs.y;
	}

	inline void Vector2D::Mul(float v) {
		x *= v; y *= v;
	}

	inline void Vector2D::Mul(float vx, float vy) {
		x *= vx; y *= vy;
	}

	inline void Vector2D::Div(const Vector2D& rhs) {
		x /= rhs.x; y /= rhs.y;
	}

	inline void Vector2D::Div(float v) {
		x /= v; y /= v;
	}

	inline void Vector2D::Div(float vx, float vy) {
		x /= vx; y /= vy;
	}

	inline float Vector2D::Dot(const Vector2D& rhs) const {
		return (x*rhs.x + y*rhs.y);
	}

	inline bool Vector2D::Equal(const Vector2D& rhs, float epsilon) const {
		return fabs(x - rhs.x) < epsilon && fabs(y - rhs.y) < epsilon;
	}

	inline Vector2D operator*(const float a, const Vector2D vec) {
		return Vector2D(a*vec.x, a*vec.y);
	}

	inline Vector2D Vector2D::Clone() const {
		return Vector2D(x, y);
	}

	inline std::ostream& operator<<(std::ostream& os, const Vector2D& vec) {
		os << "(" << vec.x << ", " << vec.y << ")";
		return os;
	}
	// END OF VECTOR2 /////////////////////////////////////////////////////////////




	///////////////////////////////////////////////////////////////////////////////
	// inline functions for Vector3
	///////////////////////////////////////////////////////////////////////////////
	inline Vector3D Vector3D::operator-() const {
		return Vector3D(-x, -y, -z);
	}

	inline Vector3D Vector3D::operator+(const Vector3D& rhs) const {
		return Vector3D(x + rhs.x, y + rhs.y, z + rhs.z);
	}

	inline Vector3D Vector3D::operator-(const Vector3D& rhs) const {
		return Vector3D(x - rhs.x, y - rhs.y, z - rhs.z);
	}

	inline Vector3D& Vector3D::operator+=(const Vector3D& rhs) {
		x += rhs.x; y += rhs.y; z += rhs.z; return *this;
	}

	inline Vector3D& Vector3D::operator-=(const Vector3D& rhs) {
		x -= rhs.x; y -= rhs.y; z -= rhs.z; return *this;
	}

	inline Vector3D Vector3D::operator*(const float a) const {
		return Vector3D(x*a, y*a, z*a);
	}

	inline Vector3D Vector3D::operator*(const Vector3D& rhs) const {
		return Vector3D(x*rhs.x, y*rhs.y, z*rhs.z);
	}

	inline Vector3D& Vector3D::operator*=(const float a) {
		x *= a; y *= a; z *= a; return *this;
	}

	inline Vector3D& Vector3D::operator*=(const Vector3D& rhs) {
		x *= rhs.x; y *= rhs.y; z *= rhs.z; return *this;
	}

	inline Vector3D Vector3D::operator/(const float a) const {
		return Vector3D(x / a, y / a, z / a);
	}

	inline Vector3D Vector3D::operator/(const Vector3D& rhs) const {
		return Vector3D(x / rhs.x, y / rhs.y, z / rhs.z);
	}

	inline Vector3D& Vector3D::operator/=(const float a) {
		x /= a; y /= a; z /= a; return *this;
	}

	inline bool Vector3D::operator==(const Vector3D& rhs) const {
		return (x == rhs.x) && (y == rhs.y) && (z == rhs.z);
	}

	inline bool Vector3D::operator!=(const Vector3D& rhs) const {
		return (x != rhs.x) || (y != rhs.y) || (z != rhs.z);
	}

	inline bool Vector3D::operator<(const Vector3D& rhs) const {
		if (x < rhs.x) return true;
		if (x > rhs.x) return false;
		if (y < rhs.y) return true;
		if (y > rhs.y) return false;
		if (z < rhs.z) return true;
		if (z > rhs.z) return false;
		return false;
	}

	inline void Vector3D::operator=(const Vector3D& rhs) {
		x = rhs.x; y = rhs.y; z = rhs.z;
	}

	inline float Vector3D::operator[](int index) const {
		return (&x)[index];
	}

	inline float& Vector3D::operator[](int index) {
		return (&x)[index];
	}

	inline void Vector3D::Set(float x, float y, float z) {
		this->x = x; this->y = y; this->z = z;
	}

	inline float Vector3D::Length() const {
		return sqrtf(x*x + y*y + z*z);
	}

	inline float Vector3D::Distance(const Vector3D& vec) const {
		return sqrtf((vec.x - x)*(vec.x - x) + (vec.y - y)*(vec.y - y) + (vec.z - z)*(vec.z - z));
	}

	inline float Vector3D::Angle(const Vector3D& vec) const {
		// return angle between [0, 180]
		float l1 = this->Length();
		float l2 = vec.Length();
		float d = this->Dot(vec);
		float angle = acosf(d / (l1 * l2)) / 3.141592f * 180.0f;
		return angle;
	}

	inline Vector3D& Vector3D::Normalize() {
		//@@const float EPSILON = 0.000001f;
		float xxyyzz = x*x + y*y + z*z;
		//@@if(xxyyzz < EPSILON)
		//@@    return *this; // do nothing if it is ~zero vector

		//float invLength = invSqrt(xxyyzz);
		float invLength = 1.0f / sqrtf(xxyyzz);
		x *= invLength;
		y *= invLength;
		z *= invLength;
		return *this;
	}

	inline void Vector3D::Negative() {
		x = -x; y = -y, z = -z;
	}

	inline void Vector3D::Add(const Vector3D& rhs) {
		x += rhs.x; y += rhs.y, z += rhs.z;
	}

	inline void Vector3D::Add(float v) {
		x += v; y += v; z += v;
	}

	inline void Vector3D::Add(float v, const Vector2D& rhs) {
		x += v; y += rhs.x; z += rhs.y;
	}

	inline void Vector3D::Add(const Vector2D& rhs, float v) {
		x += rhs.x; y += rhs.y; z += v;
	}

	inline void Vector3D::Add(float vx, float vy, float vz) {
		x += vx; y += vy; z += vz;
	}

	inline void Vector3D::Sub(const Vector3D& rhs) {
		x -= rhs.x; y -= rhs.y; z -= rhs.z;
	}

	inline void Vector3D::Sub(float v) {
		x -= v; y -= v; z -= v;
	}

	inline void Vector3D::Sub(float v, const Vector2D& rhs) {
		x -= v; y -= rhs.x; z -= rhs.y;
	}

	inline void Vector3D::Sub(const Vector2D& rhs, float v) {
		x -= rhs.x; y -= rhs.y; z -= v;
	}

	inline void Vector3D::Sub(float vx, float vy, float vz) {
		x -= vx; y -= vy; z -= vz;
	}

	inline void Vector3D::Mul(const Vector3D& rhs) {
		x *= rhs.x; y *= rhs.y; z *= rhs.z;
	}

	inline void Vector3D::Mul(float v) {
		x *= v; y *= v; z *= v;
	}

	inline void Vector3D::Mul(float vx, float vy, float vz) {
		x *= vx; y *= vy; z *= vz;
	}

	inline void Vector3D::Div(const Vector3D& rhs) {
		x /= rhs.x; y /= rhs.y; z /= rhs.z;
	}

	inline void Vector3D::Div(float v) {
		x /= v; y /= v; z /= v;
	}

	inline void Vector3D::Div(float vx, float vy, float vz) {
		x /= vx; y /= vy; z /= vz;
	}

	inline float Vector3D::Dot(const Vector3D& rhs) const {
		return (x*rhs.x + y*rhs.y + z*rhs.z);
	}

	inline Vector3D Vector3D::Cross(const Vector3D& rhs) const {
		return Vector3D(y*rhs.z - z*rhs.y, z*rhs.x - x*rhs.z, x*rhs.y - y*rhs.x);
	}

	inline void Vector3D::Cross(float vx, float vy, float vz) {
		float tx = x, ty = y, tz = z;
		x = ty*vz - tz*vy;
		y = tz*vx - tx*vz;
		z = tx*vy - ty*vx;
	}

	inline void Vector3D::CrossC(const Vector3D& rhs) {
		float tx = x, ty = y, tz = z;
		x = ty*rhs.z - tz*rhs.y;
		y = tz*rhs.x - tx*rhs.z;
		z = tx*rhs.y - ty*rhs.x;
	}

	inline bool Vector3D::Equal(const Vector3D& rhs, float epsilon) const {
		return fabs(x - rhs.x) < epsilon && fabs(y - rhs.y) < epsilon && fabs(z - rhs.z) < epsilon;
	}

	inline Vector3D Vector3D::Clone() const {
		return Vector3D(x, y, z);
	}

	inline Vector3D operator*(const float a, const Vector3D vec) {
		return Vector3D(a*vec.x, a*vec.y, a*vec.z);
	}

	inline std::ostream& operator<<(std::ostream& os, const Vector3D& vec) {
		os << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
		return os;
	}
	// END OF VECTOR3 /////////////////////////////////////////////////////////////



	///////////////////////////////////////////////////////////////////////////////
	// inline functions for Vector4
	///////////////////////////////////////////////////////////////////////////////
	inline Vector4D Vector4D::operator-() const {
		return Vector4D(-x, -y, -z, -w);
	}

	inline Vector4D Vector4D::operator+(const Vector4D& rhs) const {
		return Vector4D(x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w);
	}

	inline Vector4D Vector4D::operator-(const Vector4D& rhs) const {
		return Vector4D(x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w);
	}

	inline Vector4D& Vector4D::operator+=(const Vector4D& rhs) {
		x += rhs.x; y += rhs.y; z += rhs.z; w += rhs.w; return *this;
	}

	inline Vector4D& Vector4D::operator-=(const Vector4D& rhs) {
		x -= rhs.x; y -= rhs.y; z -= rhs.z; w -= rhs.w; return *this;
	}

	inline Vector4D Vector4D::operator*(const float a) const {
		return Vector4D(x*a, y*a, z*a, w*a);
	}

	inline Vector4D Vector4D::operator*(const Vector4D& rhs) const {
		return Vector4D(x*rhs.x, y*rhs.y, z*rhs.z, w*rhs.w);
	}

	inline Vector4D& Vector4D::operator*=(const float a) {
		x *= a; y *= a; z *= a; w *= a; return *this;
	}

	inline Vector4D& Vector4D::operator*=(const Vector4D& rhs) {
		x *= rhs.x; y *= rhs.y; z *= rhs.z; w *= rhs.w; return *this;
	}

	inline Vector4D Vector4D::operator/(const float a) const {
		return Vector4D(x / a, y / a, z / a, w / a);
	}

	inline Vector4D Vector4D::operator/(const Vector4D& rhs) const {
		return Vector4D(x / rhs.x, y / rhs.y, z / rhs.z, w / rhs.z);
	}

	inline Vector4D& Vector4D::operator/=(const float a) {
		x /= a; y /= a; z /= a; w /= a; return *this;
	}

	inline bool Vector4D::operator==(const Vector4D& rhs) const {
		return (x == rhs.x) && (y == rhs.y) && (z == rhs.z) && (w == rhs.w);
	}

	inline bool Vector4D::operator!=(const Vector4D& rhs) const {
		return (x != rhs.x) || (y != rhs.y) || (z != rhs.z) || (w != rhs.w);
	}

	inline bool Vector4D::operator<(const Vector4D& rhs) const {
		if (x < rhs.x) return true;
		if (x > rhs.x) return false;
		if (y < rhs.y) return true;
		if (y > rhs.y) return false;
		if (z < rhs.z) return true;
		if (z > rhs.z) return false;
		if (w < rhs.w) return true;
		if (w > rhs.w) return false;
		return false;
	}

	inline void Vector4D::operator=(const Vector4D& rhs) {
		x = rhs.x; y = rhs.y; z = rhs.z; w = rhs.w;
	}

	inline float Vector4D::operator[](int index) const {
		return (&x)[index];
	}

	inline float& Vector4D::operator[](int index) {
		return (&x)[index];
	}

	inline void Vector4D::Set(float x, float y, float z, float w) {
		this->x = x; this->y = y; this->z = z; this->w = w;
	}

	inline float Vector4D::Length() const {
		return sqrtf(x*x + y*y + z*z + w*w);
	}

	inline float Vector4D::Distance(const Vector4D& vec) const {
		return sqrtf((vec.x - x)*(vec.x - x) + (vec.y - y)*(vec.y - y) + (vec.z - z)*(vec.z - z) + (vec.w - w)*(vec.w - w));
	}

	inline Vector4D& Vector4D::Normalize() {
		//NOTE: leave w-component untouched
		//@@const float EPSILON = 0.000001f;
		float xxyyzz = x*x + y*y + z*z;
		//@@if(xxyyzz < EPSILON)
		//@@    return *this; // do nothing if it is zero vector

		//float invLength = invSqrt(xxyyzz);
		float invLength = 1.0f / sqrtf(xxyyzz);
		x *= invLength;
		y *= invLength;
		z *= invLength;
		return *this;
	}

	inline void Vector4D::Negative() {
		x = -x; y = -y, z = -z; w = -w;
	}

	inline void Vector4D::Add(const Vector4D& rhs) {
		x += rhs.x; y += rhs.y, z += rhs.z; w += rhs.w;
	}

	inline void Vector4D::Add(float v) {
		x += v; y += v; z += v; w += v;
	}

	inline void Vector4D::Add(float v, const Vector3D& rhs) {
		x += v; y += rhs.x; z += rhs.y; w += rhs.z;
	}

	inline void Vector4D::Add(const Vector3D& rhs, float v) {
		x += rhs.x; y += rhs.y; z += rhs.z; w += v;
	}

	inline void Vector4D::Add(const Vector2D& rhs, float vz, float vw) {
		x += rhs.x; y += rhs.y; z += vz; w += vw;
	}

	inline void Vector4D::Add(float vx, const Vector2D& rhs, float vw) {
		x += vx; y += rhs.x; z += rhs.y; w += vw;
	}

	inline void Vector4D::Add(float vx, float vy, const Vector2D& rhs) {
		x += vx; y += vy; z += rhs.x; w += rhs.y;
	}

	inline void Vector4D::Add(float vx, float vy, float vz, float vw) {
		x += vx; y += vy; z += vz; w += vw;
	}

	inline void Vector4D::Sub(const Vector4D& rhs) {
		x -= rhs.x; y -= rhs.y; z -= rhs.z; w -= rhs.w;
	}

	inline void Vector4D::Sub(float v) {
		x -= v; y -= v; z -= v; w -= v;
	}

	inline void Vector4D::Sub(float v, const Vector3D& rhs) {
		x -= v; y -= rhs.x; z -= rhs.y; w -= rhs.z;
	}

	inline void Vector4D::Sub(const Vector3D& rhs, float v) {
		x -= rhs.x; y -= rhs.y; z -= rhs.z; w -= v;
	}

	inline void Vector4D::Sub(const Vector2D& rhs, float vz, float vw) {
		x -= rhs.x; y -= rhs.y; z -= vz; w -= vw;
	}

	inline void Vector4D::Sub(float vx, const Vector2D& rhs, float vw) {
		x -= vx; y -= rhs.x; z -= rhs.y; w -= vw;
	}

	inline void Vector4D::Sub(float vx, float vy, const Vector2D& rhs) {
		x -= vx; y -= vy; z -= rhs.x; w -= rhs.y;
	}

	inline void Vector4D::Sub(float vx, float vy, float vz, float vw) {
		x -= vx; y -= vy; z -= vz; w -= vw;
	}

	inline void Vector4D::Mul(const Vector4D& rhs) {
		x *= rhs.x; y *= rhs.y; z *= rhs.z; w *= rhs.w;
	}

	inline void Vector4D::Mul(float v) {
		x *= v; y *= v; z *= v; w *= v;
	}

	inline void Vector4D::Mul(float vx, float vy, float vz, float vw) {
		x *= vx; y *= vy; z *= vz; w *= vw;
	}

	inline void Vector4D::Div(const Vector4D& rhs) {
		x /= rhs.x; y /= rhs.y; z /= rhs.z; w /= rhs.w;
	}

	inline void Vector4D::Div(float v) {
		x /= v; y /= v; z /= v; w /= v;
	}

	inline void Vector4D::Div(float vx, float vy, float vz, float vw) {
		x /= vx; y /= vy; z /= vz; w /= vw;
	}

	inline float Vector4D::Dot(const Vector4D& rhs) const {
		return (x*rhs.x + y*rhs.y + z*rhs.z + w*rhs.w);
	}

	inline bool Vector4D::Equal(const Vector4D& rhs, float epsilon) const {
		return fabs(x - rhs.x) < epsilon && fabs(y - rhs.y) < epsilon &&
			fabs(z - rhs.z) < epsilon && fabs(w - rhs.w) < epsilon;
	}

	inline Vector4D Vector4D::Clone() const {
		return Vector4D(x, y, z, w);
	}

	inline Vector4D operator*(const float a, const Vector4D vec) {
		return Vector4D(a*vec.x, a*vec.y, a*vec.z, a*vec.w);
	}

	inline std::ostream& operator<<(std::ostream& os, const Vector4D& vec) {
		os << "(" << vec.x << ", " << vec.y << ", " << vec.z << ", " << vec.w << ")";
		return os;
	}
	// END OF VECTOR4 /////////////////////////////////////////////////////////////
}

