#include <ctime>
#include <cmath>
#include <algorithm>
#include <iostream>
#include <vector>
#include <string>

using namespace std;

typedef long double ld;

#define all(a) a.begin(), a.end()


const ld PI = acos(-1);


struct ComplexLD {
	ld r, im;

	ComplexLD(ld r = 0, ld im = 0) {
		this->r = r;
		this->im = im;
	}

	ComplexLD operator +(ComplexLD v) {
		return ComplexLD(r + v.r, im + v.im);
	}

	ComplexLD operator -() {
		return ComplexLD(-r, -im);
	}

	ComplexLD operator -(ComplexLD v) {
		return ComplexLD(r - v.r, im - v.im);
	}

	vector <  ComplexLD > root(int deg) {
		ld m = pow(r * r + im * im, (ld)1 / (2 * deg));
		if (m <= eps)
			return { ComplexLD(0, 0) };
		ld ang = atan2(im, r) / deg;

		vector < ComplexLD > res(deg);
		for (int i = 0; i < deg; i++, ang += 2 * PI / deg)
			res[i] = ComplexLD(cos(ang) * m, sin(ang) * m);

		return res;
	}

private:
	ld eps = 0.0000001;
};


struct Vect {
	ld x, y, z;

	Vect() {
		x = 0;
		y = 0;
		z = 0;
	}

	Vect(ld x, ld y, ld z) {
		this->x = x;
		this->y = y;
		this->z = z;
	}

	void operator *=(ld v) {
		x *= v;
		y *= v;
		z *= v;
	}

	Vect operator -() {
		return Vect(-x, -y, -z);
	}

	Vect operator -(Vect v) {
		return Vect(x - v.x, y - v.y, z - v.z);
	}

	Vect operator +(Vect v) {
		return Vect(x + v.x, y + v.y, z + v.z);
	}

	Vect operator *(ld v) {
		return Vect(x * v, y * v, z * v);
	}

	ld operator *(Vect v) {
		return x * v.x + y * v.y + z * v.z;
	}

	Vect operator ^(Vect v) {
		return Vect(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
	}

	Vect operator /(ld v) {
		return Vect(x / v, y / v, z / v);
	}

	ld len() {
		return sqrtl(x * x + y * y + z * z);
	}

	ld len_sqr() {
		return x * x + y * y + z * z;
	}

	ld cos_angle(Vect v) {
		return (v * *this) / (v.len() * this->len());
	}

	Vect normalize() {
		return *this / len();
	}

	bool in_angle(Vect a, Vect b) {
		return abs((a ^ *this).cos_angle(b ^ *this) + 1) < eps;
	}

	void print() {
		cout << '(' << x << ", " << y << ", " << z << ')';
	}

private:
	ld eps = 0.00001;
};


Vect operator *(ld v, Vect p) {
	return Vect(p.x * v, p.y * v, p.z * v);
}


struct Plane {
	ld d;
	Vect n;

	Plane() {
		d = 0;
	}

	Plane(Vect p, Vect n) {
		this->n = n.normalize();
		d = -p * this->n;
	}

	Plane(Vect p1, Vect p2, Vect p3) {
		n = ((p2 - p1) ^ (p3 - p1)).normalize();
		d = -p1 * n;
	}

	ld intersect_line(Vect p, Vect v) {
		ld nv = n * v;
		if (abs(nv) <= eps)
			return inf;

		return (-d - n * p) / nv;
	}

