#ifndef NODE_H
#define NODE_H


#include "CGFappearance.h"
#include "CGFobject.h"
#include "Primitives.h"
#include "Evaluator.h"
#include <vector>
#include "Appearances.h"
#include "Animation.h"

class Node
{
private:
	string id;
	vector<Node*> descendentes;
	vector<Node*> ascendentes; 
	vector<Primitive*> primitivas;
	Appearance * aparencia;
	vector<Animation *> animations;
	float matrix[16];
	bool displayList;
	int displayListID;
	int indiceAnimacao;

public:
	Node(void);
	~Node(void);
	Node(string id);
	Node(string id, vector<Primitive*> primitivas, Appearance * aparencia,float matrix[16]);


	int getIndiceAnimacao(){return indiceAnimacao;};
	void aumentaIndiceAnimacao(){indiceAnimacao++;if(indiceAnimacao>animations.size()-1)animations.size()-1;};
	void addPrimitiva(Primitive* primitiva);
	void setAparencia(Appearance * aparencia);
	void setAnimation(vector<Animation *> anomation);
	void setMatrix(float matrix[16] );
	string getId(){return id;}
	void addDescendente(Node* n){descendentes.push_back(n);}
	void addAscendente(Node* n){ascendentes.push_back(n);}
	vector<Node*> getDescendentes(){return descendentes;}
	vector<Node*> getAscendentes(){return ascendentes;}
	Node* getDescendenteIndex(int i){if (i < 0 || i > descendentes.size()) return NULL; return descendentes[i];}
	string mostrarNo();

	unsigned int getNumeroDePrimitivas(){return primitivas.size();};
	Primitive* getPrimitiva(int i);
	float* getMatrix(){return matrix;};
	Appearance* getAppearance(){return aparencia;}
	vector<Animation *> getAnimation(){return animations;}

	bool getDisplayList(){return displayList;}
	void setDisplayList(bool b){this->displayList=b;}

	int getDisplayListID(){return this->displayListID;}
	void setDisplayListID(int id){this->displayListID = id;}
};


#endif
