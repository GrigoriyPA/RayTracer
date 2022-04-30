#pragma once

#include <vector>


class Sphere : public SceneObject {
	ld r;
	Vect3 o;

public:
	Sphere(ld r, Vect3 o, ColourRGB color, ld spec = -1, ld refl = 0) {
		this->r = r;
		this->o = o;
		this->color = color;
		this->spec = spec;
		this->refl = refl;
	}

	std::vector < ld > intersect_line(Vect3 p, Vect3 d) {
		Vect3 s = p - o;

		ld a = d.length_sqr();
		ld b = s * d * 2;
		ld c = s.length_sqr() - r * r;

		ld des = b * b - 4 * a * c;

		if (des < 0)
			return {};

		des = sqrt(des);
		ld t1 = (-b - des) / (2 * a);
		ld t2 = (-b + des) / (2 * a);

		return { t1, t2 };
	}

	Vect3 normal(Vect3 p, Vect3 p0) {
		return (p - o).normalize();
	}

	bool on_surface(Vect3 p) {
		return abs((p - o).length() - r) <= eps;
	}

	bool is_inside(Vect3 p) {
		return (p - o).length() <= r;
	}
};
