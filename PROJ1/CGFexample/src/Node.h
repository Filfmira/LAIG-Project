
#include "CGFappearance.h"
#include "CGFobject.h"

#include <vector>

class Node
{
private:
	string id;
	vector<Node *> descendentes;
	CGFobject * primitiva;
	CGFappearance * aparencia;
	float matrix[16];



public:
	Node(void);
	~Node(void);
	Node(string id);
	Node(string id, CGFobject* primitiva, CGFappearance * aparencia,float matrix[16]);

	void addDescendente(Node * n);
	void setPrimitiva(CGFobject * primitiva);
	void setAparencia(CGFappearance * aparencia);
	void setMatrix(float matrix[16] );
	string getId(){return id;}

	string mostrarNo();
};

