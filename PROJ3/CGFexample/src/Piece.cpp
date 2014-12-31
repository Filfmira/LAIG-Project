#include "Piece.h"
#include <iostream>
#include <sstream>
Piece::Piece(){
	this->aparenciaActiva = 0;
	
	this->hexagon = Poligon(6);

	this->appearanceLados.push_back(new Appearance("lados0"));
	this->appearanceLados.push_back(new Appearance("lados1"));
	this->appearanceLadosSelected.push_back(new Appearance("ladosSel0"));
	this->appearanceLadosSelected.push_back(new Appearance("ladosSel1"));


	this->appearanceLados[0]->setTexture(new Texture("lado", "madeira0/lado.jpg", 1, 1));
	this->appearanceLados[1]->setTexture(new Texture("lado", "madeira1/lado.jpg", 1, 1));
	this->appearanceLadosSelected[0]->setTexture(new Texture("ladosSel", "madeira0/selected.jpg", 1, 1));
	this->appearanceLadosSelected[1]->setTexture(new Texture("ladosSel", "madeira1/selected.jpg", 1, 1));
	
	this->ladoTabuleiro.push_back( new Texture("ladosSel", "madeira0/selected.jpg", 1, 1)) ;
	this->ladoTabuleiro.push_back( new Texture("ladosSel", "madeira1/selected.jpg", 1, 1));
	this->lado.push_back( new Texture("ladosSel", "madeira0/lado.jpg", 1, 1));
	this->lado.push_back( new Texture("ladosSel", "madeira1/lado.jpg", 1, 1));


	Texture* simplesB = new Texture("simplesB", "texturasPecas/simples.png", 1, 1);
	texturesPecas.push_back(simplesB);
	Texture* ataqueB = new Texture("ataqueB", "texturasPecas/ataque.png", 1, 1);
	texturesPecas.push_back(ataqueB);
	Texture* defesaB = new Texture("defesaB", "texturasPecas/defesa.png", 1, 1);
	texturesPecas.push_back(defesaB);
	Texture* expansaoB = new Texture("expansaoB", "texturasPecas/expansão.png", 1, 1);
	texturesPecas.push_back(expansaoB);
	Texture* saltoB = new Texture("saltoB", "texturasPecas/salto.png", 1, 1);
	texturesPecas.push_back(saltoB);
	Texture* simplesP = new Texture("simplesP", "texturasPecas/simplesP.png", 1, 1);
	texturesPecas.push_back(simplesP);
	Texture* ataqueP = new Texture("ataqueP", "texturasPecas/ataqueP.png", 1, 1);
	texturesPecas.push_back(ataqueP);
	Texture* defesaP = new Texture("defesaP", "texturasPecas/defesaP.png", 1, 1);
	texturesPecas.push_back(defesaP);
	Texture* expansaoP = new Texture("expansaoP", "texturasPecas/expansãoP.png", 1, 1);
	texturesPecas.push_back(expansaoP);
	Texture* saltoP = new Texture("saltoP", "texturasPecas/saltoP.png", 1, 1);
	texturesPecas.push_back(saltoP);
	Texture* tabuleiro0 = new Texture("tabuleiro0", "madeira0/tabuleiroDefault.jpg", 1, 1);
	texturesPecas.push_back(tabuleiro0);
	Texture* tabuleiro1 = new Texture("tabuleiro1", "madeira1/tabuleiroDefault.jpg", 1, 1);
	texturesPecas.push_back(tabuleiro1);

	this->appearanceTopos = new Appearance("topos");
	this->appearanceTopos->setTexture(texturesPecas[10]);


}

/*
	bool cor -> true: branca, false: preta
	string tipo->simples, ataque, defesa, expansao, salto
	bool selected ->true se a peça estiver selecionada pelo user
*/
void Piece::drawBooard(bool cor, string tipo, bool selected){
	draw(cor,tipo,selected);
}


void Piece::draw(bool cor, string tipo, bool selected){
/*
	if(tipo == "tabuleiro")
		this->appearanceLados->setTexture(this->ladoTabuleiro);
	else this->appearanceLados->setTexture(this->lado);*/

	this->appearanceTopos->setTexture(chooseTexture(cor, tipo)); //actualiza textura do topo, de acordo com a peça a desenhar
	glPushMatrix();
		glRotated(30, 0, 1, 0);
		
		glPushMatrix();
			this->appearanceTopos->apply();
			glTranslated(0,0.5,0);
			this->hexagon.draw();
		glPopMatrix();

		glPushMatrix();
			this->appearanceTopos->apply();
			glRotated(180,1,0,0);
			this->hexagon.draw();
		glPopMatrix();

		std::vector<Point3D> pontos = this->hexagon.getPoints();

		for (unsigned int i = 0; i < 6; i++){
			glPushMatrix();
				if (selected)
					this->appearanceLadosSelected[this->aparenciaActiva]->apply();
				else this->appearanceLados[this->aparenciaActiva]->apply();
				glRotated(60.0*i,0,1,0);
				glBegin(GL_QUADS);
					glTexCoord2d(1,1);
					glVertex3d(pontos[1].x, 0.5, pontos[1].z);
					glTexCoord2d(1,0);
					glVertex3d(pontos[1].x, 0, pontos[1].z);
					glTexCoord2d(0,0);
					glVertex3d(pontos[0].x, 0, pontos[0].z);
					glTexCoord2d(0,1);
					glVertex3d(pontos[0].x, 0.5, pontos[0].z);			
				glEnd();
			glPopMatrix();
		}
	glPopMatrix();

}


void Piece::drawAnimation(bool cor, string tipo, Animation* animation){
	animation->draw();
	this->draw(cor,tipo,false);
	glPopMatrix();

}

/*
	cor -> true ou false
	tipo -> "simples", "ataque", "defesa", "expansao" ou "salto"
*/
Texture* Piece::chooseTexture(bool cor, string tipo){
	int indexTexture = 0;
	if (tipo == "simples")
		indexTexture = 0;
	else if (tipo == "ataque")
		indexTexture = 1;
	else if (tipo == "defesa")
		indexTexture = 2;
	else if (tipo == "expansao")
		indexTexture = 3;
	else if (tipo == "salto")
		indexTexture = 4;
	else return texturesPecas[10+this->aparenciaActiva]; //default return (textura do tabuleiro)

	if (!cor)
		indexTexture += 5;

	return texturesPecas[indexTexture];
}


void Piece::changeTextures(int i){
	this->aparenciaActiva = i;	
}