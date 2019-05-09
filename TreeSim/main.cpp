#include <GL/glut.h>
#include <string>
#include <list>
#include <iostream>
#include <stdlib.h>
#include "Branch.h"
#include <time.h> 
#include <math.h>
#define PI 3.14159265
Branch* trunk;
//Globals
int branch_num = 0;

int min_len = 5;
int max_len = 7;
int min_br = 1;
int max_br = 3;
int trunk_br = 6;
int scaleh_min = 90;
int scaleh_max = 90;
int scaleb_min = 65;
int scaleb_max = 65;
int oscframes = 200;

bool main_trunk = false;
int base_radius = 20;
int base_height = 100;
vector<float> wind_vec{ 0,0,0,0 };
GLuint startList;
float rot = 0;
int osc = 0;

//change wind
bool changing = false;
float animframes = 150;
float frame = 0;
float xdiff = 0;
float zdiff = 0;
bool rotating = false;
float rotamount = 0.5;

bool flat = true;

int change = 1;
int min_wind = 1000;
int max_wind = 10000;

GLUquadricObj* qobj = gluNewQuadric();
//determines length of a branch
int treelen(Branch* b)
{
	if (b->childVec.size() == 0) {
		return 0;
	}
	else {
		int max = 0;
		for (int i = 0; i < b->childVec.size(); i++) {
			Branch* child = b->childVec.at(i);
			int h = treelen(child) + 1;
			if (h > max)
				max = h;
		}
		return max;
	}
}

Branch* make_tree2() {
	list<string> queue;
	queue.push_back("T");
	list<int> lenstack;
	list<bool> lenstackp;
	list<int> brnstack;
	list<bool> brnstackp;
	list<Branch*> branchstack;
	Branch* currBranch = new Branch(NULL);
	Branch* trunk = currBranch;
	string tree = "";
	int currlen = 1;
	int currbrn = 0;
	bool currp = true;
	bool currb = true;
	while (!queue.empty()) {
		string node = queue.front();
		queue.erase(queue.begin());
		//cout << currbrn << endl;
		if (node == "T") {
			if (currlen < min_len || currp ) {
				queue.push_front("T");
				queue.push_front("B");
				//cout << "T -> BT" << endl;
			}
			else {
				queue.push_front("L");
				//cout << "T -> L" << endl;
			}
		}
		else if (node == "B") {
			if (currlen == 2 && currbrn < trunk_br && !main_trunk) {
				queue.push_front("[T]");
				lenstack.push_back(currlen);
				lenstackp.push_back(currp);
				currbrn++;


				float r = rand() % 100;
				float r2 = 100.0 / (max_br - currbrn + 1);
				currb = currbrn < min_br || (currbrn < max_br && r > r2);

				brnstack.push_back(currbrn);
				brnstackp.push_back(currb);
				currbrn = 0;
				branchstack.push_back(currBranch);
				tree += "[";
			}
			else if (currbrn >= max_br || currbrn == 0 || !currb) {
				queue.push_front("C");
				currlen++;
				float r = rand() % 100;
				float r2 = 100.0 / (max_len - currlen + 1);
				currp = currlen < max_len &&r > r2;
				//cout << "B -> C" << endl;
				currbrn = 1;
				 r = rand() % 100;
				 r2 = 100.0 / (max_br - currbrn + 1);
				currb = currbrn < min_br || (currbrn < max_br && r > r2);
			}
			else {
				queue.push_front("[T]");
				lenstack.push_back(currlen);
				lenstackp.push_back(currp);
				currbrn++;
				
				
				float r = rand() % 100;
				float r2 = 100.0 / (max_br - currbrn + 1);
				currb = currbrn < min_br || (currbrn < max_br && r > r2);

				brnstack.push_back(currbrn);
				brnstackp.push_back(currb);
				currbrn = 0;
				branchstack.push_back(currBranch);
				tree += "[";
				//cout << "B -> [T]" << endl;
			}
		}
		else if (node == "[T]") {
			if (currlen < max_len || currp ) {
				queue.push_front("[T]");
				queue.push_front("[B]");
				//cout << "[T] -> [BT]" << endl;
			}
			else {
				queue.push_front("[L]");
				//cout << "[T] -> [L]" << endl;
			}
		}
		else if (node == "[B]") {

			if (currbrn >= max_br || currbrn == 0 || !currb) {
				queue.push_front("[C]");
				//cout << "[B] -> [C]" << endl;
				currlen++;
				float r = rand() % 100;
				float r2 = 100.0 / (max_len - currlen + 1);
				currp = currlen < max_len &&r > r2;
				currbrn=1;
				 r = rand() % 100;
				 r2 = 100.0 / (max_br - currbrn + 1);
				currb = currbrn < min_br || (currbrn < max_br && r > r2);
			}
			else {
				queue.push_front("[T]");
				lenstack.push_back(currlen);
				lenstackp.push_back(currp);
				currbrn++;
				
				
				float r = rand() % 100;
				float r2 = 100.0 / (max_br - currbrn + 1);
				currb = currbrn < min_br ||( currbrn < max_br && r > r2 );

				brnstack.push_back(currbrn);
				brnstackp.push_back(currb);
				currbrn = 0;
				branchstack.push_back(currBranch);
				tree += "[";
				//cout << "[B] -> [T]" << endl;
			}
		}
		else {
			if (node == "C" || node == "[C]") {
				
				//cout << "CURR:" << currBranch << endl;
				Branch* next = new Branch(currBranch);
				//cout << "NEXT:" << next << endl;
				currBranch->childVec.push_back(next);
				currBranch = next;
			}
			if (node.length() > 1) {
				tree += node[1];
				if (node == "[L]") {
					currb = brnstackp.back();
					brnstackp.pop_back();
					currbrn = brnstack.back();
					brnstack.pop_back();
					currp = lenstackp.back();
					lenstackp.pop_back();
					currlen = lenstack.back();
					lenstack.pop_back();
					currBranch = branchstack.back();
					branchstack.pop_back();
					tree += "]";
				}
			}
			else {
				tree += node;
			}
		}
	}
	cout << tree << endl;
	return trunk;
}

