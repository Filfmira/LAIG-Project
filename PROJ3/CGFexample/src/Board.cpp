#include "Board.h"

Board::Board(vector<vector<PieceData> > b){
	this->board = b;
	this->piece = Piece();
}


	

    void Board::draw(int selectedX, int selectedY){
           
            bool selected = false;
     
            //desenhar tabuleiro + pe�as
            for (unsigned int i = 0; i < board.size(); i++){
                    int offsetX = abs(board[i][0].getY());//ajustar linha horizontalmente
                    int offsetZ = board[i][0].getY()*2;//ajustar linha verticalmente
                   
                    glPushMatrix();
                        glPushName(board[i][0].getY());//push coordY
                        for (unsigned int j = 0; j < board[i].size(); j++){
                            //board[i][j].print();
                            selected = (selectedX == board[i][j].getX() && selectedY == board[i][j].getY());
                           
                            int offsetXpeca = 2*j;
                            //cout<<"oX:"<<offsetX<<" oZ:"<<offsetZ<<" oPX:"<<offsetXpeca<<endl;
                            glPushMatrix();
                                glPushName(board[i][j].getX()); //push coordX
                                //tabuleiro    
                                glPushMatrix();
                                        glTranslated(offsetX+offsetXpeca, -0.5, offsetZ);
                                        piece.draw(false, "tabuleiro", selected);
                                glPopMatrix();
                           
                                //pe�as
                                if (board[i][j].getTipo() != "vazia"){  
                                    glPushMatrix();
                                        glPushName(100);
										if (board[i][j].hasAnimation() && !board[i][j].getAnimation()->isEnd()){
											piece.drawAnimation(board[i][j].getCor(),board[i][j].getTipo(),board[i][j].getAnimation());
										}
										else{
											glTranslated(offsetX+offsetXpeca, 0, offsetZ);
											piece.draw(board[i][j].getCor(), board[i][j].getTipo(), false);
										}
                                        glPopName();
                                    glPopMatrix();
                                }
                                glPopName();//pop coordX
                            glPopMatrix();
                        }
                        glPopName();//pop coordY
                    glPopMatrix();
            }
            //cin.get();
           
    }


vector<vector<PieceData> > Board::getBoard(){
	return this->board;
}

void Board::setBoard(vector<vector<PieceData> > board){
	this->board = board;
}

/*
 *	substitui pe�a no tabuleiro por newPiece
 *	(a pe�a substituida ser� a pe�a com as msms coordenadas de newPiece)
*/
void Board::addPiece(PieceData newPiece){
	vector<PieceData> linha = board[newPiece.getY() + (int)board.size()/2];
	for (unsigned int i = 0; i < linha.size(); i++){
		if (linha[i].getX() == newPiece.getX()){
			board[newPiece.getY() + (int)board.size()/2][i] = newPiece;
		}
	}
}

/*
 *	substitui pe�a no tabuleiro por pe�a vazia
 *	(a pe�a substituida ser� a pe�a com as msms coordenadas de oldPiece)
*/
void Board::removePiece(PieceData oldPiece){
	vector<PieceData> linha = board[oldPiece.getY() + (int)board.size()/2];
	for (unsigned int i = 0; i < linha.size(); i++){
		if (linha[i].getX() == oldPiece.getX()){
			board[oldPiece.getY() + (int)board.size()/2][i] = PieceData(oldPiece.getX(),oldPiece.getY(), true, "vazia");
		}
	}
}

PieceData Board::getPiece(int x, int y){
	cout << (int)board.size()/2 << endl;
	return board[y + (int)board.size()/2][x + (int)board.size()/2]; //N SEI SE EST� BEM || MT PROVAVELMENTE NAO
}

void Board::addJogadaHistorico(Jogada pd){
	this->historico.push_back(pd);
}

vector<Jogada> Board::getHistorico(){
	return historico;
}

void Board::popBackHistorico(){
	this->historico.pop_back();
}


void Board::changeTextures(int i){
	this->piece.changeTextures(i);
}


void Board::restartAnimacoes(){
	for (unsigned int i = 0; i < historico.size(); i++)
		historico[i].restartAnimacoes();
}