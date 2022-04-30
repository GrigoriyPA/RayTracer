class Block : public SceneObject {
	SceneObject* o1, * o2;
	char ty;

public:
	Block(char ty, SceneObject* o1, ColourRGB color, ld spec = -1, ld refl = 0) {
		this->ty = ty;
		this->o1 = o1;
		this->o2 = nullptr;
		this->color = color;
		this->spec = spec;
		this->refl = refl;
	}

	Block(char ty, SceneObject* o1, SceneObject* o2, ColourRGB color, ld spec = -1, ld refl = 0) {
		this->ty = ty;
		this->o1 = o1;
		this->o2 = o2;
		this->color = color;
		this->spec = spec;
		this->refl = refl;
	}

	std::vector < ld > intersect_line(Vect3 p, Vect3 d) {
		std::vector < ld > t1 = o1->intersect_line(p, d), t2;
		if (o2 != nullptr)
			t2 = o2->intersect_line(p, d);

		if (ty == '~') {
			for (int i = 0; i < t1.size(); i += 2) {
				t1[i] -= eps;
				t1[i + 1] += eps;
			}
			t1.insert(t1.begin(), -inf);
			t1.push_back(inf);
			return t1;
		}

		std::vector < ld > res;
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

	Vect3 normal(Vect3 p, Vect3 p0) {
		if (ty == '~')
			return -o1->normal(p, p0);
		if (o1->on_surface(p))
			return o1->normal(p, p0);
		else
			return o2->normal(p, p0);
	}

	bool on_surface(Vect3 p) {
		if (ty == '&')
			return (o1->on_surface(p) && o2->is_inside(p)) || (o2->on_surface(p) && o1->is_inside(p));
		if (ty == '|')
			return (o1->on_surface(p) && !o2->is_inside(p)) || (o2->on_surface(p) && !o1->is_inside(p));
		return o1->on_surface(p);
	}

	bool is_inside(Vect3 p) {
		if (ty == '&')
			return o1->is_inside(p) && o2->is_inside(p);
		if (ty == '|')
			return o1->is_inside(p) || o2->is_inside(p);
		return !o1->is_inside(p);
	}
};
