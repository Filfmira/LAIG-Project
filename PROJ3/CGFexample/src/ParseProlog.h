#ifndef PARSE_PROLOG_H
#define PARSE_PROLOG_H
#include <vector>
#include <string>
#include "PieceData.h"

std::vector<vector<PieceData> > parseBoard(std::string board_string);
PieceData parsePiece(std::string piece);
std::string boardToString(std::vector<std::vector<PieceData> > board);
std::string jogadaToString(PieceData jogada, std::vector<vector<PieceData> > board);

std::string jogadaComputadorToString(std::vector<vector<PieceData> > board);

std::string pieceToString(PieceData piece);
bool parseAnswerJogada(std::string answer, vector<vector<PieceData> > &newBoard);
#endif