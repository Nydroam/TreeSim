#include "Branch.h"

Branch::Branch(){}

/*Branch::Branch(vector<float> b, vector<float> t, vector<float> r, Branch* p) {
	this->base = b;
	this->top = t;
	this->rot = r;
	this->parent = p;
}*/

Branch::Branch(Branch*p) {
	this->parent = p;
}

