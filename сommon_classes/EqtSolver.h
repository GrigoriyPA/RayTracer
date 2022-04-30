#pragma once

#include <vector>


std::vector < ld > sol_eqt(ld a, ld b, ld c) {
	ld eps = 0.000001;

	ld p = b - a * a / 3;
	ld q = c + 2 * a * a * a / 27 - a * b / 3;
	ComplexLD rt = ComplexLD(q * q / 4 + p * p * p / 27).root(2)[0];

	std::vector < ComplexLD > v1 = (rt - q / 2).root(3);
	std::vector < ComplexLD > v2 = (-rt - q / 2).root(3);
	std::vector < ld > res;
	for (ComplexLD el1 : v1) {
		for (ComplexLD el2 : v2) {
			if (abs((el1 + el2).im) <= eps)
				res.push_back((el1 + el2).re - a / 3);
		}
	}

	return res;
}


std::vector < ld > sol_eqt(ld a, ld b, ld c, ld d) {
	ld eps = 0.00000001;

	ld p = b - 3 * a * a / 8;
	ld q = a * a * a / 8 - a * b / 2 + c;
	ld r = -3 * a * a * a * a / 256 + a * a * b / 16 - c * a / 4 + d;
	ld s = sol_eqt(-p / 2, -r, r * p / 2 - q * q / 8)[0];

	if (abs(q) <= eps) {
		ld dis = p * p - 4 * r;
		if (dis < 0)
			return {};
		dis = sqrtl(dis);

		std::vector < ld > res;
		ld k1 = (-p + dis) / 2, k2 = (-p - dis) / 2;
		if (k1 >= 0) {
			k1 = sqrtl(k1);
			res.push_back(k1 - a / 4);
			res.push_back(-k1 - a / 4);
		}
		if (k2 >= 0) {
			k2 = sqrtl(k2);
			res.push_back(k2 - a / 4);
			res.push_back(-k2 - a / 4);
		}

		return res;
	}

	ld t = 2 * s - p;
	if (t < 0)
		return {};
	t = sqrtl(t);

	ld k1 = -t, k2 = q / (2 * t) + s, k3 = t, k4 = -q / (2 * t) + s;
	ld d1 = k1 * k1 - 4 * k2, d2 = k3 * k3 - 4 * k4;

	std::vector < ld > res;
	if (d1 >= 0) {
		d1 = sqrtl(d1);
		res.push_back((-k1 + d1) / 2 - a / 4);
		res.push_back((-k1 - d1) / 2 - a / 4);
	}
	if (d2 >= 0) {
		d2 = sqrtl(d2);
		res.push_back((-k3 + d2) / 2 - a / 4);
		res.push_back((-k3 - d2) / 2 - a / 4);
	}

	return res;
}
