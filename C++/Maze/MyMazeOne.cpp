#include <ics46/factory/DynamicFactory.hpp>
#include "MyMazeOne.hpp"
#include "Maze.hpp"
#include "MazeVerifier.hpp"

#include <random>

ICS46_DYNAMIC_FACTORY_REGISTER(MazeGenerator, MyMazeOne, "My Maze Generator Algorithm (Required)");

//constructor
MyMazeOne::MyMazeOne() 
{

}

//other member functions
void MyMazeOne::generateMaze(Maze& maze) 
{
    
    maze.addAllWalls();

    int xInit = 0, yInit = 0;   //Initial position. The top left corner is (0,0)

    //true = visited, false = not visited
    std::vector<std::vector<bool>> cellInfo(maze.getWidth()
                                            , std::vector<bool>(maze.getHeight(), false));

    //start generating maze
    recursivePath(xInit, yInit, cellInfo, maze);
}

void MyMazeOne::recursivePath(int x, int y, 
                                std::vector<std::vector<bool>>& vct,
                                Maze& m) 
{
    Direction nextMove;
    std::vector<Direction> directionVct;        //This vector will store possible moves

    //matk the cell as visited
    vct.at(x).at(y) = true;

    while(true)
    {
        directionVct.clear();
        possibleMoveFilter(x, y, vct, directionVct);
        
        //if there is no possible move, leave the loop
        if(directionVct.size() == 0) {
            break;
        }
        else {
            nextMove = determineMove(directionVct);

            if(nextMove == Direction::up) {
                m.removeWall(x, y, Direction::up);
                recursivePath(x, y - 1, vct, m);
            }
            else if (nextMove == Direction::down) {
                m.removeWall(x, y, Direction::down);
                recursivePath(x, y + 1, vct, m);
            }   
            else if (nextMove == Direction::left) {
                m.removeWall(x, y, Direction::left);
                recursivePath(x - 1, y, vct, m);
            }
            else if (nextMove == Direction::right) {
                m.removeWall(x, y, Direction::right);
                recursivePath(x + 1, y, vct, m);
            }
        }
    }

    return;     //this should go back to previous function
}

//This function is designed to be used only in possibleMoveFilter function
//Ohterwise, this may cause out of bounds exception
void MyMazeOne::visitCheckAndAddDirection(const int x, const int y, const Direction& d
                            , std::vector<Direction>& v
                            , const std::vector<std::vector<bool>>& cv)
{
    //check if the cell in the correponding direction is visted or not
    if(d == Direction::up
        && !cv.at(x).at(y - 1)) 
    {
        v.push_back(Direction::up);
    }
    else if(d == Direction::left
        && !cv.at(x - 1).at(y)) 
    {
        v.push_back(Direction::left);
    }
    else if(d == Direction::down
        && !cv.at(x).at(y + 1)) 
    {
        v.push_back(Direction::down);
    }
    else if(d == Direction::right
        && !cv.at(x + 1).at(y)) 
    {
        v.push_back(Direction::right);
    }
}

//the size of directionVct is expected to be maximum of 4
Direction MyMazeOne::determineMove(const std::vector<Direction>& directionVct)
{
    //set up random
    std::random_device device;
    std::default_random_engine engine(device());

    //set maximum random value as the size of directionVct
    const int vMax = directionVct.size() - 1;
    std::uniform_int_distribution<int> distribution {0, vMax};
    int rand = distribution(engine);
    
    if(rand == 0) {
        return directionVct.at(0);
    }
    else if(rand == 1) {
        return directionVct.at(1);
    }
    else if(rand == 2) {
        return directionVct.at(2);
    }
    else {
        return directionVct.at(3);
    }
} 

//This function modifies direc vector depending on x and y
void MyMazeOne::possibleMoveFilter(const int x, const int y
                                , std::vector<std::vector<bool>>& vct
                                , std::vector<Direction>& direc )
{
    const int xMax = vct.size() - 1;
    const int yMax = vct.at(x).size() - 1;
    
    if(x == 0 && y == 0) {
        //top left corner
        visitCheckAndAddDirection(0, 0, Direction::down, direc, vct);
        visitCheckAndAddDirection(0, 0, Direction::right, direc, vct);
    }
    else if(x == xMax && y == 0) {
        //top right corner
        visitCheckAndAddDirection(xMax, 0, Direction::left, direc, vct);
        visitCheckAndAddDirection(xMax, 0, Direction::down, direc, vct);
    }
    else if(x == 0 && y == yMax) {
        //bottom left corner
        visitCheckAndAddDirection(0, yMax, Direction::right, direc, vct);
        visitCheckAndAddDirection(0, yMax, Direction::up, direc, vct);
    }
    else if(x == xMax && y == yMax) {
        //bottom right corner
        visitCheckAndAddDirection(xMax, yMax, Direction::left, direc, vct);
        visitCheckAndAddDirection(xMax, yMax, Direction::up, direc, vct);
    }
    else if(y == 0) {
        //top cells except corners
        visitCheckAndAddDirection(x, y, Direction::right, direc, vct);
        visitCheckAndAddDirection(x, y, Direction::left, direc, vct);
        visitCheckAndAddDirection(x, y, Direction::down, direc, vct);
    }
    else if(y == yMax) {
        //bottom cells except corners
        visitCheckAndAddDirection(x, y, Direction::right, direc, vct);
        visitCheckAndAddDirection(x, y, Direction::left, direc, vct);
        visitCheckAndAddDirection(x, y, Direction::up, direc, vct);
    }
    else if(x == 0) {
        //leftmost cells except corners
        visitCheckAndAddDirection(x, y, Direction::right, direc, vct);
        visitCheckAndAddDirection(x, y, Direction::up, direc, vct);
        visitCheckAndAddDirection(x, y, Direction::down, direc, vct);
    }
    else if(x == xMax) {
        //rightmost cells except corners
        visitCheckAndAddDirection(x, y, Direction::up, direc, vct);
        visitCheckAndAddDirection(x, y, Direction::left, direc, vct);
        visitCheckAndAddDirection(x, y, Direction::down, direc, vct);
    }
    else {
        visitCheckAndAddDirection(x, y, Direction::up, direc, vct);
        visitCheckAndAddDirection(x, y, Direction::right, direc, vct);
        visitCheckAndAddDirection(x, y, Direction::left, direc, vct);
        visitCheckAndAddDirection(x, y, Direction::down, direc, vct);
    }
}