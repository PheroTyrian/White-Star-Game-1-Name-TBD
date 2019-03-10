#pragma once

//#include <vector>
//#include <cassert>
//#include <iostream>
//#include <sstream>
//#include <algorithm>
//
//#include "Vector.h"
//#include "Rectangle.h"
//#include "line.h"

namespace HAPISPACE {

	// A polygon class, assumes last point and first point are joined
	// Always uses float vectors
	class Polygon final
	{
	public:
		std::vector<Vector<float>> points;

		Polygon() noexcept = default;

		// Construct from points
		// Note: you need 3 or more points to make a poly
		Polygon(std::vector<Vector<float>> pIn) noexcept : points(std::move(pIn)) {}

		friend std::ostream& operator<<(std::ostream& os, const Polygon& l);

		std::string ToString() const { std::stringstream str; str << *this; return str.str(); }

		void Clear() { points.clear(); }

		// Adds a point to the end of the polygon
		void AddPoint(Vector<float> newPoint) {
			points.emplace_back(newPoint);
		}

		// Same as num points as last line goes back to start
		size_t GetNumLines() const { return points.size(); }

		Line<float> GetLine(size_t which) const
		{
			assert(which < GetNumLines());
			if (which == GetNumLines() - 1)
			{
				return Line<float>(points[which], points[0]);
			}

			return Line<float>(points[which], points[which + 1]);
		}

		std::vector<Line<float>> GetLines() const
		{
			std::vector<Line<float>> lines;
			lines.resize(GetNumLines());
			for (size_t i = 0; i < GetNumLines(); i++)
				lines[i] = GetLine(i);
			return lines;
		}

		// Creates a new polygon representing the area of intersection of this with other
		// Note: if the returned polygon has no vertices there was no intersection
		Polygon GetAreaOfIntersection(const Polygon& other) const;

		// What determines the centre of a polygon? This is a rough result
		VectorF GetCentre() const
		{
			VectorF sum{ 0,0 };
			for (const auto& p : points)
				sum += p;
			return (sum / points.size());
		}

		RectangleF GetBounds() const
		{
			if (points.empty())
				return Rectangle<float>();

			RectangleF bounds{ points[0].x,points[0].x, points[0].y,points[0].y };

			for (const auto& p : points)
			{
				bounds.left = std::min( bounds.left,p.x );
				bounds.right = std::max( bounds.right,p.x );
				bounds.top = std::min( bounds.top,p.y );
				bounds.bottom = std::max( bounds.bottom,p.y );
			}

			return bounds;
		}

		// Changes all points by adding trans
		void Translate(const VectorF &trans)
		{
			for (auto& p : points)
				p += trans;
		}

		// Retruns translated version
		Polygon Translated(const VectorF &trans) const
		{
			Polygon newPoly{ points };
			newPoly.Translate(trans);
			return newPoly;
		}

		// Gets the position of the first intersection of a line and this poly
		// Normal is from the line intersection
		// Returns false if no intersection
		bool Intersection(const LineF& line, VectorF& pnt, VectorF& N) const;

		bool IntersectionInfinity(const LineF& line, VectorF& pnt, VectorF& N) const;
	};

	// Can be used with cout to output values to output pane and log
	inline std::ostream& operator<<(std::ostream& os, const Polygon& l)
	{
		for (size_t i = 0; i < l.points.size(); i++)
			os << "P" << i << ": " << l.points[i] << " ";
		return os;
	}
}