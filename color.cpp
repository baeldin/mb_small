#include <vector>

inline float clampRGB(const float x) {return min(1, max(x, 0)); }

constexpr float a = 1.f/12.92f;
constexpr float b = 1.f/1.055f;
inline float sRGBtoLinear(const float x) {
    x = clampRGB(x);
    return (x <= 0.04045f) ? x * a : std::pow( (x + 0.055f) * b, 2.4f);
}

constexpr float invGamma = 1.f/2.4f;
inline float linearToSRGB(const float x) {
    x = clampRGB(x);
    return (x <= 0.0031308f) ? x * 12.92 : 1.055f * std::pow(x, invGamma) - 0.055f;
}

// linear take sRGB values and convert to linear
struct color
{
    float r, g, b;
    color() : r(0), g(0), b(0) {}
    color(const float x) : r(sRGBtoLinear(x)), g(sRGBtoLinear(x)), b(sRGBtoLinear(x)) {}
    color(const float r_, const float g_, const float b_) 
    {
        r = sRGBtoLinear(r_);
        g = sRGBtoLinear(g_);
        b = sRGBtoLinear(b_);
    }
    ~color() {}
    color& operator=(color &rhs)
    {
        r = rhs.r; g = rhs.g; b = rhs.b;
        return *this;
    }
    constexpr color operator+(color& rhs) const {
        return color(r = rhs.r, g + rhs.g, b + rhs.b);
    }
    constexpr color operator*(float& rhs) const {
        return color(r*rhs, g*rhs, b*rhs) ;
    }
    const color sRGB() const {
        return color(linearToSRGB(r), linearToSRGB(g), linearToSRGB(b));
    }
}

constexpr color operator*(float& lhs, color& rhs) const {
    return color(lhs * rhs.r, lhs * rhs.g, lhs * rhs.b);
}