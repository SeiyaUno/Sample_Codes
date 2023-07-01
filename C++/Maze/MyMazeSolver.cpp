#include <ics46/factory/DynamicFactory.hpp>
#include "MyMazeSolver.hpp"
#include "MyMazeOne.hpp"

#include <random>

ICS46_DYNAMIC_FACTORY_REGISTER(MazeSolver, MyMazeSolver, "My Maze Solver Algorithm (Required)");

//constructor
MyMazeSolver::MyMazeSolver() 
{

}

//other member functions
void MyMazeSolver::solveMaze(const Maze& maze, MazeSolution& mazeSolution) 
{

    mazeSolution.restart();
    //true = visited, false = not visited
    std::vector<std::vector<bool>> pathCheck(maze.getWidth()
                                                , std::vector<bool>(maze.getHeight(), false));
    std::pair<int, int> initialPos = mazeSolution.getStartingCell();

    recursivePathFinder(initialPos.first, initialPos.second, pathCheck, maze, mazeSolution);
}

bool MyMazeSolver::recursivePathFinder(int x, int y
                                , std::vector<std::vector<bool>>& vct
                                , const Maze& m
                                , MazeSolution& mSoln)
{
    Direction move;
    std::vector<Direction> directionVct;
    bool isNotAtGoal = true;

    //mark the cell as visited
    vct.at(x).at(y) = true;

    //If the program reaches the goal
    if(mSoln.getCurrentCell() == mSoln.getEndingCell())
    {
        isNotAtGoal = false;
    }

    while (isNotAtGoal)
    {
        directionVct.clear();
        FindPossibleMove(x, y, vct, directionVct, m);

        //if there is no possible move, leave the loop
        if(directionVct.empty()) {
            break;
        }
        else {
            move = decideMove(x, y, directionVct, mSoln);

            if(move == Direction::up) {
                mSoln.extend(move);
                isNotAtGoal = recursivePathFinder(x, y - 1, vct, m, mSoln);
            }
            else if(move == Direction::left) {
                mSoln.extend(move);
                isNotAtGoal = recursivePathFinder(x - 1, y, vct, m, mSoln);
            }
            else if(move == Direction::down) {
                mSoln.extend(move);
                isNotAtGoal = recursivePathFinder(x, y + 1, vct, m, mSoln);
            }
            else if(move == Direction::right) {
                mSoln.extend(move);
                isNotAtGoal = recursivePathFinder(x + 1, y, vct, m, mSoln);
            }
        }

    }

    if(isNotAtGoal) {

        //The recursion will go back to the starting position,
        //if there is no solution
        //The recursive function will go back to solveMaze function
        //before callig mSoln.backUp()
        if(x == mSoln.getStartingCell().first 
            && y == mSoln.getStartingCell().second)
        {
            return isNotAtGoal;
        }
        mSoln.backUp();
    }

    return isNotAtGoal;
}

void MyMazeSolver::FindPossibleMove(const int x, const int y
                                        , std::vector<std::vector<bool>>& v
                                        , std::vector<Direction>& direcVct
                                        , const Maze& m)
{
    //No wall means we can move to that direction
    if(!m.wallExists(x, y, Direction::up)) {
        //Make sure the cell in the direction is not visited yet
        if(!v.at(x).at(y - 1)) {
            direcVct.push_back(Direction::up);
        }
    }

    if(!m.wallExists(x, y, Direction::left)) {
        if(!v.at(x - 1).at(y)) {
            direcVct.push_back(Direction::left);
        }
    }

    if(!m.wallExists(x, y, Direction::down)) {
        if(!v.at(x).at(y + 1)) {
            direcVct.push_back(Direction::down);
        }
    }

    if(!m.wallExists(x, y, Direction::right)) {
        if(!v.at(x + 1).at(y)) {
            direcVct.push_back(Direction::right);
        }
    }
}

Direction MyMazeSolver::decideMove(const int x, const int y
                        , std::vector<Direction>& dVct
                        , MazeSolution& mSolution)
{
    std::pair<int, int> endCell = mSolution.getEndingCell();

    for(unsigned int i = 0; i < dVct.size(); i++) {
        if(dVct.at(i) == Direction::up) {
            if( pow(endCell.second - (y-1), 2)  < pow(endCell.second - y, 2))
            {
                //if moving up makes it closer to the endCell
                return Direction::up;
            }
        }
        else if(dVct.at(i) == Direction::down) {
            if( pow(endCell.second - (y+1), 2)  < pow(endCell.second - y, 2))
            {
                //if moving down makes it closer to the endCell
                return Direction::down;
            }
        }
        else if(dVct.at(i) == Direction::right) {
            if( pow(endCell.first - (x+1), 2)  < pow(endCell.first - x, 2))
            {
                //if moving right makes it closer to the endCell
                return Direction::right;
            }
        }
        else if(dVct.at(i) == Direction::left) {
            if( pow(endCell.first - (x-1), 2)  < pow(endCell.first - x, 2))
            {
                //if moving right makes it closer to the endCell
                return Direction::left;
            }
        }
    }

    //if any matching direction was not found above
    //call determineMove() from MyMazeOne class
    MyMazeOne forGettingDirection;
    return forGettingDirection.determineMove(dVct);
}