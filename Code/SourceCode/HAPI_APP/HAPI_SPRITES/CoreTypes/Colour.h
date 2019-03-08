#pragma once

namespace HAPISPACE {

	// BYTE colour
	class Colour255 final
	{
	public:
		BYTE red{ 255 };
		BYTE green{ 255 };
		BYTE blue{ 255 };
		BYTE alpha{ 255 };

		// Constructor 1 - Colour defaults to white	
		Colour255()  noexcept = default;

		// Constructor 2 - Colour channels passed in, alpha defaults to 255 (opaque) if not provided
		Colour255(BYTE r, BYTE g, BYTE b, BYTE a = 255) noexcept : red(r), green(g), blue(b), alpha(a) {}

		// Constructor 3 - gray
		Colour255(BYTE gray, BYTE a = 255)  noexcept : red(gray), green(gray), blue(gray), alpha(a) {}

		// Helpers
		static Colour255 BLACK;
		static Colour255 WHITE;
		static Colour255 RED;
		static Colour255 GREEN;
		static Colour255 BLUE;
		static Colour255 YELLOW;
		static Colour255 CYAN;
		static Colour255 MAGENTA;
		static Colour255 HORRID_PINK;
		static Colour255 ZERO; // 0,0,0,0

		// Returns a random colour
		static Colour255 Random() { return Colour255(rand() % 255, rand() % 255, rand() % 255); }

		friend std::ostream& operator<<(std::ostream& os, const Colour255& col);

		std::string ToString() const { std::stringstream str; str << *this; return str.str(); }

		// Cannot be done via a cast operator unfortunately
		const HAPI_TColour& AsHAPI_TColour() const
		{
			return *(const HAPI_TColour*)this;
		}

		// Modulate. Note: must be accurate so no bit shift used
		Colour255& operator*=(const Colour255 &rhs)
		{
			red = (red * rhs.red) / 255;
			green = (green * rhs.green) / 255;
			blue = (blue * rhs.blue) / 255;
			alpha = (alpha * rhs.alpha) / 255;
			return *this;
		}

		Colour255& operator=(HAPI_TColour &rhs)
		{
			// Byte copy
			*this = *(const Colour255*)&rhs;
			return *this;
		}
	};

	// Can be used with cout to output colour to output pane and log
	inline std::ostream& operator<<(std::ostream& os, const Colour255& col)
	{
		os << "R: " + std::to_string(col.red) +
			" G: " + std::to_string(col.green) +
			" B: " + std::to_string(col.blue) +
			" A: " + std::to_string(col.alpha);
		return os;
	}

	// Equivelance operator
	inline bool operator==(const Colour255& lhs, const Colour255& rhs)
	{
		return (*(DWORD*)&lhs == *(DWORD*)&rhs);
	}

	inline bool operator!=(const Colour255& lhs, const Colour255& rhs)
	{
		return !(lhs == rhs);
	}

	// Subtract one colour from the other handling wrap around
	inline Colour255 operator- (const Colour255& lhs, const Colour255& rhs)
	{
		// Allow loop around
		int r = lhs.red - rhs.red;
		if (r < 0) r += 255;
		int g = lhs.green - rhs.green;
		if (g < 0) g += 255;
		int b = lhs.blue - rhs.blue;
		if (b < 0) b += 255;
		int a = lhs.alpha - rhs.alpha;
		if (a < 0) a += 255;
		return Colour255((BYTE)r, (BYTE)g, (BYTE)b, (BYTE)a);
	}

	// Modulate one colour by another
	inline Colour255 operator* (const Colour255& lhs, Colour255 rhs)
	{
		Colour255 sourceMod{ lhs };
		sourceMod.red = (lhs.red * rhs.red) >> 8;
		sourceMod.green = (lhs.green * rhs.green) >> 8;
		sourceMod.blue = (lhs.blue * rhs.blue) >> 8;
		sourceMod.alpha = (lhs.alpha * rhs.alpha) >> 8;
		return sourceMod;
	}

