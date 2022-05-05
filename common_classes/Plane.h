#pragma once


class Plane {
	ld eps = 0.000001, inf = 1e18;

public:
	ld d;
	Vect3 n;

	Plane() {
		d = 0;
	}

	Plane(Vect3 p, Vect3 n) {
		this->n = n.normalize();
		d = -p * this->n;
	}

	Plane(Vect3 p1, Vect3 p2, Vect3 p3) {
		n = ((p2 - p1) ^ (p3 - p1)).normalize();
		d = -p1 * n;
	}

	ld intersect_line(Vect3 p, Vect3 v) {
		ld nv = n * v;
		if (abs(nv) <= eps)
			return inf;

		return (-d - n * p) / nv;
	}

	bool on_surface(Vect3 p) {
		return abs(p * n + d) <= eps;
	}
};
