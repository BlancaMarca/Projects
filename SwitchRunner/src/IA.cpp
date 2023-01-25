#include "IA.h"
#include "world.h"
#include "road.h"
#include "collision.h"



void IA::moveIA(Car* car,double *dt)
{
	//TEMPORAL
	//car->physics.engineForce = 0;
	//HASTA AQUI
	Matrix44 o;
	Matrix44 rl;
	Matrix44 rf;
	Matrix44 rr;

	o.translate(0, 0, 0);
	rl.translate(0, 0, -50);
	rf.translate(1, 0, 0);
	rr.translate(0, 0, 69.9/2);

	o = o * car->model;
	rl = rl * car->model;
	rf = rf * car->model;
	rr = rr * car->model;

	Vector2 rll;
	Vector2 rff;
	Vector2 rrr;
	Vector2 oo;

	oo.set(o.getTranslation().x, o.getTranslation().z);
	rll.set(rl.getTranslation().x, rl.getTranslation().z);
	rrr.set(rr.getTranslation().x, rr.getTranslation().z);
	rff.set(rf.getTranslation().x, rf.getTranslation().z);


	std::vector<std::tuple<Vector3, Vector3>> pointsl;
	std::vector<std::tuple<Vector3, Vector3>> pointsr;

	Vector2 p0;
	Vector2 p1;
	Vector2 p2;
	float dl;
	float dr;
	float df;

	bool end = 0;
	float rot = 10;

	bool col;

	for (int j = 0; j < 2 && !end; j++) {
		//std::cout << "road" << j << std::endl;
		Road* temp = dynamic_cast<Road*>(World::instance->roadmap.children[car->roadpos+j]);
		pointsl = Collision::borderRays(temp->getGlobalMatrix(), temp->roadtype, temp->size, 0);
		pointsr = Collision::borderRays(temp->getGlobalMatrix(), temp->roadtype, temp->size, 1);



		/*for (int i = 0; i < pointsl.size() && !end; i++) {
			p1.set(std::get<0>(pointsl[i]).x, std::get<0>(pointsl[i]).z);
			p2.set(std::get<1>(pointsl[i]).x, std::get<1>(pointsl[i]).z);

			if (temp->roadtype == LEFT) {
				if (segmentIntersection(oo, rll, p1, p2)) {
					//end = TRUE;
					std::cout << "giro izq 1" << std::endl;
					//car->model.rotate(rot * *dt * DEG2RAD, Vector3(0.0f, -1.0f, 0.0f));
				}
			}
			else if(temp->roadtype==RIGHT){
				if (segmentIntersection(oo, rrr, p1, p2)) {
					//end = TRUE;
					std::cout << "giro derecha 1" << std::endl;
					//car->model.rotate(rot * *dt * DEG2RAD, Vector3(0.0f, -1.0f, 0.0f));
				}
			}
			else if (temp->roadtype == STRAIGHT) {
				if (segmentIntersection(oo, rrr, p1, p2)) {
					//end = TRUE;
					std::cout << "recto 1" << std::endl;
					//car->model.rotate(rot * *dt * DEG2RAD, Vector3(0.0f, -1.0f, 0.0f));
				}
			}
			//dl = distancePL(rrr, p1, p2);
			//std::cout <<dl  << std::endl;
		}

		for (int i = 0; i < pointsr.size(); i++) {
			p1.set(std::get<0>(pointsr[i]).x, std::get<0>(pointsr[i]).z);
			p2.set(std::get<1>(pointsr[i]).x, std::get<1>(pointsr[i]).z);

			if (temp->roadtype == LEFT) {
				if (segmentIntersection(oo, rrr, p1, p2)) {
					//end = TRUE;
					std::cout << "giro izq 2"  << std::endl;
					//car->model.rotate(-rot * *dt * DEG2RAD, Vector3(0.0f, -1.0f, 0.0f));
				}
			}
			else if (temp->roadtype == RIGHT) {
				if (segmentIntersection(oo, rll, p1, p2)) {
					//end = TRUE;
					std::cout << "giro der 2" << std::endl;
					//car->model.rotate(-rot * *dt * DEG2RAD, Vector3(0.0f, -1.0f, 0.0f));
				}
			}
			else if (temp->roadtype == STRAIGHT) {
				if (segmentIntersection(oo, rll, p1, p2)) {
					//end = TRUE;
					std::cout << "recto 2" << std::endl;
					//car->model.rotate(-rot * *dt * DEG2RAD, Vector3(0.0f, -1.0f, 0.0f));
				}
			}
		}*/
		//std::cout << pointsl << j << std::endl;

		for (size_t i = 0; i < pointsl.size(); i++)
		{
			p1.set(std::get<0>(pointsl[i]).x, std::get<0>(pointsl[i]).z);
			p2.set(std::get<1>(pointsl[i]).x, std::get<1>(pointsl[i]).z);
			col = segmentIntersection(p1, p2, oo, rll);

			if (!col)
			{
				//std::cout << "lejos" << std::endl;
			}
		}
	}
}

float IA::distancePL(Vector2 p0, Vector2 p1, Vector2 p2)
{
	return abs((p2.x - p1.x) * (p1.y - p0.y) - (p1.x - p0.x) * (p2.y - p1.y)) / sqrt((p2.x - p1.x) * (p2.x - p1.x) + (p2.y - p1.y) * (p2.y - p1.y));
}

bool IA::segmentIntersection(Vector2 p1, Vector2 q1, Vector2 p2, Vector2 q2)
{
	// Find the four orientations needed for general and
   // special cases
	int o1 = orientation(p1, q1, p2);
	int o2 = orientation(p1, q1, q2);
	int o3 = orientation(p2, q2, p1);
	int o4 = orientation(p2, q2, q1);

	// General case
	if (o1 != o2 && o3 != o4)
		return true;

	// Special Cases
	// p1, q1 and p2 are colinear and p2 lies on segment p1q1
	if (o1 == 0 && onSegment(p1, p2, q1)) return true;

	// p1, q1 and q2 are colinear and q2 lies on segment p1q1
	if (o2 == 0 && onSegment(p1, q2, q1)) return true;

	// p2, q2 and p1 are colinear and p1 lies on segment p2q2
	if (o3 == 0 && onSegment(p2, p1, q2)) return true;

	// p2, q2 and q1 are colinear and q1 lies on segment p2q2
	if (o4 == 0 && onSegment(p2, q1, q2)) return true;

	return false; // Doesn't fall in any of the above cases
}

int IA::orientation(Vector2 p, Vector2 q, Vector2 r)
{
	int val = (q.y - p.y) * (r.x - q.x) -
		(q.x - p.x) * (r.y - q.y);

	if (val == 0) return 0;  // colinear

	return (val > 0) ? 1 : 2; // clock or counterclock wise
}

bool IA::onSegment(Vector2 p, Vector2 q, Vector2 r)
{
	if (q.x <= max(p.x, r.x) && q.x >= min(p.x, r.x) &&
		q.y <= max(p.y, r.y) && q.y >= min(p.y, r.y))
		return true;

	return false;
}