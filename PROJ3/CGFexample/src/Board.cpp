#include "Board.h"

Board::Board(vector<vector<PieceData> > b){
	this->board = b;
	this->piece = Piece();
}


void Board::draw(int selectedX, int selectedY){
	
	bool selected = false;

	//desenhar tabuleiro + peças
	for (unsigned int i = 0; i < board.size(); i++){
		int offsetX = abs(board[i][0].getY());
		int offsetZ = board[i][0].getY()*2;
		glPushMatrix();
			glPushName(board[i][0].getY());//push coordY
			glTranslated(offsetX, 0, offsetZ); //ajustar linha vertical e horizontalmente
			
			for (unsigned int j = 0; j < board[i].size(); j++){
				//board[i][j].print();
				selected = (selectedX == board[i][j].getX() && selectedY == board[i][j].getY());
			
				int offsetXpeca = 2*j;
				glPushMatrix();
					glPushName(board[i][j].getX()); //push coordX
					//tabuleiro	
					glPushMatrix();
						glTranslated(offsetXpeca, -0.5, 0);
						piece.draw(false, "tabuleiro", selected);
					glPopMatrix();
			
					//peças
					if (board[i][j].getTipo() != "vazia"){				
						glPushMatrix();
							glPushName(100);
							glTranslated(offsetXpeca, 0, 0);
							piece.draw(board[i][j].getCor(), board[i][j].getTipo(), false);
							glPopName();
						glPopMatrix();
					}
					glPopName();//pop coordX
				glPopMatrix();
			}
			glPopName();//pop coordY
		glPopMatrix();
	}
	
}

vector<vector<PieceData> > Board::getBoard(){
	return this->board;
}

void Board::setBoard(vector<vector<PieceData> > board){
	this->board = board;
}

void Board::addPiece(PieceData newPiece){
	
	//TODO
}

PieceData Board::getPiece(int x, int y){
	cout << (int)board.size()/2 << endl;
	return board[y + (int)board.size()/2][x + (int)board.size()/2]; //N SEI SE ESTÁ BEM || MT PROVAVELMENTE NAO
}