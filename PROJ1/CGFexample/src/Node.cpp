#include "Node.h"
#include <sstream>

Node::Node(void)
{
}


Node::~Node(void)
{
	for (unsigned int i = 0; i < primitivas.size(); i++){
		delete(primitivas[i]);
	}
}

Node::Node(string id)
{
	//printf("estou a por um fucking id que é %s\n\n",id);
	this->id=id;
	//printf("estou a por um fucking id que é %s\n\n",id);
}

Node::Node(string id, vector<Primitive*> primitivas, CGFappearance * aparencia,float matrix[16])
{
	this->id=id;
	this->primitivas=primitivas;
	this->aparencia=aparencia;
	this->setMatrix(matrix);

}


/*
void Node::addDescendente(Node *n)
{	
	this->descendentes.push_back(n);
}

*/
void Node::setMatrix(float m[16] )				
{
	
	for(int i=0;i<16;i++)
		matrix[i]=m[i];
}


void Node::addPrimitiva(Primitive* primitiva)
{
	this->primitivas.push_back(primitiva);
}

void Node::setAparencia(CGFappearance * aparencia)	
{
	this->aparencia=aparencia;
}



string Node::mostrarNo(){
	std::ostringstream s;
	/*s<<"\n=======NODE=======\n\n";
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
	s<<"\nSize:"<<descendentes.size()<<endl;
	s<<"\n";

	s<<"Ascendentes:";
	for(int i=0;i<this->ascendentes.size();i++)
		s<<ascendentes.at(i)->getId()<<" , ";
	s<<"\nSize:"<<ascendentes.size()<<endl;
	s<<"\n";

	s<<"\n\n=================\n";*/
	return s.str();

}


Primitive* Node::getPrimitiva(int i){
	if (i < 0 || i >= primitivas.size())
		return NULL;
	return primitivas[i];
}