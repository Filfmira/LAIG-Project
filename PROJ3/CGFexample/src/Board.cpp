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
                            //cout<<endl;
                            for (unsigned int j = 0; j < board[i].size(); j++){
                                    //board[i][j].print();
                                    selected = (selectedX == board[i][j].getX() && selectedY == board[i][j].getY());
                           
                                    int offsetXpeca = 2*j;
                                    //cout<<"oX:"<<offsetX<<" oZ:"<<offsetZ<<" oPX:"<<offsetXpeca<<endl;
                                    glPushMatrix();
                                            glPushName(board[i][j].getX()); //push coordX
                                            //tabuleiro    
                                            glPushMatrix();
                                                    glTranslated(offsetXpeca, -0.5, 0);
                                                    piece.draw(false, "tabuleiro", selected);
                                            glPopMatrix();
                           
                                            //peças
                                            if (board[i][j].getTipo() != "vazia"){  
												if(!board[i][j].hasAnimation())
												{
                                                    glPushMatrix();
                                                            glPushName(100);
                                                            glTranslated(offsetXpeca, 0, 0);
                                                            piece.draw(board[i][j].getCor(), board[i][j].getTipo(), false);
                                                            glPopName();
                                                    glPopMatrix();
												}
                                            }
                                            glPopName();//pop coordX
                                    glPopMatrix();
                            }
                            glPopName();//pop coordY
                    glPopMatrix();
            }
            //cin.get();

			for(int i=0;i<board.size();i++)
				for(int j=0;j<board[i].size();j++)
				{
					if(board[i][j].hasAnimation())
					{
						cout<<"entrou"<<endl;
						//board[i][j].getAnimation().draw();
						piece.drawAnimation(board[i][j].getCor(),board[i][j].getTipo(),board[i][j].getAnimation());
						//glPopMatrix();

					}
				}
           
    }


vector<vector<PieceData> > Board::getBoard(){
	return this->board;
}

void Board::setBoard(vector<vector<PieceData> > board){
	this->board = board;
}

/*
 *	substitui peça no tabuleiro por newPiece
 *	(a peça substituida será a peça com as msms coordenadas de newPiece)
*/
void Board::addPiece(PieceData newPiece){
	vector<PieceData> linha = board[newPiece.getY() + (int)board.size()/2];
	for (unsigned int i = 0; i < linha.size(); i++){
		if (linha[i].getX() == newPiece.getX()){
			board[newPiece.getY() + (int)board.size()/2][i] = newPiece;
			cout << "trocou peça\n";
		}
	}
}

PieceData Board::getPiece(int x, int y){
	cout << (int)board.size()/2 << endl;
	return board[y + (int)board.size()/2][x + (int)board.size()/2]; //N SEI SE ESTÁ BEM || MT PROVAVELMENTE NAO
}

void Board::addPieceHistorico(PieceData pd){
	this->historico.push_back(pd);
}