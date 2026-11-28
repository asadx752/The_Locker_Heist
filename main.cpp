#include "engine.h"

/*
 * Entry point for The Locker Heist.
 * Initializes the scoreboard and game engine singleton, then executes
 * the main game loop until the player chooses to exit.
 */
int main()
{
    ScoreBoard board;
    GameEngine& engine = GameEngine::getInstance();
    engine.attachScoreBoard(&board);

    // Main event loop: process game steps and handle runtime exceptions
    while (engine.getState() != Exit)
    {
        try
        {
            engine.runStep();
        }
        catch (GameException& e)
        {
            // Recoverable or fatal gameplay exception handled by engine
            engine.handleException(e);
        }
        catch (...)
        {
            cout << "Unexpected error encountered. Exiting game." << endl;
            break;
        }
    }

    return 0;
}
