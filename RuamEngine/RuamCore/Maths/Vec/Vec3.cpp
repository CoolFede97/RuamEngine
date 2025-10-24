#include "Vec3.h"
#include <cmath> // Para std::sqrt
#include "Vec2.h"
#include "Vec4.h"

Vec3::Vec3(float xP, float yP, float zP) : x(xP), y(yP), z(zP) {}
Vec3::Vec3(const glm::vec3& v) : Vec3(v.x, v.y, v.z) {}

Vec3::Vec3(const Vec2& other) : Vec3(other.x, other.y, 0.0f) {}
Vec3::Vec3(const Vec4& other) : Vec3(other.x, other.y, other.z) {}

Vec3 Vec3::operator+(Vec3 other) const {
    return Vec3(x + other.x, y + other.y, z + other.z);
}

Vec3 Vec3::operator-(Vec3 other) const {
    return Vec3(x - other.x, y - other.y, z - other.z);
}

Vec3 Vec3::operator*(Vec3 other) const {
    return Vec3(x * other.x, y * other.y, z * other.z);
}

Vec3 Vec3::operator/(Vec3 other) const {
    return Vec3(x / other.x, y / other.y, z / other.z);
}

Vec3 Vec3::operator/(float number) const {
    return Vec3(x / number, y / number, z / number);
}

float Vec3::Magnitude() const {
    return std::sqrt(x * x + y * y + z * z);
}

Vec3 Vec3::Normalized() const {
    float mag = Magnitude();
    return (mag != 0) ? Vec3(x / mag, y / mag, z / mag) : Zero();
}

Vec3 Vec3::To(Vec3 target) const {
    return target - *this;
}

float Vec3::DotProduct(Vec3 other) const {
    Vec3 a = Normalized();
    Vec3 b = other.Normalized();
    return static_cast<float>(a.x * b.x + a.y * b.y + a.z * b.z);
}

Vec3 Vec3::CrossProduct(Vec3 other) const
{
    Vec3 a = Normalized();
    Vec3 b = other.Normalized();

    Vec3 cross;
    cross.x = a.y * b.z - a.z * b.y;
    cross.y = a.z * b.x - a.x * b.z;
    cross.z = a.x * b.y - a.y * b.x;
    return cross;
}

Vec3 Vec3::GetDirectionFromEuler(Vec3 eulerAngles)
{
    float pitch = eulerAngles.x;
    float yaw = eulerAngles.y;

    Vec3 direction;
    direction.x = cos(pitch) * cos(yaw);
    direction.y = sin(pitch);
    direction.z = cos(pitch) * sin(yaw);

	return direction.Normalized();
}

Vec3 Vec3::Right() { return Vec3(1, 0, 0); }
Vec3 Vec3::Left() { return Vec3(-1, 0, 0); }
Vec3 Vec3::Up() { return Vec3(0, 1, 0); }
Vec3 Vec3::Down() { return Vec3(0, -1, 0); }
Vec3 Vec3::Forward() { return Vec3(0, 0, 1); }
Vec3 Vec3::Backward() { return Vec3(0, 0, -1); }
Vec3 Vec3::Zero() { return Vec3(0, 0, 0); }
Vec3 Vec3::One() { return Vec3(1, 1, 1); }

Vec3& Vec3::operator=(Vec3 other) {
    x = other.x;
    y = other.y;
    z = other.z;
    return *this;
}

Vec3& Vec3::operator+=(Vec3 other) {
    x += other.x;
    y += other.y;
    z += other.z;
    return *this;
}

Vec3& Vec3::operator-=(Vec3 other) {
    x -= other.x;
    y -= other.y;
    z -= other.z;
    return *this;
}

Vec3& Vec3::operator*=(Vec3 other) {
    x *= other.x;
    y *= other.y;
    z *= other.z;
    return *this;
}

Vec3& Vec3::operator/=(Vec3 other) {
    x /= other.x;
    y /= other.y;
    z /= other.z;
    return *this;
}

Vec3& Vec3::operator*=(float number) {
    x *= number;
    y *= number;
    z *= number;
    return *this;
}



std::ostream& operator<<(std::ostream& os, const Vec3& v) {
    os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
    return os;
}