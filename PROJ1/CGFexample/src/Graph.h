
#include "Node.h"
#include <vector>

class Graph
{
private:
	vector<Node> nodes;


public:
	Graph(void){};
	void addNode(Node n) {nodes.push_back(n);}
	void addNode(Node * noPai, string novoID);
	void addNode(string paiID, string novoID);
	void addNode(Node * noPai,string id, CGFobject* primitiva, CGFappearance * aparencia,float matrix[][4]);
	void addNode(string id, CGFobject* primitiva, CGFappearance * aparencia,float matrix[][4]);
	Node* searchForNode(string id);	//pesquisa node por nome, retorna null se nao existir;
};
