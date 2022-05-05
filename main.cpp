typedef long double ld;

#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <cmath>

#include "common_classes/Vect3.h"
#include "common_classes/ComplexLD.h"
#include "common_classes/EqtSolver.h"
#include "common_classes/Plane.h"
#include "common_classes/ColourRGB.h"

#include "scene_objects/Light.h"
#include "scene_objects/SceneObject.h"

#include "scene_objects/primitives/Sphere.h"
#include "scene_objects/primitives/Triangle.h"
#include "scene_objects/primitives/Cylinder.h"
#include "scene_objects/primitives/Thor.h"
#include "scene_objects/primitives/Block.h"

#include "structural_classes/Scene.h"

using namespace sf;


const ld PI = acos(-1), FOV = PI / 3;
const int DEPTH = 3, RESOLUTION = 1, W = 1400, H = 700;


void drow(Vect3 o, Vect3 dir, Vect3 horizon, Scene& scene, RenderWindow& window) {
	dir = dir.normalize();
	horizon = horizon.normalize();
	Vect3 vertical = dir ^ horizon;

	RectangleShape rect(Vector2f(RESOLUTION, RESOLUTION));
	int cnt_x = (H - 1) / RESOLUTION + 1, cnt_y = (W - 1) / RESOLUTION + 1, cur_cnt = 0;
	Vect3 p0 = o + dir - tan(FOV / 2) * (horizon  - ((ld)H / W) * vertical);
	Vect3 dx = 2 * tan(FOV / 2) * ((ld)H / W) * (-vertical) / cnt_x;
	Vect3 dy = 2 * tan(FOV / 2) * horizon / cnt_y;
	for (int i = 0; i < cnt_x; i++) {
		for (int j = 0; j < cnt_y; j++) {
			ColourRGB cur = scene.get_colour(o, p0 - o + i * dx + j * dy + dx / 2 + dy / 2, DEPTH);

			rect.setPosition(j * RESOLUTION, i * RESOLUTION);
			rect.setFillColor(Color(cur.r, cur.g, cur.b));
			window.draw(rect);

			cur_cnt++;
			if ((cur_cnt * 100) / (cnt_x * cnt_y) > ((cur_cnt - 1) * 100) / (cnt_x * cnt_y) && 1) {
				system("cls");
				std::cout << "Process complete: " << (cur_cnt * 100) / (cnt_x * cnt_y) << "%\n";
			}
		}
	}
	system("cls");

	window.display();
}


void add_plane(Vect3 p1, Vect3 p2, Vect3 p3, Vect3 p4, Scene& scene) {
	scene.add_object(new Triangle(p1, p2, p3, ColourRGB(255, 255, 255), -1));
	scene.add_object(new Triangle(p1, p3, p4, ColourRGB(255, 255, 255), -1));
}


signed main() {
	RenderWindow window(VideoMode(W, H), "Ray tracer");

	ld pos = 2, r1 = sqrt(3) / 12, r2 = r1 / 2, d = 0.3, f = 0.15;
	Vect3 p1(0, 0, sqrt(3) / 3), p2(0, -0.5, -sqrt(3) / 6), p3(0, 0.5, -sqrt(3) / 6), dt = Vect3(-d, 0, 0);
	Vect3 p11 = p1 + (f * 2.0 / sqrt(3)) * (p3 - p1), p12 = p1 + (f * 2.0 / sqrt(3)) * (p2 - p1);
	Vect3 p21 = p2 + (f * 2.0 / sqrt(3)) * (p3 - p2), p22 = p2 + (f * 2.0 / sqrt(3)) * (p1 - p2);
	Vect3 p31 = p3 + (f * 2.0 / sqrt(3)) * (p1 - p3), p32 = p3 + (f * 2.0 / sqrt(3)) * (p2 - p3);
	Vect3 p4 = p1 + (f * 2.0 / sqrt(3)) * (p2 - p3), p5 = p2 + (f * 2.0 / sqrt(3)) * (p1 - p3);
	Vect3 p6 = p2 + (f * 2.0 / sqrt(3)) * (p3 - p1), p7 = p3 + (f * 2.0 / sqrt(3)) * (p2 - p1);
	Vect3 p8 = p3 + (f * 2.0 / sqrt(3)) * (p1 - p2), p9 = p1 + (f * 2.0 / sqrt(3)) * (p3 - p2);

	SceneObject* c1 = new Cylinder(r1, d, Vect3(0, 0, 0), Vect3(-1, 0, 0), ColourRGB(255, 0, 0));
	SceneObject* c2 = new Cylinder(r2, d, Vect3(0, 0, 0), Vect3(-1, 0, 0), ColourRGB(255, 0, 0));
	SceneObject* c12 = new Block('&', c1, new Block('~', c2, ColourRGB(255, 0, 0), -1), ColourRGB(255, 0, 0), -1);

	Scene scene({ c12 }, { Light('a', 0.2), Light('p', 0.4, Vect3(1, 0, 0)), Light('d', 0.4, Vect3(-1, -1, -1)) });

	add_plane(p1, p2, p2 + dt, p1 + dt, scene);
	add_plane(p2, p3, p3 + dt, p2 + dt, scene);

	scene.add_object(new Triangle(p1, p11, p12, ColourRGB(255, 255, 255), -1));
	scene.add_object(new Triangle(p2, p21, p22, ColourRGB(255, 255, 255), -1));
	scene.add_object(new Triangle(p3, p31, p32, ColourRGB(255, 255, 255), -1));

	add_plane(p11, p12, p12 + dt, p11 + dt, scene);
	add_plane(p21, p22, p22 + dt, p21 + dt, scene);
	add_plane(p31, p32, p32 + dt, p31 + dt, scene);

	add_plane(p1, p2, p5, p4, scene);
	add_plane(p2, p3, p7, p6, scene);
	add_plane(p3, p1, p9, p8, scene);

	add_plane(p9, p8, p8 + dt, p9 + dt, scene);

	ld dy = 0.5, dz = dy / sqrt(3);
	drow(Vect3(pos, dy, dz), Vect3(-pos, -dy, -dz + 0.1), Vect3(dy, -pos, 0), scene, window);

	while (window.isOpen()) {
		Event event;
		while (window.pollEvent(event)) {
			if (event.type == Event::Closed)
				window.close();
		}
	}

	return 0;
}
