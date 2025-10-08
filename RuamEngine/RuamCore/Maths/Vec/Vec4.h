#pragma once

#include <cmath>
#include <iostream>
#include "glm/glm.hpp"

struct Vec4
{
    float x;
    float y;
    float z;
    float w;

    // Constructor
    Vec4(float xP = 0.0f, float yP = 0.0f, float zP = 0.0f, float wP = 0.0f);
	Vec4(const glm::vec4& other);

    // Basic operators
    Vec4 operator+(Vec4 other) const;
    Vec4 operator-(Vec4 other) const;
    Vec4 operator*(Vec4 other) const;
    Vec4 operator/(Vec4 other) const;
    Vec4 operator/(float number) const;

    // Vector operations
    float Magnitude() const;
    Vec4 Normalized() const;
    Vec4 To(Vec4 target) const;
    float DotProduct(Vec4 other) const;

    // Common directions
    static Vec4 Right();     // (1, 0, 0, 0)
    static Vec4 Left();      // (-1, 0, 0, 0)
    static Vec4 Up();        // (0, 1, 0, 0)
    static Vec4 Down();      // (0, -1, 0, 0)
    static Vec4 Forward();   // (0, 0, 1, 0)
    static Vec4 Backward();  // (0, 0, -1, 0)
    static Vec4 In();        // (0, 0, 0, 1)
    static Vec4 Out();       // (0, 0, 0, -1)
    static Vec4 Zero();      // (0, 0, 0, 0)
    static Vec4 One();       // (1, 1, 1, 1)

    // Compound assignment operators
    Vec4& operator=(Vec4 other);
    Vec4& operator+=(Vec4 other);
    Vec4& operator-=(Vec4 other);
    Vec4& operator*=(Vec4 other);
    Vec4& operator/=(Vec4 other);
    Vec4& operator*=(float number);
	operator glm::vec4() {
		return glm::vec4(x, y, z, w);
	}
    // Output
    friend std::ostream& operator<<(std::ostream& os, const Vec4& v);
};
