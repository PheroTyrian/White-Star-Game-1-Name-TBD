#pragma once

//#include <iostream>

namespace HAPISPACE {

	/*
		Vector type
		A basic vector or point type
		Performance wise is best passed by value not const reference
	*/
	template <typename T>
	class Vector final
	{
	public:
		T x{ 0 };
		T y{ 0 };

		// Construct x and y values
		Vector() noexcept {}
		Vector(T bothIn) noexcept : x(bothIn), y(bothIn) {}
		Vector(T xin, T yin) noexcept : x(xin), y(yin) {}

		// Converts U to T
		template <typename U>
		explicit Vector(const Vector<U>& vector) :
			x(static_cast<T>(vector.x)), y(static_cast<T>(vector.y)) {}

		// Add rhs to this
		Vector& operator+=(Vector rhs)
		{
			x += rhs.x;
			y += rhs.y;
			return *this;
		}

		// Subtract rhs from this
		Vector& operator-=(Vector rhs)
		{
			x -= rhs.x;
			y -= rhs.y;
			return *this;
		}

		// Scale this 
		Vector& operator*=(T mult)
		{
			x *= mult;
			y *= mult;
			return *this;
		}

		// Scale this 
		Vector& operator*=(Vector<T> mult)
		{
			x *= mult.x;
			y *= mult.y;
			return *this;
		}

		// Divide this
		Vector& operator/=(T div)
		{
			x /= div;
			y /= div;
			return *this;
		}

		// Negate this
		Vector operator-() const
		{
			return Vector{ -x,-y };
		}

		// Returns cosine of the angle between this and other
		float Dot(const Vector& other) const
		{			
			return x * other.x + y * other.y;
		}

		// Calculates the 2D cross product of 2 vectors 
		float Cross(const Vector& other) const
		{
			return (x * other.y) - (y * other.x);
		}

		// Distance to other point vector from this point vector squared
		T DistanceBetweenSquared(const Vector<T> &other) const
		{
			return (other.x - x)*(other.x - x) + (other.y - y)*(other.y - y);
		}

		// Distance to other point vector from this point vector
		float DistanceBetween(const Vector &other) const
		{
			return sqrt((float)DistanceBetweenSquared(other));
		}

		// Manhattan distance to other point vector from this point vector
		int ManhattanDistanceBetween(const Vector &p2) const
		{
			return (int)(abs(x - p2.x) + abs(y - p2.y));
		}

		float LengthSquared() { return (float)(x*x + y*y); }
		float Length() { return sqrt((float)(x*x + y*y)); }

		// Scale this so vector length is newLength
		void ScaleToLength(T newLength)
		{
			float len{ Length() };
			if (len != 0)
			{
				x = T((float)x * newLength / len);
				y = T((float)y * newLength / len);
			}
		}

		// Returns scaled so vector length is newLength
		Vector<T> ScaledToLength(T newLength)
		{
			Vector<T> ret{ *this };
			ret.ScaledToLength(newLength);
			return ret;
		}

		static Vector<T> Zero() { return Vector<T>{0, 0}; }
		bool IsZero() const { return (x == 0 && y == 0); }

		// only relevant for a float vector
		void Normalize()
		{
			float length{ Length() };
			if (length != 0)
			{
				x = T((float)x / length);
				y = T((float)y / length);
			}
		}

		// Normal of this vector
		// does not change this
		Vector<float> Normalized() const
		{
			Vector<float> ret((Vector<float>)*this);
			ret.Normalize();
			return ret;
		}

		
		// Rotate by angle in radians
		// Does not change this
		Vector Rotated(float angle) const
		{
			Vector ret;

			ret.x = x * cos(angle) - y * sin(angle);
			ret.y = y * cos(angle) + x * sin(angle);

			return ret;
		}

		// Rotate by angle in radians
		void Rotate(float angle)
		{
			T oldX{ x };
			x = x * cos(angle) - y * sin(angle);
			y = y * cos(angle) + oldX * sin(angle);
		}

		// Creates a random vector with x and y values between min and max
		static Vector Random(T minX, T maxX, T minY, T maxY)
		{
			return Vector(static_cast<T>(minX + rand() % (int)(maxX - minX)),
				static_cast<T>(minY + rand() % (int)(maxY - minY)));
		};

		// Creates a random vector from 0 to max
		static Vector Random(T maxX, T maxY)
		{
			return Random(0, maxX, 0, maxY);
		};

		// Creates a random directional vector (length is unity)
		static Vector RandomDirection()
		{
			Vector res{ (rand() % 100)-50.0f, (rand() % 100)-50.0f };
			res.Normalize();
			return res;
		};
		
		// implicit conversion
		// Note: it is worth making this explicit sometimes to detect any uneeded casts automatically happening	(it happens a lot)	
		template <typename U> // explicit
			operator Vector<U>() const { return Vector<U>((U)x, U(y)); }

		template <typename T>
		friend std::ostream& operator<<(std::ostream& os, const Vector<T>& p);

		std::string ToString() const { std::stringstream str; str << *this; return str.str(); }

		static Vector<T> Up() { return Vector<T>{0, -1}; }
		static Vector<T> Down() { return Vector<T>{0, 1}; }
		static Vector<T> Left() { return Vector<T>{-1, 0}; }
		static Vector<T> Right() { return Vector<T>{1, 0}; }

		// No need for assignment operator or copy or move since default bitwise ones work fine (its just 8 bytes normally)
	};
	
	// Can be used with cout to output values to output pane and log
	template <typename T>
	inline std::ostream& operator<<(std::ostream& os, const Vector<T>& p)
	{
		os << "X: " << p.x << " Y: " << p.y;
		return os;
	}

	// Equals
	template <typename T>
	inline bool operator==(const Vector<T>& lhs,const Vector<T>& rhs)
	{ 
		return (lhs.x == rhs.x && lhs.y == rhs.y); 
	}
	
	// Does not equal
	template <typename T>
	inline bool operator!=(Vector<T> lhs, Vector<T> rhs)
	{
		return !operator==(lhs, rhs); 
	}

	// Other comparison operators like <,>,<=,>= are meaningless when dealing with vectors
	// Unary operators are also meaningless

	// Add two vector
	template <typename T>
	inline Vector<T> operator+(Vector<T> lhs, const Vector<T> &rhs)
	{
		lhs += rhs;
		return lhs;
	}	

	// Subtract two vector
	template <typename T>
	inline Vector<T> operator-(Vector<T> lhs, const Vector<T> &rhs)
	{
		lhs -= rhs;
		return lhs;
	}	

	template <typename T, typename U>
	inline Vector<T> operator*(const U &s, Vector<T> rhs)
	{
		return rhs *= (T)s;
	}

	template <typename T, typename U>
	inline Vector<T> operator*(Vector<T> lhs, const U &s)
	{
		return lhs *= (T)s;
	}

	template <typename T, typename U>
	inline Vector<T> operator*(Vector<T> lhs, const Vector<U> &s)
	{
		return lhs *= s;
	}

	// divide vector
	template <typename T,typename U>
	inline Vector<T> operator/(Vector<T> lhs, const U &s)
	{
		return lhs /= (T)s;
	}

	// Helpers
	using VectorI = Vector<int>;
	using VectorF = Vector<float>;

	// Reflect off surface described by normal, vectors must be floats
	inline VectorF Reflect(const VectorF& v, const VectorF& normal)
	{
		VectorF res = normal * -2 * v.Dot(normal) + v;
		return res;
	}
}