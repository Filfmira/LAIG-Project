#include "Node.h"


Node::Node(void)
{
}


Node::~Node(void)
{
}

Node::Node(string id)
{
	//printf("estou a por um fucking id que é %s\n\n",id);
	this->id=id;
	//printf("estou a por um fucking id que é %s\n\n",id);
}

Node::Node(string id, CGFobject* primitiva, CGFappearance * aparencia,float matrix[][4])
{
	this->id=id;
	this->primitiva=primitiva;
	this->aparencia=aparencia;
	this->setMatrix(matrix);

}



void Node::addDescendente(Node *n)
{	
	this->descendentes.push_back(n);
}


void Node::setMatrix(float matrix[][4] )				
{
	for(int i=0;i<4;i++)
		for(int j=0;j<4;j++)
			this->matrix[i][j]=matrix[i][j];
}


void Node::setPrimitiva(CGFobject * primitiva)		
{
	this->primitiva=primitiva;
}

void Node::setAparencia(CGFappearance * aparencia)	
{
	this->aparencia=aparencia;
}