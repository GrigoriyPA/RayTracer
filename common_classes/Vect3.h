#pragma once

#include <algorithm>
#include <iostream>


class Vect3 {
	ld inf = 1e18, eps = 0.00001;

public:
	ld x, y, z;

	Vect3(ld x = 0, ld y = 0, ld z = 0) {
		this->x = x;
		this->y = y;
		this->z = z;
	}

	bool operator ==(Vect3 other) {
		return x == other.x && y == other.y && z == other.z;
	}

	bool operator !=(Vect3 other) {
		return x != other.x || y != other.y || z != other.z;
	}

	void operator +=(Vect3 other) {
		x += other.x;
		y += other.y;
		z += other.z;
	}

	void operator -=(Vect3 other) {
		x -= other.x;
		y -= other.y;
		z -= other.z;
	}

	void operator *=(ld other) {
		x *= other;
		y *= other;
		z *= other;
	}

	void operator /=(ld other) {
		if (other != 0) {
			x /= other;
			y /= other;
			z /= other;
		}
		else {
			x = inf;
			y = inf;
			z = inf;
		}
	}

	Vect3 operator -() {
		return Vect3(-x, -y, -z);
	}

	Vect3 operator +(Vect3 other) {
		return Vect3(x + other.x, y + other.y, z + other.z);
	}

	Vect3 operator -(Vect3 other) {
		return Vect3(x - other.x, y - other.y, z - other.z);
	}

	Vect3 operator *(ld other) {
		return Vect3(x * other, y * other, z * other);
	}

	ld operator *(Vect3 other) {
		return x * other.x + y * other.y + z * other.z;
	}

	Vect3 operator ^(Vect3 other) {
		return Vect3(y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x);
	}

	Vect3 operator /(ld other) {
		if (other != 0)
			return Vect3(x / other, y / other, z / other);
		return Vect3(inf, inf, inf);
	}

	ld length_sqr() {
		return *this * *this;
	}

	ld length() {
		return sqrt(*this * *this);
	}

	Vect3 normalize() {
		return *this / (*this).length();
	}

	ld cos_angle(Vect3 V) {
		return (*this * V) / ((*this).length() * V.length());
	}

	ld sin_angle(Vect3 V) {
		return ((*this ^ V).length()) / ((*this).length() * V.length());
	}

	Vect3 reflect_vect(Vect3 N) {
		return N * (2 * (N * *this)) - *this;
	}

	Vect3 set_max(Vect3 V) {
		x = std::max(x, V.x);
		y = std::max(y, V.y);
		z = std::max(z, V.z);
		return *this;
	}

	Vect3 set_min(Vect3 V) {
		x = std::min(x, V.x);
		y = std::min(y, V.y);
		z = std::min(z, V.z);
		return *this;
	}

	bool in_angle(Vect3 a, Vect3 b) {
		return abs((a ^ *this).cos_angle(b ^ *this) + 1) < eps;
	}

	void print() {
		std::cout << '(' << x << ", " << y << ", " << z << ')';
	}
};

Vect3 operator *(ld x, Vect3 v) {
	return Vect3(v.x * x, v.y * x, v.z * x);
}
