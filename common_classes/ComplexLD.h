#pragma once

#include <vector>
#include <cmath>


class ComplexLD {
	ld eps = 0.0000001, pi = acos(-1);

public:
	ld re, im;

	ComplexLD(ld re = 0, ld im = 0) {
		this->re = re;
		this->im = im;
	}

	ComplexLD operator +(ComplexLD v) {
		return ComplexLD(re + v.re, im + v.im);
	}

	ComplexLD operator -(ComplexLD v) {
		return ComplexLD(re - v.re, im - v.im);
	}

	ComplexLD operator -() {
		return ComplexLD(-re, -im);
	}

	std::vector < ComplexLD > root(int deg) {
		ld m = pow(re * re + im * im, (ld)1 / (2 * deg));
		if (m <= eps)
			return { ComplexLD(0, 0) };
		ld ang = atan2(im, re) / deg;

		std::vector < ComplexLD > res(deg);
		for (int i = 0; i < deg; i++, ang += 2 * pi / deg)
			res[i] = ComplexLD(cos(ang) * m, sin(ang) * m);

		return res;
	}
};