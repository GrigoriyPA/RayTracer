#pragma once

#include <algorithm>
#include <vector>


class Thor : public SceneObject {
	ld w, r;
	Vect3 o, d;

public:
	Thor(ld w, ld r, Vect3 o, Vect3 d, ColourRGB color, ld spec = -1, ld refl = 0) {
		this->w = w;
		this->r = r;
		this->o = o;
		this->d = d.normalize();
		this->color = color;
		this->spec = spec;
		this->refl = refl;
	}

	std::vector < ld > intersect_line(Vect3 p, Vect3 v) {
		ld t1 = (p - o).length_sqr() - w * w - r * r;
		ld t2 = v * (p - o);
		ld t3 = v * d;
		ld t4 = d * (p - o);

		ld k0 = v.length_sqr() * v.length_sqr() / (4 * w * w);
		ld k1 = t2 * v.length_sqr() / (w * w);
		ld k2 = v.length_sqr() * t1 / (2 * w * w) + t2 * t2 / (w * w) + t3 * t3;
		ld k3 = t1 * t2 / (w * w) + 2 * t3 * t4;
		ld k4 = t1 * t1 / (4 * w * w) + t4 * t4 - r * r;

		std::vector < ld > t = sol_eqt(k1 / k0, k2 / k0, k3 / k0, k4 / k0);
		std::sort(t.begin(), t.end());

		return t;
	}

	Vect3 normal(Vect3 p, Vect3 p0) {
		Vect3 v = p - o;
		Vect3 t = (v - d * (v * d)).normalize() * w;
		return (v - t).normalize();
	}

	bool on_surface(Vect3 p) {
		Vect3 v = p - o;
		Vect3 t = (v - d * (v * d)).normalize() * w;
		return abs((v - t).length() - r) <= eps;
	}

	bool is_inside(Vect3 p) {
		Vect3 v = p - o;
		Vect3 t = (v - d * (v * d)).normalize() * w;
		return (v - t).length() <= r;
	}
};
