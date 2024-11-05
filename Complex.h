#pragma once
#include <stdio.h>
#include <iostream>
#include <bitset>

using std::cout;

// make a double version only for now
class complex {
public:
	double x, y;
	// constructors
	complex() {}
	constexpr complex(double x_) : x(x_), y(0) {}
	constexpr complex(double x_, double y_) : x(x_), y(y_) {}
	// basic methods
	constexpr double cabs_squared() const { return x*x + y*y; }
	constexpr double cabs() const { return std::sqrt(this->cabs_squared()); }
	constexpr complex abs() const { return complex(std::abs(x), std::abs(y)); }
	constexpr complex flip() const { return complex(y, x); }
	constexpr complex conj() const { return complex(x, -y); }
	constexpr double angle() const { return std::atan2(y, x); }
	constexpr complex sqr() const { return complex(x*x - y*y, 2*x*y); }
	constexpr complex cube() const { return complex(x, y) * complex(x, y).sqr(); }
	// complex logarithm (principal value)
	constexpr complex log() const {
		return complex(std::log(this->cabs()), this->angle());
	}
	// complex exponential function (principal value)
	constexpr complex exp() const {
		return complex(std::cos(y), std::sin(y)) * std::exp(x); 
	}

	// OPERATORS
	complex& operator= (const complex& rhs) {
		x = rhs.x;
		y = rhs.y;
		return *this;
	}
	complex& operator+= (const complex& rhs) {
		x = x + rhs.x;
		y = y + rhs.y;
		return *this;
	}
	complex& operator-= (const complex& rhs) {
		x = x - rhs.x;
		y = y - rhs.y;
		return *this;
	}
	complex& operator *= (const complex& rhs) {
		x = x * rhs.x + y * rhs.y;
		y = y * rhs.x - x * rhs.y;
		return  *this;
	}
	constexpr complex operator+ (const double rhs) const   { return complex(x + rhs, y);                                   }
	constexpr complex operator+ (const complex& rhs) const { return complex(x + rhs.x, y + rhs.y);                         }
	constexpr complex operator- (const double rhs) const   { return complex(x - rhs, y);                                   }
	constexpr complex operator- (const complex& rhs) const { return complex(x - rhs.x, y - rhs.y);                         }
	constexpr complex operator* (const double rhs) const   { return complex(x * rhs, y * rhs);                             }
	constexpr complex operator* (const complex& rhs) const { return complex(x * rhs.x - y * rhs.y, x * rhs.y + y * rhs.x); }
	constexpr complex operator/ (const double rhs) const   { return complex(x / rhs, y / rhs);                             }
	constexpr complex operator/ (const complex& rhs) const {
		double den = rhs.cabs_squared();
		return complex(x * rhs.x + y * rhs.y, y * rhs.x - x * rhs.y) / den;
	}


};


// overload << operator
std::ostream& operator<<(std::ostream& os, const complex& z) {
	const char sign = (z.y < 0) ? '\0' : '+';
	os << z.x << sign << z.y << "i";
	return os;
}

// real + complex
template <typename T>
constexpr complex operator+ (const T lhs, const complex& rhs)
{
	return complex(rhs.x + lhs, rhs.y);
}

// real - complex
template <typename T>
constexpr complex operator- (const T lhs, const complex& rhs)
{
	return complex(lhs - rhs.x, -rhs.y);
}

// real * complex
template <typename T>
constexpr complex operator* (const T lhs, const complex& rhs)
{
	return complex(rhs.x * lhs, rhs.y * lhs);
}

// real / complex - use complex division
template <typename T>
constexpr complex operator/ (const T lhs, const complex& rhs)
{
	return complex(lhs) / rhs;
}

//exponentiation
// complex number raised to integer power
constexpr complex pow(const complex& z_, const int n)
{
	complex z = z_;
	uint16_t nAbs = std::abs(n);
	switch (nAbs)
	{
		case 0:                              return complex(1);
		case 1:                              return (n >= 0) ? z : 1/z;
		case 2: z = z.sqr();                 return (n >= 0) ? z : 1/z;
		case 3: z = z.sqr() * z;             return (n >= 0) ? z : 1/z;
		case 4: z = (z.sqr()).sqr();         return (n >= 0) ? z : 1/z;
		case 5: z = z.sqr().sqr() * z;       return (n >= 0) ? z : 1/z;
		case 6: z = (z.sqr() * z).sqr();     return (n >= 0) ? z : 1/z;
		case 7: z = (z.sqr() * z).sqr() * z; return (n >= 0) ? z : 1/z;
		case 8: z = z.sqr().sqr().sqr();     return (n >= 0) ? z : 1/z;
		default:
		{
			cout << "\nEXECUTING DEFAULT CASE\n";
			complex z2(1);
			cout << "nAbs = " << nAbs << " (binary " << std::bitset<8*sizeof(nAbs)>(nAbs) << ")\n";
			while (nAbs > 1)
			{
				if (nAbs & 1)
				{
				    z2 *= z;
				    cout << "last bit was 1, multiplying zRet *= z2, it is now " << z2 << "\n";
				}
				z = z.sqr();
				cout << "squaring z2, it is now " << z2 << ", incrementing shifting nAbs by 1 bit.\n";
				nAbs >>= 1;
				cout << "nAbs = " << nAbs << " (binary " << std::bitset<8*sizeof(nAbs)>(nAbs) << ")\n";
			}
			z *= z2;
			return (n >= 0) ? z : 1/z;	
		}
	}
}

//complex number raised to non-integer real power
constexpr complex pow(const complex&z, const double x) {
	double r = z.cabs_squared();
	double phi = z.angle();
	return complex(std::cos(x*phi), std::sin(x*phi)) * std::pow(r, x*0.5);
}