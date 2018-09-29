#pragma once

#include <triobj.h>

#include <LiarVertexBuffer.h>

namespace Liar
{
	class LiarStructUtil
	{
	public:

		/// ======================= p3 - color ==================================
		static void ParseColor(Liar::Vector3D* p3, const Color& color)
		{
			p3->x = color.r;
			p3->y = color.g;
			p3->z = color.b;
		}

		static void ParseColor(Liar::Vector3D& p3, const Color& color)
		{
			p3.x = color.r;
			p3.y = color.g;
			p3.z = color.b;
		}
		/// ======================= p3 - color ==================================


		/// ======================= p3 - p3 ==================================
		static void ParsePoint3(Liar::Vector3D* p3, const Point3& color)
		{
			p3->x = color.x;
			p3->y = color.y;
			p3->z = color.z;
		}

		static void ParsePoint3(Liar::Vector3D& p3, const Point3& color)
		{
			p3.x = color.x;
			p3.y = color.y;
			p3.z = color.z;
		}

		static void ParsePoint3(Liar::Vector3D* p3, const Point3& color, bool zy)
		{
			if (zy)
			{
				p3->x = color.x;
				p3->y = color.z;
				p3->z = color.y;
			}
			else
			{
				Liar::LiarStructUtil::ParsePoint3(p3, color);
			}
		}

		static void ParsePoint3(Liar::Vector3D& p3, const Point3& color, bool zy)
		{
			if (zy)
			{
				p3.x = color.x;
				p3.y = color.z;
				p3.z = color.y;
			}
			else
			{
				Liar::LiarStructUtil::ParsePoint3(p3, color);
			}
		}
		/// ======================= p3 - p3 ==================================

		static void ParsePoint2(Liar::Vector2D* p2, const Point2& point)
		{
			p2->x = point.x;
			p2->y = point.y;
		}

		static void ParsePoint3(Liar::Vector2D* p2, const Point3& point)
		{
			p2->x = point.x;
			p2->y = point.y;
		}

		static void ParsePoint3(Liar::Vector3D* p2, const Point2& point)
		{
			p2->x = point.x;
			p2->y = point.y;
			p2->z = 0.0f;
		}

		static void ParsePoint3(Liar::Vector3D* p2, const Point2& point, bool delV)
		{
			if (delV)
			{
				p2->x = point.x;
				p2->y = 1 - point.y;
			}
			else
			{
				Liar::LiarStructUtil::ParsePoint3(p2, point);
			}
		}

		static void ParsePoint2(Liar::Vector2D* p2, const Point2& point, bool delV)
		{
			if (delV)
			{
				p2->x = point.x;
				p2->y = 1 - point.y;
			}
			else
			{
				Liar::LiarStructUtil::ParsePoint2(p2, point);
			}
		}

		static void ParsePoint3(Liar::Vector2D* p2, const Point3& point, bool delV)
		{
			if (delV)
			{
				p2->x = point.x;
				p2->y = 1 - point.y;
			}
			else
			{
				Liar::LiarStructUtil::ParsePoint3(p2, point);
			}
		}

		// ============================================================================
		static void ParsePoint2(Liar::Vector2D& p2, const Point2& point)
		{
			p2.x = point.x;
			p2.y = point.y;
		}

		static void ParsePoint3(Liar::Vector2D& p2, const Point3& point)
		{
			p2.x = point.x;
			p2.y = point.y;
		}

		static void ParsePoint2(Liar::Vector2D& p2, const Point2& point, bool delV)
		{
			if (delV)
			{
				p2.x = point.x;
				p2.y = 1 - point.y;
			}
			else
			{
				Liar::LiarStructUtil::ParsePoint2(p2, point);
			}
		}

		static void ParsePoint3(Liar::Vector2D& p2, const Point3& point, bool delV)
		{
			if (delV)
			{
				p2.x = point.x;
				p2.y = 1 - point.y;
			}
			else
			{
				Liar::LiarStructUtil::ParsePoint3(p2, point);
			}
		}

		static void ParsePoint4(Liar::Vector4D& p4, const Quat& quat)
		{
			p4.x = quat.x;
			p4.y = quat.y;
			p4.z = quat.z;
			p4.w = quat.w;
		}

		static void ParsePoint4(Liar::Vector4D* p4, const Quat& quat)
		{
			ParsePoint4(*p4, quat);
		}

		static bool Equal(float x, float y, float z, 
						float cx, float cy, float cz,
						float epsilon) 
		{
			return fabs(x - cx) < epsilon && fabs(y - cy) < epsilon && fabs(z - cz) < epsilon;
		}

		static bool Equal(float x, float y, float z, float w,
							float cx, float cy, float cz, float cw,
							float epsilon)
		{
			return fabs(x - cx) < epsilon && fabs(y - cy) < epsilon && fabs(z - cz) < epsilon && fabs(w - cw) < epsilon;
		}

		static bool Equal(Point3& p, float cx, float cy, float cz,
			float epsilon)
		{
			return Equal(p.x, p.y, p.z, cx, cy, cz, epsilon);
		}

		static bool Equal(Quat& p, float cx, float cy, float cz, float cw,
			float epsilon)
		{
			return Equal(p.x, p.y, p.z, p.w, cx, cy, cz, cw, epsilon);
		}

	};
}