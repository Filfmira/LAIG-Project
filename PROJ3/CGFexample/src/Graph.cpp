#include "Graph.h"

void Graph::addNode(Node * noPai, string novoId)
{
		Node* novo= new Node(novoId);
		nodes.push_back(novo);
}

void Graph::addNode(string paiId, string novoId)
{
	addNode(searchForNode(paiId),novoId);
}

void Graph::addNode(Node * noPai,string novoID, vector<Primitive*> primitivas, Appearance * aparencia,float matrix[16]){
	Node* novo= new Node(novoID,primitivas,aparencia,matrix);
	nodes.push_back(novo);

}

void Graph::addNode(string novoId, vector<Primitive*> primitivas, Appearance * aparencia,float matrix[16]){
	Node* novo= new Node(novoId,primitivas,aparencia,matrix);
	nodes.push_back(novo);
}


Node* Graph::searchForNode(string id)
{
	for(unsigned int i =0;i<this->nodes.size();i++)
		if(nodes.at(i)->getId()==id)
			return nodes.at(i);
	return NULL;
}

Node* Graph::searchForNode(int num){
	if (num < 0 || num >= (int) nodes.size())
		return NULL;
	return nodes[num];
}

void Graph::addNode(Node* n) {nodes.push_back(n);}