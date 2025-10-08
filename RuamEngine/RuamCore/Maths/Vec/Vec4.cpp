#include "Vec4.h"

Vec4::Vec4(float xP, float yP, float zP, float wP)
    : x(xP), y(yP), z(zP), w(wP) {
}

Vec4::Vec4(const glm::vec4& v) : Vec4(v.x, v.y, v.z, v.w) {}

Vec4 Vec4::operator+(Vec4 other) const {
    return Vec4(x + other.x, y + other.y, z + other.z, w + other.w);
}

Vec4 Vec4::operator-(Vec4 other) const {
    return Vec4(x - other.x, y - other.y, z - other.z, w - other.w);
}

Vec4 Vec4::operator*(Vec4 other) const {
    return Vec4(x * other.x, y * other.y, z * other.z, w * other.w);
}

Vec4 Vec4::operator/(Vec4 other) const {
    return Vec4(x / other.x, y / other.y, z / other.z, w / other.w);
}

Vec4 Vec4::operator/(float number) const {
    return Vec4(x / number, y / number, z / number, w / number);
}

float Vec4::Magnitude() const {
    return std::sqrt(x * x + y * y + z * z + w * w);
}

Vec4 Vec4::Normalized() const {
    float mag = Magnitude();
    return (mag != 0) ? Vec4(x / mag, y / mag, z / mag, w / mag) : Zero();
}

Vec4 Vec4::To(Vec4 target) const {
    return target - *this;
}

float Vec4::DotProduct(Vec4 other) const {
    Vec4 a = Normalized();
    Vec4 b = other.Normalized();
    return static_cast<float>(a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w);
}

// Common directions
Vec4 Vec4::Right() { return Vec4(1, 0, 0, 0); }
Vec4 Vec4::Left() { return Vec4(-1, 0, 0, 0); }
Vec4 Vec4::Up() { return Vec4(0, 1, 0, 0); }
Vec4 Vec4::Down() { return Vec4(0, -1, 0, 0); }
Vec4 Vec4::Forward() { return Vec4(0, 0, 1, 0); }
Vec4 Vec4::Backward() { return Vec4(0, 0, -1, 0); }
Vec4 Vec4::In() { return Vec4(0, 0, 0, 1); }
Vec4 Vec4::Out() { return Vec4(0, 0, 0, -1); }
Vec4 Vec4::Zero() { return Vec4(0, 0, 0, 0); }
Vec4 Vec4::One() { return Vec4(1, 1, 1, 1); }

// Compound assignment operators
Vec4& Vec4::operator=(Vec4 other) {
    x = other.x;
    y = other.y;
    z = other.z;
    w = other.w;
    return *this;
}

Vec4& Vec4::operator+=(Vec4 other) {
    x += other.x;
    y += other.y;
    z += other.z;
    w += other.w;
    return *this;
}

Vec4& Vec4::operator-=(Vec4 other) {
    x -= other.x;
    y -= other.y;
    z -= other.z;
    w -= other.w;
    return *this;
}

Vec4& Vec4::operator*=(Vec4 other) {
    x *= other.x;
    y *= other.y;
    z *= other.z;
    w *= other.w;
    return *this;
}

Vec4& Vec4::operator/=(Vec4 other) {
    x /= other.x;
    y /= other.y;
    z /= other.z;
    w /= other.w;
    return *this;
}

Vec4& Vec4::operator*=(float number) {
    x *= number;
    y *= number;
    z *= number;
    w *= number;
    return *this;
}

// Output
std::ostream& operator<<(std::ostream& os, const Vec4& v) {
    os << "(" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ")";
    return os;
}
