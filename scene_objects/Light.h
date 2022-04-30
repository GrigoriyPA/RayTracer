#pragma once


class Light {
public:
	char type; // 'a' - ambient; 'd' - directional; 'p' - point
	Vect3 power, v;

	Light(char type, ld power, Vect3 v = Vect3(0, 0, 0)) {
		this->type = type;
		this->power = Vect3(power, power, power);
		this->v = v;
	}

	Light(char type, Vect3 power, Vect3 v = Vect3(0, 0, 0)) {
		this->type = type;
		this->power = power;
		this->v = v;
	}

	Vect3 get_lighting(Vect3 p, Vect3 n, Vect3 o = Vect3(0, 0, 0), ld spec = -1) {
		if (type == 'a')
			return power;

		Vect3 d;
		if (type == 'd')
			d = -v;
		else
			d = v - p;

		Vect3 res = (power * (d * n) / d.length()).set_max(Vect3(0, 0, 0));

		if (spec != -1) {
			Vect3 r = d.reflect_vect(n);
			ld cos_ang = r.cos_angle(o - p);
			if (cos_ang > 0)
				res += power * pow(cos_ang, spec);
		}

		return res;
	}
};
