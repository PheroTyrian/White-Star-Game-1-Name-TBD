#pragma once

//#include <iostream>
//#include "Vector.h"

namespace HAPISPACE {

	// A basic line class
	// Performance wise it is normally best to pass by value not const reference
	template <typename T>
	class Line final
	{
	public:
		Vector<T> p1, p2;

		// Default, invalid line
		Line() noexcept : p1{ 0, 0 }, p2{ 0,0 } {}

		// Construct from two points
		Line(Vector<T> p1in, Vector<T> p2in)  noexcept : p1(p1in), p2(p2in) {}
		
		// Construct from x,y values for each point
		Line(T x1, T y1, T x2, T y2) noexcept : p1(x1,y1), p2(x2,y2) {}

		// Translates this line by d
		void Translate(const Vector<T> &d) {p1 += d; p2 += d;}

		// Returns the manhattan length of the line
		int ManhattanLength() const {return p1.ManhattanDistanceBetween(p2);}

		// Returns the length squared (useful for optimisation)
		float LengthSquared() const { return p1.DistanceBetween(p2); }

		// Returns the actual length of the line
		float Length() const { return sqrt(LengthSquared()); }

		// Returns the direction of the line from p1 to p2
		Vector<T> Direction() const { return (p2 - p1).Normalized(); }

		// A 2D line has two normals depending on perceived rotation
		// Normal of the line. 
		Vector<T> GetNormal(bool clockwise) const
		{
			Vector<T> edge{ (p2 - p1).Normalized() };
			if (clockwise)
				return Vector<T>{-edge.y, edge.x};
			return Vector<T>{edge.y, -edge.x};
		}

		// Determines if a point is clockwise to this line i.e. right side or below if horizontal
		//https://stackoverflow.com/questions/1560492/how-to-tell-whether-a-point-is-to-the-right-or-left-side-of-a-line
		bool IsClockwise(const Vector<T> &pnt) const {
			return ((p2.x - p1.x)*(pnt.y- p1.y) - (p2.y - p1.y)*(pnt.x - p1.x)) > 0;
		}

		bool IsHorizontal() const {
			return (p1.y == p2.y);
		}

		bool IsVertical() const {
			return (p1.x == p2.x);
		}

		// implicit conversion
		// Note: it is worth making this explicit sometimes to detect any uneeded casts automatically happening
		template <typename U>
		operator Line<U>() const { return Line<U>((Vector<U>)p1, (Vector<U>)p2); }

		template <typename T>
		friend std::ostream& operator<<(std::ostream& os, const Line<T>& l);

		std::string ToString() const { std::stringstream str; str << *this; return str.str(); }
	};

	// Can be used with cout to output values to output pane and log
	template <typename T>
	inline std::ostream& operator<<(std::ostream& os, const Line<T>& l)
	{
		os << "P1: " << l.p1 << " P2: " << l.p2;
		return os;
	}

	// Equivelance operator
	template <typename T>
	inline bool operator==(const Line<T>& lhs, const Line<T>& rhs)
	{
		return (lhs.p1 == rhs.p1 && lhs.p2 == rhs.p2);
	}

	// Does not equal operator
	template <typename T>
	inline bool operator!=(const Line<T>& lhs, const Line<T>& rhs)
	{
		return !operator==(lhs, rhs);
	}
	
	// Shortcut aliases
	using LineI = Line<int>;
	using LineF = Line<float>;	
}