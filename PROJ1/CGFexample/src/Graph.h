#include "Node.h"
#include <vector>

class Graph
{
private:
	vector<Node*> nodes;
	Node* root;

public:
	//vector<Node> nodes;
	Graph(void){};
	void addNode(Node* n);
	void addNode(Node * noPai, string novoID);
	void addNode(string paiID, string novoID);
	void addNode(Node * noPai,string id, vector<Primitive*> primitivas, CGFappearance * aparencia,float matrix[16]);
	void addNode(string id, vector<Primitive*> primitivas, CGFappearance * aparencia,float matrix[16]);
	Node* searchForNode(string id);	//pesquisa node por nome, retorna null se nao existir;
	Node* searchForNode(int num);	//pesquisa node por posicao no vector, retorna null se nao existir;

	unsigned int getNumberOfNodes(){return nodes.size();}
	void setRoot(Node* n){root = n;};
	Node* getRoot(){return root;};
};
