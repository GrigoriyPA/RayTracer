#pragma once

#include <algorithm>
#include <vector>


class Cylinder : public SceneObject {
	ld r, h;
	Vect3 o, d;
	Plane pl1, pl2;

public:
	Cylinder(ld r, ld h, Vect3 o, Vect3 d, ColourRGB color, ld spec = -1, ld refl = 0) {
		this->r = r;
		this->h = h;
		this->o = o;
		this->d = d.normalize();
		pl1 = Plane(o, -d);
		pl2 = Plane(o + this->d * h, d);
		this->color = color;
		this->spec = spec;
		this->refl = refl;
	}

	std::vector < ld > intersect_line(Vect3 p, Vect3 v) {
		std::vector < ld > res;
		ld t1 = pl1.intersect_line(p, v);
		ld t2 = pl2.intersect_line(p, v);
		if (on_surface(p + v * t1))
			res.push_back(t1);
		if (on_surface(p + v * t2))
			res.push_back(t2);

		Vect3 vb = p - o;
		ld av = d * v, ab = d * vb;

		ld a = v.length_sqr() - av * av;
		ld b = 2 * (vb * v - ab * av);
		ld c = vb.length_sqr() - ab * ab - r * r;

		ld des = b * b - 4 * a * c;

		if (des < 0)
			return res;

		des = sqrtl(des);
		ld t3 = (-b - des) / (2 * a);
		ld t4 = (-b + des) / (2 * a);
		if (on_surface(p + v * t3) && abs(t3 - t1) > eps && abs(t3 - t2) > eps)
			res.push_back(t3);
		if (on_surface(p + v * t4) && abs(t4 - t1) > eps && abs(t4 - t2) > eps)
			res.push_back(t4);

		std::sort(res.begin(), res.end());
		return res;
	}

	Vect3 normal(Vect3 p, Vect3 p0) {
		Vect3 v = p - o;
		ld dist = v * d;
		if (dist <= eps)
			return -d;
		if (dist >= h - eps)
			return d;
		return (v - d * (v * d)).normalize();
	}

	bool on_surface(Vect3 p) {
		Vect3 v = p - o;
		ld dist = v * d, cur_r = v.length_sqr() - dist * dist;
		return (abs(cur_r - r * r) <= eps && -eps <= dist && dist <= h + eps) || (cur_r < r * r && (abs(dist) <= eps || abs(dist - h) <= eps));
	}

	bool is_inside(Vect3 p) {
		Vect3 v = p - o;
		ld dist = v * d, cur_r = v.length_sqr() - dist * dist;
		return cur_r <= r * r && 0 <= dist && dist <= h;
	}
};
