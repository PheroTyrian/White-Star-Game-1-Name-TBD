////////////////////////////////////////////////////////////////////////////////////////////////////
// file:	Line.h
//
// summary:	Declares the line class
////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

namespace HAPISPACE {

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>
	/// A basic line class. Performance wise it is normally best to pass by value not const reference.
	/// </summary>
	///
	/// <typeparam name="T">	Generic type parameter. </typeparam>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	template <typename T> class Line final
	{
	public:
		Vector<T> p1, p2;

		/// <summary>	Default, invalid line. </summary>
		Line() noexcept : p1{ 0, 0 }, p2{ 0,0 } {}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Construct from two points. </summary>
		///
		/// <param name="p1in">	The 1in. </param>
		/// <param name="p2in">	The 2in. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		Line(Vector<T> p1in, Vector<T> p2in)  noexcept : p1(p1in), p2(p2in) {}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Construct from x,y values for each point. </summary>
		///
		/// <param name="x1">	The first x value. </param>
		/// <param name="y1">	The first y value. </param>
		/// <param name="x2">	The second x value. </param>
		/// <param name="y2">	The second y value. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		Line(T x1, T y1, T x2, T y2) noexcept : p1(x1,y1), p2(x2,y2) {}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Translates this line by d. </summary>
		///
		/// <param name="d">	A Vector&lt;T&gt; to process. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void Translate(const Vector<T> &d) {p1 += d; p2 += d;}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Returns the manhattan length of the line. </summary>
		///
		/// <returns>	An int. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		int ManhattanLength() const {return p1.ManhattanDistanceBetween(p2);}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Returns the length squared (useful for optimisation) </summary>
		///
		/// <returns>	A float. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		float LengthSquared() const { return p1.DistanceBetween(p2); }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Returns the actual length of the line. </summary>
		///
		/// <returns>	A float. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		float Length() const { return sqrt(LengthSquared()); }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Returns the direction of the line from p1 to p2. </summary>
		///
		/// <returns>	A Vector&lt;T&gt; </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		Vector<T> Direction() const { return (p2 - p1).Normalized(); }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// A 2D line has two normals depending on perceived rotation Normal of the line.
		/// </summary>
		///
		/// <param name="clockwise">	True to clockwise. </param>
		///
		/// <returns>	The normal. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		Vector<T> GetNormal(bool clockwise) const
		{
			Vector<T> edge{ (p2 - p1).Normalized() };
			if (clockwise)
				return Vector<T>{-edge.y, edge.x};
			return Vector<T>{edge.y, -edge.x};
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Determines if a point is clockwise to this line i.e. right side or below if horizontal
		/// https://stackoverflow.com/questions/1560492/how-to-tell-whether-a-point-is-to-the-right-or-left-side-of-a-line.
		/// </summary>
		///
		/// <param name="pnt">	The point. </param>
		///
		/// <returns>	True if clockwise, false if not. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		bool IsClockwise(const Vector<T> &pnt) const {
			return ((p2.x - p1.x)*(pnt.y- p1.y) - (p2.y - p1.y)*(pnt.x - p1.x)) > 0;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Query if this object is horizontal. </summary>
		///
		/// <returns>	True if horizontal, false if not. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		bool IsHorizontal() const {
			return (p1.y == p2.y);
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Query if this object is vertical. </summary>
		///
		/// <returns>	True if vertical, false if not. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		bool IsVertical() const {
			return (p1.x == p2.x);
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// implicit conversion Note: it is worth making this explicit sometimes to detect any uneeded
		/// casts automatically happening.
		/// </summary>
		///
		/// <returns>	The result of the operation. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		template <typename U>
		operator Line<U>() const { return Line<U>((Vector<U>)p1, (Vector<U>)p2); }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Cast that converts the given std::ostream&amp; to a &lt;&lt;&lt;typename T&gt;
		/// </summary>
		///
		/// <param name="os">	[in,out] The operating system. </param>
		/// <param name="l"> 	A Line&lt;T&gt; to process. </param>
		///
		/// <returns>	The result of the operation. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		template <typename T>
		friend std::ostream& operator<<(std::ostream& os, const Line<T>& l);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Convert this object into a string representation. </summary>
		///
		/// <returns>	A std::string that represents this object. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		std::string ToString() const { std::stringstream str; str << *this; return str.str(); }
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Can be used with cout to output values to output pane and log. </summary>
	///
	/// <param name="os">	[in,out] The operating system. </param>
	/// <param name="l"> 	A Line&lt;T&gt; to process. </param>
	///
	/// <returns>	The result of the operation. </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	template <typename T>
	inline std::ostream& operator<<(std::ostream& os, const Line<T>& l)
	{
		os << "P1: " << l.p1 << " P2: " << l.p2;
		return os;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Equivelance operator. </summary>
	///
	/// <param name="lhs">	The left hand side. </param>
	/// <param name="rhs">	The right hand side. </param>
	///
	/// <returns>	The result of the operation. </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	template <typename T>
	inline bool operator==(const Line<T>& lhs, const Line<T>& rhs)
	{
		return (lhs.p1 == rhs.p1 && lhs.p2 == rhs.p2);
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Does not equal operator. </summary>
	///
	/// <param name="lhs">	The left hand side. </param>
	/// <param name="rhs">	The right hand side. </param>
	///
	/// <returns>	The result of the operation. </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	template <typename T>
	inline bool operator!=(const Line<T>& lhs, const Line<T>& rhs)
	{
		return !operator==(lhs, rhs);
	}
	
	/// <summary>	Shortcut aliases. </summary>
	using LineI = Line<int>;
	using LineF = Line<float>;	
}