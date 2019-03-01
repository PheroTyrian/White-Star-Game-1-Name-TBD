////////////////////////////////////////////////////////////////////////////////////////////////////
// file:	ShapeMath.h
//
// summary:	Declares the shape mathematics class. Set of math functions between shapes
////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include "Vector.h"
#include "Line.h"
#include "Rectangle.h"
#include "Circle.h"
#include "Polygon.h"
#include "CoreTypes\Transform.h"

namespace HAPISPACE {

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Query if this object intersects the given r1. </summary>
	///
	/// <typeparam name="T">	Generic type parameter. </typeparam>
	/// <param name="r1">	The first Rectangle&lt;T&gt; </param>
	/// <param name="r2">	The second Rectangle&lt;T&gt; </param>
	///
	/// <returns>	True if it succeeds, false if it fails. </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	template <typename T>
	bool Intersects(const Rectangle<T>& r1, const Rectangle<T>& r2) noexcept
	{
		// Using <= since for my rects right is one outside the rect

		if (r1.right <= r2.left)
			return false;

		if (r1.bottom <= r2.top)
			return false;

		if (r1.left >= r2.right)
			return false;

		if (r1.top >= r2.bottom)
			return false;

		return true;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>
	/// Passed 3 points, the first two are the line to test, the last is used to determine which side
	/// is 'inside'.
	/// </summary>
	///
	/// <typeparam name="T">	Generic type parameter. </typeparam>
	/// <param name="p1">  	The first Vector&lt;T&gt; </param>
	/// <param name="p2">  	The second Vector&lt;T&gt; </param>
	/// <param name="p3">  	The third Vector&lt;T&gt; </param>
	/// <param name="poly">	The polygon. </param>
	///
	/// <returns>	True if the test passes, false if the test fails. </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	template <typename T>	
	bool EdgeTest(const Vector<T>& p1, const Vector<T>& p2, const Vector<T>& p3,const Polygon &poly) 
	{
		Vector<T> edge{ p2 - p1 };
		Vector<T> perp{ -edge.y, edge.x };

		// Work out side that is inside using our 3rd point i.e. work out which side of the edge it is
		// Using dot product
		bool sign = (perp.x * (p3.x - p1.x) + perp.y * (p3.y - p1.y) >= 0);

		// Now test which side the points of the poly are
		// If they are on the same side then this line cannot be a separating line
		for (size_t i = 0; i < poly.points.size(); i++)
		{
			// What side of the edge is point
			if ((perp.x * (poly.points[i].x - p1.x) + perp.y * (poly.points[i].y - p1.y) >= 0) == sign)
				return false; // Line p1 to p2 cannot be a seperating line
		}

		// Line p1 to p2 may be a seperating line
		return true;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>
	/// based on
	/// https://stackoverflow.com/questions/217578/how-can-i-determine-whether-a-2d-point-is-within-a-polygon.
	/// </summary>
	///
	/// <typeparam name="T">	Generic type parameter. </typeparam>
	/// <param name="poly">	The polygon. </param>
	/// <param name="test">	The test. </param>
	///
	/// <returns>	An int. </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	template <typename T>
	inline int PointInPolygon(const Polygon &poly, const Vector<T>& test)
	{
		int nverts = (int)poly.points.size();
		int i, j, c = 0;
		for (i = 0, j = nverts - 1; i < nverts; j = i++) {
			if (((poly.points[i].y>test.y) != (poly.points[j].y>test.y)) &&
				(test.x < (poly.points[j].x - poly.points[i].x) * (test.y - poly.points[i].y) / (poly.points[j].y - poly.points[i].y) + poly.points[i].x))
				c = !c;
		}
		return c;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	convex polygon test. </summary>
	///
	/// <param name="poly1">	The first polygon. </param>
	/// <param name="poly2">	The second polygon. </param>
	///
	/// <returns>	True if it succeeds, false if it fails. </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	inline bool Intersects(const Polygon &poly1, const Polygon &poly2)
	{
		// Find a separating line
		// Note: could be sped up for rectangle case - see Tristan answer
		// https://stackoverflow.com/questions/115426/algorithm-to-detect-intersection-of-two-rectangles

		// Have to handle the situation where one poly is completely within another
		bool inside{ true };
		for (const auto& p : poly1.points)
		{
			if (!PointInPolygon(poly2, p))
			{
				inside = false;
				break;
			}
		}
		if (inside)
			return true;

		inside = true;
		for (const auto& p : poly2.points)
		{
			if (!PointInPolygon(poly1, p))
			{
				inside = false;
				break;
			}
		}
		if (inside)
			return true;

		// Seperating edges test
		for (size_t i=0;i<poly1.points.size();i++)
		{
			size_t i2{ i + 1 };
			if (i2 >= poly1.points.size())
				i2 -= poly1.points.size();
			size_t i3{ i2 + 1 };
			if (i3 >= poly1.points.size())
				i3 -= poly1.points.size();

			const VectorF p1{ poly1.points[i] };
			const VectorF p2{ poly1.points[i2] };
			const VectorF p3{ poly1.points[i3] };

			if (EdgeTest(p1, p2, p3, poly2))
			{
		//		std::cout << "Poly1 seperating edge from Poly2" << std::endl;

				// The line in poly1 is completely to one side of poly2
				// Meaning it could be a seperating axis but we need to make sure the other points of poly1 are
				// on the other side
				return false;
			}
		}

	//	std::cout << "Poly1 NO seperating edge from Poly2" << std::endl;

		// Need to do same for the other poly
		for (size_t i = 0; i<poly2.points.size(); i ++)
		{
			size_t i2{ i + 1 };
			if (i2 >= poly2.points.size())
				i2 -= poly2.points.size();
			size_t i3{ i2 + 1 };
			if (i3 >= poly2.points.size())
				i3 -= poly2.points.size();

			const VectorF p1{ poly2.points[i] };
			const VectorF p2{ poly2.points[i2] };
			const VectorF p3{ poly2.points[i3] };

			if (EdgeTest(p1, p2, p3, poly1))
			{
		//		std::cout << "Poly2 seperating edge from Poly1" << std::endl;
				// The line in poly1 is completely to one side of poly2
				// Meaning it could be a seperating axis but we need to make sure the other points of poly1 are
				// on the other side
				return false;
			}
		}
			

//		std::cout << "Poly2 NO seperating edge from Poly1" << std::endl;

		// Must intersect
		return true;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Query if this object intersects the given r1. </summary>
	///
	/// <typeparam name="T">	Generic type parameter. </typeparam>
	/// <param name="r1">	The first RectangleOriented&lt;T&gt; </param>
	/// <param name="r2">	The second RectangleOriented&lt;T&gt; </param>
	///
	/// <returns>	True if it succeeds, false if it fails. </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	template <typename T>
	bool Intersects(const RectangleOriented<T>& r1, const RectangleOriented<T>& r2)
	{
		// Use Convex polygon intersection
		return Intersects(Polygon(r1.GetOutline()), Polygon(r2.GetOutline()));
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	pnt is optonal and gives the intersection point (if there is one) </summary>
	///
	/// <typeparam name="T">	Generic type parameter. </typeparam>
	/// <param name="circle">	The circle. </param>
	/// <param name="line">  	The line. </param>
	/// <param name="pnt">   	[in,out] (Optional) If non-null, the point. </param>
	///
	/// <returns>	True if it succeeds, false if it fails. </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	template <typename T>
	bool Intersects(const Circle& circle, const Line<T> line,Vector<T> *pnt=nullptr)
	{
		// See: https://stackoverflow.com/questions/1073336/circle-line-segment-collision-detection-algorithm

		//  Direction vector of ray, from start to end
		Vector<T> d = line.p2 - line.p1;
		// Vector from center sphere to ray start 
		Vector<T> f = line.p1 - circle.centre;

		float r = circle.radius;
		float a = d.Dot(d);
		float b = 2 * f.Dot(d);
		float c = f.Dot(f) - r*r;

		float discriminant = b*b - 4 * a*c;
		if (discriminant < 0)
		{
			// no intersection
		}
		else
		{
			// ray didn't totally miss sphere,
			// so there is a solution to
			// the equation.

			discriminant = sqrt(discriminant);

			// either solution may be on or off the ray so need to test both
			// t1 is always the smaller value, because BOTH discriminant and
			// a are nonnegative.
			float t1 = (-b - discriminant) / (2 * a);
			float t2 = (-b + discriminant) / (2 * a);

			// 3x HIT cases:
			//          -o->             --|-->  |            |  --|->
			// Impale(t1 hit,t2 hit), Poke(t1 hit,t2>1), ExitWound(t1<0, t2 hit), 

			// 3x MISS cases:
			//       ->  o                     o ->              | -> |
			// FallShort (t1>1,t2>1), Past (t1<0,t2<0), CompletelyInside(t1<0, t2>1)

			if (t1 >= 0 && t1 <= 1)
			{
				// t1 is the intersection, and it's closer than t2
				// (since t1 uses -b - discriminant)
				// Impale, Poke
				if (pnt)
				{
					// Intersection point P = E + t*d;	
					// E is start of the ray, d is direction of ray
					*pnt = Vector<T>{ line.p1 + t1 * d };
				}

				return true;
			}

			// here t1 didn't intersect so we are either started
			// inside the sphere or completely past it
			if (t2 >= 0 && t2 <= 1)
			{
				// ExitWound

				if (pnt)
				{
					// Intersection point P = E + t*d;	
					// E is start of the ray, d is direction of ray
					*pnt = Vector<T>{ line.p1 + t2 * d };
				}
			

				return true;
			}
		}
		
		// no intn: FallShort, Past, CompletelyInside
		return false;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>
	/// See
	/// https://stackoverflow.com/questions/401847/circle-rectangle-collision-detection-intersection
	/// Note: could be expanded to work with any polygon not just a rect.
	/// </summary>
	///
	/// <typeparam name="T">	Generic type parameter. </typeparam>
	/// <param name="rect">  	The rectangle. </param>
	/// <param name="circle">	The circle. </param>
	///
	/// <returns>	True if it succeeds, false if it fails. </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	template <typename T>
	bool Intersects(const RectangleOriented<T>& rect, const Circle& circle)
	{
		// if none of the edges of the rectangle has a point in the circle
		// i.e. if all the edges are completely "outside" the circle
		// then the only way the circle can still intersect the polygon is if it lies completely inside the polygon.

		// Simple initial test to see if centre of circle is inside the rectangle
		if (rect.Contains(circle.centre))
			return true;

		// Now check to see if line endpoints are in the circle
		if (circle.Contains(rect.corners[0]))
			return true;
		if (circle.Contains(rect.corners[1]))
			return true;
		if (circle.Contains(rect.corners[2]))
			return true;
		if (circle.Contains(rect.corners[3]))
			return true;

		// Now the more complex part which is if a point on the line is within the circle
		std::vector<Line<T>> lines = Polygon(rect.GetOutline()).GetLines();

		for (const auto& l : lines)
		{
			if (Intersects(circle, l))
				return true;
		}

		return false;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Query if this object intersects the given c1. </summary>
	///
	/// <param name="c1">	The first Circle. </param>
	/// <param name="c2">	The second Circle. </param>
	///
	/// <returns>	True if it succeeds, false if it fails. </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	inline bool Intersects(const Circle &c1,const Circle &c2)
	{
		//return (c1.centre.DistanceBetweenSquared(c2.centre) <= 
			//(c1.radius + c2.radius) * (c1.radius + c2.radius));
		return (c1.centre.DistanceBetween(c2.centre) <= (c1.radius + c2.radius));
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>
	/// Intersection of two lines
	/// http://www.ahristov.com/tutorial/geometry-games/intersection-lines.html Note: this presumes
	/// infinitly long lines TODO: redo these infinity ones to make them rays as thats what they
	/// really are.
	/// </summary>
	///
	/// <typeparam name="T">	Generic type parameter. </typeparam>
	/// <param name="line1">			  	The first line. </param>
	/// <param name="line2">			  	The second line. </param>
	/// <param name="intersectionVectorF">	[in,out] The intersection vector f. </param>
	///
	/// <returns>	True if it succeeds, false if it fails. </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	template <typename T>
	bool IntersectsInfinity(const Line<T> &line1, const Line<T> &line2, Vector<T>& intersectionVectorF)
	{
		// x1,y1 and x2,y2 are Line 1
		// x3,y3 and x4,y4 are Line 2
		T x1 = line1.p1.x;
		T y1 = line1.p1.y;
		T x2 = line1.p2.x;
		T y2 = line1.p2.y;

		T x3 = line2.p1.x;
		T y3 = line2.p1.y;
		T x4 = line2.p2.x;
		T y4 = line2.p2.y;

		T d = (x1 - x2)*(y3 - y4) - (y1 - y2)*(x3 - x4);
		if (d == 0) return false;

		T xi = ((x3 - x4)*(x1*y2 - y1*x2) - (x1 - x2)*(x3*y4 - y3*x4)) / d;
		T yi = ((y3 - y4)*(x1*y2 - y1*x2) - (y1 - y2)*(x3*y4 - y3*x4)) / d;
		
		intersectionVectorF = Vector<T>(xi, yi);
		return true;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>
	/// https://stackoverflow.com/questions/563198/how-do-you-detect-where-two-line-segments-intersect.
	/// </summary>
	///
	/// <typeparam name="T">	Generic type parameter. </typeparam>
	/// <param name="line1">			  	The first line. </param>
	/// <param name="line2">			  	The second line. </param>
	/// <param name="intersectionVectorF">	[in,out] The intersection vector f. </param>
	///
	/// <returns>	True if it succeeds, false if it fails. </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	template <typename T>
	bool Intersects(const Line<T> &line1, const Line<T> &line2, Vector<T>& intersectionVectorF)
	{
		// x1,y1 and x2,y2 are Line 1
		// x3,y3 and x4,y4 are Line 2
		T p0_x = line1.p1.x;
		T p0_y = line1.p1.y;
		T p1_x = line1.p2.x;
		T p1_y = line1.p2.y;

		T p2_x = line2.p1.x;
		T p2_y = line2.p1.y; 
		T p3_x = line2.p2.x;
		T p3_y = line2.p2.y;

		float s1_x, s1_y, s2_x, s2_y;
		s1_x = p1_x - p0_x;     s1_y = p1_y - p0_y;
		s2_x = p3_x - p2_x;     s2_y = p3_y - p2_y;

		float s, t;
		s = (-s1_y * (p0_x - p2_x) + s1_x * (p0_y - p2_y)) / (-s2_x * s1_y + s1_x * s2_y);
		t = (s2_x * (p0_y - p2_y) - s2_y * (p0_x - p2_x)) / (-s2_x * s1_y + s1_x * s2_y);

		if (s >= 0 && s <= 1 && t >= 0 && t <= 1)
		{
			// Collision detected
			intersectionVectorF.x = p0_x + (t * s1_x);;
			intersectionVectorF.y = p0_y + (t * s1_y);
				
			return true;
		}

		return false; // No collision
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Transform vector. </summary>
	///
	/// <typeparam name="T">	Generic type parameter. </typeparam>
	/// <param name="v">		A Vector&lt;T&gt; to process. </param>
	/// <param name="trans">	The transform. </param>
	///
	/// <returns>	A Vector&lt;T&gt; </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	template <typename T>
	Vector<T> TransformVector(const Vector<T> v, const Transform& trans)
	{
		// Round origin
		VectorF ret = (VectorF)v - trans.origin;

		// S
		ret *= trans.scale;

		// R
		if (trans.IsRotated())
			ret.Rotate(trans.rotation);

		// T
		ret += trans.position;

		// Origin
		ret += trans.origin;

		return Vector<T>(ret);
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Inverse transform vector. </summary>
	///
	/// <typeparam name="T">	Generic type parameter. </typeparam>
	/// <param name="v">		A Vector&lt;T&gt; to process. </param>
	/// <param name="trans">	The transform. </param>
	///
	/// <returns>	A Vector&lt;T&gt; </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	template <typename T>
	Vector<T> InverseTransformVector(const Vector<T> v, const Transform& trans)
	{
		// Round origin
		VectorF ret = (VectorF)v - trans.origin;

		// T
		ret -= trans.position;

		// R
		if (trans.IsRotated())
			ret.Rotate(-trans.rotation);

		// S
		ret.x *= 1.0f / trans.scale.x;
		ret.y *= 1.0f / trans.scale.y;

		// Origin
		ret += trans.origin;

		return Vector<T>(ret);
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	As above for a vector of points. </summary>
	///
	/// <typeparam name="T">	Generic type parameter. </typeparam>
	/// <param name="pnts"> 	The pnts. </param>
	/// <param name="trans">	The transform. </param>
	///
	/// <returns>	A std::vector&lt;Vector&lt;T&gt;&gt; </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	template <typename T>
	std::vector<Vector<T>> TransformVectors(const std::vector<Vector<T>> &pnts, const Transform& trans)
	{
		std::vector<Vector<T>> ret{ pnts };
		for (auto& p : ret)
			p = TransformVector(p, trans);

		return ret;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	As above for a vector of points. </summary>
	///
	/// <typeparam name="T">	Generic type parameter. </typeparam>
	/// <param name="pnts"> 	The pnts. </param>
	/// <param name="trans">	The transform. </param>
	///
	/// <returns>	A std::vector&lt;Vector&lt;T&gt;&gt; </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	template <typename T>
	std::vector<Vector<T>> InverseTransformVectors(const std::vector<Vector<T>> &pnts, const Transform& trans)
	{
		std::vector<Vector<T>> ret{ pnts };
		for (auto& p : ret)
			p = InverseTransformVector(p, trans);

		return ret;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Retrieve a rectangle that bounds a line. </summary>
	///
	/// <typeparam name="T">	Generic type parameter. </typeparam>
	/// <param name="line">	The line. </param>
	///
	/// <returns>	The bounding rectangle. </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	template <typename T>
	Rectangle<T> GetBoundingRect(const Line<T>& line)
	{
		Rectangle<T> rect;

		rect.left = std::min(line.p1.x, line.p2.x);
		rect.right = std::max(line.p1.x, line.p2.x);

		rect.top = std::min(line.p1.y, line.p2.y);
		rect.bottom = std::max(line.p1.y, line.p2.y);

		return rect;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Retrieve a rectangle that bounds a number of line. </summary>
	///
	/// <typeparam name="T">	Generic type parameter. </typeparam>
	/// <param name="line">	The line. </param>
	///
	/// <returns>	The bounding rectangle. </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	template <typename T>
	Rectangle<T> GetBoundingRect(const std::vector<Line<T>>& line)
	{
		Rectangle<T> rect;

		if (line.empty())
			return rect;

		rect.left = std::min(line[0].p1.x, line[0].p2.x);
		rect.right = std::max(line[0].p1.x, line[0].p2.x);

		rect.top = std::min(line[0].p1.y, line[0].p2.y);
		rect.bottom = std::max(line[0].p1.y, line[0].p2.y);

		for (size_t i = 1; i < line.size(); i++)
		{
			rect.Encompass(GetBoundingRect(line[i]));
		}

		return rect;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Transform rectangle oriented. </summary>
	///
	/// <typeparam name="T">	Generic type parameter. </typeparam>
	/// <param name="v">		A RectangleOriented&lt;T&gt; to process. </param>
	/// <param name="trans">	The transform. </param>
	///
	/// <returns>	A RectangleOriented&lt;T&gt; </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	template <typename T>
	// Transforms oriented rectangle. Scaling & rotation is around the origin in the transform
	RectangleOriented<T> TransformRectangleOriented(const RectangleOriented<T> v, const Transform& trans)
	{
		RectangleOriented<T> ret{ v };
		
		// S
		ret.Scale(trans.scale,trans.origin);

		// R
		ret.Rotate(trans.rotation, trans.origin);

		// T
		ret.Translate(trans.position);

		return ret;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>
	/// Transform rectangle.
	///  Transforms aligned rectangle. Scaling is done via centre of rectangle while rotation is
	///  around the origin in the transform
	/// Returns an oriented rectangle (as rotation takes away axis-aligned nature)
	/// </summary>
	///
	/// <typeparam name="T">	Generic type parameter. </typeparam>
	/// <param name="v">		A Rectangle&lt;T&gt; to process. </param>
	/// <param name="trans">	The transform. </param>
	///
	/// <returns>	A RectangleOriented&lt;T&gt; </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	template <typename T>
	RectangleOrientedF TransformRectangle(const Rectangle<T> v, const Transform& trans)
	{
		return TransformRectangleOriented(RectangleOrientedF(v),trans);
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Transform circle. </summary>
	///
	/// <param name="c">		A Circle to process. </param>
	/// <param name="trans">	The transform. </param>
	///
	/// <returns>	A Circle. </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	inline Circle TransformCircle(const Circle c, const Transform& trans)
	{
		Circle ret{ c };

		ret.Translate(-trans.origin);

		// S		
		ret.radius *= std::max(trans.scale.x, trans.scale.y);

		// Also scale centre point
		ret.centre *= trans.scale;
		
		// R
		// Do need to rotate centre as if entity rotates and circle not on centre of rotation it would be wrong
		ret.centre.Rotate(trans.rotation);

		// T
		ret.Translate(trans.position);

		ret.Translate(trans.origin);

		return ret;
	}
}
