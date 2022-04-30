#pragma once

#include <vector>


class Scene {
	ld eps = 0.0001;
	int obj_id = 0, light_id = 0;
	std::vector < std::pair < SceneObject*, int > > obj;
	std::vector < std::pair < Light, int > > light;

	std::pair < SceneObject*, Vect3 > closest_intersection(Vect3 p, Vect3 d) {
		ld t = -1;
		SceneObject* res_obj = nullptr;
		for (std::pair < SceneObject*, int > el : obj) {
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

	Vect3 compute_lighting(Vect3 p, Vect3 n, Vect3 o, ld spec) {
		Vect3 power(0, 0, 0);
		for (std::pair < Light, int > el : light) {
			Light cur = el.first;
			if (cur.type == 'a') {
				power += cur.get_lighting(p, n);
				continue;
			}

			Vect3 d = -cur.v;
			if (cur.type == 'p')
				d = cur.v - p;

			std::pair < SceneObject*, Vect3 > sh = closest_intersection(p + d * eps, d);

			if (sh.first != nullptr && (cur.type == 'd' || (sh.second - p).length() <= (cur.v - p).length()))
				continue;

			power += cur.get_lighting(p, n, o, spec);
		}
		return power;
	}

public:
	ColourRGB background_color = ColourRGB(0, 0, 0);

	Scene() {
	}

	Scene(std::vector < SceneObject* > init_obj, std::vector < Light > init_light) {
		for (SceneObject* el : init_obj)
			obj.push_back({ el, obj_id++ });
		for (Light el : init_light)
			light.push_back({ el, light_id++ });
	}

	ColourRGB get_colour(Vect3 o, Vect3 d, int depth) {
		std::pair < SceneObject*, Vect3 > intersect = closest_intersection(o, d);
		SceneObject* cur_obj = intersect.first;
		Vect3 p = intersect.second;

		if (cur_obj == nullptr)
			return background_color;
		Vect3 n = cur_obj->normal(p, o);

		ColourRGB local_color = cur_obj->color * compute_lighting(p, n, o, cur_obj->spec);

		if (depth <= 0)
			return local_color;

		ColourRGB reflected_color;
		if (cur_obj->refl > eps) {
			Vect3 r = (-d).reflect_vect(n);
			reflected_color = get_colour(p + eps * r, r, depth - 1);
		}

		return local_color * (1 - cur_obj->refl) + reflected_color * cur_obj->refl;
	}

	void change_light(int id, Light nw) {
		bool fd = false;
		for (std::pair < Light, int >& el : light) {
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
		for (std::pair < SceneObject*, int >& el : obj) {
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

	int add_light(Light nw) {
		light.push_back({ nw, light_id++ });
		return light_id - 1;
	}

	int add_object(SceneObject* nw) {
		obj.push_back({ nw, obj_id++ });
		return obj_id - 1;
	}
};
