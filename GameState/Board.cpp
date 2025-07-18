#include "Board.h"

const Board::TypeChecker Board::movableObjects {Board::Type::BOX, Board::Type::BOARD};
const Board::TypeChecker Board::transparentObjects {Board::Type::GOAL, Board::Type::EMPTY};