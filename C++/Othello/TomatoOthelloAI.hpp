#include "OthelloAI.hpp"

namespace unos
{
    class TomatoOthelloAI : public OthelloAI
    {
    public:
        std::pair<int, int> chooseMove(const OthelloGameState& state) override;
    private:
        int searchMoveWhite(std::unique_ptr<OthelloGameState> & state, int depth, const int h, const int w);
        int searchMoveBlack(std::unique_ptr<OthelloGameState> & state, int depth, const int h, const int w);
        int basicEvaluation(std::unique_ptr<OthelloGameState> & state);
        int additionalEvaluation(std::unique_ptr<OthelloGameState> & state, const int x, const int y,
                            const int w, const int h);
    };
}