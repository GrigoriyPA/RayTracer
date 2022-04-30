#pragma once

#include <vector>


class SceneObject {
public:
	ld eps = 0.00000001, inf = 1e18, spec, refl;
	ColourRGB color;

	virtual std::vector < ld > intersect_line(Vect3 p, Vect3 d) = 0;

	virtual Vect3 normal(Vect3 p, Vect3 p0) = 0;

	virtual bool on_surface(Vect3 p) = 0;

	virtual bool is_inside(Vect3 p) = 0;

	ld intersect_ray(Vect3 p, Vect3 d) {
		std::vector < ld > t = intersect_line(p, d);
		for (ld cur : t) {
			if (cur >= 0)
				return cur;
		}
		return -1;
	}
};
