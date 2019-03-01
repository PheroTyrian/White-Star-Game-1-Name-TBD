////////////////////////////////////////////////////////////////////////////////////////////////////
// file:	Circle.h
//
// summary:	Declares the circle class
////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

namespace HAPISPACE {

	/// <summary>	A simple circle class. Not templated as needs to be floats. </summary>
	class Circle
	{
	public:
		/// <summary>	The radius. </summary>
		float radius{ 0 };
		/// <summary>	The centre. </summary>
		VectorF centre;
	public:
		/// <summary>	Default constructor. </summary>
		Circle() noexcept = default;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Constructor. </summary>
		///
		/// <param name="cent">	The cent. </param>
		/// <param name="radi">	The radi. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		Circle(VectorF cent, float radi) noexcept : centre(cent), radius(radi) {}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Query if this object contains the given pnt. </summary>
		///
		/// <typeparam name="T">	Generic type parameter. </typeparam>
		/// <param name="pnt">	The point. </param>
		///
		/// <returns>	True if the object is in this collection, false if not. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		template <typename T>
		bool Contains(const Vector<T>& pnt) const
		{
			return (centre.DistanceBetweenSquared(pnt) <= radius * radius);
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Translates the given tran. </summary>
		///
		/// <typeparam name="T">	Generic type parameter. </typeparam>
		/// <param name="tran">	The tran. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		template <typename T>
		void Translate(const Vector<T>& tran)
		{
			centre += (VectorF)tran;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Translated the given tran. </summary>
		///
		/// <typeparam name="T">	Generic type parameter. </typeparam>
		/// <param name="tran">	The tran. </param>
		///
		/// <returns>	A Circle. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		template <typename T>
		Circle Translated(const Vector<T>& tran) const
		{
			Circle ret{ *this };
			ret.Translate(tran);
			return ret;
		}

		/*
			Expand to cover other circle
			See: https://stackoverflow.com/questions/2084695/finding-the-smallest-circle-that-encompasses-other-circles
		*/

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Encompass the given other. </summary>
		///
		/// <param name="other">	The other. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void Encompass(const Circle& other)
		{
			// Given two circles, with centers[x1, y1], [x2, y2], and radii R1 and R2.What is the center of the enclosing circle ?
			VectorF c1{ centre };
			VectorF c2{ other.centre };
			float r1{ radius };
			float r2{ other.radius };

			// Assume that R1 is no larger than R2. If the second circle is the smaller, then just swap them.
			if (r1 > r2)
			{
				std::swap(c1, c2);
				std::swap(r1, r2);
			}

			// Compute the distance between centers of the circles.
			// D = sqrt((x1 - x2) ^ 2 + (y1 - y2) ^ 2)			
			float dist = (other.centre - centre).Length();

			// Does the first circle lie entirely inside the second circle ? Thus if (D + R1) <= R2, 
			// then we are done. Return the larger circle as the enclosing circle, with a center of[x1, x2], with radius R2.
			if (dist + r1 <= r2)
			{
				centre = c2;
				radius = r2;
				return;
			}

			/* If(D + R1) > R2, then the enclosing circle has a radius of(D + R1 + R2) / 2
			In this latter case, the center of the enclosing circle must lie along the line connecting the two centers.
			So we can write the new center as
				center = (1 - theta)*[x1, y1] + theta*[x2, y2]
			where theta is given by
				theta = 1 / 2 + (R2 - R1) / (2 * D)
			Note that theta will always be a positive number, since we have assured that(D + R1) > R2.
			Likewise, we should be able to ensure that theta is never larger than 1. 
			These two conditions ensure that the enclosing center lies strictly between the two original circle centers.*/

			radius = dist + r1 + r2;
			float theta = 0.5f + (r2 - r1) / (2 * dist);

			centre = (1.0f - theta) * c1 + theta * c2;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets enclosing a bb. </summary>
		///
		/// <returns>	The enclosing a bb. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		RectangleF GetEnclosingAABB() const noexcept
		{
			return RectangleF(centre.x - radius, centre.x + radius, centre.y - radius, centre.y + radius);
		}
	};
}
