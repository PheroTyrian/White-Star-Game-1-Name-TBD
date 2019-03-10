#pragma once

//#include "Vector.h"
//#include "Line.h"

namespace HAPISPACE {

	/*
		Rectangle type
		A simple rectangle class with left, right, top and bottom members.
		Bottom is > top (y down the screen)
		HAPI Sprites treats bottom and left as outside borders e.g. for frames
	*/
	template <typename T>
	class Rectangle final
	{
	public:
		// Default values are for an invalid rectangle
		T left{ 0 }, right{ 0 }, top{ 0 }, bottom{ 0 };

		// Constructors
		Rectangle()  noexcept {}

		// Construct from values for left, right, top and bottom
		Rectangle(T leftIn, T rightIn, T topIn, T bottomIn) noexcept : left(leftIn), right(rightIn), top(topIn), bottom(bottomIn) {}

		// Construct from a top left Vector<T> and bottom right point
		Rectangle(Vector<T> topLeft, Vector<T> bottomRight) noexcept : left(topLeft.x), right(bottomRight.x), top(topLeft.y), bottom(bottomRight.y) {}

		// Construct from top left and width and height
		Rectangle(Vector<T> topLeft, T width, T height)  noexcept : left(topLeft.x), right(topLeft.x + width), top(topLeft.y), bottom(topLeft.y + height) {}

		// Construct from width and height
		Rectangle(T width, T height)  noexcept : left(0), right(width), top(0), bottom(height) {}

		// Construct to cover all points
		Rectangle(const std::vector<Vector<T>>& points)
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

		// Returns the width of the rectagle
		T Width() const { return right - left; }

		// Return the height of the rectange
		T Height() const { return bottom - top; }

		// A valid rectangle has right>left and bottom>top (assuming +y is down) 
		bool IsValid() const { if (left >= right || top >= bottom) return false; return true; }

		// Does this rectangle completely contain the other?		
		bool Contains(const Rectangle& other) const { return (other.left >= left && other.right < right && other.top >= top && other.bottom < bottom); }

		// Does this rectangle completely contain the point
		bool Contains(Vector<T> other) const { return !(other.x < left || other.x > right || other.y < top || other.y > bottom); }

		// Is this rectangle completely outside of the other?
		bool OutsideOf(const Rectangle& other) const { return (right < other.left || bottom < other.top || left >= other.right || top >= other.bottom); }

		// Translate this rectangle
		Rectangle<T>& Translate(T dx, T dy) { left += dx; right += dx; top += dy; bottom += dy; return *this;}

		// Translate this rectangle using a point
		Rectangle<T>& Translate(Vector<T> d) { left += d.x; right += d.x; top += d.y; bottom += d.y; return *this; }

		// Scales around origin (centre)
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

		// Returns translated version of this
		Rectangle<T> Translated(T dx, T dy) const
		{
			Rectangle<T> ret{ *this };
			ret.Translate(dx, dy);
			return ret;
		}

		// Translate rectangle using a point
		// Returns translated version of this
		Rectangle<T> Translated(Vector<T> d) const
		{
			Rectangle<T> ret{ *this };
			ret.Translate(d.x, d.y);
			return ret;
		}

		// Translate to origin
		void TranslateToOrigin() { right -= left; left = 0; bottom -= top; top = 0; }

		// Returns translated version of this to origin
		Rectangle<T> TranslatedToOrigin() { Rectangle<T> rt{ *this }; rt.TranslateToOrigin(); return rt; }

		// Expand leaves left, top the same
		void Expand(Vector<T> d) { right += d.x; bottom += d.y; }
		
		// ExpandAll works on all edges
		void ExpandAll(Vector<T> d) { right += d.x; bottom += d.y; left -= d.x; top -= d.y; }

		// Encompass passed in rectangle i.e. grow if required
		void Encompass(const Rectangle& other) {
			left = std::min(left, other.left); right = std::max(right, other.right);
			top = std::min(top, other.top); bottom = std::max(bottom, other.bottom);
		}

		// Encompass passed in vector i.e. grow if required
		void Encompass(const Vector<T>& other) {
			left = std::min(left, other.x); right = std::max(right, other.x);
			top = std::min(top, other.y); bottom = std::max(bottom, other.y);
		}

		// Clip this rectangle against the other
		void ClipTo(const Rectangle& other)
		{
			left = std::max(left, other.left);
			right = std::min(right, other.right);
			top = std::max(top, other.top);
			bottom = std::min(bottom, other.bottom);
		}

		// Vector<T> class getters for individual access to the 4 corners of the rectangle
		Vector<T> TopLeft() const { return Vector<T>(left, top); }
		Vector<T> TopRight() const { return Vector<T>(right, top); }
		Vector<T> BottomLeft() const { return Vector<T>(left, bottom); }
		Vector<T> BottomRight() const { return Vector<T>(right, bottom); }
		Vector<T> Dimensions() const { return Vector<T>(Width(), Height()); }