void build_tree(Branch* trunk, float x, float y, float z, float r, float h, float rx, float ry, float rz, bool isTrunk) {
	//scale = 0.8;
	trunk->baseX = x;
	trunk->baseY = y;
	trunk->baseZ = z;
	trunk->baseR = r;
	trunk->height = h;
	trunk->rotX = rx;
	trunk->rotY = ry;
	trunk->rotZ = rz;
	trunk->baseScale = (rand()%(scaleb_max - scaleb_min + 1) + scaleb_min)/100.0;
	trunk->heightScale = (rand() % (scaleh_max - scaleh_min + 1) + scaleh_min) / 100.0;;
	trunk->b = rand() % oscframes;
	

	float scaleb = trunk->baseScale;
	float scaleh = trunk->heightScale;
	trunk->topR = r * scaleb;
	if (isTrunk) {
		trunk->isTrunk = true;
	}
	if (trunk->childVec.size() > 0) {
		if (main_trunk) {
			Branch* mainb = trunk->childVec.back();

			if (isTrunk) {
				trunk->isTrunk = true;
				build_tree(mainb, x, y + h, z, r*scaleb, h*scaleh, rx, ry, rand()%10-5, true);
			}
			else {
				trunk->isTrunk = false;
				build_tree(mainb, x, y + h, z, r*scaleb, h*scaleh, rx, rand() % 100 - 50, rand() % 50, false);
			}
			float sections = trunk->childVec.size() - 1;
			if (trunk->childVec.size() > 1) {
				for (int i = 0; i < sections; i++) {

					Branch* curr = trunk->childVec.at(i);
					if (trunk->childVec.size() == 2) {
						build_tree(curr, x, y + h, z, r*scaleb, h*scaleh, rx, rand() % 360, rand() % 40 + 20, false);
					}
					else
						build_tree(curr, x, y + h, z, r*scaleb, h*scaleh, rx, 360 / sections * i + (rand() % 30 - 15), rand() % 40 + 20, false);

				}
			}
		}
		else {
			trunk->isTrunk = false;
			float sections = trunk->childVec.size();
			if (trunk->childVec.size() > 0) {
				for (int i = 0; i < sections; i++) {

					Branch* curr = trunk->childVec.at(i);
					if (trunk->childVec.size() == 1) {
						if (isTrunk) {
							build_tree(curr, x, y + h, z, r*scaleb, h*scaleh, rx, ry,rand()%10-5, true);
						}
						else {
							build_tree(curr, x, y + h, z, r*scaleb, h*scaleh, rx, 0, rand()%10 + 40, false);
						}
						
					}
					else {
						if (isTrunk) {
							build_tree(curr, x, y + h, z, r*scaleb, h*scaleh, rx, 360.0 / trunk_br * i, rand() %10 + 40, false);
						}
						else {
							build_tree(curr, x, y + h, z, r*scaleb, h*scaleh, rx, 50 / sections * i - 25 , rand() % 10 + 40, false);
						}
					}
				}
			}
		}
		

	}
}

