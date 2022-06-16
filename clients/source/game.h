#pragma once
#include "vector"
#include "string"
#include <SFML/Graphics.hpp>
#include "windows.h"


enum Color {white, black, empty};

struct Coord{
    int x, y;
    Coord(int _x, int _y): x(_x), y(_y) {};
};

struct Figure{
    char type = 'c';
    char symbol;
    Color color;

    explicit Figure(char symb);

    Figure & operator= (Figure fig);

    void UpdFigure(){
        type = 'r';
        color == black ? symbol = 'x' : symbol = 'X';
    }
};

class GameBoard{
protected:
    std::vector<std::vector<Figure>> board;
    int downedWhite = 0;
    int downedBlack = 0;
public:

    unsigned int moveCounter = 0;
    Color move = white;

//    Coord lastDownedFigure = {-1, -1};
    std::string lastDownedFigure;
    char * buff;

    GameBoard();

    void View();
    bool Move(std::string& from, std::string& to);

    Figure & GetFigure(Coord coord);


    bool EndGame() const;
    bool MoveYourFigure(const std::string& from);
    bool ChkForUpd(const std::string &from, const std::string &to);
    bool CorrectMoveCommon(const std::string& from, const std::string& to);
    bool CorrectMoveRoyal(const std::string& from, const std::string& to);
    bool downFigCommon(const std::string& from, const std::string& to);
    bool downFigRoyal(const std::string& from, const std::string& to);
    char * packager ();

};