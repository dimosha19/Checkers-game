#include "game.h"

#include "iostream"

#define endl '\n'
#define bsize 8
#define figNum 11

using namespace sf;

Coord GetCoord(const std::string & pos){
    int y1 = pos[0] - 'a';
    int x1 = 8 - (pos[1] - '0');
    return {x1, y1};
}

Figure::Figure(char symb) {
    symbol = symb;
    if (symb == ' ') color = empty;
    else if (isupper(symb)) color = white;
    else color = black;
}

Figure & GameBoard::GetFigure(Coord coord) {
    return static_cast<Figure &>(board[coord.x][coord.y]);
}

GameBoard::GameBoard() {
    buff = new char[1024];

    board.resize(bsize);


    board[0] = {Figure(' '), Figure('o'), Figure(' '), Figure('o'), Figure(' '), Figure('o'), Figure(' '), Figure('o')};
    board[1] = {Figure('o'), Figure(' '), Figure('o'), Figure(' '), Figure('o'), Figure(' '), Figure('o'), Figure(' ')};
    board[2] = {Figure(' '), Figure('o'), Figure(' '), Figure('o'), Figure(' '), Figure('o'), Figure(' '), Figure('o')};
    board[3] = {Figure(' '), Figure(' '), Figure(' '), Figure(' '), Figure(' '), Figure(' '), Figure(' '), Figure(' ')};
    board[4] = {Figure(' '), Figure(' '), Figure(' '), Figure(' '), Figure(' '), Figure(' '), Figure(' '), Figure(' ')};
    board[5] = {Figure('O'), Figure(' '), Figure('O'), Figure(' '), Figure('O'), Figure(' '), Figure('O'), Figure(' ')};
    board[6] = {Figure(' '), Figure('O'), Figure(' '), Figure('O'), Figure(' '), Figure('O'), Figure(' '), Figure('O')};
    board[7] = {Figure('O'), Figure(' '), Figure('O'), Figure(' '), Figure('O'), Figure(' '), Figure('O'), Figure(' ')};

}

void GameBoard::View() {
    int k = 8;
    for (auto & i : board){
        std::cout << "  _________________________________" << endl;
        std::cout << k-- << " ";
        for (auto j : i){
            std::cout << "| " << j.symbol << " ";
        }
        std::cout << "|" << endl;
    }
    std::cout << "  _________________________________" << endl;
    std::cout << "    a   b   c   d   e   f   g   h   " << endl;
}

bool GameBoard::ChkForUpd(const std::string &from, const std::string &to){
    auto coordsFrom = GetCoord(from);
    if (to[1] == '1' && GetFigure(coordsFrom).color == black) return true;
    if (to[1] == '8' && GetFigure(coordsFrom).color == white) return true;
    return false;
}

bool GameBoard::Move(std::string& from, std::string& to) {
    auto coordsFrom = GetCoord(from);
    auto coordsTo = GetCoord(to);
    if (GetFigure(coordsFrom).type == 'c') {
        if (CorrectMoveCommon(from, to) && MoveYourFigure(from)) {
            if (ChkForUpd(from, to)) GetFigure(coordsFrom).UpdFigure();
            board[coordsTo.x][coordsTo.y] = board[coordsFrom.x][coordsFrom.y];
            board[coordsFrom.x][coordsFrom.y] = Figure(' ');
            move == white ? move = black : move = white;
            moveCounter++;
        } else return false;
    } else {
        if (CorrectMoveRoyal(from, to) && MoveYourFigure(from)) {
            board[coordsTo.x][coordsTo.y] = board[coordsFrom.x][coordsFrom.y];
            board[coordsFrom.x][coordsFrom.y] = Figure(' ');
            move == white ? move = black : move = white;
            moveCounter++;
        } else return false;
    }
    return true;
}

bool GameBoard::EndGame() const {
    if (downedWhite == figNum || downedBlack == figNum)
        return true;
    return false;
}