void init(void) {
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	glOrtho(-250, 250, -250, 300, -250, 250);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	//glMatrixMode(GL_MODELVIEW);

	//random seed
	srand(time(NULL));
	trunk = make_tree2();
	build_tree(trunk, 0, -200, 0, base_radius, base_height, 0, 0, 0,true);

}

void printBranch(Branch* b) {
	cout << "Memory: " << b << endl;
	cout << "Coordinates: " << b->baseX << ", " << b->baseY << ", " << b->baseZ << endl;
	cout << "Rotations: " << b->rotX << ", " << b->rotY << ", " << b->rotZ << endl;
	cout << "Height and Radius: " << b->height << ", " << b->baseR << endl;
	cout << "Parent: " << b->parent << endl;
	cout << "Children Size: " << b->childVec.size() << endl;
	cout << "Tree Length: " << b->treelen << endl;
	cout << endl;
}

void print_vector(vector<float> v) {
	cout << "[";
	for (int i = 0; i < v.size(); i++) {
		cout << " " << v.at(i) << " ";
	}
	cout << "]" << endl;
}

void print_matrix(float *m) {
	cout << "MATRIX" << endl;
	for (int i = 0; i < 4; i++) {
		cout << "[";
		for (int j = 0; j < 4; j++) {
			cout << " " << m[i + j*4] << " ";
		}
		cout << "]" << endl;
	}
	cout << endl;
}

//multiply a matrix by a vector
vector<float> apply_matrix(float *m, vector<float>v) {
	vector<float> result;
	for (int i = 0; i < 4; i++) {
		float sum = 0;
		for (int j = 0; j < 4; j++) {
			sum += m[i + j*4] * v.at(j);
		}
		result.push_back(sum);
	}
	return result;
}

//dot product of two vectors
float dot(vector<float> a, vector<float> b) {
	float result = 0;
	for (int i = 0; i < a.size(); i++) {
		result += a.at(i) * b.at(i);
	}
	return result;
}

//scalar projection of a to b
float proj(vector<float> a, vector<float> b) {
	float bmag = 0;
	for (int i = 0; i < b.size(); i++) {
		bmag += b.at(i) * b.at(i);
	}
	bmag = sqrt(bmag);
	//cout << "BMAG: " << bmag << endl;
	return dot(a, b) / bmag;
}