		// Returns all 4 corners of the rectangle in clockwise order
		// Note: once Vis fully supports c++14 can remove extra bracket
		std::vector<Vector<T>> GetCorners() const { return{ { {TopLeft()},{TopRight()},{BottomRight()},{BottomLeft()} } }; }

		// Returns 4 lines representing the outline of this rectangle or the edges
		std::vector<Line<T>> GetOutline() const { return{ { {TopLeft(),TopRight()},{TopRight(),BottomRight()},{ BottomRight(),BottomLeft() },{ BottomLeft(),TopLeft() } } }; }

		// Length from top left to bottom right squared
		T DiagonalLengthSquared() const { return (right - left)*(right - left) + (bottom - top)*(bottom - top); }

		// Centre of rectangle
		Vector<T> GetCentre() const {
			return Vector<T>{Width() / 2 + left, Height() / 2 + top};
		}

		// enclossing = true returns radius of a circle around this rectangle
		// enclossing = false returns radius of a circle within this rectangle
		float GetRadius(bool enclossing=true) const 
		{
			VectorF floatCentre{ 0.5f * Width() + left,0.5f * Height() + top };

			if (enclossing)
				return ((VectorF)TopLeft() - floatCentre).Length();
		
			VectorF pointOn{ floatCentre.x , (float)top+1 };

			return (pointOn-floatCentre).Length();
		}

		// Creates a random rectangle between min and max points
		// Note max must be > min by at least 2
		static Rectangle Random(Vector<T> min, Vector<T> max)
		{
			Vector<T> topLeft(Vector::Random(min.x, max.x - 2, min.y, max.y - 2));
			Vector<T> bottomRight(Vector::Random(topLeft.x + 1, max.x, topLeft.y + 1, max.y));
			return Rectangle(topLeft, bottomRight);
		}

		// float * this rectangle evenly
		Rectangle& operator*=(float mult)
		{
			left = (T)(left * mult);
			right = (T)(right * mult);
			top = (T)(top* mult);
			bottom = (T)(bottom* mult);
			return *this;
		}

		// vector float * this rectangle
		Rectangle& operator*=(const VectorF &mult)
		{
			left = (T)(left * mult.x);
			right = (T)(right * mult.x);
			top = (T)(top* mult.y);
			bottom = (T)(bottom* mult.y);
			return *this;
		}

		// Subtract other from this
		Rectangle<T> operator-=(const Rectangle<T> &other)
		{
			left -= other.left;
			right -= other.right;
			top -= other.top;
			bottom -= other.bottom;
			return *this;
		}

		// Add other to this
		Rectangle<T> operator+=(const Rectangle<T> &other)
		{
			left += other.left;
			right += other.right;
			top += other.top;
			bottom += other.bottom;
			return *this;
		}

		template <typename T>
		friend std::ostream& operator<<(std::ostream& os, const Rectangle<T>& r);

		std::string ToString() const { std::stringstream str; str << *this; return str.str(); }

		// Returns the normal for a position on the rectangle edge
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

		void WriteXML(CHapiXMLNode &node) const
		{
			node.AddAttribute({ "left",std::to_string(left) });
			node.AddAttribute({ "right",std::to_string(right) });
			node.AddAttribute({ "top",std::to_string(top) });
			node.AddAttribute({ "bottom",std::to_string(bottom) });
		}

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

		// Decided to make  both of these explicit so I can spot any slow conversions
		// explicit conversion to int
		explicit operator Rectangle<int>() const { return Rectangle<int>((int)left,(int)right,(int)top,(int)bottom); }

		// explicit conversion to float
		explicit operator Rectangle<float>() const { return Rectangle<float>((float)left, (float)right, (float)top, (float)bottom); }
	};

	// vector float * this rectangle
	template <typename T>
	inline Rectangle<T> operator*(Rectangle<T> lhs, const VectorF &mult)
	{
		return lhs *= mult;
	}

	// Can be used with cout to output values to output pane and log
	template <typename T>
	inline std::ostream& operator<<(std::ostream& os, const Rectangle<T>& r)
	{
		os << "Left:" + std::to_string(r.left) + " Right:" + std::to_string(r.right) +
			" Top:" + std::to_string(r.top) + " Bottom:" + std::to_string(r.bottom) +
			" (Width:" + std::to_string(r.Width()) + " Height:" + std::to_string(r.Height()) + ")";
		return os;
	}

	template <typename T>
	inline Rectangle<T> operator-(Rectangle<T> lhs, const Rectangle<T> &rhs)
	{
		Rectangle<T> ret{ lhs };
		ret -= rhs;
		return ret;
	}

	template <typename T>
	inline Rectangle<T> operator+(Rectangle<T> lhs, const Rectangle<T> &rhs)
	{
		Rectangle<T> ret{ lhs };
		ret += rhs;
		return ret;
	}

