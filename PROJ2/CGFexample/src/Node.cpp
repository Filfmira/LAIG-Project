#include "Node.h"
#include <sstream>

Node::Node(void)
{
	this->aparencia = NULL;
	this->displayList = false;
}


Node::~Node(void)
{
	for (unsigned int i = 0; i < primitivas.size(); i++){
		delete(primitivas[i]);
	}
}

Node::Node(string id)
{
	this->id=id;
	this->aparencia = NULL;
	this->displayList = false;
	this->displayListID = -1;
}

Node::Node(string id, vector<Primitive*> primitivas, Appearance * aparencia,float matrix[16])
{
	this->id=id;
	this->primitivas=primitivas;
	this->aparencia=aparencia;
	this->setMatrix(matrix);
	this->displayList = false;
	this->displayListID = -1;
}


void Node::setMatrix(float m[16] )				
{
	
	for(int i=0;i<16;i++)
		matrix[i]=m[i];
}


void Node::addPrimitiva(Primitive* primitiva)
{
	this->primitivas.push_back(primitiva);
}

void Node::setAparencia(Appearance * aparencia)	
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
	s<<"\nSize:"<<descendentes.size()<<endl;
	s<<"\n";

	s<<"Ascendentes:";
	for(int i=0;i<this->ascendentes.size();i++)
		s<<ascendentes.at(i)->getId()<<" , ";
	s<<"\nSize:"<<ascendentes.size()<<endl;
	s<<"\n";

	s<<"\n\n=================\n";
	return s.str();

}


Primitive* Node::getPrimitiva(int i){
	if (i < 0 || i >= primitivas.size())
		return NULL;
	return primitivas[i];
}