void draw_tree(Branch* trunk) {
	glTranslatef(trunk->baseX, trunk->baseY, trunk->baseZ);
	
	

	//wind physics
	float m[16];
	
	//print_matrix(m);
	
	//local coordinate system
	vector<float> x_com{ 50.0,0,0,0 };
	vector<float> z_com{ 0,0,50.0,0 };
	vector<float> y_com{ 0, 50, 0, 0 };

	//print_vector(x_com);
	//print_vector(z_com);
	//print_matrix(m);
	glPushMatrix();
	glRotatef(trunk->rotX, 1.0, 0, 0);
	glRotatef(trunk->rotY, 0, 1.0, 0);
	glRotatef(trunk->rotZ, 0.0, 0, 1.0);
	glGetFloatv(GL_MODELVIEW_MATRIX, m);
	glLoadIdentity();
	glRotatef(-1*rot, 0, 1, 0);
	glMultMatrixf(m);

	glGetFloatv(GL_MODELVIEW_MATRIX, m);
	//print_matrix(m);
	x_com = apply_matrix(m,x_com);
	z_com = apply_matrix(m, z_com); 
	y_com = apply_matrix(m, y_com);
	glPopMatrix();

	float Fx = proj(wind_vec, x_com);
	float Fz = proj(wind_vec, z_com);
	int adj = osc + trunk->b;
	if (adj > oscframes) {
		adj = (oscframes)-(adj - oscframes);
	}
	if (adj == oscframes) {
		adj--;
	}
	if (adj < 0) {
		adj = -1* (adj);
	}
	float tc = sin(((adj) % oscframes - oscframes/2.0) / (oscframes/2));
	//cout << "osc: " << osc << endl;
	float Px = Fx * (1 + tc * trunk->rotZ/50.0);
	float Pz = Fz;

	
	// elastic modlulus (MPa)
	float E = 5.0 * 1000;
	float convert = 15;
	float t3 = (trunk->baseR * 2 * convert - trunk->topR * 2 * convert)*(trunk->baseR * 2 * convert - trunk->topR * 2 * convert)*(trunk->baseR * 2 * convert - trunk->topR * 2 * convert);
	//spring constant
	float k = E * (trunk->baseR * 2 * 15) * t3 / (trunk->height * trunk->height * trunk->height * 4 * convert * convert * convert);

	//displacements
	float Dx = Px / k;
	float Dz = Pz / k;
	
	float Sx = Dx / (trunk->height*convert);
	float Sz = Dz / (trunk->height*convert);

	float Tx = asin(Sx) * 180 / PI;
	float Tz = asin(Sz) * 180 / PI;
	if (Tz > 20 || Sz >= 1)
		Tz = 20;
	if (Tz < -20 || Sz <= -1)
		Tz = -20;
	if (Tx > 20 || Sx >= 1)
		Tx = 20;
	if (Tx < -20 || Sx <= -1)
		Tx = -20;

	glRotatef(trunk->rotX, 1.0, 0, 0);
	glRotatef(trunk->rotY, 0, 1.0, 0);
	glRotatef(trunk->rotZ, 0.0, 0, 1.0);

	//miracle bugfix
	glGetFloatv(GL_MODELVIEW_MATRIX, m);
	glPushMatrix();
	glLoadIdentity();
	glRotatef(-1 * rot, 0, 1, 0);
	glMultMatrixf(m);
	glGetFloatv(GL_MODELVIEW_MATRIX, m);
	glPopMatrix();
	float tempx = m[12];
	float tempy = m[13];
	float tempz = m[14];
	m[12] = 0;
	m[13] = 0;
	m[14] = 0;
	glPushMatrix();
	glLoadIdentity();
	glRotatef(rot, 0, 1, 0);
	glTranslatef(tempx, tempy, tempz);
	glRotatef(-1 * Tx, z_com.at(0), z_com.at(1), z_com.at(2));
	glRotatef(Tz, x_com.at(0), x_com.at(1), x_com.at(2));
	glMultMatrixf(m);
	glColor4f(139 / 255.0, 69 / 255.0, 19 / 255.0, 1.0);
	glPushMatrix();
	glRotatef(-90, 1.0, 0, 0);
	
	gluSphere(qobj, trunk->baseR, 15, 14);
	
	
	if (trunk->childVec.size() == 0)
		gluCylinder(qobj, trunk->baseR, 0, trunk->height, 10, 3);
	
	else {
		gluCylinder(qobj, trunk->baseR, trunk->baseR*trunk->baseScale, trunk->height, 10, 3);
	}
	//}
	glPopMatrix();

	

	glTranslatef(-1 * trunk->baseX, -1 * trunk->baseY, -1 * trunk->baseZ);

	for (int i = 0; i < trunk->childVec.size(); i++) {
		glPushMatrix();
		Branch*curr = trunk->childVec.at(i);
		draw_tree(curr);
		glPopMatrix();
	}
	glPopMatrix();
}

