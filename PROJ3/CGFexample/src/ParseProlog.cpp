#include "ParseProlog.h"
#include <vector>
#include <string>
#include <iostream>

using namespace std;

vector<vector<PieceData> > parseBoard(string board_string){
	vector<vector<PieceData> > ret;
	cout << "Parsing Board:" << endl << endl;
	//cout << board_string << endl << endl;
	board_string = board_string.substr(1, board_string.size()-4); //retira [ ] 
	
	////força a string a ter um ,[ antes da primeira linha do tabuleiro
	// de forma a tornar o processo igual em todas as linhas
	string temp = ",[";
	temp.append(board_string);
	board_string = temp;
	////////////////////////////////////////////////////////////////

	while(board_string.size() > 1){
		board_string = board_string.substr(2); //tira ,[ no inicio da linha do tabuleiro

		vector<PieceData> linhaTemp;
		while(1){
			int pos2 = board_string.find_first_of(']');
			if (pos2 == 0){ //fim da linha
				board_string = board_string.substr(1);
				break;
			}
			int pos1 = board_string.find_first_of('[');
			string pecaTemp = board_string.substr(pos1, pos2-pos1+1); //substr(primeiroChar, numeroDeChars);

			PieceData piece = parsePiece(pecaTemp);
			linhaTemp.push_back(piece);
			board_string = board_string.substr(pos2+1);
		}
		if (linhaTemp.size() > 0) ret.push_back(linhaTemp);
		cout << endl;
	}
	return ret;
}


PieceData parsePiece(string piece){
	//cout << "Peca para parse: " << piece << endl;
	int x,y;
	bool cor;
	string tipo, temp;
	//x
	int pos = piece.find_first_of(',');
	temp = piece.substr(1, pos); 
	x = atoi(temp.c_str()); 
	piece = piece.substr(pos+1);
	//y
	pos = piece.find_first_of(',');
	temp = piece.substr(0, pos); 
	y = atoi(temp.c_str()); 
	piece = piece.substr(pos+1);
	//cor
	pos = piece.find_first_of(',');
	temp = piece.substr(0, pos); 
	cor = (temp == "branca"? true : false); 
	piece = piece.substr(pos+1);
	//tipo
	pos = piece.find(']');
	tipo = piece.substr(0, pos); 

	PieceData tempPiece = PieceData(x,y,cor,tipo);
	tempPiece.print();
	return tempPiece;

}

