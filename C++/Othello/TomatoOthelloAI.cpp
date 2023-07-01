#include <ics46/factory/DynamicFactory.hpp>
#include "TomatoOthelloAI.hpp"

ICS46_DYNAMIC_FACTORY_REGISTER(OthelloAI, unos::TomatoOthelloAI, "Tomato Othello AI (Required)");

//This function is called when there is a valid move
std::pair<int, int> unos::TomatoOthelloAI::chooseMove(const OthelloGameState& state)
{
    int searchDepth = 4;    //max 4 for the sake of CPU time. Depth does not imply strength
    int maxScore = -1000;
    std::pair<int, int> bestMove;
    const OthelloBoard& board = state.board();
    const int height = board.height();
    const int width = board.width();
    std::unique_ptr<OthelloGameState> currentBoardPtr = state.clone();

    if(state.isBlackTurn()) {
        //check each cell
        for (int i = 0; i < height; i++) //y axis
        {
            for(int s = 0; s < width; s++) //x axis
            {
                if(currentBoardPtr->isValidMove(s, i)) {
                    //for each valid move, check the future move
                    std::unique_ptr<OthelloGameState> cBoardPtrDuplicate = state.clone();
                    cBoardPtrDuplicate->makeMove(s,i);
                    int temp = searchMoveBlack(cBoardPtrDuplicate, searchDepth, height, width);
                    
                    temp += additionalEvaluation(cBoardPtrDuplicate, s, i, width, height);
                    if(temp > maxScore) {
                        maxScore = temp;
                        bestMove = {s, i};
                    }
                }
            }
        }   
    }
    else {
        for (int i = 0; i < height; i++) //y axis
        {
            for(int s = 0; s < width; s++) //x axis
            {
                if(currentBoardPtr->isValidMove(s, i)) {
                    //for each valid move, check the future move
                    std::unique_ptr<OthelloGameState> cBoardPtrDuplicate = state.clone();
                    cBoardPtrDuplicate->makeMove(s,i);
                    int temp = searchMoveWhite(cBoardPtrDuplicate, searchDepth, height, width);

                    temp += additionalEvaluation(cBoardPtrDuplicate, s, i, width, height);
                    if(temp > maxScore) {
                        maxScore = temp;
                        bestMove = {s, i};
                    }
                }
            }
        }
    }
    
    return bestMove;
}

//search move if AI is playing as white
int unos::TomatoOthelloAI::searchMoveWhite(std::unique_ptr<OthelloGameState> & state, int depth, const int h, const int w)
{
    if(state->blackScore() + state->whiteScore() == w * h) {
        //if the board is completely filled, return
        return basicEvaluation(state);
    }

    if(depth == 0) {
        //return evaluation
        return basicEvaluation(state);
    }
    else {
        if(state->isWhiteTurn()) {
            //check each valid move
            //start from corner and scan all
            int highestWhite = -1000;
            for (int i = 0; i < h; i++) //y axis
            {
                for(int s = 0; s < w; s++) //x axis
                {
                    if(state->isValidMove(s, i)) {
                        std::unique_ptr<OthelloGameState> stateDash = state->clone();
                        stateDash->makeMove(s, i);
                        
                        int temp = searchMoveWhite(stateDash, depth - 1, h, w);    
                        temp += additionalEvaluation(stateDash, s, i, w, h);
                        //compare returned value and the current heighest
                        if(temp > highestWhite) {
                            highestWhite = temp;
                        }
                    }
                }
            }
            return highestWhite;
        }
        else {

            int lowestBlack = 1000;
            for (int i = 0; i < h; i++) //y axis
            {
                for(int s = 0; s < w; s++) //x axis
                {
                    if(state->isValidMove(s, i)) {
                        std::unique_ptr<OthelloGameState> stateDash = state->clone();
                        stateDash->makeMove(s, i);

                        int temp = searchMoveWhite(stateDash, depth - 1, h, w);
                        temp -= additionalEvaluation(stateDash, s, i, w, h);
                        //compare returned value and the current lowest
                        if(temp < lowestBlack) {
                            lowestBlack = temp;
                        }
                    }
                }
            }
            return lowestBlack;
        }
    }
}

//search move if AI is playing as black
int unos::TomatoOthelloAI::searchMoveBlack(std::unique_ptr<OthelloGameState> & state, int depth, const int h, const int w)
{
    if(state->blackScore() + state->whiteScore() == w * h) {
        //if the board is completely filled, return
        return basicEvaluation(state);
    }

    if(depth == 0) {
        return basicEvaluation(state);
    }
    else {
        if(state->isBlackTurn()) {
            //check each valid move
            //start from the top left corner and scan all
            int highestBlack = -1000;
            for (int i = 0; i < h; i++) //y axis
            {
                for(int s = 0; s < w; s++) //x axis
                {
                    if(state->isValidMove(s, i)) {
                        std::unique_ptr<OthelloGameState> stateDash = state->clone();
                        stateDash->makeMove(s, i);

                        int temp = searchMoveBlack(stateDash, depth - 1, h, w);
                        temp += additionalEvaluation(stateDash, s, i, w, h);
                        //compare returned value and the current heighest
                        if(temp > highestBlack) {
                            highestBlack = temp;
                        }
                    }
                }
            }
            return highestBlack;
        }
        else {

            int lowestWhite = 1000;
            for (int i = 0; i < h; i++) //y axis
            {
                for(int s = 0; s < w; s++) //x axis
                {
                    if(state->isValidMove(s, i)) {
                        std::unique_ptr<OthelloGameState> stateDash = state->clone();
                        stateDash->makeMove(s, i);

                        int temp = searchMoveBlack(stateDash, depth - 1, h, w);
                        temp -= additionalEvaluation(stateDash, s, i, w, h);
                        //compare returned value and the current lowest
                        if(temp < lowestWhite) {
                            lowestWhite = temp;
                        }
                    }
                }
            }
            return lowestWhite;
        }
    }
}

int unos::TomatoOthelloAI::basicEvaluation(std::unique_ptr<OthelloGameState> & state) {
    if(state->isBlackTurn()) {
        //this is the turn after white made the last move
        return state->whiteScore() - state->blackScore();
    }
    else {
        //this is the turn after black made the last move
        return state->blackScore() - state->whiteScore();
    }
}

int unos::TomatoOthelloAI::additionalEvaluation(std::unique_ptr<OthelloGameState> & state,
    const int x, const int y, const int w, const int h ) {
    
    int extraPoints = 0;
    int totalTurns = state->blackScore() + state->whiteScore() - 4;

    if((x == 0 && y == 0) || (x == w - 1 && y == 0) || 
        (x == 0 && y == h - 1) || (x == w - 1 && y == h - 1)) {
        //This section adds more points if the last move was at the corner
        extraPoints += 40;
    }
    else if(x == 0 || y == 0 || x == w - 1 || y == h - 1) {
        //edges 
        extraPoints += 20;
    }
    else if(x == 1 || x == w - 1 || y == 1 || y == h - 1) {
        //Avoid danger zone
        extraPoints -= 50;
    }
    else if(totalTurns < 16) {
        if(state->isBlackTurn()) {
            //This section evaluates for white player
            if(state->whiteScore() < state->blackScore()) {
                extraPoints += 10;
            }
        }
        else {
            //This section evaluates for black player
            if(state->whiteScore() > state->blackScore()) {
                extraPoints += 10;
            }
        }
    }
    return extraPoints;
}
