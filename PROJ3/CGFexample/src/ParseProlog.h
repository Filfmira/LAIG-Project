#ifndef PARSE_PROLOG_H
#define PARSE_PROLOG_H
#include <vector>
#include <string>
#include "PieceData.h"

std::vector<vector<PieceData> > parseBoard(std::string board_string);
PieceData parsePiece(std::string piece);

#endif