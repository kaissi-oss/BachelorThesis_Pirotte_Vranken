#include "ist/InnerSphereTree.h"
#include <iostream>
#include <math.h>
#include <limits>

using namespace std;

namespace chai3d {

	/*
		Constructor of an inner sphere tree.
	*/
	InnerSphereTree::InnerSphereTree() {
		
	}

	/*
		Destructor of the inner sphere tree.
	*/
	InnerSphereTree::~InnerSphereTree() {
		delete rootSphere;
	}

	void InnerSphereTree::printAABBCollisionTree(int diepte) {
		cout << "INNER SPHERE TREE" << endl;
		for (int i = 0; i < spheres.size(); i++) {
			cout << "Sphere: " << spheres[i]->getPosition() << endl;
		}

		
	}

	/*
		This function computes if a collision has occured between two inner sphere trees.

		\param ist2					he other inner sphere tree to compute the possible collision.
		\param setting				The traversalsettings.
		\param collisionfeedback	How close the collision is.
		\param maxdiepte			The maximumdepth the function is allowed to check for collision.

		\return If to inner sphere trees have collided.
	*/
	bool InnerSphereTree::computeCollision(cGenericCollision* ist2, traversalSetting setting, double &collisionfeedback, int maxdiepte, cVector3d myLocal, cVector3d BLocal, cVector3d& positie) {
		// Sanity check
		if (ist2 == NULL) return false;
		if (this->getCollisionTreeType() != ist2->getCollisionTreeType()) return false;

		switch (setting) {
		case traversalSetting::DISTANCE: {
			InnerSphereTree* IST_B = dynamic_cast<InnerSphereTree*>(ist2);
			InnerSphereTree* IST_A = this;

			Sphere* parent_A = IST_B->getRootSphere();
			Sphere* parent_B = IST_A->getRootSphere();

			double mindist = std::numeric_limits<double>::max();
			int huidige = 0;
		}
		case traversalSetting::COMBINED: return false;
		case traversalSetting::VOLUME_PEN: return false;
		default: return false;
		}
		return true;
	}

	/*
		Get the root sphere of this inner sphere tree.

		\return The rootsphere.
	*/
	Sphere* InnerSphereTree::getRootSphere() {
		return rootSphere;
	}
	int InnerSphereTree::buildTree(std::vector<Sphere*> leafs, const int a_depth)
	{
		for (int i = 0; i < leafs.size(); i++) {
			spheres.push_back(leafs[i]);
		}
		return 0;
	}

	//implementation of the BNG algorthm
	//n-->the number of prototypes
	void InnerSphereTree::BNG(double size, Sphere* node, std::vector<Sphere*> leafs)
	{
#define TMAX 500
		struct prototype {
			cVector3d pos;
			std::vector<Sphere*> lfs;
		};

		prototype w[4];

		double x = node->getPosition().x();
		double y = node->getPosition().y();
		double z = node->getPosition().z();

		double r = node->getRadius();
		
		//chose start pos prototypes
		w[0].pos.set(x + r, y + r, z);
		w[1].pos.set(x + r, y - r, z + r);
		w[2].pos.set(x - r, y + r, z - r);
		w[3].pos.set(x - r, y - r, z - r);

		//define epsilon
		double eps = 0.00001 * size;
		std::vector<std::vector<int>> weights;
		int t = 0;
		bool stop = false;

		while (!stop && (t <= TMAX)) {
			int teller = 0;
			//for every sphere we calculate the distance to each prototype and decide the weights 
			for (int j = 0; j < leafs.size(); j++) {
				double d[4];
				int n[4] = { 0,0,0,0 };
				d[0] = (leafs[j]->getPosition() - w[0].pos).length();
				d[1] = (leafs[j]->getPosition() - w[1].pos).length();
				d[2] = (leafs[j]->getPosition() - w[2].pos).length();
				d[3] = (leafs[j]->getPosition() - w[3].pos).length();

				for (int i = 0; i < 4; i++) {
					for (int k = i + 1; k < 4; k++) {
						if (d[i] < d[k]) n[i]++;
						else n[k]++;
					}
				}

				for (int i = 0; i < 4; i++) weights[i][j] = n[i];
			}

			//calculate new prototype positions
			float L = 2 * pow((0.01 / 2.0), t / TMAX);
			for (int k = 0; k < 4; k++) {
				double sumf = 0;
				cVector3d sumv = cVector3d(0, 0, 0);
				for (int i = 0; i < leafs.size(); i++) {
					float volume = pow(leafs[i]->getRadius(), 3)*(4.0 / 3.0)* M_PI;

					float hL = exp(-weights[0][i] / L);
					float f = hL*volume;
					cVector3d vec = cVector3d(0, 0, 0);
					vec = f*(leafs[i]->getPosition());

					sumf += f;
					sumv += vec;
				}

				sumv = sumv / sumf;
				
				if((w[k].pos - sumv).length() < eps) teller++;
				if (teller == 4) stop = true;

				w[k].pos = sumv;
			}
			t++;
		}

		float max[4] = { 0,0,0,0 };
		for (int j = 0; j < leafs.size(); j++) {
			float mindist = numeric_limits<float>::infinity();
			float rad;
			int num;
			for (int i = 0; i < 4; i++) {
				float d = (leafs[j]->getPosition() - w[i].pos).length();
				if (d < mindist) {
					mindist = d;
					num = i;
					rad = leafs[j]->getRadius();
				}
			}
			if (max[num] < (mindist + rad)) max[num] = (mindist + rad);
			w[num].lfs.push_back(leafs[j]);
			}

			//we got all wheights with a vector to their leaves
			//with all including radius of their leaves

		for (int i = 0; i < 4; i++) {
			Sphere* s = new Sphere();
			s->setPosition(w[i].pos);
			s->setRadius(max[i]);
			s->setState(sphereState::SPHERE_INTERNAL);
			
			//set as child of node

			BNG(size, s, w[i].lfs);
		}
	}
	

	void InnerSphereTree::render(cRenderOptions& a_options) {
#ifdef C_USE_OPENGL
		
		glDisable(GL_LIGHTING);
		glLineWidth(1.0);
		glColor4fv(cColorf(1.0, 0, 0).getData());
		for (int i = 0; i < spheres.size(); i++) {
			spheres[i]->render();
		}

		glEnable(GL_LIGHTING);
#endif
	}

}