	bool on_surface(Vect p) {
		return abs(p * n + d) <= eps;
	}

private:
	ld eps = 0.000001, inf = 1e18;
};


vector < ld > sol_eqt(ld a, ld b, ld c) {
	ld eps = 0.000001;

	ld p = b - a * a / 3;
	ld q = c + 2 * a * a * a / 27 - a * b / 3;
	ComplexLD rt = ComplexLD(q * q / 4 + p * p * p / 27).root(2)[0];

	vector < ComplexLD > v1 = (rt - q / 2).root(3);
	vector < ComplexLD > v2 = (-rt - q / 2).root(3);
	vector < ld > res;
	for (ComplexLD el1 : v1) {
		for (ComplexLD el2 : v2) {
			if (abs((el1 + el2).im) <= eps)
				res.push_back((el1 + el2).r - a / 3);
		}
	}

	return res;
}


vector < ld > sol_eqt(ld a, ld b, ld c, ld d) {
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

		vector < ld > res;
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

	vector < ld > res;
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


struct Light {
	Vect v;
	ld pw;
	char ty;

	Light(char ty, ld pw, Vect v = Vect()) {
		this->ty = ty;
		this->pw = pw;
		this->v = v;
	}

	ld get_lighting(Vect p, Vect n) {
		if (ty == 'a')
			return pw;

		Vect d;
		if (ty == 'd')
			d = -v;
		else
			d = v - p;

		return max(pw * (d * n) / d.len(), (ld)0);
	}
};


class SceneObject {
public:
	ld eps = 0.00001, inf = 1e18;

	virtual vector < ld > intersect_line(Vect p, Vect d) = 0;

	virtual Vect normal(Vect p, Vect p0) = 0;

	virtual bool on_surface(Vect p) = 0;

	virtual bool is_inside(Vect p) = 0;

	ld intersect_ray(Vect p, Vect d) {
		vector < ld > t = intersect_line(p, d);
		for (ld cur : t) {
			if (cur >= 0)
				return cur;
		}
		return -1;
	}
};


class Block : public SceneObject {
	SceneObject* o1, * o2;
	char ty;

public:
	Block(char ty, SceneObject* o1, SceneObject* o2 = nullptr) {
		this->ty = ty;
		this->o1 = o1;
		this->o2 = o2;
	}

	vector < ld > intersect_line(Vect p, Vect d) {
		vector < ld > t1 = o1->intersect_line(p, d), t2;
		if (o2 != nullptr)
			t2 = o2->intersect_line(p, d);

		if (ty == '~') {
			t1.insert(t1.begin(), -inf);
			t1.push_back(inf);
			return t1;
		}

		vector < ld > res;
		for (int i = 0, j = 0; i < t1.size() || j < t2.size();) {
			if (j == t2.size() || (i < t1.size() && t1[i + 1] < t2[j])) {
				if (ty == '|') {
					res.push_back(t1[i]);
					res.push_back(t1[i + 1]);
				}
				i += 2;
				continue;
			}

			if (i == t1.size() || (j < t2.size() && t2[j + 1] < t1[i])) {
				if (ty == '|') {
					res.push_back(t2[j]);
					res.push_back(t2[j + 1]);
				}
				j += 2;
				continue;
			}

			if (t2[j] <= t1[i] && t1[i + 1] <= t2[j + 1]) {
				if (ty == '&') {
					res.push_back(t1[i]);
					res.push_back(t1[i + 1]);
				}
				i += 2;
				continue;
			}

			if (t1[i] <= t2[j] && t2[j + 1] <= t1[i + 1]) {
				if (ty == '&') {
					res.push_back(t2[j]);
					res.push_back(t2[j + 1]);
				}
				j += 2;
				continue;
			}

			if (t1[i] < t2[j]) {
				if (ty == '&') {
					res.push_back(t2[j]);
					res.push_back(t1[i + 1]);
				}
				else
					t2[j] = t1[i];
				i += 2;
				continue;
			}

			if (t2[j] < t1[i]) {
				if (ty == '&') {
					res.push_back(t1[i]);
					res.push_back(t2[j + 1]);
				}
				else
					t1[i] = t2[j];
				j += 2;
				continue;
			}
		}
		return res;
	}

	Vect normal(Vect p, Vect p0) {
		if (ty == '~')
			return -o1->normal(p, p0);
		if (o1->on_surface(p))
			return o1->normal(p, p0);
		else
			return o2->normal(p, p0);
	}

	bool on_surface(Vect p) {
		if (ty == '&')
			return (o1->on_surface(p) && o2->is_inside(p)) || (o2->on_surface(p) && o1->is_inside(p));
		if (ty == '|')
			return (o1->on_surface(p) && !o2->is_inside(p)) || (o2->on_surface(p) && !o1->is_inside(p));
		return o1->on_surface(p);
	}

	bool is_inside(Vect p) {
		if (ty == '&')
			return o1->is_inside(p) && o2->is_inside(p);
		if (ty == '|')
			return o1->is_inside(p) || o2->is_inside(p);
		return !o1->is_inside(p);
	}
};


class CutPlane : public SceneObject {
	Vect n;
	Plane pl;

public:
	CutPlane(Vect p1, Vect p2, Vect p3) {
		n = ((p3 - p1) ^ (p3 - p2)).normalize();
		pl = Plane(p1, n);
	}

	CutPlane(Vect p, Vect n) {
		this->n = n.normalize();
		pl = Plane(p, n);
	}

	vector < ld > intersect_line(Vect p, Vect v) {
		ld t = pl.intersect_line(p, v);

		if (on_surface(p + t * v))
			return { t, t };

		return {};
	}

	Vect normal(Vect p, Vect p0) {
		if ((p0 - p) * n < 0)
			return -n;
		return n;
	}

	bool on_surface(Vect p) {
		return pl.on_surface(p);
	}

	bool is_inside(Vect p) {
		return pl.on_surface(p);
	}
};


class Triangle : public SceneObject {
	Vect p1, p2, p3, n;
	Plane pl;

public:
	Triangle(Vect p1, Vect p2, Vect p3) {
		this->p1 = p1;
		this->p2 = p2;
		this->p3 = p3;
		n = ((p3 - p1) ^ (p3 - p2)).normalize();
		pl = Plane(p1, n);
	}

	vector < ld > intersect_line(Vect p, Vect v) {
		ld t = pl.intersect_line(p, v);

		if (on_surface(p + t * v))
			return { t, t };

		return {};
	}

	Vect normal(Vect p, Vect p0) {
		if ((p0 - p) * n < 0)
			return -n;
		return n;
	}

	bool on_surface(Vect p) {
		return (p - p1).in_angle(p2 - p1, p3 - p1) && (p - p2).in_angle(p1 - p2, p3 - p2);
	}

	bool is_inside(Vect p) {
		return (p - p1).in_angle(p2 - p1, p3 - p1) && (p - p2).in_angle(p1 - p2, p3 - p2);
	}
};


class Thor : public SceneObject {
	ld w, r;
	Vect o, d;

public:
	Thor(ld w, ld r, Vect o, Vect d) {
		this->w = w;
		this->r = r;
		this->o = o;
		this->d = d.normalize();
	}

	vector < ld > intersect_line(Vect p, Vect v) {
		ld t1 = (p - o).len_sqr() - w * w - r * r;
		ld t2 = v * (p - o);
		ld t3 = v * d;
		ld t4 = d * (p - o);

		ld k0 = v.len_sqr() * v.len_sqr() / (4 * w * w);
		ld k1 = t2 * v.len_sqr() / (w * w);
		ld k2 = v.len_sqr() * t1 / (2 * w * w) + t2 * t2 / (w * w) + t3 * t3;
		ld k3 = t1 * t2 / (w * w) + 2 * t3 * t4;
		ld k4 = t1 * t1 / (4 * w * w) + t4 * t4 - r * r;

		vector < ld > t = sol_eqt(k1 / k0, k2 / k0, k3 / k0, k4 / k0);
		sort(all(t));

		return t;
	}

	Vect normal(Vect p, Vect p0) {
		Vect v = p - o;
		Vect t = (v - d * (v * d)).normalize() * w;
		return (v - t).normalize();
	}

	bool on_surface(Vect p) {
		Vect v = p - o;
		Vect t = (v - d * (v * d)).normalize() * w;
		return abs((v - t).len() - r) <= eps;
	}

	bool is_inside(Vect p) {
		Vect v = p - o;
		Vect t = (v - d * (v * d)).normalize() * w;
		return (v - t).len() <= r;
	}
};


class Cylinder : public SceneObject {
	ld r, h;
	Plane pl1, pl2;
	Vect o, d;

public:
	Cylinder(ld r, ld h, Vect o, Vect d) {
		this->r = r;
		this->h = h;
		this->o = o;
		this->d = d.normalize();
		pl1 = Plane(o, -d);
		pl2 = Plane(o + this->d * h, d);
	}

	vector < ld > intersect_line(Vect p, Vect v) {
		vector < ld > res;
		ld t1 = pl1.intersect_line(p, v);
		ld t2 = pl2.intersect_line(p, v);
		if (on_surface(p + v * t1))
			res.push_back(t1);
		if (on_surface(p + v * t2))
			res.push_back(t2);

		Vect vb = p - o;
		ld av = d * v, ab = d * vb;

		ld a = v.len_sqr() - av * av;
		ld b = 2 * (vb * v - ab * av);
		ld c = vb.len_sqr() - ab * ab - r * r;

		ld des = b * b - 4 * a * c;

		if (des < 0)
			return res;

		des = sqrtl(des);
		ld t3 = (-b - des) / (2 * a);
		ld t4 = (-b + des) / (2 * a);
		if (on_surface(p + v * t3))
			res.push_back(t3);
		if (on_surface(p + v * t4))
			res.push_back(t4);
		sort(all(res));

		return res;
	}

	Vect normal(Vect p, Vect p0) {
		Vect v = p - o;
		ld dist = v * d;
		if (dist <= eps)
			return -d;
		if (dist >= h - eps)
			return d;
		return (v - d * (v * d)).normalize();
	}

	bool on_surface(Vect p) {
		Vect v = p - o;
		ld dist = v * d, cur_r = v.len_sqr() - dist * dist;
		return (abs(cur_r - r * r) <= eps && -eps <= dist && dist <= h + eps) || (cur_r < r* r && (abs(dist) <= eps || abs(dist - h) <= eps));
	}

	bool is_inside(Vect p) {
		Vect v = p - o;
		ld dist = v * d, cur_r = v.len_sqr() - dist * dist;
		return cur_r <= r * r && 0 <= dist && dist <= h;
	}
};


class Sphere : public SceneObject {
	ld r;
	Vect o;

public:
	Sphere(ld r, Vect o) {
		this->r = r;
		this->o = o;
	}

	vector < ld > intersect_line(Vect p, Vect d) {
		Vect s = p - o;

		ld a = d.len_sqr();
		ld b = s * d * 2;
		ld c = s.len_sqr() - r * r;

		ld des = b * b - 4 * a * c;

		if (des < 0)
			return {};

		des = sqrtl(des);
		ld t1 = (-b - des) / (2 * a);
		ld t2 = (-b + des) / (2 * a);

		return { t1, t2 };
	}

	Vect normal(Vect p, Vect p0) {
		return (p - o).normalize();
	}

	bool on_surface(Vect p) {
		return abs((p - o).len() - r) <= eps;
	}

	bool is_inside(Vect p) {
		return (p - o).len() <= r;
	}
};


class Scene {
	ld eps = 0.0001;
	int obj_id = 0, light_id = 0;
	vector < pair < SceneObject*, int > > obj;
	vector < pair < Light, int > > light;

	pair < SceneObject*, Vect >  closest_intersection(Vect p, Vect d) {
		ld t = -1;
		SceneObject* res_obj = nullptr;
		for (pair < SceneObject*, int > el : obj) {
			SceneObject* cur_obj = el.first;
			ld cur_t = cur_obj->intersect_ray(p, d);
			if (cur_t < 0)
				continue;

			if (t < 0 || t > cur_t) {
				t = cur_t;
				res_obj = cur_obj;
			}
		}
		return { res_obj, p + d * t };
	}

	ld compute_lighting(Vect p, Vect n) {
		ld res = 0;
		for (pair < Light, int > el : light) {
			Light cur = el.first;
			if (cur.ty == 'a') {
				res += cur.get_lighting(p, n);
				continue;
			}

			Vect d = -cur.v;
			if (cur.ty == 'p')
				d = cur.v - p;

			pair < SceneObject*, Vect > sh = closest_intersection(p + d * eps, d);

			if (sh.first != nullptr && (cur.ty == 'd' || (sh.second - p).len() <= (cur.v - p).len()))
				continue;

			res += cur.get_lighting(p, n);
		}
		return res;
	}

public:
	Scene() {
	}

	Scene(vector < SceneObject* > init_obj, vector < Light > init_light) {
		for (SceneObject* el : init_obj)
			obj.push_back({ el, obj_id++ });
		for (Light el : init_light)
			light.push_back({ el, light_id++ });
	}

	ld get_colour(Vect o, Vect d) {
		pair < SceneObject*, Vect > intersect = closest_intersection(o, d);
		SceneObject* cur_obj = intersect.first;
		Vect p = intersect.second;

		if (cur_obj == nullptr)
			return 0;

		return compute_lighting(p, cur_obj->normal(p, o));
	}

	void change_light(int id, Light nw) {
		bool fd = false;
		for (pair < Light, int >& el : light) {
			if (el.second != id)
				continue;

			el.first = nw;
			fd = true;
			break;
		}
		if (!fd) {
			light.push_back({ nw, id });
			light_id += light_id == id;
		}
	}

	void change_object(int id, SceneObject* nw) {
		bool fd = false;
		for (pair < SceneObject*, int >& el : obj) {
			if (el.second != id)
				continue;

			delete el.first;
			el.first = nw;
			fd = true;
			break;
		}
		if (!fd) {
			obj.push_back({ nw, id });
			obj_id += obj_id == id;
		}
	}

	void add_object(SceneObject* nw) {
		obj.push_back({ nw, obj_id++ });
	}
};


char colour(ld x) {
	string grad = " .:;!/r1(l4Z9H8$W@";

	x = min(x, (ld)1);

	ld p = x * (grad.size() - 1);
	int i = int(p);
	if ((ld)ceil(p) - p < p - i)
		i = ceil(p);

	return grad[i];
}


void drow(ld w, ld h, ld fov, Vect o, Vect dir, Vect a, Scene& scene) {
	vector < vector < ld > > col(h + 1, vector < ld >(w + 1));
	a = a.normalize();
	dir = dir.normalize();
	Vect b = a ^ dir;
	Vect v = dir + (a + b * (2 * h / w)) * tan(fov / 2);
	for (int i = 0; i <= h; i++) {
		for (int j = 0; j <= w; j++) {
			col[i][j] = scene.get_colour(o, v - (a * j + b * 2 * i) * (2 * tan(fov / 2) / w));
		}

		reverse(all(col[i]));
	}

	system("cls");
	string img;
	for (int i = 0; i <= h; i++) {
		for (int j = 0; j <= w; j++)
			img += colour(col[i][j]);
		img += '\n';
	}
	cout << img;
}


signed main() {
	ios_base::sync_with_stdio(0);

	SceneObject* a = new Thor(2, 1, Vect(0, 0, 0), Vect(1, 0, 0));
	SceneObject* b = new Sphere(2, Vect(0, 0, 0));
	SceneObject* c = new Block('&', a, b);
	SceneObject* d = new Block('&', b, new Block('~', a));

	Scene scene({ c }, { });

	ld k = 50, r = 5;
	for (ld i = 0; true; i++) {
		ld ang = 2 * PI * i / k;
		Vect o(cos(ang) * r, sin(ang) * r, 0);
		Vect d = -o;
		Vect a(o.y, -o.x, 0);

		scene.change_light(0, Light('p', 1, o));

		drow(200, 49, PI / 3, o, d, a, scene);
	}

	return 0;
}
//  !"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_`abcdefghijklmnopqrstuvwxyz{|}~