void change_wind(float x, float z) {
	
		changing = true;
		xdiff = (x - wind_vec.at(0)) / animframes;
		zdiff = (z - wind_vec.at(2)) / animframes;
	
}

void display(void) {
	if(rotating)
		rot += rotamount;
	osc+= change;
	if (osc >= oscframes  || osc <= -1 * oscframes ) {
		change *= -1;
		osc += change * 2;
	}
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluQuadricDrawStyle(qobj, GLU_FLAT);
	gluQuadricNormals(qobj, GLU_SMOOTH);
	
	glRotatef(rot, 0, 1.0, 0);

	//update wind vector
	if (changing) {
		frame++;
		wind_vec = { wind_vec.at(0) + xdiff,0,wind_vec.at(1) + zdiff,0 };
		//cout << frame << endl;
		if (frame == animframes) {
			
			changing = false;
			frame = 0;
		}
	}

	//draw wind vector
	glPushMatrix();
	glTranslatef(0, 250, 0);
	float mag = sqrt(wind_vec.at(0)*wind_vec.at(0) + wind_vec.at(2)*wind_vec.at(2))/sqrt(max_wind*max_wind*2)*300.0;
	float deg = atan2(-1 * wind_vec.at(2), wind_vec.at(0)) * 180.0 / PI;
	float m[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, m);
	glRotatef(90 + deg, 0, 1, 0);
	
	glColor4f(0, 0, 1, 1);
	gluCylinder(qobj, 10, 0, mag, 10, 3);
	glPopMatrix();

	//glTranslatef(0, -200, 0);
	
	draw_tree(trunk);
	
	glutSwapBuffers();
}
void keyboard(unsigned char key, int x, int y) {
	if (key == 'r') {
		rotating = !rotating;
	}
	if (key == 'e') {
		rotamount = -1 * rotamount;
	}
	if (key == 's') {
		cout << "HI" << endl;
		flat = !flat;
		if (flat) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glEnable(GL_LIGHTING);
		}
		else {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glDisable(GL_LIGHTING);
		}
	}
	if (key == 'w') {
		if (!changing) {
			cout << "Stopping Wind" << endl;
			change_wind(0, 0);
		}
	}
	if (key == 'c') {
		if (!changing) {
			float newx = rand() % max_wind *2 - max_wind ;
			if (newx >= 0 && newx < min_wind) {
				newx = min_wind;
			}
			if (newx < 0 && newx > -1 * min_wind) {
				newx = -1 * min_wind;
			}
			float newz = rand() % max_wind - max_wind / 2;
			if (newz >= 0 && newz < min_wind) {
				newz = min_wind;
			}
			if (newz < 0 && newz > -1 * min_wind) {
				newz = -1 * min_wind;
			}
			cout << "Wind Change To: " << newx << ", " << newz << endl;
			change_wind(newx, newz);
		}
	}
	glutPostRedisplay();
}
int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	//window settings
	glutInitWindowSize(500, 550);
	glutInitWindowPosition(250, 250);
	glutCreateWindow("Tree Simulation Demo");
	glutDisplayFunc(display);
	// register a keyboard function
	glutKeyboardFunc(keyboard);
	init();
	glutIdleFunc(display);
	glutMainLoop();

	return 0;
}