#ifndef __BRANCH_H__
#define __BRANCH_H__
#include <vector>
using namespace std;
class Branch
{
public:
	
	
	vector<Branch*> childVec;
	Branch* parent;

	//base X and Y and radius, rotation X and Y and Z
	float baseX, baseY, baseZ, baseR, height,topR, rotX, rotY, rotZ;
	//scaling
	float baseScale, heightScale;
	bool isTrunk;
	int treelen;

	//random factor
	int b;
	//constructors
	Branch();
	//Branch(vector<float> b, vector<float> t, vector<float> r, Branch* p);
	Branch(Branch*p);
	
};

#endif