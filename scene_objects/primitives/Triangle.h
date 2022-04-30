#pragma once

#include <vector>


class Triangle : public SceneObject {
	Vect3 p1, p2, p3, n;
	Plane pl;

public:
	Triangle(Vect3 p1, Vect3 p2, Vect3 p3, ColourRGB color, ld spec = -1, ld refl = 0) {
		this->p1 = p1;
		this->p2 = p2;
		this->p3 = p3;
		n = ((p3 - p1) ^ (p3 - p2)).normalize();
		pl = Plane(p1, n);
		this->color = color;
		this->spec = spec;
		this->refl = refl;
	}

	std::vector < ld > intersect_line(Vect3 p, Vect3 d) {
		ld t = pl.intersect_line(p, d);

		if (on_surface(p + t * d))
			return { t, t };

		return {};
	}

	Vect3 normal(Vect3 p, Vect3 p0) {
		if ((p0 - p) * n < 0)
			return -n;
		return n;
	}

	bool on_surface(Vect3 p) {
		return (p - p1).in_angle(p2 - p1, p3 - p1) && (p - p2).in_angle(p1 - p2, p3 - p2);
	}

	bool is_inside(Vect3 p) {
		return (p - p1).in_angle(p2 - p1, p3 - p1) && (p - p2).in_angle(p1 - p2, p3 - p2);
	}
};