Figure & Figure::operator=(Figure fig) {
    symbol = fig.symbol;
    color = fig.color;
    type = fig.type;
    return *this;
}

bool GameBoard::MoveYourFigure(const std::string& from) {
    if (move == GetFigure(GetCoord(from)).color) return true;
    return false;
}



bool GameBoard::CorrectMoveCommon(const std::string &from, const std::string &to) {
    auto coordTo = GetCoord(to);
    auto coordFrom = GetCoord(from);
    auto downed = downFigCommon(from, to);
    if (move == white) {
        if (coordTo.x >= 0 && coordTo.x < 8 &&
            coordTo.y >= 0 && coordTo.y < 8) {
            if ((coordFrom.x == coordTo.x + 2 && downed) ||
                (coordFrom.x == coordTo.x + 1 && !downed)) {
                if ((abs(coordTo.y - coordFrom.y) == 2 && downed) ||
                    abs(coordTo.y - coordFrom.y) == 1 && !downed) {
                    if (GetFigure(coordTo).symbol == ' ' && GetFigure(coordFrom).symbol != ' ') {
                        return true;
                    }
                }
            }
        }
    } else {
        if (coordTo.x >= 0 && coordTo.x < 8 &&
            coordTo.y >= 0 && coordTo.y < 8) {
            if ((coordFrom.x == coordTo.x - 2 && downed) ||
                (coordFrom.x == coordTo.x - 1 && !downed)) {
                if ((abs(coordTo.y - coordFrom.y) == 2 && downed) ||
                    abs(coordTo.y - coordFrom.y) == 1 && !downed) {
                    if (GetFigure(coordTo).symbol == ' ' && GetFigure(coordFrom).symbol != ' ') {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

Coord GetBetweenFigure(const std::string &from, const std::string &to){
    auto coordTo = GetCoord(to);
    auto coordFrom = GetCoord(from);
    auto diffx = (coordFrom.x - coordTo.x) / 2;
    auto diffy = (coordFrom.y - coordTo.y) / 2;
    Coord res = {0, 0};
    if (diffx < 0){
        res.x = std::min(coordFrom.x, coordTo.x) - diffx;
    } else{
        res.x = std::max(coordFrom.x, coordTo.x) - diffx;
    }
    if (diffy < 0){
        res.y = std::min(coordFrom.y, coordTo.y) - diffy;
    } else {
        res.y = std::max(coordFrom.y, coordTo.y) - diffy;
    }
    return res;
}

bool GameBoard::downFigCommon(const std::string &from, const std::string &to) {
    auto coordTo = GetCoord(to);
    auto coordFrom = GetCoord(from);
    std::string pos;
    if (abs(coordTo.y - coordFrom.y) == 2){
        if (coordTo.y - coordFrom.y == 2) {
            auto fig = Figure(' ');
            move == white ? fig = GetFigure({coordFrom.x - 1, coordFrom.y + 1}) : fig = GetFigure({coordFrom.x + 1, coordFrom.y + 1});
            if (GetFigure(coordFrom).color != fig.color && fig.color != empty) {
                move == white ? downedBlack++ : downedWhite++;
                auto coordBetween = GetBetweenFigure(from, to);
                GetFigure(coordBetween) = Figure(' ');
                pos += (coordBetween.y + 'a');
                pos += (8 - coordBetween.x) + '0';
                lastDownedFigure = pos;
                return true;
            }
        } else {
            auto fig = Figure(' ');
            move == white ? fig = GetFigure({coordFrom.x - 1, coordFrom.y - 1}) : fig = GetFigure({coordFrom.x + 1, coordFrom.y - 1});
            if (GetFigure(coordFrom).color != fig.color && fig.color != empty) {
                move == white ? downedBlack++ : downedWhite++;
                auto coordBetween = GetBetweenFigure(from, to);
                GetFigure(coordBetween) = Figure(' ');
                pos += (coordBetween.y + 'a');
                pos += (8 - coordBetween.x) + '0';
                lastDownedFigure = pos;
                return true;
            }
        }
    }
    return false;
}

bool GameBoard::downFigRoyal(const std::string &from, const std::string &to) {
    auto coordTo = GetCoord(to);
    auto coordFrom = GetCoord(from);
    auto diffx = coordTo.x - coordFrom.x;
    auto diffy = coordTo.y - coordFrom.y;
    if (diffx > 0){
        for (int i = 1; i < diffx; i++){
            if (GetFigure({coordFrom.x + i, coordFrom.y}).symbol != ' '){
                if (GetFigure({coordFrom.x + i, coordFrom.y}).symbol != GetFigure({coordFrom.x, coordFrom.y}).symbol) {
                    GetFigure({coordFrom.x + i, coordFrom.y}) = Figure(' ');
                    std::string pos;
                    pos += (coordFrom.y + 'a');
                    pos += (8 - coordFrom.x - i) + '0';
                    lastDownedFigure = pos;
                    move == white ? downedBlack++ : downedWhite++;
                    return true;
                } else return false;
            }
        }
    } else if (diffx < 0){
        for (int i = 1; i < -diffx; i++){
            if (GetFigure({coordFrom.x - i, coordFrom.y}).symbol != ' '){
                if (GetFigure({coordFrom.x - i, coordFrom.y}).symbol != GetFigure({coordFrom.x, coordFrom.y}).symbol) {
                    GetFigure({coordFrom.x - i, coordFrom.y}) = Figure(' ');
                    std::string pos;
                    pos += (coordFrom.y + 'a');
                    pos += (8 - coordFrom.x + i) + '0';
                    lastDownedFigure = pos;
                    move == white ? downedBlack++ : downedWhite++;
                    return true;
                } else return false;
            }
        }
    } else if (diffy > 0){
        for (int i = 1; i < diffy; i++){
            if (GetFigure({coordFrom.x, coordFrom.y + i}).symbol != ' '){
                if (GetFigure({coordFrom.x, coordFrom.y + i}).symbol != GetFigure({coordFrom.x, coordFrom.y}).symbol) {
                    GetFigure({coordFrom.x, coordFrom.y + i}) = Figure(' ');
                    std::string pos;
                    pos += ((coordFrom.y + i) + 'a');
                    pos += (8 - coordFrom.x) + '0';
                    lastDownedFigure = pos;
                    move == white ? downedBlack++ : downedWhite++;
                    return true;
                } else return false;
            }
        }
    } else {
        for (int i = 1; i < -diffy; i++){
            if (GetFigure({coordFrom.x, coordFrom.y - i}).symbol != ' '){
                if (GetFigure({coordFrom.x, coordFrom.y - i}).symbol != GetFigure({coordFrom.x, coordFrom.y}).symbol) {
                    GetFigure({coordFrom.x, coordFrom.y - i}) = Figure(' ');
                    std::string pos;
                    pos += ((coordFrom.y - i) + 'a');
                    pos += (8 - coordFrom.x) + '0';
                    lastDownedFigure = pos;
                    move == white ? downedBlack++ : downedWhite++;
                    return true;
                } else return false;
            }
        }
    }
    return false;
}

bool GameBoard::CorrectMoveRoyal(const std::string &from, const std::string &to) {
    auto coordTo = GetCoord(to);
    auto coordFrom = GetCoord(from);
    auto diffx = coordTo.x - coordFrom.x;
    auto diffy = coordTo.y - coordFrom.y;
    if (coordTo.x < 0 || coordTo.x >= 8 || coordTo.y < 0 || coordTo.y >= 8) return false;
    if (GetFigure(coordTo).symbol != ' ') return false;
    if (diffx != 0 && diffy != 0) return false;
    downFigRoyal(from, to);
    return true;
}

char * GameBoard::packager() {
    int it = 0;
    for (int i = 0; i < bsize; i++){
        for (int j = 0; j < bsize; j++){
            buff[it] = board[i][j].symbol;
            it++;
        }
    }
    buff[it] = '\0';
    return buff;
}