	// Equals
	template <typename T>
	inline bool operator==(const Rectangle<T> &lhs, const Rectangle<T> &rhs)
	{
		return (lhs.left == rhs.left && lhs.right == rhs.right && lhs.top == rhs.top && lhs.bottom == rhs.bottom);
	}

	template <typename T>
	inline bool operator!=(const Rectangle<T> &lhs, const Rectangle<T> &rhs)
	{
		return !(lhs==rhs);
	}

	using RectangleI = Rectangle<int>;
	using RectangleF = Rectangle<float>;

	template <typename T>
	class RectangleOriented final
	{
	public:
		// Cannot describe as left, top etc. as may not be
		Vector<T> corners[4];

		RectangleOriented() = default;

		// Individually
		RectangleOriented(Vector<T> c1, Vector<T> c2, Vector<T> c3, Vector<T> c4)
		{
			corners[0] = c1;  corners[1] = c2, corners[2] = c3, corners[3] = c4;
		}
		
		// Construct from aligned rect
		template <typename U>
		RectangleOriented(const Rectangle<U> &rect) : corners
		{
			{ (T)rect.left, (T)rect.top },{ (T)rect.right, (T)rect.top } ,
			{ (T)rect.right, (T)rect.bottom },{ (T)rect.left, (T)rect.bottom }
		} {}

		// Scales around origin
		void Scale(const VectorF& scale, Vector<T> origin)
		{
			Translate(-origin);

			for (int i = 0; i < 4; i++)
				corners[i] = Vector<T>(corners[i] * scale);

			Translate(origin);
		}

		// cos and sine precalculated to feed in:
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

		// Rotates. Note this rotates corners as-is and does not work around centre
		void Rotate(float radians)
		{
			Rotate(cos(radians), sin(radians));
		}		

		// Rotate around a point
		void Rotate(float radians, const Vector<T>& origin)
		{
			Translate(-origin);
			Rotate(radians);
			Translate(origin);
		}

		// Uses centre as point of rotation
		void RotateAroundCentre(float radians)
		{
			Rotate(radians, GetCentre());
		}

		// Translate rectangle
		void Translate(T dx, T dy)
		{
			for (int i = 0; i < 4; i++)
			{
				corners[i].x += dx;
				corners[i].y += dy;
			}
		}

		// Translate rectangle using a point
		void Translate(Vector<T> d)
		{
			Translate(d.x, d.y);
		}

		RectangleOriented<T> Translated(T dx, T dy) const
		{
			Rectangle<T> ret{ *this };
			ret.Translate(dx, dy);
			return ret;
		}

		// Translate rectangle using a point
		RectangleOriented<T> Translated(Vector<T> d) const
		{
			Rectangle<T> ret{ *this };
			ret.Translate(d.x, d.y);
			return ret;
		}

		// Note: assumes corners are defined clockwise
		Vector<T> GetCentre() const
		{
			return corners[0] + ((corners[2] - corners[0]) / 2);
		}

		// Radius of a circle around this rectangle
		float GetRadius() const {
			return ((VectorF)GetCentre() - (VectorF)corners[0]).Length();
		}

		Rectangle<T> GetEnclosingAABB() const noexcept
		{
			return Rectangle<T>(
				T(std::min(corners[0].x, std::min(corners[1].x, std::min(corners[2].x, corners[3].x)))),
				T(std::max(corners[0].x, std::max(corners[1].x, std::max(corners[2].x, corners[3].x)))),
				T(std::min(corners[0].y, std::min(corners[1].y, std::min(corners[2].y, corners[3].y)))),
				T(std::max(corners[0].y, std::max(corners[1].y, std::max(corners[2].y, corners[3].y))))
				);
		}

		// Assumes points are defined clockwise this is corner 2 - corner 1
		T Width() const
		{
			return (T)(((VectorF)(corners[1] - corners[0])).Length());
		}

		// Assumes points are defined clockwise this is corner 3 - corner 0
		T Height() const
		{
			return (T)(((VectorF)(corners[3] - corners[0])).Length());
		}

		// To avoid linked dependencies now returning points
		std::vector<Vector<T>> GetOutline() const {
			//return std::vector<Vector<T>>(corners, corners + 4); // Vector construction from pointers as iterators - cool :)
			// but always 4 so better:
			return std::vector<Vector<T>>{corners[0], corners[1], corners[2], corners[3]};
		}

		// Does this rectangle completely contain the point
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

		// vector float * this rectangle's corners (scale)
		RectangleOriented<T>& operator*=(const VectorF &mult)
		{
			for (auto &c : corners)
				c = (VectorF)c * mult;

			return *this;
		}

		// Emplicit conversion as quite a bit of work
		template <typename U>
		explicit operator RectangleOriented<U>() const 
		{ 
			RectangleOriented<U> newRect;
			for (int i = 0; i < 4; i++)
				newRect.corners[i] = (Vector<U>)corners[i];
			return newRect;
		}
	};

	using RectangleOrientedF = RectangleOriented<float>;
}
