
#include "CGFappearance.h"
#include "CGFobject.h"
#include "Primitives.h"
#include <vector>

class Node
{
private:
	string id;
	vector<string> descendentes;
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
	void addDescendente(string id){descendentes.push_back(id);}
	vector<string> getDescendentes(){return descendentes;}

	string mostrarNo();

	unsigned int getNumeroDePrimitivas(){return primitivas.size();};
	Primitive* getPrimitiva(int i);
	float* getMatrix(){return matrix;};
};

