#include "Graph.h"

void Graph::addNode(Node * noPai, string novoId)
{
		Node novo= Node(novoId);
		nodes.push_back(novo);
		Node * n=&nodes.at(nodes.size()-1);
		noPai->addDescendente(n);
}

void Graph::addNode(string paiId, string novoId)
{
	addNode(searchForNode(paiId),novoId);
}

void Graph::addNode(Node * noPai,string novoID, CGFobject* primitiva, CGFappearance * aparencia,float matrix[][4]){
	Node novo= Node(novoID,primitiva,aparencia,matrix);
	nodes.push_back(novo);
	Node * n=&nodes.at(nodes.size()-1);
	noPai->addDescendente(n);
}

void Graph::addNode(string novoId, CGFobject* primitiva, CGFappearance * aparencia,float matrix[][4]){
	Node novo= Node(novoId,primitiva,aparencia,matrix);
	nodes.push_back(novo);
}


Node* Graph::searchForNode(string id)
{
	for(int i =0;i<this->nodes.size();i++)
		if(nodes.at(i).getId()==id)
			return &nodes.at(i);
	return NULL;
}