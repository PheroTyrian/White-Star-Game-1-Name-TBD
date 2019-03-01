////////////////////////////////////////////////////////////////////////////////////////////////////
// file:	Rectangle.h
//
// summary:	Declares the rectangle class
////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

namespace HAPISPACE {

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>
	/// A rectangle. A simple rectangle class with left, right, top and bottom members. Bottom is >
	/// top(y down the screen)
	/// HAPI Sprites treats bottom and left as outside borders e.g. for frames.
	/// </summary>
	///
	/// <typeparam name="T">	Generic type parameter. </typeparam>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	template <typename T> class Rectangle final
	{
	public:
		/// <summary>Default values are for an invalid rectangle </summary>
		T left{ 0 }, right{ 0 }, top{ 0 }, bottom{ 0 };

		/// <summary>	Constructors. </summary>
		Rectangle()  noexcept {}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Construct from values for left, right, top and bottom. </summary>
		///
		/// <param name="leftIn">  	The left in. </param>
		/// <param name="rightIn"> 	The right in. </param>
		/// <param name="topIn">   	The top in. </param>
		/// <param name="bottomIn">	The bottom in. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		Rectangle(T leftIn, T rightIn, T topIn, T bottomIn) noexcept : left(leftIn), right(rightIn), top(topIn), bottom(bottomIn) {}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Construct from a top left Vector<T> and bottom right point. </summary>
		///
		/// <param name="topLeft">	  	The top left. </param>
		/// <param name="bottomRight">	The bottom right. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		Rectangle(Vector<T> topLeft, Vector<T> bottomRight) noexcept : left(topLeft.x), right(bottomRight.x), top(topLeft.y), bottom(bottomRight.y) {}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Construct from top left and width and height. </summary>
		///
		/// <param name="topLeft">	The top left. </param>
		/// <param name="width">  	The width. </param>
		/// <param name="height"> 	The height. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		Rectangle(Vector<T> topLeft, T width, T height)  noexcept : left(topLeft.x), right(topLeft.x + width), top(topLeft.y), bottom(topLeft.y + height) {}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Construct from width and height. </summary>
		///
		/// <param name="width"> 	The width. </param>
		/// <param name="height">	The height. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		Rectangle(T width, T height)  noexcept : left(0), right(width), top(0), bottom(height) {}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Construct to cover all points. </summary>
		///
		/// <param name="points">	The points. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		Rectangle(const std::vector<Vector<T>>& points) noexcept
		{
			if (points.empty())
				return;

			left = right = points[0].x;
			top = bottom = points[0].y;

			for (const auto &p : points)
			{
				left = std::min(left, p.x);
				right = std::max(right, p.x);

				top = std::min(top, p.y);
				bottom = std::max(bottom, p.y);
			}
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Returns the width of the rectagle. </summary>
		///
		/// <returns>	A T. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		T Width() const noexcept { return right - left; }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Return the height of the rectange. </summary>
		///
		/// <returns>	A T. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		T Height() const noexcept { return bottom - top; }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	A valid rectangle has right>left and bottom>top (assuming +y is down) </summary>
		///
		/// <returns>	True if valid, false if not. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		bool IsValid() const { if (left >= right || top >= bottom) return false; return true; }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Does this rectangle completely contain the other? </summary>
		///
		/// <param name="other">	The const Rectangle&amp; to test for containment. </param>
		///
		/// <returns>	True if the object is in this collection, false if not. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		bool Contains(const Rectangle& other) const { return (other.left >= left && other.right < right && other.top >= top && other.bottom < bottom); }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Does this rectangle completely contain the point. </summary>
		///
		/// <param name="other">	The Vector&lt;T&gt; to test for containment. </param>
		///
		/// <returns>	True if the object is in this collection, false if not. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		bool Contains(Vector<T> other) const { return !(other.x < left || other.x > right || other.y < top || other.y > bottom); }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Is this rectangle completely outside of the other? </summary>
		///
		/// <param name="other">	The other. </param>
		///
		/// <returns>	True if it succeeds, false if it fails. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		bool OutsideOf(const Rectangle& other) const { return (right < other.left || bottom < other.top || left >= other.right || top >= other.bottom); }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Translate this rectangle. </summary>
		///
		/// <param name="dx">	The dx. </param>
		/// <param name="dy">	The dy. </param>
		///
		/// <returns>	A reference to a Rectangle&lt;T&gt; </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		Rectangle<T>& Translate(T dx, T dy) { left += dx; right += dx; top += dy; bottom += dy; return *this;}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Translate this rectangle using a point. </summary>
		///
		/// <param name="d">	A Vector&lt;T&gt; to process. </param>
		///
		/// <returns>	A reference to a Rectangle&lt;T&gt; </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		Rectangle<T>& Translate(Vector<T> d) { left += d.x; right += d.x; top += d.y; bottom += d.y; return *this; }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Scales around origin (centre) </summary>
		///
		/// <param name="scale">	The scale. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void Scale(const VectorF& scale)
		{
			Vector<T> centre{ GetCentre() };

			Translate(-centre);

			left = T(left * scale.x);
			right = T(right *scale.x);
			top = T(top * scale.y);
			bottom = T(bottom * scale.y);

			Translate(centre);
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Returns translated version of this. </summary>
		///
		/// <param name="dx">	The dx. </param>
		/// <param name="dy">	The dy. </param>
		///
		/// <returns>	A Rectangle&lt;T&gt; </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		Rectangle<T> Translated(T dx, T dy) const
		{
			Rectangle<T> ret{ *this };
			ret.Translate(dx, dy);
			return ret;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Translate rectangle using a point Returns translated version of this. </summary>
		///
		/// <param name="d">	A Vector&lt;T&gt; to process. </param>
		///
		/// <returns>	A Rectangle&lt;T&gt; </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		Rectangle<T> Translated(Vector<T> d) const
		{
			Rectangle<T> ret{ *this };
			ret.Translate(d.x, d.y);
			return ret;
		}

		/// <summary>	Translate to origin. </summary>
		void TranslateToOrigin() { right -= left; left = 0; bottom -= top; top = 0; }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Returns translated version of this to origin. </summary>
		///
		/// <returns>	A Rectangle&lt;T&gt; </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		Rectangle<T> TranslatedToOrigin() { Rectangle<T> rt{ *this }; rt.TranslateToOrigin(); return rt; }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Expand leaves left, top the same. </summary>
		///
		/// <param name="d">	A Vector&lt;T&gt; to process. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void Expand(Vector<T> d) { right += d.x; bottom += d.y; }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	ExpandAll works on all edges. </summary>
		///
		/// <param name="d">	A Vector&lt;T&gt; to process. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void ExpandAll(Vector<T> d) { right += d.x; bottom += d.y; left -= d.x; top -= d.y; }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Encompass passed in rectangle i.e. grow if required. </summary>
		///
		/// <param name="other">	The other. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void Encompass(const Rectangle& other) {
			left = std::min(left, other.left); right = std::max(right, other.right);
			top = std::min(top, other.top); bottom = std::max(bottom, other.bottom);
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Encompass passed in vector i.e. grow if required. </summary>
		///
		/// <param name="other">	The other. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void Encompass(const Vector<T>& other) {
			left = std::min(left, other.x); right = std::max(right, other.x);
			top = std::min(top, other.y); bottom = std::max(bottom, other.y);
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Clip this rectangle against the other. </summary>
		///
		/// <param name="other">	The other. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void ClipTo(const Rectangle& other)
		{
			left = std::max(left, other.left);
			right = std::min(right, other.right);
			top = std::max(top, other.top);
			bottom = std::min(bottom, other.bottom);
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Vector<T> class getters for individual access to the 4 corners of the rectangle.
		/// </summary>
		///
		/// <returns>	A Vector&lt;T&gt; </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		Vector<T> TopLeft() const { return Vector<T>(left, top); }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Top right. </summary>
		///
		/// <returns>	A Vector&lt;T&gt; </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		Vector<T> TopRight() const { return Vector<T>(right, top); }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Bottom left. </summary>
		///
		/// <returns>	A Vector&lt;T&gt; </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		Vector<T> BottomLeft() const { return Vector<T>(left, bottom); }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Bottom right. </summary>
		///
		/// <returns>	A Vector&lt;T&gt; </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		Vector<T> BottomRight() const { return Vector<T>(right, bottom); }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the dimensions. </summary>
		///
		/// <returns>	A Vector&lt;T&gt; </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		Vector<T> Dimensions() const { return Vector<T>(Width(), Height()); }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Returns all 4 corners of the rectangle in clockwise order Note: once Vis fully supports c++14
		/// can remove extra bracket.
		/// </summary>
		///
		/// <returns>	The corners. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		std::vector<Vector<T>> GetCorners() const { return{ { {TopLeft()},{TopRight()},{BottomRight()},{BottomLeft()} } }; }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Returns 4 lines representing the outline of this rectangle or the edges. </summary>
		///
		/// <returns>	The outline. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		std::vector<Line<T>> GetOutline() const { return{ { {TopLeft(),TopRight()},{TopRight(),BottomRight()},{ BottomRight(),BottomLeft() },{ BottomLeft(),TopLeft() } } }; }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Length from top left to bottom right squared. </summary>
		///
		/// <returns>	A T. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		T DiagonalLengthSquared() const { return (right - left)*(right - left) + (bottom - top)*(bottom - top); }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Centre of rectangle. </summary>
		///
		/// <returns>	The centre. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		Vector<T> GetCentre() const {
			return Vector<T>{Width() / 2 + left, Height() / 2 + top};
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// enclossing = true returns radius of a circle around this rectangle enclossing = false returns
		/// radius of a circle within this rectangle.
		/// </summary>
		///
		/// <param name="enclossing">	(Optional) True to enclossing. </param>
		///
		/// <returns>	The radius. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		float GetRadius(bool enclossing=true) const 
		{
			VectorF floatCentre{ 0.5f * Width() + left,0.5f * Height() + top };

			if (enclossing)
				return ((VectorF)TopLeft() - floatCentre).Length();
		
			VectorF pointOn{ floatCentre.x , (float)top+1 };

			return (pointOn-floatCentre).Length();
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Creates a random rectangle between min and max points Note max must be > min by at least 2.
		/// </summary>
		///
		/// <param name="min">	The minimum. </param>
		/// <param name="max">	The maximum. </param>
		///
		/// <returns>	A Rectangle. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		static Rectangle Random(Vector<T> min, Vector<T> max)
		{
			Vector<T> topLeft(Vector::Random(min.x, max.x - 2, min.y, max.y - 2));
			Vector<T> bottomRight(Vector::Random(topLeft.x + 1, max.x, topLeft.y + 1, max.y));
			return Rectangle(topLeft, bottomRight);
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	float * this rectangle evenly. </summary>
		///
		/// <param name="mult">	The multiply. </param>
		///
		/// <returns>	The result of the operation. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		Rectangle& operator*=(float mult)
		{
			left = (T)(left * mult);
			right = (T)(right * mult);
			top = (T)(top* mult);
			bottom = (T)(bottom* mult);
			return *this;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	vector float * this rectangle. </summary>
		///
		/// <param name="mult">	The multiply. </param>
		///
		/// <returns>	The result of the operation. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		Rectangle& operator*=(const VectorF &mult)
		{
			left = (T)(left * mult.x);
			right = (T)(right * mult.x);
			top = (T)(top* mult.y);
			bottom = (T)(bottom* mult.y);
			return *this;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Subtract other from this. </summary>
		///
		/// <param name="other">	The other. </param>
		///
		/// <returns>	The result of the operation. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		Rectangle<T> operator-=(const Rectangle<T> &other)
		{
			left -= other.left;
			right -= other.right;
			top -= other.top;
			bottom -= other.bottom;
			return *this;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Add other to this. </summary>
		///
		/// <param name="other">	The other. </param>
		///
		/// <returns>	The result of the operation. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		Rectangle<T> operator+=(const Rectangle<T> &other)
		{
			left += other.left;
			right += other.right;
			top += other.top;
			bottom += other.bottom;
			return *this;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Cast that converts the given std::ostream&amp; to a &lt;&lt;&lt;typename T&gt;
		/// </summary>
		///
		/// <param name="os">	[in,out] The operating system. </param>
		/// <param name="r"> 	A Rectangle&lt;T&gt; to process. </param>
		///
		/// <returns>	The result of the operation. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		template <typename T>
		friend std::ostream& operator<<(std::ostream& os, const Rectangle<T>& r);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Convert this object into a string representation. </summary>
		///
		/// <returns>	A std::string that represents this object. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		std::string ToString() const { std::stringstream str; str << *this; return str.str(); }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Returns the normal for a position on the rectangle edge. </summary>
		///
		/// <param name="pos">	The position. </param>
		///
		/// <returns>	The normal on edge. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		VectorF GetNormalOnEdge(const VectorI &pos) const
		{
			// Diagonals only right on the corner
			if (pos.x == left && pos.y == top)
				return VectorF(-0.70710710678f, -0.70710710678f);
			if (pos.x >= right-1 && pos.y == top)
				return VectorF(0.70710710678f, -0.70710710678f);
			if (pos.x == left && pos.y >= bottom-1)
				return VectorF(-0.70710710678f, +0.70710710678f);
			if (pos.x >= right-1 && pos.y >= bottom - 1)
				return VectorF(+0.70710710678f, +0.70710710678f);

			if (pos.x == left)
				return VectorF(-1, 0);
			if (pos.x >= right-1)
				return VectorF(1, 0);
			if (pos.y == top)
				return VectorF(0, -1);
			if (pos.y >= bottom-1)
				return VectorF(0, 1);

			return VectorF::Zero();
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Writes an XML. </summary>
		///
		/// <param name="node">	[in,out] The node. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void WriteXML(CHapiXMLNode &node) const
		{
			node.AddAttribute({ "left",std::to_string(left) });
			node.AddAttribute({ "right",std::to_string(right) });
			node.AddAttribute({ "top",std::to_string(top) });
			node.AddAttribute({ "bottom",std::to_string(bottom) });
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Reads an XML. </summary>
		///
		/// <param name="node">	[in,out] The node. </param>
		/// <param name="rect">	[in,out] The rectangle. </param>
		///
		/// <returns>	True if it succeeds, false if it fails. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		static bool ReadXML(CHapiXMLNode &node, Rectangle<int> &rect)
		{
			CHapiXMLAttribute attribute;
			if (!node.GetAttributeWithName("left", attribute))
				return false;
			rect.left = attribute.AsInt();

			if (!node.GetAttributeWithName("right", attribute))
				return false;
			rect.right = attribute.AsInt();

			if (!node.GetAttributeWithName("top", attribute))
				return false;
			rect.top = attribute.AsInt();

			if (!node.GetAttributeWithName("bottom", attribute))
				return false;

			rect.bottom = attribute.AsInt();

			return true;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// Decided to make  both of these explicit so I can spot any slow conversions explicit
		/// conversion to int.
		/// </summary>
		///
		/// <returns>	The result of the operation. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		explicit operator Rectangle<int>() const noexcept { return Rectangle<int>((int)left,(int)right,(int)top,(int)bottom); }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	explicit conversion to float. </summary>
		///
		/// <returns>	The result of the operation. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		explicit operator Rectangle<float>() const noexcept { return Rectangle<float>((float)left, (float)right, (float)top, (float)bottom); }
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	vector float * this rectangle. </summary>
	///
	/// <param name="lhs"> 	The left hand side. </param>
	/// <param name="mult">	The multiply. </param>
	///
	/// <returns>	The result of the operation. </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	template <typename T>
	inline Rectangle<T> operator*(Rectangle<T> lhs, const VectorF &mult)
	{
		return lhs *= mult;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Can be used with cout to output values to output pane and log. </summary>
	///
	/// <param name="os">	[in,out] The operating system. </param>
	/// <param name="r"> 	A Rectangle&lt;T&gt; to process. </param>
	///
	/// <returns>	The result of the operation. </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	template <typename T>
	inline std::ostream& operator<<(std::ostream& os, const Rectangle<T>& r)
	{
		os << "Left:" + std::to_string(r.left) + " Right:" + std::to_string(r.right) +
			" Top:" + std::to_string(r.top) + " Bottom:" + std::to_string(r.bottom) +
			" (Width:" + std::to_string(r.Width()) + " Height:" + std::to_string(r.Height()) + ")";
		return os;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Subtraction operator. </summary>
	///
	/// <param name="lhs">	The left hand side. </param>
	/// <param name="rhs">	The right hand side. </param>
	///
	/// <returns>	The result of the operation. </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	template <typename T>
	inline Rectangle<T> operator-(Rectangle<T> lhs, const Rectangle<T> &rhs)
	{
		Rectangle<T> ret{ lhs };
		ret -= rhs;
		return ret;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Addition operator. </summary>
	///
	/// <param name="lhs">	The left hand side. </param>
	/// <param name="rhs">	The right hand side. </param>
	///
	/// <returns>	The result of the operation. </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	template <typename T>
	inline Rectangle<T> operator+(Rectangle<T> lhs, const Rectangle<T> &rhs)
	{
		Rectangle<T> ret{ lhs };
		ret += rhs;
		return ret;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Equals. </summary>
	///
	/// <param name="lhs">	The left hand side. </param>
	/// <param name="rhs">	The right hand side. </param>
	///
	/// <returns>	The result of the operation. </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	template <typename T>
	inline bool operator==(const Rectangle<T> &lhs, const Rectangle<T> &rhs)
	{
		return (lhs.left == rhs.left && lhs.right == rhs.right && lhs.top == rhs.top && lhs.bottom == rhs.bottom);
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Inequality operator. </summary>
	///
	/// <param name="lhs">	The left hand side. </param>
	/// <param name="rhs">	The right hand side. </param>
	///
	/// <returns>	The result of the operation. </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	template <typename T>
	inline bool operator!=(const Rectangle<T> &lhs, const Rectangle<T> &rhs)
	{
		return !(lhs==rhs);
	}

	/// <summary>	The rectangle i. </summary>
	using RectangleI = Rectangle<int>;
	/// <summary>	The rectangle f. </summary>
	using RectangleF = Rectangle<float>;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	A rectangle oriented. This class cannot be inherited. </summary>
	///
	/// <typeparam name="T">	Generic type parameter. </typeparam>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	template <typename T>
	class RectangleOriented final
	{
	public:
		// Cannot describe as left, top etc. as may not be
		/// <summary>	The corners[ 4]. </summary>
		Vector<T> corners[4];

		/// <summary>	Default constructor. </summary>
		RectangleOriented() = default;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Individually. </summary>
		///
		/// <param name="c1">	The first Vector&lt;T&gt; </param>
		/// <param name="c2">	The second Vector&lt;T&gt; </param>
		/// <param name="c3">	The third Vector&lt;T&gt; </param>
		/// <param name="c4">	The fourth Vector&lt;T&gt; </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		RectangleOriented(Vector<T> c1, Vector<T> c2, Vector<T> c3, Vector<T> c4)
		{
			corners[0] = c1;  corners[1] = c2, corners[2] = c3, corners[3] = c4;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Construct from aligned rect. </summary>
		///
		/// <typeparam name="U">	Generic type parameter. </typeparam>
		/// <param name="rect">	The rectangle. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		template <typename U>
		RectangleOriented(const Rectangle<U> &rect) : corners
		{
			{ (T)rect.left, (T)rect.top },{ (T)rect.right, (T)rect.top } ,
			{ (T)rect.right, (T)rect.bottom },{ (T)rect.left, (T)rect.bottom }
		/// <summary>	. </summary>
		} {}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Scales around origin. </summary>
		///
		/// <param name="scale"> 	The scale. </param>
		/// <param name="origin">	The origin. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void Scale(const VectorF& scale, Vector<T> origin)
		{
			Translate(-origin);

			for (int i = 0; i < 4; i++)
				corners[i] = Vector<T>(corners[i] * scale);

			Translate(origin);
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	cos and sine precalculated to feed in: </summary>
		///
		/// <param name="cosine">	The cosine. </param>
		/// <param name="sine">  	The sine. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void Rotate(float cosine, float sine)
		{
			for (int i = 0; i < 4; i++)
			{
				/*
				x' = x cos f - y sin f
				y' = y cos f + x sin f
				*/
				corners[i] = Vector<T>(T(corners[i].x * cosine - corners[i].y * sine),
					T(corners[i].y * cosine + corners[i].x * sine));
			}
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Rotates. Note this rotates corners as-is and does not work around centre. </summary>
		///
		/// <param name="radians">	The radians. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void Rotate(float radians)
		{
			Rotate(cos(radians), sin(radians));
		}		

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Rotate around a point. </summary>
		///
		/// <param name="radians">	The radians. </param>
		/// <param name="origin"> 	The origin. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void Rotate(float radians, const Vector<T>& origin)
		{
			Translate(-origin);
			Rotate(radians);
			Translate(origin);
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Uses centre as point of rotation. </summary>
		///
		/// <param name="radians">	The radians. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void RotateAroundCentre(float radians)
		{
			Rotate(radians, GetCentre());
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Translate rectangle. </summary>
		///
		/// <param name="dx">	The dx. </param>
		/// <param name="dy">	The dy. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void Translate(T dx, T dy)
		{
			for (int i = 0; i < 4; i++)
			{
				corners[i].x += dx;
				corners[i].y += dy;
			}
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Translate rectangle using a point. </summary>
		///
		/// <param name="d">	A Vector&lt;T&gt; to process. </param>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		void Translate(Vector<T> d)
		{
			Translate(d.x, d.y);
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Translated. </summary>
		///
		/// <param name="dx">	The dx. </param>
		/// <param name="dy">	The dy. </param>
		///
		/// <returns>	A RectangleOriented&lt;T&gt; </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		RectangleOriented<T> Translated(T dx, T dy) const
		{
			Rectangle<T> ret{ *this };
			ret.Translate(dx, dy);
			return ret;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Translate rectangle using a point. </summary>
		///
		/// <param name="d">	A Vector&lt;T&gt; to process. </param>
		///
		/// <returns>	A RectangleOriented&lt;T&gt; </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		RectangleOriented<T> Translated(Vector<T> d) const
		{
			Rectangle<T> ret{ *this };
			ret.Translate(d.x, d.y);
			return ret;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Note: assumes corners are defined clockwise. </summary>
		///
		/// <returns>	The centre. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		Vector<T> GetCentre() const
		{
			return corners[0] + ((corners[2] - corners[0]) / 2);
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Radius of a circle around this rectangle. </summary>
		///
		/// <returns>	The radius. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		float GetRadius() const {
			return ((VectorF)GetCentre() - (VectorF)corners[0]).Length();
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets enclosing a bb. </summary>
		///
		/// <returns>	The enclosing a bb. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		Rectangle<T> GetEnclosingAABB() const noexcept
		{
			return Rectangle<T>(
				T(std::min(corners[0].x, std::min(corners[1].x, std::min(corners[2].x, corners[3].x)))),
				T(std::max(corners[0].x, std::max(corners[1].x, std::max(corners[2].x, corners[3].x)))),
				T(std::min(corners[0].y, std::min(corners[1].y, std::min(corners[2].y, corners[3].y)))),
				T(std::max(corners[0].y, std::max(corners[1].y, std::max(corners[2].y, corners[3].y))))
				);
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Assumes points are defined clockwise this is corner 2 - corner 1. </summary>
		///
		/// <returns>	A T. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		T Width() const
		{
			return (T)(((VectorF)(corners[1] - corners[0])).Length());
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Assumes points are defined clockwise this is corner 3 - corner 0. </summary>
		///
		/// <returns>	A T. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		T Height() const
		{
			return (T)(((VectorF)(corners[3] - corners[0])).Length());
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	To avoid linked dependencies now returning points. </summary>
		///
		/// <returns>	The outline. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		std::vector<Vector<T>> GetOutline() const {
			//return std::vector<Vector<T>>(corners, corners + 4); // Vector construction from pointers as iterators - cool :)
			// but always 4 so better:
			return std::vector<Vector<T>>{corners[0], corners[1], corners[2], corners[3]};
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Does this rectangle completely contain the point. </summary>
		///
		/// <param name="point">	The Vector&lt;T&gt; to test for containment. </param>
		///
		/// <returns>	True if the object is in this collection, false if not. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		bool Contains(Vector<T> point) const 
		{ 
			// Need to do 0 <= dot(AB,AP) <= dot(AB,AB) && 0 <= dot(BC, BP) <= dot(BC, BC)
			// Where P is point in question and rect defined by ABCA
			// (projection of point onto lines AB and BC, must project onto both)			
			// Note: This only works if AB and BC are pependicular which is the case for a rectangle

			Vector<T> AB = (corners[1] - corners[0]);

			float dotABAP = AB.Dot(point - corners[0]);
			if (dotABAP < 0)
				return false;

			float dotABAB = AB.Dot(AB);
			if (dotABAB < dotABAP)
				return false;

			Vector<T> BC = (corners[2] - corners[1]);

			float dotBCBP = BC.Dot(point - corners[1]);
			if (dotBCBP < 0)
				return false;

			float dotBCBC = BC.Dot(BC);
			if (dotBCBC < dotBCBP)
				return false;

			return true;			
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	vector float * this rectangle's corners (scale) </summary>
		///
		/// <param name="mult">	The multiply. </param>
		///
		/// <returns>	The result of the operation. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		RectangleOriented<T>& operator*=(const VectorF &mult)
		{
			for (auto &c : corners)
				c = (VectorF)c * mult;

			return *this;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Emplicit conversion as quite a bit of work. </summary>
		///
		/// <returns>	The result of the operation. </returns>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		template <typename U>
		explicit operator RectangleOriented<U>() const 
		{ 
			RectangleOriented<U> newRect;
			for (int i = 0; i < 4; i++)
				newRect.corners[i] = (Vector<U>)corners[i];
			return newRect;
		}
	};

	/// <summary>	The rectangle oriented f. </summary>
	using RectangleOrientedF = RectangleOriented<float>;
}
