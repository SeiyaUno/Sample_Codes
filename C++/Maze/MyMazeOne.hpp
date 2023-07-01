#ifndef MYMAZEONE_HPP
#define MYMAZEONE_HPP

#include <vector>

#include "MazeGenerator.hpp"
#include "Direction.hpp"

class MyMazeOne : public MazeGenerator 
{
private:

public:

    //default constructor
    MyMazeOne();

    //other functions
    void generateMaze(Maze& maze) override;
    void recursivePath(int x, int y, std::vector<std::vector<bool>>& vct, Maze& m);
    void visitCheckAndAddDirection(const int x, const int y, const Direction& d
                    , std::vector<Direction>& v
                    , const std::vector<std::vector<bool>>& cv);
    Direction determineMove(const std::vector<Direction>& directionVct);
    void  possibleMoveFilter(const int x, const int y
                            , std::vector<std::vector<bool>>& vct
                            , std::vector<Direction>& d );
};

#endif