#include "ParseProlog.h"
#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream> /* teste output tabuleiro para ficheiro */
using namespace std;

vector<vector<PieceData> > parseBoard(string board_string, vector<PieceData> &pecasAdicionadas, vector<PieceData> &pecasRemovidas, vector<PieceData> &jogadasComputador){
	vector<vector<PieceData> > ret;
	cout << "Parsing Board:" << endl << endl;
	//cout << board_string << endl << endl;
	board_string = board_string.substr(1, board_string.size()-1); //retira [ ] 

	////força a string a ter um ,[ antes da primeira linha do tabuleiro
	// de forma a tornar o processo igual em todas as linhas
	string temp = ",[";
	temp.append(board_string);
	board_string = temp;
	////////////////////////////////////////////////////////////////
	//cout << endl << board_string << endl;
	while(1){
		//cout << "||||||||||||||||||||||||||\n" << board_string << endl;
		if (board_string.substr(0,2) ==  "]]")
			break;
		board_string = board_string.substr(3); //tira ,[ no inicio da linha do tabuleiro
		vector<PieceData> linhaTemp;
		while(1){
			int pos2 = board_string.find_first_of(']');
			if (pos2 == 0){ //fim da linha
				//board_string = board_string.substr(1);
				break;
			}
			int pos1 = board_string.find_first_of('[');
			string pecaTemp = board_string.substr(pos1, pos2-pos1+1); //substr(primeiroChar, numeroDeChars);

			PieceData piece = parsePiece(pecaTemp);
			linhaTemp.push_back(piece);
			board_string = board_string.substr(pos2+1);
		}
		if (linhaTemp.size() > 0) ret.push_back(linhaTemp);
		/*cout << endl;*/

	}



	cout << "Parsing added pieces..." << endl;
	board_string = board_string.substr(4); //retira ]],[

	if (board_string.find("]") == 0){ //se a lista acaba logo
		cout << "	None added" << endl;
	}
	else{
		//vector<PieceData> pecasAdicionadas;
		while(1){
			int pos2 = board_string.find_first_of(']');
			if (pos2 == 0){ //fim da linha
				//board_string = board_string.substr(1);
				break;
			}
			int pos1 = board_string.find_first_of('[');
			string pecaTemp = board_string.substr(pos1, pos2-pos1+1); //substr(primeiroChar, numeroDeChars);

			PieceData piece = parsePiece(pecaTemp);
			piece.print();
			pecasAdicionadas.push_back(piece);
			board_string = board_string.substr(pos2+1);
		}
	}
	cout << "Parsing removed pieces..." << endl;	
	board_string = board_string.substr(3); //retira ]],[

	if (board_string.find("]") == 0){ //se a lista acaba logo
		cout << "	None removed" << endl;
	}
	else{
		//vector<PieceData> pecasRemovidas;
		while(1){
			int pos2 = board_string.find_first_of(']');
			if (pos2 == 0){ //fim da linha
				//board_string = board_string.substr(1);
				break;
			}
			int pos1 = board_string.find_first_of('[');
			string pecaTemp = board_string.substr(pos1, pos2-pos1+1); //substr(primeiroChar, numeroDeChars);

			PieceData piece = parsePiece(pecaTemp);
			piece.print();
			pecasRemovidas.push_back(piece);
			board_string = board_string.substr(pos2+1);
		}
	}

	cout << "Parsing computer moves..." << endl;
	
	board_string = board_string.substr(3); //retira ]],[

	if (board_string.find("]") == 0){ //se a lista acaba logo
		cout << "	None played" << endl;
	}
	else{
		while(1){
			int pos2 = board_string.find_first_of(']');
			if (pos2 == 0){ //fim da linha
				//board_string = board_string.substr(1);
				break;
			}
			int pos1 = board_string.find_first_of('[');
			string pecaTemp = board_string.substr(pos1, pos2-pos1+1); //substr(primeiroChar, numeroDeChars);

			PieceData piece = parsePiece(pecaTemp);
			piece.print();
			jogadasComputador.push_back(piece);
			board_string = board_string.substr(pos2+1);
		}
	}

	//cout << "fim: " << board_string << endl;
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
	/*tempPiece.print();*/
	return tempPiece;

}

string boardToString(vector<vector<PieceData> > board){
	ostringstream ss;
	ss << "[";
	for (unsigned int i = 0; i < board.size(); i++){
		if (i != 0) ss << ",";
		ss << "[";
		for (unsigned int j = 0; j < board[i].size(); j++){
			if (j != 0) ss << ",";
			ss << pieceToString(board[i][j]);
		}
		ss << "]";
	}
	ss << "]";

    /*std::ofstream out("output.txt");
    out << ss.str();
    out.close();*/

	return ss.str();
}

string jogadaToString(PieceData jogada, vector<vector<PieceData> > board){
	ostringstream ss;
	ss << "[jogada,"<< pieceToString(jogada) << "," << boardToString(board) << "].\n";
	return ss.str();
}

string jogadaComputadorToString(vector<vector<PieceData> > board, bool cor){
	ostringstream ss;
	ss << "[computador," << (cor? "branca," : "preta,") << boardToString(board) << "].\n";
	return ss.str();
}

string traduzirCoordenadas(int x, int y){
	ostringstream ss;
	ss << "[traduzir,"<< x << "," << y << "].\n";
	return ss.str();
}

string pieceToString(PieceData piece){
	ostringstream ss;
	ss << "[" << piece.getX() << "," << piece.getY() << ","
		<< (piece.getTipo() == "vazia"? "vazia" : (piece.getCor()? "branca" : "preta")) << "," << piece.getTipo() << "]";
	return ss.str();
}


bool parseAnswerJogada(string answer, vector<vector<PieceData> > &newBoard, string &gameOver, 
	vector<PieceData> &pecasAdicionadas, vector<PieceData> &pecasRemovidas, vector<PieceData> &jogadasComputador){

	int pos = answer.find(".");
	answer = answer.substr(1, pos-2); //retira [ e .\n]
	pos = answer.find(",");
	string msg = answer.substr(0,pos);
	if (msg != "not-ok"){
		answer = answer.substr(pos+1);
		newBoard = parseBoard(answer, pecasAdicionadas, pecasRemovidas, jogadasComputador);
		if (msg != "ok"){
			cout << "FIM DO JOGO!\n" << answer << endl;
			gameOver=msg;
		}
		return true;
	}
	return false;
}

