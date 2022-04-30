#pragma once

#include <algorithm>


const ld C_MIN = 0, C_MAX = 255;


class ColourRGB {
public:
	ld r, g, b;

	ColourRGB(ld r = C_MAX, ld g = C_MAX, ld b = C_MAX) {
		this->r = r;
		this->g = g;
		this->b = b;
	}

	ColourRGB operator +(ColourRGB other) {
		return ColourRGB(r + other.r, g + other.g, b + other.b).ñheck();
	}

	ColourRGB operator *(ld other) {
		return ColourRGB(r * other, g * other, b * other).ñheck();
	}

	ColourRGB operator *(Vect3 other) {
		return ColourRGB(r * other.x, g * other.y, b * other.z).ñheck();
	}

	ColourRGB ñheck() {
		r = std::max(std::min(r, C_MAX), C_MIN);
		g = std::max(std::min(g, C_MAX), C_MIN);
		b = std::max(std::min(b, C_MAX), C_MIN);
		return *this;
	}
};

ColourRGB operator *(ld x, ColourRGB c) {
	return ColourRGB(c.r * x, c.g * x, c.b * x).ñheck();
}

ColourRGB operator *(Vect3 v, ColourRGB c) {
	return ColourRGB(c.r * v.x, c.g * v.y, c.b * v.z).ñheck();
}
