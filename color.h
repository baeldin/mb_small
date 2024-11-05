#pragma once
#include <algorithm>
#include <cmath>
#include <iostream>

inline float clampRGB(const float x) {return std::min(1.f, std::max(x, 0.f)); }

constexpr float a = 1.f/12.92f;
constexpr float b = 1.f/1.055f;
inline float sRGBtoLinear(const float x) {
    return (clampRGB(x) <= 0.04045f) ? clampRGB(x) * a : std::pow( (clampRGB(x) + 0.055f) * b, 2.4f);
}

constexpr float invGamma = 1.f/2.4f;
inline float linearToSRGB(const float x) {
    return (clampRGB(x) <= 0.0031308f) ? clampRGB(x) * 12.92f : 1.055f * std::pow(clampRGB(x), invGamma) - 0.055f;
}

// linear take sRGB values and convert to linear
struct color
{
    float r, g, b;
    color() : r(0), g(0), b(0) {}
    color(const float x) : r(x), g(x), b(x) {}
    color(const float r_, const float g_, const float b_) : r(r_), g(g_), b(b_) {}
    ~color() {}
    color& operator=(const color& rhs)
    {
        r = rhs.r; g = rhs.g; b = rhs.b;
        return *this;
    }
    color operator+(const color& rhs) const {
        return color(r + rhs.r, g + rhs.g, b + rhs.b);
    }
    color operator*(const float rhs) const {
        return color(r*rhs, g*rhs, b*rhs) ;
    }
};

// float * color
color operator*(const float lhs, const color& rhs) {
    return color(lhs * rhs.r, lhs * rhs.g, lhs * rhs.b);
}

// overload << operator
std::ostream& operator<<(std::ostream& os, const color& c) {
	os << "(" << c.r << ", " << c.g << ", " << c.b << ")";
	return os;
}

inline color sRGBtoLinear(const color& col) {
    return color(sRGBtoLinear(col.r), sRGBtoLinear(col.g), sRGBtoLinear(col.b));
}

inline color linearToSRGB(const color& col) {
    return color(linearToSRGB(col.r), linearToSRGB(col.g), linearToSRGB(col.b));
}