	// A simple floating point colour type
	class ColourF
	{
	public:
		float red{ 1.f };
		float green{ 1.f };
		float blue{ 1.f };
		float alpha{ 1.f };

		// Constructor 1 - Colour defaults to white	
		ColourF() noexcept = default;

		// Constructor 2 - Colours passed in, alpha defaults to 1 (opaque) if not provided
		ColourF(float r, float g, float b, float a = 1.0f) noexcept : red(r), green(g), blue(b), alpha(a) {}

		// Constructor 3 - gray
		ColourF(float gray, float a = 1.0f) noexcept : red(gray), green(gray), blue(gray), alpha(a) {}

		// Constructor 4 - from a Colour255
		ColourF(Colour255 col) noexcept : red{ col.red / 255.0f }, green{ col.green / 255.0f }, blue{ col.blue / 255.0f }, alpha{ col.alpha / 255.0f } {}

		// Add a Colour to this
		ColourF& operator+=(ColourF rhs)
		{
			red += rhs.red;
			green += rhs.green;
			blue += rhs.blue;
			alpha += rhs.alpha;
			return *this;
		}

		// Subtract a Colour from this
		ColourF& operator-=(ColourF rhs)
		{
			red -= rhs.red;
			green -= rhs.green;
			blue -= rhs.blue;
			alpha -= rhs.alpha;
			return *this;
		}

		ColourF& operator/=(float rhs)
		{
			if (rhs == 0.0f)
				return *this; // should be an error really
			red /= rhs;
			green /= rhs;
			blue /= rhs;
			alpha /= rhs;
			return *this;
		}

		// Helpers
		static ColourF BLACK;
		static ColourF WHITE;
		static ColourF RED;
		static ColourF GREEN;
		static ColourF BLUE;
		static ColourF YELLOW;
		static ColourF CYAN;
		static ColourF MAGENTA;
		static ColourF ZERO; //0,0,0,0
		static ColourF HORRID_PINK;

		static ColourF Random() { return Colour255(rand() % 255, rand() % 255, rand() % 255); }

		friend std::ostream& operator<<(std::ostream& os, const ColourF& col);

		std::string ToString() const { std::stringstream str; str << *this; return str.str(); }

		bool IsWhite() const { return (red == 1.0f && green == 1.0f && blue == 1.0f && alpha == 1.0f); }

		// made explicit as it is not a straight forward operation so user needs to know
		explicit operator Colour255() const
		{ 
			// May lose data if values outside range
			return Colour255((BYTE)(red*255.0f), (BYTE)(green*255.0f), (BYTE)(blue*255.0f), (BYTE)(alpha*255.0f));
		}	
	};

	// Can be used with cout to output colour to output pane and log
	inline std::ostream& operator<<(std::ostream& os, const ColourF& col)
	{
		os << "R: " + std::to_string(col.red) +
			" G: " + std::to_string(col.green) +
			" B: " + std::to_string(col.blue) +
			" A: " + std::to_string(col.alpha);
		return os;
	}

	inline bool operator==(const ColourF& lhs, const ColourF& rhs)
	{
		return (lhs.red == rhs.red && lhs.green == rhs.green && lhs.blue == rhs.blue && lhs.alpha == rhs.alpha);
	}

	inline bool operator!=(const ColourF& lhs, const ColourF& rhs)
	{
		return !(lhs == rhs);
	}

	inline ColourF operator- (ColourF lhs, const ColourF& rhs)
	{
		lhs -= rhs;
		return lhs;
	}

	inline ColourF operator+ (ColourF lhs, const ColourF& rhs)
	{
		lhs += rhs;
		return lhs;
	}

	// Modulate one colour by another per channel
	inline ColourF operator* (const ColourF& lhs, const ColourF &rhs)
	{
		return ColourF(lhs.red * rhs.red, lhs.green * rhs.green, lhs.blue * rhs.blue, lhs.alpha * rhs.alpha);
	}
}