
#include "CGFappearance.h"
#include "CGFobject.h"
#include "Primitives.h"
#include <vector>

class Node
{
private:
	string id;
	vector<Node*> descendentes;
	vector<Node*> ascendentes; 
	vector<Primitive*> primitivas;
	CGFappearance * aparencia;
	float matrix[16];



public:
	Node(void);
	~Node(void);
	Node(string id);
	Node(string id, vector<Primitive*> primitivas, CGFappearance * aparencia,float matrix[16]);

	//void addDescendente(Node * n);
	void addPrimitiva(Primitive* primitiva);
	void setAparencia(CGFappearance * aparencia);
	void setMatrix(float matrix[16] );
	string getId(){return id;}
	void addDescendente(Node* n){descendentes.push_back(n);}
	void addAscendente(Node* n){ascendentes.push_back(n);}
	vector<Node*> getDescendentes(){return descendentes;}
	vector<Node*> getAscendentes(){return ascendentes;}

	string mostrarNo();

	unsigned int getNumeroDePrimitivas(){return primitivas.size();};
	Primitive* getPrimitiva(int i);
	float* getMatrix(){return matrix;};
};

