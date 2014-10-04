#include "Node.h"
#include <sstream>

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

Node::Node(string id, CGFobject* primitiva, CGFappearance * aparencia,float matrix[16])
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


void Node::setMatrix(float m[16] )				
{
	for(int i=0;i<16;i++)
			matrix[i]=m[i];
}


void Node::setPrimitiva(CGFobject * primitiva)		
{
	this->primitiva=primitiva;
}

void Node::setAparencia(CGFappearance * aparencia)	
{
	this->aparencia=aparencia;
}



string Node::mostrarNo(){
	std::ostringstream s;
	s<<"\n=======NODE=======\n\n";
	s<<"Id:"<<id<<"\n";

	s<<"Matrix:";
	for(int i=0;i<16;i++)
			{
				if(i%4==0)
					s<<"\n";
				s<<matrix[i]<<"  ";
			}
	s<<"\n";

	s<<"Descendentes:";
	for(int i=0;i<this->descendentes.size();i++)
		s<<descendentes.at(i)->getId()<<" , ";
	s<<"\n";


	s<<"\n\n=================";
	return s.str();

}