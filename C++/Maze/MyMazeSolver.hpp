#ifndef MYMAZESOLVER_HPP
#define MYMAZESOLVER_HPP

#include <vector>

#include "MazeSolver.hpp"
#include "MazeSolution.hpp"
#include "Direction.hpp"
#include "Maze.hpp"

class MyMazeSolver : public MazeSolver 
{
private:

public:
    //default constructor
    MyMazeSolver();

    //other functions
    void solveMaze(const Maze& maze, MazeSolution& mazeSolution) override;
    bool recursivePathFinder(int x, int y
                    , std::vector<std::vector<bool>>& vct
                    , const Maze& m
                    , MazeSolution& mSoln);
    void FindPossibleMove(const int x, const int y
                                    , std::vector<std::vector<bool>>& v
                                    , std::vector<Direction>& direcVct
                                    , const Maze& m);
    Direction decideMove(const int x, const int y
                        , std::vector<Direction>& dVct
                        , MazeSolution& mSolution);
};

#endif